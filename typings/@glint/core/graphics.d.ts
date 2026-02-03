import { Camera } from "@glint/core/Camera";
import { NPatch } from "@glint/core/NPatch";
import { Text } from "@glint/core/Text";
import { Texture } from "@glint/core/Texture";
import { type BasicColor } from "@glint/core/Color";
import { type BasicRectangle } from "@glint/core/Rectangle";
import { type BasicVector2 } from "@glint/core/Vector2";

export interface Graphics {
    clear(color: BasicColor): Graphics;

    circle(x: number, y: number, radius: number, color: BasicColor): Graphics;

    rectangle(x: number, y: number, width: number, height: number, color: BasicColor): Graphics;
    rectangleV(position: BasicVector2, size: BasicVector2, color: BasicColor): Graphics;
    rectangleRec(rec: BasicRectangle, color: BasicColor): Graphics;
    rectanglePro(rec: BasicRectangle, origin: BasicVector2, rotation: number, color: BasicColor): Graphics;

    beginCameraMode(camera: Camera): Graphics;
    endCameraMode(): Graphics;

    texture(texture: Texture, x: number, y: number, tint: BasicColor): Graphics;
    textureV(texture: Texture, position: BasicVector2, tint: BasicColor): Graphics;
    textureEx(texture: Texture, position: BasicVector2, rotation: number, scale: number, tint: BasicColor): Graphics;
    textureRec(texture: Texture, source: BasicRectangle, position: BasicVector2, tint: BasicColor): Graphics;
    texturePro(
        texture: Texture,
        source: BasicRectangle,
        dest: BasicRectangle,
        origin: BasicVector2,
        rotation: number,
        tint: BasicColor,
    ): Graphics;
    textureNPatch(
        texture: Texture,
        nPatch: NPatch,
        dest: BasicRectangle,
        origin: BasicVector2,
        rotation: number,
        tint: BasicColor,
    ): Graphics;

    text(text: string, x: number, y: number, fontSize: number, color: BasicColor): Graphics;
    textPro(text: Text): Graphics;
}

export declare const graphics: Graphics;
export default graphics;
