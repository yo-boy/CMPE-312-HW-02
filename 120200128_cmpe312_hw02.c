#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Define a node in the binary tree
typedef struct node {
    int data;
    struct node* left;
    struct node* right;
} Node;

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
    printf("test");
    Node* binaryTreeRoot = NULL;
    binaryTreeRoot = insert(binaryTreeRoot, 10);
    insert(binaryTreeRoot, 5);
    insert(binaryTreeRoot, 15);
    insert(binaryTreeRoot, 3);
    insert(binaryTreeRoot, 7);
    insert(binaryTreeRoot, 12);
    insert(binaryTreeRoot, 17);
    printf("Inorder traversal: ");
    traverse(binaryTreeRoot);
    printf("\n");
    Node* node = search(binaryTreeRoot, 7);
    if (node != NULL) {
        printf("Found node with data %d\n", node->data);
    } else {
        printf("Node not found\n");
    }
    return 0;
}
