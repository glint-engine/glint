#include <plugins/core/font.hpp>

namespace glint::plugins::core {

auto JSFont::custom_constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) -> JSValue {
    auto opts = read_font_options_from_args(ctx, argc, argv);
    if (!opts) return jsthrow(opts.error());
    auto& e = Engine::get(ctx);

    const auto handle = std::visit(
        [&](auto&& arg) -> JSResult<ResourceStore<FontData>::Handle> {
            using T = std::decay_t<decltype(arg)>;

            if constexpr (std::is_same_v<T, JSFontLoadByName>) {
                return e.font_store().load_by_name(arg.name);
            } else if constexpr (std::is_same_v<T, JSFontLoadByParams>) {
                return e.font_store().load(arg.name, [&]() -> FontData {
                    std::optional<std::span<int>> codepoints {};
                    if (arg.codepoints) codepoints = std::span(*arg.codepoints);
                    return FontData::load(arg.path, arg.font_size, codepoints, e.file_store());
                });
            } else if constexpr (std::is_same_v<T, std::monostate>) {
                return JSError::type_error(ctx, "Either name or path must be present in options");
            } else {
                static_assert(false, "Non-exhaustive visit");
            }
        },
        std::move(*opts)
    );
    if (!handle) return jsthrow(handle.error());

    auto proto = JS_GetPropertyStr(ctx, this_val, "prototype");
    if (!JS_IsObject(proto)) {
        return JS_ThrowInternalError(ctx, "Font constructor could not get prototype");
    }
    defer(JS_FreeValue(ctx, proto));

    auto obj = JS_NewObjectProtoClass(ctx, proto, JSFont::class_id(ctx));
    if (!JS_IsObject(obj)) {
        return JS_ThrowInternalError(ctx, "Font constructor could not create object");
    }

    // NOLINTNEXTLINE(cppcoreguidelines-owning-memory): we cannot express ownership of JS object
    auto ptr = new (std::nothrow) JSFont();
    ptr->initialize(*handle);
    JS_SetOpaque(obj, ptr);

    return obj;
}

auto JSFont::custom_finalizer(JSRuntime *rt, JSValueConst val) noexcept -> void {
    auto ptr = static_cast<JSFont *>(JS_GetOpaque(val, JSFont::class_id(rt)));
    if (ptr == nullptr) {
        SPDLOG_WARN("Could not finalize instance of {}: opaque pointer is null", JSFont::class_name);
        return;
    }

    auto& e = Engine::get(rt);
    e.font_store().release(ptr->_handle);
    // NOLINTNEXTLINE(cppcoreguidelines-owning-memory): we cannot express ownership of JS object
    delete ptr;
}

auto JSFont::read_font_options_from_args(JSContext *ctx, int argc, JSValueConst *argv) noexcept
    -> JSResult<JSFontLoadMode> try {
    if (auto args_string = unpack_args<std::string>(ctx, argc, argv)) {
        auto [path] = std::move(*args_string);
        return JSFontLoadByParams {
            .path = path,
            .name = path,
            .font_size = 32,
            .codepoints = std::nullopt,
        };
    } else if (auto args_options = unpack_args<JSFontOptions>(ctx, argc, argv)) {
        auto [opts] = std::move(*args_options);
        if (opts.name && !opts.path && !opts.font_size && !opts.codepoints) {
            return JSFontLoadByName {
                .name = std::move(*opts.name),
            };
        } else if (opts.path) {
            if (!opts.name) opts.name = opts.path->string();
            return JSFontLoadByParams {
                .path = std::move(*opts.path),
                .name = std::move(*opts.name),
                .font_size = opts.font_size.value_or(32),
                .codepoints = std::move(opts.codepoints),
            };
        } else {
            return JSError::type_error(ctx, "Either name or path must be present in options");
        }
    } else {
        return JSError::type_error(ctx, "No matching overload");
    }
} catch (std::exception& e) {
    return JSError::plain_error(ctx, fmt::format("Unexpected C++ exception: {}", e.what()));
}

} // namespace glint::plugins::core
