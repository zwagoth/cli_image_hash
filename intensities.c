#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <fcntl.h>
#include <string.h>
#include <jpeglib.h>
#include <vips/vips.h>

#include "definitions.h"

void fdct(float *dst, const float *src);

quadrant_sums rgb_sums(rgb_pixel *restrict pixels, uint32_t width, uint32_t height)
{
    quadrant_sums sums = {{}};

    for (uint32_t i = 0; i < height; ++i) {
        for (uint32_t j = 0; j < width; ++j) {
            int nw = (i <= height / 2) && (j <= width / 2);
            int ne = (i <= height / 2) && (j >= width / 2);
            int sw = (i >= height / 2) && (j <= width / 2);
            int se = (i >= height / 2) && (j >= width / 2);

            rgb_pixel pix = pixels[i * width + j];

            if (nw) {
                sums.nw.r += pix.r;
                sums.nw.g += pix.g;
                sums.nw.b += pix.b;
            }

            if (ne) {
                sums.ne.r += pix.r;
                sums.ne.g += pix.g;
                sums.ne.b += pix.b;
            }

            if (sw) {
                sums.sw.r += pix.r;
                sums.sw.g += pix.g;
                sums.sw.b += pix.b;
            }

            if (se) {
                sums.se.r += pix.r;
                sums.se.g += pix.g;
                sums.se.b += pix.b;
            }
        }
    }

    return sums;
}

static intensity_data rgb_to_luma(quadrant_sums sums, raster_data data)
{
    double dim = MAX(data.width * data.height / 4.0, 1);

    double nw_luma = ((sums.nw.r / dim * 0.2126) +
                      (sums.nw.g / dim * 0.7152) +
                      (sums.nw.b / dim * 0.0772)) / 3.0;

    double ne_luma = ((sums.ne.r / dim * 0.2126) +
                      (sums.ne.g / dim * 0.7152) +
                      (sums.ne.b / dim * 0.0772)) / 3.0;

    double sw_luma = ((sums.sw.r / dim * 0.2126) +
                      (sums.sw.g / dim * 0.7152) +
                      (sums.sw.b / dim * 0.0772)) / 3.0;

    double se_luma = ((sums.se.r / dim * 0.2126) +
                      (sums.se.g / dim * 0.7152) +
                      (sums.se.b / dim * 0.0772)) / 3.0;

    return (struct intensity_data) {
        .nw = nw_luma,
        .ne = ne_luma,
        .sw = sw_luma,
        .se = se_luma,
        .error = 0
    };
}

static void dct(double* pixels, float* out) {
    float conv[32*32];
    for(int i = 0; i < 32*32; i++) {
        conv[i] = (float)pixels[i];
    }
    fdct(out, conv);
}

static float array_median(float src[64]) {
    float in[64];
    memcpy(in, src, sizeof(float)*64);
    for(int i = 0; i < 64; i++) {
        for(int j = 0; j < 63; j++) {
            if(in[j]>in[j+1]) {
                float tmp = in[j+1];
                in[j+1] = in[j];
                in[j] = tmp;
            }
        }
    }
    return (in[32] + in[31]) / 2.0;
}

static void fill_phash(const raster_data *data, intensity_data* intensities) {
    VipsImage* vbw = vips_image_new_matrix(data->width, data->height);
    VipsImage* vout;
    for(int y = 0; y < data->height; y++) {
        for(int x = 0; x < data->width; x++) {
            int offset = y*data->width + x;
            *VIPS_MATRIX(vbw, x, y) = (data->pixels[offset].r * 0.2126) + (data->pixels[offset].g * 0.7152) +
                                     (data->pixels[offset].b * 0.0772);
        }
    }
    vips_image_write_to_file(vbw, "bw.png", NULL);
    double hscale = (double)data->width / 32.0;
    double vscale = (double)data->height / 32.0;
    vips_reduce(vbw, &vout, hscale, vscale, NULL);
    vips_image_write_to_file(vout, "out.png", NULL);

    VipsRegion* region = vips_region_new(vout);
    VipsRect r = { left: 0, top: 0, width: 32, height: 32 };
    if(vips_region_prepare(region, &r)) {
        vips_error_exit( NULL );
    }
    VipsPel* pixels = VIPS_REGION_ADDR(region, 0, 0);
    float dctout[32*32];
    dct((double*)pixels, &dctout);
    float dctcrop[64];
    for(int x = 0; x < 8; x++) {
        for(int y = 0; y < 8; y++) {
            dctcrop[y*8+x] = dctout[y*32+x];
        }
    }

    float median = array_median(dctcrop);
    uint64_t hash = 0;
    for(int i = 0; i < 64; i++, hash <<= 1) {
        if(dctcrop[i] > median)
            hash |= 0x01;
    }
    intensities->phash = hash;
    g_object_unref(vbw);
    g_object_unref(region);
    g_object_unref(vout);
}

intensity_data jpeg_intensities(const char *file_name)
{
    raster_data data = read_jpeg_file(file_name);
    if (data.error)
        return (struct intensity_data) { .error = 1 };

    quadrant_sums sums = rgb_sums(data.pixels, data.width, data.height);
    intensity_data ins = rgb_to_luma(sums, data);

    fill_phash(&data, &ins);

    free(data.pixels);

    return ins;
}

intensity_data png_intensities(const char *file_name)
{
    raster_data data = read_png_file(file_name);
    if (data.error)
        return (struct intensity_data) { .error = 1 };

    quadrant_sums sums = rgb_sums(data.pixels, data.width, data.height);
    intensity_data ins = rgb_to_luma(sums, data);

    fill_phash(&data, &ins);

    free(data.pixels);

    return ins;
}
