/**
 * @constructor
 * @param {number} r
 * @param {number} g
 * @param {number} b
 * @param {number | undefined} a
 */
function Color(r, g, b, a) {
    /** @type {number} */
    this.r = assertByte(r);

    /** @type {number} */
    this.g = assertByte(g);

    /** @type {number} */
    this.b = assertByte(b);

    /** @type {number} */
    this.a = a ? assertByte(a) : 255;
}

/**
 * @param {string} code
 * @returns {Color}
 */
Color.fromHex = function (code) {
    if (typeof code !== "string" || (code.length !== 7 && code.length !== 9) || code[0] !== "#") {
        throw TypeError("Invalid arguments for Color");
    }

    var r = parseByte(code, 1);
    var g = parseByte(code, 3);
    var b = parseByte(code, 5);
    var a = code.length === 9 ? parseByte(code, 7) : 255;

    return new Color(r, g, b, a);
};

/**
 * @private
 * @param {string} s
 * @param {number} i
 * @return {number}
 */
function parseByte(s, i) {
    return parseInt(s.slice(i, i + 2), 16);
}

/**
 * @private
 * @param {number} n
 * @return {number}
 */
function assertByte(n) {
    if ((n | 0) !== n) {
        throw TypeError("Color byte must be integer");
    }

    if (n < 0 || n >= 256) {
        throw TypeError("Color byte must be in range [0; 255]");
    }

    return n;
}

module.exports = Color;
