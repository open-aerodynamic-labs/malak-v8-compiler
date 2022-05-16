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
import v8.LexicalAnalysis.{isspace, v8_make_token}

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
    var code = "var x = 2;";
    new LexicalAnalysis(code).lexps();
  }

  /** 是不是空格 */
  def isspace(ch: Char): Boolean =(ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r');

  /** 结束符 */
  def eoi(ch: Char): Boolean = ch == ';';

  /** 是不是数字 */
  def isnumber(ch: Char): Boolean = ch >= '0' && ch <= '9';

  /** 创建tok */
  def v8_make_token(value: String, kind: TokenKind, line: Int, col: Int): Token = new Token(value, kind, line, col);
}

object LexicalAnalysisState extends Enumeration {
  type LexicalAnalysisState = Value
  val
  Nop,        // 空状态
  Plus,       // +
  PlusEq,     // +=
  Minus,      // -
  Star,       // *
  Slash,      // /
  Perce,      // %
  Eof = Value
}

/**
 * 词法解析器
 */
class LexicalAnalysis(val code: String) {
  private var reader = new SourceReader(code);
  private var curtok = new StringBuilder();
  var tokens: List[Token] = listof();
  var state: LexicalAnalysisState.Value = LexicalAnalysisState.Nop;

  /** 追加tok字符 */
  private def apptok(ch: Char): Unit = curtok.append(ch);

  private def puttok(kind: TokenKind): Unit = {
    puttok(kind, curtok.toString())
    curtok.clear();
  };

  private def puttok(kind: TokenKind, ch: Char): Unit = {
    puttok(kind, String.valueOf(ch));
  }

  private def puttok(kind: TokenKind, value: String): Unit = {
    tokens.add(v8_make_token(value, kind, reader.line, reader.col));
  }

  /** 词法解析 */
  def lexps(): List[Token] = {
    /* 遍历输入源码 */
    var ch: Char = 0;
    while (!reader.eof()) {
      ch = reader.look_ahead();

      /* 开始解析 */
      if (!isspace(ch)) {
        ch match {
          // todo
        }
      }

    }

    /* 打印 token 列表 */
    tokens.forEach((tok: Token) => {
      println(tok.toString());
    });

    return tokens;
  }
}
