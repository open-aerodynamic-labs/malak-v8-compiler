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

type SyntaxKind = SyntaxKind.Value

/** tok类别枚举 */
object SyntaxKind extends Enumeration {
  val

  /* 关键字 */
  Var, Int,

  /* 字面量 */
  Identifier, String, Number,

  /* 操作符 */
  Eq, Plus, Minus, Star, Slash,

  Eoi, Eof = Value
}

/** 词法解析器token */
class Token(val value: String, val kind: SyntaxKind, val line: Int, val col: Int) {
  override def toString = s"<${kind.toString}, '$value'>"
}