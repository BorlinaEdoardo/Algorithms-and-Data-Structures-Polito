#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ese 1
int conta(char **matrice, int n, int m, char *stringa){
    int count = 0;
    int len = (int)strlen(stringa)+1;
    char *s = malloc(len*sizeof(char));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {

            // controllo per riga
            for (int k = j, pos = 0; k < m && pos < len-1; k++, pos++) {
                s[pos] = matrice[i][k];
            }
            s[strlen(s)] = '\0';
            if(strcmp(stringa, s) == 0) count++;

            // controllo per colonna
            for (int l = i, pos = 0; l < n && pos < len-1; l++, pos++) {
                s[pos] = matrice[l][j];
            }
            s[strlen(s)] = '\0';
            if(strcmp(stringa, s) == 0) count++;
        }
    }

    free(s);
    return count;
}

// ese 2
// vedere LIST.h e LIST.

// ese 3

int verify(char **stringhe, int *sol, int nsol){
    int valid = 1;
    int *mark = calloc('Z'-'A'+1, sizeof(int));
    char *s;

    for (int i = 0; i < nsol && valid; ++i) {
        s = stringhe[sol[i]];
        for (int j = 0; j < strlen(s); ++j) {
            mark[s[j]-'A'] ++;
            if(mark[s[j]-'A'] > 1) valid = 0;
        }
    }
    free(mark);
    return valid;
}

void combR(int n, int k, int start, int pos, int *mark, int *sol, int *num, char **stringhe) { // il flag indica il fatto che sia stata trovata una soluzione valida
    if(*num > 0) return;
    if(pos>=k){
        if(verify())
            *num = k;
        return;
    }

}
int disgiunte(char **stringhe, int n){
int dis = 0;



    return 1;
}




int main() {
    printf("%d\n", 'Z'-'A'+1);
    return 0;
}
