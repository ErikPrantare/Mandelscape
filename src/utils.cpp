#include "utils.h"

#include <string>
#include <fstream>

namespace utils {

std::string
readFile(std::string const& filePath)
{
    std::string output;
    std::ifstream in(filePath);

    for(std::string line; std::getline(in, line);) {
        output += line + "\n";
    }

    in.close();
    return output;
}

}    // namespace utils
