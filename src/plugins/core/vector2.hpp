#pragma once

#include <quickjs.hpp>
#include <raylib.hpp>

namespace glint::plugins::core {

using namespace js;

static inline auto read_vector_from_args(not_null<JSContext *> js, int argc, JSValueConst *argv) noexcept
    -> js::JSResult<Vector2> {
    if (argc == 1) {
        const auto args = js::unpack_args<Vector2>(js, argc, argv);
        if (!args) return args.error();
        const auto [v] = *args;
        return v;
    } else if (argc == 2) {
        const auto args = js::unpack_args<float, float>(js, argc, argv);
        if (!args) return args.error();
        const auto [x, y] = *args;
        return Vector2 {.x = x, .y = y};
    } else {
        return js::JSError::range_error(js, fmt::format("No matching overload for {} arguments", argc));
    }
}

static inline auto read_vector_and_number_from_args(not_null<JSContext *> js, int argc, JSValueConst *argv) noexcept
    -> js::JSResult<std::tuple<Vector2, float>> {
    if (argc == 2) {
        const auto args = js::unpack_args<Vector2, float>(js, argc, argv);
        if (!args) return args.error();
        return *args;
    } else if (argc == 3) {
        const auto args = js::unpack_args<float, float, float>(js, argc, argv);
        if (!args) return args.error();
        const auto [x, y, a] = *args;
        return std::tuple {Vector2 {.x = x, .y = y}, a};
    } else {
        return js::JSError::range_error(js, fmt::format("No matching overload for {} arguments", argc));
    }
}

static inline auto read_two_vectors_from_args(not_null<JSContext *> js, int argc, JSValueConst *argv)
    -> js::JSResult<std::tuple<Vector2, Vector2>> {
    if (argc == 2) {
        const auto args = js::unpack_args<Vector2, Vector2>(js, argc, argv);
        if (!args) return args.error();
        return *args;
    } else if (argc == 4) {
        const auto args = js::unpack_args<float, float, float, float>(js, argc, argv);
        if (!args) return args.error();
        const auto [x1, y1, x2, y2] = *args;
        return std::tuple {Vector2 {.x = x1, .y = y1}, Vector2 {.x = x2, .y = y2}};
    } else {
        return js::JSError::range_error(js, fmt::format("No matching overload for {} arguments", argc));
    }
}

class JSVector2: public JSClass<JSVector2> {
  public:
    static auto zero(JSContext *ctx, JSValueConst this_val) noexcept -> JSValue {
        return JSVector2::create_instance_this(borrow(ctx, this_val), 0.0f, 0.0f);
    }

    static auto one(JSContext *ctx, JSValueConst this_val) noexcept -> JSValue {
        return JSVector2::create_instance_this(borrow(ctx, this_val), 1.0f, 1.0f);
    }

    [[nodiscard]] auto get_vec() const noexcept -> ::Vector2 { return _vec; }

    [[nodiscard]] auto get_x() const noexcept -> float { return _vec.x; }

    auto set_x(float val) noexcept -> void { _vec.x = val; }

    [[nodiscard]] auto get_y() const noexcept -> float { return _vec.y; }

    auto set_y(float val) noexcept -> void { _vec.y = val; }

    [[nodiscard]] auto get_length() const noexcept -> float { return ::Vector2Length(_vec); }

    [[nodiscard]] auto get_length_sqr() const noexcept -> float { return ::Vector2LengthSqr(_vec); }

    auto add(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) noexcept -> JSValue {
        auto v = read_vector_from_args(ctx, argc, argv);
        if (!v) return jsthrow(v.error());
        _vec = Vector2Add(_vec, *v);
        return JS_DupValue(ctx, this_val);
    }

    auto sub(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) noexcept -> JSValue {
        auto v = read_vector_from_args(ctx, argc, argv);
        if (!v) return jsthrow(v.error());
        _vec = Vector2Subtract(_vec, *v);
        return JS_DupValue(ctx, this_val);
    }

    auto mul(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) noexcept -> JSValue {
        auto v = read_vector_from_args(ctx, argc, argv);
        if (!v) return jsthrow(v.error());
        _vec = Vector2Multiply(_vec, *v);
        return JS_DupValue(ctx, this_val);
    }

    auto div(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) noexcept -> JSValue {
        auto v = read_vector_from_args(ctx, argc, argv);
        if (!v) return jsthrow(v.error());
        _vec = Vector2Divide(_vec, *v);
        return JS_DupValue(ctx, this_val);
    }

    auto reflect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) noexcept -> JSValue {
        auto normal = read_vector_from_args(ctx, argc, argv);
        if (!normal) return jsthrow(normal.error());
        _vec = Vector2Reflect(_vec, *normal);
        return JS_DupValue(ctx, this_val);
    }

