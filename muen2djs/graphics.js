/**
 * @typedef {import("muen/Camera")} Camera
 * @typedef {import("muen/Color")} Color
 * @typedef {import("muen/Rectangle")} Rectangle
 * @typedef {import("muen/Vector2")} Vector2
 */
// function __muenGraphicsRectangle(rectangle: Rectangle, color: Color): void;
// function __muenGraphicsRectangle(rectangle: Rectangle, origin: Vector2, rotation: number, color: Color): void;
// function __muenGraphicsBeginCameraMode(camera: Camera): void;
// function __muenGraphicsEndCameraMode(): void;

function Graphics() {}

/**
 * @function
 * @param {Color} color
 * @returns {void}
 */ // @ts-ignore
Graphics.prototype.clear = __muenGraphicsClear;

/**
 * @function
 * @param {number} x
 * @param {number} y
 * @param {number} radius
 * @param {Color} color
 * @returns {void}
 */ // @ts-ignore
Graphics.prototype.circle = __muenGraphicsCircle;

/**
 * @function
 * @param {number} x
 * @param {number} y
 * @param {number} width
 * @param {number} height
 * @param {Color} color
 * @returns {void}
 */ // @ts-ignore
Graphics.prototype.rectangle = __muenGraphicsRectangle;

/**
 * @function
 * @param {Vector2} position
 * @param {Vector2} size
 * @param {Color} color
 * @returns {void}
 */ // @ts-ignore
Graphics.prototype.rectangleV = __muenGraphicsRectangle;

/**
 * @function
 * @param {Rectangle} rectangle
 * @param {Color} color
 * @returns {void}
 */ // @ts-ignore
Graphics.prototype.rectangleRec = __muenGraphicsRectangle;

/**
 * @function
 * @param {Rectangle} rectangle
 * @param {Vector2} origin
 * @param {number} rotation
 * @param {Color} color
 * @returns {void}
 */ // @ts-ignore
Graphics.prototype.rectanglePro = __muenGraphicsRectangle;

/**
 * @function
 * @param {Camera} camera
 * @returns {void}
 */ // @ts-ignore
Graphics.prototype.beginCameraMode = __muenGraphicsBeginCameraMode;

/**
 * @function
 * @returns {void}
 */ // @ts-ignore
Graphics.prototype.endCameraMode = __muenGraphicsEndCameraMode;

module.exports = Object.freeze(new Graphics());
