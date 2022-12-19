#pragma once

#include "magic/engine.h"
#include "tank.h"

template <typename Derived>
struct Surface : Collision<Tank> {
  void do_collision(Context& context, const Tank& other) final {
    static_cast<const Derived&>(*this).surface_effect(context, other);
  }
};

#define FULL_ALIAS($base, $me, $body) \
  struct $me : $base<$me> {                                                              \
    /* it is almost struct alias as if we pass `surface_effect` logic as template arg */ \
    using base_t = $base<$me>;                                                           \
    using base_t::base_t;                                                                \
    $body                                                                                \
  };

FULL_ALIAS(Surface, Water,  // clang-format off
  void surface_effect(Context& context, const Positioned&) const {
      TODO // react to water
  }  // clang-format on
)

FULL_ALIAS(Surface, Ice,  // clang-format off
  void surface_effect(Context& context, const Positioned&) const {
      TODO // react to ice
  }  // clang-format on
)

#undef FULL_ALIAS
