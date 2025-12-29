var Sound = require("muen/Sound");
var Color = require("muen/Color");
var screen = require("muen/screen");
var graphics = require("muen/graphics");

function Game() {
    this.config = {
        title: "Ball sounds",
    };

    this.bg = Color.fromHex("#181818");
    this.fg = Color.fromHex("#902020");

    this.x = 20;
    this.y = 20;
    this.velX = 500;
    this.velY = 500;
    this.radius = 10;
}

Game.prototype.load = function () {
    this.hitsound = Sound.load("hit.mp3");
};

Game.prototype.update = function () {
    var newX = this.x + this.velX * screen.dt;
    var newY = this.y + this.velY * screen.dt;
    var collided = false;

    if (newX >= this.radius && newX < screen.width - this.radius) {
        this.x = newX;
    } else {
        this.velX *= -1;
        collided = true;
    }

    if (newY >= this.radius && newY < screen.height - this.radius) {
        this.y = newY;
    } else {
        this.velY *= -1;
        collided = true;
    }

    if (collided) {
        this.hitsound.play();
    }
};

Game.prototype.draw = function () {
    graphics.clear(this.bg);
    graphics.circle(this.x, this.y, this.radius, this.fg);
};

module.exports = Game;
