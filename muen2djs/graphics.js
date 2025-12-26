function Graphics() {
    this.clear = __muenGraphicsClear;
    this.circle = __muenGraphicsCircle;
    this.rectangle = __muenGraphicsRectangle;
    this.beginCameraMode = __muenGraphicsBeginCameraMode;
    this.endCameraMode = __muenGraphicsEndCameraMode;
};

module.exports = Object.freeze(new Graphics());
