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
#ifndef __EPOCH_AST_H__
#define __EPOCH_AST_H__

#include "token.h"

struct xep_string_node {
      struct token * token;
      char         * value;
};

struct xep_int32_node {
      struct token * token;
      int32_t        value;
};

struct xep_int64_node {
      struct token * token;
      int64_t        value;
};

struct xep_float_node {
      struct token * token;
      float          value;
};

struct xep_double_node {
      struct token * token;
      double         value;
};

struct xep_bool_node {
      struct token * token;
      char           value;
};

struct xep_char_node {
      struct token * token;
      char           value;
};

struct xep_operate_node {
      struct token * token;
      char           value;
      void *         left;
      void *         right;
};

#endif /* __EPOCH_AST_H__ */
