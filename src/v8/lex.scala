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

import v8.LexicalAnalysis.{eoi, isnumber, isspace, let, make_tok}

import scala.util.control.Breaks.{break, breakable}

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

  /** 获取下一个字符 */
  def look_ahead(): Char = {
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

  /** 回退一个字符 */
  def back(): Unit = {
    pos -= 1;

    if (col > 0)
      col -= 1;

    if (inputc.charAt(pos) == '\n')
      line -= 1;

  }
}

object LexicalAnalysis {
  def main(args: Array[String]): Unit = {
    var code =
      """
        |var x: int = 20 + 30 * 2 / 4 - 20 * 2 * 40;
        |""".stripMargin;
    new LexicalAnalysis(code).lexps();
  }

  /** 是不是空格 */
  inline def isspace(ch: Char): Boolean = (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r');
  /** 结束符 */
  inline def eoi(ch: Char): Boolean = ch == ':';
  /** 是不是数字 */
  inline def isnumber(ch: Char): Boolean = ch >= '0' && ch <= '9' || ch == '.';
  /** 标识符 */
  inline def let(ch: Char): Boolean = isnumber(ch) || (ch >= 'A' && ch <= 'Z' || ch >= 'a' && ch <= 'z' || ch == '_');
  /** 创建tok */
  inline def make_tok(value: String, kind: SyntaxKind, line: Int, col: Int): Token = new Token(value, kind, line, col);
}

/**
 * 词法解析器
 */
class LexicalAnalysis(val code: String) {
  private var reader = new SourceReader(code);
  private var curtok = new StringBuilder();
  var tokens: List[Token] = listof();

  /** 追加tok字符 */
  private def apptok(ch: Char): Unit = curtok.append(ch);

  /** 自动识别tok类型 */
  def auto_puttok(ch: Char): Unit = {
    ch match {
      case '+' => puttok(SyntaxKind.Plus, '+');
      case '-' => puttok(SyntaxKind.Minus, '-');
      case '*' => puttok(SyntaxKind.Star, '*');
      case '/' => puttok(SyntaxKind.Slash, '/');
      case '=' => puttok(SyntaxKind.Eq, '=');
      case _ => { /* 什么都不做 */ }
    }
  }

  private def puttok(kind: SyntaxKind): Unit = {
    puttok(kind, curtok.toString())
    curtok.clear();
  };

  private def puttok(kind: SyntaxKind, ch: Char): Unit = {
    puttok(kind, String.valueOf(ch));
  }

  private def puttok(kind: SyntaxKind, value: String): Unit = {
    tokens.add(make_tok(value, kind, reader.line, reader.col - value.length()));
  }

  /** 词法解析 */
  def lexps(): List[Token] = {
    /** 遍历输入源码 */
    var ch: Char = 0;
    while (!reader.eof()) {
      ch = reader.look_ahead();
      var spc = isspace(ch);

      if (!spc && !eoi(ch) && let(ch)) {
        /** 如果是数字就一直读，读到结束 */
        if (isnumber(ch)) {
          apptok(ch);
          breakable {
            while (!reader.eof()) {
              ch = reader.look_ahead();
              if (!isnumber(ch))
                break();
              apptok(ch);
            }
          }
          puttok(SyntaxKind.Number);
        } else {
          apptok(ch);
        }
      } else {
        /** 添加关键字或者字面量TOKEN */
        if (curtok.length() > 0) {
          var tokstr = curtok.toString();
          tokstr match {
            case "var" => puttok(SyntaxKind.Var);
            case "int" => puttok(SyntaxKind.Int);
            case _ => puttok(SyntaxKind.Identifier);
          }
        }
        /** 添加符号TOKEN */
        if (!spc && !let(ch) && ch != ';')
          auto_puttok(ch);
      }

      if (ch == ';')
        puttok(SyntaxKind.Eoi, ';');

    }

    /* 打印 tok 列表 */
    tokens.forEach((tok: Token) => {
      println(tok.toString());
    });

    return tokens;
  }
}
