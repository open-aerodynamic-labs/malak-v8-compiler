object xep_source_reader(var source: string) {

    var pos: int = 0;
    var line: int = 0;
    var col: int = 0;
    var end: int = sizeof(source);

    fun look_ahead() {
        var ch: char = source[pos];

        if (ch == '\n') {
            line++;
            col = 0;
        } else {
            col++;
        }

        return ch;
    }

}