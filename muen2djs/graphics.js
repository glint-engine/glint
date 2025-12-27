function Graphics() {}

Graphics.prototype.clear = __muenGraphicsClear;
Graphics.prototype.circle = __muenGraphicsCircle;
Graphics.prototype.rectangle = __muenGraphicsRectangle;
Graphics.prototype.beginCameraMode = __muenGraphicsBeginCameraMode;
Graphics.prototype.endCameraMode = __muenGraphicsEndCameraMode;

module.exports = Object.freeze(new Graphics());
