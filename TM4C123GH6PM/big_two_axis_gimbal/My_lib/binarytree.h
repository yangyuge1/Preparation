/*
 * binarytree.h
 *
 *  Created on: 2023年7月24日
 *      Author: yyg
 */

#ifndef CMPLEX_TASK_SUPPORT_BINARYTREE_BINARYTREE_H_
#define CMPLEX_TASK_SUPPORT_BINARYTREE_BINARYTREE_H_

#include "data_structure.h"
#include "utils/uartstdio.h"
#include <stdint.h>
#include <stdio.h>

#define BTDataType int
// 二叉链
typedef struct BinaryTreeNode
{
    struct BinaryTreeNode* left; // 指向当前节点左孩子
    struct BinaryTreeNode* right; // 指向当前节点右孩子
    struct BinaryTreeNode* parent;
    int height;
    BTDataType data; // 当前节点值域
}BinaryTreeNode;

BinaryTreeNode* Search(BinaryTreeNode* root, BTDataType x);
BinaryTreeNode* Insert(BinaryTreeNode* root, BTDataType x);
BinaryTreeNode* Inserts(BinaryTreeNode* root, BTDataType str[], int n);
int GetMaxDepth(BinaryTreeNode* root);
int GetMinDepth(BinaryTreeNode* root);
void DeleteNode(BinaryTreeNode* root);
void PrintTree(BinaryTreeNode* root);



#endif /* CMPLEX_TASK_SUPPORT_BINARYTREE_BINARYTREE_H_ */
