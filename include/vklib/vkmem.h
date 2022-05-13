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

/**
 * 内存拷贝
 *
 * @param dest  目标内存
 * @param dpos  偏移量
 * @param src   拷贝内存
 * @param spos  偏移量
 * @param size  拷贝大小
 */
void xmemcpy(void *dest, int dpos, const void *src, int spos, int size);

#endif /* __V8_VMEM_H__ */
