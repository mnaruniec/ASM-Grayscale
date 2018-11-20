#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// Set color weights. Arguments should sum to 256 for best results.
extern void set_rgb(unsigned red, unsigned green, unsigned blue);

extern void to_grayscale(unsigned char *buffer, unsigned width, unsigned height);


int main(int argc, char **argv) {
    FILE *input;
    FILE *output;
    unsigned width = 0, height = 0, depth = 0;
    size_t length = 0;
    unsigned tmp = 0;
    unsigned char *buffer = NULL;
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

    if(!(buffer = malloc(3 * length))) {
        perror("malloc(buffer)");
        goto close_input;
    }

    for (size_t i = 0; i < 3 * length; i++) {
        tmp = 0;
        if (EOF == fscanf(input, "%u", &tmp) || tmp > 255) {
            printf("pixel read error\n");
            goto free_buffer;
        }
        buffer[i] = (unsigned char)tmp;
    }

    to_grayscale(buffer, width, height);

    if(!(output = fopen(argv[2], "w"))) {
        perror("fopen(output)");
        goto free_buffer;
    }

    fprintf(output, "P2\n%u %u\n255\n", width, height);

    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            fprintf(output, "%u ", (unsigned) (buffer[i * width + j]));
        }
        if (0 >= fprintf(output, "\n")) {
            printf("fprintf error");
            goto close_output;
        }
    }

    ret = 0;

    close_output:
        pclose(output);
    free_buffer:
        free(buffer);
    close_input:
        pclose(input);
    return ret;
}
