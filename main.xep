object xep_source_reader
{
    var source: string;
    var pos: int;

    fun xep_source_reader(source: string) {
        this.source = source;
    }

    fun look_ahead() {
        var ch: char = this.source[pos];
        pos++;
        return ch;
    }
}


fun main(argc: int, varargs args: string) {
    var reader: xep_source_reader();
    println(reader->look_ahead() + 255.0);
}