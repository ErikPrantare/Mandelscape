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

#ifndef MANDELLANDSCAPE_MANDEL_TYPE_TRAITS_HPP
#define MANDELLANDSCAPE_MANDEL_TYPE_TRAITS_HPP

#include <type_traits>

template<bool predicate, class T = void>
using Require = std::enable_if_t<predicate, T>;

template<class T1, class T2>
inline bool constexpr same = std::is_same_v<T1, T2>;

template<class T1, class T2>
using RequireSame = Require<same<T1, T2>>;

template<class Callable, class ResultType, class... Args>
inline bool constexpr returns =
        same<std::invoke_result_t<Callable, Args...>, ResultType>;

template<class Callable, class ResultType, class... Args>
using RequireReturns = Require<returns<Callable, ResultType, Args...>>;

template<class Callable, class... Arguments>
using RequireCallableWith = std::invoke_result_t<Callable, Arguments...>;

template<class Callable, class Argument>
static bool constexpr endomorphismOf = returns<Callable, Argument, Argument>;

template<class Callable, class Argument>
using RequireEndomorphismOf = Require<endomorphismOf<Callable, Argument>>;

template<class Type, template<class...> class Template>
struct SpecialisationOf : std::false_type {
    using type = Type;
};

template<template<class...> class Template, class... Args>
struct SpecialisationOf<Template<Args...>, Template> : std::true_type {
    using type = Template<Args...>;
};

#endif    // MANDELLANDSCAPE_MANDEL_TYPE_TRAITS_HPP
