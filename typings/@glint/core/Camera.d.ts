import { type BasicVector2 } from "@glint/core/Vector2";

export class Camera {
    /** Camera offset (displacement from target) */
    offset: BasicVector2;

    /** Camera target (rotation and zoom origin) */
    target: BasicVector2;

    /** Camera rotation in degrees */
    rotation: number;

    /** Camera zoom (scaling) */
    zoom: number;

    /**
     * @param offset Camera offset (displacement from target)
     * @param target Camera target (rotation and zoom origin)
     * @param rotation Camera rotation in degrees
     * @param zoom Camera zoom (scaling)
     */
    constructor(offset: BasicVector2, target: BasicVector2, rotation: number, zoom: number);

    /**
     * Constructs camera with default parameters
     */
    static default(): Camera;
}

export default Camera;
