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

#include "tokenkind.h"
#include <string>

/**
 * token
 */
struct token {
      tokenkind_t kind;
      std::string value;
      int line;
      int col;
};

/** 创建一个token */
inline static void epc_make_token(struct token *p_token, std::string &v, tokenkind_t kind,
                                  int line,
                                  int col)
{
      p_token->kind = kind;
      p_token->value = v;
      p_token->line = line;
      p_token->col = col;
}

/**
 * 添加符号token
 * @return true表示是一个有用的符号，false表示是一个无用的符号
 */
inline static bool epc_make_eoitok(struct token *p_token, char ch, int line, int col)
{
      // char 转 std::string
      std::string tokvalue;
      tokvalue.push_back(ch);

      tokenkind_t tokenkind;

      switch (ch) {
            case '=': {
                  tokenkind = KIND_EQ;
                  goto FLAG_EPC_MAKE_EOITOK_END;
            }
            case ';': {
                  tokenkind = KIND_EOF;
                  goto FLAG_EPC_MAKE_EOITOK_END;
            }
            default: return false;
      }

FLAG_EPC_MAKE_EOITOK_END:
      epc_make_token(p_token, tokvalue, tokenkind, line, col);
      return true;
}

/** 打印token */
#define epc_print_token(p_token) printf("<%d, %s>\n", (p_token).kind, (p_token).value.c_str())

#endif /* __EPOCH_SYNTAX_TOKEN_H__ */