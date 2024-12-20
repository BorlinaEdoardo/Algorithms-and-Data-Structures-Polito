//
// Created by edoar on 12/09/2024.
//
#include <stdio.h>
#include "LIST.h"

typedef struct node_s *link;
struct node_s{
    int val;
    link next;
};

struct list_s{
    link h;
    int n;
};

void swap(LIST L, int k){
    if(L->h == NULL) return;
    int n, tmp;
    if(k >= L->n) return;
    else if(k >= L->n){
        n = k;
        k = (L->n-1)-n;
    } else{
        n = (L->n-1)-k;
    }

    link p = L->h, v1, v2;
    for (int i = 0; i<=n; i++, p=p->next) {
        if(i == k){
            v1 = p;
        } else if (i == n){
            v2 = p;
        }

        tmp = v2->val;
        v2->val = v1->val;
        v1->val = tmp;

    }
}