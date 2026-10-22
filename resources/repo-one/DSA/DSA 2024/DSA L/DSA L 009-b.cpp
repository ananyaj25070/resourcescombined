#include <iostream>
using namespace std;

struct node 
{
    int key;
    int height;
    node* left;
    node* right;

    node(int k) 
    {
        key = k;
        height = 1;
        left = nullptr;
        right = nullptr;
    }
};

struct avl 
{
    node* root;
    int bf;

    avl() 
    {
        root = nullptr;
        bf = 0;
    }

    int height(node* node) 
    {
        if (node == nullptr) return 0;
        return node->height;
    }

    int balancefactor(node* node) 
    {
        if (node == nullptr) return 0;
        return height(node->left) - height(node->right);
    }

    void updateheight(node* node) 
    {
        node->height = 1 + max(height(node->left), height(node->right));
    }

    node* rightrotate(node* y) 
    {
        node* x = y->left;
        node* T = x->right;
        x->right = y;
        y->left = T;
        updateheight(y);
        updateheight(x);
        return x;
    }

    node* leftrotate(node* x) 
    {
        node* y = x->right;
        node* T = y->left;
        y->left = x;
        x->right = T;
        updateheight(x);
        updateheight(y);
        return y;
    }

    node* insert(node* node, int key) 
    {   
        node* z = new node(key);
        if (node == nullptr) 
        {
            return z;
        }

        if (key < node->key) node->left = insert(node->left, key);
        else if (key > node->key) node->right = insert(node->right, key);

        updateheight(node);

        int balanceFactor = balancefactor(node);

        if (balanceFactor > 1 && key < node->left->key) return rightrotate(node);

        if (balanceFactor < -1 && key > node->right->key) return leftrotate(node);

        if (balanceFactor > 1 && key > node->left->key) 
        {
            node->left = leftrotate(node->left);
            return rightrotate(node);
        }

        if (balanceFactor < -1 && key < node->right->key) 
        {
            node->right = rightrotate(node->right);
            return leftrotate(node);
        }

        return node;
    }

    void insert(int key) 
    {
        root = insert(root, key);
    }

    node* minValueNode(node* node) 
    {   
        node* current;
        current = node;
        while (current && current->left != nullptr)
            current = current->left;
        return current;
    }

    node* remove(node* node, int key) {
    if (node == nullptr) return node;

    if (key < node->key) node->left = remove(node->left, key);
    else if (key > node->key) node->right = remove(node->right, key);
    else 
    {
        if (node->left == nullptr || node->right == nullptr) 
        {   
            node* temp;
            temp = node->left ? node->left : node->right;
            if (temp == nullptr) 
            {
                temp = node;
                node = nullptr;
            } 
            else
                *node = *temp;
            delete temp;
        } 
        else 
        {
            node* temp = minValueNode(node->right);
            node->key = temp->key;
            node->right = remove(node->right, temp->key);
        }
    }

    if (node == nullptr) return node;

    updateheight(node);

    int balanceFactor = balancefactor(node);

    if (balanceFactor > 1 && balancefactor(node->left) >= 0) return rightrotate(node);

    if (balanceFactor < -1 && balancefactor(node->right) <= 0) return leftrotate(node);

    if (balanceFactor > 1 && balancefactor(node->left) < 0) 
    {
        node->left = leftrotate(node->left);
        return rightrotate(node);
    }
    if (balanceFactor < -1 && balancefactor(node->right) > 0) 
    {
        node->right = rightrotate(node->right);
        return leftrotate(node);
    }
    return node;
}


    void remove(int key) 
    {
        root = remove(root, key);
    }

    node* search(int key) 
    {
        node* current = root;
        while (current != nullptr) 
        {
            if (current->key == key)
                return current;
            else if (current->key < key)
                current = current->right;
            else
                current = current->left;
        }
        return nullptr;
    }
};

int main() {
    return 0;
}
