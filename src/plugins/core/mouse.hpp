#pragma once

#include <unordered_map>

#include <plugins/core/vector2.hpp>
#include <quickjs.hpp>
#include <raylib.hpp>

namespace glint::js {

static const auto BUTTON_MAP = std::unordered_map<std::string, MouseButton> {
    {"left", MOUSE_BUTTON_LEFT},
    {"right", MOUSE_BUTTON_RIGHT},
    {"middle", MOUSE_BUTTON_MIDDLE},
    {"side", MOUSE_BUTTON_SIDE},
    {"extra", MOUSE_BUTTON_EXTRA},
    {"forward", MOUSE_BUTTON_FORWARD},
    {"back", MOUSE_BUTTON_BACK},
};

static const auto CURSOR_MAP = std::unordered_map<std::string, MouseCursor> {
    {"default", MOUSE_CURSOR_DEFAULT},
    {"arrow", MOUSE_CURSOR_ARROW},
    {"ibeam", MOUSE_CURSOR_IBEAM},
    {"crosshair", MOUSE_CURSOR_CROSSHAIR},
    {"pointingHand", MOUSE_CURSOR_POINTING_HAND},
    {"resizeEw", MOUSE_CURSOR_RESIZE_EW},
    {"resizeNs", MOUSE_CURSOR_RESIZE_NS},
    {"resizeNwse", MOUSE_CURSOR_RESIZE_NWSE},
    {"resizeNesw", MOUSE_CURSOR_RESIZE_NESW},
    {"resizeAll", MOUSE_CURSOR_RESIZE_ALL},
    {"notAllowed", MOUSE_CURSOR_NOT_ALLOWED},
};

template<>
inline auto convert_from_js<MouseButton>(const Value& val) noexcept -> JSResult<MouseButton> {
    auto str = convert_from_js<std::string>(val);
    if (!str) return str.error();
    if (auto it = BUTTON_MAP.find(*str); it != BUTTON_MAP.end()) {
        return it->second;
    } else {
        return JSError::type_error(val.ctx(), fmt::format("Unknown mouse button `{}`", *str));
    }
}

template<>
inline auto convert_from_js<MouseCursor>(const Value& val) noexcept -> JSResult<MouseCursor> {
    auto str = convert_from_js<std::string>(val);
    if (!str) return str.error();
    if (auto it = CURSOR_MAP.find(*str); it != CURSOR_MAP.end()) {
        return it->second;
    } else {
        return JSError::type_error(val.ctx(), fmt::format("Unknown mouse cursor `{}`", *str));
    }
}

} // namespace glint::js

namespace glint::plugins::core {

using namespace js;

class JSMouse: public JSClass<JSMouse> {
  public:
    // Button state methods
    [[nodiscard]] auto is_button_pressed(MouseButton button) const noexcept -> bool {
        return IsMouseButtonPressed(button);
    }

    [[nodiscard]] auto is_button_down(MouseButton button) const noexcept -> bool { return IsMouseButtonDown(button); }

    [[nodiscard]] auto is_button_released(MouseButton button) const noexcept -> bool {
        return IsMouseButtonReleased(button);
    }

    [[nodiscard]] auto is_button_up(MouseButton button) const noexcept -> bool { return IsMouseButtonUp(button); }

    [[nodiscard]] auto get_x() const noexcept -> int { return GetMouseX(); }

    auto set_x(int x) noexcept -> void {
        auto y = GetMouseY();
        SetMousePosition(x, y);
    }

    [[nodiscard]] auto get_y() const noexcept -> int { return GetMouseY(); }

    auto set_y(int y) noexcept -> void {
        auto x = GetMouseX();
        SetMousePosition(x, y);
    }

    auto get_position(JSContext *ctx) const noexcept -> JSValue {
        auto pos = GetMousePosition();
        return JSVector2::create_instance(ctx, pos.x, pos.y);
    }

    auto set_position(Vector2 pos) noexcept -> void { SetMousePosition(int(pos.x), int(pos.y)); }

    auto get_delta(JSContext *ctx) const noexcept -> JSValue {
        auto delta = ::GetMouseDelta();
        return JSVector2::create_instance(ctx, delta.x, delta.y);
    }

    auto set_cursor(MouseCursor cursor) noexcept -> void { SetMouseCursor(cursor); }

    [[nodiscard]] auto get_visible() const noexcept -> bool { return !IsCursorHidden(); }

    auto set_visible(bool visible) noexcept -> void {
        if (visible) ShowCursor();
        else HideCursor();
    }

    auto set_enabled(bool enabled) noexcept -> void {
        if (enabled) EnableCursor();
        else DisableCursor();
    }

    [[nodiscard]] auto get_is_on_screen() const noexcept -> bool { return IsCursorOnScreen(); }

  public: // JSClass implementation
    constexpr static auto class_name = "Mouse";

    inline static auto static_properties = PropertyList {};

    inline static auto instance_properties = PropertyList {
        export_method<&JSMouse::is_button_pressed>("isButtonPressed"),
        export_method<&JSMouse::is_button_down>("isButtonDown"),
        export_method<&JSMouse::is_button_released>("isButtonReleased"),
        export_method<&JSMouse::is_button_up>("isButtonUp"),
        export_getset<&JSMouse::get_x, &JSMouse::set_x>("x"),
        export_getset<&JSMouse::get_y, &JSMouse::set_y>("y"),
        export_getset<&JSMouse::get_position, &JSMouse::set_position>("position"),
        export_get_only<&JSMouse::get_delta>("delta"),
        export_set_only<&JSMouse::set_cursor>("cursor"),
        export_getset<&JSMouse::get_visible, &JSMouse::set_visible>("visible"),
        export_set_only<&JSMouse::set_enabled>("enabled"),
        export_get_only<&JSMouse::get_is_on_screen>("isOnScreen"),
    };

    auto initialize() noexcept {}
};

inline auto mouse_module(JSContext *ctx) -> JSModuleDef * {
    auto m = JS_NewCModule(ctx, "@glint/core/mouse", [](auto ctx, auto m) -> int {
        JSMouse::define(ctx);
        auto instance = JSMouse::create_instance(ctx);
        JS_SetModuleExport(ctx, m, "mouse", JS_DupValue(ctx, instance));
        JS_SetModuleExport(ctx, m, "default", instance);

        return 0;
    });

    JS_AddModuleExport(ctx, m, "mouse");
    JS_AddModuleExport(ctx, m, "default");

    return m;
}
} // namespace glint::plugins::core
