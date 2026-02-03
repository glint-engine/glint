#include <plugins/audio/music.hpp>

namespace glint::plugins::audio {

auto JSMusic::custom_constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) noexcept
    -> JSValue {
    auto& e = Engine::get(ctx);
    auto args = unpack_args<std::string>(ctx, argc, argv);
    if (!args) return jsthrow(args.error());
    const auto [filename] = std::move(*args);

    auto music_result = music::load(filename, e.file_store());
    if (!music_result) {
        return JS_ThrowInternalError(
            ctx,
            "%s",
            fmt::format("Could not load music: {}", music_result.error()->msg()).c_str()
        );
    }
    // NOLINTNEXTLINE(cppcoreguidelines-owning-memory): we cannot express ownership of JS object
    auto music = new (std::nothrow) audio::Music {std::move(*music_result)};

    auto proto = JS_GetPropertyStr(ctx, this_val, "prototype");
    if (JS_IsException(proto)) {
        return proto;
    }
    defer(JS_FreeValue(ctx, proto));

    auto obj = JS_NewObjectProtoClass(ctx, proto, JSMusic::class_id(ctx));
    if (JS_HasException(ctx)) {
        JS_FreeValue(ctx, obj);
        return JS_GetException(ctx);
    }

    // NOLINTNEXTLINE(cppcoreguidelines-owning-memory): we cannot express ownership of JS object
    auto ptr = new (std::nothrow) JSMusic();
    ptr->initialize(music);
    JS_SetOpaque(obj, ptr);

    // Register with audio system
    audio::get().musics.insert(music);

    return obj;
}

auto JSMusic::custom_finalizer(JSRuntime *rt, JSValueConst val) noexcept -> void {
    auto ptr = static_cast<JSMusic *>(JS_GetOpaque(val, JSMusic::class_id(rt)));
    if (ptr == nullptr) {
        SPDLOG_WARN("Could not finalize instance of {}: opaque pointer is null", JSMusic::class_name);
        return;
    }

    audio::get().musics.erase(ptr->_music);
    // NOLINTNEXTLINE(cppcoreguidelines-owning-memory): we cannot express ownership of JS object
    delete ptr;
};

} // namespace glint::plugins::audio
