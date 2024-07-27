/*
* FILE : project.cpp
* PROJECT : SENG1050 - Project
* PROGRAMMER : Justin Blake & Josh Visentin.
* FIRST VERSION : 2024-07-26
* DESCRIPTION:
* The functions in this file are used to utilize tree data structures to 
* manage various products & display output based on specific requirements.
*/
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#pragma warning (disable: 4996)

#pragma region Constants
//Constants.
#define kMaxNames 5000
#define kMaxLength 21
#define kBuckets 127
//Status Codes.
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
void insertInTree(TreeNode**, TreeNode*);
TreeNode* searchInTree(TreeNode*, int);
void printTree(TreeNode*);
void freeMemory(TreeNode*, HashTable*);
#pragma endregion

int main(void) {
    return kSuccess;
}

unsigned long calculateHash(const char* string) {
    return kSuccess;
}
/**
 * FUNCTION: createHashTable
 * DESCRIPTION:
 * Initializes empty hash table.
 * PARAMETERS:
 * None.
 * RETURNS:
 * Node*: Pointer to newly created hash table (array of pointers).
 */
HashTable* createHashTable() {
    return NULL;
}
/**
 * FUNCTION: collectDataFromFile
 * DESCRIPTION:
 * Parses file & puts data into tree struct.
 * PARAMETERS:
 * None.
 * RETURNS:
 * void: No return value.
 */
void collectDataFromFile() {

}
/**
 * FUNCTION: createTreeNode
 * DESCRIPTION:
 * Allocates memory for & initializes new tree node.
 * PARAMETERS:
 * char* pDestination: Pointer to destination of item.
 * int weight: Weight of item.
 * float value: Value of item.
 * RETURNS:
 * TreeNode*: Pointer to newly created node.
 */
TreeNode* createTreeNode(char* pDestination, int weight, float value) {
    return NULL;
}
/**
 * FUNCTION: insertInTree
 * DESCRIPTION:
 * Inserts item into tree structure.
 * PARAMETERS:
 * TreeNode** root: Double pointer to root node of tree.
 * TreeNode*: Pointer to item to be inserted.
 * RETURNS:
 * void: No return value.
 */
void insertInTree(TreeNode** root, TreeNode* item) {

}
/**
 * FUNCTION: searchInTree
 * DESCRIPTION:
 * Searches tree structure for specified weight.
 * PARAMETERS:
 * TreeNode* root: Pointer to root node of tree.
 * int weight: Weight to search for.
 * RETURNS:
 * TreeNode*: Returns found node if found, else NULL.
 */
TreeNode* searchInTree(TreeNode* root, int weight) {
    return NULL;
}
/**
 * FUNCTION: printTree
 * DESCRIPTION:
 * Prints tree structure to console.
 * PARAMETERS:
 * TreeNode* root: Pointer to root of tree to print.
 * RETURNS:
 * Void: no return value.
 */
void printTree(TreeNode* root) {

}
/**
 * FUNCTION: freeMemory
 * DESCRIPTION:
 * Frees memory allocated for hash table & tree structure.
 * PARAMETERS:
 * TreeNode* root: Pointer to root of tree to be freed.
 * HashTable* hashTable: Pointer to hash table to be freed.
 * RETURNS:
 * void: No return value.
 */
void freeMemory(TreeNode* root, HashTable* table) {

}