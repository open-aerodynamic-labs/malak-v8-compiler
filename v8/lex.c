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
#include "lex.h"

#define VK_SYMBOL_EQ    "="
#define VK_SYMBOL_ADD   "+"
#define VK_SYMBOL_SUB   "-"
#define VK_SYMBOL_MUL   "*"
#define VK_SYMBOL_DIV   "/"

#define VK_STATE_NOP    0x00000
#define VK_STATE_INT    0x00001

void _addtok(struct token* arrtok, int *idxtok, const char *value, kind_t kind, int curline)
{
      struct token* tok;

      tok = &arrtok[*idxtok];
      xstrcmp(tok->value, "x");
      tok->kind = VK_KIND_ADD;
      tok->line = curline;

      *idxtok = *idxtok + 1;
}

struct token *lexps(const char *code, size_t *len)
{
      char              ch;
      struct token*     arrtok;
      int               idxtok = 0;
      int               curline = 1;
      unsigned          cur_state = VK_STATE_NOP;
      unsigned          prev_state = VK_STATE_NOP;
      char              value[255];
      int               idxval = 0;

/* 清空字符串数组 */
#define cleanv()        memset(value, 0, sizeof(value)); idxval = 0
/* 添加tok并清空字符串数组 */
#define addtok(v, k, l) _addtok(arrtok, &idxtok, v, k, l); cleanv()
/* 添加一个字符到字符串数组 */
#define putvalue(ch)    value[idxval] = ch; ++idxval
/* while循环下一个字符 */
#define nextchar(x)     ++x

      arrtok = malloc(sizeof(struct token) * 20);

      while ((ch = *code) != '\0') {

            if (ch == '\n') {
                  ++curline;
                  goto NEXT_CHAR_FLAG;
            }

            switch (ch) {
                  case ' ': {
                        if (xstrcmp(value, "int")) {
                              cur_state = VK_STATE_INT;
                              addtok(value, VK_KIND_INT, curline);
                              goto NEXT_CHAR_FLAG;
                        }
                  }

                  default: {
                        putvalue(ch);
                        goto NEXT_CHAR_FLAG;
                  }
            }

NEXT_CHAR_FLAG:
            nextchar(code);
      }

      return NULL;
}