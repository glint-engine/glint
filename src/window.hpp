#pragma once

namespace window {

struct Config {
    int width;
    int height;
    int fps;
    const char *title;
};

struct Window {
    Config config;
};

auto setup() -> void;
auto create(Config config) -> Window;
void close(Window& self);
bool should_close(Window& self);
void begin_drawing(Window& self);
void end_drawing(Window& self);
void draw_fps(Window& self);

} // namespace window
