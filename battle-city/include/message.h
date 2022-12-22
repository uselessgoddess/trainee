#pragma once

#include <array>
#include <bitset>
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
  auto operator()(const either<T, U>& self) const -> std::string {
    return std::visit([&self](auto&& visited) { return char(self.index()) + to_string(visited); },
                      self);
  }
};
}  // namespace ser

namespace de {
template <deserialized T, deserialized U>
struct deserialize<either<T, U>> {
  auto operator()(std::string_view str) const -> std::pair<either<T, U>, std::string_view> {
    if (str.at(0) == '\0') {
      return from_str<T>(str.substr(1));
    } else {
      return from_str<U>(str.substr(1));
    }
  }
};
}  // namespace de

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
    /* it is unlikely to check it */ std::is_scoped_enum_v<Enum> && requires { Enum::__Max; } &&
    static_cast<size_t>(Enum::__Max) < 64;

enum struct Possible : size_t {
  Damage,
  Name,
  Lol,

  __Max,
};

template <possible_enum Enum>
consteval auto places_for() noexcept {
  return to_underlying(Enum::__Max);
}

template <possible_enum Enum, typename... Args>
class Message {
  // clang-format off
  // may be use `std::unordered_map`
  using place_t = std::array<
    std::optional<either<Args...>>,
    places_for<Enum>()
  >;
  // clang-format on
  // to store array on heap (2..64 * 8..24 ~ 156 -- is great than 128)
  std::unique_ptr<place_t> place;

  friend struct ser::serialize<Message<Enum, Args...>>;
  friend struct de::deserialize<Message<Enum, Args...>>;

 public:
  Message() : place(std::make_unique<place_t>()) {}
  // use later
  // Message(const Message& other) : place(std::make_unique<place_t>(*other.place)) {}

  template <any_of<Args...> U>
  auto emplace(Enum e, U value) {
    place->at(to_underlying(e)) =
        std::optional{either<Args...>(std::in_place_type<U>, std::move(value))};
  }

  auto get_raw(Enum e) const {
    return place->at(to_underlying(e));
  }

  template <any_of<Args...> U>
  auto get(Enum e) const {
    return get_raw(e).transform([](auto&& either) { return std::get<U>(either); });
  }
};

namespace noexport {
static_assert(sizeof(unsigned long long) * 8 == 64);

// generic function require more complex logic - I am lazy
auto bitset_to_bytes(std::bitset<64> set) -> std::string {
  auto place = std::string(8, '\0');
  auto value = set.to_ullong();
  // why here `unsigned long long` instead of `std::uint64_t`
  std::memcpy(&place[0], &value, sizeof(unsigned long long));
  return place;
}

auto bitset_from_bytes(std::string_view str) -> std::bitset<64> {
  unsigned long long place;
  std::memcpy(&place, &str[0], sizeof(unsigned long long));
  return place;
}
}  // namespace noexport

namespace ser {
template <possible_enum Enum, serialized... Args>
struct serialize<Message<Enum, Args...>> {
  auto operator()(const Message<Enum, Args...>& self) const -> std::string {
    auto&& place = *self.place;
    static_assert(std::extent_v<decltype(place)> < 64);

    std::string tail;
    std::bitset<64> base;
    // dreams about `ranges::enumerate`
    for (size_t i = 0; i < place.size(); i++) {
      if (auto&& value = place[i]; value.has_value()) {
        tail += to_string(*value);
        base[i] = true;
      }
    }
    return noexport::bitset_to_bytes(base) + tail;
  }
};
}  // namespace ser

namespace de {
template <possible_enum Enum, deserialized... Args>
struct deserialize<Message<Enum, Args...>> {
  auto operator()(std::string_view str) const
      -> std::pair<Message<Enum, Args...>, std::string_view> {
    auto mask = noexport::bitset_from_bytes(str.substr(0, 8));
    auto empty = Message<Enum, Args...>{};
    auto& place = *empty.place;

    str = str.substr(8);

    // we can use `enumerate + filter`, but i like C++
    for (size_t i = 0; i < mask.size(); i++) {
      if (mask[i]) {
        auto [value, new_str] = de::from_str<either<Args...>>(str);
        place[i] = std::optional(value);
        str = new_str;
      }
    }

    // this code doesn't use RVO?
    return {std::move(empty), str};
  }
};
}  // namespace de
