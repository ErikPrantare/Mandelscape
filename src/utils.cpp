#include <string>
#include <fstream>

#include "utils.h"

std::string
readFile(const std::string& fileName) {
    std::ifstream in(fileName);
    std::string output;

    for(std::string line; std::getline(in, line);) {
        output += line + "\n";
    }

    in.close();
    return output;
}
