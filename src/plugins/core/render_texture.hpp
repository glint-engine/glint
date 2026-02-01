#pragma once

#include <quickjs.hpp>
#include <raylib.hpp>

namespace glint::plugins::core {

using namespace js;

class JSRenderTexture: public JSClass<JSRenderTexture> {
  public:
    [[nodiscard]] auto get_texture() const noexcept -> const rl::RenderTexture& { return _texture; }

    [[nodiscard]] auto get_width() const noexcept -> int { return _texture.texture.width; }

    auto set_width(int width) noexcept -> void {
        auto height = _texture.texture.height;
        _texture = rl::RenderTexture::load(width, height);
    }

    [[nodiscard]] auto get_height() const noexcept -> int { return _texture.texture.height; }

    auto set_height(int height) noexcept -> void {
        auto width = _texture.texture.width;
        _texture = rl::RenderTexture::load(width, height);
    }

    [[nodiscard]] auto to_string() const noexcept -> std::string { return fmt::format("{}", _texture); }

  public: // JSClass implementation
    constexpr static auto class_name = "RenderTexture";

    inline static auto static_properties = PropertyList {};

    inline static auto instance_properties = PropertyList {
        export_getset<&JSRenderTexture::get_width, &JSRenderTexture::set_width>("width"),
        export_getset<&JSRenderTexture::get_height, &JSRenderTexture::set_height>("height"),
        export_method<&JSRenderTexture::to_string>("toString"),
    };

    auto initialize(int width, int height) noexcept {
        _texture = rl::RenderTexture::load(width, height);
        if (!IsRenderTextureValid(_texture)) {
            SPDLOG_ERROR("Could not create RenderTexture with dimensions {}x{}", width, height);
        }
    }

  private:
    rl::RenderTexture _texture {};
};

inline auto render_texture_module(JSContext *ctx) -> JSModuleDef * {
    auto m = JS_NewCModule(ctx, "@glint/core/RenderTexture", [](auto ctx, auto m) -> int {
        auto ctor = JSRenderTexture::define(ctx).take();
        JS_SetModuleExport(ctx, m, "RenderTexture", JS_DupValue(ctx, ctor));
        JS_SetModuleExport(ctx, m, "default", ctor);
        return 0;
    });

    JS_AddModuleExport(ctx, m, "RenderTexture");
    JS_AddModuleExport(ctx, m, "default");
    return m;
}

} // namespace glint::plugins::core

namespace glint::js {

using plugins::core::JSRenderTexture;

template<>
inline auto convert_from_js<JSRenderTexture *>(const Value& val) noexcept -> JSResult<JSRenderTexture *> {
    return JSRenderTexture::get_instance(val);
}

template<>
inline auto convert_from_js<const rl::RenderTexture *>(const Value& val) noexcept
    -> JSResult<const rl::RenderTexture *> {
    if (auto r = convert_from_js<JSRenderTexture *>(val)) return &(*r)->get_texture();
    else return r.error();
}

} // namespace glint::js
