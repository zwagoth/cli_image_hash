#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "definitions.h"

#define MIN(x,y) ((x) < (y) ? (x) : (y))
#define MAX(x,y) ((x) > (y) ? (x) : (y))
#define CLAMP(min,val,max) MIN(MAX(min, val), max)

// Lanczos window parameter
#define a 3

static float lanczos_kernel(float input)
{
    if (fabsf(input) < 0x1.0p-16) return 1;
    if (fabsf(input) > a) return 0;

    return (a * sinf(M_PI * input) * sinf(M_PI * input / a)) / ((M_PI * M_PI) * (input * input));
}

static rgb_pixel lanczos_interp32h(const rgb_pixel *restrict src, uint32_t width, float point)
{
    float r = 0;
    float g = 0;
    float b = 0;

    for (int64_t i = point - a + 1; i < point + a; ++i) {
        float kernel_val = lanczos_kernel(point - i);
        rgb_pixel sample = src[CLAMP(0, i, width - 1)];

        r += sample.r * kernel_val;
        g += sample.g * kernel_val;
        b += sample.b * kernel_val;
    }

    return (rgb_pixel) {
        .r = r,
        .g = g,
        .b = b
    };
}

static rgb_pixel lanczos_interp32v(const rgb_pixel *restrict src, uint32_t height, float point)
{
    float r = 0;
    float g = 0;
    float b = 0;

    for (int64_t i = point - a + 1; i < point + a; ++i) {
        float kernel_val = lanczos_kernel(point - i);
        rgb_pixel sample = src[32 * CLAMP(0, i, height - 1)];

        r += sample.r * kernel_val;
        g += sample.g * kernel_val;
        b += sample.b * kernel_val;
    }

    return (rgb_pixel) {
        .r = r,
        .g = g,
        .b = b
    };
}

static void lanczos_scale32h(rgb_pixel *restrict dst, const rgb_pixel *restrict src, uint32_t width, uint32_t height)
{
    for (uint32_t i = 0; i < height; ++i) {
        for (uint32_t j = 0; j < 32; ++j) {
            dst[32 * i + j] = lanczos_interp32h(src + width * i, width, (j / 32.0) * width);
        }
    }
}

static void lanczos_scale32v(rgb_pixel *restrict dst, const rgb_pixel *restrict src, uint32_t height)
{
    for (uint32_t j = 0; j < 32; ++j) {
        for (uint32_t i = 0; i < 32; ++i) {
            dst[32 * i + j] = lanczos_interp32v(src + j, height, (i / 32.0) * height);
        }
    }
}

static rgb_pixel *scale_32x32(const rgb_pixel *restrict src, uint32_t width, uint32_t height)
{
    rgb_pixel *s1 = calloc(32 * height, sizeof(rgb_pixel));
    rgb_pixel *s2 = calloc(32 * 32, sizeof(rgb_pixel));

    lanczos_scale32h(s1, src, width, height);
    lanczos_scale32v(s2, s1, height);

    free(s1);
    return s2;
}

static void compute_intensity(float intensity[static 1024], const rgb_pixel *restrict src)
{
    for (size_t i = 0; i < 1024; ++i) {
        intensity[i] = ((src[i].r * sRGB_R_Y) +
                        (src[i].g * sRGB_G_Y) + 
                        (src[i].b * sRGB_B_Y));
    }
}

static void compute_components(float components[static 64], const float dct[static 1024])
{
    // Top left corner, excluding DC offset
    for (uint32_t i = 0; i < 8; ++i) {
        for (uint32_t j = 0; j < 8; ++j) {
            if (i == 0 && j == 0) continue;

            components[i * 8 + j - 1] = dct[i * 32 + j];
        }
    }

    // Bottom outlier (to fill top gap)
    components[7*8 + 7] = dct[8*32 + 8];
}

static float component_median(const float components[static 64])
{
    float input[64];

    memcpy(input, components, sizeof(float) * 64);

    // Tired: quickselect
    // Wired: insertion sort
    for (size_t i = 0; i < 64; ++i) {
        for (size_t j = 0; j < 63; ++j) {
            if (input[j] > input[j + 1]) {
                float tmp = input[j + 1];

                input[j + 1] = input[j];
                input[j] = tmp;
            }
        }
    }

    return (input[31] + input[32]) / 2;
}

uint64_t phash(const rgb_pixel *restrict pixels, uint32_t width, uint32_t height)
{
    rgb_pixel *scaled = scale_32x32(pixels, width, height);

    float intensity[1024] = {};
    float dct[1024]       = {};
    float components[64]  = {};

    compute_intensity(intensity, scaled);
    free(scaled);

    fdct(dct, intensity);
    compute_components(components, dct);

    float median = component_median(components);
    uint64_t phash = 0;

    for (size_t i = 0; i < 64; ++i, phash <<= 1) {
        phash |= (components[i] > median);
    }

    return phash;
}
