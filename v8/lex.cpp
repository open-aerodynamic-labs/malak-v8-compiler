/**
 * Copyright 2022 open-aerodynamic-airduck
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "v8/lex.h"
#include "srcreader.h"
#include <sstream>
#include <map>
#include <algorithm>

#define clearbuf(x) x.str("")

#define __EMPTY_STRING__() ""

/** 词法解析回调函数，获取当前token kind */
typedef void (*f_lexc)(tokenkind_t *);

/**
 *   词法解析器map，用于方便匹配关键字等字符。
 * 相当于就是高级语言中对string做switch。
 */
typedef std::map<std::string, f_lexc>     lexmap_t;
typedef std::map<char, f_lexc>            eoimap_t;

#define xep_add_lexc(map, key, kind) (map)->insert(std::make_pair(key, [](tokenkind_t *tk) { *tk = kind; }))

/**
 * 解析阶段
 */
#define PHASE_NOP        (-1)
#define PHASE_DONE         0
#define PHASE_INTEGER      1
#define PHASE_DECIMAL      2
#define PHASE_HEX          5
#define PHASE_BACKSLASH    4

/** 初始化词法MAP */
void init_lexc_map(lexmap_t *map)
{
      xep_add_lexc(map, "let", KIND_LET);
      xep_add_lexc(map, "char", KIND_CHAR);
      xep_add_lexc(map, "int", KIND_INT);
      xep_add_lexc(map, "long", KIND_LONG);
      xep_add_lexc(map, "float", KIND_FLOAT);
      xep_add_lexc(map, "double", KIND_DOUBLE);
      xep_add_lexc(map, "string", KIND_STRING);
      xep_add_lexc(map, "fun", KIND_FUN);
      xep_add_lexc(map, "varargs", KIND_VARARGS);
      xep_add_lexc(map, "any", KIND_ANY);
      xep_add_lexc(map, "return", KIND_RETURN);
      xep_add_lexc(map, "goto", KIND_GOTO);
      xep_add_lexc(map, "object", KIND_OBJECT);
      xep_add_lexc(map, "this", KIND_THIS);
      xep_add_lexc(map, "static", KIND_STATIC);
      xep_add_lexc(map, "const", KIND_CONST);
      xep_add_lexc(map, "if", KIND_IF);
      xep_add_lexc(map, "else", KIND_ELSE);
      xep_add_lexc(map, "elif", KIND_ELIF);
      xep_add_lexc(map, "while", KIND_WHILE);
      xep_add_lexc(map, "for", KIND_FOR);
      xep_add_lexc(map, "do", KIND_DO);
      xep_add_lexc(map, "break", KIND_BREAK);
      xep_add_lexc(map, "continue", KIND_CONTINUE);
      xep_add_lexc(map, "switch", KIND_SWITCH);
      xep_add_lexc(map, "case", KIND_CASE);
      xep_add_lexc(map, "default", KIND_DEFAULT);
      xep_add_lexc(map, "sizeof", KIND_SIZEOF);
      xep_add_lexc(map, "null", KIND_NULL);
      xep_add_lexc(map, "struct", KIND_STRUCT);
      xep_add_lexc(map, "__builtin", KIND_BUILTIN);
      xep_add_lexc(map, "void", KIND_VOID);

}

/** 初始化符号MAP */
void init_eoic_map(eoimap_t *map)
{
      xep_add_lexc(map, ' ', KIND_NOP);
      xep_add_lexc(map, '!', KIND_NOP);
      xep_add_lexc(map, '=', KIND_EQ);
      xep_add_lexc(map, '+', KIND_ADD);
      xep_add_lexc(map, '-', KIND_SUB);
      xep_add_lexc(map, '*', KIND_STAR);
      xep_add_lexc(map, '/', KIND_SLASH);
      xep_add_lexc(map, ';', KIND_EOI);
      xep_add_lexc(map, '.', KIND_DOT);
      xep_add_lexc(map, ',', KIND_COMMA);
      xep_add_lexc(map, '(', KIND_OPEN_PAREN);
      xep_add_lexc(map, ')', KIND_CLOSE_PAREN);
      xep_add_lexc(map, '{', KIND_OPEN_BRACE);
      xep_add_lexc(map, '}', KIND_CLOSE_BRACE);
      xep_add_lexc(map, '[', KIND_OPEN_BRACKET);
      xep_add_lexc(map, ']', KIND_CLOSE_BRACKET);
      xep_add_lexc(map, ':', KIND_COLON);
      xep_add_lexc(map, '>', KIND_GT);
      xep_add_lexc(map, '<', KIND_LT);
      xep_add_lexc(map, '|', KIND_OR);
      xep_add_lexc(map, '&', KIND_AND);
      xep_add_lexc(map, '~', KIND_NOT);
      xep_add_lexc(map, '^', KIND_XOR);
      xep_add_lexc(map, '\0', KIND_EOF);
}

