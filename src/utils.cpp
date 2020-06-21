#include "utils.hpp"

#include <string>
#include <fstream>

#include <glm/glm.hpp>

namespace util {

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

}    // namespace util
