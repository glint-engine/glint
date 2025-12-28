/**
 * @param {number} id
 */
function Sound(id) {
    this.id = id;
}

/**
 * @param {string} path
 * @returns {Sound}
 */
Sound.load = function (path) {
    var id = __muenSoundLoad(path);
    return new Sound(id);
};

module.exports = Sound;

Sound.prototype.unload = function () {
    __muenSoundUnload(this.id);
};

Sound.prototype.play = function () {
    __muenSoundPlay(this.id);
};

Sound.prototype.stop = function () {
    __muenSoundStop(this.id);
};

Object.defineProperties(Sound.prototype, {
    playing: {
        get: function () {
            return __muenSoundIsPlaying(this.id);
        },
    },
    volume: {
        get: function () {
            return __muenSoundGetVolume(this.id);
        },
        set: function (volume) {
            __muenSoundSetVolume(this.id, volume);
        },
    },
    pan: {
        get: function () {
            return __muenSoundGetPan(this.id);
        },
        set: function (pan) {
            __muenSoundSetPan(this.id, pan);
        },
    },
    pitch: {
        get: function () {
            return __muenSoundGetPitch(this.id);
        },
        set: function (pitch) {
            __muenSoundSetPitch(this.id, pitch);
        },
    },
});
