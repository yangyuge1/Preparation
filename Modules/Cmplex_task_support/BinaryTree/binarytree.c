#include "binarytree.h"


#define recursion

/**
 * @brief       ���������������㷨
 * @param       root    : ���ڵ��ַ
 *              x       : �����ҵ�ֵ
 * @retval      BinaryTreeNode*     : Ŀ��ڵ�ĵ�ַ
 */
//��bug
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
 * @brief       ���������������㷨
 * @param       node    : ����Ŀ�ʼ�ڵ�
 *              x       : �������ֵ
 * @retval      BinaryTreeNode*     : ������ɺ��½ڵ�ĵ�ַ
 * @example     Insert(NULL,1);     ���½�����������ڵ�
 *              Insert(p,1);        ����P����ʼ���²��룬p��ΪNULL��Ϊ��֤��������������ȷ�ԣ�pӦʼ��Ϊ���ĸ��ڵ�
 */
BinaryTreeNode* Insert_(BinaryTreeNode* parent,BinaryTreeNode* node, BTDataType x) 
{
    if (node == NULL && parent!=NULL) {//�����δ�����Ľڵ�
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
        else {//�ڴ泬��
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
    if (root == NULL) {//�����δ���������ĸ��ڵ�
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
    else if (root != NULL && root->parent == NULL&&root->height==1) {//����ǺϷ������ĸ��ڵ�
        return Insert_(NULL, root, x);
    }
    else if (root != NULL && root->parent != NULL && root->height>1) {//����ǺϷ��Ľڵ�
        return Insert_(root->parent, root, x);
    }
    else {//��β��Ϸ�
        return NULL;
    }
}
/**
 * @brief       ������������ֵ��������
 * @param       str[]   : �����������
 *              n       : ���鳤��
 *              BinaryTreeNode*     : ���ĸ��ڵ�
 * @retval      BinaryTreeNode*     : ���ĸ��ڵ�
 */
BinaryTreeNode* Inserts(BinaryTreeNode* root,BTDataType str[], int n)
{
    int i;
    if (root == NULL) {//�����δ���������ĸ��ڵ�
        root=Insert(NULL, str[0]);
        for (i = 1; i < n; i++) {
            if (Insert(root, str[i]) == NULL) {
                return NULL;
            }
        }
        return root;
    }
    else if(root != NULL && root->parent == NULL && root->height == 1) {//����ǺϷ������ĸ��ڵ�
        for (i = 0; i < n; i++) {
            if (Insert(root, str[i]) == NULL) {
                return NULL;
            }
        }
        return root;
    }
    else if (root != NULL && root->parent != NULL && root->height > 1) {//����ǺϷ��Ľڵ�
        for (i = 0; i < n; i++) {
            if (Insert(root, str[i]) == NULL) {
                return NULL;
            }
        }
        return root;
    }
    else {//��β��Ϸ�
        return NULL;
    }
}

/**
 * @brief       �����������ȡ
 * @param       BinaryTreeNode   : �ڵ�
 * @retval      int              : ����νڵ����µ�������
 */
int GetMaxDepth(BinaryTreeNode* root)
{
    int right_depth, left_depth;
    if (root != NULL ) {//����ǺϷ���
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
    else {//��β��Ϸ�
        return -1;
    }
}
int GetMinDepth(BinaryTreeNode* root)
{
    int right_depth, left_depth;
    if (root != NULL) {//����ǺϷ���
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
    else {//��β��Ϸ�
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
void UpdateHeights(BinaryTreeNode* root,int Queue_lenth)			//		��α���������
{
    BinaryTreeNode** p=(BinaryTreeNode**)getMemory(sizeof(BinaryTreeNode*)* Queue_lenth);
    int i = 0,j=0;
    //��Ϊ�գ�ֱ�ӷ���
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
    if (root != NULL && root->parent != NULL) {//�Ϸ��ڵ�
        //��ɾ���Ľڵ���Ҷ�ӽڵ�
        if (root->left == NULL && root->right == NULL) {
            //ֱ��ɾ��
            if (root == root->parent->left) {
                root->parent->left = NULL;
                freeMemory(root);
            }
            else if (root == root->parent->right) {
                root->parent->right = NULL;
                freeMemory(root);
            }
        }
        //��ɾ���Ľڵ�ֻ��һ�����ӣ�����ֻ��һ���Һ���
        else if ( (root->left == NULL && root->right != NULL)||(root->left != NULL && root->right == NULL) ) {
            if (root->right != NULL) {//�Һ���
                //ֱ��ɾ��
                if (root == root->parent->left) {
                    root->parent->left = root->right;
                    root->right->parent = root->parent;
                    //����
                    UpdateHeights(root->right, (GetMaxDepth(root->right) - root->right->height) * 2 + 1);
                    freeMemory(root);
                }
                else if (root == root->parent->right) {
                    root->parent->right = root->right;
                    root->right->parent = root->parent;
                    //����
                    UpdateHeights(root->right, (GetMaxDepth(root->right) - root->right->height) * 2 + 1);
                    freeMemory(root);
                }
            }
            else if (root->left != NULL) {//����
                //ֱ��ɾ��
                if (root == root->parent->left) {
                    root->parent->left = root->left;
                    root->left->parent = root->parent;
                    //����
                    UpdateHeights(root->left, (GetMaxDepth(root->left) - root->left->height) * 2 + 1);
                    freeMemory(root);
                }
                else if (root == root->parent->right) {
                    root->parent->right = root->left;
                    root->left->parent = root->parent;
                    //����
                    UpdateHeights(root->left, (GetMaxDepth(root->left) - root->left->height) * 2 + 1);
                    freeMemory(root);
                }
            }
        }
        //��ɾ���Ľڵ�������ӣ�����һ���Һ���
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

int Print_(BinaryTreeNode* root, char* p,int depth_max,int width,int width_p)			//		�������������
{
    if (root == NULL)						//	�ݹ�������NULL��������һ��ڵ�
    {
        return width_p;
    }
    int change;
    width_p =Print_(root->left,p, depth_max, width, width_p);			//	�ݹ����������

    *(p+(root->height - 1)* width+width_p) =(root->data);
    printf("(%2d,%d,%d)", (root->data), root->height, width_p);
    width_p += 1;

    width_p =Print_(root->right, p, depth_max, width, width_p);			//	�ݹ����������


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
