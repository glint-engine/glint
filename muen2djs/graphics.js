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

    /**
     * @param {Texture} texture
     * @param {number} x
     * @param {number} y
     * @param {Color} tint
     * @returns {typeof graphics}
     */ // @ts-ignore
    texture: function (texture, x, y, tint) {
        __muenGraphicsTexture(texture, x, y, tint);
        return this;
    },

    /**
     * @param {Texture} texture
     * @param {Vector2} position
     * @param {Color} tint
     * @returns {typeof graphics}
     */ // @ts-ignore
    textureV: function (texture, position, tint) {
        __muenGraphicsTextureV(texture, position, tint);
        return this;
    },

    /**
     * @param {Texture} texture
     * @param {Vector2} position
     * @param {number} rotation
     * @param {number} scale
     * @param {Color} tint
     * @returns {typeof graphics}
     */ // @ts-ignore
    textureEx: function (texture, position, rotation, scale, tint) {
        __muenGraphicsTextureEx(texture, position, rotation, scale, tint);
        return this;
    },

    /**
     * @param {Texture} texture
     * @param {Rectangle} source
     * @param {Vector2} position
     * @param {Color} tint
     * @returns {typeof graphics}
     */ // @ts-ignore
    textureRec: function (texture, source, position, tint) {
        __muenGraphicsTextureRec(texture, source, position, tint);
        return this;
    },

    /**
     * @param {Texture} texture
     * @param {Rectangle} source
     * @param {Rectangle} dest
     * @param {Vector2} origin
     * @param {number} rotation
     * @param {Color} tint
     * @returns {typeof graphics}
     */ // @ts-ignore
    texturePro: function (texture, source, dest, origin, rotation, tint) {
        __muenGraphicsTexturePro(texture, source, dest, origin, rotation, tint);
        return this;
    },

    /**
     * @param {Texture} texture
     * @param {NPatch} nPatch
     * @param {Rectangle} dest
     * @param {Vector2} origin
     * @param {number} rotation
     * @param {Color} tint
     * @returns {typeof graphics}
     */ // @ts-ignore
    textureNPatch: function (texture, nPatch, dest, origin, rotation, tint) {
        __muenGraphicsTextureNPatch(texture, nPatch, dest, origin, rotation, tint);
        return this;
    },
});

module.exports = Object.freeze(graphics);
