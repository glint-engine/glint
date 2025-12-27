var Camera = require("muen/Camera");
var Rectangle = require("muen/Rectangle");
var graphics = require("muen/graphics");
var Color = require("muen/Color");

function Game() {
    this.camera = new Camera();
}

Game.prototype.update = function () {};

Game.prototype.draw = function () {
    graphics.clear(new Color("#181818"));
    graphics.beginCameraMode(this.camera);
    graphics.rectangle(new Rectangle(150, 150, 50, 100), new Color("#e17cf0ff"));
    graphics.endCameraMode();
};

module.exports = Game;
