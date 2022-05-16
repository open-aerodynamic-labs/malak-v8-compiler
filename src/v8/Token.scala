/**
 * Copyright 2022 open-aerodynamic-airduck
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package v8

import v8.Token.TokenKind

/**
 * token分类
 */
object Token extends Enumeration {
  type TokenKind = Value
  val
  /* 单个字符 token */
  kLeftParen, kRightParen,              // ( )
  kLeftBracket, kRightBracket,          // [ ]
  kLeftBrace, kRightBrace,              // { }
  kComma, kDot,                         // , .
  kPlus, kMinus,                        // + -
  kSemicolon, kSlash, kStar,            // ; / *

  // 一个或多个字符 token
  kBang, kBangEqual,                    // ! !=
  kEqual, kEqualEqual,                  // = ==
  kGreater, kGreaterEqual,              // > >=
  kLess, kLessEqual,                    // < <=

  // 字面量
  kIdentifier, kString, kNumber,         // 标识符、字符串和数字

  // 关键字.
  kClass, kEnum, kInterface, kFunc, kVar, kConst,
  kThis, kSuper, kIf, Else, kFor, kWhile,
  kSwitch, kCase, kDefault, kBreak, kContinue,
  kReturn, kGoto, kImport, kExport, kIn, kStatic,
  kInt, kLong, kShort, kFloat, kDouble, kBool, kChar,
  kTrue, kFalse, kNull, kStruct, kSizeof, kInit,
  kValueof, kTypedef,

  kComment, kEoi, kSpace, kError, kUnknown, kNop,

  kEof = Value

}

/**
 * 词法解析器token
 */
class Token(val value: String, val kind: TokenKind, val line: Int, val col: Int) {
  override def toString = kind.toString + ":" + value
}