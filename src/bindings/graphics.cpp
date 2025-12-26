#include "graphics.hpp"

#include <mujs.h>
#include <raylib.h>

#include "bindings.hpp"
#include "bindings/types.hpp"
#include "js.hpp"

namespace muen::bindings::graphics {

auto define(js::State *j) -> void {
    define_global_function(
        j,
        [](js::State *j) {
            ClearBackground(types::tocolor(j, 1));
            js::pushundefined(j);
        },
        "__muenGraphicsClear",
        1
    );

    define_global_function(
        j,
        [](js::State *j) {
            int x = js::tointeger(j, 1);
            int y = js::tointeger(j, 2);
            int radius = js::tointeger(j, 3);
            Color color = types::tocolor(j, 4);
            DrawCircle(x, y, radius, color);
            js::pushundefined(j);
        },
        "__muenGraphicsCircle",
        4
    );

    define_global_function(
        j,
        [](js::State *j) {
            Rectangle rec {};
            Vector2 origin {};
            float rotation {};
            Color color {};

            int top = js::gettop(j);

            if (top == 6) { // DrawRectangle
                rec.x = js::tointeger(j, 1);
                rec.y = js::tointeger(j, 2);
                rec.width = js::tointeger(j, 3);
                rec.height = js::tointeger(j, 4);
                color = types::tocolor(j, 5);
            } else if (top == 4) { // DrawRectangleV
                Vector2 position = types::tovector2(j, 1);
                Vector2 size = types::tovector2(j, 2);
                color = types::tocolor(j, 3);
                rec.x = position.x;
                rec.y = position.y;
                rec.width = size.x;
                rec.height = size.y;
            } else if (top == 3) { // DrawRectangleRec
                rec = types::torectangle(j, 1);
                color = types::tocolor(j, 2);
            } else if (top == 5) { // DrawRectanglePro
                rec = types::torectangle(j, 1);
                origin = types::tovector2(j, 2);
                rotation = static_cast<float>(js::tonumber(j, 3));
                color = types::tocolor(j, 4);
            } else {
                js::typeerror(j, "Invalid arguments count for graphics.rectangle");
            }

            DrawRectanglePro(rec, origin, rotation, color);

            js::pushundefined(j);
        },
        "__muenGraphicsRectangle"
    );

    define_global_function(
        j,
        [](js::State *j) {
            BeginMode2D(types::tocamera(j, 1));
            js::pushundefined(j);
        },
        "__muenGraphicsBeginCameraMode",
        1
    );

    define_global_function(
        j,
        [](js::State *j) {
            EndMode2D();
            js::pushundefined(j);
        },
        "__muenGraphicsEndCameraMode"
    );
}

} // namespace muen::bindings::graphics
