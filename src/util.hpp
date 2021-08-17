/* Mandelscape
 * Copyright (C) 2020-2021 Erik Präntare
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MANDELLANDSCAPE_UTILS_HPP
#define MANDELLANDSCAPE_UTILS_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <future>
#include <cmath>
#include <map>
#include <set>
#include <queue>
#include <optional>
#include <type_traits>
#include <filesystem>
#include <variant>

#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.hpp>

#include <nfd.hpp>

#include "lua.hpp"

#include "glfwEnums.hpp"
#include "mandelTypeTraits.hpp"

namespace util {

// CPP20 std::numbers::pi
double constexpr pi = glm::pi<double>();

// Template so forwarding reference can accept rvalues
template<class In = std::istream>
[[nodiscard]] auto
getContents(In&& in) -> std::string
{
    return std::string(std::istreambuf_iterator(in), {});
}

// CPP20 https://en.cppreference.com/w/cpp/experimental/future/is_ready
template<typename T>
[[nodiscard]] auto
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
[[nodiscard]] auto
contains(std::map<KeyType, ValueType> const& map, KeyType key) -> bool
{
    return map.find(key) != map.end();
}

// CPP20 https://en.cppreference.com/w/cpp/container/set/contains
template<typename ValueType>
[[nodiscard]] auto
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

auto constexpr nop = [](auto&&...) {
};

auto
currentDatetimeString() -> std::string;

template<typename T, typename Container>
[[nodiscard]] auto
pop(std::queue<T, Container>& queue) -> std::optional<T>
{
    if(queue.empty()) {
        return std::nullopt;
    }

    auto a = queue.front();
    queue.pop();
    return a;
}

[[nodiscard]] auto constexpr pixelsToAngle(
        glm::dvec2 nrPixels,
        double sensitivity = 0.01) -> glm::dvec2
{
    //-x, refer to right hand rule with y up and positive pixels rightwards
    return sensitivity * glm::dvec2(-nrPixels.x, nrPixels.y);
}

[[nodiscard]] auto constexpr planePos(glm::dvec3 spacePos) -> glm::dvec2
{
    return {spacePos.x, spacePos.z};
}

[[nodiscard]] auto constexpr truncateExponent(double value, double base)
        -> double
{
    return std::pow(base, (long int)(std::log(value) / std::log(base)));
}

[[nodiscard]] auto
toGpuVec(glm::dvec3 v) -> glm::vec3;

// CPP20 Make constexpr
[[nodiscard]] auto
unitVec2(double theta) noexcept -> glm::dvec2;

// CPP20 Make constexpr
[[nodiscard]] auto
isModifier(Input::Key key) noexcept -> bool;

// CPP20 Make constexpr
[[nodiscard]] auto
toMod(Input::Key key) noexcept(false) -> Input::Mod;

// CPP20 std::string::ends_with
template<class T>
[[nodiscard]] auto
endsWith(
        std::basic_string<T> const& value,
        std::basic_string<T> const& ending) noexcept -> bool
{
    if(ending.size() > value.size()) {
        return false;
    }
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

// CPP?? https://en.cppreference.com/w/cpp/experimental/scope_exit
class ScopeGuard {
public:
    ScopeGuard(std::function<void()>&& destructor) : m_destructor(destructor)
    {}
    ScopeGuard(ScopeGuard const&) = delete;
    ScopeGuard(ScopeGuard&&)      = delete;
    auto
    operator=(ScopeGuard const&) -> ScopeGuard& = delete;
    auto
    operator=(ScopeGuard&&) -> ScopeGuard& = delete;

    ~ScopeGuard()
    {
        m_destructor();
    }

private:
    std::function<void()> m_destructor;
};

template<class Variant, class... Function>
auto
dispatch(Variant& variant, Function... fs)
{
    std::visit(util::Overload{fs..., util::nop}, variant);
};

}    // namespace util

namespace util::nfd {
namespace literal {
    auto operator""_nfd(char const* str, size_t size)
            -> std::filesystem::path::string_type;
}

namespace fs = std::filesystem;
using string = fs::path::string_type;

struct FilterItem {
    string niceName;
    string suffixes;
};

[[nodiscard]] auto
saveDialog(
        std::vector<FilterItem> const& filterItems,
        fs::path const& startPath,
        string const& defaultName) -> std::optional<fs::path>;

[[nodiscard]] auto
openDialog(
        std::vector<FilterItem> const& filterItems,
        fs::path const& startPath) -> std::optional<fs::path>;

[[nodiscard]] auto
openDialogMultiple(
        std::vector<FilterItem> const& filterItems,
        fs::path const& startPath) -> std::vector<fs::path>;
}    // namespace util::nfd

#endif    // MANDELLANDSCAPE_UTILS_HPP
