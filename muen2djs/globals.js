// @ts-nocheck

var Console = function () {};

Console.prototype.trace = __muenConsoleTrace;
Console.prototype.debug = __muenConsoleDebug;
Console.prototype.log = __muenConsoleLog;
Console.prototype.warn = __muenConsoleWarn;
Console.prototype.error = __muenConsoleError;

var console = Object.freeze(new Console());

module.exports = {
    console: console,
};
