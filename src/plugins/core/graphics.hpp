#pragma once

#include <variant>
#include <optional>

#include <raylib.h>
#include <spdlog/spdlog.h>

#include <defer.hpp>
#include <plugins/core/vector2.hpp>
#include <quickjs.hpp>
#include <raylib.hpp>

namespace glint::js {

struct JSText {
    std::variant<std::string, int, std::vector<int>> text = {};
    float font_size = {};
    Vector2 position = {};
    Color color = {};
    std::optional<Vector2> origin = std::nullopt;
    std::optional<float> rotation = std::nullopt;
    std::optional<float> spacing = std::nullopt;
    std::optional<const rl::Font *> font = std::nullopt;
};

template<>
auto convert_from_js<JSText>(const Value& val) noexcept -> JSResult<JSText> {
    auto text = JSText {};
    auto obj = Object::from_value(val);
    if (!obj) return obj.error();

    if (auto t = obj->at<std::string>("text")) {
        text.text = std::move(*t);
    } else if (auto c = obj->at<int>("codepoint")) {
        text.text = *c;
    } else if (auto cs = obj->at<std::vector<int>>("codepoints")) {
        text.text = std::move(*cs);
    } else {
        return JSError::type_error(
            val.ctx(),
            "Text object must either have `text` (string), `codepoint` (number) or `codepoints` (number[]) property"
        );
    }

    if (auto v = obj->at<float>("fontSize")) text.font_size = *v;
    else return v.error();
    if (auto v = obj->at<Vector2>("position")) text.position = *v;
    else return v.error();
    if (auto v = obj->at<Color>("color")) text.color = *v;
    else return v.error();
    if (auto v = obj->at<std::optional<Vector2>>("origin")) text.origin = *v;
    else return v.error();
    if (auto v = obj->at<std::optional<float>>("rotation")) text.rotation = *v;
    else return v.error();
    if (auto v = obj->at<std::optional<float>>("spacing")) text.spacing = *v;
    else return v.error();
    if (auto v = obj->at<std::optional<const rl::Font *>>("font")) text.font = *v;
    else return v.error();

    return text;
}

} // namespace glint::js

namespace glint::plugins::core {

using namespace js;

class JSGraphics: public JSClass<JSGraphics> {
  public:
    auto clear(JSContext *ctx, JSValueConst this_val, Color color) noexcept -> JSValue {
        SPDLOG_TRACE("ClearBackground({})", color);
        ClearBackground(color);
        return JS_DupValue(ctx, this_val);
    }

    auto circle(JSContext *ctx, JSValueConst this_val, int x, int y, float radius, Color color) noexcept -> JSValue {
        SPDLOG_TRACE("DrawCircle({}, {}, {}, {})", x, y, radius, color);
        DrawCircle(x, y, radius, color);
        return JS_DupValue(ctx, this_val);
    }

    auto rectangle(JSContext *ctx, JSValueConst this_val, int x, int y, int width, int height, Color color) noexcept
        -> JSValue {
        SPDLOG_TRACE("DrawRectangle({}, {}, {}, {}, {})", x, y, width, height, color);
        DrawRectangle(x, y, width, height, color);
        return JS_DupValue(ctx, this_val);
    }

    auto rectangle_v(JSContext *ctx, JSValueConst this_val, Vector2 position, Vector2 size, Color color) noexcept
        -> JSValue {
        SPDLOG_TRACE("DrawRectangleV({}, {}, {})", position, size, color);
        DrawRectangleV(position, size, color);
        return JS_DupValue(ctx, this_val);
    }

    auto rectangle_rec(JSContext *ctx, JSValueConst this_val, Rectangle rec, Color color) noexcept -> JSValue {
        SPDLOG_TRACE("DrawRectangleRec({}, {})", rec, color);
        DrawRectangleRec(rec, color);
        return JS_DupValue(ctx, this_val);
    }

    auto rectangle_pro(
        JSContext *ctx,
        JSValueConst this_val,
        Rectangle rec,
        Vector2 origin,
        float rotation,
        Color color
    ) noexcept -> JSValue {
        SPDLOG_TRACE("DrawRectanglePro({}, {}, {}, {})", rec, origin, rotation, color);
        DrawRectanglePro(rec, origin, rotation, color);
        return JS_DupValue(ctx, this_val);
    }

