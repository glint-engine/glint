var Vector2 = require("muen/Vector2");

/**
 * @constructor
 * @param {Vector2} offset Camera offset (displacement from target)
 * @param {Vector2} target Camera target (rotation and zoom origin)
 * @param {number} rotation Camera rotation in degrees
 * @param {number} zoom Camera zoom (scaling)
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
 * @returns {Camera}
 */
Camera.default = function () {
    return new Camera(Vector2.zero(), Vector2.zero(), 0, 1);
};

module.exports = Camera;
