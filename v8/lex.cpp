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

/** 词法解析回调函数，获取当前tokenkind */
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

/** 初始化词法解析器map */
void init_lexc_map(lexmap *map)
{
      map->insert(std::make_pair("var", [](tokenkind_t *tk) { *tk = KIND_VAR; }));
      map->insert(std::make_pair("char", [](tokenkind_t *tk) { *tk = KIND_CHAR; }));
      map->insert(std::make_pair("int", [](tokenkind_t *tk) { *tk = KIND_INT; }));
      map->insert(std::make_pair("long", [](tokenkind_t *tk) { *tk = KIND_LONG; }));
      map->insert(std::make_pair("float", [](tokenkind_t *tk) { *tk = KIND_FLOAT; }));
      map->insert(std::make_pair("double", [](tokenkind_t *tk) { *tk = KIND_DOUBLE; }));
}

/** 初始化eoi符号解析器map */
void init_eoic_map(eoimap *map)
{
      map->insert(std::make_pair('=', [](tokenkind_t *tk) { *tk = KIND_EQ; }));
      map->insert(std::make_pair('+', [](tokenkind_t *tk) { *tk = KIND_PLUS; }));
      map->insert(std::make_pair('-', [](tokenkind_t *tk) { *tk = KIND_MINUS; }));
      map->insert(std::make_pair('*', [](tokenkind_t *tk) { *tk = KIND_STAR; }));
      map->insert(std::make_pair('/', [](tokenkind_t *tk) { *tk = KIND_SLASH; }));
      map->insert(std::make_pair(';', [](tokenkind_t *tk) { *tk = KIND_EOI; }));
      map->insert(std::make_pair(':', [](tokenkind_t *tk) { *tk = KIND_UNKNOWN; }));
      map->insert(std::make_pair('\0', [](tokenkind_t *tk) { *tk = KIND_EOF; }));
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

      /* 初始化词法解析器map */
      init_lexc_map(&lexc);
      init_eoic_map(&eoic);

      /* 循环读入字符 */
      while (!source_reader.look_ahead(&ch, &line, &col)) {
            bool contains_eoi = eoic.count(ch) > 0;
            if (isspace(ch) || contains_eoi) {
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
                  if (contains_eoi) {
                        std::string v;
                        v.push_back(ch);

                        eoic[ch](&tokenkind);

                        epc_make_token(&tok, v, tokenkind, line, col);
                        tokens.push_back(tok);
                  }

                  clearbuf(buf);
                  continue;
            }

            buf << ch;
      }

      for (auto &itok : tokens) {
            epc_print_token(itok);
      }

      return tokens;
}
