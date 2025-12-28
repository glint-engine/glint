var console = Object.create({
    trace: function () {
        __muenConsoleTrace(arguments);
    },
    debug: function () {
        __muenConsoleDebug(arguments);
    },
    log: function () {
        __muenConsoleLog(arguments);
    },
    warn: function () {
        __muenConsoleWarn(arguments);
    },
    error: function () {
        __muenConsoleError;
    },
});

module.exports = Object.freeze(console);
