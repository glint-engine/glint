declare function require(name: string): any;
declare const console: {
    trace: (...args: unknown[]) => void;
    debug: (...args: unknown[]) => void;
    log: (...args: unknown[]) => void;
    warn: (...args: unknown[]) => void;
    error: (...args: unknown[]) => void;
};
