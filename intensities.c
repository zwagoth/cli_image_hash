#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <fcntl.h>
#include <string.h>
#include <jpeglib.h>

#include "definitions.h"

quadrant_sums rgb_sums(const rgb_pixel *restrict pixels, uint32_t width, uint32_t height)
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

intensity_data rgb_to_luma(quadrant_sums sums, raster_data data)
{
    double dim = ((data.width + 1) / 2) * ((data.height + 1) / 2);

    double nw_luma = ((sums.nw.r / dim * sRGB_R_Y) +
                      (sums.nw.g / dim * sRGB_G_Y) +
                      (sums.nw.b / dim * sRGB_B_Y));

    double ne_luma = ((sums.ne.r / dim * sRGB_R_Y) +
                      (sums.ne.g / dim * sRGB_G_Y) +
                      (sums.ne.b / dim * sRGB_B_Y));

    double sw_luma = ((sums.sw.r / dim * sRGB_R_Y) +
                      (sums.sw.g / dim * sRGB_G_Y) +
                      (sums.sw.b / dim * sRGB_B_Y));

    double se_luma = ((sums.se.r / dim * sRGB_R_Y) +
                      (sums.se.g / dim * sRGB_G_Y) +
                      (sums.se.b / dim * sRGB_B_Y));

    return (struct intensity_data) {
        .nw = nw_luma,
        .ne = ne_luma,
        .sw = sw_luma,
        .se = se_luma,
        .error = 0
    };
}
