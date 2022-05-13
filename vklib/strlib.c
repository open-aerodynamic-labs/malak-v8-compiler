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
#include "vklib/strlib.h"
#include "vklib/log.h"
#include "vklib/vkmem.h"
#include <string.h>
#include <malloc.h>

char *strreplace(const char *str, const char *tok, const char *rep)
{
      char*   replaced;
      int     reppos    = 0;
      size_t  srclen    = strlen(str);
      size_t  toklen    = strlen(tok);
      size_t  replen    = strlen(rep);
      int     curpos    = 0;
      int     offsetlen = 4;
      int*    tok_offset= malloc(sizeof(int) * offsetlen);
      int     curoffset = 0;

      for (unsigned int i = 0; i < srclen; i++) {
            int findtok = str[i] == tok[curpos];

            if (findtok)
                  ++curpos;

            /* 如果下一个字符串不正确就重置 */
            if (curpos > 0 && !findtok) {
                  curpos = 0;
                  i -= 1;
                  continue;
            }

            /* 扫描到了一个完整的tok, 记录下偏移量 */
            if (curpos == toklen) {
                  curpos = 0;

                  /* 如果偏移量数组不够了重新扩容 */
                  if (curoffset >= offsetlen) {
                        offsetlen = offsetlen * 4;
                        tok_offset = realloc(tok_offset, sizeof(int) * offsetlen);
                  }

                  tok_offset[curoffset] = (i + 1) - toklen;
                  ++curoffset;
            }
      }

      /* 计算替换后的字符串空间大小 */
      replaced = (char *) malloc((srclen - (toklen * curoffset)) + (replen * curoffset));

      for (int i = 0; i < curoffset; i++) {
            int offset = tok_offset[i];
            int cpylen = offset - curpos;
            xmemcpy(replaced, reppos, str, curpos, cpylen);
            curpos += cpylen;
            reppos += cpylen;
            xmemcpy(replaced, reppos, rep, 0, replen);
            reppos += replen;
            curpos += toklen;
      }

      int rem = srclen - curpos;
      xmemcpy(replaced, reppos, str, srclen - rem, rem);
      xmemcpy(replaced, reppos + rem, "\0", 0, 1);

      printf("replaced: %s\n", replaced);
      return replaced;
}