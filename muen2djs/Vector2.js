/**
 * @class
 * @constructor
 * @param {number} x
 * @param {number} y
 */
function Vector2(x, y) {
    if (typeof x !== "number" || typeof y !== "number") {
        throw TypeError("Invalid arguments for Vector2");
    }

    /** @type {number} */
    this.x = x;

    /** @type {number} */
    this.y = y;
}

/**
 * @returns {Vector2}
 */
Vector2.zero = function () {
    return new Vector2(0, 0);
};

/**
 * @returns {Vector2}
 */
Vector2.prototype.clone = function () {
    return new Vector2(this.x, this.y);
};

module.exports = Vector2;
