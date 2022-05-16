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

import v8.Token.*

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

  def back(): Unit = {
    pos -= 1;

    if (col > 0)
      col -= 1;

    if (inputc.charAt(pos) == '\n')
      line -= 1;

  }

}

/**
 * 解析状态
 */
object StatPs extends Enumeration {
  type StatPs = Value
  val
  StatNop, StatEoi, StatNumber,
  StatString, StatComment,
  Eof = Value
}


/**
 * 词法解析器
 */
object Lexer {

  def main(args: Array[String]): Unit = {
    var code =
      """
        | var x = 2;
        |""".stripMargin;
    lexps(code);
  }

  /** 是不是空格 */
  def isspace(ch: Char): Boolean =
    (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r');

  def eoi(ch: Char): Boolean = ch == ';';

  /** 是不是数字 */
  def isnumber(ch: Char): Boolean = ch >= '0' && ch <= '9';

  def v8_make_token(value: String, kind: TokenKind, line: Int, col: Int): Token
  = new Token(value, kind, line, col);

  /**
   * 词法解析器
   *
   * @param source 输入源
   */
  def lexps(input_code: String): List[Token] = {
    var reader = new SourceReader(input_code);

    var stat = kNop
    var kind = kNop;
    var tokbuild = new StringBuilder();
    var toklist = listof[Token]();

    /* 遍历输入源码 */
    var ch: Character = null;
    while (!reader.eof()) {
      ch = reader.advance();

      if (isspace(ch)) {
        stat = kSpace;
      }

      stat match {
        case Token.kNop => {
          if (isnumber(ch))
            stat = kNumber;
          else if (ch == '"')
            stat = kString;
          else if (ch == '/')
            stat = kComment;
          else if (eoi(ch))
            stat = kEoi;
          else
            tokbuild.append(ch);
        }
        case Token.kNumber => {
          if (isnumber(ch))
            tokbuild.append(ch);
          else {
            ch match {
              case '.' => {
                stat = kNumber;
                tokbuild.append(ch);
              };
              case _ => {
                kind = stat;
                stat = kNop;
                reader.back();
              }
            }
          }
        }
        case Token.kEoi => {
          toklist.add(v8_make_token(tokbuild.toString(), kEoi, reader.line, reader.col));
          tokbuild.clear();
          stat = kNop;
        }
        case Token.kSpace => {
          if (tokbuild.length > 0) {
            toklist.add(v8_make_token(tokbuild.toString(), Token.kIdentifier, reader.line, reader.col));
            tokbuild.clear();
          }
          stat = kNop;
        }
      }
    }

    /* 如果最后还有token */
    if (tokbuild.length() > 0) {
      toklist.add(v8_make_token(tokbuild.toString(), stat, reader.line, reader.col));
    }

    toklist.forEach((tok: Token) => {
      println(tok.toString());
    });

    return toklist;
  }

}
