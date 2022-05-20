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
#ifndef __VARUKA_SOURCE_READER_H__
#define __VARUKA_SOURCE_READER_H__

#include <string>

/** 源码读取器 */
class SourceReader {
public:
      /** 读取字符串 */
      SourceReader(const std::string &source);
      /** 读取下一个字符 */
      char look_ahead();
      /** 回退到上一个字符 */
      void back();
      /* 查看下一个字符 */
      char peek_next();
      /** 是否读到文件末尾 */
      const bool eof();

private:
      int         self_line;
      int         self_col;
      size_t      self_pos;
      std::string self_source;
      size_t      self_src_size;
};

#endif /* __VARUKA_SOURCE_READER_H__ */
