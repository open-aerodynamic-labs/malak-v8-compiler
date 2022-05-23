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
#ifndef __EPOCH_TOKENKIND_H__
#define __EPOCH_TOKENKIND_H__

/* 关键字 */
#define KIND_VAR                      1001
#define KIND_FUNC                     1002
#define KIND_CHAR                     1003
#define KIND_INT                      1004
#define KIND_LONG                     1005
#define KIND_FLOAT                    1006
#define KIND_DOUBLE                   1007

/* 字面量 */
#define KIND_IDENTIFIER               2001
#define KIND_STRING                   2002
#define KIND_NUMBER                   2003
#define KIND_STRING_LITERAL           2004
#define KIND_CHARACTER_LITERAL        2005

/* 符号 */
#define KIND_EQ                       3001
#define KIND_ADD                      3002
#define KIND_SUB                      3003
#define KIND_STAR                     3004
#define KIND_SLASH                    3005
#define KIND_OPEN_PAREN               3006
#define KIND_CLOSE_PAREN              3007
#define KIND_OPEN_BRACE               3008
#define KIND_CLOSE_BRACE              3009

#define KIND_NOP                         0
#define KIND_EOI                         1
#define KIND_EOF                         2

typedef unsigned tokenkind_t;

#endif /* __EPOCH_TOKENKIND_H__ */
