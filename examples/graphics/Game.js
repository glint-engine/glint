var graphics = require("muen/graphics");
var Color = require("muen/Color");
var Vector2 = require("muen/Vector2");
var Rectangle = require("muen/Rectangle");

function Game() {
    this.config = { title: "Graphics" };
    this.bgColor = Color.fromHex("#181818");
}

Game.prototype.update = function () {};

Game.prototype.draw = function () {
    graphics.clear(this.bgColor);
    graphics.rectangle(50, 100, 30, 30, Color.fromHex("#c49bffff"));
    graphics.rectangle(new Vector2(100, 100), new Vector2(30, 30), Color.fromHex("#bf37bfff"));
    graphics.rectangle(new Rectangle(150, 100, 30, 30), Color.fromHex("#ff9ba5ff"));
    graphics.rectangle(new Rectangle(210, 100, 30, 30), new Vector2(10, 10), 45, Color.fromHex("#ffc787ff"));

    // Error running game: TypeError: Invalid arguments count for graphics.rectangle
    // graphics.rectangle(50, 100, 30, 30, 40, Color.fromHex("#c49bffff"));
};

module.exports = Game;
