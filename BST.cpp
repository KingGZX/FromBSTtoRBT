#include <iostream>
#include <vector>
using namespace std;

template<typename T>
struct TreeNode{
    T data;
    TreeNode *parent;
    TreeNode *left;
    TreeNode *right;
    TreeNode(T num, TreeNode* l = NULL, TreeNode* r = NULL):data(num), left(l), right(r){}
};

template<typename T>
class BSTree{
    private:
        TreeNode<T> *root;
    public:
        BSTree() { root = NULL; }
        TreeNode<T> *getroot() { return root; }
        bool createTree(vector<T> vec);
        bool insert(T data);
        bool deletenode(T data);
        bool search(T data);
};

template<typename T>
bool BSTree<T>::createTree(vector<T> vec){
    int num = 0;
    bool flag = true;
    if(root == NULL){
        root = new TreeNode<T>(vec[0]);
        root->parent = NULL;
        num = 1;
    }
    for (size_t i = num; i < vec.size(); i ++){
        flag = insert(vec[i]);
        if(!flag){
            cout << "false at:" << vec[i] << endl;
            break;
        }
    }
    return flag;
}

template<typename T>
bool BSTree<T>::insert(T data){    // 这里采用的是非递归的方法，递归显然会更加容易
    TreeNode<T> *node = new TreeNode<T>(data);
    node->data = data;
    TreeNode<T> *temp = root;
    while(temp){
        if(data < temp->data){
            if(temp->left)
                temp = temp->left;
            else break;
        }
        else if(data > temp->data){
            if(temp->right)
                temp = temp->right;
            else break;
        }
        else
            return false;
    }
    if(data < temp->data){
        temp->left = node;
        node->parent = temp;
    }
    else{
        temp->right = node;
        node->parent = temp;
    }
    return true;
}

template<typename T>
bool BSTree<T>::deletenode(T data){
    if(!search(data)){
        return false;
    }
    TreeNode<T> *temp = root;
    while(temp){
        if(data > temp->data){
            temp = temp->right;
        }
        else if(data < temp->data){
            temp = temp->left;
        }
        else{
            /* 
            这里找到了要删除的那个节点，删除的具体步骤如下：
            找到此节点的左子树上的最右节点为新的根(代替被删除节点)，然后左子树挂到这个节点的左边。
            同时这个节点一定没有右孩子了但也许还有左孩子，把这个左孩子放到原先自己的位置上即可
            */
            TreeNode<T> *p = temp;                  // 用这一根指针指向被删除的节点
            TreeNode<T> *leftsubtree = temp->left;  // 用这一根指针指向被删除节点的左子树
            TreeNode<T> *rightsubtree = temp->right;
            TreeNode<T> *father = temp->parent;   // 先找到被删除节点的父节点
            temp = temp->left;
            if(!temp){
                if(father == NULL){
                    root = rightsubtree;
                }
                else{
                    father->left = rightsubtree;
                    rightsubtree->parent = father;
                }
            }
            else if(!temp->right){
                temp->right = rightsubtree;
                rightsubtree->parent = temp;
                if(father == NULL)
                    root = temp;
                else{
                    father->left = temp;
                }
                temp->parent = father;
            }
            else{
                while(temp && temp->right){
                    temp = temp->right;
                }  // 经过这个步骤我们的temp已经是指向自己左子树的最右节点了！
                TreeNode<T> *newroot = temp;   // 最右节点命名为新根
                TreeNode<T> *newrootleftsubtree = newroot->left;   //  最右节点的左子树应该挂到自己原来的父亲右边去！
                /* 
                所以变量设定完成，接下来开始挂载操作
                */
                newroot->parent->right = newrootleftsubtree;
                if(father == NULL)
                    root = newroot;
                else
                    father->left = newroot;
                newroot->left = leftsubtree;
                newroot->right = rightsubtree;
                leftsubtree->parent = newroot;
                rightsubtree->parent = newroot;
            }
            delete[] p;
        }
    }
    return true;
}

template<typename T>
bool BSTree<T>::search(T data){
    TreeNode<T> *temp = root;
    while(temp){
        if(data > temp->data){
            temp = temp->right;
        }
        else if(data < temp->data){
            temp = temp->left;
        }
        else{
            return true;
        }
    }
    return false;
}


template<typename T>
void MidOrder(TreeNode<T>* root){
    if(root){
        MidOrder(root->left);
        cout << root->data << " ";
        MidOrder(root->right);
    }
}

int main(){
    BSTree<int> tree;
    vector<int> vec{-4, 0, 7, 4, 9, -6, -1, -7};    // 插入时数组需要无重复元素，否则会在中间直接报错
    bool ans = tree.createTree(vec);
    cout << boolalpha << ans << endl;
    MidOrder(tree.getroot());
    cout << endl;
    tree.deletenode(-4);
    MidOrder(tree.getroot());
    return 0;
}
