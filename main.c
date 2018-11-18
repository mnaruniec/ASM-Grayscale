#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

extern void to_grayscale(unsigned char *dest, unsigned char *src,
                         unsigned width, unsigned height);

void to_grayscale(unsigned char *dest, unsigned char *src,
                  unsigned width, unsigned height) {
    size_t length = ((size_t)width) * (size_t)height;
    for(size_t i = 0; i < length; i++) {
        unsigned r = src[3 * i] * 77;
        unsigned g = src[3 * i + 1] * 151;
        unsigned b = src[3 * i + 2] * 28;
        unsigned rgb = (r + g + b) / 256;
        dest[i] = (unsigned char)rgb;
    }
}

int main(int argc, char **argv) {
    FILE *input;
    FILE *output;
    unsigned width = 0, height = 0;
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
    fscanf(input, "P3 %u %u 255", &width, &height);
    if(!height) {
        printf("expected non-zero image height");
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
            fprintf(output, "%u ", (unsigned)(dest[i * width + j]));
        }
        fprintf(output, "\n");
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
