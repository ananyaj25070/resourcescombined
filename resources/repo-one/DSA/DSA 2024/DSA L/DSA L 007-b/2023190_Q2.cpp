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
    int minimum(node* root)
    {
        while(root->left != nullptr)
        {
            root = root -> left;
        }
        return root->key;
    }

    //function to search maximum element in the tree
    //theta(h) complexity
    int maximum(node* root)
    {
        while(root->right != nullptr)
        {
            root = root -> right;
        }
        return root->key;
    }


    // function to check if valid BST
    bool validity(node* root)
    {
        if (root == nullptr)
        {
            return true;
        }

        bool left_valid = (root->left == nullptr || (root->left->key < root->key && validity(root->left)));
        bool right_valid = (root->right == nullptr || (root->right->key > root->key && validity(root->right)));

        return left_valid && right_valid;
    }

};

int main()
{
    int sz;
    cout<<"Enter number of elements in BST: ";
    cin>>sz;

    BST bst;
    int in;
    cout<<"Enter space seperated BST elements: ";
    for(int i = 0; i <sz; i++)
    {
        cin>>in;
        bst.insert(in);
    }

    cout<<endl<<bst.validity(bst.root);
}