#include<iostream>
using namespace std;

//struct for node
struct node
{
    int key;
    node* left;
    node* right;
    node* prnt;

    node(int k) : key(k), left(nullptr), right(nullptr), prnt(nullptr) {}
};

//struct for BST
struct BST
{
    node* root;

    //constructor
    BST()
    {
        root = nullptr;
    }

    //function to insert into tree
    //theta(h) complexity
    void insert(int key)
    {   
        node* z = new node(key);
        node* y = nullptr;
        node* x = root;

        while (x != nullptr)
        {
            y = x;
            if (z->key < x->key)
                x = x->left;
            else
                x = x->right;
        }

        z->prnt = y;
        if (y == nullptr)
            root = z; 
        else if (z->key < y->key)
            y->left = z;
        else
            y->right = z;
    }

    //function to search minimum element in the tree
    //theta(h) complexity
    node* minimum(node* root)
    {
        while(root->left != nullptr)
        {
            root = root -> left;
        }
        return root;
    }

    //function to transplant subtrees
    //to be used in deletion
    //O(1)
    void transplant(BST& bst, node* u, node* v)
    {
        if(u->prnt == nullptr)
        {
            bst.root = v;
        }
        else if (u == u->prnt->left)
        {
            u->prnt->left = v;
        }
        else
        {
            u->prnt->right = v;
        }

        if(v != nullptr)
        {
            v->prnt = u->prnt;
        }
    }

    //function to delete nodes
    //o(h)
    void deletion(BST& bst, node* z)
    {
        if(z->left == nullptr)
        {
            transplant(bst, z, z->right);
        }
        
        else if(z->right == nullptr)
        {
            transplant(bst, z, z->left);
        }

        else
        {
            node* y = minimum(z->right);
            if(y != z->right)
            {
                transplant(bst, y, y->right);
                y->right = z->right;
                y->right->prnt = y;
            }

            transplant(bst, z, y);
            y->left = z->left;
            y->left->prnt = y;
        }
    }

    //function to perfrom in order walk
    //theta(n)
    //T(n) = T(k) + T(n-k+1) + c
    void inorder(node* root)
    {
        if(root != nullptr)
        {
            inorder(root->left);
            cout<<root->key<<" ";
            inorder(root->right);
        }
    }

    //function to perfrom pre order walk
    //theta(n)
    //T(n) = T(k) + T(n-k+1) + c
    void preorder(node* root)
    {
        if(root != nullptr)
        {   
            cout<<root->key<<" ";
            preorder(root->left);
            preorder(root->right);
        }
    }

    //function to perfrom post order walk
    //theta(n)
    //T(n) = T(k) + T(n-k+1) + c
    void postorder(node* root)
    {
        if(root != nullptr)
        {   
            postorder(root->left);
            postorder(root->right);
            cout<<root->key<<" ";
        }
    }

    //function to find height of BST
    //O(n) n is number of nodes
    int height(node* root) 
    {
    if (root == NULL) return -1;
    int leftHeight = height(root->left);
    int rightHeight = height(root->right);
    return 1 + max(leftHeight, rightHeight);
    }

    //function to find number of nodes
    //O(n) n is number of nodes
    int nonodes(node* root) 
    {
    if (root == NULL)
        return 0;
    return 1 + nonodes(root->left) + nonodes(root->right);
    }
};

int main()
{
    //idk implement toh kardiya ye sab karne ko bola nahi tha sorta
    return 0;
}
