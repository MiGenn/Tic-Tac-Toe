#pragma once
#include <concepts>

template <typename T>
concept Integer = std::integral<T> && (!std::same_as<bool, T>);

template <typename T>
concept Number = Integer<T> || std::floating_point<T>;
