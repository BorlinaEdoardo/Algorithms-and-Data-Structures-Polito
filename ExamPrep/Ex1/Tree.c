//
// Created by edoar on 25/08/2024.
//

#include "Tree.h"
#include <stdlib.h>

typedef struct node_s node_t, *link;
struct node_s{
    int val;
    link *children;
};

struct TREE_s{
    link h;
    link z;
    int childnum;
};

void maxlR(link h, int n, int *max, int count){
    if(h == NULL){
        if(count > *max)
            *max = count;
        return;
    }
    for (int i = 0; i < n; ++i) {
        if(h->val >= 0)
            maxlR(h->children[i], n, max, count+1);
    }
}

// wrapper
int TREEMaxLength(TREE T){
    int max = 0;
    if(T == NULL) return 0;
    maxlR(T->h, T->childnum, &max, 0);
    return max;
}
TREE TREEInit(int childNum){
    TREE T = malloc(sizeof *T);
    T->h = T->z = NULL;
    T->childnum = childNum;
    return T;
}

void freeR(link h, int n){
    if(h == NULL) return;
    for (int i = 0; i < n; ++i) {
        freeR(h->children[i], n);
    }
    free(h);
}

void TREEFree(TREE T){
    // terminazione
    if(T->h == T->z)
        return;
    freeR(T->h, T->childnum);

}


