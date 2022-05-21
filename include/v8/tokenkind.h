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

/**
 * token类别
 */
enum tokenkind {
      /* 关键字 */
      kVar, kChar, kInt, kLong, kFloat, kDouble,

      /* 字面量 */
      kIdentifier, kString, kNumber,

      /* 操作符 */
      kEq, kPlus, kMinus, kStar, kSlash,

      kEoi, kEof
};

#endif /* __EPOCH_TOKENKIND_H__ */
