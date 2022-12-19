#pragma once

#include <exception>  // std::terminate

namespace noexport {
struct infallible {
  constexpr infallible() = delete;
  constexpr infallible(const infallible& /*unused*/) noexcept = default;

  template <typename T>
  [[noreturn]] constexpr operator T&() const noexcept {
    std::terminate();
  }
  template <typename T>
  [[noreturn]] constexpr operator T&&() const noexcept {
    std::terminate();
  }
};
}  // namespace noexport