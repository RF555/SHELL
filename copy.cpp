#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char *argv[]) {
    // Check if enough arguments were provided
    if (argc < 3) {
        std::cerr << "Usage: copy <file1> <file2> [-v] [-f]" << std::endl;
        return 1;
    }

    // Initialize flag variables
    bool verbose = false;
    bool force = false;

    // Parse command line arguments
    for (int i = 3; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-v") {
            verbose = true;
        } else if (arg == "-f") {
            force = true;
        }
    }

    // Open the input and output files
    std::ifstream src(argv[1], std::ios::binary);
    std::ofstream dst(argv[2], force ? std::ios::binary : (std::ios::binary | std::ios::ate));

    if (!src) {
        std::cerr << "Error: Failed to open input file." << std::endl;
        return 1;
    }

    if (!dst) {
        std::cerr << "Error: Failed to open output file." << std::endl;
        return 1;
    }

    // Copy the input file to the output file
    dst << src.rdbuf();

    // Check if the copy was successful
    if (src.eof() && dst.tellp() == src.tellg()) {
        if (verbose) {
            std::cout << "success" << std::endl;
        }
        return 0;
    } else if (dst.tellp() != src.tellg()) {
        if (verbose) {
            std::cout << "target file exists" << std::endl;
        }
        return 1;
    } else {
        if (verbose) {
            std::cout << "general failure" << std::endl;
        }
        return 1;
    }
}
