#pragma once

#include <concepts>
#include <cstring>
#include <string>
#include <variant>

namespace ser {

template <typename Self>
struct serialize {
  // signature
  // auto operator()(const Self& self) const -> std::string;
};

// it can be simplified?
template <typename Self>
concept serialized =
    noexport::pure<Self> && requires(Self self) { serialize<std::remove_cvref_t<Self>>{}(self); };

auto to_string(serialized auto&& self) -> std::string {
  constexpr auto ser = serialize<std::remove_cvref_t<decltype(self)>>{};
  return ser(self);
}

template <std::integral Self>
struct serialize<Self> {
  // signature
  auto operator()(const Self& self) const -> std::string {
    std::string buf(sizeof(Self), '\0');
    std::memcpy(&buf[0], &self, sizeof(Self));
    return buf;
  }
};

template <>
struct serialize<std::string> {
  // signature
  auto operator()(const std::string& self) const -> std::string {
    return self + '\0';
  }
};

}  // namespace ser