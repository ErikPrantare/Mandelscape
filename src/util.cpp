#include "util.hpp"

#include <string>
#include <fstream>

#include <glm/glm.hpp>
#include <iostream>

namespace util {

auto
readFile(std::string const& filePath) -> std::string
{
    std::cout << filePath;
    std::string output;
    std::ifstream in(filePath);

    for(std::string line; std::getline(in, line);) {
        output += line + "\n";
    }

    in.close();
    return output;
}

auto
unitVec2(double theta) -> glm::dvec2
{
    return {std::cos(theta), std::sin(theta)};
}

}    // namespace util
