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

    void inorder(node* root, int arr[], int& count)
    {
        if(root != nullptr)
        {
            inorder(root->left, arr, count);
            arr[count] = root->key;
            count++;
            inorder(root->right, arr, count);
        }
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

    int k;
    cout<<"Enter K: ";
    cin>>k;

    int arr[sz];
    int count = 0;
    bst.inorder(bst.root, arr, count);

    cout<<"The output is: "<<arr[k-1];

}