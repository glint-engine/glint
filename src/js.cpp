#include "js.hpp"

#include <format>

namespace muen::js {

auto eval_file(State *j, const char *path) -> void {
    js::loadfile(j, path);
    js::pushundefined(j);
    js::call(j, 0);
    js::pop(j, 1);
}

auto js_create_function(js_State *J, const char *source) -> void {}

Exception::Exception(js_State *j) {
    js::getproperty(j, -1, "stack");
    auto stack = js::tostring(j, -1);
    js::pop(j, 1);

    auto desc = js::tostring(j, -1);
    js::pop(j, 1);

    this->message = std::format("{}\nStack trace:{}", desc, stack);
}

const char *Exception::what() const noexcept {
    return this->message.c_str();
}

} // namespace muen::js
