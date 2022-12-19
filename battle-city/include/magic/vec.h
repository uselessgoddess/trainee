#pragma once

#include <cstddef>  // size_t
#include <cstdint>  // std::int64_t

#include "infallible.h"

template <typename T, size_t N>
// struct Vector;
using Vector = noexport::infallible;

template <typename T>
using Vector2 = Vector<T, 2>;

using Vec2 = Vector2<std::int64_t>;