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

#define nextchar()  ++code
#define nextline()  { ++line; goto CODE_LEX_NEXT_CHAR; }
#define puttok(x)   tok[pos] = x; ++pos
#define resettok()  memset(tok, 0, sizeof(tok)); pos = 0
#define eoi(ch)     (ch == ';')

/** --------------------- 编译状态 ---------------------- */

#define VK_PHASE_NOP          0     /* 什么都不做 */
#define VK_PHASE_NEXT_CHR     1     /* 扫描TOK中 */
#define VK_PHASE_TOK          2     /* 扫描到一个tok */
#define VK_PHASE_EOI          3     /* 扫描到结束符 */

/** --------------------------------------------------- */

/** 当前字符是tok还是文本 */
int let(char ch)
{
      return (ch >= 'a' && ch <= 'z') ||
             (ch >= 'A' && ch <= 'Z') || ch == '$';
}

token_t *codelex(const char *code)
{
      /* 一个完整的token文本 */
      char        tok[255];
      unsigned    pos = 0;
      int         line = 0;
      tokkind_t   tokkind;
      char        ch;
      int         phase = VK_PHASE_NEXT_CHR;

      /* 循环读取源码 */
      while((ch = *code) != '\0')
      {
            if (ch == '\n')
                  nextline();

            if (isspace(ch))
                  phase = VK_PHASE_TOK;

            if (eoi(ch)) {
                  phase = VK_PHASE_EOI;
            }

            switch(phase) {
                  /* 下一个字符 */
                  case VK_PHASE_NEXT_CHR: {
                        puttok(ch);
                        goto CODE_LEX_NEXT_CHAR;
                  }
                  /* 扫描到tok */
                  case VK_PHASE_TOK: {
                        printf("tok='%s', line=%d\n", tok, line);
                        resettok();
                        phase = VK_PHASE_NEXT_CHR;
                        goto CODE_LEX_NEXT_CHAR;
                  }
            }

            /** next char */
CODE_LEX_NEXT_CHAR:
            nextchar();
      }

      tokkind = VK_TOK_EOF;

      return NULL;
}