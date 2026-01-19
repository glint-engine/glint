#include <jsutil.hpp>

#include <cassert>
#include <utility>

namespace js {

Value::Value(not_null<JSContext *> js, JSValue value) noexcept : _js(js), _value(value) {};

auto Value::operator*() noexcept -> JSValue& {
    assert(_value.has_value());
    return *_value;
}

auto Value::js() const noexcept -> JSContext * {
    return _js;
}

auto Value::get() noexcept -> JSValue& {
    assert(_value.has_value());
    return *_value;
}

Value::~Value() noexcept {
    if (_value) JS_FreeValue(_js, *_value);
}

Value::Value(const Value& other) noexcept {
    if (!other._value) return;
    _js = other._js;
    _value = JS_DupValue(other._js, *other._value);
}

Value::Value(Value&& other) noexcept {
    if (!other._value) return;

    _js = other._js;
    _value = other._value;

    other._js = nullptr;
    other._value = std::nullopt;
}

auto Value::operator=(const Value& other) -> Value& {
    if (this == &other) return *this;
    if (_value) JS_FreeValue(_js, *_value);
    _js = nullptr;
    _value = std::nullopt;

    if (!other._value) return *this;

    _js = other._js;
    _value = JS_DupValue(_js, *other._value);

    return *this;
}

auto Value::operator=(Value&& other) noexcept -> Value& {
    swap(*this, other);
    return *this;
}

auto swap(Value& a, Value& b) noexcept -> void {
    using std::swap;
    swap(a._js, b._js);
    swap(a._value, b._value);
}

auto Function::from_value(gsl::not_null<JSContext *> js, JSValue value) noexcept -> std::expected<Function, JSValue> {
    if (!JS_IsFunction(js, value)) return std::unexpected(JS_NewTypeError(js.get(), "Not a function"));
    auto val = Value(js, value);
    return Function(std::move(val));
}

auto Function::from_value(Value&& value) noexcept -> std::expected<Function, JSValue> {
    return Function(std::move(value));
}

auto Function::from_value(Value value) noexcept -> std::expected<Function, JSValue> {
    return Function(std::move(value));
}

auto Function::operator()() noexcept -> JSValue {
    return this->operator()(JS_UNDEFINED, {});
}

auto Function::operator()(std::span<JSValueConst> args) noexcept -> JSValue {
    return this->operator()(JS_UNDEFINED, args);
}

auto Function::operator()(JSValueConst this_value, std::span<JSValueConst> args) noexcept -> JSValue {
    return JS_Call(_value.js(), *_value, this_value, int(args.size()), args.data());
}

Function::Function(Value&& value) noexcept : _value(std::move(value)) {}

auto to_string(::JSContext *js, ::JSValueConst value) -> std::string {
    auto len = size_t {};
    auto cstr = ::JS_ToCStringLen(js, &len, value);
    if (cstr == nullptr) {
        return std::string {};
    }
    defer(::JS_FreeCString(js, cstr));
    return {cstr, len};
}

} // namespace js
