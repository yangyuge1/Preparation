#include "binarytree.h"


#define recursion

/**
 * @brief       二叉排序树查找算法
 * @param       root    : 根节点地址
 *              x       : 待查找的值
 * @retval      BinaryTreeNode*     : 目标节点的地址
 */
//有bug
BinaryTreeNode* Search(BinaryTreeNode* root, BTDataType x) 
{
    if (root == NULL){
        return NULL;
    }
    if (root->data == x) {
        return root;
    }
    else if (x < root->data) {
        return Search(root->left, x);
    }
    else {
        return Search(root->right, x);
    }
}
/**
 * @brief       二叉排序树插入算法
 * @param       node    : 插入的开始节点
 *              x       : 待插入的值
 * @retval      BinaryTreeNode*     : 插入完成后新节点的地址
 * @example     Insert(NULL,1);     ：新建树并插入根节点
 *              Insert(p,1);        ：从P处开始往下插入，p不为NULL，为保证二叉排序树的正确性，p应始终为树的根节点
 */
BinaryTreeNode* Insert_(BinaryTreeNode* parent,BinaryTreeNode* node, BTDataType x) 
{
    if (node == NULL && parent!=NULL) {//入参是未创建的节点
        BinaryTreeNode* p = (BinaryTreeNode*)getMemory(sizeof(BinaryTreeNode));
        if (p != NULL) {
            p->data = x;
            p->height = parent->height + 1;
            p->parent = parent;
            if (x < parent->data) {
                parent->left = p;
            }
            else if(x > parent->data) {
                parent->right = p;
            }
            return p;
        }
        else {//内存超出
            return NULL;
        }

    }
    else {
        if (x < node->data) {
            return Insert_(node, node->left, x);
        }
        else if (x > node->data) {
            return Insert_(node, node->right, x);
        }
        else if (x == node->data) {
            return node;
        }
    }
}
BinaryTreeNode* Insert(BinaryTreeNode* root, BTDataType x)
{
    if (root == NULL) {//入参是未创建的树的根节点
        BinaryTreeNode* p = (BinaryTreeNode*)getMemory(sizeof(BinaryTreeNode));
        if (p != NULL) {
            p->data = x;
            p->left = NULL;
            p->right = NULL;
            p->parent = NULL;
            p->height = 1;
            return p;
        }
        else {
            return NULL;
        }

    }
    else if (root != NULL && root->parent == NULL&&root->height==1) {//入参是合法的树的根节点
        return Insert_(NULL, root, x);
    }
    else if (root != NULL && root->parent != NULL && root->height>1) {//入参是合法的节点
        return Insert_(root->parent, root, x);
    }
    else {//入参不合法
        return NULL;
    }
}
/**
 * @brief       二叉排序树多值连续插入
 * @param       str[]   : 待插入的数组
 *              n       : 数组长度
 *              BinaryTreeNode*     : 树的根节点
 * @retval      BinaryTreeNode*     : 树的根节点
 */
BinaryTreeNode* Inserts(BinaryTreeNode* root,BTDataType str[], int n)
{
    int i;
    if (root == NULL) {//入参是未创建的树的根节点
        root=Insert(NULL, str[0]);
        for (i = 1; i < n; i++) {
            if (Insert(root, str[i]) == NULL) {
                return NULL;
            }
        }
        return root;
    }
    else if(root != NULL && root->parent == NULL && root->height == 1) {//入参是合法的树的根节点
        for (i = 0; i < n; i++) {
            if (Insert(root, str[i]) == NULL) {
                return NULL;
            }
        }
        return root;
    }
    else if (root != NULL && root->parent != NULL && root->height > 1) {//入参是合法的节点
        for (i = 0; i < n; i++) {
            if (Insert(root, str[i]) == NULL) {
                return NULL;
            }
        }
        return root;
    }
    else {//入参不合法
        return NULL;
    }
}

/**
 * @brief       二叉树深度求取
 * @param       BinaryTreeNode   : 节点
 * @retval      int              : 从入参节点往下的最大深度
 */
