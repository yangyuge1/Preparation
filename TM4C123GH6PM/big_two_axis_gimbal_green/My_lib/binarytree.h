/*
 * binarytree.h
 *
 *  Created on: 2023��7��24��
 *      Author: yyg
 */

#ifndef CMPLEX_TASK_SUPPORT_BINARYTREE_BINARYTREE_H_
#define CMPLEX_TASK_SUPPORT_BINARYTREE_BINARYTREE_H_

#include "data_structure.h"
#include "utils/uartstdio.h"
#include <stdint.h>
#include <stdio.h>

#define BTDataType int
// ������
typedef struct BinaryTreeNode
{
    struct BinaryTreeNode* left; // ָ��ǰ�ڵ�����
    struct BinaryTreeNode* right; // ָ��ǰ�ڵ��Һ���
    struct BinaryTreeNode* parent;
    int height;
    BTDataType data; // ��ǰ�ڵ�ֵ��
}BinaryTreeNode;

BinaryTreeNode* Search(BinaryTreeNode* root, BTDataType x);
BinaryTreeNode* Insert(BinaryTreeNode* root, BTDataType x);
BinaryTreeNode* Inserts(BinaryTreeNode* root, BTDataType str[], int n);
int GetMaxDepth(BinaryTreeNode* root);
int GetMinDepth(BinaryTreeNode* root);
void DeleteNode(BinaryTreeNode* root);
void PrintTree(BinaryTreeNode* root);



#endif /* CMPLEX_TASK_SUPPORT_BINARYTREE_BINARYTREE_H_ */
