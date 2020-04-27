#ifndef MANDELLANDSCAPE_MANDEL_TYPE_TRAITS_H
#define MANDELLANDSCAPE_MANDEL_TYPE_TRAITS_H

#include <type_traits>

template<bool predicate>
using Require = std::enable_if_t<predicate>;

template<class T1, class T2>
inline bool constexpr Same = std::is_same_v<T1, T2>;

template<class T1, class T2>
using RequireSame = Require<Same<T1, T2>>;

template<class Callable, class ResultType, class... Args>
inline bool constexpr Returns =
        Same<std::invoke_result_t<Callable, Args...>, ResultType>;

template<class Callable, class ResultType, class... Args>
using RequireReturns = Require<Returns<Callable, ResultType, Args...>>;

template<class Callable, class... Arguments>
using RequireCallableWith = std::invoke_result_t<Callable, Arguments...>;

template<class Callable, class Argument>
static bool constexpr EndomorphismOf = Returns<Callable, Argument, Argument>;

template<class Callable, class Argument>
using RequireEndomorphismOf = Require<EndomorphismOf<Callable, Argument>>;

template<class Type, template<class...> class Template>
struct SpecialisationOf : std::false_type {
    using type = Type;
};

template<template<class...> class Template, class... Args>
struct SpecialisationOf<Template<Args...>, Template> : std::true_type {
    using type = Template<Args...>;
};

#endif    // MANDELLANDSCAPE_MANDEL_TYPE_TRAITS_H
