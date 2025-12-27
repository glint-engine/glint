declare class Color {
    r: number;
    g: number;
    b: number;
    a: number;

    constructor();
    constructor(color: string);
    constructor(r: number, g: number, b: number);
    constructor(r: number, g: number, b: number, a: number);
}

export = Color;
