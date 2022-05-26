fun main(varargs args: string) -> void {

    let fp: *FILE = fopen("/tmp/test.txt", "w")
    fp->write(0, "Hello, world!\n", 14)

}