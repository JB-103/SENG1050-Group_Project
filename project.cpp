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
#include <math.h>
#pragma warning (disable: 4996)

#pragma region Constants
//Constants.
#define kMaxParcels 5000
#define kMaxDestLength 21
#define kMaxLineLength kMaxDestLength+16 //5 weight, 7 value, 2 Comma/Spaces.
#define kBuckets 127
#define kCountries 73
#define kMinWeight 100
#define kMaxWeight 50000
#define kMinValue 10.00
#define kMaxValue 2000.00
#define kFilename "couriers.txt"
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
#define kMemoryError -4
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
struct ListNode {
    TreeNode* tNode;
    ListNode* next;
};
struct ListHashTable {
    ListNode* table[kBuckets];
};

#pragma region Prototypes
unsigned long calculateHash(const char*);
HashTable* createHashTable();
int collectDataFromFile(HashTable*);
void linkListIntoBST(ListNode**, TreeNode**, int);
TreeNode* createTreeNode(char*, int, float);
void insertInTree(TreeNode**, TreeNode*);
TreeNode* searchInTree(TreeNode*, int);
int getCountry();
void printTree(TreeNode*);
void getTotal(TreeNode*, int*, float*);
void getMinMaxValue(TreeNode*, float*, float*);
void printMinMaxWeight(TreeNode*);
void freeMemory(TreeNode*, HashTable*);
#pragma endregion

