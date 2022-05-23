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

#define KIND_VAR                      1
#define KIND_CHAR                     2
#define KIND_INT                      3
#define KIND_LONG                     4
#define KIND_FLOAT                    5
#define KIND_DOUBLE                   6
#define KIND_IDENTIFIER               7
#define KIND_STRING                   8
#define KIND_NUMBER                   9
#define KIND_EQ                      10
#define KIND_ADD                     11
#define KIND_SUB                     12
#define KIND_STAR                    13
#define KIND_SLASH                   14
#define KIND_NOP                     15
#define KIND_EOI                     16
#define KIND_EOF                     17
#define KIND_SPACE                   18

typedef unsigned tokenkind_t;

#endif /* __EPOCH_TOKENKIND_H__ */
