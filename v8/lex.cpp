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

#define clearbuf(x) x.str("")

/** 词法解析回调函数，获取当前token kind */
typedef void (*f_lexc)(tokenkind_t *);

/**
 *   词法解析器map，用于方便匹配关键字等字符。
 * 相当于就是高级语言中对string做switch。
 */
typedef std::map<std::string, f_lexc>     lexmap;
typedef std::map<char, f_lexc>            eoimap;

#define epc_add_lexc(map, key, kind) (map)->insert(std::make_pair(key, [](tokenkind_t *tk) { *tk = kind; }))

/**
 * 解析阶段
 */
#define PHASE_INTEGER 1
#define PHASE_DECIMAL 2

#define error_invalid_number(line, col) throw std::runtime_error("lexical error: invalid number")

/** 初始化词法MAP */
void init_lexc_map(lexmap *map)
{
      epc_add_lexc(map, "var", KIND_VAR);
      epc_add_lexc(map, "char", KIND_CHAR);
      epc_add_lexc(map, "int", KIND_INT);
      epc_add_lexc(map, "long", KIND_LONG);
      epc_add_lexc(map, "float", KIND_FLOAT);
      epc_add_lexc(map, "double", KIND_DOUBLE);
}

/** 初始化符号MAP */
void init_eoic_map(eoimap *map)
{
      epc_add_lexc(map, ' ', KIND_NOP);
      epc_add_lexc(map, '=', KIND_EQ);
      epc_add_lexc(map, '+', KIND_ADD);
      epc_add_lexc(map, '-', KIND_SUB);
      epc_add_lexc(map, '*', KIND_STAR);
      epc_add_lexc(map, '/', KIND_SLASH);
      epc_add_lexc(map, ';', KIND_EOI);
      epc_add_lexc(map, ':', KIND_NOP);
      epc_add_lexc(map, '\0', KIND_EOF);
}

/**
 * 开始对源码做词法分析
 */
std::vector<struct token> lexps(std::string &src)
{
      char                          ch;
      std::stringstream             buf;
      struct token                  tok;
      SourceReader                  reader(src);
      int                           line;
      int                           col;
      std::vector<struct token>     tokens;
      lexmap                        lexc;
      eoimap                        eoic;
      tokenkind_t                   tokenkind;
      std::string                   buftok;

      /* 初始化词法MAP */
      init_lexc_map(&lexc);
      init_eoic_map(&eoic);

      /* 添加token方法 */
#define epc_push_token(strtok, kind)                              \
      epc_make_token(&tok, strtok, kind, line, col);              \
      tokens.push_back(tok);                                      \
      buftok.clear();                                             \
      clearbuf(buf);                                              \
      tokenkind = KIND_NOP;

      /* 循环读入字符 */
      while (!reader.look_ahead(&ch, &line, &col)) {
            // 判断是否是eoi符号
            bool __exist_eoi = eoic.count(ch) > 0;

            if (isspace(ch) || __exist_eoi) {
                  buftok = buf.str();

                  // 如果是空格或者结束符，则认为是结束。添加当前缓存内容到token列表
                  if (buftok.length() > 0) {
                        if (lexc.count(buftok)) {
                              lexc[buftok](&tokenkind);
                        } else {
                              tokenkind = KIND_IDENTIFIER;
                        }

                        epc_push_token(buftok, tokenkind);
                  }

                  // 如果是结束符判断是不是特殊符号，比如：'=', '(', ')'等字符
                  if (__exist_eoi) {
                        buftok.push_back(ch);
                        eoic[ch](&tokenkind);

                        if (tokenkind != KIND_NOP) {
                              epc_push_token(buftok, tokenkind);
                        }
                  }

                  continue;
            }

            /* 读到数字, 就一直循环往下读。知道读取到的内容不是数字为止 */
            if (isnumber(ch) && buf.str().length() == 0) {
                  buf << ch;
                  int phase = PHASE_INTEGER;
                  while(!reader.look_ahead(&ch, &line, &col)) {
                        if (isnumber(ch)) {
                              buf << ch;
                              continue;
                        }

                        if (ch == '.') {
                              if (phase == PHASE_DECIMAL)
                                    error_invalid_number(line, col);

                              buf << ch;
                              phase = PHASE_DECIMAL;
                              continue;
                        }

                        buf >> buftok;

                        if (phase == PHASE_INTEGER) {
                              // long
                              if (ch == 'L' || ch == 'l') {
                                    buftok.insert(0, "L");
                                    goto FLAG_READ_NUMBER_BREAK;
                              }

                              buftok.insert(0, "I");
                              goto FLAG_READ_NUMBER_CHECK_EOI_BREAK;
                        } else {
                              // float
                              if (ch == 'F' || ch == 'f') {
                                    buftok.insert(0, "F");
                                    goto FLAG_READ_NUMBER_BREAK;
                              }

                              buftok.insert(0, "F");
                              goto FLAG_READ_NUMBER_CHECK_EOI_BREAK;
                        }

FLAG_READ_NUMBER_CHECK_EOI_BREAK:
                        if (!eoic.count(ch))
                              error_invalid_number(line, col);
FLAG_READ_NUMBER_BREAK:
                        reader.back(&line, &col);
                        epc_push_token(buftok, KIND_NUMBER);
                        break;
                  }
                  continue;
            }

            buf << ch;
      }

      /* 遍历token */
      for (auto &itok : tokens) {
            epc_print_token(itok);
      }

      return tokens;
}
