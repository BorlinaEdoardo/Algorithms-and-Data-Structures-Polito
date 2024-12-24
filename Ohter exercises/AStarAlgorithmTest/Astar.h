//
// Created by edoar on 20/12/2024.
//

#ifndef ASTARALGORITHM_ASTAR_H
#define ASTARALGORITHM_ASTAR_H

#define COL 48
#define ROW 56

#define MR 56		// map number of rows
#define MC 48		// map number of columns

#define SETSIZE 101

typedef struct coord_s{
    int x;
    int y;
}coord_t;

typedef struct cell_s{
    coord_t parent;
    // f = g + h
    int f;
    int g;
    int h;
}cell_t;


// Set implemented with a first class ADT hash table (with linear probing for collision management)

typedef struct setNode_s setNode_t;


struct setNode_s {
    coord_t c; // Coordinate stored in the cell
    int f;     // f = g + h
    setNode_t *left;   // Left child
    setNode_t *right;  // Right child
};

typedef struct set_s *SET;

SET SETinit();
void SETfree(SET set);

int SETisEmpty(SET set);
int SETcontains(SET set, coord_t c, int f);

int SETgetValue(SET set, coord_t c, int* value);
int SETinsert(SET set, coord_t c, int f);
int SETremove(SET set, coord_t c, int f);

setNode_t *SETbegin(SET set);

// STACK implementation (through simple dynamic vector) as I class ADT
typedef struct stack_s *STACK;
void STACKpush(STACK S, coord_t val);
coord_t STACKpop(STACK S);
int STACKisEmpty(STACK S);

STACK STACKinit(int initialMaxDim);
void STACKfree(STACK S);

// Heuristic (Movement allowed in four directions only => Manhattan Distance)
static int abs(int num);
static int heuristic(coord_t current, coord_t goal);

// A Utility Function to check whether given cell (row, col)
// is a valid cell or not.
int isValid(int row, int col);

// A Utility Function to check whether the given cell is
// blocked or not
int isUnBlocked(int grid[][COL], int row, int col);

// A Utility Function to check whether destination cell has
// been reached or not
int isDestination(int row, int col, coord_t dest);

// A Utility Function to trace the path from the source
// to destination
void tracePath(cell_t cellDetails[][COL], coord_t dest);
coord_t toCoord(int x, int y);

void aStarSearch(int grid[][COL], coord_t src, coord_t dest);

#endif //ASTARALGORITHM_ASTAR_H
