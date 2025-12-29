#include "graphics.hpp"

#include <raylib.h>

#include "bindings.hpp"
#include "bindings/types.hpp"
#include "js.hpp"

namespace muen::bindings::graphics {

auto define(js::State *j) -> void {
    define_global_function(
        j,
        [](js::State *j) -> void {
            const auto color = types::tocolor(j, 1);
            ClearBackground(color);
            js::pushundefined(j);
        },
        "__muenGraphicsClear",
        1
    );

    define_global_function(
        j,
        [](js::State *j) -> void {
            const auto x = js::tointeger(j, 1);
            const auto y = js::tointeger(j, 2);
            const auto radius = js::tofloat(j, 3);
            const auto color = types::tocolor(j, 4);
            DrawCircle(x, y, radius, color);
            js::pushundefined(j);
        },
        "__muenGraphicsCircle",
        4
    );

    define_global_function(
        j,
        [](js::State *j) -> void {
            const auto x = js::tointeger(j, 1);
            const auto y = js::tointeger(j, 2);
            const auto width = js::tointeger(j, 3);
            const auto height = js::tointeger(j, 4);
            const auto color = types::tocolor(j, 5);
            DrawRectangle(x, y, width, height, color);
            js::pushundefined(j);
        },
        "__muenGraphicsRectangle",
        5
    );

    define_global_function(
        j,
        [](js::State *j) -> void {
            const auto position = types::tovector2(j, 1);
            const auto size = types::tovector2(j, 2);
            const auto color = types::tocolor(j, 3);
            DrawRectangleV(position, size, color);
            js::pushundefined(j);
        },
        "__muenGraphicsRectangleV",
        3
    );

    define_global_function(
        j,
        [](js::State *j) -> void {
            const auto rec = types::torectangle(j, 1);
            const auto color = types::tocolor(j, 2);
            DrawRectangleRec(rec, color);
            js::pushundefined(j);
        },
        "__muenGraphicsRectangleRec",
        2
    );

    define_global_function(
        j,
        [](js::State *j) -> void {
            const auto rec = types::torectangle(j, 1);
            const auto origin = types::tovector2(j, 2);
            const auto rotation = static_cast<float>(js::tonumber(j, 3));
            const auto color = types::tocolor(j, 4);
            DrawRectangleRec(rec, color);
            js::pushundefined(j);
        },
        "__muenGraphicsRectanglePro",
        4
    );

    define_global_function(
        j,
        [](js::State *j) -> void {
            const auto camera = types::tocamera(j, 1);
            BeginMode2D(camera);
            js::pushundefined(j);
        },
        "__muenGraphicsBeginCameraMode",
        1
    );

    define_global_function(
        j,
        [](js::State *j) -> void {
            EndMode2D();
            js::pushundefined(j);
        },
        "__muenGraphicsEndCameraMode"
    );

    define_global_function(
        j,
        [](js::State *j) -> void {
            const auto texture = types::totexture(j, 1);
            const auto x = js::tointeger(j, 2);
            const auto y = js::tointeger(j, 3);
            const auto tint = types::tocolor(j, 4);
            DrawTexture(texture, x, y, tint);
            js::pushundefined(j);
        },
        "__muenGraphicsTexture",
        5
    );

    define_global_function(
        j,
        [](js::State *j) -> void {
            const auto texture = types::totexture(j, 1);
            const auto position = types::tovector2(j, 2);
            const auto tint = types::tocolor(j, 3);
            DrawTextureV(texture, position, tint);
            js::pushundefined(j);
        },
        "__muenGraphicsTextureV",
        4
    );

    define_global_function(
        j,
        [](js::State *j) -> void {
            const auto texture = types::totexture(j, 1);
            const auto position = types::tovector2(j, 2);
            const auto rotation = js::tofloat(j, 3);
            const auto scale = js::tofloat(j, 4);
            const auto tint = types::tocolor(j, 5);
            DrawTextureEx(texture, position, rotation, scale, tint);
            js::pushundefined(j);
        },
        "__muenGraphicsTextureEx",
        6
    );

    define_global_function(
        j,
        [](js::State *j) -> void {
            const auto texture = types::totexture(j, 1);
            const auto source = types::torectangle(j, 2);
            const auto position = types::tovector2(j, 3);
            const auto tint = types::tocolor(j, 4);
            DrawTextureRec(texture, source, position, tint);
            js::pushundefined(j);
        },
        "__muenGraphicsTextureRec",
        5
    );

    define_global_function(
        j,
        [](js::State *j) -> void {
            const auto texture = types::totexture(j, 1);
            const auto source = types::torectangle(j, 2);
            const auto dest = types::torectangle(j, 3);
            const auto origin = types::tovector2(j, 4);
            const auto rotation = js::tofloat(j, 5);
            const auto tint = types::tocolor(j, 6);
            DrawTexturePro(texture, source, dest, origin, rotation, tint);
            js::pushundefined(j);
        },
        "__muenGraphicsTexturePro",
        7
    );

    define_global_function(
        j,
        [](js::State *j) -> void {
            const auto texture = types::totexture(j, 1);
            const auto nPatch = types::tonpatch(j, 2);
            const auto dest = types::torectangle(j, 3);
            const auto origin = types::tovector2(j, 4);
            const auto rotation = js::tofloat(j, 5);
            const auto tint = types::tocolor(j, 6);
            DrawTextureNPatch(texture, nPatch, dest, origin, rotation, tint);
            js::pushundefined(j);
        },
        "__muenGraphicsTextureNPatch",
        7
    );
}

} // namespace muen::bindings::graphics
