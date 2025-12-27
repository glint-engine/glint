var Vector2 = require("muen/Vector2");

function Camera() {
    if (arguments.length === 0) {
        this.offset = new Vector2();
        this.target = new Vector2();
        this.rotation = 0;
        this.zoom = 1;
    } else if (arguments.length === 4) {
        if (typeof arguments[2] !== "number" || typeof arguments[3] !== "number") {
            throw TypeError("Invalid arguments for Camera");
        }
        this.offset = new Vector2(arguments[0].x, arguments[0].y);
        this.target = new Vector2(arguments[1].x, arguments[1].y);
        this.rotation = arguments[2];
        this.zoom = arguments[3];
    } else {
        throw TypeError("Invalid arguments for Camera");
    }
}

module.exports = Camera;
