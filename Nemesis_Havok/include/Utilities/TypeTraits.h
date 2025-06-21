#pragma once

#include <type_traits>

// Primary template (false case)
template <template <typename...> class Base, typename T>
struct is_specialization_of : std::false_type
{
};

// Specialization (true case)
template <template <typename...> class Base, typename... Args>
struct is_specialization_of<Base, Base<Args...>> : std::true_type
{
};

// Helper variable template (C++14+)
template <template <typename...> class Base, typename T>
constexpr bool is_specialization_of_v = is_specialization_of<Base, T>::value;

template <typename T, typename = void>
struct is_complete : std::false_type
{
};

// Specialization kicks in only if sizeof(T) is valid
template <typename T>
struct is_complete<T, std::void_t<decltype(sizeof(T))>> : std::true_type
{
};

// Helper variable template (C++17+)
template <typename T>
constexpr bool is_complete_v = is_complete<T>::value;
