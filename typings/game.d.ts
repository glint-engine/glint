/**
 * @inline
 */
export interface Config {
    window: {
        /**
         * Window title
         */
        title?: string;

        /**
         * Window width
         */
        width?: number;

        /**
         * Window height
         */
        height?: number;

        /**
         * Target fps. Engine will try to call your `update` and `draw` callbacks
         * exactly `fps` times per second
         */
        fps?: number;

        /**
         * Set to try enabling V-Sync on GPU
         */
        vsync?: boolean;

        /**
         * Set to run program in fullscreen
         */
        fullscreenMode?: boolean;

        /**
         * Set to allow resizable window
         */
        resizable?: boolean;

        /**
         * Set to disable window decoration (frame and buttons)
         */
        undecorated?: boolean;

        /**
         * Set to hide window
         */
        hidden?: boolean;

        /**
         * Set to minimize window (iconify)
         */
        minimized?: boolean;

        /**
         * Set to maximize window (expanded to monitor)
         */
        maximized?: boolean;

        /**
         * Set to window non focused
         */
        unfocused?: boolean;

        /**
         * Set to window always on top
         */
        topmost?: boolean;

        /**
         * Set to allow windows running while minimized
         */
        alwaysRun?: boolean;

        /**
         * Set to allow transparent framebuffer
         */
        transparent?: boolean;

        /**
         * Set to support HighDPI
         */
        highdpi?: boolean;

        /**
         * Set to support mouse passthrough, only supported when FLAG_WINDOW_UNDECORATED
         */
        mousePassthrough?: boolean;

        /**
         * Set to run program in borderless windowed mode
         */
        borderlessWindowedMode?: boolean;

        /**
         * Set to try enabling MSAA 4X
         */
        msaa4x?: boolean;

        /**
         * Set to try enabling interlaced video format (for V3D)
         */
        interlaced?: boolean;
    };
}

/**
 * Configures how the engine executes the game
 * @optional
 */
export declare const config: Config | undefined;

/**
 * Called once when engine initializes game
 *
 * @event
 * @optional
 */
export function load(): void;

/**
 * Called every frame when engine updates game
 *
 * @event
 * @optional
 */
export function update(): void;

/**
 * Called every frame when engine draws game
 *
 * @event
 * @optional
 */
export function draw(): void;

/**
 * Called before game reloading. This function returns JS value that later will
 * be passed to {@link postReload}
 *
 * @event
 * @optional
 */
export function preReload(): unknown;

/**
 * Called after game reloading
 *
 * @param state JS value returned by {@link preReload}
 *
 * @event
 * @optional
 */
export function postReload(state: unknown): void;
