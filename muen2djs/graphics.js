/**
 * @namespace
 */
var graphics = Object.create({
    /**
     * @param {Color} color
     * @returns {typeof graphics}
     */
    clear: function (color) {
        __muenGraphicsClear(color);
        return this;
    },

    /**
     * @param {number} x
     * @param {number} y
     * @param {number} radius
     * @param {Color} color
     * @returns {typeof graphics}
     */
    circle: function (x, y, radius, color) {
        __muenGraphicsCircle(x, y, radius, color);
        return this;
    },

    /**
     * @param {number} x
     * @param {number} y
     * @param {number} width
     * @param {number} height
     * @param {Color} color
     * @returns {typeof graphics}
     */
    rectangle: function (x, y, width, height, color) {
        __muenGraphicsRectangle(x, y, width, height, color);
        return this;
    },

    /**
     * @param {Vector2} position
     * @param {Vector2} size
     * @param {Color} color
     * @returns {typeof graphics}
     */
    rectangleV: function (position, size, color) {
        __muenGraphicsRectangleV(position, size, color);
        return this;
    },

    /**
     * @param {Rectangle} rectangle
     * @param {Color} color
     * @returns {typeof graphics}
     */
    rectangleRec: function (rectangle, color) {
        __muenGraphicsRectangleRec(rectangle, color);
        return this;
    },

    /**
     * @param {Rectangle} rec
     * @param {Vector2} origin
     * @param {number} rotation
     * @param {Color} color
     * @returns {typeof graphics}
     */ // @ts-ignore
    rectanglePro: function (rec, origin, rotation, color) {
        __muenGraphicsRectanglePro(rec, origin, rotation, color);
        return this;
    },

    /**
     * @param {Camera} camera
     * @returns {typeof graphics}
     */ // @ts-ignore
    beginCameraMode: function (camera) {
        __muenGraphicsBeginCameraMode(camera);
        return this;
    },

    /**
     * @returns {typeof graphics}
     */ // @ts-ignore
    endCameraMode: function () {
        __muenGraphicsEndCameraMode();
        return this;
    },
});

module.exports = Object.freeze(graphics);
