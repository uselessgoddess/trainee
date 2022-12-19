#pragma once

struct Tank : StrongCollision, Oriented {
  void on_shot(Context& ctx) {
    TODO
  }
};

struct Player : Tank, EventListener<events::Keyboard> {
  void do_event(const events::Keyboard&) final {
    TODO  // may be call `on_shot`
  }
};
