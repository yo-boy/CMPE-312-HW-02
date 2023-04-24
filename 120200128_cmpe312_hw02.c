#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Define a node in the binary tree
typedef struct node {
    int data;
    struct node* left;
    struct node* right;
} Node;

// the tree structure holds a mutex to insure synchronization and that no inserts or searches happen at the same time
typedef struct tree {
    Node* root;
    pthread_mutex_t mutex;
} Tree;

// this struct holds all the arguments needed for a search or an insert, we use it since a thread function cannot take anything other than a void* pointer
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
        root = createNode(data);
        return root;
    }
    if (data < root->data) {
        root->left = insert(root->left, data);
    } else if (data > root->data) {
        root->right = insert(root->right, data);
    }
    return root;
}

// Function to search for a node with the given data in the binary tree
void search(Node* root, int data) {
    if (root == NULL) {
        printf("Node with data %d not found\n", data);
        return;
    }
    if (root->data == data) {
        printf("Found node with data %d\n", data);
        return;
    }
    if (data < root->data) {
        search(root->left, data);
    } else {
        search(root->right, data);
    }
}

// Function to traverse the binary tree in order and print out the data of each node
void traversehelper(Node* root) {
    if (root != NULL) {
        traversehelper(root->left);
        printf("%d ", root->data);
        traversehelper(root->right);
    }
}
// the extra function is to just print it nicely while keeping the recursive nature of the traversal
void traverse(Node* root){
    printf("Inorder traversal: ");
    traversehelper(root);
    printf("\n");
}

// Initialize the root node of a tree
void initTree(Tree* tree, int data){
    tree->root = createNode(data);
}

// this function can be run in a thread, it locks the mutex of the tree and then runs a search before unlocking the mutex again
void* threadedSearch(void* args){
    callArgs* arguments = (callArgs*) args;
    pthread_mutex_lock(&arguments->tree->mutex);
    search(arguments->tree->root, arguments->data);
    pthread_mutex_unlock(&arguments->tree->mutex);
    return NULL;
}

// this function locks the mutex of the tree and then inserts the data we give it before unlocking the mutex again
void* threadedInsert(void* args){
    callArgs* arguments = (callArgs*) args;
    pthread_mutex_lock(&arguments->tree->mutex);
    insert(arguments->tree->root, arguments->data);
    pthread_mutex_unlock(&arguments->tree->mutex);
    return NULL;
}

int main() {
    // we define and Initialize the tree and then fill it and search it with non-threaded functions
    Tree tree;
    initTree(&tree, 10);
    insert(tree.root, 5);
    insert(tree.root, 12);
    insert(tree.root, 17);
    traverse(tree.root);
    search(tree.root, 12);
    search(tree.root, 17);
    search(tree.root, 123);
    traverse(tree.root);
    insert(tree.root, 123);
    traverse(tree.root);
    search(tree.root, 123);

    // Initialize the mutex before usage
    pthread_mutex_init(&tree.mutex, NULL);

    // we create two threads for search and insert
    pthread_t t1;
    pthread_t t2;
    // defining the arguments that will be passed to the thread functions
    callArgs args = {.tree = &tree, .data = 7};
    callArgs diffArgs = {.tree = &tree, .data = 13345};
    /*
    * here we call both an insert and a search with the same data (that isn't already in the tree),
    * each time we run this program the result can be different
    * because we cannot predict which thread will go first and lock the tree,
    * this is non-deterministic behavior, if we want to avoid it we-
    * must wait for all threads that insert into the tree to finish and join-
    * before we search for the same values they had inserted with another thread.
    */
    pthread_create(&t2, NULL, threadedInsert, &args);
    pthread_create(&t1, NULL, threadedSearch, &args);
    // we wait for both threads to finish before using them again
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    // here we do the same thing again
    pthread_create(&t2, NULL, threadedInsert, &diffArgs);
    pthread_create(&t1, NULL, threadedSearch, &diffArgs);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    // destroy the mutex after we're done with it
    pthread_mutex_destroy(&tree.mutex);

    return 0;
}
