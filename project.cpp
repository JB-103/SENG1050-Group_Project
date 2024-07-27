#include <stdio.h>
#include <string.h>
#include <malloc.h>
#pragma warning (disable: 4996)

#pragma region Constants
#define kMaxNames 5000
#define kMaxLength 21
#define kBuckets 127

#define kSuccess 0
#define kFileOpenError -1
#define kFileReadError -2
#define kFileCloseError -3
#pragma endregion

struct TreeNode {
    char* pDestination;
    int weight;
    float value;
    TreeNode* left;
    TreeNode* right;
}; 
struct HashTable {
    TreeNode* table[kBuckets];
};

#pragma region Prototypes
unsigned long calculateHash(const char*);
HashTable* createHashTable();
void collectDataFromFile();
TreeNode* createTreeNode(char*, int, float);
void insertInTree(TreeNode**);
TreeNode* searchInTree(TreeNode*, int);
void printTree(TreeNode*);
void freeMemory(TreeNode*, HashTable*);
#pragma endregion

int main(void) {
    return kSuccess;
}

unsigned long calculateHash(const char*) {
    return kSuccess;
}
HashTable* createHashTable() {
    return NULL;
}
void collectDataFromFile() {

}
TreeNode* createTreeNode(char*, int, float) {
    return NULL;
}
void insertInTree(TreeNode**) {

}
TreeNode* searchInTree(TreeNode*, int) {
    return NULL;
}
void printTree(TreeNode*) {

}
void freeMemory(TreeNode*, HashTable*) {

}