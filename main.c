#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void automata(char inst[80]){
    int size=0;
    char unit='M';
    char nombre[20];
    char path[50];
    const char s[20]=" ::-\"\\+";
    char *token;
    int estado=0;
    token = strtok(inst,s);
    while(token!=NULL){

       // printf("%s",token);
        switch(estado){
            case 0:
                if(strcmp(token,"mkdisk")==0){
                    printf("En mkdisk\n");
                    estado=1;
                }
                break;
            case 1:
                if(strcmp(token,"size")==0){
                    printf("En size\n");
                    estado=2;
                }
                if(strcmp(token,"name")==0){
                    printf("En name\n");
                    estado=3;
                }
                if(strcmp(token,"path")==0){
                    printf("En path\n");
                    estado=4;
                }
                if(strcmp(token,"unit")==0){
                    printf("En unit\n");
                    estado=5;
                }else{
                    estado=0;
                }
                break;
            case 2:
                printf("En valor de size: %s\n",token);
                estado =1;
                break;
            case 3:
                printf("En valor de name: %s\n",token);
                estado =1;
                break;
            case 4:
                printf("En valor de path: %s\n",token);
                estado =1;
                break;
            case 5:
                printf("En valor de unit: %s\n",token);
                estado =1;
                break;
        }

        token = strtok(NULL, s);
    }
}

/*void crearDisco(char comando[80]){
    int size=0;
    char unit='M';
    char nombre[20];
    char path[50];
    const char s[20]=" ::-\"\\+";
    char *token;
    int estado=0;
    token = strtok(inst,s);
}*/

int main()
{
    int i =1;
    char instruccion[80];
    do{
            printf("root@omar_201403981:~$ ");
            fgets(instruccion,80,stdin);
            strcat(instruccion,"$");
            automata(instruccion);

    }while(strcmp(instruccion,"exit")!=0);

    return 0;
}
