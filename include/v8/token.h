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
#include <stdexcept>

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
 * 抛出异常
 */
inline static void epc_throw_error(std::string fmt, int line, int col)
{
      fmt = fmt + " (line=%d, col=%d)\n";
      char buf[512];
      snprintf(buf, sizeof(buf), fmt.c_str(), line, col);
      throw std::runtime_error(buf);
}

/** 打印token */
#define epc_print_token(p_token) printf("<%d, %s>\n", (p_token).kind, (p_token).value.c_str())

#endif /* __EPOCH_SYNTAX_TOKEN_H__ */