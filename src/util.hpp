#ifndef MANDELLANDSCAPE_UTILS_HPP
#define MANDELLANDSCAPE_UTILS_HPP

#include <string>
#include <fstream>
#include <future>
#include <cmath>
#include <map>
#include <set>
#include <queue>
#include <optional>

#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.hpp>

#include "mandelTypeTraits.hpp"

namespace util {

double constexpr pi = glm::pi<double>();

auto
readFile(std::string const& filePath) -> std::string;

// CPP20 https://en.cppreference.com/w/cpp/experimental/future/is_ready
template<typename T>
auto
isDone(std::future<T> const& f) -> bool
{
    return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

template<typename T>
class LowPassFilter {
public:
    LowPassFilter(T init, double amount) :
                m_filteredValue(init),
                m_amount(amount){};

    auto
    operator()(T const newValue, double const weight = 1.0f) -> T
    {
        double const factor = std::pow(m_amount, weight);

        m_filteredValue = factor * m_filteredValue + (1.0 - factor) * newValue;
        return m_filteredValue;
    }

private:
    T m_filteredValue;
    double m_amount;
};

// CPP20 https://en.cppreference.com/w/cpp/container/map/contains
template<typename KeyType, typename ValueType>
auto
contains(std::map<KeyType, ValueType> const& map, KeyType key) -> bool
{
    return map.find(key) != map.end();
}

// CPP20 https://en.cppreference.com/w/cpp/container/set/contains
template<typename ValueType>
auto
contains(std::set<ValueType> const& set, ValueType key) -> bool
{
    return set.find(key) != set.end();
}

template<typename... Callables>
struct Overload : Callables... {
    using Callables::operator()...;
};

// CPP20
// https://en.cppreference.com/w/cpp/language/class_template_argument_deduction
template<typename... Callables>
Overload(Callables...) -> Overload<Callables...>;

auto constexpr unaryNOP = [](auto&&) {
};

template<typename T, typename Container>
auto
pop(std::queue<T, Container>& queue) -> std::optional<T>
{
    if(queue.empty()) {
        return std::nullopt;
    }

    auto a = queue.front();
    queue.pop();
    return a;
}

auto constexpr pixelsToAngle(glm::dvec2 nrPixels, double sensitivity = 0.01)
        -> glm::dvec2
{
    //-x, refer to right hand rule with y up and positive pixels rightwards
    return sensitivity * glm::dvec2(-nrPixels.x, nrPixels.y);
}

auto constexpr planePos(glm::dvec3 spacePos) -> glm::dvec2
{
    return {spacePos.x, spacePos.z};
}

// CPP20 Make constexpr
auto
unitVec2(double theta) -> glm::dvec2;

}    // namespace util

#endif    // MANDELLANDSCAPE_UTILS_HPP
