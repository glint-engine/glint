#pragma once

#include <unordered_map>
#include <string>

#include "js.hpp"

namespace muen::engine {

struct Config {
    int width = 800;
    int height = 600;
    int fps = 60;
    const char *title = "muen2d";
};

struct Engine {
    js::State *js;
    const char *root_path;
    std::unordered_map<std::string, std::string> *modules;
};

Engine create();

void destroy(Engine& window);

int run(Engine& self, const char *path);

} // namespace muen::engine