/**
 * 处理读取到数字的情况
 *
 * @param ch        第一个数字
 * @param reader    读取器
 * @param eoimap    符号MAP
 * @param line      行号
 * @param col       列号
 * @return          读取到的数字
 */
std::string lexc_read_number(char ch, xep_source_reader &reader, eoimap_t &eoimap, int *line, int *col)
{
      std::stringstream       buf;
      std::string             buftok;

      /* 如果当前数字是减号，那么在return的时候添加到前缀 */
      bool is_sub_symbol = ch == '-';
      if (!is_sub_symbol)
            buf << ch;

      int phase = PHASE_INTEGER;

      while(!reader.look_ahead(&ch, line, col)) {
            // 如果当前解析状态是已完成，就不再解析数字了。直接做符号判断
            if (phase != PHASE_DONE) {
                  if (isnumber(ch)) {
                        buf << ch;
                        continue;
                  }

                  /* 如果是扫描到了16进制，那么则允许 a - f | A - F的字符 */
                  if (phase == PHASE_HEX && ((ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F'))) {
                        buf << ch;
                        continue;
                  }

                  // 如果扫描到小数点，那么就代表是浮点数
                  if (ch == '.') {
                        // 重复扫描到小数点就是错误的token，抛出异常
                        if (phase == PHASE_DECIMAL || phase == PHASE_HEX)
                              goto FLAG_THROW_INVALID_NUMBER;

                        buf << ch;
                        phase = PHASE_DECIMAL;
                        continue;
                  }

                  // 读到了16进制数字
                  if ((ch == 'x' || ch == 'X') && reader.peek_back() == '0') {
                        buf << ch;
                        phase = PHASE_HEX;
                        continue;
                  }

                  // 如果是其他字符那么就开始对当前的token缓冲区进行处理
                  buf >> buftok;

                  int tmp = phase;
                  phase = PHASE_DONE;
                  if (tmp == PHASE_INTEGER) {
                        // long | int
                        if (ch == 'L' || ch == 'l') {
                              buftok.insert(0, "L");
                              continue;
                        } else {
                              buftok.insert(0, "I");
                        }
                  } else if (tmp == PHASE_DECIMAL) {
                        // float | double
                        if (ch == 'F' || ch == 'f') {
                              buftok.insert(0, "F");
                              continue;
                        } else {
                              buftok.insert(0, "D");
                        }
                  } else {
                        // hex
                        std::transform(buftok.begin(), buftok.end(), buftok.begin(), ::toupper);
                        buftok.insert(0, "H");
                  }
            }

            // 如果是空格不做处理
            if (isspace(ch))
                  continue;

            /**
             *   每个数字符号读到结束的时候，它们的结束符号必须是eoi符号。这样才可以让解析器知道当前的token是否结束。
             * 并且满足一个表达式的结束条件。
             *
             * eoi = 所有的有作用的符号都是一个eoi字符。包括但不限于，{, }, (, ), +, -, *, /, ...
             *
             *   假设当数字结束了，它的结束符是')', 我们可以认为这个调用函数的参数。它可能是 x(100)。
             * 其他的符号也是类似的。都可以被编译器推理出来。所以这是数字结束的必要条件，必须是符号！
             *
             */
            if (!eoimap.count(ch))
                  goto FLAG_THROW_INVALID_NUMBER;

            reader.back(line, col);

            if (is_sub_symbol)
                  buftok.insert(1, "-");

            return buftok;
      }

FLAG_THROW_INVALID_NUMBER:
      xep_throw_error("lexc error: invalid number;", *line, *col);
      return __EMPTY_STRING__();
}

/**
 * 读取到字符串
 *
 * @param ch        第一个符号
 * @param reader    读取器
 * @param line      行号
 * @param col       列号
 * @return          读取到的字符串
 */
std::string lexc_read_string(char ch, xep_source_reader &reader, int *line, int *col)
{
      std::stringstream       buf;
      int                     strline = *line;

      buf << ch;

      int phase = PHASE_NOP;
      while(!reader.look_ahead(&ch, line, col)) {
            if (*line > strline)
                  xep_throw_error("lexc error: string too long;", *line, *col);

            if (ch != '"') {
                  if (ch == '\\') {
                        phase = PHASE_BACKSLASH;
                  } else {
                        phase = PHASE_NOP;
                  }

                  buf << ch;
                  continue;
            } else {
                  // 如果是反斜杠，那么就是转义字符。加入到字符串中，继续扫描。
                  if (phase == PHASE_BACKSLASH) {
                        buf << ch;
                        phase = PHASE_NOP;
                        continue;
                  }

                  buf << ch;
                  return buf.str();
            }

      }

      xep_throw_error("lexc error: string not closed;", *line, *col);
      return __EMPTY_STRING__();
}

/**
 * 读取到字符
 *
 * @param ch        第一个符号
 * @param reader    读取器
 * @param line      行号
 * @param col       列号
 * @return          读取到的字符串
 */
std::string lexc_read_character(char ch, xep_source_reader &reader, int *line, int *col)
{
      std::stringstream       buf;

      buf << ch;

      // 读取字符内容
      reader.look_ahead(&ch, line, col);
      buf << ch;

      /*
       * 如果读取到下划线，那么就是转义字符。它的转义字符有:
       *    \n  换行
       *    \r  回车
       *    \t  水平制表符
       *    \b  退格
       *    \f  换页
       *    \\  反斜杠
       *    \'  单引号
       *
       */
      if (ch == '\\') {
            reader.look_ahead(&ch, line, col);
            buf << ch;

            // 这里只是为了做异常处理，而判断的
            switch (ch) {
                  case 'a':
                  case 'n':
                  case 'r':
                  case 't':
                  case 'b':
                  case 'f':
                  case '\\':
                  case '\'': {
                        reader.look_ahead(&ch, line, col); // 再往下读一个字符，拿到最后的结束符号
                        break;
                  }

                  /* 非法字符 */
                  default: goto FLAG_INVALID_CHARACTER;
            }
      } else {
            reader.look_ahead(&ch, line, col);
      }

      // 这个字符必须是单引号，否则报错
      if (ch == '\'') {
            buf << ch;
            return buf.str();
      }

FLAG_INVALID_CHARACTER:
      char xnbuf[512];
      snprintf(xnbuf, sizeof(xnbuf), "lexc error: invalid character %s;", buf.str().c_str());
      xep_throw_error(xnbuf, *line, *col);
      return __EMPTY_STRING__();
}

/**
 * 特殊符号判断
 *
 * @param ch        当前读取到的字符
 * @param reader    源码读取器
 * @param eoimap    特殊符号映射表
 * @param kind      特殊符号类型指针，如果值为 KIND_NOP, 那么就什么都不做。
 * @param line      行号
 * @param col       列号
 * @return          token
 */
std::string xep_read_eoi(char ch, xep_source_reader &reader, eoimap_t &eoimap,
                         tokenkind_t *kind, int *line, int *col)
{
      std::string buftok;

      switch (ch) {
            case '+': {
                  switch (reader.peek_next()) {
                        case '+': {
                              reader.skip_next();
                              buftok = "++";
                              *kind = KIND_ADDADD;
                              return buftok;
                        }
                        case '=': {
                              reader.skip_next();
                              buftok = "+=";
                              *kind = KIND_ADDEQ;
                              return buftok;
                        }
                  }

                  goto JUMP_TO_DEFAULT;
            }

            case '-': {
                  switch (reader.peek_next()) {
                        case '-': {
                              reader.skip_next();
                              buftok = "--";
                              *kind = KIND_SUBSUB;
                              return buftok;
                        }

                        case '>': {
                              reader.skip_next();
                              buftok = "->";
                              *kind = KIND_ARROW;
                              return buftok;
                        }

                        case '=': {
                              reader.skip_next();
                              buftok = "-=";
                              *kind = KIND_SUBEQ;
                              return buftok;
                        }

                        default: {
                              /* 如果是数字，那么有可能是负数。交给数字解析函数去解析 */
                              if (isnumber(reader.peek_next())) {
                                    buftok = lexc_read_number(ch, reader, eoimap, line, col);
                                    *kind = KIND_NUMBER;
                                    return buftok;
                              }

                              /* 如果紧跟着字符，那么也有可能是负数变量。 比如：-x */
                              if (let(reader.peek_next())) {
                                    buftok.push_back(ch);
                                    while(!reader.look_ahead(&ch, line, col)) {
                                          // 读到了一个符号
                                          if (eoimap.count(ch)) {
                                                reader.back(line, col);
                                                *kind = KIND_NUMBER;
                                                return buftok;
                                          }

                                          buftok.push_back(ch);
                                    }
                              }

                        }
                  }

                  goto JUMP_TO_DEFAULT;
            }

            case '*': {
                  switch (reader.peek_next()) {
                        case '=': {
                              reader.skip_next();
                              buftok = "*=";
                              *kind = KIND_STAREQ;
                              return buftok;
                        }
                  }

                  goto JUMP_TO_DEFAULT;
            }

            case '/': {
                  switch (reader.peek_next()) {
                        case '=': {
                              reader.skip_next();
                              buftok = "/=";
                              *kind = KIND_SLASHEQ;
                              return buftok;
                        }

                              /* 读取到了单行注释，直接跳过这一行 */
                        case '/': {
                              reader.skip_line();
                              *kind = KIND_NOP;
                              return __EMPTY_STRING__();
                        }

                              /* 读到了多行注释，直接读到结束位置 */
                        case '*': {
                              while (!reader.look_ahead(&ch, line, col)) {
                                    if (ch == '*' && reader.peek_next() == '/') {
                                          reader.skip_next();
                                          *kind = KIND_NOP;
                                          return __EMPTY_STRING__();
                                    }
                              }
                        }
                  }

                  goto JUMP_TO_DEFAULT;
            }

            case '<': {
                  switch (reader.peek_next()) {
                        case '<': {
                              reader.skip_next();
                              buftok = "<<";
                              *kind = KIND_LSHIFT;
                              return buftok;
                        }

                        case '=': {
                              reader.skip_next();
                              buftok = "<=";
                              *kind = KIND_LTEQ;
                              return buftok;
                        }
                  }

                  goto JUMP_TO_DEFAULT;
            }

            case '>': {
                  switch (reader.peek_next()) {
                        case '>': {
                              reader.skip_next();
                              buftok = ">";
                              *kind = KIND_RSHIFT;
                              return buftok;
                        }

                        case '=': {
                              reader.skip_next();
                              buftok = ">=";
                              *kind = KIND_GTEQ;
                              return buftok;
                        }
                  }

                  goto JUMP_TO_DEFAULT;
            }

            case '!': {
                  switch (reader.peek_next()) {
                        case '=': {
                              reader.skip_next();
                              buftok = "!=";
                              *kind = KIND_NE;
                              return buftok;
                        }
                  }

                  goto JUMP_TO_DEFAULT;
            }

            case '=': {
                  switch (reader.peek_next()) {
                        case '=': {
                              reader.skip_next();
                              buftok = "==";
                              *kind = KIND_EQEQ;
                              return buftok;
                        }
                  }

                  goto JUMP_TO_DEFAULT;
            }

            case '|': {
                  switch (reader.peek_next()) {
                        case '|': {
                              reader.skip_next();
                              buftok = "||";
                              *kind = KIND_OROR;
                              return buftok;
                        }

                        case '=': {
                              reader.skip_next();
                              buftok = "|=";
                              *kind = KIND_OREQ;
                              return buftok;
                        }
                  }

                  goto JUMP_TO_DEFAULT;
            }

            case '&': {
                  switch (reader.peek_next()) {
                        case '&': {
                              reader.skip_next();
                              buftok = "&&";
                              *kind = KIND_ANDAND;
                              return buftok;
                        }

                        case '=': {
                              reader.skip_next();
                              buftok = "&=";
                              *kind = KIND_ANDEQ;
                              return buftok;
                        }
                  }

                  goto JUMP_TO_DEFAULT;
            }

            case '~': {
                  switch (reader.peek_next()) {
                        case '=': {
                              reader.skip_next();
                              buftok = "~=";
                              *kind = KIND_NOTEQ;
                              return buftok;
                        }
                  }

                  goto JUMP_TO_DEFAULT;
            }

            case '^': {
                  switch (reader.peek_next()) {
                        case '=': {
                              reader.skip_next();
                              buftok = "^=";
                              *kind = KIND_XOREQ;
                              return buftok;
                        }
                  }

                  goto JUMP_TO_DEFAULT;
            }

JUMP_TO_DEFAULT:
            default: {
                  buftok.push_back(ch);
                  eoimap[ch](kind);

                  if (*kind != KIND_NOP) {
                        return buftok;
                  }
            }
      }

      *kind = KIND_NOP;
      return __EMPTY_STRING__();
}

/**
 * 开始对源码做词法分析
 *
 * @param src     源码
 * @return        词法分析结果
 */
std::vector<struct token> xep_run_lexc(std::string &src)
{
      char                          ch;
      std::stringstream             buf;
      struct token                  tok;
      xep_source_reader             reader(src);
      int                           line;
      int                           col;
      std::vector<struct token>     tokens;
      lexmap_t                      lexmap;
      eoimap_t                      eoimap;
      tokenkind_t                   tokenkind;
      std::string                   buftok;

      /* 初始化词法MAP */
      init_lexc_map(&lexmap);
      init_eoic_map(&eoimap);

      /* 添加token方法 */
#define xep_push_token(strtok, kind)                              \
      xep_make_token(&tok, strtok, kind, line, col);              \
      tokens.push_back(tok);                                      \
      buftok.clear();                                             \
      clearbuf(buf);                                              \
      tokenkind = KIND_NOP;

      /* 循环读入字符 */
      while (!reader.look_ahead(&ch, &line, &col)) {
            // 判断是否是eoi符号
            bool eoi_ch = eoimap.count(ch) > 0;

            if (isspace(ch) || eoi_ch) {
                  buftok = buf.str();

                  // 如果是空格或者结束符，则认为是结束。添加当前缓存内容到token列表
                  if (buftok.length() > 0) {
                        if (lexmap.count(buftok)) {
                              lexmap[buftok](&tokenkind);
                        } else {
                              tokenkind = KIND_IDENTIFIER;
                        }

                        xep_push_token(buftok, tokenkind);
                  }

                  // 如果是结束符判断是不是特殊符号，比如：'=', '(', ')'等字符
                  if (eoi_ch) {
                        buftok = xep_read_eoi(ch, reader, eoimap, &tokenkind, &line, &col);
                        if (tokenkind != KIND_NOP) {
                              xep_push_token(buftok, tokenkind);
                        }
                  }

                  goto FLAG_LOOK_AHEAD_CONTINUE;
            }

            /* 读到char */
            if (ch == '\'') {
                  buftok = lexc_read_character(ch, reader, &line, &col);
                  xep_push_token(buftok, KIND_CHARACTER_LITERAL);
                  goto FLAG_LOOK_AHEAD_CONTINUE;
            }

            /* 读取到string */
            if (ch == '"') {
                  buftok = lexc_read_string(ch, reader, &line, &col);
                  xep_push_token(buftok, KIND_STRING_LITERAL);
                  goto FLAG_LOOK_AHEAD_CONTINUE;
            }

            /* 读到数字, 就一直循环往下读。知道读取到的内容不是数字为止 */
            if (isnumber(ch) && buf.str().length() == 0) {
                  buftok = lexc_read_number(ch, reader, eoimap, &line, &col);
                  xep_push_token(buftok, KIND_NUMBER);
                  goto FLAG_LOOK_AHEAD_CONTINUE;
            }

            buf << ch;

FLAG_LOOK_AHEAD_CONTINUE:
            continue;
      }

      return tokens;
}
