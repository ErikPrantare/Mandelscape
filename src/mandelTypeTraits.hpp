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
