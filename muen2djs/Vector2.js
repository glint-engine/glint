/**
 * @module muen/Vector2
 */
module.exports = Vector2;

/**
 * Represents 2-dimensional vector {x,y}
 * @constructor
 * @class
 * @param {number} x
 * @param {number} y
 */
function Vector2(x, y) {
    if (typeof x !== "number" || typeof y !== "number") {
        throw TypeError("Invalid arguments for Vector2");
    }

    /** @member {number} */
    this.x = x;
    /** @member {number} */
    this.y = y;
}

/**
 * Creates Vector2 with x=0 and y=0
 * @static
 * @returns {Vector2}
 */
Vector2.zero = function () {
    return new Vector2(0, 0);
};

/**
 * Clones vector creating new instance
 * @returns {Vector2}
 */
Vector2.prototype.clone = function () {
    return new Vector2(this.x, this.y);
};
