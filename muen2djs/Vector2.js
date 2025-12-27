function Vector2() {
    if (arguments.length === 0) {
        this.x = 0;
        this.y = 0;
    } else if (arguments.length === 2) {
        if (typeof arguments[0] !== "number" || typeof arguments[1] !== "number") {
            throw TypeError("Invalid arguments for Vector2");
        }
        this.x = arguments[0];
        this.y = arguments[1];
    } else {
        throw TypeError("Invalid arguments for Vector2");
    }
}

module.export = Vector2;
