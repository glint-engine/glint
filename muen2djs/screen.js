var screen = Object.create(
    {},
    {
        dt: {
            enumerable: false,
            configurable: false,
            get: function () {
                return __muenScreenDt();
            },
        },

        time: {
            enumerable: false,
            configurable: false,
            get: function () {
                return __muenScreenTime();
            },
        },

        width: {
            get: function () {
                return __muenScreenWidth();
            },
            enumerable: false,
            configurable: false,
        },

        height: {
            get: function () {
                return __muenScreenHeight();
            },
            enumerable: false,
            configurable: false,
        },
    }
);

module.exports = Object.freeze(screen);
