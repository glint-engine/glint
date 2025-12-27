import Camera = require("muen/Camera");
import Color = require("muen/Color");
import Rectangle = require("muen/Rectangle");
import Vector2 = require("muen/Vector2");

declare global {
    // @ts-ignore
    const console: {
        log(...args: unknown[]): void;
        error(...args: unknown[]): void;
        warn(...args: unknown[]): void;
        trace(...args: unknown[]): void;
        debug(...args: unknown[]): void;
    };

    function __muenConsoleLog(...args: unknown[]): void;
    function __muenConsoleError(...args: unknown[]): void;
    function __muenConsoleWarn(...args: unknown[]): void;
    function __muenConsoleTrace(...args: unknown[]): void;
    function __muenConsoleDebug(...args: unknown[]): void;

    function __muenScreenDt(): number;
    function __muenScreenHeight(): number;
    function __muenScreenWidth(): number;

    function __muenGraphicsClear(color: Color): void;
    function __muenGraphicsCircle(x: number, y: number, radius: number, color: Color): void;
    function __muenGraphicsRectangle(x: number, y: number, width: number, height: number, color: Color): void;
    function __muenGraphicsRectangle(position: Vector2, size: Vector2, color: Color): void;
    function __muenGraphicsRectangle(rectangle: Rectangle, color: Color): void;
    function __muenGraphicsRectangle(rectangle: Rectangle, origin: Vector2, rotation: number, color: Color): void;
    function __muenGraphicsBeginCameraMode(camera: Camera): void;
    function __muenGraphicsEndCameraMode(): void;
}
