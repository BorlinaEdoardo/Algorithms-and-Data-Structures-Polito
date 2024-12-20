//
// Created by edoar on 20/12/2024.
//

#include "Astar.h"
#include <stdio.h> // for the NULL constant
#include <stdlib.h>

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



    return;
}