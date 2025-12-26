module.exports = function Vector2() {
    if (arguments.length === 0) {
        this.x = 0;
        this.y = 0;

    } else if (arguments.length === 2) {
        var x = arguments[0];
        var y = arguments[1];
        if (typeof x !== 'number' || typeof y !== 'number') {
            throw TypeError("Invalid arguments for Vector2");
        }
        this.x = x;
        this.y = y;
    } else {
        throw TypeError("Invalid arguments for Vector2")
    }
}

