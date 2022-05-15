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

/**
 * 源码读取器
 */
class SourceReader(inputc: String) {

  var line: Int = 0;
  var col: Int = 0;
  var pos: Int = 0;

  var size: Int = inputc.length;

  /**
   * 是否读到结尾
   */
  def eof(): Boolean = {
    return pos >= size;
  }

  /**
   * 获取下一个字符
   */
  def advance(): Character = {
    var ch = inputc.charAt(pos);
    pos += 1;

    ch match {
      case '\n' => {
        line += 1;
        col = 0;
      }
      case '\t' => {
        col += 4;
      }
      case ' ' =>
      case _ => col += 1;
    }

    return ch;
  }

}

object Lexer {

  def main(args: Array[String]): Unit = {
    var code =
      """
        | var x = 2;
        | var y = 4;
        | x = x * y;
        |""".stripMargin;
    var reader = new SourceReader(code);

    while (!reader.eof()) {
      var ch = reader.advance();
      println(ch);
    }

  }

  /**
   * 词法解析器
   *
   * @param source 输入源
   */
  def lexps(input_code: String): List[Token] = {
    return null;
  }

}
