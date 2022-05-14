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
#define puttok(x)   tokident[pos] = x; ++pos
#define resettok()  memset(tokident, 0, sizeof(tokident)); pos = 0
#define eoi(ch)     (ch == ';')

/** --------------------- 编译状态 ---------------------- */

#define VK_PHASE_NOP          0     /* 什么都不做 */
#define VK_PHASE_NEXT_CHR     1     /* 扫描TOK中 */
#define VK_PHASE_TOK          2     /* 扫描到一个tok */
#define VK_PHASE_EOI          3     /* 扫描到结束符 */

/** ----------------------- 符号 ------------------------ */

#define setkind(p_kind, kind, p_state, state) { *p_kind = kind; *p_state = state; return; }

#define VK_SYMBOL_ADD    "+"
#define VK_SYMBOL_SUB    "-"
#define VK_SYMBOL_MUL    "*"
#define VK_SYMBOL_DIV    "/"
#define VK_SYMBOL_EOI    ";"
#define VK_SYMBOL_INT    "int"

/** --------------------- 当前状态 ---------------------- */

typedef unsigned state_t;

#define VK_STATE_ERR             0     /* 错误tok */
#define VK_STATE_NOP             1     /* 无状态（默认状态）*/
#define VK_STATE_STATEMENT_INT   2     /* 声明一个int变量 */

/** 检索tok类型 */
void search_state(const char *tok, kind_t *p_kind, state_t *p_state)
{
      if (xstrcmp(tok, VK_SYMBOL_INT))
            setkind(p_kind, VK_KIND_INT, p_state, VK_STATE_STATEMENT_INT)
}

/** 识别tok分类 */
void scankind(const char *tok, kind_t *kind)
{
      static state_t state = VK_STATE_NOP;

      /* 根据状态推导 */
      switch (state) {
            case VK_STATE_NOP: {
                  search_state(tok, kind, &state);
                  goto SCANKIND_OUT;
            }

            /*
             * 如果当前tok是声明一个int变量，可以推导出以下写法：
             *
             *    int x;
             *    int x = 0;
             *    int x = y + x ... + n;
             *
             */
            case VK_STATE_STATEMENT_INT: {
            }

      }

SCANKIND_OUT:
      if (state != VK_STATE_ERR)
            return;
}

/**
 * 添加一个新扫描到的tok
 *
 * @param arrtok    tok数组
 * @param tokidx    tok数组当前下标
 * @param line      tok行数
 * @param value     tok字符
 */
void addtok(tok_t *arrtok, int *tokidx, int line, const char *value)
{
      static kind_t lastkind;

      scankind(value, &lastkind);

      tok_t *elem = &arrtok[*tokidx];
      elem->line = line;
      elem->kind = 0;
      strcpy(elem->value, value);
      *tokidx = *tokidx + 1;
}

/**
 * 词法解析器
 * @param code 源码字符串
 */
tok_t *lexps(const char *code)
{
      /* 一个完整的token文本 */
      char        tokident[255];
      unsigned    pos = 0;
      int         line = 0;
      int         phase = VK_PHASE_NEXT_CHR;
      tok_t*      arrtok = malloc(sizeof(tok_t) * 20); /* token数组 */
      int         tokidx = 0;
      char        ch;

      /* 循环读取源码 */
      while((ch = *code) != '\0')
      {
            if (ch == '\n')
                  nextline();

            if (isspc(ch))
                  phase = VK_PHASE_TOK;

            /* 如果是结束符 */
            if (eoi(ch))
                  phase = VK_PHASE_EOI;

            switch(phase) {
                  /* 下一个字符 */
                  case VK_PHASE_NEXT_CHR: {
                        puttok(ch);
                        goto CODE_LEX_NEXT_CHAR;
                  }
                  /* 扫描到tok */
                  case VK_PHASE_TOK: {
                        addtok(arrtok, &tokidx, line, tokident);
                        resettok();
                        phase = VK_PHASE_NEXT_CHR;
                        goto CODE_LEX_NEXT_CHAR;
                  }
            }

            /** next char */
CODE_LEX_NEXT_CHAR:
            nextchar();
      }

      for (int i = 0; i < tokidx; i++) {
            tok_t tok = arrtok[i];
            printf("tok='%s', line=%d, kind=%d\n", tok.value, tok.line, tok.kind);
      }

      return NULL;
}