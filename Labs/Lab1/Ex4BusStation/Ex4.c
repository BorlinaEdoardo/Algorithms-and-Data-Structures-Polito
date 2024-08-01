
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define LOG "corse.txt"
#define COMMANDSNUM 8
#define DBG 1

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

typedef enum {r_help, r_date, r_partenza, r_capolinea, r_ritardo, r_ritardo_tot, r_print, r_fine}comando_e;

char *commands[] = {"r_help","r_date", "r_partenza", "r_capolinea", "r_ritardo", "r_ritardo_tot", "r_print", "r_fine"};

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

int timeToInt(char* date){
    int hh, mm, ss;
    sscanf(date, "%d:%d:%d", &hh, &mm, &ss);
    return hh*3600+mm*60+ss;
}

int dateToInt(char *date){
    int y, m, d;
    sscanf(date, "%d/%d/%d", &y, &m, &d);
    return y*10000+m*100+d;
}

void printLine(route_t line, FILE *fd){
    if(fd == NULL) return;
    fprintf(fd, "%s %s %s %s %s %s %d\n",
            line.id, line.departure, line.destination,
            line.date, line.depTime, line.arrTime, line.delay);

}

// this function handles all implemented commands
void commandHandling(deposit_t deposit, comando_e command);


int main(){
    deposit_t deposit = newDeposit();
    comando_e command;
    char line[100];

    // loadings lines from file
    FILE *fd;
    fd = fopen(LOG, "r");
    fscanf(fd, "%d\n", &deposit.maxSize);
    deposit.lines = (route_t *) malloc(deposit.maxSize * sizeof(route_t));
    for (int i = 0; i < deposit.maxSize; i++) {
        fgets(line, 100, fd);
        #if DBG
            printf("%s", line);
        #endif
        addLine(&deposit, line);
    }
    fclose(fd);

    // commands handling
    printMenu();
    char input[30];
    do{
        printf("@command >_");
        fflush(stdout);
        scanf("%s", input);
        command = commandSearch(input);
        if(command != r_fine) commandHandling(deposit, command);
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
    printf("\n-----------------------------------------------------------------\n"
           "Available commands:\n"
           "\tr_help: print available commands\n"
           "\tr_date: shows all trips that departed between two dates\n"
           "\tr_partenza: shows all routes starting from a specified stop\n"
           "\tr_capolinea: shows all lines ending at a specified stop\n"
           "\tr_ritardo: displays the delay for a specific route\n"
           "\tr_ritardo_tot: displays the total delay accumulated\n"
           "\tr_print: print all existing lines\n"
           "\tr_fine: ends the program\n"
           "-----------------------------------------------------------------\n");
}

void addLine(deposit_t *deposit, char *line){
    route_t r;
    char id[30], dep[30], dest[30] , date[30], arrTime[30], depTime[30];
    sscanf(line, "%s %s %s %s %s %s %d\n", id, dep, dest, date, arrTime, depTime, &r.delay);
    r.id = strdup(id);
    r.departure = strdup(dep);
    r.destination = strdup(dest);
    r.date = strdup(date);
    r.depTime = strdup(depTime);
    r.arrTime = strdup(arrTime);

    deposit -> lines[(deposit->size)++] = r;
}


void commandHandling(deposit_t deposit, comando_e command){
    int i = 0;
    char in1[100];
    char in2[100];
    int delayTot = 0;
    switch (command) {
        // r_help
        case r_help:
            printMenu();
            break;

        // r_date: shows all trips that departed between two dates
        case r_date:
            printf("\tinsert dates <yyyy/mm/dd yyyy/mm/dd>:");
            fflush(stdout);
            scanf("%s %s", in1, in2);
            int start = dateToInt(in1), end = dateToInt(in2);
            for(i = 0; i < deposit.maxSize; i++)
                if(dateToInt(deposit.lines[i].date) >= start && dateToInt(deposit.lines[i].date) <= end)
                    printLine(deposit.lines[i], stdout);

            break;

        // r_partenza: shows all routes starting from a specified stop
        case r_partenza:
            break;

        // r_capolinea: shows all lines ending at a specified stop
        case r_capolinea:
            break;

        // r_ritardo: displays the delay for a specific route
        case r_ritardo:
            break;

        // r_ritardo_tot: displays the total delay accumulated
        case r_ritardo_tot:
            for(i = 0; i < deposit.maxSize; i++) {
                delayTot += deposit.lines[i].delay;
            }
            printf("Total delay: %d\n", delayTot);

            break;

        //  r_print: print all existing lines
        case r_print:
            for (i = 0; i < deposit.size; ++i) {
                printLine(deposit.lines[i],stdout);
            }
            break;

        default:
            printf("the specified command does not exists\n");

    }
}