int GetMaxDepth(BinaryTreeNode* root)
{
    int right_depth, left_depth;
    if (root != NULL ) {//入参是合法的
        if (root->left == NULL && root->right == NULL) {
            return root->height;
        }
        if (root->left != NULL && root->right == NULL) {
            return GetMaxDepth(root->left);
        }
        if (root->left == NULL && root->right != NULL) {
            return GetMaxDepth(root->right);
        }
        if (root->left != NULL && root->right != NULL) {
            left_depth = GetMaxDepth(root->left);
            right_depth = GetMaxDepth(root->right);
            return left_depth >= right_depth ? left_depth : right_depth;
        }
    }
    else {//入参不合法
        return -1;
    }
}
int GetMinDepth(BinaryTreeNode* root)
{
    int right_depth, left_depth;
    if (root != NULL) {//入参是合法的
        if (root->left == NULL && root->right == NULL) {
            return root->height;
        }
        if (root->left != NULL && root->right == NULL) {
            return GetMaxDepth(root->left);
        }
        if (root->left == NULL && root->right != NULL) {
            return GetMaxDepth(root->right);
        }
        if (root->left != NULL && root->right != NULL) {
            left_depth = GetMaxDepth(root->left);
            right_depth = GetMaxDepth(root->right);
            return left_depth <= right_depth ? left_depth : right_depth;
        }
    }
    else {//入参不合法
        return -1;
    }
}
BinaryTreeNode* FindLeftNode(BinaryTreeNode* root) 
{
    if (root->left == NULL) {
        return root;
    }
    else {
        return FindLeftNode(root->left);
    }
}
BinaryTreeNode* FindRightNode(BinaryTreeNode* root)
{
    if (root->right == NULL) {
        return root;
    }
    else {
        return FindRightNode(root->right);
    }
}
void UpdateHeights(BinaryTreeNode* root,int Queue_lenth)			//		层次遍历二叉树
{
    BinaryTreeNode** p=(BinaryTreeNode**)getMemory(sizeof(BinaryTreeNode*)* Queue_lenth);
    int i = 0,j=0;
    //树为空，直接返回
    if (root == NULL)
    {
        return;
    }
    root->height = root->parent->height + 1;
    printf("(%d)", root->data);
    if (root->left != NULL) {
        p[j] = root->left;
        j++;
    }
    if (root->right != NULL) {
        p[j] = root->right;
        j++;
    }

    while (i<j&&i<Queue_lenth&&j<Queue_lenth)
    {
        root = p[i];
        root->height = root->parent->height + 1;
        printf("(%d)", root->data);
        if (root->left != NULL) {
            p[j] = root->left;
            j++;
        }
        if (root->right != NULL) {
            p[j] = root->right;
            j++;
        }
        i++;
    }

}

void DeleteNode(BinaryTreeNode* root)
{
    BinaryTreeNode* node;
    if (root != NULL && root->parent != NULL) {//合法节点
        //所删除的节点是叶子节点
        if (root->left == NULL && root->right == NULL) {
            //直接删除
            if (root == root->parent->left) {
                root->parent->left = NULL;
                freeMemory(root);
            }
            else if (root == root->parent->right) {
                root->parent->right = NULL;
                freeMemory(root);
            }
        }
        //所删除的节点只有一个左孩子，或者只有一个右孩子
        else if ( (root->left == NULL && root->right != NULL)||(root->left != NULL && root->right == NULL) ) {
            if (root->right != NULL) {//右孩子
                //直接删除
                if (root == root->parent->left) {
                    root->parent->left = root->right;
                    root->right->parent = root->parent;
                    //更新
                    UpdateHeights(root->right, (GetMaxDepth(root->right) - root->right->height) * 2 + 1);
                    freeMemory(root);
                }
                else if (root == root->parent->right) {
                    root->parent->right = root->right;
                    root->right->parent = root->parent;
                    //更新
                    UpdateHeights(root->right, (GetMaxDepth(root->right) - root->right->height) * 2 + 1);
                    freeMemory(root);
                }
            }
            else if (root->left != NULL) {//左孩子
                //直接删除
                if (root == root->parent->left) {
                    root->parent->left = root->left;
                    root->left->parent = root->parent;
                    //更新
                    UpdateHeights(root->left, (GetMaxDepth(root->left) - root->left->height) * 2 + 1);
                    freeMemory(root);
                }
                else if (root == root->parent->right) {
                    root->parent->right = root->left;
                    root->left->parent = root->parent;
                    //更新
                    UpdateHeights(root->left, (GetMaxDepth(root->left) - root->left->height) * 2 + 1);
                    freeMemory(root);
                }
            }
        }
        //所删除的节点既有左孩子，又有一个右孩子
        else {
            node = FindLeftNode(root->right);
            root->data = node->data;
            DeleteNode(node);
            //node = FindLeftNode(root->left);
            //root->data = node->data;
            //DeleteNode(node);
        }
    }
}

int Print_(BinaryTreeNode* root, char* p,int depth_max,int width,int width_p)			//		先序遍历二叉树
{
    if (root == NULL)						//	递归中遇到NULL，返回上一层节点
    {
        return width_p;
    }
    int change;
    width_p =Print_(root->left,p, depth_max, width, width_p);			//	递归遍历左子树

    *(p+(root->height - 1)* width+width_p) =(root->data);
    printf("(%2d,%d,%d)", (root->data), root->height, width_p);
    width_p += 1;

    width_p =Print_(root->right, p, depth_max, width, width_p);			//	递归遍历右子树


    return width_p;
}

void PrintTree(BinaryTreeNode *root)
{
    int depth_max = GetMaxDepth(root);
    int width = depth_max * 2 + 1;
    printf("\n==%d,%d==\n", depth_max, width);
    char *p= (char*)getMemory((depth_max * width));
    if (p != NULL) {
        Print_(root,p, depth_max, width,0);
        printf("\n");
        int i, j;
        for (i = 0; i < depth_max; i++) {
            printf("|");
            for (j = 0; j < width; j++) {
                if (*(p + i * width + j) == 0) {
                    printf("  ");
                }
                else {
                    printf("%2d",(* (p + i * width + j)));
                }
            }
            printf("|\n");
        }
    }
    freeMemory(p);
}
