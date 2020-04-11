#ifndef MANDELLANDSCAPE_UTILS_H
#define MANDELLANDSCAPE_UTILS_H

#include <string>
#include <fstream>
#include <future>

namespace util {

std::string
readFile(std::string const& filePath);

// CPP20 https://en.cppreference.com/w/cpp/experimental/future/is_ready
template<typename T>
static bool
isDone(const std::future<T>& f)
{
    return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

}    // namespace util

#endif    // MANDELLANDSCAPE_UTILS_H
