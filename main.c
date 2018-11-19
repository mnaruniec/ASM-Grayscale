#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// Set color weights. Arguments should sum to 256 for best results.
extern void set_rgb(unsigned char red, unsigned char green, unsigned char blue);

extern void to_grayscale(unsigned char *dest, unsigned char *src,
                         unsigned width, unsigned height);


int main(int argc, char **argv) {
    FILE *input;
    FILE *output;
    unsigned width = 0, height = 0, depth = 0;
    size_t length = 0;
    unsigned tmp = 0;
    unsigned char *src = NULL;
    unsigned char *dest = NULL;
    int ret = 1;

    if(argc < 3) {
        printf("usage: %s input_file output_file\n", argc > 0 ? argv[0] : "to_grayscale");
        return 1;
    }

    if(!(input = fopen(argv[1], "r"))) {
        perror("fopen(input)");
        return 1;
    }

    errno = 0;
    fscanf(input, "P3 %u %u %u", &width, &height, &depth);
    if (depth != 255) {
        printf("incorrect input header");
        goto close_input;
    }

    length = ((size_t)width) * (size_t)height;

    if(!(src = malloc(3 * length))) {
        perror("malloc(src)");
        goto close_input;
    }

    for (size_t i = 0; i < 3 * length; i++) {
        tmp = 0;
        if (EOF == fscanf(input, "%u", &tmp) || tmp > 255) {
            printf("pixel read error\n");
            goto free_src;
        }
        src[i] = (unsigned char)tmp;
    }

    if(!(dest = malloc(length))) {
        perror("malloc(dest)");
        goto free_src;
    }

    to_grayscale(dest, src, width, height);

    if(!(output = fopen(argv[2], "w"))) {
        perror("fopen(output)");
        goto free_dest;
    }

    fprintf(output, "P2\n%u %u\n255\n", width, height);

    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            fprintf(output, "%u ", (unsigned) (dest[i * width + j]));
        }
        if (0 >= fprintf(output, "\n")) {
            printf("fprintf error");
            goto close_output;
        }
    }

    ret = 0;

    close_output:
        pclose(output);
    free_dest:
        free(dest);
    free_src:
        free(src);
    close_input:
        pclose(input);
    return ret;
}
