#pragma once

#include <plugins/core/vector2.hpp>

#include <quickjs.hpp>
#include <raylib.hpp>

namespace glint::plugins::core {

using namespace js;

class JSCamera: public JSClass<JSCamera> {
  public:
    static auto default_camera(JSContext *ctx, JSValueConst this_val) noexcept -> JSValue {
        return JSCamera::create_instance_this(
            borrow(ctx, this_val),
            Vector2 {0.0f, 0.0f},
            Vector2 {0.0f, 0.0f},
            0.0f,
            1.0f
        );
    }

    [[nodiscard]] auto get_camera() const noexcept -> ::Camera2D { return _camera; }

    auto get_offset(JSContext *ctx) const noexcept -> JSValue {
        return JSVector2::create_instance(ctx, _camera.offset.x, _camera.offset.y);
    }

    auto set_offset(Vector2 offset) noexcept -> void { _camera.offset = offset; }

    auto get_target(JSContext *ctx) const noexcept -> JSValue {
        return JSVector2::create_instance(ctx, _camera.target.x, _camera.target.y);
    }

    auto set_target(Vector2 target) noexcept -> void { _camera.target = target; }

    [[nodiscard]] auto get_rotation() const noexcept -> float { return _camera.rotation; }

    auto set_rotation(float rotation) noexcept -> void { _camera.rotation = rotation; }

    [[nodiscard]] auto get_zoom() const noexcept -> float { return _camera.zoom; }

    auto set_zoom(float zoom) noexcept -> void { _camera.zoom = zoom; }

    [[nodiscard]] auto to_string() const noexcept -> std::string { return fmt::format("{}", _camera); }

  public: // JSClass implementation
    constexpr static auto class_name = "Camera";

    inline static auto static_properties = PropertyList {
        export_static_method<&JSCamera::default_camera>("default"),
    };

    inline static auto instance_properties = PropertyList {
        export_getset<&JSCamera::get_offset, &JSCamera::set_offset>("offset"),
        export_getset<&JSCamera::get_target, &JSCamera::set_target>("target"),
        export_getset<&JSCamera::get_rotation, &JSCamera::set_rotation>("rotation"),
        export_getset<&JSCamera::get_zoom, &JSCamera::set_zoom>("zoom"),
        export_method<&JSCamera::to_string>("toString"),
    };

    auto initialize(Vector2 offset, Vector2 target, float rotation, float zoom) noexcept {
        _camera.offset = offset;
        _camera.target = target;
        _camera.rotation = rotation;
        _camera.zoom = zoom;
    }

  private:
    ::Camera2D _camera {};
};

inline auto camera_module(JSContext *ctx) -> JSModuleDef * {
    auto m = JS_NewCModule(ctx, "@glint/core/Camera", [](auto ctx, auto m) -> int {
        auto ctor = JSCamera::define(ctx).take();
        JS_SetModuleExport(ctx, m, "Camera", JS_DupValue(ctx, ctor));
        JS_SetModuleExport(ctx, m, "default", ctor);
        return 0;
    });

    JS_AddModuleExport(ctx, m, "Camera");
    JS_AddModuleExport(ctx, m, "default");
    return m;
}

} // namespace glint::plugins::core

namespace glint::js {

using plugins::core::JSCamera;

template<>
inline auto convert_from_js<JSCamera *>(const Value& val) noexcept -> JSResult<JSCamera *> {
    return JSCamera::get_instance(val);
}

template<>
inline auto convert_from_js<Camera2D>(const Value& val) noexcept -> JSResult<Camera2D> {
    if (auto r = convert_from_js<JSCamera *>(val)) return (*r)->get_camera();

    auto cam = ::Camera2D {};
    auto obj = Object::from_value(val);
    if (!obj) return obj.error();

    if (auto v = obj->at<Vector2>("offset")) cam.offset = *v;
    else return v.error();
    if (auto v = obj->at<Vector2>("target")) cam.target = *v;
    else return v.error();
    if (auto v = obj->at<float>("rotation")) cam.rotation = *v;
    else return v.error();
    if (auto v = obj->at<float>("zoom")) cam.zoom = *v;
    else return v.error();

    return cam;
}

} // namespace glint::js
