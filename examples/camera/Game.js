var Camera = require("muen/Camera");
var Rectangle = require("muen/Rectangle");
var graphics = require("muen/graphics");
var Color = require("muen/Color");
var screen = require("muen/screen");

function Game() {
    this.camera = Camera.default();
    this.rec = Rectangle.zero();
    this.bg = Color.fromHex("#181818");
    this.fg = Color.fromHex("#e17cf0ff");
}

Game.prototype.update = function () {
    this.camera.rotation += screen.dt * 150;

    this.camera.offset.x = screen.width * 0.5;
    this.camera.offset.y = screen.height * 0.5;
    this.camera.target.x = screen.width * 0.5;
    this.camera.target.y = screen.height * 0.5;
    this.camera.zoom = Math.sin(screen.time) * 0.5 + 1.2;
};

Game.prototype.draw = function () {
    graphics.clear(this.bg);
    graphics.beginCameraMode(this.camera);
    graphics.rectangle(screen.width * 0.3, screen.height * 0.3, 50, 100, this.fg);
    graphics.endCameraMode();
};

module.exports = Game;
