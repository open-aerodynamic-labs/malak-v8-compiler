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
#ifndef __EPOCH_SYNTAX_TOKEN_H__
#define __EPOCH_SYNTAX_TOKEN_H__

#include <string>
#include <stdexcept>

/* 关键字 */
#define KIND_LET                    1001
#define KIND_FUN                    1002
#define KIND_CHAR                   1003
#define KIND_INT                    1004
#define KIND_LONG                   1005
#define KIND_FLOAT                  1006
#define KIND_DOUBLE                 1007
#define KIND_VARARGS                1008
#define KIND_ANY                    1009
#define KIND_RETURN                 1010
#define KIND_GOTO                   1011
#define KIND_OBJECT                 1012
#define KIND_THIS                   1013
#define KIND_STATIC                 1014
#define KIND_CONST                  1015
#define KIND_IF                     1016
#define KIND_ELSE                   1017
#define KIND_ELIF                   1018
#define KIND_FOR                    1019
#define KIND_WHILE                  1020
#define KIND_DO                     1021
#define KIND_SWITCH                 1022
#define KIND_CASE                   1023
#define KIND_DEFAULT                1024
#define KIND_BREAK                  1025
#define KIND_CONTINUE               1026
#define KIND_SIZEOF                 1027
#define KIND_NULL                   1028

/* 字面量 */
#define KIND_IDENTIFIER             2001
#define KIND_STRING                 2002
#define KIND_NUMBER                 2003
#define KIND_STRING_LITERAL         2004
#define KIND_CHARACTER_LITERAL      2005

/* 符号 */
#define KIND_EQ                     3000
#define KIND_NE                     3001
#define KIND_ADD                    3002
#define KIND_SUB                    3003
#define KIND_STAR                   3004
#define KIND_SLASH                  3005
#define KIND_OPEN_PAREN             3006
#define KIND_CLOSE_PAREN            3007
#define KIND_OPEN_BRACE             3008
#define KIND_CLOSE_BRACE            3009
#define KIND_DOT                    3010
#define KIND_COMMA                  3011
#define KIND_OPEN_BRACKET           3012
#define KIND_CLOSE_BRACKET          3013
#define KIND_COLON                  3014
#define KIND_ADDADD                 3015
#define KIND_SUBSUB                 3016
#define KIND_ARROW                  3017
#define KIND_ADDEQ                  3018
#define KIND_SUBEQ                  3019
#define KIND_STAREQ                 3020
#define KIND_SLASHEQ                3021
#define KIND_GT                     3022
#define KIND_LT                     3023
#define KIND_GTEQ                   3024
#define KIND_LTEQ                   3025
#define KIND_LSHIFT                 3026
#define KIND_RSHIFT                 3027
#define KIND_EQEQ                   3028
#define KIND_OR                     3029
#define KIND_AND                    3030
#define KIND_NOT                    3031
#define KIND_XOR                    3032
#define KIND_OROR                   3033
#define KIND_OREQ                   3034
#define KIND_ANDAND                 3035
#define KIND_ANDEQ                  3036
#define KIND_NOTEQ                  3037
#define KIND_XOREQ                  3038

#define KIND_NOP                       0
#define KIND_EOI                       1
#define KIND_EOF                       2

typedef unsigned tokenkind_t;

/**
 * token
 */
struct token {
      tokenkind_t       kind;
      std::string       value;
      int               lineno;
      int               colno;
};

/** 创建一个token */
inline static void xep_make_token(struct token *p_token, std::string &v, tokenkind_t kind,
                                  int line,
                                  int col)
{
      p_token->kind = kind;
      p_token->value = v;
      p_token->lineno = line;
      p_token->colno = col;
}

/**
 * 抛出异常
 */
inline static void xep_throw_error(std::string fmt, int line, int col, ...)
{
      fmt = fmt + " (line=%d, col=%d)\n";
      char buf[512];
      snprintf(buf, sizeof(buf), fmt.c_str(), line, col);
      throw std::runtime_error(buf);
}

/** 打印token */
#define xep_print_token(p_token) printf("<%d, %s>\n", (p_token).kind, (p_token).value.c_str())

#endif /* __EPOCH_SYNTAX_TOKEN_H__ */