//
// Created by edoar on 20/12/2024.
//

#include "Astar.h"
#include <stdio.h> // for the NULL constant
#include <stdlib.h>
#include <string.h>



// Heuristic (Movement allowed in four directions only => Manhattan Distance)
static int abs(int num){
    return num < 0 ? -num : num;
}
static int heuristic(coord_t current, coord_t goal){
    return abs (current.x - goal.x) + abs (current.y - goal.y);
}

// stack implementation -> stack implemented with a dynamic vector
struct stack_s{
    coord_t *path;
    int maxDim;
    int dim;
    int isEmpty;
};

void STACKpush(STACK S, coord_t val){
    if (S->path == NULL) return;
    else if(S->dim>=S->maxDim){
        S-> maxDim = S->maxDim*2;
        S->path = realloc(S->path, S->maxDim * sizeof(coord_t));
        if(S->path == NULL) return; // realloc simple error management
    }

    if(STACKisEmpty(S)) S->isEmpty = 0;

    S->path[S->dim++] = val;
}

coord_t STACKpop(STACK S){
    coord_t retVal;

    if(S->isEmpty) retVal = toCoord(-1, -1);
    else if(S->dim == 0) {retVal = S->path[0]; S->isEmpty = 1;}
    else retVal = S->path[--S->dim];

    return retVal;
}

int STACKisEmpty(STACK S){
    return S->isEmpty;
}

STACK STACKinit(int initialMaxDim){
    STACK S = malloc(sizeof *S);
    S->dim = 0;
    S->isEmpty = 1;
    S->path = malloc(initialMaxDim*sizeof(coord_t));
    S->maxDim = initialMaxDim;

    return S;
}
void STACKfree(STACK S){
    free(S->path);
    free(S);
}

// Set

// Set structure
typedef struct set_s {
    setNode_t *root; // Root node of the BST
    int size;        // Number of elements in the set
} set_t;

