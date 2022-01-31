// LeetCode 110 判断一棵树是否为平衡树，带领我们初步地认识AVL TREE

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
/*
class Solution {
public:
    bool isBalanced(TreeNode* root) {
        if(MaxHeight(root) == -1)
            return false;
        return true;
    }

    int MaxHeight(TreeNode* root){
        if(root == nullptr){
            return 0; // 代表到了一个空节点，此时默认高度值为0
        }
        int LeftHeight = MaxHeight(root->left);    // 返回-1的情况就说明自己的左子树不平衡了
        int RightHeight = MaxHeight(root->right);
        if(LeftHeight == -1 || RightHeight == -1 || abs(LeftHeight - RightHeight) > 1){
            // 需要左子树和右子树均平衡并且以自己为根节点的树也是平衡的
            return -1;
        }
        return max(LeftHeight, RightHeight) + 1; // 返回自身的高度
    }
};
*/


/*
全部思路按照清华大学->数据结构C语言版实现：
比较精巧减少工程量的内容->把双旋操作进行了分解，省去了一大堆功夫！
switch用得较多，这也是之前一直缺失的[用的较少的语法]。
遇到的bug->switch的case底下不能定义变量！！！否则编译器就炸了
需要注意的point---->  
1. 一定要理解深刻为什么在函数中我们传递了指针的引用！！因为我们要修改那块指针指向的区域而不单单是值！
2. 一定要深刻理解四种旋转来维护平衡的情况，以及画图来计算平衡后的balance_factor。
*/


#include <iostream>
#include <vector>
#include <queue>
using namespace std;

#define LeftHigher 1
#define RightHigher -1
#define Equal 0

template<typename T>
struct AVLNode{
    T data;
    int balance_factor;    // 按照清华大学数据结构书的定义 平衡因子为左子树高度减去右子树高度！
    AVLNode<T>* left;
    AVLNode<T>* right;
    AVLNode<T>* parent;
    AVLNode(T n, int bf = 0) : data(n), balance_factor(bf), left(nullptr), right(nullptr), parent(nullptr){};
};

template<typename T>
class AVLTREE{
private:
    /*
    实际上我们的私有属性root是不需要的，因为"旋转"的性质导致我们的root始终在调整变化！
    */
    AVLNode<T> *root;
    bool taller = false;
public:
    AVLTREE() { root = nullptr; };
    AVLNode<T> *&getroot() { return root; };
    void insert(AVLNode<T>* &root, T data);  // 因为这里要递归地去进行插入操作，所以应该有一个当前root结点，非类的root
    bool deletenode(T data);
    bool search(AVLNode<T>* root, T data);
    void R_Rotate(AVLNode<T>* &_root);
    void L_Rotate(AVLNode<T>* &_root);
    // 定义了上面两种旋转方式，但实际上还有"双旋"的情况，但是双旋是可以分步完成的！分别是不同情况的失衡时的解决策略
    void LeftBalance(AVLNode<T>* &_root);
    void RightBalance(AVLNode<T>* &_root);
    // 展示是否有序！
    void MidOrderTraverse(AVLNode<T> *_root);
    int GetHeight(AVLNode<T> *_root);
};

template<typename T>
bool AVLTREE<T>::search(AVLNode<T>* _root, T data){
    // 这里和BSTREE的实现方式不一样，我们用递归的方式来寻找
    if(_root){
        if(_root->data == data){
            return true;
        }
        else if(_root->data > data){
            return search(_root->left, data);
        }
        else{
            return search(_root->right, data);
        }
    }
    return false;
}

template<typename T>
void AVLTREE<T>::R_Rotate(AVLNode<T>* &_root){
    AVLNode<T> *left_sub_tree = _root->left;
    _root->left = left_sub_tree->right;
    left_sub_tree->right = _root;
    _root = left_sub_tree;
}

template<typename T>
void AVLTREE<T>::L_Rotate(AVLNode<T>* &_root){
    AVLNode<T>* right_sub_tree = _root->right;
    _root->right = right_sub_tree->left;
    right_sub_tree->left = _root;
    _root = right_sub_tree;
}

