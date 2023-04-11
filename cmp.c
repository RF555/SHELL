#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SUCCESS 0
#define ERROR 1

// Function to compare two files
int cmp_files(FILE *file1, FILE *file2, int ignore_case, int verbose) {
    int equal = 1; // assume files are equal until proven otherwise
    int ch1, ch2;
    long pos = 0;
    while (1) {
        ch1 = fgetc(file1);
        ch2 = fgetc(file2);
        pos++;
        if (ignore_case) {
            ch1 = toupper(ch1);
            ch2 = toupper(ch2);
        }
        if (ch1 == EOF || ch2 == EOF) {
            if (ch1 != ch2) {
                equal = 0;
            }
            break;
        }
        if (ch1 != ch2) {
            equal = 0;
            break;
        }
    }
    if (verbose) {
        if (equal) {
            printf("equal\n");
        } else {
            printf("distinct\n");
        }
    }
    return equal ? SUCCESS : ERROR;
}

int main(int argc, char *argv[]) {
    int ignore_case = 1;
    int verbose = 0;
    FILE *file1, *file2;
    // Check for flags
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0) {
            ignore_case = 1;
        } else if (strcmp(argv[i], "-v") == 0) {
            verbose = 1;
        } else {
            if (file1 == NULL) {
                file1 = fopen(argv[i], "r");
            } else if (file2 == NULL) {
                file2 = fopen(argv[i], "r");
            }
        }
    }
    if (file1 == NULL || file2 == NULL) {
        printf("Usage: cmp <file1> <file2> [-v] [-i]\n");
        return ERROR;
    }
    int result = cmp_files(file1, file2, ignore_case, verbose);
    fclose(file1);
    fclose(file2);
    return result;
}
