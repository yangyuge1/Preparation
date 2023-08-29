#ifndef _BINARYTREE_H_
#define _BINARYTREE_H_

#include "memorypool.h"
#include <stdint.h>
#include <stdio.h>

typedef struct data_box {
    int x;
    int y;
    int point;
}data_box;

typedef enum
{
    road = 0,
    house = 1,
} point_type;

#define BTDataType int
// ???
typedef struct BinaryTreeNode
{
    struct BinaryTreeNode* left; // ?????????
    struct BinaryTreeNode* right; // ?????????
    struct BinaryTreeNode* parent;
    int height;
    BTDataType data; // ??????
    data_box value;
}BinaryTreeNode;

BinaryTreeNode* Search(BinaryTreeNode* root, BTDataType x);
BinaryTreeNode* Insert(BinaryTreeNode* root, BTDataType x);
BinaryTreeNode* Inserts(BinaryTreeNode* root, BTDataType str[], int n);
int GetMaxDepth(BinaryTreeNode* root);
int GetMinDepth(BinaryTreeNode* root);
void DeleteNode(BinaryTreeNode* root);
void PrintTree(BinaryTreeNode* root);

#endif