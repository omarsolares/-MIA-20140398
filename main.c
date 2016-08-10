#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
typedef struct particion{
    char part_status;
    char part_type;
    char part_fit;
    int part_start;
    int part_size;
    char part_name[16];
};
struct mbr{
        int mbr_tamano;
        char mbr_fecha_creacion[128];
        int mbr_disk_signature;
        struct particion *mbr_partition_1;
        struct particion *mbr_partition_2;
        struct particion *mbr_partition_3;
        struct particion *mbr_partition_4;
};

void automata(char inst[80]){
    int size=0;
    char unit[5];
    strcpy(unit,"M");
    char nombre[20];
    char path[50];
    const char s[20]=" ::-\"\\+";
    char *token;
    int estado=0;
    token = strtok(inst,s);
    while(token!=NULL){
        //printf("%d",estado);
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
                else if(strcmp(token,"name")==0){
                    printf("En name\n");
                    estado=3;
                }
                else if(strcmp(token,"path")==0){
                    printf("En path\n");
                    estado=4;
                }
                else if(strcmp(token,"unit")==0){
                    printf("En unit\n");
                    estado=5;
                }else{
                    crearDisco(size,path,nombre,unit);
                    leer(path,nombre);
                    estado=0;
                }
                break;
            case 2:
                printf("En valor de size: %s\n",token);
                size=atoi(token);
                estado =1;
                break;
            case 3:
                printf("En valor de name: %s\n",token);
                strcpy(nombre,token);
                estado =1;
                break;
            case 4:
                printf("En valor de path: %s\n",token);
                strcpy(path,token);
                estado =1;
                break;
            case 5:
                printf("En valor de unit: %s\n",token);
                strcpy(unit,token);
                estado =1;
                break;
        }

        token = strtok(NULL, s);
    }
    printf("size: %d, name: %s, path: %s, unit: %s\n",size,nombre,path,unit);
}

void crearDisco(int size, char path[50], char nombre[50], char unit[5]){
    srand(time(NULL));

    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    char output[128];
    strftime(output,128,"%d/%m/%y %H:%M:%S",tlocal);

    struct mbr nuevoDisco;
    nuevoDisco.mbr_tamano=size;
    strcpy(nuevoDisco.mbr_fecha_creacion,output);
    nuevoDisco.mbr_disk_signature=10+(rand()%991);
    nuevoDisco.mbr_partition_1=NULL;
    nuevoDisco.mbr_partition_2=NULL;
    nuevoDisco.mbr_partition_3=NULL;
    nuevoDisco.mbr_partition_4=NULL;
    FILE *file;
    char direccion[80];
    strcpy(direccion,path);
    strcat(direccion,nombre);

    file = fopen(direccion,"w");
    //fseek(file,sizeof(mbr),SEEK_SET);
    fwrite(&nuevoDisco,sizeof(struct mbr),1,file);
    fclose(file);
}
void leer(char path[50], char nombre[50]){

    struct mbr a;
    char direccion[80];
    strcpy(direccion,path);
    strcat(direccion,nombre);
    FILE *file;
    file = fopen(direccion,"r");

    fread(&a,sizeof(struct mbr),1,file);

    printf("Fecha de creacion: %s\n",a.mbr_fecha_creacion);
    fclose(file);
}

int main()
{
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    char output[128];
    strftime(output,128,"%d/%m/%y %H:%M:%S",tlocal);
    printf("%s\n",output);
    char a[50];
    char b[50];
    strcpy(a,"/home/omar/Escritorio/");
    strcpy(b,"pruba.dsk");
    leer(a,b);
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
