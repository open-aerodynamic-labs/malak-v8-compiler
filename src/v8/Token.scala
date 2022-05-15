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
  LeftParen, RightParen,              // ( )
  LeftBracket, RightBracket,          // [ ]
  LeftBrace, RightBrace,              // { }
  Comma, Dot,                         // , .
  Plus, Minus,                        // + -
  Semicolon, Slash, Star,             // ; / *

  // 一个或多个字符 token
  Bang, BangEqual,                    // ! !=
  Equal, EqualEqual,                  // = ==
  Greater, GreaterEqual,              // > >=
  Less, LessEqual,                    // < <=

  // 字面量
  Identifier, String, Number,         // 标识符、字符串和数字

  // 关键字.
  Class, Enum, Interface, Func, Var, Const,
  This, Super, If, Else, For, While,
  Switch, Case, Default, Break, Continue,
  Return, Goto, Import, Export, In, Static,
  Int, Long, Short, Float, Double, Bool, Char,
  True, False, Null, Struct, Sizeof, Init,
  Valueof, Typedef,

  Eof = Value
}

/**
 * 词法解析器token
 */
class Token(val kind: TokenKind, val value: String, val line: Int, val col: Int) {
  override def toString = kind.toString + ":" + value
}