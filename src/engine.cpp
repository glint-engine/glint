#include "engine.hpp"

#include <spdlog/spdlog.h>

#include "js.hpp"
#include "bindings.hpp"
#include "defer.hpp"
#include "window.hpp"

namespace muen::engine {

Config read_config(Engine& self);

Engine create() {
    auto js = js::newstate(nullptr, nullptr, js::STRICT);
    return Engine {.js = js};
}

void destroy(Engine& self) {
    js::freestate(self.js);
}

int run(Engine& self, const char *path) {
    window::setup();

    self.root_path = path;
    self.modules = new std::unordered_map<std::string, std::string>{
        {
            "muen/globals.js",
            {
#include "globals.js.h"
            }
        },
        {
            "muen/screen.js",
            {
#include "screen.js.h"
            }
        },
        {
            "muen/Color.js",
            {
#include "Color.js.h"
            }
        },
        {
            "muen/graphics.js",
            {
#include "graphics.js.h"
            }
        },
        {
            "muen/Vector2.js",
            {
#include "Vector2.js.h"
            }
        },
        {
            "muen/Rectangle.js",
            {
#include "Rectangle.js.h"
            }
        },
        {
            "muen/Camera.js",
            {
#include "Camera.js.h"
            }
        }
    };

    js::setcontext(self.js, &self);

    bindings::define(self.js);

    auto game_path = std::string {path};
    if (game_path[game_path.size() - 1] != '/') {
        game_path += '/';
    }
    game_path += "Game.js";

    Config config;
    try {
        mujs_catch(self.js);

        js::pushstring(
            self.js,
            R"(
            var Game = require("Game");
            var game = new Game();
            )"
        );
        js::eval(self.js);
        js::pop(self.js, 1);
        js::endtry(self.js);

        config = read_config(self);
    } catch (const js::Exception& e) {
        spdlog::error("Error initializing game: {}", e.what());
        return 1;
    }

    auto w = window::create(
        window::Config {
            .width = config.width,
            .height = config.height,
            .fps = config.fps,
            .title = config.title,
        }
    );
    defer(window::close(w));

    try {
        mujs_catch(self.js);
        while (!window::should_close(w)) {
            js::getglobal(self.js, "game");
            js::getproperty(self.js, -1, "update");
            js::rot2(self.js);
            js::call(self.js, 0);
            js::pop(self.js, 1);

            window::begin_drawing(w);

            js::getglobal(self.js, "game");
            js::getproperty(self.js, -1, "draw");
            js::rot2(self.js);
            js::call(self.js, 0);
            js::pop(self.js, 1);

            window::draw_fps(w);
            window::end_drawing(w);
        }
        js::endtry(self.js);
    } catch (js::Exception& e) {
        spdlog::error("Error running game: {}", e.what());
        printf("\n");
        js::dump(self.js);
        return 1;
    }

    return 0;
}

Config read_config(Engine& self) {
    mujs_catch(self.js);

    auto config = Config {};
    js::getglobal(self.js, "game");
    if (js::hasproperty(self.js, -1, "config")) {
        if (js::hasproperty(self.js, -1, "fps")) {
            config.fps = js::tointeger(self.js, -1);
            js::pop(self.js, 1);
        }

        if (js::hasproperty(self.js, -1, "width")) {
            config.width = js::tointeger(self.js, -1);
            js::pop(self.js, 1);
        }

        if (js::hasproperty(self.js, -1, "height")) {
            config.height = js::tointeger(self.js, -1);
            js::pop(self.js, 1);
        }

        if (js::hasproperty(self.js, -1, "title")) {
            config.title = js::tostring(self.js, -1);
            js::pop(self.js, 1);
        }

        js::pop(self.js, 1);
    }
    js::pop(self.js, 1);

    js::endtry(self.js);

    return config;
}

} // namespace muen::engine
