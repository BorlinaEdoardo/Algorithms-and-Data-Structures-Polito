#include <stdio.h>
#include <stdlib.h>
#include <string.h>



// ese 1
void f1(int *v1, int *v2, int d1, int d2, int ***M){
    if(d1 <= 0 || d2 <= 0) exit(0);

    int **sol = malloc(sizeof(int*)*d1);
    for (int i = 0; i < d1; ++i) {
        sol[i] = malloc(sizeof(int)*d2);
    }

    for (int i = 0; i < d1; ++i) {
        for (int j = 0; j < d2; ++j) {
            sol[i][j] = v1[i]*v2[j];
        }
    }

    *M = sol;
}

// ese 2
// -> vedesi LIST.c e LIST.h

// ese 3
typedef struct part_s{
    char *s;
    int pos;
    int costo;
}part;

int valid(part *val, int *sol, char *target, int n){
    int flag;
    char *str = malloc(100);
    str[0] = '\0';
    for (int i = 0; i < n; ++i) {
        strcat(str, val[sol[i]].s);
    }
    if(strcmp(target, str) == 0) flag = 1;
    else flag = 0;
    free(str);
    return flag;
}

void dispR(int *mark, int pos, int *sol, int *bs, int cost, int *bc, int n, int k, part *val, char *target, int charpos){
    // terminazione
    if(pos >= k) {
        if( valid(val, sol, target, k) && cost < *bc){
            *bc = cost;
            for (int i = 0; i < k; ++i) {
                bs[i] = sol[i];
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        if(!mark[i] && val[i].pos <= charpos){
            //pruning
            if(cost+val[i].costo < *bc){
                mark[i] = 1;
                sol[pos] = i;
                cost += val[i].costo;
                dispR(mark, pos+1, sol, bs, cost, bc, n, k, val, target, charpos+strlen(val[i].s));

                //backtracking
                mark[i] = 0;
                cost -= val[i].costo;

            }
        }
    }
}

void solve(char *target, part *p, int nParts){
    int *mark, *sol, *bestsol;
    int bestCost = INT_MAX;
    int i;
    sol = calloc(nParts,sizeof(int));
    bestsol = calloc(nParts,sizeof(int));
    for (i = nParts; i > 0; i--) {
        mark = calloc(i,sizeof(int));
        dispR(mark, 0, sol, bestsol, 0, &bestCost, nParts, i, p, target, 0);
        free(mark);
        if(bestCost < INT_MAX) //=> soluzione valida trovata
            break;
    }
    if(bestCost < INT_MAX) {
        printf("sono necesessarie %d partizioni, con un costo %d:\n", i, bestCost);
        for (int j = 0; j < i; j++){
            printf("p%d ", bestsol[j]);
        }
        printf("\n");
        for (int j = 0; j < i; j++){
            printf("%s ", p[bestsol[j]].s);
        }

    }

    free(sol);
    free(bestsol);
}

int main() {
// Definizione della stringa target
    char target[] = "persona";

    // Definizione e inizializzazione delle parti
    part P[] = {
            {"p", 0, 1},    // p0
            {"pers", 0, 5}, // p1
            {"er", 1, 4},   // p2
            {"ers", 1, 4},  // p3
            {"sa", 3, 1},   // p4
            {"so", 3, 2},   // p5
            {"ato", 0, 1},  // p6
            {"on", 4, 2},   // p7
            {"ona", 4, 3},  // p8
            {"a", 6, 1}     // p9
    };

    // Numero di parti
    int nParts = 9;

    // Chiamata alla funzione solve
    solve(target, P, nParts);
    fflush(stdout);
    return 0;
}
