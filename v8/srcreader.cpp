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
#include "srcreader.h"

xep_source_reader::xep_source_reader(const std::string &source)
{
      self_line = 1;
      self_col = 0;
      self_pos = 0;
      self_source = source;
      self_src_size = self_source.size();
}

bool xep_source_reader::look_ahead(char *p_ch, int *p_line, int *p_col)
{
      bool iseof = eof();
      if (!iseof) {
            char ch = self_source[self_pos];
            self_pos++;

            if (ch == '\n') {
                  self_line++;
                  self_col = 0;
            } else {
                  self_col++;
            }

            *p_ch = ch;
      }

      *p_line     = self_line;
      *p_col      = self_col;
      return iseof;
}

void xep_source_reader::back(int *p_line, int *p_col)
{
      if (self_pos > 0) {
            self_pos--;
            char ch = self_source[self_pos];

            if (ch == '\n') {
                  self_line--;
                  self_col = 0;
            } else {
                  self_col--;
            }
      }

      *p_line = self_line;
      *p_col = self_col;
}

const bool xep_source_reader::eof()
{
      return self_pos >= self_src_size;
}

char xep_source_reader::skip_next()
{

      char ch;
      look_ahead(&ch, &self_line, &self_col);
      return ch;
}

char xep_source_reader::peek_next()
{
      if (self_pos < self_src_size) {
            return self_source[self_pos];
      }

      return '\0';
}