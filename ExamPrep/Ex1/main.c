#include <stdio.h>
#include <stdlib.h>

#define FILENAME "table.txt"
#define VALSIZE 3

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
