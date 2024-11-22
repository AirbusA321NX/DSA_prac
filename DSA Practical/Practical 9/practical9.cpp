#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// AVL Tree Node
struct AVLNode
{
    int key;
    struct AVLNode *left;
    struct AVLNode *right;
    int height;
};

// B-tree Node
struct BTreeNode
{
    int *keys;
    int t;
    struct BTreeNode **children;
    int n;
    int leaf;
};

// AVL Tree Functions
struct AVLNode *newAVLNode(int key)
{
    struct AVLNode *node = (struct AVLNode *)malloc(sizeof(struct AVLNode));
    node->key = key;
    node->left = node->right = NULL;
    node->height = 1;
    return node;
}

int height(struct AVLNode *node)
{
    return node ? node->height : 0;
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

int getBalance(struct AVLNode *node)
{
    return node ? height(node->left) - height(node->right) : 0;
}

struct AVLNode *rightRotate(struct AVLNode *y)
{
    struct AVLNode *x = y->left;
    struct AVLNode *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

struct AVLNode *leftRotate(struct AVLNode *x)
{
    struct AVLNode *y = x->right;
    struct AVLNode *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

struct AVLNode *insert(struct AVLNode *node, int key)
{
    if (!node)
        return newAVLNode(key);

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else
        return node; // Duplicates not allowed

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    // Left Left Case
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && key > node->left->key)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && key < node->right->key)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

struct AVLNode *search(struct AVLNode *root, int key)
{
    if (!root || root->key == key)
        return root;

    if (root->key < key)
        return search(root->right, key);

    return search(root->left, key);
}

void inorderTraversal(struct AVLNode *root)
{
    if (root)
    {
        inorderTraversal(root->left);
        printf("%d ", root->key);
        inorderTraversal(root->right);
    }
}

// B-tree Functions
struct BTreeNode *newBTreeNode(int t, int leaf)
{
    struct BTreeNode *node = (struct BTreeNode *)malloc(sizeof(struct BTreeNode));
    node->t = t;
    node->leaf = leaf;
    node->keys = (int *)malloc((2 * t - 1) * sizeof(int));
    node->children = (struct BTreeNode **)malloc(2 * t * sizeof(struct BTreeNode *));
    node->n = 0;
    return node;
}

void splitChild(struct BTreeNode *parent, int i, struct BTreeNode *child)
{
    int t = parent->t;
    struct BTreeNode *newNode = newBTreeNode(t, child->leaf);
    newNode->n = t - 1;

    for (int j = 0; j < t - 1; j++)
    {
        newNode->keys[j] = child->keys[j + t];
    }

    if (!child->leaf)
    {
        for (int j = 0; j < t; j++)
        {
            newNode->children[j] = child->children[j + t];
        }
    }

    child->n = t - 1;

    for (int j = parent->n; j > i; j--)
    {
        parent->children[j + 1] = parent->children[j];
    }
    parent->children[i + 1] = newNode;

    for (int j = parent->n - 1; j > i; j--)
    {
        parent->keys[j + 1] = parent->keys[j];
    }

    parent->keys[i] = child->keys[t - 1];
    parent->n++;
}

void insertNonFull(struct BTreeNode *node, int key)
{
    int i = node->n - 1;

    if (node->leaf)
    {
        while (i >= 0 && node->keys[i] > key)
        {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->n++;
    }
    else
    {
        while (i >= 0 && node->keys[i] > key)
        {
            i--;
        }
        i++;

        if (node->children[i]->n == 2 * node->t - 1)
        {
            splitChild(node, i, node->children[i]);

            if (node->keys[i] < key)
            {
                i++;
            }
        }
        insertNonFull(node->children[i], key);
    }
}

void insertBTree(struct BTreeNode **root, int t, int key)
{
    struct BTreeNode *r = *root;
    if (r->n == 2 * t - 1)
    {
        struct BTreeNode *s = newBTreeNode(t, false);
        s->children[0] = r;
        *root = s;
        splitChild(s, 0, r);
        insertNonFull(s, key);
    }
    else
    {
        insertNonFull(r, key);
    }
}

void displayBTree(struct BTreeNode *node, int level)
{
    if (node == NULL)
        return;

    printf("\nLevel %d: ", level);

    for (int i = 0; i < node->n; i++)
    {
        printf("%d ", node->keys[i]);
    }

    if (!node->leaf)
    {
        for (int i = 0; i <= node->n; i++)
        {
            displayBTree(node->children[i], level + 1);
        }
    }
}

// Main function
int main()
{
    struct AVLNode *avlRoot = NULL;
    int data[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97};
    int degree;

    for (int i = 0; i < 25; i++)
    {
        avlRoot = insert(avlRoot, data[i]);
    }

    printf("AVL Tree In-order Traversal:\n");
    inorderTraversal(avlRoot);

    printf("\nEnter degree of B-tree: ");
    scanf("%d", &degree);

    struct BTreeNode *btreeRoot = newBTreeNode(degree, 1);

    for (int i = 0; i < 25; i++)
    {
        insertBTree(&btreeRoot, degree, data[i]);
    }

    int choice;
    do
    {
        printf("\nAVL Tree operations:\n");
        printf("1. Insert\n2. Search\n3. Display AVL Tree\n4. Display B-tree\n5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
        {
            int key;
            printf("Enter key to insert: ");
            scanf("%d", &key);
            avlRoot = insert(avlRoot, key);
            printf("Key inserted successfully.\n");
            break;
        }
        case 2:
        {
            int key;
            printf("Enter key to search: ");
            scanf("%d", &key);
            struct AVLNode *result = search(avlRoot, key);
            if (result)
            {
                printf("Key found in AVL tree.\n");
            }
            else
            {
                printf("Key not found in AVL tree.\n");
            }
            break;
        }
        case 3:
            printf("AVL Tree In-order Traversal:\n");
            inorderTraversal(avlRoot);
            break;
        case 4:
            printf("B-tree structure:\n");
            displayBTree(btreeRoot, 0);
            break;
        case 5:
            printf("Exiting program.\n");
            break;
        default:
            printf("Invalid choice.\n");
            break;
        }
    } while (choice != 5);

    return 0;
}