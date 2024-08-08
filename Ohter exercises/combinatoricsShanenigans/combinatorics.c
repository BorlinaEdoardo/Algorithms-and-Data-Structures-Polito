//
// Created by edoar on 08/08/2024.
//

#include <stdio.h>
#include <stdlib.h>
int count = 0;


void complexDispR(char *v, int n, int k, int pos, char *s){
    if(pos >= k){
        printf("%d\t", ++count);
        for(int i = 0; i < k;i++) printf("%c ", s[i]);
        printf("\n");
        return;
    }

    for (int i = 0; i < n; ++i) {
        s[pos] = v[i];
        complexDispR(v, n, k, pos+1, s);
    }
}
void complexDisp(char *v, int k, int size){
    char *s = malloc(k * sizeof(char));
    complexDispR(v, size, k, 0, s);
    free(s);
}

void simpleDispR(char *v, int n, int k, int pos, char *s, int *mark){
    if(pos >= k){
        printf("%d\t", ++count);
        for(int i = 0; i < k;i++) printf("%c ", s[i]);
        printf("\n");
        return;
    }

    for (int i = 0; i < n; ++i) {
        if(mark[i]>0){
            s[pos] = v[i];
            mark[i] --;
            simpleDispR(v, n, k, pos+1, s, mark);
            mark[i] ++;
        }
    }
}

void simpleDisp(char *v, int k, int size){
    int *mark = calloc(size, sizeof(int) );
    char *s = malloc(k * sizeof(char));
    for (int i = 0; i < k; ++i) {
        mark[i] = 1;
    }
    simpleDispR(v, size, k, 0, s, mark);
    free(s);
    free(mark);
}

void perm(char *v, int size){
    int *mark = calloc(size, sizeof(int) );
    char *s = malloc(size*sizeof(char));
    for (int i = 0; i < size; ++i) {
        mark[i] = 1;
    }
    simpleDispR(v, size, size, 0, s, mark);
    free(s);
    free(mark);
}

void complPerm(char *v, int size) {
    if(v == NULL) return;

    int n = 1;
    int *mark = calloc(size, sizeof(char));
    char *s = malloc(size*sizeof(char));
    char *valSet = malloc(size*sizeof(char));
    char val;

    val = v[0];
    valSet[0] = val;
    mark[0]++;

    for (int i = 1; i < size; ++i) {
        if(v[i] != val){
            val = v[i];
            valSet[n++] = val;
        }
        mark[n-1]++;
    }

    simpleDispR(valSet, n, size, 0, s, mark);

    free(valSet);
    free(mark);
    free(s);
}

void combR(char *v, int n, int k, int pos, int start, int *mark, int *sol, int rep){
    if(pos >= k){
        printf("%d\t", ++count);
        for(int i = 0; i < k;i++) printf("%c ", v[sol[i]]);
        printf("\n");
        fflush(stdout);
        return;
    }

    for (int i = start; i < n; ++i) {
        if(!rep && mark[i] > 0){
            sol[pos] = i;
            mark[i]--;
            combR(v, n, k, pos+1, i+1, mark, sol, rep);
            mark[i]++;
        } else if(rep){
            sol[pos] = i;
            combR(v, n, k, pos+1, start, mark, sol, rep);
            start++;
        }
    }
}

void comb(char *v, int n, int k){
    int *mark = calloc(n, sizeof(int) );
    int *s = malloc(k * sizeof(int));
    for (int i = 0; i < n; ++i) {
        mark[i]++;
    }
    combR(v, n, k, 0, 0, mark, s, 0);
    free(s);
    free(mark);
}

void complComb(char *v, int n, int k){
    int *s = malloc(k * sizeof(int));
    combR(v, n, k, 0, 0, NULL, s, 1);
    free(s);
}


int main(){
    char v[] = {'A', 'A', 'B', 'B', 'B', 'C'};
    char vset[] = {'A', 'B', 'C', 'D', 'E', 'F'};
    complComb(vset, 6, 3);
    return 0;
}
