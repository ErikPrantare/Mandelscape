#ifndef MANDELLANDSCAPE_MANDEL_TYPE_TRAITS_H
#define MANDELLANDSCAPE_MANDEL_TYPE_TRAITS_H

#include <type_traits>

template<bool predicate>
using Require = std::enable_if_t<predicate>;

template<typename T1, typename T2>
inline bool constexpr Same = std::is_same_v<T1, T2>;

template<typename T1, typename T2>
using RequireSame = Require<Same<T1, T2>>;

template<typename Callable, typename ResultType, typename... Args>
inline bool constexpr Returns =
        Same<std::invoke_result_t<Callable, Args...>, ResultType>;

template<typename Callable, typename ResultType, typename... Args>
using RequireReturns = Require<Returns<Callable, ResultType, Args...>>;

template<typename Callable, typename... Argument>
using RequireCallableWith = std::invoke_result_t<Callable, Argument...>;

template<typename Callable, typename Argument>
static bool constexpr EndomorphismOf = Returns<Callable, Argument, Argument>;

template<typename Callable, typename Argument>
using RequireEndomorphismOf = Require<EndomorphismOf<Callable, Argument>>;

#endif    // MANDELLANDSCAPE_MANDEL_TYPE_TRAITS_H
