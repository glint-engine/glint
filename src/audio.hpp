#pragma once

#include <raylib.h>
#include <expected>
#include <string>

namespace muen::audio {

struct Audio {};

auto create() -> Audio;
auto close(Audio& self) -> void;

namespace sound {

    struct Sound {
        ::Sound sound {};
        float volume = 1.0f;
        float pitch = 1.0f;
        float pan = 0.5f;
    };

    auto load(const std::string& path) -> std::expected<Sound, std::string>;
    auto unload(Sound& self) -> void;
    auto play(Sound& self) -> void;
    auto stop(Sound& self) -> void;
    auto pause(Sound& self) -> void;
    auto resume(Sound& self) -> void;
    auto is_playing(Sound& self) -> bool;
    auto get_volume(Sound& self) -> float;
    auto set_volume(Sound& self, float volume) -> void;
    auto get_pan(Sound& self) -> float;
    auto set_pan(Sound& self, float pan) -> void;
    auto get_pitch(Sound& self) -> float;
    auto set_pitch(Sound& self, float pitch) -> void;

} // namespace sound

using Sound = sound::Sound;

} // namespace muen::audio
