#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
struct particion{
    char part_status;
    char part_type[5];
    char part_fit[5];
    int part_start;
    long part_size;
    char part_name[16];
};
struct mbr{
        int mbr_tamano;
        char mbr_fecha_creacion[128];
        int mbr_disk_signature;
        struct particion part[4];

};

void automata(char inst[80]){
    int size_part;
    char name_rep[20];

    char path_mount[50];
    char name[10];

    char path_part[50];
    char name_part[20];
    char unit_par[5];
    char type_part[5];
    char fit[5];
    strcpy(fit,"N");
    strcpy(type_part,"P");
    strcpy(unit_par,"k");

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
                }else if(strcmp(token,"mount")){
                    estado=17;
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
                    printf("Nombre_part: %s, size_part: %d, path_part: %s-\n",name_part,size_part,path_part);
                    crearParticion(path_part,size_part,name_part,type_part,fit);
                    estado=0;
                }
                break;
            case 9:
                printf("valor p size\n");
                size_part=atoi(token);
                estado=8;
                break;
            case 10:
                printf("valor p unit\n");
                estado=8;
                break;
            case 11:
                printf("valor p path\n");
                strcpy(path_part,token);
                estado=8;
                break;
            case 12:
                printf("valor p type\n");
                estado=8;
                break;
            case 13:
                printf("valor p fit\n");
                strcpy(fit,token);
                estado=8;
                break;
            case 14:
                printf("valor p delete\n");
                estado=8;
                break;
            case 15:
                printf("valor p name\n");
                strcpy(name_part,token);
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

void crearDisco(int size, char path[100], char nombre[50], char unit[5]){

    mkdir(path,0777);

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
    nuevoDisco.mbr_tamano=unitMult;
    printf("tamaño: %d", unitMult);
    strcpy(nuevoDisco.mbr_fecha_creacion,output);
    nuevoDisco.mbr_disk_signature=10+(rand()%991);
    nuevoDisco.part[0].part_status=0;
    nuevoDisco.part[1].part_status=0;
    nuevoDisco.part[2].part_status=0;
    nuevoDisco.part[3].part_status=0;


    FILE *file;
    char direccion[80];
    strcpy(direccion,path);
    strcat(direccion,nombre);

    file = fopen(direccion,"w");
    //fseek(file,sizeof(struct mbr),SEEK_SET);
    fwrite(&nuevoDisco,sizeof(struct mbr),1,file);
    int j=0;
    for(j=0;j<unitMult;j++){

        fwrite(&kilobyte,sizeof(kilobyte),1,file);
    }

    //fwrite(0,sizeof(BYTE_ORDER),2048,file);
    fclose(file);
}

void crearParticion(char path[50], int size, char name[16], char type_part[5], char fit[5], char unit[5]){
    printf("En metodo particion\n");
    long size_part=0;

    int num_primarias=0;
    int num_extendidas=0;

    if(strcmp(unit,"K")==0 || strcmp(unit,"k")==0){
        size_part=size*1024;
    }else if(strcmp(unit,"M")==0 || strcmp(unit,"m")==0){
        printf("en M\n");
        size_part=size*1024*1024;
    }else if(strcmp(unit,"B")==0 || strcmp(unit,"b")==0){
         size_part=size;
    }
    struct particion part;
    strcpy(part.part_name,name);
    part.part_size=size_part;
    strcpy(part.part_type,type_part);
    strcpy(part.part_fit,fit);
    part.part_status=1;

    struct mbr a,b;
    FILE* file;
    file=fopen(path,"r+");

    if(file){

        fread(&a,sizeof(struct mbr),1,file);
        int size_disk=a.mbr_tamano*1024;
        int tam_acumalado_part=0;

        int n=0;
        for(n=0;n<4;n++){
            if(strcmp(a.part[n].part_type,"E")==0){
                printf("contando Extendidas\n");
                num_extendidas++;
            }
            else if(strcmp(a.part[n].part_type,"P")==0){
                printf("contando primarias\n");
                num_primarias++;
            }
        }

        if(num_primarias<4 && num_extendidas<=1){
            int i=0;
            for(i=0; i<4; i++){
                if(a.part[i].part_status==0){

                        if(size_part<(size_disk-tam_acumalado_part)){

                            part.part_start=tam_acumalado_part+256;
                            a.part[i]=part;
                            fseek(file,0,SEEK_SET);
                            fwrite(&a,sizeof(struct mbr),1,file);
                             printf("Se a creado la particion\n");
                             break;
                        }
                        else{
                            printf("no hay espacion suficiente en el disco para crear la particion\n");
                            break;
                        }

                }else{
                    tam_acumalado_part+=a.part[i].part_size;
                }
            }
        }else{
            if(num_primarias==4)
                printf("No se puede crear particion, ya se a creado el numero maximo de particiones primarias\n");
            else if(num_extendidas>=1)
                printf("No se puede crear particion, ya se a creado el numero maximo de particiones Extendidas\n");
        }




        fseek(file,0,SEEK_SET);
        fread(&b,sizeof(struct mbr),1,file);

        int j=0;
        for(j=0;j<4;j++){
            printf("Nombre: %s, size: %d, Ajuste: %s, Tipo: %s, start: %ld\n",
                                                b.part[j].part_name,b.part[j].part_size,b.part[j].part_fit,b.part[j].part_type,b.part[j].part_start);
        }

        reporte("/home/omar/Escritorio/tablaMBR.txt",b);

     /*   printf("Fecha de creacion: %s, %ld, Tamaño: %d, Nombre part 1: %s\n",
                                    b.mbr_fecha_creacion,ftell(file),b.mbr_tamano,b.part[0].part_name);*/

        fclose(file);
    }else{
        printf("Archivo no existe\n");
    }


}
void leer(char path[50], char nombre[50]){

    struct mbr a;
    char direccion[80];
    strcpy(direccion,path);
    strcat(direccion,nombre);
    FILE *file;
    file = fopen(direccion,"r");
    if(file){
        (file,0,SEEK_END);
        fread(&a,sizeof(struct mbr),1,file);

        printf("Fecha de creacion: %s, %ld, Tamaño: %d, Disk_signature: %d\n",
                                    a.mbr_fecha_creacion,ftell(file),a.mbr_tamano,a.mbr_disk_signature);
        fclose(file);
    }else{
        printf("Archivo no existe");
    }

}

