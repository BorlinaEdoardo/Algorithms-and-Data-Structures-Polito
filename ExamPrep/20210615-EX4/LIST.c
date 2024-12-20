//
// Created by edoar on 09/09/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LIST.h"

typedef struct node *link, node_t;
struct list{
    link h;
    int n;
};

struct node{
    char *val;
    link next;
};

link newNode(char* val, link next){
     link l;
     l = malloc(sizeof *l);
     l->val = strdup(val);
     l->next = next;

     return l;
}

// tail insert
link insert(LIST l, char *val){
    if(l->n == 0) { // head insert
        l->n++;
        return newNode(val, NULL);
    }
    link p;
    for (p = l->h; p->next != NULL; p = p->next);
    p->next = newNode(val, NULL);
    l->n++;
    return l->h;
}

void split(LIST l, char *str, char sep){
    int pos = 0;
    if(l == NULL) {
        l = malloc(sizeof *l);
        l->n = 0;
    }
    char s[100];
    s[0] = '\0'; // qualora il primo carattere di str fosse il separatore

    for (int i = 0; i < strlen(str); ++i) {
        if(str[i] == sep) {
            s[pos] = '\0';
            l->h = insert(l, s);
            pos = 0;
        }
        s[pos++] = str[i];
    }
}








