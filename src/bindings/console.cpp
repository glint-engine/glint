#include "console.hpp"

#include <sstream>

#include <raylib.h>
#include <spdlog/spdlog.h>

#include "bindings.hpp"
#include "js.hpp"

namespace muen::bindings::console {

using spdlog::level::level_enum;

auto log(js_State *J, level_enum level) -> void;

auto define(js::State *j) -> void {
    define_global_function(j, [](js::State *j) -> void { log(j, level_enum::trace); }, "__muenConsoleTrace");
    define_global_function(j, [](js::State *j) -> void { log(j, level_enum::debug); }, "__muenConsoleDebug");
    define_global_function(j, [](js::State *j) -> void { log(j, level_enum::info); }, "__muenConsoleLog");
    define_global_function(j, [](js::State *j) -> void { log(j, level_enum::warn); }, "__muenConsoleWarn");
    define_global_function(j, [](js::State *j) -> void { log(j, level_enum::err); }, "__muenConsoleError");
}

auto log(js_State *J, level_enum level) -> void {
    auto top = js_gettop(J);

    auto buf = std::stringstream {};
    for (auto i = 1; i < top; ++i) {
        buf << js_tostring(J, i);
        if (i < top - 1) {
            buf << " ";
        }
    }

    auto msg = buf.str();
    spdlog::log(level, "{}", msg);

    js_pushundefined(J);
}

} // namespace muen::bindings::console
