#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#define COUNT 6
using namespace std;

struct node
{
    int key;
    struct node *left, *right;
};

struct node *newNode(int item)
{
    struct node *temp = (struct node *)malloc(sizeof(struct node));
    temp->key = item;
    temp->left = temp->right = NULL;
    return temp;
}

void postorder(node *p, int indent = 5)
{
    if (p != NULL)
    {
        if (p->right)
        {
            postorder(p->right, indent + 4);
        }
        if (indent)
        {
            std::cout << std::setw(indent) << ' ';
        }
        if (p->right)
            std::cout << " /\n"
                      << std::setw(indent) << ' ';
        std::cout << p->key << "\n ";
        if (p->left)
        {
            std::cout << std::setw(indent) << ' ' << " \\\n";
            postorder(p->left, indent + 4);
        }
    }
}

struct node *insert(struct node *node, int key)
{
    if (node == NULL)
        return newNode(key);

    if (key < node->key)
        node->left = insert(node->left, key);
    else
        node->right = insert(node->right, key);

    return node;
}

struct node *minValueNode(struct node *node)
{
    struct node *current = node;

    while (current && current->left != NULL)
        current = current->left;

    return current;
}

struct node *deleteNode(struct node *root, int key)
{
    if (root == NULL)
        return root;

    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);

    else
    {
        if (root->left == NULL)
        {
            struct node *temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL)
        {
            struct node *temp = root->left;
            free(root);
            return temp;
        }

        struct node *temp = minValueNode(root->right);

        root->key = temp->key;
        root->right = deleteNode(root->right, temp->key);
    }
    return root;
}

void _clearTree(struct node *root)
{
    if (root == NULL)
    {
        return;
    }
    _clearTree(root->left);
    _clearTree(root->right);
    free(root);
}

struct node *clearTree(struct node *root)
{
    _clearTree(root);
    root = NULL;
    return root;
}

int main()
{
    struct node *root = NULL;
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 20);
    root = insert(root, 40);

    postorder(root);

    printf("\nDelete 30\n");
    root = deleteNode(root, 30);
    postorder(root);

    printf("\nDelete 50\n");
    root = deleteNode(root, 50);
    postorder(root);

    root = clearTree(root);

    postorder(root);

    return 0;
}