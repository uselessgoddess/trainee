#pragma once

namespace de {

template <typename Self>
struct deserialize {
  // associated -- later
  // using error;

  // signature
  // auto operator()(std::string_view str) const -> std::pair<Self, std::string_view>;
};

// it can be simplified?
template <typename Self>
concept deserialized =
    noexport::pure<Self> && requires(std::string_view str) { deserialize<Self>{}(str); };

template <deserialized Self>
/* result is more idiomatic -- it `std::optional` + error type */
auto from_str(std::string_view str) -> std::pair<Self, std::string_view> {
  constexpr auto de = deserialize<Self>{};
  return de(str);
}

template <std::integral Self>
struct deserialize<Self> {
  auto operator()(std::string_view str) const -> std::pair<Self, std::string_view> {
    Self place;
    memcpy(&place, &str[0], sizeof(Self));
    return {place, str.substr(sizeof(Self))};
  }
};

template <>
struct deserialize<std::string> {
  // string is "{string_data}\0"
  auto operator()(std::string_view str) const -> std::pair<std::string, std::string_view> {
    if (auto nil = str.find('\0'); nil != std::string_view::npos) {
      return {std::string(str.substr(0, nil)), str.substr(nil + 1)};
    } else {
      throw std::logic_error("expected `\\0` terminated string");
    }
  }
};

}  // namespace de