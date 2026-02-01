#pragma once

#include <engine.hpp>
#include <engine/audio.hpp>
#include <quickjs.hpp>
#include <raylib.hpp>

namespace glint::plugins::audio {

using namespace js;
namespace audio = engine::audio;
namespace music = engine::audio::music;

class JSMusic: public JSClass<JSMusic> {
  public:
    auto get_music() noexcept -> audio::Music& { return *_music; }

    auto get_playing() noexcept -> bool { return music::is_playing(*_music); }

    auto get_looping() noexcept -> bool { return music::get_looping(*_music); }

    auto set_looping(bool looping) noexcept -> void { music::set_looping(*_music, looping); }

    auto get_volume() noexcept -> float { return music::get_volume(*_music); }

    auto set_volume(float volume) noexcept -> void { music::set_volume(*_music, volume); }

    auto get_pan() noexcept -> float { return music::get_pan(*_music); }

    auto set_pan(float pan) noexcept -> void { music::set_pan(*_music, pan); }

    auto get_pitch() noexcept -> float { return music::get_pitch(*_music); }

    auto set_pitch(float pitch) noexcept -> void { music::set_pitch(*_music, pitch); }

    auto play() noexcept -> void { music::play(*_music); }

    auto stop() noexcept -> void { music::stop(*_music); }

    auto pause() noexcept -> void { music::pause(*_music); }

    auto resume() noexcept -> void { music::resume(*_music); }

    auto seek(float cursor) noexcept -> void { music::seek(*_music, cursor); }

    auto unload() noexcept -> void {
        audio::get().musics.erase(this->_music);
        // The actual deletion will happen in the finalizer
    }

  private:
    audio::Music *_music {};

    static auto custom_constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) noexcept
        -> JSValue;

    static auto custom_finalizer(JSRuntime *rt, JSValueConst val) noexcept -> void;

  public: // JSClass implementation
    constexpr static auto class_name = "Music";

    inline static auto static_properties = PropertyList {};

    inline static auto instance_properties = PropertyList {
        export_get_only<&JSMusic::get_playing>("playing"),
        export_getset<&JSMusic::get_looping, &JSMusic::set_looping>("looping"),
        export_getset<&JSMusic::get_volume, &JSMusic::set_volume>("volume"),
        export_getset<&JSMusic::get_pan, &JSMusic::set_pan>("pan"),
        export_getset<&JSMusic::get_pitch, &JSMusic::set_pitch>("pitch"),
        export_method<&JSMusic::play>("play"),
        export_method<&JSMusic::stop>("stop"),
        export_method<&JSMusic::pause>("pause"),
        export_method<&JSMusic::resume>("resume"),
        export_method<&JSMusic::seek>("seek"),
        export_method<&JSMusic::unload>("unload"),
    };

    // Custom constructor to load music from file
    constexpr static JSCFunction *constructor = &custom_constructor;

    // Custom finalizer to remove from audio system
    constexpr static JSClassFinalizer *class_finalizer = &custom_finalizer;

    auto initialize(audio::Music *music) noexcept { _music = music; }
};

inline auto music_module(JSContext *ctx) -> JSModuleDef * {
    auto m = JS_NewCModule(ctx, "@glint/audio/music", [](auto ctx, auto m) -> int {
        auto ctor = JSMusic::define(ctx).take();
        JS_SetModuleExport(ctx, m, "Music", JS_DupValue(ctx, ctor));
        JS_SetModuleExport(ctx, m, "default", ctor);
        return 0;
    });

    JS_AddModuleExport(ctx, m, "Music");
    JS_AddModuleExport(ctx, m, "default");
    return m;
}

} // namespace glint::plugins::audio

namespace glint::js {

using plugins::audio::JSMusic;

template<>
inline auto convert_from_js<JSMusic *>(const Value& val) noexcept -> JSResult<JSMusic *> {
    return JSMusic::get_instance(val);
}

template<>
inline auto convert_from_js<engine::audio::Music *>(const Value& val) noexcept -> JSResult<engine::audio::Music *> {
    if (auto r = convert_from_js<JSMusic *>(val)) return &(*r)->get_music();
    return JSError::type_error(val.ctx(), "Not an instance of Music");
}

} // namespace glint::js
