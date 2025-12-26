var Vector2 = require("muen/Vector2");

function Camera() {
    if (arguments.length == 0) {
        this.offset = new Vector2();
        this.target = new Vector2();
        this.rotation = 0;
        this.zoom = 1;
    } else if (arguments.length == 4) {
        var offset = arguments[0];
        var target = arguments[1];
        var rotation = arguments[2];
        var zoom = arguments[3];
        if (typeof rotation !== 'number' || typeof zoom !== 'number') {
            throw TypeError("Invalid arguments for Camera");
        }
        this.offset = new Vector2(offset.x, offset.y);
        this.target = new Vector2(target.x, target.y);
        this.rotation = rotation;
        this.zoom = zoom;
    } else {
        throw TypeError("Invalid arguments for Camera");
    }
}

module.exports = Camera;