    auto min(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) noexcept -> JSValue {
        auto v = read_vector_from_args(ctx, argc, argv);
        if (!v) return jsthrow(v.error());
        _vec = Vector2Min(_vec, *v);
        return JS_DupValue(ctx, this_val);
    }

    auto max(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) noexcept -> JSValue {
        auto v = read_vector_from_args(ctx, argc, argv);
        if (!v) return jsthrow(v.error());
        _vec = Vector2Max(_vec, *v);
        return JS_DupValue(ctx, this_val);
    }

    auto add_num(JSContext *ctx, JSValueConst this_val, float num) noexcept -> JSValue {
        _vec.x += num;
        _vec.y += num;
        return JS_DupValue(ctx, this_val);
    }

    auto sub_num(JSContext *ctx, JSValueConst this_val, float num) noexcept -> JSValue {
        _vec.x -= num;
        _vec.y -= num;
        return JS_DupValue(ctx, this_val);
    }

    auto scale(JSContext *ctx, JSValueConst this_val, float s) noexcept -> JSValue {
        _vec = Vector2Scale(_vec, s);
        return JS_DupValue(ctx, this_val);
    }

    auto rotate(JSContext *ctx, JSValueConst this_val, float angle) noexcept -> JSValue {
        _vec = Vector2Rotate(_vec, angle);
        return JS_DupValue(ctx, this_val);
    }

    auto negate(JSContext *ctx, JSValueConst this_val) noexcept -> JSValue {
        _vec = Vector2Negate(_vec);
        return JS_DupValue(ctx, this_val);
    }

    auto norm(JSContext *ctx, JSValueConst this_val) noexcept -> JSValue {
        _vec = Vector2Normalize(_vec);
        return JS_DupValue(ctx, this_val);
    }

    auto invert(JSContext *ctx, JSValueConst this_val) noexcept -> JSValue {
        _vec = Vector2Invert(_vec);
        return JS_DupValue(ctx, this_val);
    }

    auto lerp(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) noexcept -> JSValue {
        auto args = read_vector_and_number_from_args(ctx, argc, argv);
        if (!args) return jsthrow(args.error());
        const auto [v, amount] = *args;
        _vec = Vector2Lerp(_vec, v, amount);
        return JS_DupValue(ctx, this_val);
    }

    auto move_towards(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) noexcept -> JSValue {
        auto args = read_vector_and_number_from_args(ctx, argc, argv);
        if (!args) return jsthrow(args.error());
        const auto [target, max_distance] = *args;
        _vec = Vector2MoveTowards(_vec, target, max_distance);
        return JS_DupValue(ctx, this_val);
    }

    auto refract(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) noexcept -> JSValue {
        auto args = read_vector_and_number_from_args(ctx, argc, argv);
        if (!args) return jsthrow(args.error());
        const auto [normal, r] = *args;
        _vec = Vector2Refract(_vec, normal, r);
        return JS_DupValue(ctx, this_val);
    }

    auto clamp(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) noexcept -> JSValue {
        auto args = read_two_vectors_from_args(ctx, argc, argv);
        if (!args) return jsthrow(args.error());
        const auto [min_val, max_val] = *args;
        _vec = Vector2Clamp(_vec, min_val, max_val);
        return JS_DupValue(ctx, this_val);
    }

    auto clamp_value(JSContext *ctx, JSValueConst this_val, float min_val, float max_val) noexcept -> JSValue {
        _vec = Vector2ClampValue(_vec, min_val, max_val);
        return JS_DupValue(ctx, this_val);
    }

    auto clone(JSContext *ctx) const noexcept -> JSValue { return create_instance(ctx, _vec.x, _vec.y); }

    auto dot(JSContext *ctx, JSValueConst, int argc, JSValueConst *argv) const noexcept -> JSResult<float> {
        auto v = read_vector_from_args(ctx, argc, argv);
        if (!v) return v.error();
        return Vector2DotProduct(_vec, *v);
    }

    auto distance(JSContext *ctx, JSValueConst, int argc, JSValueConst *argv) const noexcept -> JSResult<float> {
        auto v = read_vector_from_args(ctx, argc, argv);
        if (!v) return v.error();
        return Vector2Distance(_vec, *v);
    }

    auto distance_sqr(JSContext *ctx, JSValueConst, int argc, JSValueConst *argv) const noexcept -> JSResult<float> {
        auto v = read_vector_from_args(ctx, argc, argv);
        if (!v) return v.error();
        return Vector2DistanceSqr(_vec, *v);
    }

