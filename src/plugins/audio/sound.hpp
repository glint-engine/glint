#pragma once

#include <quickjs.hpp>
#include <raylib.hpp>
#include <engine/audio.hpp>

namespace glint::plugins::audio {

using namespace js;
namespace audio = engine::audio;
namespace sound = engine::audio::sound;

class JSSound: public JSClass<JSSound> {
  public:
    auto get_sound() noexcept -> audio::Sound& { return *_sound; }

    auto get_playing() noexcept -> bool { return sound::is_playing(*_sound); }

    auto get_volume() noexcept -> float { return sound::get_volume(*_sound); }

    auto set_volume(float volume) noexcept -> void { sound::set_volume(*_sound, volume); }

    auto get_pan() noexcept -> float { return sound::get_pan(*_sound); }

    auto set_pan(float pan) noexcept -> void { sound::set_pan(*_sound, pan); }

    auto get_pitch() noexcept -> float { return sound::get_pitch(*_sound); }

    auto set_pitch(float pitch) noexcept -> void { sound::set_pitch(*_sound, pitch); }

    auto play() noexcept -> void { sound::play(*_sound); }

    auto stop() noexcept -> void { sound::stop(*_sound); }

    auto unload() noexcept -> void { audio::get().sounds.erase(_sound); }

  private:
    audio::Sound *_sound = nullptr;
    static auto custom_constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) noexcept
        -> JSValue;

    static auto custom_finalizer(JSRuntime *rt, JSValueConst val) noexcept -> void;

  public: // JSClass implementation
    constexpr static auto class_name = "Sound";

    inline static auto static_properties = PropertyList {};

    inline static auto instance_properties = PropertyList {
        export_get_only<&JSSound::get_playing>("playing"),
        export_getset<&JSSound::get_volume, &JSSound::set_volume>("volume"),
        export_getset<&JSSound::get_pan, &JSSound::set_pan>("pan"),
        export_getset<&JSSound::get_pitch, &JSSound::set_pitch>("pitch"),
        export_method<&JSSound::play>("play"),
        export_method<&JSSound::stop>("stop"),
        export_method<&JSSound::unload>("unload"),
    };

    constexpr static JSCFunction *constructor = custom_constructor;

    constexpr static JSClassFinalizer *class_finalizer = custom_finalizer;

    auto initialize(audio::Sound *sound) noexcept { _sound = sound; }
};

inline auto sound_module(JSContext *ctx) -> JSModuleDef * {
    auto m = JS_NewCModule(ctx, "glint:sound", [](auto ctx, auto m) -> int {
        auto ctor = JSSound::define(ctx).take();
        JS_SetModuleExport(ctx, m, "Sound", JS_DupValue(ctx, ctor));
        JS_SetModuleExport(ctx, m, "default", ctor);
        return 0;
    });

    JS_AddModuleExport(ctx, m, "Sound");
    JS_AddModuleExport(ctx, m, "default");
    return m;
}

} // namespace glint::plugins::audio

namespace glint::js {

using plugins::audio::JSSound;

template<>
inline auto convert_from_js<JSSound *>(const Value& val) noexcept -> JSResult<JSSound *> {
    return JSSound::get_instance(val);
}

template<>
inline auto convert_from_js<engine::audio::Sound *>(const Value& val) noexcept -> JSResult<engine::audio::Sound *> {
    if (auto r = convert_from_js<JSSound *>(val)) return &(*r)->get_sound();
    return JSError::type_error(val.ctx(), "Not an instance of Sound");
}

} // namespace glint::js
