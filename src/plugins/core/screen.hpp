#pragma once

#include <plugins/core/vector2.hpp>

#include <quickjs.h>
#include <raylib.h>
#include <quickjs.hpp>

namespace glint::plugins::core {

using namespace js;

class JSScreen: public JSClass<JSScreen> {
  public:
    [[nodiscard]] auto get_dt() const noexcept -> float { return GetFrameTime(); }

    [[nodiscard]] auto get_time() const noexcept -> double { return GetTime(); }

    [[nodiscard]] auto get_width() const noexcept -> int { return GetScreenWidth(); }

    [[nodiscard]] auto get_height() const noexcept -> int { return GetScreenHeight(); }

  public: // JSClass implementation
    constexpr static auto class_name = "Screen";

    inline static auto static_properties = PropertyList {};

    inline static auto instance_properties = PropertyList {
        export_get_only<&JSScreen::get_dt>("dt"),
        export_get_only<&JSScreen::get_time>("time"),
        export_get_only<&JSScreen::get_width>("width"),
        export_get_only<&JSScreen::get_height>("height"),
    };

    auto initialize() noexcept {}
};

inline auto screen_module(::JSContext *ctx) -> ::JSModuleDef * {
    auto m = ::JS_NewCModule(ctx, "@glint/core/screen", [](auto ctx, auto m) -> int {
        JSScreen::define(ctx);
        auto instance = JSScreen::create_instance(ctx);
        JS_SetModuleExport(ctx, m, "screen", JS_DupValue(ctx, instance));
        JS_SetModuleExport(ctx, m, "default", instance);

        return 0;
    });

    JS_AddModuleExport(ctx, m, "screen");
    JS_AddModuleExport(ctx, m, "default");

    return m;
}

} // namespace glint::plugins::core
