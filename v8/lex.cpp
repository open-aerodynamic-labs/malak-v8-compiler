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
typedef std::map<std::string, f_lexc> lexmap;

/**
 * 符号map
 */
typedef std::map<char, f_lexc> eoimap;

#define epc_add_lexc(map, key, kind) (map)->insert(std::make_pair(key, [](tokenkind_t *tk) { *tk = kind; }))

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
      epc_add_lexc(map, '=', KIND_EQ);
      epc_add_lexc(map, '+', KIND_ADD);
      epc_add_lexc(map, '-', KIND_SUB);
      epc_add_lexc(map, '*', KIND_STAR);
      epc_add_lexc(map, '/', KIND_SLASH);
      epc_add_lexc(map, ';', KIND_EOI);
      epc_add_lexc(map, ':', KIND_UNKNOWN);
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
      SourceReader                  source_reader(src);
      int                           line;
      int                           col;
      std::vector<struct token>     tokens;
      lexmap                        lexc;
      eoimap                        eoic;
      tokenkind_t                   tokenkind;

      /* 初始化词法MAP */
      init_lexc_map(&lexc);
      init_eoic_map(&eoic);

      /* 循环读入字符 */
      while (!source_reader.look_ahead(&ch, &line, &col)) {
            // 判断是否是eoi符号
            bool __exist_eoi = eoic.count(ch) > 0;

            if (isspace(ch) || __exist_eoi) {
                  auto buftok = buf.str();

                  // 如果是空格或者结束符，则认为是结束。添加当前缓存内容到token列表
                  if (buftok.length() > 0) {
                        if (lexc.count(buftok)) {
                              lexc[buftok](&tokenkind);
                        } else {
                              tokenkind = KIND_IDENTIFIER;
                        }

                        epc_make_token(&tok, buftok, tokenkind, line, col);
                        tokens.push_back(tok);
                  }

                  // 如果是结束符判断是不是特殊符号，比如：'=', '(', ')'等字符
                  if (__exist_eoi) {
                        std::string v;
                        v.push_back(ch);

                        eoic[ch](&tokenkind);

                        if (tokenkind != KIND_UNKNOWN) {
                              epc_make_token(&tok, v, tokenkind, line, col);
                              tokens.push_back(tok);
                        }
                  }

                  clearbuf(buf);
                  continue;
            }

            /* 读到数字 */
            if (isnumber(ch) && buf.str().length() == 0) {
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
