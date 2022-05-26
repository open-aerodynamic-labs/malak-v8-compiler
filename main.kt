struct FILE {
    let fhd: *byte;
    let flag: int;
}

enum _SeekMode { EOF, SET, CUR }

__fundecl fun fopen(fp: *FILE, path: string): int;
__fundecl fun fclose(fp: *FILE): int;
__fundecl fun fread(buf: *void, size: long, count: int, fp: *FILE): int;
__fundecl fun fwrite(buf: *void, size: long, count: int, fp: *FILE): int;
__fundecl fun fseek(fp: *FILE, off: long, whence: _SeekMode): int;
__fundecl fun ftell(fp: *FILE): long;
__fundecl fun freadall(fp: *FILE): *byte; /* 文件所有内容，以及文件大小 */
__fundecl fun fappend(buf: *void, size: long, count: int, fp: *FILE): int;

fun fsize(fp: *FILE): long {
    let pos = ftell(fp);
    fseek(fp, 0, SEEK_END);
    let size = ftell(fp);
    fseek(fp, pos, SEEK_SET);
    return size;
}

fun fputc(c: int, fp: *FILE): int = fwrite(&c, sizeof(int), 1, fp);
fun fputs(str: string, fp: *FILE): int = fwrite(str.cptr(), lenof(str), 1, fp);
fun fgetc(fp: *FILE): int = fread(&c, sizeof(int), 1, fp);
