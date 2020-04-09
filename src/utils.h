#ifndef MANDELLANDSCAPE_UTILS_H
#define MANDELLANDSCAPE_UTILS_H

#include <string>
#include <fstream>

std::string
readFile(std::string const& filePath);

constexpr bool
invert(bool const b)
{
    return !b;
}

#endif    // MANDELLANDSCAPE_UTILS_H
