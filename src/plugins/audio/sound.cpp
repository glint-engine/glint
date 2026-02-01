#include <plugins/audio/sound.hpp>

#include <engine.hpp>

namespace glint::plugins::audio {

auto JSSound::custom_constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) noexcept
    -> JSValue {
    auto& e = Engine::get(ctx);
    auto args = unpack_args<std::string>(ctx, argc, argv);
    if (!args) return jsthrow(args.error());
    const auto [filename] = std::move(*args);

    auto sound_result = sound::load(filename, e.file_store());
    if (!sound_result) {
        return JS_ThrowInternalError(
            ctx,
            "%s",
            fmt::format("Could not load sound: {}", sound_result.error()->msg()).c_str()
        );
    }

    auto proto = JS_GetPropertyStr(ctx, this_val, "prototype");
    if (JS_IsException(proto)) {
        return proto;
    }
    defer(JS_FreeValue(ctx, proto));

    auto obj = JS_NewObjectProtoClass(ctx, proto, JSSound::class_id(ctx));
    if (JS_HasException(ctx)) {
        JS_FreeValue(ctx, obj);
        return JS_GetException(ctx);
    }

    // NOLINTNEXTLINE(cppcoreguidelines-owning-memory): we cannot express ownership of JS object
    auto sound_ptr = new (std::nothrow) audio::Sound {std::move(*sound_result)};

    // NOLINTNEXTLINE(cppcoreguidelines-owning-memory): we cannot express ownership of JS object
    auto ptr = new (std::nothrow) JSSound();

    ptr->initialize(sound_ptr);
    JS_SetOpaque(obj, ptr);

    audio::get().sounds.insert(sound_ptr);

    return obj;
}

auto JSSound::custom_finalizer(JSRuntime *rt, JSValueConst val) noexcept -> void {
    auto ptr = static_cast<JSSound *>(JS_GetOpaque(val, JSSound::class_id(rt)));
    if (ptr == nullptr) {
        SPDLOG_WARN("Could not finalize instance of {}: opaque pointer is null", JSSound::class_name);
        return;
    }

    audio::get().sounds.erase(ptr->_sound);

    // NOLINTNEXTLINE(cppcoreguidelines-owning-memory): we cannot express ownership of JS object
    delete ptr;
}

} // namespace glint::plugins::audio
