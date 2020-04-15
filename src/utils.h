#ifndef MANDELLANDSCAPE_UTILS_H
#define MANDELLANDSCAPE_UTILS_H

#include <string>
#include <fstream>
#include <queue>
#include <optional>

#include "mandelTypeTraits.h"

namespace utils {

std::string
readFile(std::string const& filePath);

template<typename... Callables>
struct overload : Callables... {
    using Callables::operator()...;
};

template<typename... Callables>
overload(Callables...)->overload<Callables...>;

template<typename T, typename Container>
std::optional<T>
pop(std::queue<T, Container>& queue)
{
    if(queue.empty())
        return std::nullopt;

    auto a = queue.front();
    queue.pop();
    return a;
}

template<
        typename T,
        typename Yielder,
        typename = RequireReturns<Yielder, std::optional<T>>,
        typename Consumer,
        typename = RequireCallableWith<Consumer, T>>
void
untilNullopt(Yielder&& yielder, Consumer&& consumer)
{
    while(auto a = yielder()) {
        consumer(*a);
    }
}

}    // namespace utils

#endif    // MANDELLANDSCAPE_UTILS_H
