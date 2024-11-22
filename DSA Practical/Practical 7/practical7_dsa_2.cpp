#include <bits/stdc++.h>
using namespace std;
struct Node{
    int data;
    struct Node *left;
    struct Node *right;
};
// Create a new node
struct Node *createNode(int data){
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}
// Insert nodes based on user input
void insertNode(struct Node **root){
    int value, direction;
    char input[10];
    set<int> nodeValues;
    if (*root == NULL){
        printf("Enter root node value: ");
        scanf("%d", &value);
        *root = createNode(value);
        nodeValues.insert(value);
    }
    struct Node *temp = *root;
    while (1){
        printf("Enter the value to insert (E to end input): ");
        scanf("%s", input);
        if (input[0] == 'E' || input[0] == 'e')
            break;

        value = atoi(input);
        // Check for duplicates
        if (nodeValues.count(value)){
            printf("Error: Duplicate value! Please enter a different value.\n");
            continue;
        }
        temp = *root;
        while (1){
            printf("Enter direction (1 for Left, 2 for Right) of node %d: ", temp->data);
            scanf("%d", &direction);
            if (direction == 1){
                if (temp->left == NULL){
                    temp->left = createNode(value);
                    nodeValues.insert(value); // Add the new value to the set
                    break;
                }
                else{
                    temp = temp->left;
                }
            }
            else if (direction == 2){
                if (temp->right == NULL){
                    temp->right = createNode(value);
                    nodeValues.insert(value); // Add the new value to the set
                    break;
                }
                else{
                    temp = temp->right;
                }
            }
            else{
                printf("Invalid direction! Try again.\n"); }
        }
    }
}
// Print the tree in a human-readable format with slashes
void printTree(struct Node *root, int space){
    if (root == NULL)
        return;
    int indent = 4; // Number of spaces for indentation
    // Print right child first
    if (root->right != NULL)
    {
        printTree(root->right, space + indent);
        for (int i = 0; i < space + indent - 2; i++)
            printf(" ");
        printf("  /\n");
    }
    // Print current node
    for (int i = 0; i < space; i++)
        printf(" ");
    printf("%d\n", root->data);
    // Print left child
    if (root->left != NULL)
    {
        for (int i = 0; i < space + indent - 2; i++)
            printf(" ");
        printf("  \\\n");
        printTree(root->left, space + indent);
    }
}
// Wrapper function to print the binary tree
void printBinaryTree(struct Node *root)
{
    printTree(root, 0);
}
// Pre-order traversal
void preOrder(struct Node *root, int *array, int *index)
{
    if (root != NULL)
    {
        array[(*index)++] = root->data; // Store node data
        preOrder(root->left, array, index);
        preOrder(root->right, array, index);
    }
}
// In-order traversal
void inOrder(struct Node *root, int *array, int *index)
{
    if (root != NULL)
    {
        inOrder(root->left, array, index);
        array[(*index)++] = root->data; // Store node data
        inOrder(root->right, array, index);
    }
}
// Post-order traversal
void postOrder(struct Node *root, int *array, int *index)
{
    if (root != NULL)
    {
        postOrder(root->left, array, index);
        postOrder(root->right, array, index);
        array[(*index)++] = root->data; // Store node data
    }
}
// Function to create a tree from traversal input
struct Node *createTreeFromTraversal(int *array, int start, int end)
{
    if (start > end)
        return NULL;
    // Create a new node with the current element
    struct Node *node = createNode(array[start]);
    // Recursively create left and right subtrees
    node->left = createTreeFromTraversal(array, start + 1, start + (end - start) / 2);
    node->right = createTreeFromTraversal(array, start + (end - start) / 2 + 1, end);
    return node;
}
struct Node *buildTree(int preOrder[], int inOrder[], int inStart, int inEnd, int *preIndex)
{
    if (inStart > inEnd)
    {
        return NULL;
    }
    // Create the root node using the current index of preOrder
    struct Node *root = createNode(preOrder[*preIndex]);
    (*preIndex)++;

    // If this node has no children, return
    if (inStart == inEnd)
    {
        return root;
    }
    // Find the index of this node in inOrder
    int inIndex;
    for (inIndex = inStart; inIndex <= inEnd; inIndex++)
    {
        if (inOrder[inIndex] == root->data)
        {
            break;
        }
    }
    // Recursively build the left and right subtrees
    root->left = buildTree(preOrder, inOrder, inStart, inIndex - 1, preIndex);
    root->right = buildTree(preOrder, inOrder, inIndex + 1, inEnd, preIndex);
    return root;
}
int main()
{
    struct Node *root = NULL;
    int choice;
    while (1)
    {
        printf("\n=== Binary Tree Operations ===\n");
        printf("1. Create Binary Tree\n");
        printf("2. Print tree via Pre-Order Traversal and In-Order Traversal\n");
        printf("3. Pre-order Traversal\n");
        printf("4. In-order Traversal\n");
        printf("5. Post-order Traversal\n");
        printf("6. Print Binary Tree\n");
        printf("7. Exit\n");
        printf("Enter your choice: \n");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            insertNode(&root); // Create the binary tree
            break;
        case 2:
        {
            int n;
            printf("Enter number of elements in the tree: ");
            scanf("%d", &n);
            int preOrder[n], inOrder[n];
            printf("Enter elements in Pre-order:\n");
            for (int i = 0; i < n; i++)
            {
                scanf("%d", &preOrder[i]);
            }
            printf("Enter elements in In-order:\n");
            for (int i = 0; i < n; i++)
            {
                scanf("%d", &inOrder[i]);
            }
            int preIndex = 0;
            root = buildTree(preOrder, inOrder, 0, n - 1, &preIndex);
            printf("Tree built successfully from Pre-order and In-order traversals.\n");
            printf("Binary Tree (in human-readable format):\n");
            printBinaryTree(root); // Print the newly built tree
            printf("\n");
            break;
        }
        case 3:
        {
            int preOrderArray[100], preIndex = 0;
            printf("Pre-order Traversal: ");
            preOrder(root, preOrderArray, &preIndex); // Pre-order traversal
            for (int i = 0; i < preIndex; i++)
            {
                printf("%d ", preOrderArray[i]);
            }
            printf("\n");
            break;
        }
        case 4:
        {
            int inOrderArray[100], inIndex = 0;
            printf("In-order Traversal: ");
            inOrder(root, inOrderArray, &inIndex); // In-order traversal
            for (int i = 0; i < inIndex; i++)
            {
                printf("%d ", inOrderArray[i]);
            }
            printf("\n");
            break;
        }
        case 5:
        {
            int postOrderArray[100], postIndex = 0;
            printf("Post-order Traversal: ");
            postOrder(root, postOrderArray, &postIndex); // Post-order traversal
            for (int i = 0; i < postIndex; i++)
            {
                printf("%d ", postOrderArray[i]);
            }
            printf("\n");
            break;
        }
        case 6:
        {
            printf("Binary Tree (in human-readable format):\n");
            printBinaryTree(root); // Print the tree
            printf("\n");
            break;
        }
        case 7:
            exit(0);
        default:
            printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}