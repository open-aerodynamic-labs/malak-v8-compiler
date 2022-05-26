object FILE {
    let path: string;
    let name: string;
    let bufsize: int;
    let mode: int;
    let flag: int;

    __builtin fun open() -> int;
    __builtin fun close() -> int;
    __builtin fun read(off: int, value: *string, len: int) -> int;
    __builtin fun write(off: int, value: *string, len: int) -> int;
}