var graphics = require("muen/graphics");
var Color = require("muen/Color");
var Texture = require("muen/Texture");
var Vector2 = require("muen/Vector2");

function Game() {
    this.config = { title: "Graphics" };
    this.bgColor = Color.fromHex("#181818");
    this.t = undefined;
}

Game.prototype.load = function () {
    this.t = Texture.load("cat.jpg");
    console.log(this.t.id);
    console.log(this.t.valid);
};

Game.prototype.update = function () {};

Game.prototype.draw = function () {
    graphics.clear(this.bgColor);
    graphics.textureEx(this.t, new Vector2(0, 0), 0, 0.65, new Color(255, 255, 255));
};

module.exports = Game;
