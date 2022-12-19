#pragma once

// `Privilege` need for provide access to chance drop level after destroying headquarters
struct HeadQ : Collision<Bullet>, Privilege<Level> {
  void do_collision(Context &context, const Bullet&) override {
    TODO
  }
};
