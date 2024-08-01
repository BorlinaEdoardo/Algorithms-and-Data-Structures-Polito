
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define LOG "corse.txt"
#define COMMANDSNUM 7

typedef struct route_s {
    char *id;
    char *departure;
    char *destination;
    char *date;
    char *arrTime;
    char *depTime;
    int delay;
}route_t;

typedef struct deposit_s{
    route_t *lines;
    int size;
    int maxSize;
}deposit_t;

deposit_t newDeposit(){
    deposit_t deposit;
    deposit.lines = NULL;
    deposit.maxSize = 0;
    deposit.size = 0;
    return  deposit;
}

typedef enum {r_help, r_date, r_partenza, r_capolinea, r_ritardo, r_ritardo_tot, r_fine}comando_e;

char *commands[] = {"r_help","r_date", "r_partenza", "r_capolinea", "r_ritardo", "r_ritardo_tot", "r_fine"};

// function used to convert an input from string to comando_e
comando_e commandSearch(char* input);

// add a new line to a specified deposit
void addLine(deposit_t *deposit, char *line);

// This function is used to print the menu, it can also be called with the command help
void printMenu();

// this fuction releases all the resources of used by an instance of deposit_t, it is called with the command  r_end
void freeDeposit(deposit_t d){
    for(int i = 0; i<d.size; i++){
        free(d.lines[i].id);
        free(d.lines[i].date);
        free(d.lines[i].arrTime);
        free(d.lines[i].depTime);
        free(d.lines[i].destination);
        free(d.lines[i].departure);
    }
    free(d.lines);
    d.size = 0;
    d.maxSize = 0;
}


int main(){
    deposit_t deposit = newDeposit();
    comando_e command;
    char input[30];
    do{
        printMenu();
        printf("\tcommand >_");
        fflush(stdout);
        scanf("%s", input);
        command = commandSearch(input);
    } while(command != r_fine);
    printf("goodbye\n");
    freeDeposit(deposit);


    return 0;
}

comando_e commandSearch(char* input){
    for (int i = 0; i < COMMANDSNUM; ++i) {
        if(strcmp(input, commands[i]) == 0)
            return i;
    }
    return -1;
}

void printMenu(){
    printf("-----------------------------------------------------------------\n"
           "Available commands:\n"
           "\tr_help: print available commands\n"
           "\tr_date: shows all trips that departed between two dates\n"
           "\tr_partenza: shows all routes starting from a specified stop\n"
           "\tr_capolinea: shows all lines ending at a specified stop\n"
           "\tr_ritardo: displays the delay for a specific route\n"
           "\tr_ritardo_tot: displays the total delay accumulated\n"
           "\tr_fine: ends the program\n"
           "-----------------------------------------------------------------\n");
}

void addLine(deposit_t *deposit, char *line){
    route_t r;
    char id[30], dep[30], dest[30] , date[30], arrTime[30], depTime[30];
    sscanf("%s %s %s %s %s %s %s %d", id, dep, dest, date, arrTime, depTime, r.delay);
    r.id = strdup(id);
    r.departure = strdup(dep);
    r.destination = strdup(dest);
    r.date = strdup(date);
    r.depTime = strdup(depTime);
    r.arrTime = strdup(arrTime);

    deposit -> lines[(deposit->size)++] = r;
}
