#pragma once

#include <array>
#include <span>

#include <spdlog/spdlog.h>

#include <quickjs.hpp>
#include <defer.hpp>

namespace glint::plugins::core {

using spdlog::level::level_enum;

inline auto log(JSContext *js, std::span<JSValueConst> args, level_enum level) -> JSValue {
    auto buf = std::stringstream {};
    for (size_t i = 0; i < args.size(); i++) {
        const auto str = JS_ToCString(js, args[i]);
        defer(JS_FreeCString(js, str));

        buf << str;
        if (i < args.size() - 1) {
            buf << " ";
        }
    }

    auto msg = buf.str();
    spdlog::log(level, "{}", msg);

    return JS_UNDEFINED;
}

inline auto console_trace(JSContext *js, JSValueConst, int argc, JSValueConst *argv) -> JSValue {
    return log(js, std::span(argv, argc), level_enum::trace);
}

inline auto console_debug(JSContext *js, JSValueConst, int argc, JSValueConst *argv) -> JSValue {
    return log(js, std::span(argv, argc), level_enum::debug);
}

inline auto console_log(JSContext *js, JSValueConst, int argc, JSValueConst *argv) -> JSValue {
    return log(js, std::span(argv, argc), level_enum::info);
}

inline auto console_warn(JSContext *js, JSValueConst, int argc, JSValueConst *argv) -> JSValue {
    return log(js, std::span(argv, argc), level_enum::warn);
}

inline auto console_error(JSContext *js, JSValueConst, int argc, JSValueConst *argv) -> JSValue {
    return log(js, std::span(argv, argc), level_enum::err);
}

const static auto FUNCS = std::array {
    ::JSCFunctionListEntry JS_CFUNC_DEF("trace", 0, console_trace),
    ::JSCFunctionListEntry JS_CFUNC_DEF("debug", 0, console_debug),
    ::JSCFunctionListEntry JS_CFUNC_DEF("log", 0, console_log),
    ::JSCFunctionListEntry JS_CFUNC_DEF("warn", 0, console_warn),
    ::JSCFunctionListEntry JS_CFUNC_DEF("error", 0, console_error),
};

inline auto console_module(JSContext *js) -> JSModuleDef * {
    auto m = JS_NewCModule(js, "glint:console", [](auto ctx, auto m) -> int {
        auto o = JS_NewObject(ctx);
        JS_SetPropertyFunctionList(ctx, o, FUNCS.data(), int {FUNCS.size()});
        JS_SetModuleExport(ctx, m, "console", JS_DupValue(ctx, o));
        JS_SetModuleExport(ctx, m, "default", o);

        return 0;
    });

    JS_AddModuleExport(js, m, "console");
    JS_AddModuleExport(js, m, "default");
    return m;
}

} // namespace glint::plugins::core
