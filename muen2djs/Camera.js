var Vector2 = require("muen/Vector2");

/**
 * @module muen/Camera
 */
module.exports = Camera;

/**
 * Represents two-dimensional camera
 * @constructor
 * @class
 * @param {Vector2} offset
 * @param {Vector2} target
 * @param {number} rotation
 * @param {number} zoom
 */
function Camera(offset, target, rotation, zoom) {
    if (
        !(offset instanceof Vector2) ||
        !(target instanceof Vector2) ||
        typeof rotation !== "number" ||
        typeof zoom !== "number"
    ) {
        throw TypeError("Invalid arguments for Camera");
    }

    /** @type {Vector2} */
    this.offset = offset.clone();

    /** @type {Vector2} */
    this.target = target.clone();

    /** @type {number} */
    this.rotation = rotation;

    /** @type {number} */
    this.zoom = zoom;
}

/**
 * Constructs camera with default parameters
 * @static
 * @returns {Camera}
 */
Camera.default = function () {
    return new Camera(Vector2.zero(), Vector2.zero(), 0, 1);
};
