#pragma once

#include <plugins/core/rectangle.hpp>

#include <quickjs.hpp>
#include <raylib.hpp>

namespace glint::plugins::core {

using namespace js;

class JSNPatch: public JSClass<JSNPatch> {
  public:
    [[nodiscard]] auto get_npatch() const noexcept -> ::NPatchInfo { return _npatch; }

    auto get_source(JSContext *ctx) const noexcept -> JSValue {
        return JSRectangle::create_instance(
            ctx,
            _npatch.source.x,
            _npatch.source.y,
            _npatch.source.width,
            _npatch.source.height
        );
    }

    auto set_source(Rectangle source) noexcept -> void { _npatch.source = source; }

    [[nodiscard]] auto get_left() const noexcept -> int { return _npatch.left; }

    auto set_left(int left) noexcept -> void { _npatch.left = left; }

    [[nodiscard]] auto get_top() const noexcept -> int { return _npatch.top; }

    auto set_top(int top) noexcept -> void { _npatch.top = top; }

    [[nodiscard]] auto get_right() const noexcept -> int { return _npatch.right; }

    auto set_right(int right) noexcept -> void { _npatch.right = right; }

    [[nodiscard]] auto get_bottom() const noexcept -> int { return _npatch.bottom; }

    auto set_bottom(int bottom) noexcept -> void { _npatch.bottom = bottom; }

    [[nodiscard]] auto get_layout() const noexcept -> int { return _npatch.layout; }

    auto set_layout(int layout) noexcept -> void { _npatch.layout = layout; }

    [[nodiscard]] auto to_string() const noexcept -> std::string { return fmt::format("{}", _npatch); }

  public: // JSClass implementation
    constexpr static auto class_name = "NPatch";

    inline static auto static_properties = PropertyList {};

    inline static auto instance_properties = PropertyList {
        export_getset<&JSNPatch::get_source, &JSNPatch::set_source>("source"),
        export_getset<&JSNPatch::get_left, &JSNPatch::set_left>("left"),
        export_getset<&JSNPatch::get_top, &JSNPatch::set_top>("top"),
        export_getset<&JSNPatch::get_right, &JSNPatch::set_right>("right"),
        export_getset<&JSNPatch::get_bottom, &JSNPatch::set_bottom>("bottom"),
        export_getset<&JSNPatch::get_layout, &JSNPatch::set_layout>("layout"),
        export_method<&JSNPatch::to_string>("toString"),
    };

    auto initialize(Rectangle source, int left, int top, int right, int bottom, int layout) noexcept {
        _npatch.source = source;
        _npatch.left = left;
        _npatch.top = top;
        _npatch.right = right;
        _npatch.bottom = bottom;
        _npatch.layout = layout;
    }

  private:
    ::NPatchInfo _npatch {};
};

inline auto npatch_module(JSContext *ctx) -> JSModuleDef * {
    auto m = JS_NewCModule(ctx, "@glint/core/NPatch", [](auto ctx, auto m) -> int {
        auto ctor = JSNPatch::define(ctx).take();
        JS_SetModuleExport(ctx, m, "NPatch", JS_DupValue(ctx, ctor));
        JS_SetModuleExport(ctx, m, "default", ctor);
        return 0;
    });

    JS_AddModuleExport(ctx, m, "NPatch");
    JS_AddModuleExport(ctx, m, "default");
    return m;
}

} // namespace glint::plugins::core

namespace glint::js {

using plugins::core::JSNPatch;

template<>
inline auto convert_from_js<JSNPatch *>(const Value& val) noexcept -> JSResult<JSNPatch *> {
    return JSNPatch::get_instance(val);
}

template<>
inline auto convert_from_js<NPatchInfo>(const Value& val) noexcept -> JSResult<NPatchInfo> {
    if (auto r = convert_from_js<JSNPatch *>(val)) return (*r)->get_npatch();

    auto np = ::NPatchInfo {};
    auto obj = Object::from_value(val);
    if (!obj) return obj.error();

    if (auto v = obj->at<Rectangle>("source")) np.source = *v;
    else return v.error();
    if (auto v = obj->at<int>("left")) np.left = *v;
    else return v.error();
    if (auto v = obj->at<int>("top")) np.top = *v;
    else return v.error();
    if (auto v = obj->at<int>("right")) np.right = *v;
    else return v.error();
    if (auto v = obj->at<int>("bottom")) np.bottom = *v;
    else return v.error();
    if (auto v = obj->at<int>("layout")) np.layout = *v;
    else return v.error();

    return np;
}

} // namespace glint::js
