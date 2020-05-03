#include <stdlib.h>
#include <math.h>

// debug
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "definitions.h"

#define MIN(x,y) ((x) < (y) ? (x) : (y))
#define MAX(x,y) ((x) > (y) ? (x) : (y))
#define CLAMP(min,val,max) MIN(MAX(min, val), max)

const int a = 2;

static void dump(const rgb_pixel *restrict src)
{
    int fd = open("scale.rgb", O_RDWR | O_CREAT | O_TRUNC);
    write(fd, src, 32 * 32 * sizeof(rgb_pixel));
    close(fd);
}

static float lanczos_kernel(float input)
{
    if (fabsf(input) < 0x1.0p-16) return 1;
    if (fabsf(input) > a) return 0;

    return (a * sinf(M_PI * input) * sinf(M_PI * input / a)) / ((M_PI * M_PI) * (input * input));
}

static rgb_pixel lanczos_interp32h(const rgb_pixel *restrict src, uint32_t width, double point)
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

static rgb_pixel lanczos_interp32v(const rgb_pixel *restrict src, uint32_t height, double point)
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
            dst[32 * i + j] = lanczos_interp32h(src, width, (j / 32.0) * width);
        }
    }
}

static void lanczos_scale32v(rgb_pixel *restrict dst, const rgb_pixel *restrict src, uint32_t height)
{
    for (uint32_t j = 0; j < 32; ++j) {
        for (uint32_t i = 0; i < 32; ++i) {
            dst[32 * i + j] = lanczos_interp32v(src, height, (i / 32.0) * height);
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
    dump(s2);

    return s2;
}

uint64_t phash(rgb_pixel *restrict pixels, uint32_t width, uint32_t height)
{
    free(scale_32x32(pixels, width, height));

    return 0LL;
}