void reporte(char path[100], struct mbr disco){
    char texto[2000];
    strcpy(texto,"digraph G {\nnode [shape=plaintext]\na [label=<<table border=\"0\" cellborder=\"1\" cellspacing=\"0\">\n");
    strcat(texto,"<tr><td><b>Nombre</b></td><td><b>Valor</b></td></tr>\n");
    strcat(texto,"<tr><td>mbr_tamaño</td><td>");
    char str[10];
    sprintf(str, "%d",(disco.mbr_tamano*1024));
    strcat(texto,str);
    strcat(texto,"</td></tr>");
    strcat(texto,"<tr><td>mbr_fecha_creacion</td><td>");
    strcat(texto,disco.mbr_fecha_creacion);
    strcat(texto,"</td></tr>");
    strcat(texto,"<tr><td>mbr_disk_signature</td><td>");
    char str2[10];
    sprintf(str2, "%d",disco.mbr_disk_signature);
    strcat(texto,str2);
    strcat(texto,"</td></tr>");

    int i=0;
    int num_part=1;
    for(i=0;i<4;i++){
        char str3[3];

        sprintf(str3, "%d",num_part);
        strcat(texto,"<tr><td>part_status_");
        strcat(texto,str3);
        strcat(texto,"</td><td>");
        char status[3];
        sprintf(status, "%d",disco.part[i].part_status);
        strcat(texto,status);
        strcat(texto,"</td></tr><tr><td>part_type_");
        strcat(texto,str3);
        strcat(texto,"</td><td>");
        strcat(texto,disco.part[i].part_type);
        strcat(texto,"</td></tr><tr><td>part_fit_");
        strcat(texto,str3);
        strcat(texto,"</td><td>");
        strcat(texto,disco.part[i].part_fit);
        strcat(texto,"</td></tr><tr><td>part_start_");
        strcat(texto,str3);
        strcat(texto,"</td><td>");
        char start[10];
        sprintf(start, "%ld",disco.part[i].part_start);
        strcat(texto,start);
        strcat(texto,"</td></tr><tr><td>part_size_");
        strcat(texto,str3);
        strcat(texto,"</td><td>");
        char si[10];
        sprintf(si, "%d",disco.part[i].part_size);
        strcat(texto,si);
        strcat(texto,"</td></tr><tr><td>part_name_");
        strcat(texto,str3);
        strcat(texto,"</td><td>");
        strcat(texto,disco.part[i].part_name);
        strcat(texto,"</td></tr>");
    num_part++;
    }

    strcat(texto,"</table>>];}");
    FILE *file;
    file=fopen(path,"w");
    fputs(texto,file);
    fclose(file);

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
  /*FILE* fichero;
    fichero = fopen("/home/omar/Escritorio/pr.txt", "wt");
    fputs("jajajaja", fichero);
    fclose(fichero);*/
    //mkdir("/home/omar/Escritorio/misDiscos/discos",0777);
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    char output[128];
    strftime(output,128,"%d/%m/%y %H:%M:%S",tlocal);
    printf("%s\n",output);
    char a[50];
    char b[50];
    strcpy(a,"/home/omar/Escritorio/");
    strcpy(b,"disco1.dsk");
    //leer(a,b);
//crearDisco(50,"/home/omar/Escritorio/","disco1.dsk","K");
    int i =1;
    char instruccion[80];
  crearParticion("/home/omar/Escritorio/disco1.dsk",5,"part1_prueba","P","FF","K");
    //strcpy(instruccion,"mkdisk -size::200 -name::\"disco2.dsk\" -path::\"/home/omar/Escritorio/\"");
    do{
            printf("root@omar_201403981:~$ ");
            fgets(instruccion,80,stdin);
            strcat(instruccion," $");
            automata(instruccion);

    }while(strcmp(instruccion,"exit")!=0);

    return 0;
}
