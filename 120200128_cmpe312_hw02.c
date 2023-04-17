#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Define a node in the binary tree
typedef struct node {
    int data;
    struct node* left;
    struct node* right;
} Node;

typedef struct tree {
    Node* root;
    pthread_mutex_t mutex;
} Tree;

typedef struct binaryTreeFunctionArgs {
    Tree* tree;
    int data;
} callArgs;

// Function to create a new node with the given data
Node* createNode(int data) {
    // Allocate memory for the new node
    Node* newNode = malloc(sizeof(Node));
    // if unable to allocate the memory exit with an error
    if (newNode == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory for new node\n");
        exit(1);
    }
    // Initialize the node's data and children pointers
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Function to insert a new node with the given data into the binary tree
Node* insert(Node* root, int data) {
    if (root == NULL) {
        return createNode(data);
    }
    if (data < root->data) {
        root->left = insert(root->left, data);
    } else if (data > root->data) {
        root->right = insert(root->right, data);
    }
    return root;
}

// Function to search for a node with the given data in the binary tree
Node* search(Node* root, int data) {
    if (root == NULL || root->data == data) {
        return root;
    }
    if (data < root->data) {
        return search(root->left, data);
    } else {
        return search(root->right, data);
    }
}

// Function to traverse the binary tree in order and print out the data of each node
void traverse(Node* root) {
    if (root != NULL) {
        traverse(root->left);
        printf("%d ", root->data);
        traverse(root->right);
    }
}

// Main function for testing
int main() {
    Tree tree;
    printf("test");
    tree.root = NULL;
    tree.root = insert(tree.root, 10);
    insert(tree.root, 5);
    insert(tree.root, 15);
    insert(tree.root, 3);
    insert(tree.root, 7);
    insert(tree.root, 12);
    insert(tree.root, 17);
    printf("Inorder traversal: ");
    traverse(tree.root);
    printf("\n");
    Node* node = search(tree.root, 7);
    if (node != NULL) {
        printf("Found node with data %d\n", node->data);
    } else {
        printf("Node not found\n");
    }
    return 0;
}
