function Rectangle() {
    if (arguments.length === 0) {
        this.x = 0;
        this.y = 0;
        this.width = 0;
        this.height = 0;
    } else if (arguments.length === 4) {
        if (
            typeof arguments[0] !== "number" ||
            typeof arguments[1] !== "number" ||
            typeof arguments[2] !== "number" ||
            typeof arguments[3] !== "number"
        ) {
            throw TypeError("Invalid arguments for Rectangle");
        }
        this.x = arguments[0];
        this.y = arguments[1];
        this.width = arguments[2];
        this.height = arguments[3];
    } else {
        throw TypeError("Invalid arguments for Rectangle");
    }
}

module.exports = Rectangle;
