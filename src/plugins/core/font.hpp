#pragma once

#include <filesystem>
#include <variant>

#include <engine.hpp>
#include <quickjs.hpp>
#include <raylib.hpp>

namespace glint::plugins::core {

using namespace js;

struct JSFontOptions {
    std::optional<std::filesystem::path> path {};
    std::optional<std::string> name {};
    std::optional<int> font_size {};
    std::optional<std::vector<int>> codepoints {};
};

struct JSFontLoadByName {
    std::string name;
};

struct JSFontLoadByParams {
    std::filesystem::path path;
    std::string name;
    int font_size;
    std::optional<std::vector<int>> codepoints;
};

using JSFontLoadMode = std::variant<std::monostate, JSFontLoadByName, JSFontLoadByParams>;

class JSFont: public JSClass<JSFont> {
  public:
    [[nodiscard]] auto get_font(JSContext *ctx) const noexcept -> const rl::Font& {
        auto& e = Engine::get(ctx);
        return e.font_store().borrow(_handle);
    }

    [[nodiscard]] auto get_valid(JSContext *ctx) const noexcept -> bool {
        auto& font = get_font(ctx);
        return IsFontValid(font);
    }

    [[nodiscard]] auto to_string(JSContext *ctx) const noexcept -> std::string {
        auto& font = get_font(ctx);
        return fmt::format("{}", font);
    }

  private:
    ResourceStore<FontData>::Handle _handle {};

    static auto custom_constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) -> JSValue;

    static auto custom_finalizer(JSRuntime *rt, JSValueConst val) noexcept -> void;

    static auto read_font_options_from_args(JSContext *ctx, int argc, JSValueConst *argv) noexcept
        -> JSResult<JSFontLoadMode>;

  public: // JSClass implementation
    constexpr static auto class_name = "Font";

    constexpr static auto static_properties = PropertyList {};

    inline static auto instance_properties = PropertyList {
        export_get_only<&JSFont::get_valid>("valid"),
        export_method<&JSFont::to_string>("toString"),
    };

    // TODO: This needs custom constructor handling due to complex argument parsing
    // The initialize signature doesn't match the actual JS constructor args
    // Need to either:
    // 1. Extend JSClass to support custom constructor functions
    // 2. Add variant support to handle string | FontOptions overloading
    // 3. Make initialize() take a variant parameter
    auto initialize(ResourceStore<FontData>::Handle handle) noexcept { _handle = handle; }

    // Custom constructor - TODO: Integrate this into JSClass framework
    constexpr static JSCFunction *constructor = &custom_constructor;

    // Custom finalizer to release the font handle
    constexpr static JSClassFinalizer *class_finalizer = &custom_finalizer;
};

inline auto font_module(JSContext *ctx) -> JSModuleDef * {
    auto m = JS_NewCModule(ctx, "@glint/core/Font", [](auto ctx, auto m) -> int {
        auto ctor = JSFont::define(ctx).take();
        JS_SetModuleExport(ctx, m, "Font", JS_DupValue(ctx, ctor));
        JS_SetModuleExport(ctx, m, "default", ctor);
        return 0;
    });

    JS_AddModuleExport(ctx, m, "Font");
    JS_AddModuleExport(ctx, m, "default");
    return m;
}

} // namespace glint::plugins::core

namespace glint::js {

using plugins::core::JSFont;
using plugins::core::JSFontOptions;

template<>
inline auto convert_from_js<JSFont *>(const Value& val) noexcept -> JSResult<JSFont *> {
    return JSFont::get_instance(val);
}

template<>
inline auto convert_from_js<const rl::Font *>(const Value& val) noexcept -> JSResult<const rl::Font *> {
    if (auto r = convert_from_js<JSFont *>(val)) return &(*r)->get_font(val.ctx());
    return JSError::type_error(val.ctx(), "Not an instance of Font");
}

template<>
inline auto convert_from_js<JSFontOptions>(const Value& val) noexcept -> JSResult<JSFontOptions> try {
    auto o = JSFontOptions {};
    auto obj = Object::from_value(val);
    if (!obj) return obj.error();

    if (auto v = obj->at<std::optional<std::string>>("path")) o.path = *v;
    else return v.error();
    if (auto v = obj->at<std::optional<std::string>>("name")) o.name = *v;
    else return v.error();
    if (auto v = obj->at<std::optional<int>>("fontSize")) o.font_size = *v;
    else return v.error();
    if (auto v = obj->at<std::optional<std::vector<int>>>("codepoints")) o.codepoints = *v;
    else return v.error();

    return o;
} catch (std::exception& e) {
    return JSError::plain_error(val.ctx(), fmt::format("Unexpected C++ exception: {}", e.what()));
}

} // namespace glint::js
