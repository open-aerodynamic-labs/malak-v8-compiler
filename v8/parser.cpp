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
#include "v8/parser.h"

/** 语法解析 */
void xep_run_parser(std::vector<struct token> &tokens)
{
      struct token      *p_tok;
      xep_token_reader   reader(tokens);

      while((p_tok = reader.look_ahead()) != nullptr) {

            switch(p_tok->kind) {

                  /**
                   * fun        -> fun identifier '(' parameters ')'
                   * parameters -> identifier ':' type *parameters
                   * return     -> ':' type
                   * block      -> '{' statements '}'
                   */
                  case KIND_FUN: {

                  }

            }
      }
}