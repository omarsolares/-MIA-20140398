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
        struct particion mbr_partition_1;
        struct particion mbr_partition_2;
        struct particion mbr_partition_3;
        struct particion mbr_partition_4;
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
                else if(strcmp(token,"rmdisk")==0){
                    printf("En rmdisk\n");
                    estado=6;
                }else if(strcmp(token,"fdisk")==0){
                    printf("En fdisk\n");
                    estado=8;
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
                    printf("size: %d, name: %s, path: %s, unit: %s-\n",size,nombre,path,unit);
                    crearDisco(size,path,nombre,unit);
                    //leer(path,nombre);
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
            case 6:
                if(strcmp(token,"path")==0){
                    printf("En path rmdisk\n");
                    estado=7;
                }else{
                    eliminarDisco(path);
                    estado =0;
                }
                break;
            case 7:
                printf("En valor de path: %s\n",token);
                strcpy(path,token);
                estado =6;
                break;

            case 8:
                if(strcmp(token,"size")==0){
                    estado=9;
                }
                else if(strcmp(token,"unit")==0){
                    estado=10;
                }
                 else if(strcmp(token,"path")==0){
                    estado=11;
                }
                 else if(strcmp(token,"type")==0){
                    estado=12;
                }
                 else if(strcmp(token,"fit")==0){
                    estado=13;
                }
                 else if(strcmp(token,"delete")==0){
                    estado=14;
                }
                 else if(strcmp(token,"name")==0){
                    estado=15;
                }
                 else if(strcmp(token,"add")==0){
                    estado=16;
                }else{
                    estado=0;
                }
                break;
            case 9:
                printf("valor p size\n");
                estado=8;
                break;
            case 10:
                printf("valor p unit\n");
                estado=8;
                break;
            case 11:
                printf("valor p path\n");
                estado=8;
                break;
            case 12:
                printf("valor p type\n");
                estado=8;
                break;
            case 13:
                printf("valor p fit\n");
                estado=8;
                break;
            case 14:
                printf("valor p delete\n");
                estado=8;
                break;
            case 15:
                printf("valor p name\n");
                estado=8;
                break;
            case 16:
                printf("valor p add\n");
                estado=8;
                break;
        }

        token = strtok(NULL, s);
    }
   // printf("size: %d, name: %s, path: %s, unit: %s\n",size,nombre,path,unit);
}

void crearDisco(int size, char path[50], char nombre[50], char unit[5]){
    srand(time(NULL));
    int unitMult=1;
    char kilobyte[1024];
    int i=0;
    for (i=0;i<1024;i++){
        kilobyte[i]='\0';
    }

    if(strcmp(unit,"M")==0||strcmp(unit,"m")==0){
        unitMult=size*1000;
    }
    else if(strcmp(unit,"K")==0||strcmp(unit,"k")==0||strcmp(unit,"K\n")==0||strcmp(unit,"k\n")==0){
        unitMult=size;
    }

    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    char output[128];
    strftime(output,128,"%d/%m/%y %H:%M:%S",tlocal);

    struct mbr nuevoDisco;
    nuevoDisco.mbr_tamano=size;
    strcpy(nuevoDisco.mbr_fecha_creacion,output);
    nuevoDisco.mbr_disk_signature=10+(rand()%991);

    FILE *file;
    char direccion[80];
    strcpy(direccion,path);
    strcat(direccion,nombre);

    file = fopen(direccion,"w");
    fseek(file,sizeof(struct mbr),SEEK_SET);
    fwrite(&nuevoDisco,sizeof(struct mbr),1,file);
    int j=0;
    for(j=0;j<unitMult;j++){

        fwrite(&kilobyte,sizeof(kilobyte),1,file);
    }

    //fwrite(0,sizeof(BYTE_ORDER),2048,file);
    fclose(file);
}
void leer(char path[50], char nombre[50]){

    struct mbr a;
    char direccion[80];
    strcpy(direccion,path);
    strcat(direccion,nombre);
    FILE *file;
    file = fopen(direccion,"r");
    if(file){
        //fseek(file,sizeof(struct mbr),SEEK_SET);
        fread(&a,sizeof(struct mbr),1,file);

        printf("Fecha de creacion: %s, %ld\n",a.mbr_fecha_creacion,ftell(file));
        fclose(file);
    }else{
        printf("Archivo no existe");
    }

}

void eliminarDisco(char path[50]){
    if(remove(path)==0){
        printf("Se ha eliminado disco: %s\n",path);
    }else{
        printf("No se pudo eliminar disco: %s\n",path);
    }
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
  //  strcpy(instruccion,"mkdisk -size::200 -name::\"disco2.dsk\" -path::\"/home/omar/Escritorio/\"");
    do{
            printf("root@omar_201403981:~$ ");
            fgets(instruccion,80,stdin);
            strcat(instruccion," $");
            automata(instruccion);

    }while(strcmp(instruccion,"exit")!=0);

    return 0;
}