// Helper function to create a new tree node
setNode_t* createNode(coord_t c, int f) {
    setNode_t* newNode = (setNode_t*)malloc(sizeof(setNode_t));
    if (!newNode) return NULL;
    newNode->c = c;
    newNode->f = f;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Comparison function for ordering nodes
// Returns:
//   -1 if (f1, c1) < (f2, c2)
//    0 if (f1, c1) == (f2, c2)
//    1 if (f1, c1) > (f2, c2)
int compareNodes(int f1, coord_t c1, int f2, coord_t c2) {
    if (f1 < f2) {
        return -1; // Smaller f comes first
    } else if (f1 > f2) {
        return 1; // Larger f comes last
    } else {
        // f1 == f2, compare coordinates lexicographically
        if (c1.x < c2.x || (c1.x == c2.x && c1.y < c2.y)) {
            return -1;
        } else if (c1.x > c2.x || (c1.x == c2.x && c1.y > c2.y)) {
            return 1;
        } else {
            return 0; // Identical nodes
        }
    }
}

// Initialize the set
set_t* SETinit() {
    set_t* set = (set_t*)malloc(sizeof(set_t));
    if (!set) return NULL;
    set->root = NULL;
    set->size = 0;
    return set;
}

// Helper function to insert a node into the BST
setNode_t* insertNode(setNode_t* root, coord_t c, int f, int* inserted) {
    if (!root) {
        *inserted = 1;
        return createNode(c, f); // Create a new node if the root is null
    }

    int comparison = compareNodes(f, c, root->f, root->c);
    if (comparison < 0) {
        root->left = insertNode(root->left, c, f, inserted);
    } else if (comparison > 0) {
        root->right = insertNode(root->right, c, f, inserted);
    } else {
        *inserted = 0; // Node already exists (same f and coordinates)
    }

    return root;
}

// Insert a coordinate into the set
int SETinsert(set_t* set, coord_t c, int f) {
    if (!set) return -1;

    int inserted = 0;
    set->root = insertNode(set->root, c, f, &inserted);
    if (inserted) {
        set->size++;
    }
    return inserted; // Return 1 if inserted, 0 if duplicate
}

// Helper function to search for a node in the BST based on coordinates and `f`
setNode_t* findNode(setNode_t* root, coord_t c, int f) {
    if (!root) return NULL;

    int comparison = compareNodes(f, c, root->f, root->c);
    if (comparison < 0) {
        return findNode(root->left, c, f);
    } else if (comparison > 0) {
        return findNode(root->right, c, f);
    } else {
        return root; // Node found
    }
}

// Check if the set contains a coordinate with a specific `f`
int SETcontains(set_t* set, coord_t c, int f) {
    if (!set) return 0;

    setNode_t* node = findNode(set->root, c, f);
    return (node != NULL) ? 1 : 0;
}

// Get the value (f) associated with a coordinate
int SETgetValue(set_t* set, coord_t c, int* value) {
    if (!set) return 0;

    setNode_t* node = findNode(set->root, c, *value);
    if (node) {
        *value = node->f;
        return 1; // Value found
    }

    return 0; // Value not found
}

// Helper function to find the node with the smallest `f` in a subtree
setNode_t* findMinF(setNode_t* root) {
    if (!root) return NULL;

    setNode_t* current = root;
    while (current->left) {
        current = current->left; // Keep going left to find the smallest `f`
    }
    return current;
}

// Helper function to remove a node from the BST
setNode_t* removeNode(setNode_t* root, coord_t c, int f, int* removed) {
    if (!root) return NULL;

    int comparison = compareNodes(f, c, root->f, root->c);
    if (comparison < 0) {
        root->left = removeNode(root->left, c, f, removed);
    } else if (comparison > 0) {
        root->right = removeNode(root->right, c, f, removed);
    } else {
        *removed = 1;

        // Node with only one child or no child
        if (!root->left) {
            setNode_t* temp = root->right;
            free(root);
            return temp;
        } else if (!root->right) {
            setNode_t* temp = root->left;
            free(root);
            return temp;
        }

        // Node with two children: Get the inorder successor
        setNode_t* temp = findMinF(root->right);
        root->c = temp->c;
        root->f = temp->f;
        root->right = removeNode(root->right, temp->c, temp->f, removed);
    }

    return root;
}

// Remove a coordinate from the set
int SETremove(set_t* set, coord_t c, int f) {
    if (!set) return 0;

    int removed = 0;
    set->root = removeNode(set->root, c, f, &removed);
    if (removed) {
        set->size--;
    }
    return removed; // Return 1 if removed, 0 if not found
}

// Check if the set is empty
int SETisEmpty(set_t* set) {
    return (set && set->size == 0) ? 1 : 0;
}

// Free the memory used by the BST
void freeTree(setNode_t* root) {
    if (!root) return;

    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

// Free the set
void SETfree(set_t* set) {
    if (set) {
        freeTree(set->root);
        free(set);
    }
}

// Return a pointer to the node with the smallest `f` value
setNode_t* SETbegin(set_t* set) {
    if (!set || !set->root) return NULL;

    return findMinF(set->root); // Find the node with the smallest `f`
}

///////////////////////////////////////////////////////////

coord_t toCoord(int x, int y){
    coord_t retVal;
    retVal.x = x;
    retVal.y = y;

    return retVal;
}

void tracePath(cell_t cellDetails[][COL], coord_t dest)
{
    int row = dest.y;
    int col = dest.x;

    STACK Path = STACKinit(10);

    while (!(cellDetails[row][col].parent.x == col
             && cellDetails[row][col].parent.y == row)) {
        STACKpush(Path, toCoord(row, col));
        int temp_col = cellDetails[row][col].parent.x;
        int temp_row = cellDetails[row][col].parent.y;
        row = temp_row;
        col = temp_col;
    }

    STACKpush(Path, toCoord(row, col));
    while (!STACKisEmpty(Path)) {
        coord_t p = STACKpop(Path);
        printf("-> (%d,%d) ", p.x, p.y);
    }
    STACKfree(Path);
}

void aStarSearch(int grid[][COL], coord_t src, coord_t dest)
{
    // If the source is out of range
    if (isValid(src.y, src.x) == 0) {
        printf("Source is invalid\n");
        return;
    }

    // If the destination is out of range
    if (isValid(dest.y, dest.x) == 0) {
        printf("Destination is invalid\n");
        return;
    }

    // Either the source or the destination is blocked
    if (isUnBlocked(grid, src.y, src.x) == 0
        || isUnBlocked(grid, dest.y, dest.x)
           == 0) {
        printf("Source or the destination is blocked\n");
        return;
    }

    // If the destination cell is the same as source cell
    if (isDestination(src.y, src.x, dest)
        == 1) {
        printf("We are already at the destination\n");
        return;
    }

    // Create a closed list and initialise it to false which
    // means that no cell has been included yet This closed
    // list is implemented as a boolean 2D array
    int closedList[ROW][COL];
    memset(closedList, 0, sizeof(closedList));

    // Declare a 2D array of structure to hold the details
    // of that cell
    cell_t cellDetails[ROW][COL];

    int i, j, k;
    int fNew, gNew, hNew;
    int neighborMat[4][2] = {
            {-1, 0}, // up
            {+1, 0}, // down
            {0, +1}, // right
            {0, -1}  // left
    };

    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
            cellDetails[i][j].f = INT_MAX;
            cellDetails[i][j].g = INT_MAX;
            cellDetails[i][j].h = INT_MAX;
            cellDetails[i][j].parent.x = -1;
            cellDetails[i][j].parent.y = -1;
        }
    }

    // Initialising the parameters of the starting node
    i = src.y, j = src.x;
    cellDetails[i][j].f = 0;
    cellDetails[i][j].g = 0;
    cellDetails[i][j].h = 0;
    cellDetails[i][j].parent.x = j;
    cellDetails[i][j].parent.y = i;

    /*
     Create an open list having information as-
     <f, <i, j>>
     where f = g + h,
     and i, j are the row and column index of that cell
     Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
     This open list is implemented as a set of pair.*/
    SET openList = SETinit();

    // Put the starting cell on the open list and set its
    // 'f' as 0
    SETinsert(openList, toCoord(j, i), 0);

    // We set this boolean value as false as initially
    // the destination is not reached.
    int foundDest = 0;
    setNode_t p;
    while (!SETisEmpty(openList) && !foundDest) {
        p = *SETbegin(openList);

        // Remove this vertex from the open list
        SETremove(openList, p.c, p.f);

        // Add this vertex to the closed list
        i = p.c.y;
        j = p.c.x;
        closedList[i][j] = 1;

        /*
         Generating all the 4 successor of this cell

         Cell-->Popped Cell (i, j)
         N -->  North       (i-1, j)
         S -->  South       (i+1, j)
         E -->  East        (i, j+1)
         W -->  West        (i, j-1) */


        for (k = 0; k < 4; k++) {
            if (isValid(i + neighborMat[k][0], j + neighborMat[k][1]) == 1){
                // If the destination cell is the same as the
                // current successor
                if (isDestination(i + neighborMat[k][0], j + neighborMat[k][1], dest) == 1) {
                    // Set the Parent of the destination cell
                    cellDetails[i + neighborMat[k][0]][j + neighborMat[k][1]].parent.y = i;
                    cellDetails[i + neighborMat[k][0]][j + neighborMat[k][1]].parent.x = j;
                    printf("The destination cell is found\n");
                    tracePath(cellDetails, dest);
                    foundDest = 1;

                    break;
                }
                    // If the successor is already on the closed
                    // list or if it is blocked, then ignore it.
                    // Else do the following
                else if (closedList[i + neighborMat[k][0]][j + neighborMat[k][1]] == 0
                         && isUnBlocked(grid, i + neighborMat[k][0],j + neighborMat[k][1])
                            == 1) {
                    gNew = cellDetails[i][j].g + 1;
                    hNew = heuristic(toCoord(j + neighborMat[k][1], i + neighborMat[k][0]), dest);
                    fNew = gNew + hNew;

                    // If it isn’t on the open list, add it to
                    // the open list. Make the current square
                    // the parent of this square. Record the
                    // f, g, and h costs of the square cell
                    //                OR
                    // If it is on the open list already, check
                    // to see if this path to that square is
                    // better, using 'f' cost as the measure.
                    if (cellDetails[i + neighborMat[k][0]][j + neighborMat[k][1]].f == INT_MAX
                        || cellDetails[i + neighborMat[k][0]][j + neighborMat[k][1]].f > fNew) {

                        SETinsert(openList, toCoord(j + neighborMat[k][1], i + neighborMat[k][0]), fNew);

                        // Update the details of this cell
                        cellDetails[i + neighborMat[k][0]][j + neighborMat[k][1]].f = fNew;
                        cellDetails[i + neighborMat[k][0]][j + neighborMat[k][1]].g = gNew;
                        cellDetails[i + neighborMat[k][0]][j + neighborMat[k][1]].h = hNew;
                        cellDetails[i + neighborMat[k][0]][j + neighborMat[k][1]].parent.y = i;
                        cellDetails[i + neighborMat[k][0]][j + neighborMat[k][1]].parent.x = j;
                    }
                }

            }
        }
    }

    // When the destination cell is not found and the open
    // list is empty, then we conclude that we failed to
    // reach the destination cell. This may happen when the
    // there is no way to destination cell (due to
    // blockages)
    if (foundDest == 0)
        printf("Failed to find the Destination Cell\n");
    SETfree(openList);
}


int isValid(int row, int col)
{
    // Returns true if row number and column number
    // is in range
    return (row >= 0) && (row < ROW) && (col >= 0)
           && (col < COL);
}

int isUnBlocked(int grid[][COL], int row, int col)
{
    int i, j;
    // Returns true if the 3x3 square is not blocked else false
    for (i = -1; i <= 1; i++) {
        for (j = -1; j <= 1; j++) {
            if (grid[row+i][col+j] == 1)
                return 0;
        }
    }
    return 1;
}

int isDestination(int row, int col, coord_t dest)
{
    if (row == dest.y && col == dest.x)
        return (1);
    else
        return (0);
}
