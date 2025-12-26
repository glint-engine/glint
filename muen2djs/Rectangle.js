module.exports = function Rectangle() {
    if (arguments.length === 0) {
        this.x = 0;
        this.y = 0;
        this.width = 0;
        this.height = 0;

    } else if (arguments.length === 4) {
        var x = arguments[0];
        var y = arguments[1];
        var width = arguments[2];
        var height = arguments[3];
        if (typeof x !== 'number' || typeof y !== 'number' || typeof width !== 'number' || typeof height !== 'number') {
            throw TypeError("Invalid arguments for Rectangle");
        }
        this.x = x;
        this.y = y;
        this.width = width;
        this.height = height;
    } else {
        throw TypeError("Invalid arguments for Rectangle")
    }
}
