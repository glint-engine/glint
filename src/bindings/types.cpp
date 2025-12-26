#include "types.hpp"

#include "js.hpp"

namespace muen::bindings::types {

auto tocolor(js::State *j, int idx) -> ::Color {
    ::Color c;

    js::getproperty(j, idx, "r");
    c.r = js::tointeger(j, -1);
    js::pop(j, 1);

    js::getproperty(j, idx, "g");
    c.g = js::tointeger(j, -1);
    js::pop(j, 1);

    js::getproperty(j, idx, "b");
    c.b = js::tointeger(j, -1);
    js::pop(j, 1);

    js::getproperty(j, idx, "a");
    c.a = js::tointeger(j, -1);
    js::pop(j, 1);

    return c;
}

auto tovector2(js::State *j, int idx) -> ::Vector2 {
    ::Vector2 v;

    js::getproperty(j, idx, "x");
    v.x = js::tofloat(j, -1);
    js::pop(j, 1);

    js::getproperty(j, idx, "y");
    v.y = js::tofloat(j, -1);
    js::pop(j, 1);

    return v;
}

auto torectangle(js_State *j, int idx) -> ::Rectangle {
    ::Rectangle r;

    js::getproperty(j, idx, "x");
    r.x = js::tofloat(j, -1);
    js::pop(j, 1);

    js::getproperty(j, idx, "y");
    r.y = js::tofloat(j, -1);
    js::pop(j, 1);

    js::getproperty(j, idx, "width");
    r.width = js::tofloat(j, -1);
    js::pop(j, 1);

    js::getproperty(j, idx, "height");
    r.height = js::tofloat(j, -1);
    js::pop(j, 1);

    return r;
}

auto tocamera(js_State *j, int idx) -> ::Camera2D {
    ::Camera2D c;

    js::getproperty(j, idx, "offset");
    c.offset = tovector2(j, -1);
    js::pop(j, 1);

    js::getproperty(j, idx, "target");
    c.target = tovector2(j, -1);
    js::pop(j, 1);

    js::getproperty(j, idx, "rotation");
    c.rotation = js::tofloat(j, -1);
    js::pop(j, 1);

    js::getproperty(j, idx, "zoom");
    c.zoom = js::tofloat(j, -1);
    js::pop(j, 1);

    return c;
}

} // namespace muen::bindings::types
