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

/** -------------------- token类型定义 ----------------------- */

typedef unsigned kind_t;

#define VK_KIND_NUM 0x000001
#define VK_KIND_ADD 0x000010
#define VK_KIND_SUB 0x000011
#define VK_KIND_MUL 0x000100
#define VK_KIND_DIV 0x000101
#define VK_KIND_EOF 0x000111
#define VK_KIND_EOI 0x001000
#define VK_KIND_INT 0x001001

/** ------------------------------------------------------ */

typedef struct token {
      kind_t   kind;
      int      line;
      char     value[255];
} tok_t;

/**
 * 词法解析
 * @param code 源码
 */
tok_t *lexps(const char *code);

#endif /* __V8_CMP_LEXPAS_H__ */