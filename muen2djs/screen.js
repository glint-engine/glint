/**
 * @module muen/screen
 */
module.exports = Object.freeze(new Screen());

/**
 * @constructor
 * @class
 */
function Screen() {}

/**
 * Delta time
 * @property {number}
 * @readonly
 */
Object.defineProperty(Screen.prototype, "dt", {
    enumerable: false,
    configurable: false,
    get: function () {
        // @ts-ignore
        return __muenScreenDt();
    },
});

/**
 * Delta time
 * @property {number}
 * @readonly
 */
Object.defineProperty(Screen.prototype, "width", {
    get: function () {
        // @ts-ignore
        return __muenScreenWidth();
    },
    enumerable: false,
    configurable: false,
});

/**
 * Delta time
 * @property {number}
 * @readonly
 */
Object.defineProperty(Screen.prototype, "height", {
    get: function () {
        // @ts-ignore
        return __muenScreenHeight();
    },
    enumerable: false,
    configurable: false,
});
