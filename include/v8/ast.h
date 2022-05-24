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
#ifndef __EPOCH_AST_H__
#define __EPOCH_AST_H__

#include "token.h"
#include <vector>

#define xep_end_node (xep_ast_node *) nullptr

/* 定义AST节点类型 */
#define xep_node_type unsigned int

#define XEP_NODE_TYPE_PROGRAM       0

/**
 * 抽象语法树结构
 */
struct xep_ast_node {
      struct token *token;
      xep_node_type type;
      struct xep_ast_node *parent = xep_end_node;
      struct xep_ast_node *child = xep_end_node;
      struct xep_ast_node *next = xep_end_node;
};

/**
 * 创建节点
 *
 * @param token token指针
 * @param type  节点类型
 * @return      节点指针
 */
inline static
struct xep_ast_node *xep_make_node(struct token *token, xep_node_type type)
{
      auto *node = new struct xep_ast_node;
      node->token = token;
      node->type = type;
      return node;
}

/**
 * 语法分析
 *
 * @param tokens 词法解析器解析出来的token列表
 * @param root   语法树根节点
 */
void xep_ast_pars(std::vector<struct token> &tokens, xep_ast_node *root);

#endif /* __EPOCH_AST_H__ */
