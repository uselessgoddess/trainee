#pragma once

namespace noexport {
template <typename T>
concept pure = !(std::is_reference_v<T> || std::is_pointer_v<T>);
}

#include "de.h"
#include "ser.h"