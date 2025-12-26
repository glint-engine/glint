#pragma once

struct js_State;

namespace muen::bindings {

auto define_global_function(js_State *j, void (*fun)(js_State *), const char *name, int length = 0) -> void;
auto define(js_State *j) -> void;

} // namespace muen::bindings
