#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ese 1
void dfsR(int **M, int n, int v, int k, int depth, int *mark, int **sol, int start){
    // term
    if(depth == k){
        for (int i = 0; i < n; ++i) {
            if(mark[i]) sol[v][i] = i;
        }
        return;
    }

    for (int i = start; i < n; ++i) {
        if(M[v][i] && !mark[i]){
            mark[i] = 1;
            dfsR(M, n, v, k, depth+1,mark,sol,i+1);
            mark[i] = 0;
        }
    }

}

void dfs(int **M, int n, int k){
    int **sol = malloc(n*sizeof(int));
    for (int i = 0; i < n; ++i) {
        sol[i] = calloc(n, sizeof(int));
    }
    int *mark;

    for (int i = 0; i < n; ++i) {
        mark = calloc(n, sizeof(int));
        dfsR(M, n, i, k, 0, mark, sol, 0);
        free(mark);
    }

    printf("Coppie valide:\n");
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if(sol[i][j] > 0)
                printf("(%d, %d), ", i, sol[i][j]);
        }
    }



    for (int i = 0; i < n; ++i) {
        free(sol[i]);
    }
    free(sol);
}

// ese 2
// si vedano LIST.h e LIST.c

// ese 3
typedef struct sub_s{
    char *s;
    int pos;
    int costo;
}sub;

int verify(char *start,char *end, sub *sol, int n){
    int valid = 0;
    char *s = malloc(100);
    strcpy(s, start);  // Copia la stringa di partenza

    for (int i = 0; i < n; ++i) {
        int pos = sol[i].pos;
        int len = (int)strlen(sol[i].s);
        strncpy(&s[pos], sol[i].s, len);  // Sostituisce parte della stringa
    }

    if(strcasecmp(end, s) == 0)
        valid = 1;
    free(s);
    return valid;
}

void combR(int n, int k, int pos, int start, sub *val, sub *s, sub *bs, int *bestDim, int cost, int *bestCost, int *mark, int charpos, char *startS, char *endS){

    if(pos >= k){ // && verify
        if(cost < *bestCost && verify(startS, endS, s, k)){ // non è necessario fare il controllo sulla dimensione per come è strutturato il "powerset"
            *bestCost = cost;
            *bestDim = k;

            for (int i = 0; i < k; ++i) {
                bs[i] = s[i];
            }
        }
        return;
    }

    for (int i = start; i < n; ++i) {

        if(!mark[i] && val[i].pos == charpos){
            if(cost+val[i].costo <= *bestCost){ // pruning
                mark[i] = 1;
                s[pos] = val[i];
                combR(n, k, pos+1, i+1, val, s, bs, bestDim,
                      cost+val[i].costo, bestCost, mark,
                      charpos+(int)strlen(val[i].s), startS, endS);
                // backtracking
                mark[i] = 0;
            }
        }
    }
    // proviamo a lascera la posizione vuota
    if(charpos < strlen(endS))
        combR(n, k, pos, start, val, s, bs, bestDim,
          cost, bestCost, mark,
          charpos+1, startS, endS);
}


void solve(char* start, char* end, sub *S, int nSubs){
    // utilizziamo powerset modificato, implementato mediante combinazioni semplici
    // al fine di avere le soluzioni in ordine di dimensione decrescente
    // nota: affinché ciò sia possibile è necessario ordinare il vettore per posizione

    //sortByPos(s);

    sub *bs = calloc(nSubs, sizeof *bs);
    sub *sol= calloc(nSubs, sizeof *sol);
    int bestSolDim = 0;
    int bestSolCost = INT_MAX;
    int *mark;

    for (int i = nSubs; i > 0; i--) {
        mark =  calloc(nSubs, sizeof(int));
        //combinazioni semplici
        combR(nSubs, i, 0, 0, S, sol, bs, &bestSolDim, 0, &bestSolCost, mark, 0, start, end);
        free(mark);
    }

    printf("Il costo della sostituzione e': %d\n"
           "la soluzione e':\n\t", bestSolCost);
    for (int i = 0; i < bestSolDim; ++i) {
        printf("sol[%d] = %s\n\t", i, bs[i].s);
    }
    fflush(stdout);


    free(bs);
    free(sol);
}

int main() {
    // Definizione della stringa di partenza e della stringa target (di arrivo)
    char start[] = "passato";
    char end[] = "persona";

    // Definizione del vettore di sostituzioni "S"
    sub S[] = {
            {"er", 1, 4},
            {"ers", 1, 5},
            {"sa", 3, 1},
            {"so", 3, 2},
            {"ato", 0, 1},
            {"on", 4, 2},
            {"ona", 4, 3},
            {"a", 6, 1}
    };

    // Numero di sostituzioni disponibili
    int nSubs = 8;

    // Chiamata alla funzione "solve"
    solve(start, end, S, nSubs);

    return 0;
}