int main(void) {
    char userInput[kMaxDestLength] = "";
    HashTable* hashTable = createHashTable();
    TreeNode* foundNode = NULL;
    bool active = true;
    int index = -1, totalWeight = -1;
    float totalValue = -1, minValue = -1, maxValue = -1;
    //Parse file.
    int result = collectDataFromFile(hashTable);
    if (result != kSuccess) return result;
    //Prompt user.
    while (active) {
        //Print menu.
        printf("\nMenu:\n");
        printf("1. Display all the parcels details for a country.\n");
        printf("2. Display higher/lower weights of country & parcel.\n");
        printf("3. Display total parcel load & valuation for country.\n");
        printf("4. Display cheapest & most expensive parcels.\n");
        printf("5. Display lightest & heaviest parcel.\n");
        printf("6. Exit.\n");
        printf("Enter Number: ");
        //Get input.
        fgets(userInput, kMaxDestLength, stdin);
        int menuInput = atoi(userInput);
        //Check input & call functions accordingly.
        switch (menuInput) {
        case kMenu1:
            printTree(hashTable->table[getCountry()]);
            break;
        case kMenu2:
            index = getCountry();
            printf("Enter weight: ");
            fgets(userInput, kMaxDestLength, stdin);
            foundNode = searchInTree(hashTable->table[index], atoi(userInput));
            printTree(foundNode);
            break;
        case kMenu3:
            totalWeight = 0;
            totalValue = 0;
            getTotal(hashTable->table[getCountry()], &totalWeight, &totalValue);
            printf("Total Weight: %i\n Total Valuation: %d\n", totalWeight, totalValue);
            break;
        case kMenu4:
            minValue = 0, maxValue = 0;
            getMinMaxValue(hashTable->table[getCountry()], &minValue, &maxValue);
            printf("Minimum Valuation: %d\n Maximum Valuation: %d\n", minValue, maxValue);
            break;
        case kMenu5:
            printMinMaxWeight(hashTable->table[getCountry()]);
            break;
        case kMenuExit:
            active = false;
            break;
        default:
            printf("Invalid menu input. Please enter 1-6.\n");
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
    //Allocate memory for & validate allocation for hashtable.
    HashTable* hashTable = (HashTable*)malloc(sizeof(HashTable));
    if (hashTable == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    //Initialize all buckets to NULL.
    for (int counter = 0; counter < kBuckets; counter++) hashTable->table[counter] = NULL;
    //Return created & initialized hashtable
    return hashTable;
}
/**
 * FUNCTION: collectDataFromFile
 * DESCRIPTION:
 * Parses file & inserts data into tree struct.
 * PARAMETERS:
 * HashTable* hashTable: Pointer to hash table.
 * RETURNS:
 * int: returns 0 for success or non zero otherwise.
 */
int collectDataFromFile(HashTable* hashTable) {
    //Allocate memory for, verify & initialize temporary linked list hashtable.
    ListHashTable* listHashTable = (ListHashTable*)malloc(sizeof(ListHashTable));
    if (listHashTable == NULL) {
        printf("Memory allocation failed\n");
        return kMemoryError;
    }
    for (int counter = 0; counter < kBuckets; counter++) listHashTable->table[counter] = NULL;
    //Variables.
    int counts[kBuckets]{ 0 }, weight = -1;
    float value = -1;
    char destination[kMaxDestLength]{}, tempLine[kMaxLineLength]{};
    //Open file for reading & handle open error.
    FILE* pFile = fopen(kFilename, "r");
    if (pFile == NULL) {
        printf("Error: Could not open file: %s\n", kFilename);
        return kFileOpenError;
    }
    //Iterate through file untill end of file or 5000 parcels.
    for (int counter = 1; counter < kMaxParcels && fgets(tempLine, kMaxLineLength, pFile) != NULL; counter++) {
        tempLine[strcspn(tempLine, "\n")] = '\0';
        //Handle empty lines.
        if (tempLine[0] == '\0') continue;
        //Parse line into variables & round value.
        if (sscanf(tempLine, "%[^,], %i, %f", destination, &weight, &value) != 3) {
            printf("Error parsing line: %s\n", tempLine);
            return kFileReadError;
        }
        value = (float)roundf(value * 100) / 100;
        //Allocate memory for nodes.
        ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
        //Handle allocation failure.
        if (newNode == NULL) {
            printf("Memory allocation failed\n");
            return kMemoryError;
        }
        //initialize node.
        newNode->next = NULL;
        newNode->tNode = createTreeNode(destination, weight, value);
        //Initialize head of linked list.
        ListNode** head = &listHashTable->table[calculateHash(destination)];
        //Keep track of amount of items in linked list.
        counts[calculateHash(destination)]++;
        //Handle empty list.
        if (*head == NULL) {
            *head = newNode;
            newNode->next = NULL;
            continue;
        }
        //Iterate list to find insertion point.
        ListNode* current = *head;
        ListNode* previous = NULL;
        while (current != NULL && current->tNode->weight < weight) {
            previous = current;
            current = current->next;
        }
        //If duplicate free memory for newNode & skip insertion.
        if (current != NULL && current->tNode->weight == weight) {
            free(newNode);
            counts[calculateHash(destination)]--;
            continue;
        }
        //Insert before head.
        if (previous == NULL) {
            newNode->next = *head;
            *head = newNode;
        }
        else {
            //Insert in middle or end.
            newNode->next = current;
            previous->next = newNode;
        }
    }
    //Handle read errors.
    if (ferror(pFile)) {
        printf("Error: Encountered an error while reading file: %s\n", kFilename);
        if (fclose(pFile) != 0) {
            printf("Error: Could not close file: %s\n", kFilename);
            return kFileCloseError;
        }
        return kFileReadError;
    }
    //Close file & handle closing error.
    if (fclose(pFile) != 0) {
        printf("Error: Could not close file: %s\n", kFilename);
        return kFileCloseError;
    }
    //Convert linked list into balanced tree structure.
    for (int i = 0; i < (sizeof(counts) / sizeof(int)); i++) {
        int n = counts[i];
        if (n == 0) continue;
        linkListIntoBST(&listHashTable->table[i], &hashTable->table[i], counts[i]);
    }
    //Free temp hashtable & linked lists.
    if (listHashTable != NULL) {
        for (int count = 0; count < kBuckets; count++) {
            ListNode* list = listHashTable->table[count];
            while (list != NULL) {
                ListNode* temp = list;
                list = list->next;
                free(temp);
            }
        }
        free(listHashTable);
    }
    //Return if successful.
    return kSuccess;
}
/**
 * FUNCTION: linkListIntoBST
 * DESCRIPTION:
 * Converts linked list into binary search tree.
 * PARAMETERS:
 * ListNode** head: Double pointer to head of list.
 * TreeNode** root: Double pointer to tree root.
 * int n: amount of items.
 * RETURNS:
 * Void: No return value.
 */
void linkListIntoBST(ListNode** head, TreeNode** root, int n) {
    //Handle empty list.
    if (*head == NULL || n == 0) return;
    //Calculate & find middle node.
    int middle = n / 2;
    ListNode* currentNode = *head;
    for (int count = 0; count < middle && currentNode->next != NULL; count++) currentNode = currentNode->next;
    //Set middle node as root & return if on last node.
    *root = currentNode->tNode;
    if (n == 1) return;
    // Create a new list head for the right subtree & disconnect middle of list.
    ListNode* rightHead = currentNode->next;
    currentNode->next = NULL;
    //Iterate through left & right subtrees untill linked list is empty.
    linkListIntoBST(head, &(*root)->left, middle);
    linkListIntoBST(&rightHead, &(*root)->right, (n - middle - 1));
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
 * TreeNode* item: Pointer to item to be inserted.
 * RETURNS:
 * void: No return value.
 */
void insertInTree(TreeNode** root, TreeNode* item) {
    //Handle empty root.
    if (*root == NULL) *root = item;
    //Check if you should insert the node to the left
    if (item->weight < (*root)->weight) {
        insertInTree(&(*root)->left, item);
    }
    else if (item->weight > (*root)->weight) {
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
    if (root->weight == weight) return root;
    //Check if less than or greater than.
    if (weight < root->weight) return searchInTree(root->left, weight);
    if (weight > root->weight) return searchInTree(root->right, weight);
}
/**
 * FUNCTION: getCountry
 * DESCRIPTION:
 * Prompts user to enter & gets hashTable index of country.
 * PARAMETERS:
 * None: Prompts user.
 * RETURNS:
 * int: hashTable index of country.
 */
int getCountry() {
    char userInput[kMaxDestLength] = "";
    //Prompt user & get input.
    do {
        printf("Enter country: ");
        fgets(userInput, kMaxDestLength, stdin);
        if (userInput[0] == '\n') printf("Invalid country.\n");
    } while (userInput[0] == '\n');
    userInput[strcspn(userInput, "\n")] = '\0';
    //Return hashvalue of country input.
    return calculateHash(userInput);
}
/**
 * FUNCTION: printTree
 * DESCRIPTION:
 * Prints full country tree structure & data.
 * PARAMETERS:
 * TreeNode* root: Pointer to root of tree structure.
 * RETURNS:
 * Void: no return value.
 */
void printTree(TreeNode* root) {
    //Handle empty tree.
    if (root == NULL) return;
    //Print weight & value for parcel.
    printf("%d, %.2f\t", root->weight, root->value);
    //Traverse left subtree.
    printTree(root->left);
    //Traverse right subtree.
    printTree(root->right);
}
/**
 * FUNCTION: getTotal
 * DESCRIPTION:
 * Calculates total weight & value of country tree structure.
 * PARAMETERS:
 * TreeNode* root: Pointer to root of tree structure.
 * int* totalWeight: Pointer to keep track of total weight.
 * float* totalValue: Pointer to keep track of total value.
 * RETURNS:
 * Void: no return value.
 */
void getTotal(TreeNode* root, int* totalWeight, float* totalValue) {
    //Handle empty tree.
    if (root == NULL) return;
    //Add weight & value.
    *totalWeight += root->weight;
    *totalValue += root->value;
    //Traverse left subtree.
    getTotal(root->left, totalWeight, totalValue);
    //Traverse right subtree.
    getTotal(root->right, totalWeight, totalValue);
}
/**
 * FUNCTION: getprintMinMaxValue
 * DESCRIPTION:
 * Get least & most expensive parcel in tree structure.
 * PARAMETERS:
 * TreeNode* root: Pointer to root of tree structure.
 * float* minValue: Pointer to keep track of min value.
 * float* maxValue: Pointer to keep track of max value.
 * RETURNS:
 * Void: no return value.
 */
void getMinMaxValue(TreeNode* root, float* minValue, float* maxValue) {
    //Handle empty tree.
    if (root == NULL) return;
    //Compare values & find min & max.
    if (root->value < *minValue) *minValue = root->value;
    if (root->value > *maxValue) *minValue = root->value;
    //Traverse left subtree.
    getMinMaxValue(root->left, minValue, maxValue);
    //Traverse right subtree.
    getMinMaxValue(root->right, minValue, maxValue);
}
/**
 * FUNCTION: printMinMaxWeight
 * DESCRIPTION:
 * Prints lightest & heaviest parcel in tree structure.
 * PARAMETERS:
 * TreeNode* root: Pointer to root of tree structure.
 * RETURNS:
 * Void: no return value.
 */
void printMinMaxWeight(TreeNode* root) {
    //Handle empty tree.
    if (root == NULL) return;
    //Find & print left most weight.
    TreeNode* tempNode = root;
    while (tempNode->left != NULL) tempNode = tempNode->left;
    printf("Minimum weight: %i\n", tempNode->weight);
    //Find & print right most weight.
    tempNode = root;
    while (tempNode->right != NULL) tempNode = tempNode->right;
    printf("Maximum weight: %i\n", tempNode->weight);
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
    if (root->left != NULL) freeMemory(root->left, hashTable);
    if (root->right != NULL) freeMemory(root->right, hashTable);
    if (root->left == NULL && root->right == NULL) free(root);
    //Free memory for the hash table.
    if (hashTable != NULL) {
        //Free memory for each tree in hash table.
        for (int count = 0; count < kBuckets; count++) freeMemory(hashTable->table[count], NULL);
        free(hashTable);
    }
}