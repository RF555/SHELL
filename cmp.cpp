#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

int main(int argc, char *argv[]) {
    // Check if enough arguments were provided
    if (argc < 3) {
        std::cerr << "Usage: cmp <file1> <file2> [-v] [-i]" << std::endl;
        return 1;
    }

    // Initialize flag variables
    bool verbose = false;
    bool ignore_case = false;

    // Parse command line arguments
    for (int i = 3; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-v") {
            verbose = true;
        } else if (arg == "-i") {
            ignore_case = true;
        }
    }

    // Open the input files
    std::ifstream file1(argv[1], std::ios::binary);
    std::ifstream file2(argv[2], std::ios::binary);

    if (!file1 || !file2) {
        std::cerr << "Error: Failed to open input files." << std::endl;
        return 1;
    }

    // Compare the files character by character
    bool equal = true;

    while (!file1.eof() || !file2.eof()) {
        char c1, c2;
        file1.get(c1);
        file2.get(c2);

        if (ignore_case) {
            c1 = std::tolower(c1);
            c2 = std::tolower(c2);
        }

        if (c1 != c2) {
            equal = false;
            break;
        }
    }

    // Check if the files are equal
    if (file1.eof() && file2.eof() && equal) {
        if (verbose) {
            std::cout << "equal" << std::endl;
        }
        return 0;
    } else {
        if (verbose) {
            std::cout << "distinct" << std::endl;
        }
        return 1;
    }
}