    auto begin_camera_mode(JSContext *ctx, JSValueConst this_val, Camera2D camera) noexcept -> JSValue {
        SPDLOG_TRACE("BeginMode2D({})", camera);
        BeginMode2D(camera);
        return JS_DupValue(ctx, this_val);
    }

    auto end_camera_mode(JSContext *ctx, JSValueConst this_val) noexcept -> JSValue {
        SPDLOG_TRACE("EndMode2D()");
        EndMode2D();
        return JS_DupValue(ctx, this_val);
    }

    auto texture(JSContext *ctx, JSValueConst this_val, const rl::Texture *texture, int x, int y, Color tint) noexcept
        -> JSValue {
        SPDLOG_TRACE("DrawTexture({}, {}, {}, {})", *texture, x, y, tint);
        DrawTexture(*texture, x, y, tint);
        return JS_DupValue(ctx, this_val);
    }

    auto
    texture_v(JSContext *ctx, JSValueConst this_val, const rl::Texture *texture, Vector2 position, Color tint) noexcept
        -> JSValue {
        SPDLOG_TRACE("DrawTextureV({}, {}, {})", *texture, position, tint);
        DrawTextureV(*texture, position, tint);
        return JS_DupValue(ctx, this_val);
    }

    auto texture_ex(
        JSContext *ctx,
        JSValueConst this_val,
        const rl::Texture *texture,
        Vector2 position,
        float rotation,
        float scale,
        Color tint
    ) noexcept -> JSValue {
        SPDLOG_TRACE("DrawTextureEx({}, {}, {}, {}, {})", *texture, position, rotation, scale, tint);
        DrawTextureEx(*texture, position, rotation, scale, tint);
        return JS_DupValue(ctx, this_val);
    }

    auto texture_rec(
        JSContext *ctx,
        JSValueConst this_val,
        const rl::Texture *texture,
        Rectangle source,
        Vector2 position,
        Color tint
    ) noexcept -> JSValue {
        SPDLOG_TRACE("DrawTextureRec({}, {}, {}, {})", *texture, source, position, tint);
        DrawTextureRec(*texture, source, position, tint);
        return JS_DupValue(ctx, this_val);
    }

    auto texture_pro(
        JSContext *ctx,
        JSValueConst this_val,
        const rl::Texture *texture,
        Rectangle source,
        Rectangle dest,
        Vector2 origin,
        float rotation,
        Color tint
    ) noexcept -> JSValue {
        SPDLOG_TRACE("DrawTexturePro({}, {}, {}, {}, {}, {})", *texture, source, dest, origin, rotation, tint);
        DrawTexturePro(*texture, source, dest, origin, rotation, tint);
        return JS_DupValue(ctx, this_val);
    }

    auto texture_npatch(
        JSContext *ctx,
        JSValueConst this_val,
        const rl::Texture *texture,
        NPatchInfo npatch,
        Rectangle dest,
        Vector2 origin,
        float rotation,
        Color tint
    ) noexcept -> JSValue {
        SPDLOG_TRACE("DrawTextureNPatch({}, {}, {}, {}, {}, {});", *texture, npatch, dest, origin, rotation, tint);
        DrawTextureNPatch(*texture, npatch, dest, origin, rotation, tint);
        return JS_DupValue(ctx, this_val);
    }

    auto
    text(JSContext *ctx, JSValueConst this_val, std::string text, int x, int y, int font_size, Color color) noexcept
        -> JSValue {
        SPDLOG_TRACE("DrawText('{}', {}, {}, {}, {})", text, x, y, font_size, color);
        DrawText(text.c_str(), x, y, font_size, color);
        return JS_DupValue(ctx, this_val);
    }

