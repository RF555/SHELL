#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SUCCESS 0
#define FAILURE 1

void print_usage() {
    printf("Usage: copy <source_file> <destination_file> [-v] [-f]\n");
    printf("Options:\n");
    printf("\t-v\tVerbose output\n");
    printf("\t-f\tForce overwrite of destination file\n");
}

int main(int argc, char *argv[]) {
    FILE *src, *dst;
    char c;
    int verbose = 0, force = 0;

    // Check for correct number of arguments
    if (argc < 3) {
        print_usage();
        return FAILURE;
    }

    // Parse command line arguments
    if (argc >= 4) {
        if (strcmp(argv[3], "-v") == 0) {
            verbose = 1;
        } else if (strcmp(argv[3], "-f") == 0) {
            force = 1;
        }
    }
    if (argc == 5) {
        if (strcmp(argv[4], "-v") == 0) {
            verbose = 1;
        }
    }

    // Open source file
    src = fopen(argv[1], "rb");
    if (src == NULL) {
        printf("Error: could not open source file\n");
        return FAILURE;
    }

    // Check if destination file exists and force flag not set
    dst = fopen(argv[2], "rb");
    if (dst != NULL && !force) {
        printf("Target file exists\n");
        fclose(dst);
        fclose(src);
        return FAILURE;
    }

    // Open destination file
    dst = fopen(argv[2], "wb");
    if (dst == NULL) {
        printf("Error: could not create destination file\n");
        fclose(src);
        return FAILURE;
    }

    // Copy contents of source file to destination file
    while ((c = fgetc(src)) != EOF) {
        fputc(c, dst);
    }

    // Close files
    fclose(dst);
    fclose(src);

    // Print verbose output if enabled
    if (verbose) {
        printf("Success\n");
    }

    return SUCCESS;
}
