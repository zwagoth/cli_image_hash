#include "definitions.h"
#include <magic.h>
#include <stdio.h>
#include <string.h>
#include <vips/vips.h>

static const char *mime_type(const char *filename)
{
    const char *ret;
    magic_t magic = magic_open(MAGIC_MIME_TYPE);
    magic_load(magic, NULL);

    ret = magic_file(magic, filename);

    return ret;
}

static int print_data(intensity_data data)
{
    if (data.error) {
        fprintf(stderr, "Decoder library returned an error\n");
        return 1;
    }

    printf("%f\t%f\t%f\t%f\t%lu\n", data.nw, data.ne, data.sw, data.se, data.phash);

    return 0;
}

int main(int argc, char *argv[])
{
    const char *file_mime;
    intensity_data data;

    if (argc != 2) {
        fprintf(stderr, "Expected to be called with 2 arguments\n");
        return 1;
    }

    if( VIPS_INIT( argv[0] ) )
        vips_error_exit( NULL );

    file_mime = mime_type(argv[1]);

    if (strcmp(file_mime, "image/png") == 0) {
        data = png_intensities(argv[1]);
    } else if (strcmp(file_mime, "image/jpeg") == 0) {
        data = jpeg_intensities(argv[1]);
    } else {
        fprintf(stderr, "Unrecognized file type `%s'\n", file_mime);
        return 1;
    }

    return print_data(data);
}