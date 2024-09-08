#include <stdio.h>
#include <stdlib.h>

#define FILENAME "table.txt"
#define VALSIZE 3
#define V 5

int sos = 0;

/*
 * Traccia da 12 del 07/02/2023
 */

int VAL[] = {-1, 0, 1};

void free2d(void **M, int r){
    for (int i = 0; i < r; ++i) {
        free(M[i]);
    }
    free(M);
}

// ese 1
void f(int **M, int r, int c, float ***Mp);
void avgR(int **M, int i, int j, int r, int c, float *avg, int *num, int pos);
void print2dInt(int **M, int r, int c){
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            printf("%d ", M[i][j]);
        }
        printf("\n");
    }
}


// ese 3
void connections2(int n, int M[][V]);
int verify(int n, int m, int *sol, int M[][V]);
void verifyR(int v, int n, int M[][V], int depth, int *visited, int *sol, int g);
void erPartR(int *bs, int *sol, int n, int pos, int m, int M[][V], int *min);

/*void connR(int g[V][V], int v, int id, int *sol, int *bs, int *min, int depth, int *num) {
    // Debug print
    printf("\n%d, %d\n", id, v);

    // Terminazione se tutti i nodi sono stati esplorati
    if (v >= V) {
        if (*num < *min) {
            *min = *num;
            for (int i = 0; i < V; ++i) {
                bs[i] = sol[i];
            }
        }
        return;
    }

    // Terminazione se la profondità è eccessiva o id supera min
    if (depth > 2) return;

    // Esplora i nodi rimanenti
    if (sol[v] == -1) {
        // Assegna il nodo al gruppo corrente
        sol[v] = id;

        // Continua a esplorare i nodi collegati
        for (int i = v + 1; i < V; ++i) {
            if (g[v][i] > 0) {
                connR(g, i, id, sol, bs, min, depth + 1, num);
            }
        }

        // Ripristina il nodo e prova un nuovo gruppo
        sol[v] = id+1;
        *num+=1;
        connR(g, v + 1, id + 1, sol, bs, min, 0, num);
    } else {
        *num+=1;
        connR(g, v + 1, id, sol, bs, min, depth, num);
    }
}*/


void connR(int g[V][V], int v, int id, int *sol, int *bs, int *min, int depth) {
    // Se il depth è superiore a 2, termina la ricorsione
    if (depth > 2) return;

    // Se abbiamo esaminato tutte le città
    if (v >= V) {
        int currentMaxId = id + 1; // id rappresenta l'ultimo gruppo creato, quindi id + 1 è il numero totale di gruppi
        if (currentMaxId < *min) {
            *min = currentMaxId;
            for (int i = 0; i < V; i++) {
                bs[i] = sol[i]; // Salva la soluzione migliore
            }
        }
        return;
    }

    if (sol[v] == -1) { // Se la città v non è stata ancora assegnata a un gruppo
        sol[v] = id; // Assegna la città v al gruppo corrente

        // Prova a collegare altre città connessi a v
        for (int i = 0; i < V; i++) {
            if (g[v][i] > 0 && sol[i] == -1) {
                connR(g, i, id, sol, bs, min, depth + 1); // Continua la DFS
            }
        }

        sol[v] = -1; // Backtracking: rimuove la città v dal gruppo corrente
    }

    connR(g, v + 1, id + 1, sol, bs, min, 0); // Prova a iniziare un nuovo gruppo con la prossima città
}


/*void connR(int g[V][V], int v, int id, int *sol, int *bs, int *min, int depth, int *num) {
    if(depth > 2 || *num > V) return;

    // debug print
    printf("%d\n", *num);

    // terminazione
    if(v >= V){
        // inclusione nodi non presi
        for (int i = 0; i < V; ++i) {
            if(sol[i] == -1){
                (*num)++;
                connR(g, i, *num - 1, sol, bs, min, 0, num);
            }
        }

        if (*num < *min) {
            *min = *num;
            for (int i = 0; i < V; ++i) {
                bs[i] = sol[i];
            }
        }
        return;
    }

    if(sol[v] == -1) {
        sol[v] = id;
        for (int i = 0; i < V; ++i) {
            if(g[v][i] > 0 && sol[i] == -1 && v != i){
                connR(g, i, id, sol, bs, min, depth+1, num);

            }
        }
        sol[v] = -1;
        connR(g, v+1, id, sol, bs, min, 0, num);
    }
}*/

void connections(int Madj[V][V]){
    int *sol = malloc(V*sizeof(int));
    int *bestSol = malloc(V*sizeof(int));
    int min = V;
    int num = 1;
    for (int i = 0; i < V; ++i) sol[i] = -1;
    connR(Madj, 0, 0, sol, bestSol, &min, 0);
    printf("Il numero minimo di raggruppamenti e': %d, e sono:\n", min);
    for (int i = 0; i < V; ++i) {
        printf("Nodo %d in gruppo %d\n", i, bestSol[i]);
    }
    free(sol);
    free(bestSol);

}

