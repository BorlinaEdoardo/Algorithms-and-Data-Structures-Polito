//
// Created by edoar on 20/12/2024.
//

#ifndef ASTARALGORITHM_ASTAR_H
#define ASTARALGORITHM_ASTAR_H

#define COL 10
#define ROW 20

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
}cell_t, *CELL;

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
int isValid(int row, int col)
{
    // Returns true if row number and column number
    // is in range
    return (row >= 0) && (row < ROW) && (col >= 0)
           && (col < COL);
}

// A Utility Function to check whether the given cell is
// blocked or not
int isUnBlocked(int grid[][COL], int row, int col)
{
    // Returns true if the cell is not blocked else false
    if (grid[row][col] != 1)
        return 1;
    else
        return 0;
}

// A Utility Function to check whether destination cell has
// been reached or not
int isDestination(int row, int col, coord_t dest)
{
    if (row == dest.x && col == dest.y)
        return (1);
    else
        return (0);
}

// A Utility Function to trace the path from the source
// to destination
void tracePath(cell_t cellDetails[][COL], coord_t dest);
coord_t toCoord(int x, int y);

#endif //ASTARALGORITHM_ASTAR_H
