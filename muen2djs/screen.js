function Screen() {
    Object.defineProperty(this, "dt", {
        enumerable: false,
        configurable: false,
        get: function () {
            return __muenScreenDt();
        },
    });

    Object.defineProperty(this, "width", {
        get: function () {
            return __muenScreenWidth();
        },
        enumerable: false,
        configurable: false,
    });

    Object.defineProperty(this, "height", {
        get: function () {
            return __muenScreenHeight();
        },
        enumerable: false,
        configurable: false,
    });
}

module.exports = Object.freeze(new Screen());
