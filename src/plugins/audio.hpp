#pragma once

#include <spdlog/spdlog.h>

#include <plugins/audio/music.hpp>
#include <plugins/audio/sound.hpp>

namespace glint::plugins::audio {

constexpr static char AUDIO_MODULE[] = {
#include "audio_module.js.h"
};

constexpr static char AUDIO_LOAD[] = {
#include "audio_load.js.h"
};

auto plugin(JSContext *ctx) -> EnginePlugin {
    return EnginePlugin {
        .name = "audio",
        .c_modules =
            {
                {"@glint/audio/Music", music_module(ctx)},
                {"@glint/audio/Sound", sound_module(ctx)},
            },
        .js_modules =
            {
                {"@glint/audio", AUDIO_MODULE},
            },
        .load = [=]() -> Result<> {
            auto ret = JS_Eval(ctx, AUDIO_LOAD, sizeof(AUDIO_LOAD) - 1, "@glint/audio/load.js", JS_EVAL_TYPE_MODULE);
            JS_FreeValue(ctx, ret);
            engine::audio::init();
            return {};
        },

        .unload = []() -> Result<> {
            engine::audio::close();
            return {};
        },
        .update = []() -> Result<> {
            for (const auto music : engine::audio::get().musics) {
                if (engine::audio::music::is_playing(*music)) {
                    engine::audio::music::update(*music);
                }
            }
            return {};
        }
    };
}

} // namespace glint::plugins::audio
