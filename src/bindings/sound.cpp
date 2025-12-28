#include "sound.hpp"

#include "bindings.hpp"
#include "engine.hpp"

namespace muen::bindings::sound {

namespace sound = audio::sound;

auto define(js_State *j) -> void {
    define_global_function(
        j,
        [](js_State *j) -> void {
            const auto e = static_cast<engine::Engine *>(js::getcontext(j));
            if (!engine::is_audio_ready(*e)) {
                js::error(
                    j,
                    "Could not load sound when audio device is not ready. Loading sounds outside of Game.load?"
                );
            }

            const auto filename = js::tostring(j, 1);
            auto path = std::string {e->root_path};
            if (path[path.size() - 1] != '/') {
                path += '/';
            }
            path += filename;

            const auto id = engine::load_sound(*e, path);
            if (id.has_value()) {
                js::pushnumber(j, id.value());
            } else {
                js::error(j, "Error loading sound: %s", id.error().c_str());
            }
        },
        "__muenSoundLoad",
        1
    );

    define_global_function(
        j,
        [](js_State *j) -> void {
            const auto id = uint32_t(js::tonumber(j, 1));
            const auto e = static_cast<engine::Engine *>(js::getcontext(j));
            engine::unload_sound(*e, id);
            js::pushundefined(j);
        },
        "__muenSoundUnload",
        1
    );

    define_global_function(
        j,
        [](js_State *j) -> void {
            const auto id = uint32_t(js::tonumber(j, 1));
            const auto e = static_cast<engine::Engine *>(js::getcontext(j));
            if (auto s = engine::get_sound(*e, id)) {
                sound::play(*s);
                js::pushundefined(j);
            } else {
                js::error(j, "Could not play invalid sound");
            }
        },
        "__muenSoundPlay",
        1
    );

    define_global_function(
        j,
        [](js_State *j) -> void {
            const auto id = uint32_t(js::tonumber(j, 1));
            const auto e = static_cast<engine::Engine *>(js::getcontext(j));
            if (auto s = engine::get_sound(*e, id)) {
                sound::stop(*s);
                js::pushundefined(j);
            } else {
                js::error(j, "Could not stop invalid sound");
            }
        },
        "__muenSoundStop",
        1
    );

    define_global_function(
        j,
        [](js_State *j) -> void {
            const auto id = uint32_t(js::tonumber(j, 1));
            const auto e = static_cast<engine::Engine *>(js::getcontext(j));
            if (auto s = engine::get_sound(*e, id)) {
                js::pushboolean(j, sound::is_playing(*s));
            } else {
                js::error(j, "Could not inspect invalid sound");
            }
        },
        "__muenSoundIsPlaying",
        1
    );

    define_global_function(
        j,
        [](js_State *j) -> void {
            const auto id = uint32_t(js::tonumber(j, 1));
            const auto e = static_cast<engine::Engine *>(js::getcontext(j));
            if (auto s = engine::get_sound(*e, id)) {
                js::pushnumber(j, sound::get_volume(*s));
            } else {
                js::error(j, "Could not get volume of invalid sound");
            }
        },
        "__muenSoundGetVolume",
        1
    );

    define_global_function(
        j,
        [](js_State *j) -> void {
            const auto id = uint32_t(js::tonumber(j, 1));
            const auto volume = float(js::tonumber(j, 2));
            const auto e = static_cast<engine::Engine *>(js::getcontext(j));
            if (auto s = engine::get_sound(*e, id)) {
                sound::set_volume(*s, volume);
                js::pushundefined(j);
            } else {
                js::error(j, "Could not set volume of invalid sound");
            }
        },
        "__muenSoundSetVolume",
        2
    );

    define_global_function(
        j,
        [](js_State *j) -> void {
            const auto id = uint32_t(js::tonumber(j, 1));
            const auto e = static_cast<engine::Engine *>(js::getcontext(j));
            if (auto s = engine::get_sound(*e, id)) {
                js::pushnumber(j, sound::get_pitch(*s));
            } else {
                js::error(j, "Could not get pitch of invalid sound");
            }
        },
        "__muenSoundGetPitch",
        1
    );

    define_global_function(
        j,
        [](js_State *j) -> void {
            const auto id = uint32_t(js::tonumber(j, 1));
            const auto pitch = float(js::tonumber(j, 2));
            const auto e = static_cast<engine::Engine *>(js::getcontext(j));
            if (auto s = engine::get_sound(*e, id)) {
                sound::set_pitch(*s, pitch);
                js::pushundefined(j);
            } else {
                js::error(j, "Could not set pitch of invalid sound");
            }
        },
        "__muenSoundSetPitch",
        2
    );

    define_global_function(
        j,
        [](js_State *j) -> void {
            const auto id = uint32_t(js::tonumber(j, 1));
            const auto e = static_cast<engine::Engine *>(js::getcontext(j));
            if (auto s = engine::get_sound(*e, id)) {
                js::pushnumber(j, sound::get_pan(*s));
            } else {
                js::error(j, "Could not get pan of invalid sound");
            }
        },
        "__muenSoundGetPan",
        1
    );

    define_global_function(
        j,
        [](js_State *j) -> void {
            const auto id = uint32_t(js::tonumber(j, 1));
            const auto pan = float(js::tonumber(j, 2));
            auto e = static_cast<engine::Engine *>(js::getcontext(j));
            if (auto s = engine::get_sound(*e, id)) {
                sound::set_pan(*s, pan);
                js::pushundefined(j);
            } else {
                js::error(j, "Could not set pan of invalid sound");
            }
        },
        "__muenSoundSetPan",
        2
    );
}

} // namespace muen::bindings::sound
