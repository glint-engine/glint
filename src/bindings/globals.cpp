#include "globals.hpp"

#include "js.hpp"

#include <raylib.h>

namespace muen::bindings::globals {

auto define(js_State *j) -> void {
    js::pushnull(j);
    js::pushliteral(j, "name");
    js::pushliteral(
        j,
        R"(
            var cache = require.cache;
            if (name in cache) {
                return cache[name];
            }

            var module = {};
            __muenFsLoad(name + '.js')(module);
            cache[name] = module.exports;
            return module.exports;
        )"
    );

    js::create_function(j, "require");

    js::defglobal(j, "require", js::READONLY | js::DONTCONF | js::DONTENUM);

    js::pushliteral(j, "require.cache = Object.create(null);");
    js::eval(j);
    js::pop(j, 1);

    js::pushliteral(j, "require('muen/globals')");
    js::eval(j);
    js::getproperty(j, -1, "console");
    js::defglobal(j, "console", js::READONLY | js::DONTCONF | js::DONTENUM);
    js::pop(j, 1);
}

} // namespace muen::bindings::globals
