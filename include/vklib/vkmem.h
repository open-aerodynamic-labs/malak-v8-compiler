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
#ifndef __V8_VMEM_H__
#define __V8_VMEM_H__

#include <mem.h>
#include <malloc.h>

/**
 * 堆中的指针内容转移到栈上
 *
 * @param stack 作用域在栈上的指针
 * @param heap  作用域在堆上的指针
 * @param hsize 堆的数据大小
 */
void heap_mov_stack(void *stack, void *heap, size_t hsize);

/**
 * 内存拷贝
 *
 * @param dest  目标内存
 * @param dpos  偏移量
 * @param src   拷贝内存
 * @param spos  偏移量
 * @param size  拷贝大小
 */
void xmemcpy(void *dest, size_t dpos, const void *src, size_t spos, size_t size);

#endif /* __V8_VMEM_H__ */
