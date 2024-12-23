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
    setCell_t* table; // Dynamic array for the hash table
    int size;         // Number of elements in the set
    int capacity;     // Hash table size
} set_t;

// Hash function for a 2D coordinate
unsigned int hash(coord_t c, int capacity) {
    return (abs(c.x * 49 + c.y) % capacity);
}

// Initialize the set with a given capacity
set_t* SETinit(int capacity) {
    set_t* set = (set_t*)malloc(sizeof(set_t));
    if (set == NULL) return NULL;

    set->table = (setCell_t*)calloc(capacity, sizeof(setCell_t));
    if (set->table == NULL) {
        free(set);
        return NULL;
    }

    set->size = 0;
    set->capacity = capacity;

    return set;
}

// Insert a coordinate into the set
int SETinsert(SET set, coord_t c, int f) {
    unsigned int index = hash(c, set->capacity);
    int i;
    for (i = 0; i < set->capacity; i++) {
        unsigned int probe = (index + i) % set->capacity;

        if (set->table[probe].flag == 0) { // Empty cell
            set->table[probe].c = c;
            set->table[probe].f = f;  // Store the provided value
            set->table[probe].flag = 1;  // Mark cell as occupied
            set->size++;
            return 1; // Successful insertion
        } else if (set->table[probe].flag == 1 &&
                   set->table[probe].c.x == c.x &&
                   set->table[probe].c.y == c.y) {
            return 0; // Element already exists
        }
    }

    return -1; // Insertion failed: table is full
}

// Check if the set contains a coordinate
int SETcontains(SET set, coord_t c) {
    unsigned int index = hash(c, set->capacity);
    int i;
    for (i = 0; i < set->capacity; i++) {
        unsigned int probe = (index + i) % set->capacity;

        if (set->table[probe].flag == 0) {
            return 0; // Element not found
        } else if (set->table[probe].flag == 1 &&
                   set->table[probe].c.x == c.x &&
                   set->table[probe].c.y == c.y) {
            return 1; // Element found
        }
    }

    return 0; // Element not found
}

// Get the value (f) associated with a coordinate
int SETgetValue(SET set, coord_t c, int* value) {
    unsigned int index = hash(c, set->capacity);
    int i;
    for (i = 0; i < set->capacity; i++) {
        unsigned int probe = (index + i) % set->capacity;

        if (set->table[probe].flag == 0) {
            return 0; // Element not found
        } else if (set->table[probe].flag == 1 &&
                   set->table[probe].c.x == c.x &&
                   set->table[probe].c.y == c.y) {
            *value = set->table[probe].f; // Retrieve the value
            return 1; // Value found
        }
    }

    return 0; // Element not found
}

// Remove a coordinate from the set
int SETremove(set_t* set, coord_t c) {
    unsigned int index = hash(c, set->capacity);
    int i;
    for (i = 0; i < set->capacity; i++) {
        unsigned int probe = (index + i) % set->capacity;

        if (set->table[probe].flag == 0) {
            return 0; // Element not found
        } else if (set->table[probe].flag == 1 &&
                   set->table[probe].c.x == c.x &&
                   set->table[probe].c.y == c.y) {
            set->table[probe].flag = -1; // Mark as "removed"
            set->size--;
            return 1; // Element removed
        }
    }

    return 0; // Element not found
}

// Check if the set is empty
int SETisEmpty(SET set) {
    if (set == NULL) {
        return 1; // A non-initialized set is considered empty
    }
    return (set->size == 0) ? 1 : 0;
}

// Destroy the set and free memory
void SETfree(SET set) {
    if (set) {
        free(set->table);
        free(set);
    }
}

// Return a pointer to the first valid (occupied) element in the set
setCell_t* SETbegin(set_t* set) {
    if (set == NULL || set->size == 0) return NULL; // Return NULL if the set is empty
    int i;
    for (i = 0; i < set->capacity; i++) {
        if (set->table[i].flag == 1) { // Check if the cell is occupied
            return &(set->table[i]);  // Return a pointer to the first valid element
        }
    }

    return NULL; // No valid elements found
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
    int row = dest.x;
    int col = dest.y;

    STACK Path = STACKinit(10);

    while (!(cellDetails[row][col].parent.x == row
             && cellDetails[row][col].parent.y == col)) {
        STACKpush(Path, toCoord(row, col));
        int temp_row = cellDetails[row][col].parent.x;
        int temp_col = cellDetails[row][col].parent.y;
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
    if (isValid(src.x, src.y) == 0) {
        printf("Source is invalid\n");
        return;
    }

    // If the destination is out of range
    if (isValid(dest.x, dest.y) == 0) {
        printf("Destination is invalid\n");
        return;
    }

    // Either the source or the destination is blocked
    if (isUnBlocked(grid, src.x, src.y) == 0
        || isUnBlocked(grid, dest.x, dest.y)
           == 0) {
        printf("Source or the destination is blocked\n");
        return;
    }

    // If the destination cell is the same as source cell
    if (isDestination(src.x, src.y, dest)
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

    int i, j;
    int fNew, gNew, hNew;

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
    cellDetails[i][j].parent.x = i;
    cellDetails[i][j].parent.y = j;

    /*
     Create an open list having information as-
     <f, <i, j>>
     where f = g + h,
     and i, j are the row and column index of that cell
     Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
     This open list is implemented as a set of pair.*/
    SET openList = SETinit(SETSIZE);

    // Put the starting cell on the open list and set its
    // 'f' as 0
    SETinsert(openList, toCoord(j, i), 0);

    // We set this boolean value as false as initially
    // the destination is not reached.
    int foundDest = 0;

    while (!SETisEmpty(openList)) {
        setCell_t p = *SETbegin(openList);

        // Remove this vertex from the open list
        SETremove(openList, p.c);

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
         W -->  West           (i, j-1) */

        int neighborMat[4][2] = {
                {-1, 0},
                {+1, 0},
                {0, +1},
                {0, -1}
        };
        int k;
        for (k = 0; k < 4; k++) {
            if (isValid(i - 1, j) == 1){
                // If the destination cell is the same as the
                // current successor
                if (isDestination(i - 1, j, dest) == 1) {
                    // Set the Parent of the destination cell
                    cellDetails[i + neighborMat[k][0]][j + neighborMat[k][1]].parent.y = i;
                    cellDetails[i + neighborMat[k][0]][j + neighborMat[k][1]].parent.x = j;
                    printf("The destination cell is found\n");
                    tracePath(cellDetails, dest);
                    foundDest = 1;
                    return;
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

}







