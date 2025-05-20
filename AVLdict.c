#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#define COUNT 5
#define MAX 100
typedef struct Node
{
    char data[MAX];
    struct Node *left, *right;
    int height;
}Node;

Node* createNode(char*);
int getHeight(Node*);
int getBalance(Node*);
int max(int ,int);
void updateHeight(Node*);
Node* rightRotate(Node*);
Node* leftRotate(Node*);
Node* insert(Node* ,char*);
Node* search(Node*,char*,Node*);
Node* findInorderPredecessor(Node*,char*);
Node* findInorderSuccessor(Node*,char*);
int countNodes(Node* );
void freeTree(Node*);
void preprocessWord(char*);
Node* loadDictionary(char* );
void displayTreeStats(Node* );
void checkSpelling(Node*,char*);
void processSentence(Node* , char* );
void printTree(Node* , int);

void main(){
    Node *root = NULL;
    char *arr[] = {"hello", "world", "hi", "avl"};
    for (int i = 0; i < 4; i++)
    {
        root = insert(root, arr[i]);
    }
    printTree(root, 0); 
    //FILE *fptr = fopen("Dictionary.txt", "r");
    //fclose(fptr);
}

int getBalance(Node *n){
    if (n == NULL) return 0;
    return height(n->left) - height(n->right);
}

int max(int a, int b){
    return (a > b) ? a : b;
}

int height(Node *n){
    if (n == NULL) return 0;
    return n->height;
}

Node *newNode(char *data){
    Node *node = (Node*)malloc(sizeof(Node));
    strncpy(node->data, data, MAX);
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

Node *rightrotate(Node *y){
    Node *x = y->left;
    Node *T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

Node *leftRotate(Node *x){
    Node *y = x->right;
    Node *T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;    
    return y;
}

Node *insert(Node *node, char *data){
    if (node == NULL) return newNode(data);
    int compare_value = strcasecmp(data, node->data);
    
    if (compare_value < 0) node->left = insert(node->left, data);
    else if (compare_value > 0) node->right = insert(node->right, data);
    else return node;

    
    node->height = 1 + max(height(node->left), height(node->right));
    
    int balance = getBalance(node);

    if (balance > 1 && strcasecmp(data, node->left->data) < 0) return rightrotate(node);
    if (balance < -1 && strcasecmp(data, node->right->data) > 0) return leftRotate(node);
    if (balance > 1 && strcasecmp(data, node->left->data) > 0)
    {
        node->left = leftRotate(node->left);
        return rightrotate(node);
    }
    if (balance < -1 && strcasecmp(data, node->right->data) < 0)
    {
        node->right = rightrotate(node->right);
        return leftRotate(node);
    }
    return node;
}

Node *Search(Node *root, char *key){
    if (root == NULL) return NULL;
    
    if (strcasecmp(root->data, key) == 0) return root;
    else if (strcasecmp(root->data, key) < 0) return Search(root->left, key);
    else return Search(root->right, key);
    
    
}

void printTree(Node* root, int space) {
    if (root == NULL)
        return;

    space += COUNT;

    printTree(root->right, space);

    printf("\n");
    for (int i = COUNT; i < space; i++)
        printf(" ");
    printf("%s\n", root->data);

    printTree(root->left, space);
}