    auto angle(JSContext *ctx, JSValueConst, int argc, JSValueConst *argv) const noexcept -> JSResult<float> {
        auto v = read_vector_from_args(ctx, argc, argv);
        if (!v) return v.error();
        return Vector2Angle(_vec, *v);
    }

    auto line_angle(JSContext *ctx, JSValueConst, int argc, JSValueConst *argv) const noexcept -> JSResult<float> {
        auto end = read_vector_from_args(ctx, argc, argv);
        if (!end) return end.error();
        return Vector2LineAngle(_vec, *end);
    }

    auto equals(JSContext *ctx, JSValueConst, int argc, JSValueConst *argv) const noexcept -> JSResult<bool> {
        auto v = read_vector_from_args(ctx, argc, argv);
        if (!v) return v.error();
        return Vector2Equals(_vec, *v);
    }

    [[nodiscard]] auto to_string() const noexcept -> std::string { return fmt::format("{}", _vec); }

  public: // JSClass implementation
    constexpr static auto class_name = "Vector2";

    inline static auto static_properties = PropertyList {
        export_static_method<&JSVector2::zero>("zero"),
        export_static_method<&JSVector2::one>("one"),
    };

    inline static auto instance_properties = PropertyList {
        export_getset<&JSVector2::get_x, &JSVector2::set_x>("x"),
        export_getset<&JSVector2::get_y, &JSVector2::set_y>("y"),
        export_get_only<&JSVector2::get_length>("length"),
        export_get_only<&JSVector2::get_length_sqr>("lengthSqr"),
        export_method<&JSVector2::add>("add"),
        export_method<&JSVector2::add_num>("addNum"),
        export_method<&JSVector2::sub>("sub"),
        export_method<&JSVector2::sub_num>("subNum"),
        export_method<&JSVector2::dot>("dot"),
        export_method<&JSVector2::distance>("distance"),
        export_method<&JSVector2::distance_sqr>("distanceSqr"),
        export_method<&JSVector2::angle>("angle"),
        export_method<&JSVector2::line_angle>("lineAngle"),
        export_method<&JSVector2::scale>("scale"),
        export_method<&JSVector2::mul>("mul"),
        export_method<&JSVector2::negate>("negate"),
        export_method<&JSVector2::div>("div"),
        export_method<&JSVector2::norm>("norm"),
        export_method<&JSVector2::lerp>("lerp"),
        export_method<&JSVector2::reflect>("reflect"),
        export_method<&JSVector2::min>("min"),
        export_method<&JSVector2::max>("max"),
        export_method<&JSVector2::rotate>("rotate"),
        export_method<&JSVector2::move_towards>("moveTowards"),
        export_method<&JSVector2::invert>("invert"),
        export_method<&JSVector2::clamp>("clamp"),
        export_method<&JSVector2::clamp_value>("clampValue"),
        export_method<&JSVector2::equals>("equals"),
        export_method<&JSVector2::refract>("refract"),
        export_method<&JSVector2::clone>("clone"),
        export_method<&JSVector2::to_string>("toString"),
    };

    auto initialize(float x, float y) noexcept {
        _vec.x = x;
        _vec.y = y;
    }

  private:
    ::Vector2 _vec {};
};

inline auto vector2_module(JSContext *ctx) -> JSModuleDef * {
    auto m = JS_NewCModule(ctx, "@glint/core/Vector2", [](auto ctx, auto m) -> int {
        auto ctor = JSVector2::define(ctx).take();
        JS_SetModuleExport(ctx, m, "Vector2", JS_DupValue(ctx, ctor));
        JS_SetModuleExport(ctx, m, "default", ctor);
        return 0;
    });

    JS_AddModuleExport(ctx, m, "Vector2");
    JS_AddModuleExport(ctx, m, "default");
    return m;
}

} // namespace glint::plugins::core

namespace glint::js {

using plugins::core::JSVector2;

template<>
inline auto convert_from_js<JSVector2 *>(const Value& val) noexcept -> JSResult<JSVector2 *> {
    return JSVector2::get_instance(val);
}

template<>
inline auto convert_from_js<Vector2>(const Value& val) noexcept -> JSResult<Vector2> {
    if (auto r = convert_from_js<JSVector2 *>(val)) return (*r)->get_vec();

    auto vec = ::Vector2 {};
    auto obj = Object::from_value(val);
    if (!obj) return obj.error();

    if (auto v = obj->at<float>("x")) vec.x = *v;
    else return v.error();
    if (auto v = obj->at<float>("y")) vec.y = *v;
    else return v.error();

    return vec;
}

} // namespace glint::js