void print2dFloat(float **M, int r, int c){
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            printf("%2.2f ", M[i][j]);
        }
        printf("\n");
    }
}

int main(int argv, char *argc[]) {
    // ese 1
    if(argv < 3) return -1;
    int r = atoi(argc[1]);
    int c = atoi(argc[2]);
    int **M = malloc(sizeof(int*)*r);
    float **Mp = NULL;
    FILE *fd;

    if((fd = fopen(FILENAME, "r")) == NULL) return -1;

    for (int i = 0; i < r; ++i) {
        M[i] = malloc(sizeof(int)*c);
    }

    for(int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            if(fscanf(fd, "%d ", &M[i][j]) < 1) return -1;
        }
    }
    fclose(fd);

    f(M, r, c, &Mp);

    print2dInt(M, r, c);
    print2dFloat(Mp, r, c);

    free2d((void **) M, r);
    free2d((void **) Mp, r);

    // ese 2
    // si vedano Tree.c e Tree.h

    // ese 3
    /*
    int g[V][V] = {{1, 0, 1, 0, 0},
                   {0, 0, 0, 1, 0},
                   {1, 0, 0, 0, 1},
                   {0, 1, 0, 0, 0},
                   {0, 0, 1, 0, 0}
    };
    */
    int g[V][V] = {{0, 0, 0, 0, 1},
                   {0, 0, 0, 1, 0},
                   {0, 0, 0, 1, 1},
                   {0, 0, 1, 0, 1},
                   {1, 1, 1, 1, 0}
    };
    printf("\n\n");
    connections2(V, g);

    return 0;
}

void avgR(int **M, int i, int j, int r, int c, float *avg, int *num, int pos){
    switch (pos) {
        case 0:
            for (int k = 0; k < VALSIZE; ++k) {
                avgR(M, i + VAL[k], j, r, c, avg, num, pos+1);
            }
            break;
        case 1:
            for (int k = 0; k < VALSIZE; ++k) {
                avgR(M, i, j+VAL[k], r, c, avg, num, pos+1);
            }
            break;

        // terminazione
        default:
            if(i >= 0 && i < r && j >= 0 && j < c){
                *num += 1;
                *avg += (float)M[i][j];
                return;
            }
    }
}

void f(int **M, int r, int c, float ***Mp){
    float **s = *Mp;
    float avg = 0;
    int num = 0;

    if(s == NULL){
        s =  malloc(sizeof(float*)*r);
        for (int i = 0; i < r; ++i) {
            s[i] = malloc(sizeof(float)*c);
        }

        for (int i = 0; i < r; ++i) {
            for (int j = 0; j < c; ++j) {
                avgR(M, i, j, r, c, &avg, &num, 0);
                s[i][j] = avg/((float)num);
                avg = 0;
                num = 0;
            }
        }
    }
    *Mp = s;

}



void connections2(int n, int M[][V]){
    int *sol = malloc(n * sizeof(int)),
        *bs = malloc(n * sizeof(int)),
        min = n+1;
    erPartR(bs, sol, n, 0, 0, M, &min);
    printf("Il numero minimo di raggruppamenti e': %d, e sono:\n", min);
    for (int i = 0; i < V; ++i) {
        printf("Nodo %d in gruppo %d\n", i, bs[i]);
    }

    free(sol);
    free(bs);
}

void erPartR(int *bs, int *sol, int n, int pos, int m, int M[][V], int *min){
    //if(m > *min) return;
    if(pos >= n){
        if(m < *min && verify(n, m, sol, M)){
            *min = m;
            for (int i = 0; i < n; ++i) {
                bs[i] = sol[i];
            }
        }
        return;
    }

    for (int i = 0; i < m; ++i) {
        sol[pos] = i;
        erPartR(bs, sol, n, pos+1, m, M, min);
    }
    sol[pos] = m;
    erPartR(bs, sol, n, pos+1, m+1, M, min);

}

int verify(int n, int m, int *sol, int M[][V]){
    int *mark = calloc(m, sizeof(int)),
        *visited = calloc(n ,sizeof(int));
    int flag = 1;
    int i;
    for (int g = 0; g < m; ++g) {
        if(mark[g] == 0){
            for (i = 0; i < n && sol[i] != g; ++i);
            mark[g] = 1;
            verifyR(i, n, M, 0, visited, sol, g);
        }
    }
    for (int j = 0; j < n && flag; ++j) {
        if(!visited[j]) flag = 0;
    }

    free(mark);
    free(visited);
    return flag;
}

void verifyR(int v, int n, int M[][V], int depth, int *visited, int *sol, int g) {
    if (depth > 2) return;
    visited[v] = 1;
    for (int i = 0; i < n; ++i) {
        if (M[v][i] > 0 && sol[i] == g && !visited[i]) {
            verifyR(i, n, M, depth + 1, visited, sol, g);
        }
    }
}