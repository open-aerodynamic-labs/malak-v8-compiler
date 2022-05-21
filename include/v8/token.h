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
      enum tokenkind kind;
      std::string value;
      int line;
      int col;
};

/** 创建一个token */
inline static struct token *epc_make_token(std::string &v, enum tokenkind kind, int line, int col)
{
      auto *t = new struct token;
      t->kind = kind;
      t->value = v;
      t->line = line;
      t->col = col;
      return t;
}

/** 释放一个token */
#define epc_free_token(p_token) delete p_token

/** 打印token */
#define epc_print_token(p_token) printf("<%d, %s>\n", p_token->kind, p_token->value.c_str())

#endif /* __EPOCH_SYNTAX_TOKEN_H__ */