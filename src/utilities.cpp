#include "utilities.h"
#include <fstream>
#include <streambuf>

std::string read_file(const std::string& filepath) {
    std::ifstream file(filepath);

    std::string contents((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());
    return contents;
}


bool file_exists(const std::string& filepath) {
    std::ifstream fin(filepath);
    return fin.good();
}
