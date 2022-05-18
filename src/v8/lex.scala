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
      case '\t' => col += 4;
      case _ => col += 1;
    }

    return ch;
  }

  /** 查看下一个字符 */
  def peek_next(): Char = inputc.charAt(pos);

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
  /** 读到字符 */
  inline def start_char(ch: Char): Boolean = ch == '\'';
  /** 读到字符串  */
  inline def start_string(ch: Char): Boolean = ch == '"';
  /** 是不是空格 */
  inline def isspace(ch: Char): Boolean = (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r');
  /** 是不是数字 */
  inline def isnumber(ch: Char): Boolean = ch >= '0' && ch <= '9';
  /** 标识符 */
  inline def let(ch: Char): Boolean = isnumber(ch) || (ch >= 'A' && ch <= 'Z' || ch >= 'a' && ch <= 'z' || ch == '_');
  /** 创建tok */
  inline def make_tok(value: String, kind: SyntaxKind, line: Int, col: Int): Token = new Token(value, kind, line, col);
  /** 结束符 */
  inline def eoi(ch: Char): Boolean = {
      ch match {
        case   '(' | ')'
             | '{' | '}'
             | '[' | ']'
             | '<' | '>'
             | ','
             | ';'
             | ':'
             | '='
             | '+'
             | '-'
             | '*'
             | '/'
             | '%'
             | '!'
             | '&'
             | '|'
             | '^'
             | '~'
             | '?'
             | ' '
             | ';'
             | '.' => true
        case _ => false;
      }
  }

  def main(args: Array[String]): Unit = {
    var code =
      """
        |var x: int = 80;
        |var y: long = 80L;
        |""".stripMargin;
    new LexicalAnalysis(code).lexps();
  }

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
    /* 遍历输入源码 */
    var ch:  Char = 0;
    var spc: Boolean = false;
    while (!reader.eof()) {
      ch  = reader.look_ahead();
      spc = isspace(ch);

      /**
       * 这里有几个词需要解释下：
       *    - eoi eoi（end of identifier）一般是符号，如：=, +, -, *, /, (, ), {, }这些，用于分割字符
       *
       *    - let let表示标识符, 标识符的定义是：《 a-z | A-Z | 0-9 | _ | . 》, 大小写字母，数字，下划线，点。
       *      点是因为它可以作为小数的分隔符， 20.0 这种形式。下划线可以作为变量名的一部分。
       *
       *    - spc 是不是空格，space简写成的spc。
       *
       */
      if (!spc && let(ch)) {
        /* 如果是数字就一直读，读到结束 */
        if (isnumber(ch)) {
          apptok(ch);
          breakable {
            while (!reader.eof()) {
              ch = reader.look_ahead();
              /* 如果不是数字判断是不是'L'或者'l'再或者是不是'.' */
              if (!isnumber(ch)) {
                if (eoi(ch)) {
                  if (curtok.contains('.')) {
                    puttok(SyntaxKind.Float);
                  } else {
                    puttok(SyntaxKind.Int);
                  }
                  break();
                }

                /* 长整型 */
                if ((ch == 'L' || ch == 'l') && eoi(reader.peek_next())) {
                  apptok(ch);
                  puttok(SyntaxKind.Long);
                  break();
                }

                /* 小数 */
                if (ch == '.') {
                  apptok(ch);
                } else {
                  throw new IllegalSyntaxException(s"语法错误, 行=${reader.line}, 列=${reader.col}");
                }
              } else {
                apptok(ch);
              }
            }
            apptok(ch);
          }
        } else {
          apptok(ch);
        }
      } else {
        /* 添加关键字或者字面量TOKEN */
        if (curtok.length() > 0) {
          var tokstr = curtok.toString();
          tokstr match {
            case "var" => puttok(SyntaxKind.Var);
            case "int" => puttok(SyntaxKind.Int);
            case "long" => puttok(SyntaxKind.Long);
            case "float" => puttok(SyntaxKind.Float);
            case "double" => puttok(SyntaxKind.Double);
            case "char" => puttok(SyntaxKind.Char);
            case _ => puttok(SyntaxKind.Identifier);
          }
        }
        /* 添加符号TOKEN */
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
