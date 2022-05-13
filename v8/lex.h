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
#ifndef __V8_CMP_LEXPAS_H__
#define __V8_CMP_LEXPAS_H__

#include "vklib/log.h"
#include "vklib/vkstring.h"
#include <io.h>

/** token类型定义 */
#define tokkind_t unsigned

#define VK_TOK_NUM 0x000001
#define VK_TOK_ADD 0x000010
#define VK_TOK_SUB 0x000011
#define VK_TOK_MUL 0x000100
#define VK_TOK_DIV 0x000101
#define VK_TOK_EOF 0x000111
#define VK_TOK_EOI 0x001000

typedef struct token {
      tokkind_t   kind;
      char        value[255];
} token_t;

/**
 * 词法解析
 * @param code 源码
 */
token_t *codelex(const char *code);

#endif /* __V8_CMP_LEXPAS_H__ */