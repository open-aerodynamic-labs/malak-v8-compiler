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
#include "v8/v8_make_cmp.h"
#include "vklib/log.h"
#include "vklib/vkstring.h"
#include <io.h>

#define VK_EXMP_CP "D:\\projects\\varuka-v8-comp\\vkexmp\\*"

/**
 * 遍历源码文件夹
 * @param classpath
 */
inline static vk_source_t *vk_scan_cp(const char *classpath, int *srcsize);

/**
 * 启动编译器执行编译任务。
 *
 * 编译选项：
 *  -cp --classpath     选择编译的源码路径
 *
 */
int main(int argc, char **argv)
{
      int             succode;
      int             srcsize;
      vk_source_t*    sources;
      char            cp[255];

      /* classpath */
      strreplace(cp, VK_EXMP_CP, "\\", "/");

      /* 遍历源码文件夹 */
      sources = vk_scan_cp(cp, &srcsize);

      /* 初始化v8编译器 */
      vk_init_v8_cmp(sources, srcsize, &succode);

      return 0;
}

vk_source_t *vk_scan_cp(const char *classpath, int *srcsize)
{
      intptr_t                hfile;
      struct _finddata_t      finddata;
      const char*             pname;
      vk_source_t*            sources;
      int                     sourcepos;

      sourcepos   = 0;
      sources     = malloc(sizeof(vk_source_t) * 10);

      hfile = _findfirst(classpath, &finddata);
      while (_findnext(hfile, &finddata) == 0) {
            // 获取后缀为vk的文件
            pname = strrchr(finddata.name, '.');
            // 后缀判断
            if (pname != NULL && strcmp(pname, ".vk") == 0) {
                  vk_source_t *vksrc = &sources[sourcepos];
                  strcpy(vksrc->path, classpath);
                  strcpy(vksrc->file, finddata.name);
                  sourcepos++;
            }
      }

      *srcsize = sourcepos;
      return sources;
}