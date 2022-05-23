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

#define clearbuf(x) x.str("")

std::vector<struct token> lexps(std::string &src)
{
      char                          ch;
      std::stringstream             buf;
      struct token                  tok;
      SourceReader                  source_reader(src);
      int                           line;
      int                           col;
      std::vector<struct token>     tokens;
      bool                          eoi;

      /* 循环读入字符 */
      while (!source_reader.look_ahead(&ch, &line, &col)) {
            if (isspace(ch) || (eoi = iseoi(ch))) {
                  auto buftok = buf.str();

                  // 如果是空格或者结束符，则认为是结束。添加当前缓存内容到token列表
                  if (buftok.length() > 0) {
                        tokenkind_t tokenkind;

                        if (buftok == "var") {
                              tokenkind = KIND_VAR;
                        } else if (buftok == "int") {
                              tokenkind = KIND_INT;
                        } else {
                              tokenkind = KIND_IDENTIFIER;
                        }

                        epc_make_token(&tok, buftok, tokenkind, line, col);
                        tokens.push_back(tok);
                  }

                  // 如果是结束符判断是不是特殊符号，比如：'=', '(', ')'等字符
                  if (eoi && epc_make_eoitok(&tok, ch, line, col))
                        tokens.push_back(tok);

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
