#include "./audio.hpp"

#include <cstring>
#include <algorithm>

namespace muen::engine::audio::music {

auto load(const std::filesystem::path& name, std::span<char> data) noexcept -> Result<owner<Music *>> {
    const auto raylib_music = LoadMusicStreamFromMemory(
        name.extension().string().c_str(),
        // NOLINTNEXTLINE: Casting from char* to unsigned char* is explicitly allowed by the standard
        reinterpret_cast<unsigned char *>(data.data()),
        int(data.size())
    );

    if (!IsMusicValid(raylib_music)) {
        return Err(error::create("Error loading music"));
    }

    const auto music = Music {.music = raylib_music};
    SetMusicVolume(music.music, music.volume);
    SetMusicPan(music.music, music.pan);
    SetMusicPitch(music.music, music.pitch);

    return {owner<Music *>(new (std::nothrow) Music {music})};
}

auto unload(owner<Music *> self) noexcept -> void {
    UnloadMusicStream(self->music);
    delete self;
}

auto update(Music& self) noexcept -> void {
    UpdateMusicStream(self.music);
}

auto play(Music& self) noexcept -> void {
    PlayMusicStream(self.music);
}

auto stop(Music& self) noexcept -> void {
    StopMusicStream(self.music);
}

auto pause(Music& self) noexcept -> void {
    PauseMusicStream(self.music);
}

auto resume(Music& self) noexcept -> void {
    ResumeMusicStream(self.music);
}

auto seek(Music& self, float cursor) noexcept -> void {
    SeekMusicStream(self.music, cursor);
}

auto is_playing(const Music& self) noexcept -> bool {
    return IsMusicStreamPlaying(self.music);
}

auto get_looping(const Music& self) noexcept -> bool {
    return self.music.looping;
}

auto set_looping(Music& self, bool looping) noexcept -> void {
    self.music.looping = looping;
}

auto get_volume(const Music& self) noexcept -> float {
    return self.volume;
}

auto set_volume(Music& self, float volume) noexcept -> void {
    self.volume = std::clamp(volume, 0.0f, 1.0f);
    SetMusicVolume(self.music, self.volume);
}

auto get_pan(const Music& self) noexcept -> float {
    return self.pan;
}

auto set_pan(Music& self, float pan) noexcept -> void {
    self.pan = std::clamp(pan, 0.0f, 1.0f);
    SetMusicPan(self.music, self.pan);
}

auto get_pitch(const Music& self) noexcept -> float {
    return self.pitch;
}

auto set_pitch(Music& self, float pitch) noexcept -> void {
    self.pitch = pitch;
    ::SetMusicPitch(self.music, self.pitch);
}

} // namespace muen::engine::audio::music
