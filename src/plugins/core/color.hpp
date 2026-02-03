#pragma once

#include <quickjs.hpp>
#include <raylib.hpp>
#include <charconv>

namespace glint::plugins::core {

using namespace js;

class JSColor: public JSClass<JSColor> {
  public:
    static auto from_hex(JSContext *ctx, std::string hex) noexcept -> JSValue {
        if (!hex.starts_with('#')) {
            return jsthrow(JSError::type_error(ctx, "Hex code must start with '#'"));
        }
        hex = hex.substr(1);

        if (hex.length() != 6 && hex.length() != 8) {
            return jsthrow(JSError::type_error(ctx, "Hex code must be in format #rrggbb or #rrggbbaa"));
        }

        auto parse_hex_pair = [](const std::string& str, size_t pos) -> unsigned char {
            unsigned int value = 0;
            std::from_chars(str.data() + pos, str.data() + pos + 2, value, 16);
            return static_cast<unsigned char>(value);
        };

        unsigned char r = parse_hex_pair(hex, 0);
        unsigned char g = parse_hex_pair(hex, 2);
        unsigned char b = parse_hex_pair(hex, 4);
        unsigned char a = hex.length() == 8 ? parse_hex_pair(hex, 6) : 255;

        return create_instance(ctx, r, g, b, a);
    }

    [[nodiscard]] auto get_color() const noexcept -> ::Color { return _color; }

    [[nodiscard]] auto get_r() const noexcept -> unsigned char { return _color.r; }

    auto set_r(unsigned char val) noexcept -> void { _color.r = val; }

    [[nodiscard]] auto get_g() const noexcept -> unsigned char { return _color.g; }

    auto set_g(unsigned char val) noexcept -> void { _color.g = val; }

    [[nodiscard]] auto get_b() const noexcept -> unsigned char { return _color.b; }

    auto set_b(unsigned char val) noexcept -> void { _color.b = val; }

    [[nodiscard]] auto get_a() const noexcept -> unsigned char { return _color.a; }

    auto set_a(unsigned char val) noexcept -> void { _color.a = val; }

    [[nodiscard]] auto to_string() const noexcept -> std::string { return fmt::format("{}", _color); }

    // JSClass implementation

    constexpr static auto class_name = "Color";

    inline static auto static_properties = PropertyList {
        export_static_method<&JSColor::from_hex>("fromHex"),
    };

    inline static auto instance_properties = PropertyList {
        export_getset<&JSColor::get_r, &JSColor::set_r>("r"),
        export_getset<&JSColor::get_g, &JSColor::set_g>("g"),
        export_getset<&JSColor::get_b, &JSColor::set_b>("b"),
        export_getset<&JSColor::get_a, &JSColor::set_a>("a"),
        export_method<&JSColor::to_string>("toString"),
    };

    auto initialize(unsigned char r, unsigned char g, unsigned char b, std::optional<unsigned char> a) noexcept {
        _color.r = r;
        _color.g = g;
        _color.b = b;
        _color.a = a.value_or(255);
    }

  private:
    ::Color _color {};
};

inline auto color_module(JSContext *ctx) -> JSModuleDef * {
    auto m = JS_NewCModule(ctx, "@glint/core/Color", [](auto ctx, auto m) -> int {
        auto ctor = JSColor::define(ctx).take();
        JS_SetModuleExport(ctx, m, "Color", JS_DupValue(ctx, ctor));
        JS_SetModuleExport(ctx, m, "default", ctor);
        return 0;
    });

    JS_AddModuleExport(ctx, m, "Color");
    JS_AddModuleExport(ctx, m, "default");
    return m;
}

} // namespace glint::plugins::core

namespace glint::js {

using plugins::core::JSColor;

template<>
inline auto convert_from_js<JSColor *>(const Value& val) noexcept -> JSResult<JSColor *> {
    return JSColor::get_instance(val);
}

template<>
inline auto convert_from_js<Color>(const Value& val) noexcept -> JSResult<Color> {
    if (auto r = convert_from_js<JSColor *>(val)) return (*r)->get_color();

    auto c = ::Color {};
    auto obj = Object::from_value(val);
    if (!obj) return obj.error();

    if (auto v = obj->at<unsigned char>("r")) c.r = *v;
    else return v.error();
    if (auto v = obj->at<unsigned char>("g")) c.g = *v;
    else return v.error();
    if (auto v = obj->at<unsigned char>("b")) c.b = *v;
    else return v.error();
    if (auto v = obj->at<unsigned char>("a")) c.a = *v;
    else return v.error();

    return c;
}

} // namespace glint::js
