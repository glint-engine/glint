#include "screen.hpp"

#include <mujs.h>
#include <raylib.h>

#include "bindings.hpp"
#include "js.hpp"

namespace muen::bindings::screen {

auto define(js::State *j) -> void {
    define_global_function(
        j,
        [](js::State *j) { js::pushnumber(j, static_cast<double>(::GetFrameTime())); },
        "__muenScreenDt"
    );

    define_global_function(
        j,
        [](js::State *j) { js::pushnumber(j, static_cast<double>(::GetScreenWidth())); },
        "__muenScreenWidth"
    );

    define_global_function(
        j,
        [](js::State *j) { js::pushnumber(j, static_cast<double>(::GetScreenHeight())); },
        "__muenScreenHeight"
    );
}

} // namespace muen::bindings::screen
