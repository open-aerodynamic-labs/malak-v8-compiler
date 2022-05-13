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
#ifndef __V8_CMP_H__
#define __V8_CMP_H__

#include "v8/errcode.h"

/**
 * volcanos源码文件
 */
typedef struct vk_source {
        char path[255];         /* 文件夹 */
        char file[64];          /* 文件名 */
} vk_source_t;

/**
 * 初始化编译器，扫描文件结构，获取源码模块列表。
 *
 * @param [i] classpath 源码路径
 */
void vk_init_v8_cmp(vk_source_t *sources, int srcsize, int *success);

#endif /* __V8_CMP_H__ */