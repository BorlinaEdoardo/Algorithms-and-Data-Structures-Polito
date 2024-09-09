//
// Created by edoar on 09/09/2024.
//

#include "list.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct node *link, node_t;

struct list{
    link h;
    link t;
    int n;
};
struct node{
    int val;
    link prev;
    link next;
};

void purge(LIST l, int div){
    if(l->h==NULL) return;
    // q indica il nodo corrente, p il precedente
    link p, q, temp;

    for (q = l->h, p = NULL; q != NULL;p = q, q = q->next ) {
        if (q->val%div != 0) {
            if(q == l->h) l->h = q;
            if(p != NULL) p->next = q->next;
            if(q == l->t) l->t = q;
            if(q->next != NULL) q->next->prev = p;
            free(q);
            q = p;
        }
    }
}




