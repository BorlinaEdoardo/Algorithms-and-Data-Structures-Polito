#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define V 5

void print2d(int **M, int r, int c){
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            printf("%2d ", M[i][j]);
        }
        printf("\n");
    }
}

void randM(int **M, int rows, int cols) {
    // Imposta il seme per la generazione di numeri casuali
    srand(time(NULL));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            M[i][j] = rand() % 10;  // Genera numeri tra 0 e 9
        }
    }
}

void malloc2d(int ***M, int r, int c){
    int **retval;
    retval = malloc(r*sizeof(int *));
    for (int i = 0; i < r; ++i) {
        retval[i] = calloc(c, sizeof(int));
    }

    *M = retval;
}

void free2d(int **M, int r){
    for (int i = 0; i < r; ++i) {
        free(M[i]);
    }
    free(M);
}

// ex 1
void f1(int **M, int r, int c, int **sol, int *solr, int *solc){
    int flag,
        sr = 0,
        sc = 0;
    int rows[r], cols[c];

    for (int i = 0; i < r; ++i) {
        rows[i] = 0;
    }

    for (int i = 0; i < c; ++i) {
        cols[i] = 0;
    }

    // searching for cols/rows containing 0s and marking them
    for (int i = 0; i < r; ++i) {
        flag = 1;
        for (int j = 0; j < c ; ++j) {
            if(M[i][j] == 0){
                flag = 0;
                rows[i] = 1;
                cols[j] = 1;
            }
        }

    }

    int pos = 0;
    // new matrix construction
    for (int i = 0; i < r; ++i) {
        flag = 0;
        sc = 0;
        for (int j = 0; j < c; ++j) {
            if(!cols[j] && !rows[i]) {
                sol[sr][sc] = M[i][j];
                sc++;
                flag = 1;
            }
        }
        if(flag) sr++;
        if(sc > *solc) *solc = sc;
    }
    *solr = sr;
}


int main() {
    int **M, **es1;
    int nr, nc;
    malloc2d(&M, V,V);
    malloc2d(&es1, V, V);
    randM(M,V, V);
    print2d(M, V, V);

    printf("Rimozione righe e colonne con valori nulli:\n");
    fflush(stdout);

    f1(M, V, V, es1, &nr, &nc);
    print2d(es1, nr, nc);

    fflush(stdout);

    return 0;
}
