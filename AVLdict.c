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

Node* createNode(char*); //Done
int getBalance(Node*); //Done
int max(int ,int); //Done
int height(Node*); //Done
Node* rightRotate(Node*); //Done
Node* leftRotate(Node*); //Done
Node* insert(Node* ,char*); //Done
Node* search(Node*,char*,Node*); //Done
Node* findInorderPredecessor(Node*,char*);
Node* findInorderSuccessor(Node*,char*);
int countNodes(Node* ); //Done
void freeTree(Node*); //Done
void preprocessWord(char*);
Node* loadDictionary(char* ); //Done
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
    char *dict= "dictionary.txt";
    root = loadDictionary(dict);
    if (root == NULL)
    {
        printf("Failed to load dictionary.\n");
        exit(1);
    }
    displayTreeStats(root);
}

int countNodes(Node *root) {
    if (root == NULL) return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
}

void displayTreeStats(Node* root) {
    printf("Dictionary loaded!\n");
    printf("Tree size: %d\n", countNodes(root));
    printf("Tree height: %d\n",height(root));
}

Node *loadDictionary(char *dict){
    char word[MAX];
    Node *root = NULL;
    FILE *fp = fopen(dict, "r");
    if (dict == NULL)
    {
        printf("Failed to open dictionary file: %s\n", dict);
        return NULL;
    }
    while(fgets(word,MAX,fp)){
        word[strcspn(word, "\r\n")] = 0;
        if (strlen(word) > 0)
        {
            root = insert(root, word);
        }
    }
    fclose(fp);
    return root;
}

int getBalance(Node *n)
{
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

void freeTree(Node* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}