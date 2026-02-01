#pragma once

#include <filesystem>
#include <variant>

#include <engine.hpp>
#include <plugins/core/rectangle.hpp>
#include <quickjs.hpp>
#include <raylib.hpp>

namespace glint::plugins::core {

using namespace js;

struct JSTextureOptions {
    std::optional<std::filesystem::path> path {};
    std::optional<std::string> name {};
};

struct JSTextureLoadByName {
    std::string name;
};

struct JSTextureLoadByParams {
    std::filesystem::path path;
    std::string name;
};

using JSTextureLoadMode = std::variant<std::monostate, JSTextureLoadByName, JSTextureLoadByParams>;

class JSTexture: public JSClass<JSTexture> {
  public:
    auto get_texture(JSContext *ctx) const noexcept -> const rl::Texture& {
        auto& e = Engine::get(ctx);
        return e.texture_store().borrow(_handle);
    }

    auto get_source(JSContext *ctx) const noexcept -> JSValue {
        auto& tex = get_texture(ctx);
        return JSRectangle::create_instance(
            ctx,
            0.0f,
            0.0f,
            static_cast<float>(tex.width),
            static_cast<float>(tex.height)
        );
    }

    auto to_string(JSContext *ctx) const noexcept -> std::string {
        auto& tex = get_texture(ctx);
        return fmt::format("{}", tex);
    }

    auto unload(JSContext *ctx) noexcept -> void {
        auto& e = Engine::get(ctx);
        e.texture_store().release(_handle);
    }

  private:
    ResourceStore<TextureData>::Handle _handle {};

    static auto custom_constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) noexcept
        -> JSValue;

    static auto custom_finalizer(JSRuntime *rt, JSValueConst val) noexcept -> void;

    static auto read_texture_options_from_args(JSContext *ctx, int argc, JSValueConst *argv) noexcept
        -> JSResult<JSTextureLoadMode>;

  public: // JSClass implementation
    constexpr static auto class_name = "Texture";

    inline static auto static_properties = PropertyList {};

    inline static auto instance_properties = PropertyList {
        export_get_only<&JSTexture::get_source>("source"),
        export_method<&JSTexture::unload>("unload"),
        export_method<&JSTexture::to_string>("toString"),
    };

    // TODO: This needs custom constructor handling due to complex argument parsing.
    //       The initialize signature doesn't match the actual JS constructor args
    //       Need to either:
    //       1. Extend JSClass to support custom constructor functions
    //       2. Add variant support to handle string | TextureOptions overloading
    //       3. Make initialize() take a variant parameter
    auto initialize(ResourceStore<TextureData>::Handle handle) noexcept { _handle = handle; }

    //  TODO: Integrate this into JSClass framework
    constexpr static JSCFunction *constructor = &custom_constructor;

    //  TODO: Integrate this into JSClass framework
    constexpr static JSClassFinalizer *class_finalizer = &custom_finalizer;
};

inline auto texture_module(JSContext *ctx) -> JSModuleDef * {
    auto m = JS_NewCModule(ctx, "@glint/core/Texture", [](auto ctx, auto m) -> int {
        auto ctor = JSTexture::define(ctx).take();
        JS_SetModuleExport(ctx, m, "Texture", JS_DupValue(ctx, ctor));
        JS_SetModuleExport(ctx, m, "default", ctor);
        return 0;
    });

    JS_AddModuleExport(ctx, m, "Texture");
    JS_AddModuleExport(ctx, m, "default");
    return m;
}

} // namespace glint::plugins::core

namespace glint::js {

using plugins::core::JSTexture;
using plugins::core::JSTextureOptions;

template<>
inline auto convert_from_js<JSTexture *>(const Value& val) noexcept -> JSResult<JSTexture *> {
    return JSTexture::get_instance(val);
}

template<>
inline auto convert_from_js<const rl::Texture *>(const Value& val) noexcept -> JSResult<const rl::Texture *> {
    if (auto r = convert_from_js<JSTexture *>(val)) return &(*r)->get_texture(val.ctx());
    return JSError::type_error(val.ctx(), "Not an instance of Texture");
}

template<>
inline auto convert_from_js<JSTextureOptions>(const Value& val) noexcept -> JSResult<JSTextureOptions> try {
    auto o = JSTextureOptions {};
    auto obj = Object::from_value(val);
    if (!obj) return obj.error();

    if (auto v = obj->at<std::optional<std::string>>("path"); v) o.path = *v;
    else return v.error();
    if (auto v = obj->at<std::optional<std::string>>("name"); v) o.name = *v;
    else return v.error();

    return o;
} catch (std::exception& e) {
    return JSError::plain_error(val.ctx(), fmt::format("Unexpected C++ exception: {}", e.what()));
}

} // namespace glint::js
