#ifndef MANDELLANDSCAPE_MANDEL_TYPE_TRAITS_H
#define MANDELLANDSCAPE_MANDEL_TYPE_TRAITS_H

#include <type_traits>

template<bool predicate>
using Require = std::enable_if_t<predicate>;

template<
        typename Callable,
        typename Argument,
        typename = std::invoke_result_t<Callable, Argument>>
inline bool constexpr CallableWith = true;

template<typename Callable, typename Argument>
using RequireCallableWith = Require<CallableWith<Callable, Argument>>;

template<
        typename Callable,
        typename Argument,
        typename = RequireCallableWith<Callable, Argument>>
static bool constexpr EndomorphismOf = true;

template<typename Callable, typename Argument>
using RequireEndomorphismOf = Require<EndomorphismOf<Callable, Argument>>;

template<typename T1, typename T2>
using RequireSame = std::enable_if_t<std::is_same_v<T1, T2>>;

template<typename Callable, typename ResultType, typename... Args>
inline bool constexpr Returns =
        std::is_same_v<std::invoke_result_t<Callable, Args...>, ResultType>;

template<typename Callable, typename ResultType, typename... Args>
using RequireReturns = Require<Returns<Callable, ResultType, Args...>>;

#endif    // MANDELLANDSCAPE_MANDEL_TYPE_TRAITS_H
