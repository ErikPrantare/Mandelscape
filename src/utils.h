#ifndef MANDELLANDSCAPE_UTILS_H
#define MANDELLANDSCAPE_UTILS_H

#include <string>
#include <fstream>
#include <future>
#include <cmath>
#include <map>
#include <queue>
#include <optional>

#include "mandelTypeTraits.h"

namespace util {

std::string
readFile(std::string const& filePath);

// CPP20 https://en.cppreference.com/w/cpp/experimental/future/is_ready
template<typename T>
bool
isDone(std::future<T> const& f)
{
    return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

class LowPassFilter {
public:
    LowPassFilter(float const init, float const amount) :
                m_filteredValue(init),
                m_amount(amount){};

    float
    operator()(float const newValue, float const weight = 1.0f)
    {
        const float factor = std::pow(m_amount, weight);

        m_filteredValue = factor * m_filteredValue + (1.0f - factor) * newValue;
        return m_filteredValue;
    }

private:
    float m_filteredValue;
    float const m_amount;
};

// CPP20 https://en.cppreference.com/w/cpp/container/map/contains
template<typename KeyType, typename ValueType>
bool
contains(std::map<KeyType, ValueType> const& map, KeyType key)
{
    return map.find(key) != std::end(map);
}

template<typename... Callables>
struct overload : Callables... {
    using Callables::operator()...;
};

// CPP20
// https://en.cppreference.com/w/cpp/language/class_template_argument_deduction
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
        typename... ConsumerArgs,
        typename = RequireCallableWith<Consumer, T, ConsumerArgs...>>
void
untilNullopt(
        Yielder&& yielder,
        Consumer&& consumer,
        ConsumerArgs&&... consumerArgs)
{
    while(auto a = yielder()) {
        consumer(*a, std::forward<ConsumerArgs>(consumerArgs)...);
    }
}

}    // namespace util

#endif    // MANDELLANDSCAPE_UTILS_H
