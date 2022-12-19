#pragma once

#include <concepts>

#include "infallible.h"
#include "todo.h"
#include "vec.h"

struct Positioned {
  [[nodiscard]] virtual auto position() const noexcept -> Vec2 = 0;
};

enum struct Orientation { Up, Down, Left, Right };

struct Oriented {
  [[nodiscard]] virtual auto orient() const noexcept -> Orientation = 0;
};

// struct Context;
using ContextImpl = noexport::infallible;
using Context = const ContextImpl;

// react to collision
template <typename With = Positioned>
  requires std::derived_from<With, Positioned>
struct Collision : Positioned {
  // `Position` in this context is more complex denotes objects which can act to collision
  virtual void do_collision(Context& context, const With&) = 0;
};

// barrier collision
struct StrongCollision : Collision<> {
  void do_collision(Context& context, const Positioned&) final {
    TODO
  }
};

namespace events {
struct Keyboard;
}

template <typename Event>
struct EventListener {
  virtual void do_event(const Event&) = 0;
};

struct Level;

template <typename T, typename... Ts>
concept any_of = (std::same_as<T, Ts> || ...);

// add access to outside world
template <typename... Provided>
struct Privilege {
  template <any_of<Provided...> P>
  auto provide() -> const P& {
    TODO
  }
};
