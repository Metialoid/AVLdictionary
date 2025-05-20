#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define COUNT 5
#define MAX 100
/* Platform-specific includes and definitions */
#ifdef _WIN32
    #define strncasecmp _strnicmp
    #define strcasecmp _stricmp
#else
    #include <strings.h>  /* For strncasecmp on UNIX/Linux */
#endif

typedef struct Node
{
    char data[MAX];
    struct Node *left, *right;
    int height;
}Node;

int tree_max(int, int); //Done
Node* newNode(char*); //Done
int getBalance(Node*); //Done
int height(Node*); //Done
Node* rightRotate(Node*); //Done
Node* leftRotate(Node*); //Done
Node* insert(Node* ,char*); //Done
Node* Search(Node*,char*); //Done
Node* findInorderPredecessor(Node*,char*);//Done
Node* findInorderSuccessor(Node*,char*);//Done
int countNodes(Node* ); //Done
void freeTree(Node*); //Done
void preprocessWord(char*); //Done
Node* loadDictionary(char* ); //Done
void displayTreeStats(Node* ); //Done
void checkSpelling(Node*,char*);
void processSentence(Node* , char* ); //Done
void printTree(Node* , int); //Done

void main()
{
    Node *root = NULL;
    char *arr[] = {"hello", "world", "hi", "avl"};
    for (int i = 0; i < 4; i++)
    {
        root = insert(root, arr[i]);
    }
    printTree(root, 0); 
    Node *searchvalue = Search(root, "avl");
    char *dict= "dictionary.txt";
    root = loadDictionary(dict);
    if (root == NULL)
    {
        printf("Failed to load dictionary.\n");
        exit(1);
    }
    displayTreeStats(root);
    char sentence[MAX];
    while (1) {
        printf("\nEnter a sentence to check (or type '0' to quit): ");
        if (fgets(sentence, MAX, stdin) == NULL) {
            break;
        }
        if (strncasecmp(sentence, "0",1) == 0 && 
            (sentence[1] == '\n' || sentence[1] == '\0')) {
                printf("GoodBye\n");
            break;
        }
        processSentence(root, sentence);
    }
    freeTree(root);
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

void processSentence(Node* root, char *sentence) {
    char* token = strtok(sentence, " \t\n\r.,;:!?\"'()[]{}");
    while (token != NULL) {
        char processed_word[MAX];
        strncpy(processed_word, token, MAX);
        processed_word[MAX - 1] = '\0';
        preprocessWord(processed_word);
        
        if (strlen(processed_word) > 0) {
            checkSpelling(root, processed_word);
        }
        token = strtok(NULL," \t\n\r.,;:!?\"'()[]{}");
    }
}

void preprocessWord(char* word) 
{
    int i = 0, j = 0;
    while (word[i]) {
        if (isalnum((char)word[i])) {
            word[j++] = tolower((char)word[i]);
        }
        i++;
    }
    word[j] = '\0';
}

Node *loadDictionary(char *dict){
    char word[MAX];
    Node *root = NULL;
    FILE *fp = fopen(dict, "r");
    if (fp == NULL)
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

int tree_max(int a, int b)
{
    if(a > b)
        return a;
    return b;
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

Node *rightRotate(Node *y){
    Node *x = y->left;
    Node *T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = tree_max(height(y->left), height(y->right)) + 1;
    x->height = tree_max(height(x->left), height(x->right)) + 1;
    return x;
}

Node *leftRotate(Node *x){
    Node *y = x->right;
    Node *T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = tree_max(height(x->left), height(x->right)) + 1;
    y->height = tree_max(height(y->left), height(y->right)) + 1;    
    return y;
}

Node* insert(Node *node, char *data){
    if (node == NULL) return newNode(data);
    int compare_value = strcasecmp(data, node->data);
    
    if (compare_value < 0) node->left = insert(node->left, data);
    else if (compare_value > 0) node->right = insert(node->right, data);
    else return node;

    
    node->height = 1 + tree_max(height(node->left), height(node->right));
    
    int balance = getBalance(node);

    if (balance > 1 && strcasecmp(data, node->left->data) < 0) 
    return rightRotate(node);
    if (balance < -1 && strcasecmp(data, node->right->data) > 0) 
    return leftRotate(node);
    if (balance > 1 && strcasecmp(data, node->left->data) > 0)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && strcasecmp(data, node->right->data) < 0)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

Node *Search (Node *root, char *data){
    Node *current_node = root;
    Node *last_visited = NULL;
    while (current_node != NULL)
    {
        last_visited = current_node;
        int compare = strcasecmp(data, current_node->data);
        if (compare == 0) return current_node;
        else if (compare < 0) current_node = current_node->left;
        else current_node = current_node->right;
    }
    return last_visited;
}

void checkSpelling(Node* root, char* data) {
    Node* result = Search(root, data);
    
    if (result != NULL && strcasecmp(data, result->data) == 0) {
        printf("'%s': Correct\n", data);
    } else {
        printf("'%s': Incorrect. Suggestions:\n", data);
        if (result) {
            printf("A. %s\n", result->data);
            Node* predecessor = findInorderPredecessor(root, result->data);
            if (predecessor) {
                printf("B. %s\n", predecessor->data);
            } else {
                printf("B. (No predecessor available)\n");
            }
            Node* successor = findInorderSuccessor(root, result->data);
            if (successor) {
                printf("C. %s\n", successor->data);
            } else {
                printf("C. (No successor available)\n");
            }
        }
    }
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

Node *rightnode(Node* root) {
    while (root->right != NULL) {
        root = root->right;
    }
    return root;
}	
Node *leftnode(Node* root) {
    struct Node *current = root;
    while (current->left != NULL) {
        current = current->left;
    }
    return current;
}

Node* findInorderPredecessor(Node *root, char *data){
    if (root == NULL) 
    return NULL;
    Node *predecessor = NULL;
    Node *current = root;
    while (current != NULL)
    {
        if (strcasecmp(data, current->data) > 0)
        {
            predecessor = current;
            current = current->right;
        }
        else if (strcasecmp(data, current->data) < 0)
        {
            current = current->left;
        }
        else
        {
            if (current->left != NULL) return rightnode(current->left);
        }
        break;
    }
    return predecessor;
}

Node *findInorderSuccessor(Node *root, char *data){
    if (root == NULL) return NULL;
    
    if (strcasecmp(root->data, data) == 0 && root->right != NULL) return leftnode(root->right);
    
    Node *successor = NULL;
    Node *current = root;
    while (current != NULL)
    {
        if (strcasecmp(data, current->data) < 0)
        {
            successor = current;
            current = current->left;
        }
        else if (strcasecmp(data, current->data) >= 0) current = current->right;
    }
    return successor;
}