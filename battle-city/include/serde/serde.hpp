#pragma once

namespace noexport {
template <typename T>
concept pure = !(std::is_reference_v<T> || std::is_pointer_v<T>);
}

// clang-format off
#include "ser.h"
#include "de.h"
// clang-format on