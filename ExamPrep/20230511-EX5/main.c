#include <stdio.h>
#include <stdlib.h>

// Funzione per allocare una matrice dinamicamente
int **allocate_matrix(int r, int c) {
    int **matrix = (int **)malloc(r * sizeof(int *));
    for (int i = 0; i < r; i++) {
        matrix[i] = (int *)malloc(c * sizeof(int));
    }
    return matrix;
}

// Funzione per stampare una matrice
void print_matrix(int **M, int r, int c) {
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            printf("%02d ", M[i][j]);
        }
        printf("\n");
    }
}


// ese 1
void f1(int **M, int r, int c, int ***sol){
    int **s = (int**) malloc(r*sizeof(int*));
    for (int i = 0; i < r; ++i) {
        s[i] = calloc(c, sizeof(int));
        for (int j = 0; j < c; ++j) {
            s[i][j] = M[i][j];
        }
    }

    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {

            // per riga
            for (int k = 0; k < j; ++k) {
                s[i][j] += M[i][k];
            }

            // per colonna
            for (int l = 0; l < i; ++l) {
                s[i][j] += M[l][j];
            }
        }
    }

    *sol = s;
}

// ese 3

int verify(int **M, int *sol, int k, int o, int s){
    int valid = 1;
    int r;
    // mark per gli oggetti
    int *mark = calloc(o, sizeof(int));


    // controllo sottoinsiemi disgiunti
    for (int i = 0; i < k; ++i) {
        r = sol[i];
        for (int j = 0; j < o; ++j) {
            if(M[r][j]) mark[j]++;
        }
    }

    for (int i = 0; i < o && valid; ++i) {
        if(mark[i] == 0 || mark[i] > 1) valid = 0;
    }

    return valid;
}

// si usa powerset (realizzato con combinazioni semplici)
void solveR(int pos, int start, int k, int n, int *mark, int *sol, int *bestSol, int *bm, int **M, int O, int S){
    if(pos >= k){
        if(verify(M, sol, k, O, S)){
            *bm = k;

            for (int i = 0; i < k; ++i) {
                bestSol[i] = sol[i];
            }
        }
        return;
    }

    for (int i = start; i < n; ++i) {
        if(!mark[i]){
            sol[pos] = i;
            mark[i] = 1;
            solveR(pos+1, i+1, k, n, mark, sol, bestSol, bm, M, O, S);
            mark[i] = 0;
        }
    }
}

void solve(int **M, int O, int S){
    int *sol = calloc(S, sizeof(int));
    int *bestsol = calloc(S, sizeof(int));
    int *mark = calloc(S, sizeof(int));
    int num = S+1;

    for(int i = 1; i < S && num == S+1; i++){
        solveR(0, 0, i, S, mark, sol, bestsol, &num, M, O, S);
    }
    if(num == S+1){
        printf("soluzione non trovata.\n");
    } else{
        printf("soluzione:\n");
        for (int i = 0; i < num; ++i) {
            printf("\t%d in %d\n", i, bestsol[i]);
        }

    }
    free(sol);
    free(bestsol);
    free(mark);
}

int main(){
    // Allocazione e lettura della matrice M
    int r = 3, c = 3;

    // Allocazione e assegnazione diretta della matrice M
    int **M = allocate_matrix(r, c);

    // Assegniamo valori fissi alla matrice M
    int values[3][3] = {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9}
    };
    // Copiamo i valori nella matrice M
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            M[i][j] = values[i][j];
        }
    }

    // Dichiarazione della matrice M' (cumulata)
    int **Mp = NULL;

    // Chiamata alla funzione f
    f1(M, r, c, &Mp);

    // Stampa delle matrici M e M'
    printf("Matrice originale M:\n");
    print_matrix(M, r, c);

    printf("\nMatrice cumulata M':\n");
    print_matrix(Mp, r, c);

    // Liberazione della memoria
    for (int i = 0; i < r; i++) {
        free(M[i]);
        free(Mp[i]);
    }
    free(M);
    free(Mp);

    return 0;
}