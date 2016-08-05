#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void automata(char inst[80]){
    const char s[10]=" ::-/\"\\";
    char *token;

    token = strtok(inst,s);
    while(token!=NULL){
        printf( " %s\n", token );
        token = strtok(NULL, s);
    }
}

int main()
{
    int i =1;
    char instruccion[80];
    do{
            printf("root@omar_201403981:~$ ");
            scanf("%s",&instruccion);
            //automata(instruccion);

    }while(strcmp(instruccion,"exit")!=0);

    return 0;
}
