#pragma once

#include "bullet.h"
#include "magic/engine.h"

template <size_t Strength>
struct Wall : StrongCollision, Collision<Bullet> {
  [[nodiscard]] constexpr auto strength() const noexcept -> size_t {
    return Strength;
  }

  void do_collision(Context &context, const Bullet &) override {
    TODO  // damage wall
  }
};

// значения из головы
using RockWall = Wall<3>;
using SteelWall = Wall<5>;
