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
#include <stdlib.h>
#pragma warning (disable: 4996)

#pragma region Constants
//Constants.
#define kMaxNames 5000
#define kMaxLength 21
#define kBuckets 127
#define kMinWeight 100
#define kMaxWeight 50000
#define kMinValue 10
#define kMaxValue 2000
//Menu Items.
#define kMenu1 1
#define kMenu2 2
#define kMenu3 3
#define kMenu4 4
#define kMenu5 5
#define kMenuExit 6
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
    char* userInput{};
    int menuInput = -1;


    while (true) {
        printf("Menu:\n");
        printf("1. Display all the parcels details for a country.\n");
        printf("2. Display higher/lower weights of country & parcel.\n");
        printf("3. Display total parcel load & valuation for country.\n");
        printf("4. Display cheapest & most expensive parcels.\n");
        printf("5. Display lightest & heaviest parcel.\n");
        printf("6. Exit.\n");

        fgets(userInput, kMaxLength, stdin);
        menuInput = atoi(userInput);

        switch (menuInput) {
        case kMenu1:

            break;
        case kMenu2:

            break;
        case kMenu3:

            break;
        case kMenu4:

            break;
        case kMenu5:

            break;
        case kMenuExit:

            break;
        default:
            printf("Invalid menu input.\n");
            break;
        }
    }


    return kSuccess;
}
/**
 * FUNCTION: calculateHash
 * DESCRIPTION:
 * Calculates hash value for specified string using the djb2 algorithm.
 * PARAMETERS:
 * const char* str: Pointer to string.
 * RETURNS:
 * unsigned long: Hash value for the string.
 * CREDIT: Daniel J. Bernstein (djb2 hash function).
 */
unsigned long calculateHash(const char* str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++) != '\0') {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash % kBuckets;
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
    HashTable* hashTable = (HashTable*)malloc(sizeof(HashTable));
    if (hashTable == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }

    for (int counter = 0; counter < kBuckets; counter++) {
        hashTable->table[counter] = NULL;
    }

    return hashTable;
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
    //Allocate memory for new node to insert.
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    //Handle allocation failure
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    //Copy data to newly allocated node.
    newNode->pDestination = (char*)malloc(strlen(pDestination) + 1);
    strcpy(newNode->pDestination, pDestination);
    newNode->weight = weight;
    newNode->value = value;
    newNode->left = NULL;
    newNode->right = NULL;
    //Return node.
    return newNode;
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
    //Handle empty root.
    if (*root == NULL) {
        *root = item;
    }
    //Check if you should insert the node to the left
    if (item->weight < (*root)->weight) {
        insertInTree(&(*root)->left, item);
    } else if (item->weight > (*root)->weight) {
        insertInTree(&(*root)->right, item);
    }
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
    //Handle empty tree.
    if (root == NULL) {
        printf("Tree is Empty. Cannot Perform Search!");
        return NULL;
    }
    //Check if equal.
    if (root->weight == weight) {
        return root;
    }
    //Check if less than or greater than.
    if (weight < root->weight) {
        return searchInTree(root->left, weight);
    } else if (weight > root->weight) {
        return searchInTree(root->right, weight);
    }
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
void freeMemory(TreeNode* root, HashTable* hashTable) {
    //Free memory for tree node structure.
    if (root->left != NULL) {
        freeMemory(root->left, hashTable);
    } else if (root->right != NULL) {
        freeMemory(root->right, hashTable);
    } else if (root->left == NULL && root->right == NULL) {
        free(root);
    }
    //Free memory for the hash table.
    if (hashTable != NULL) {
        //Free memory for each linked list in hash table.
        for (int count = 0; count < kBuckets; count++) {
            freeMemory(hashTable->table[count], NULL);
        }
        free(hashTable);
    }
}