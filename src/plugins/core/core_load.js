import { console } from "@glint/core";

Object.defineProperty(globalThis, "console", {
    value: console,
    writable: false,
    configurable: false,
    enumerable: true,
});
