#pragma once

#include <raylib.h>

#include <engine/plugin.hpp>
#include <plugins/core/camera.hpp>
#include <plugins/core/color.hpp>
#include <plugins/core/console.hpp>
#include <plugins/core/font.hpp>
#include <plugins/core/graphics.hpp>
#include <plugins/core/keyboard.hpp>
#include <plugins/core/mouse.hpp>
#include <plugins/core/npatch.hpp>
#include <plugins/core/rectangle.hpp>
#include <plugins/core/render_texture.hpp>
#include <plugins/core/screen.hpp>
#include <plugins/core/texture.hpp>
#include <plugins/core/vector2.hpp>

static constexpr char CORE_LOAD[] = {
#include "core_load.js.h"
};

static constexpr char CORE_MODULE[] = {
#include "core_module.js.h"
};

namespace glint::plugins::core {

auto plugin(JSContext *ctx) -> EnginePlugin {
    return EnginePlugin {
        .name = "core",
        .c_modules =
            {
                {"@glint/core/Camera", camera_module(ctx)},
                {"@glint/core/Color", color_module(ctx)},
                {"@glint/core/Font", font_module(ctx)},
                {"@glint/core/NPatch", npatch_module(ctx)},
                {"@glint/core/Rectangle", rectangle_module(ctx)},
                {"@glint/core/RenderTexture", render_texture_module(ctx)},
                {"@glint/core/Texture", texture_module(ctx)},
                {"@glint/core/Vector2", vector2_module(ctx)},
                {"@glint/core/console", console_module(ctx)},
                {"@glint/core/graphics", graphics_module(ctx)},
                {"@glint/core/keyboard", keyboard_module(ctx)},
                {"@glint/core/mouse", mouse_module(ctx)},
                {"@glint/core/screen", screen_module(ctx)},
            },

        .js_modules =
            {
                {"@glint/core", CORE_MODULE},
            },

        .load = [=]() -> Result<> {
            auto ret = JS_Eval(ctx, CORE_LOAD, sizeof(CORE_LOAD) - 1, "@glint/core/load.js", JS_EVAL_TYPE_MODULE);
            JS_FreeValue(ctx, ret);
            return {};
        },

        .draw = []() -> Result<> {
            ClearBackground(BLACK);
            return {};
        }
    };
}

} // namespace glint::plugins::core
