import Vector2 = require("muen/Vector2");

declare class Camera {
    offset: Vector2;
    target: Vector2;
    rotation: number;
    zoom: number;
}

export = Camera;
