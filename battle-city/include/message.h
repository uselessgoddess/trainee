#pragma once

#include <array>
#include <memory>
#include <optional>

#include "serde/serde.hpp"

/// `std::varint` with only two possible condition
template <typename T, typename U>
// using either = std::variant<T, U>; -- does not use with pack parameters
struct either : std::variant<T, U> {
  using base_t = std::variant<T, U>;
  using base_t::base_t;
};

namespace ser {
template <serialized T, serialized U>
struct serialize<either<T, U>> {
  // signature
  auto operator()(const either<T, U>& self) const -> std::string {
    return std::visit([&self](auto&& visited) { return char(self.index()) + to_string(visited); },
                      self);
  }
};
}  // namespace ser

/*
 for example, it is invalid enum:
 ```
 enum struct Enum {
     E1 = 1,
     E2 = 228,
 }
 ```
*/

template <typename Enum>
constexpr auto to_underlying(Enum e) noexcept -> std::underlying_type_t<Enum> {
  return static_cast<std::underlying_type_t<Enum>>(e);
}

template <typename Enum>
concept possible_enum =
    /* it is unlikely to check it */ std::is_scoped_enum_v<Enum> && requires { Enum::__Max; };

enum struct Possible : size_t {
  Damage,
  Name,
  Lol,

  __Max,
};

template <possible_enum Enum, typename... Args>
class Message {
  // clang-format off
  // may be use `std::unordered_map`
  using place_t = std::array<
    std::optional<either<Args...>>,
    to_underlying(Enum::__Max)
  >;
  // clang-format on
  // to store array on heap (2..64 * 8..24 ~ 156 -- is great than 128)
  std::unique_ptr<place_t> place;

 public:
  Message() : place(std::make_unique<place_t>()) {}

  template <any_of<Args...> U>
  auto emplace(Enum e, U value) {
    place->at(to_underlying(e)) =
        std::optional{either<Args...>(std::in_place_type<U>, std::move(value))};
  }

  template <any_of<Args...> U>
  auto get(Enum e) const {
    return place->at(to_underlying(e)).template transform([](auto&& either) {
      return std::get<U>(either);
    });
  }

 public:
};