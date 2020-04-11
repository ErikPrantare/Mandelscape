#ifndef MANDELLANDSCAPE_UTILS_H
#define MANDELLANDSCAPE_UTILS_H

#include <string>
#include <fstream>
#include <future>

std::string
readFile(std::string const& filePath);

template<typename T>
static bool
isDone(const std::future<T>& f)
{
    return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

#endif    // MANDELLANDSCAPE_UTILS_H
