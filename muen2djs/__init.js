Object.defineProperty(this, "require", {
    value: (function () {
        var require = Function(
            "name",
            "var cache = require.cache;" +
                "if (name in cache) {" +
                "   return cache[name];" +
                "}" +
                "var module = {};" +
                "__muenFsLoad(name + '.js')(module);" +
                "cache[name] = module.exports;" +
                "return module.exports;"
        );

        Object.defineProperty(require, "cache", {
            value: Object.create(null),
            writable: false,
            configurable: false,
            enumerable: false,
        });

        return require;
    })(),
    writable: false,
    configurable: false,
    enumerable: false,
});

Object.defineProperty(this, "console", {
    value: require("muen/console"),
    writable: false,
    configurable: false,
    enumerable: true,
});
