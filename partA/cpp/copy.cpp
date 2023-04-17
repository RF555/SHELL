#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/stat.h>

using namespace std;

// Print usage information and exit with error code 1
void print_usage() {
    cout << "Usage: copy <source> <destination> [-f] [-v]" << endl;
    cout << "Flags:" << endl;
    cout << "  -f : Overwrite the destination file if it exists" << endl;
    cout << "  -v : Verbose output" << endl;
    exit(1);
}

// Check if a file exists
bool file_exists(const char* filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

int main(int argc, char** argv) {
    bool verbose = false;
    bool force = false;

    // Parse command-line arguments
    if (argc < 3) {
        print_usage();
    } else {
        for (int i = 3; i < argc; i++) {
            if (strcmp(argv[i], "-f") == 0) {
                force = true;
            } else if (strcmp(argv[i], "-v") == 0) {
                verbose = true;
            } else {
                print_usage();
            }
        }
    }

    const char* src_filename = argv[1];
    const char* dst_filename = argv[2];

    // Check if the source file exists
    if (!file_exists(src_filename)) {
        cerr << "Error: source file does not exist" << endl;
        return 1;
    }

    // Check if the destination file exists
    if (file_exists(dst_filename) && !force) {
        cerr << "target file exist" << endl;
        return 1;
    }

    // Open the source file for reading
    ifstream src_file(src_filename, ios::binary);
    if (!src_file.is_open()) {
        cerr << "Error: could not open source file" << endl;
        return 1;
    }

    // Open the destination file for writing
    ofstream dst_file(dst_filename, ios::binary);
    if (!dst_file.is_open()) {
        cerr << "Error: could not create destination file" << endl;
        return 1;
    }

    // Copy the contents of the source file to the destination file
    dst_file << src_file.rdbuf();

    // Close the files
    src_file.close();
    dst_file.close();

    // Print verbose output
    if (verbose) {
        cout << "success" << endl;
    }

    return 0;
}
