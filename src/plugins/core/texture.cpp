#include <plugins/core/texture.hpp>

namespace glint::plugins::core {

auto JSTexture::custom_constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) noexcept
    -> JSValue try {
    SPDLOG_TRACE("Texture.constructor/{}", argc);
    auto opts = read_texture_options_from_args(ctx, argc, argv);
    if (!opts) return jsthrow(opts.error());
    auto& e = Engine::get(ctx);

    const auto handle = std::visit(
        [&](auto&& arg) -> JSResult<ResourceStore<TextureData>::Handle> {
            using T = std::decay_t<decltype(arg)>;

            if constexpr (std::is_same_v<T, JSTextureLoadByName>) {
                return e.texture_store().load_by_name(arg.name);
            } else if constexpr (std::is_same_v<T, JSTextureLoadByParams>) {
                return e.texture_store().load(arg.name, [&]() -> TextureData {
                    return TextureData::load(arg.path, e.file_store());
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
    if (JS_IsException(proto)) {
        return proto;
    }
    defer(JS_FreeValue(ctx, proto));

    auto obj = JS_NewObjectProtoClass(ctx, proto, JSTexture::class_id(ctx));
    if (JS_HasException(ctx)) {
        JS_FreeValue(ctx, obj);
        return JS_GetException(ctx);
    }

    // NOLINTNEXTLINE(cppcoreguidelines-owning-memory): we cannot express ownership of JS object
    auto ptr = static_cast<JSTexture *>(new (std::nothrow) JSTexture());
    ptr->initialize(*handle);
    JS_SetOpaque(obj, ptr);

    return obj;
} catch (std::exception& e) {
    return JS_ThrowPlainError(ctx, "Unexpected C++ exception: %s", e.what());
}

auto JSTexture::custom_finalizer(JSRuntime *rt, JSValueConst val) noexcept -> void {
    SPDLOG_TRACE("Finalizing Texture");
    auto ptr = static_cast<JSTexture *>(JS_GetOpaque(val, JSTexture::class_id(rt)));
    if (ptr == nullptr) {
        SPDLOG_WARN("Could not finalize instance of {}: opaque pointer is null", JSTexture::class_name);
        return;
    }

    auto& e = Engine::get(rt);
    e.texture_store().release(ptr->_handle);
    // NOLINTNEXTLINE(cppcoreguidelines-owning-memory): we cannot express ownership of JS object
    delete ptr;
};

auto JSTexture::read_texture_options_from_args(JSContext *ctx, int argc, JSValueConst *argv) noexcept
    -> JSResult<JSTextureLoadMode> try {
    if (auto args_string = unpack_args<std::string>(ctx, argc, argv)) {
        auto [path] = std::move(*args_string);
        return JSTextureLoadByParams {
            .path = path,
            .name = path,
        };
    } else if (auto args_options = unpack_args<JSTextureOptions>(ctx, argc, argv)) {
        auto [opts] = std::move(*args_options);
        if (opts.name && !opts.path) {
            return JSTextureLoadByName {
                .name = std::move(*opts.name),
            };
        } else if (opts.path) {
            if (!opts.name) opts.name = opts.path->string();
            return JSTextureLoadByParams {
                .path = std::move(*opts.path),
                .name = std::move(*opts.name),
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
