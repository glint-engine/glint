var Camera = require("muen/Camera");
var Rectangle = require("muen/Rectangle");
var graphics = require("muen/graphics");
var Color = require("muen/Color");

function Game() {
    this.camera = Camera.default();
}

Game.prototype.update = function () {};

Game.prototype.draw = function () {
    graphics.clear(Color.fromHex("#181818"));
    graphics.beginCameraMode(this.camera);
    graphics.rectangle(new Rectangle(150, 150, 50, 100), Color.fromHex("#e17cf0ff"));
    graphics.endCameraMode();
};

module.exports = Game;
