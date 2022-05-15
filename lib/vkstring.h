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
#ifndef __V8_STRLIB_H__
#define __V8_STRLIB_H__

#include "vklib/vkmem.h"

/** 是不是空格 */
inline static int isspc(char ch)
{
      return ch == ' ';
}

/** 字符串比较 */
inline static int xstrcmp(const char *x, const char *y)
{
      return strcmp(x, y) == 0;
}

/** 是不是数字 */
int isnumber(const char *str);
/** 字符串替换 */
void strreplace(char *dest, const char *str, const char *tok, const char *rep);

#endif /* __V8_STRLIB_H__ */