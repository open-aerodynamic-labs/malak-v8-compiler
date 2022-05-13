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
#ifndef __V8_VKFILE_H__
#define __V8_VKFILE_H__

/**
 * 读取整个文件
 *
 * @param dest 写入目标指针
 * @param file 文件名
 * @param size 读取大小
 */
char *xfget(const char *file);

#endif /* __V8_VKFILE_H__ */
