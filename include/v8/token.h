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
#ifndef __VARUKA_SYNTAX_TOKEN_H__
#define __VARUKA_SYNTAX_TOKEN_H__

#include <string>

/**
 * token类别
 */
enum tokenkind {
      /* 关键字 */
      Var, Char, Int, Long, Float, Double,

      /* 字面量 */
      Identifier, String, Number,

      /* 操作符 */
      Eq, Plus, Minus, Star, Slash,

      Eoi, Eof
};

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
inline static struct token *v8_make_token(std::string &v, enum tokenkind kind, int line, int col);
/** 释放一个token */
inline static void v8_free_token(struct token *t);
/** 打印token */
inline static void v8_print_token(struct token *t);

#endif /* __VARUKA_SYNTAX_TOKEN_H__ */
