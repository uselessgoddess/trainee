#pragma once

#include "magic/engine.h"
#include "tank.h"

struct Bullet : Collision<Tank> {
  void do_collision(Context &context, const Tank &) override {
    TODO  // on kill or on collision with wall
  }
};
