var Console = function() {
    this.trace = __muenConsoleTrace;
    this.debug = __muenConsoleDebug;
    this.log = __muenConsoleLog;
    this.warn = __muenConsoleWarn;
    this.error = __muenConsoleError;
}

module.exports = {
    console: Object.freeze(new Console()),
}