template<typename T>
void AVLTREE<T>::LeftBalance(AVLNode<T>* &_root){
    AVLNode<T> *leftsubtree = _root->left;
    AVLNode<T> *rightsubtree = leftsubtree->right;
    switch (leftsubtree->balance_factor)
    {
    case LeftHigher:
        /*
        考虑这种情况，遇到第一个结点不平衡，且是因为左边过高才调用的LeftBalance函数。
        在此基础上自己的左子树根结点的平衡因子也是左高，就是我们的 朝左链成一条的不平衡情况
        此时只需做一个右旋就可，且旋转完两者均平衡！
        */
        _root->balance_factor = Equal;
        leftsubtree->balance_factor = Equal;
        R_Rotate(_root);
        break;
    
    case RightHigher:
        // 根据不同的情况先设置这些个结点的balance_factor
        switch (rightsubtree->balance_factor)
        {
        case Equal:
            _root->balance_factor = Equal;
            leftsubtree->balance_factor = Equal;
            break;
        
        case LeftHigher:
            rightsubtree->balance_factor = leftsubtree->balance_factor = Equal;
            _root->balance_factor = RightHigher;
            break;
        
        case RightHigher:
            _root->balance_factor = rightsubtree->balance_factor = Equal;
            leftsubtree->balance_factor = LeftHigher;
            break;

        default:
            break;
        }
        L_Rotate(_root->left);
        R_Rotate(_root);
        break;

    default:
        break;
    }
}

template<typename T>
void AVLTREE<T>::RightBalance(AVLNode<T>* &_root){
    AVLNode<T> *rightsubtree = _root->right;
    AVLNode<T> *leftsubtree = rightsubtree->left;
    switch (rightsubtree->balance_factor)
    {
    case RightHigher:
        _root->balance_factor = Equal;
        rightsubtree->balance_factor = Equal;
        L_Rotate(_root);
        break;
    
    case LeftHigher:
        // 根据不同的情况先设置这些个结点的balance_factor
        switch (leftsubtree->balance_factor)
        {
        case Equal:
            _root->balance_factor = Equal;
            rightsubtree->balance_factor = Equal;
            break;
        
        case LeftHigher:
            _root->balance_factor = leftsubtree->balance_factor = Equal;
            rightsubtree->balance_factor = RightHigher;
            break;
        
        case RightHigher:
            rightsubtree->balance_factor = leftsubtree->balance_factor = Equal;
            _root->balance_factor = LeftHigher;
            break;

        default:
            break;
        }
        R_Rotate(_root->right);
        L_Rotate(_root);
        break;

    default:
        break;
    }
}

template<typename T>
void AVLTREE<T>::insert(AVLNode<T>* &_root, T data){
    if(!_root){                    // 用指针的引用可以直接连接到树上   因为指针的引用可以改变原父结点的孩子的地址指向！
        _root = new AVLNode<T>(data);
        taller = true;
    }
    else{
        if(_root->data > data){
            // _root->balance_factor += 1;
            insert(_root->left, data);
            if(taller){
                switch (_root->balance_factor)
                {
                case RightHigher:
                    _root->balance_factor = Equal;
                    taller = false;
                    break;
                
                case Equal:
                    _root->balance_factor = LeftHigher;
                    taller = true;
                    break;

                case LeftHigher:
                    LeftBalance(_root);
                    taller = false;
                    break;

                default:
                    break;
                }
            }
        }
        else{
            // _root->balance_factor -= 1;
            insert(_root->right, data);
            if(taller){
                switch (_root->balance_factor)
                {
                case LeftHigher:
                    _root->balance_factor = Equal;
                    taller = false;
                    break;

                case Equal:
                    _root->balance_factor = RightHigher;
                    taller = true;
                    break;
                
                case RightHigher:
                    RightBalance(_root);
                    taller = false;
                    break;

                default:
                    break;
                }
            }
        }
    }
}

template<typename T>
void AVLTREE<T>::MidOrderTraverse(AVLNode<T>* _root){
    if(_root){
        MidOrderTraverse(_root->left);
        cout << _root->data << " ";
        MidOrderTraverse(_root->right);
    }
}

template<typename T>
int AVLTREE<T>::GetHeight(AVLNode<T>* _root){
    // 这里利用层序遍历【队列的方法】来进行求解高度
    if(!_root)
        return 0;
    queue<AVLNode<T> *> que;
    que.push(_root);
    int height = 0;
    while(!que.empty()){
        for (size_t i = 0; i < que.size(); i ++){
            AVLNode<T> *temp = que.front();
            if(temp->left)
                que.push(temp->left);
            if(temp->right)
                que.push(temp->right);
            que.pop();
        }
        height++;
    }
    return height;
}

int main(){
    AVLTREE<int> tree;
    AVLNode<int>* root = tree.getroot();
    vector<int> vec{-4, 0, 7, 4, 9, -6, -1, -7};
    vector<int> vec2{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};   // 用一个有序数组，对比BST和AVL树，性能差别就很明显了！
    // 在BST里会是一条长度为10[即树的高度]的链表，但在AVL中，树高仅为4！！ 这显然好很多，搜索复杂度就牛逼起来了！！

    for(auto num : vec2){
        if(!tree.search(root, num))
            tree.insert(root, num);
        else{
            cout << "It has been in the tree!" << endl;
        }
    }
    cout << "current root is:" << root->data << endl;
    cout << "height of the tree is:" << tree.GetHeight(root) << endl;
    tree.MidOrderTraverse(root);
    return 0;
}