    auto text_pro(JSContext *ctx, JSValueConst this_val, js::JSText text) noexcept -> JSValue {
        const auto font_size = text.font_size;
        const auto position = text.position;
        const auto color = text.color;
        const auto origin = text.origin.value_or(Vector2 {});
        const auto rotation = text.rotation.value_or(0);
        const auto spacing = text.spacing.value_or(0);
        auto font = GetFontDefault();
        if (text.font) font = ::Font {**text.font};

        if (const auto str = std::get_if<std::string>(&text.text)) {
            SPDLOG_TRACE("DrawTextPro({}, '{}', {}, {}, {})", font, *str, position, font_size, color);
            DrawTextPro(font, str->c_str(), position, origin, rotation, font_size, spacing, color);
        } else if (const auto codepoint = std::get_if<int>(&text.text)) {
            SPDLOG_TRACE("DrawTextCodepoint(font, {}, {}, {}, {})", font, *codepoint, position, font_size, color);
            DrawTextCodepoint(font, *codepoint, position, font_size, color);
        } else if (const auto codepoints = std::get_if<std::vector<int>>(&text.text)) {
            SPDLOG_TRACE(
                "DrawTextCodepoints({}, {}, {}, {}, {}, {})",
                font,
                (void *)codepoints->data(),
                codepoints->size(),
                position,
                font_size,
                color
            );
            DrawTextCodepoints(font, codepoints->data(), int(codepoints->size()), position, font_size, spacing, color);
        }
        return JS_DupValue(ctx, this_val);
    }

    auto begin_texture_mode(JSContext *ctx, JSValueConst this_val, const rl::RenderTexture *texture) noexcept
        -> JSValue {
        SPDLOG_TRACE("BeginTextureMode({})", *texture);
        BeginTextureMode(*texture);
        return JS_DupValue(ctx, this_val);
    }

    auto end_texture_mode(JSContext *ctx, JSValueConst this_val) noexcept -> JSValue {
        SPDLOG_TRACE("EndTextureMode()");
        EndTextureMode();
        return JS_DupValue(ctx, this_val);
    }

    auto
    with_texture(JSContext *ctx, JSValueConst this_val, const rl::RenderTexture *texture, JSValue function) noexcept
        -> JSValue {
        SPDLOG_TRACE("BeginTextureMode({})", *texture);
        BeginTextureMode(*texture);
        auto ret = JS_Call(ctx, function, JS_UNDEFINED, 0, nullptr);
        SPDLOG_TRACE("EndTextureMode()");
        EndTextureMode();
        if (JS_IsException(ret)) {
            return ret;
        }
        JS_FreeValue(ctx, ret);

        // Return this_val for chaining (will be passed in from wrapper)
        return JS_UNDEFINED;
        return JS_DupValue(ctx, this_val);
    }

    // JSClass implementation
    constexpr static auto class_name = "Graphics";

    inline static auto static_properties = PropertyList {};

    inline static auto instance_properties = PropertyList {
        export_method<&JSGraphics::clear>("clear"),
        export_method<&JSGraphics::circle>("circle"),
        export_method<&JSGraphics::rectangle>("rectangle"),
        export_method<&JSGraphics::rectangle_v>("rectangleV"),
        export_method<&JSGraphics::rectangle_rec>("rectangleRec"),
        export_method<&JSGraphics::rectangle_pro>("rectanglePro"),
        export_method<&JSGraphics::begin_camera_mode>("beginCameraMode"),
        export_method<&JSGraphics::end_camera_mode>("endCameraMode"),
        export_method<&JSGraphics::texture>("texture"),
        export_method<&JSGraphics::texture_v>("textureV"),
        export_method<&JSGraphics::texture_ex>("textureEx"),
        export_method<&JSGraphics::texture_rec>("textureRec"),
        export_method<&JSGraphics::texture_pro>("texturePro"),
        export_method<&JSGraphics::texture_npatch>("textureNPatch"),
        export_method<&JSGraphics::text>("text"),
        export_method<&JSGraphics::text_pro>("textPro"),
        export_method<&JSGraphics::begin_texture_mode>("beginTextureMode"),
        export_method<&JSGraphics::end_texture_mode>("endTextureMode"),
        export_method<&JSGraphics::with_texture>("withTexture"),
    };

    auto initialize() noexcept {}
};

inline auto graphics_module(JSContext *ctx) -> JSModuleDef * {
    auto m = JS_NewCModule(ctx, "@glint/core/graphics", [](auto ctx, auto m) -> int {
        JSGraphics::define(ctx);
        auto instance = JSGraphics::create_instance(ctx);
        JS_SetModuleExport(ctx, m, "graphics", JS_DupValue(ctx, instance));
        JS_SetModuleExport(ctx, m, "default", instance);

        return 0;
    });

    JS_AddModuleExport(ctx, m, "graphics");
    JS_AddModuleExport(ctx, m, "default");

    return m;
}

} // namespace glint::plugins::core
