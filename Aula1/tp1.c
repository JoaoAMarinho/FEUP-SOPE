#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>

void ex_4(){
    //read();
    /*
    gdb a.out
    disas read => mostra o disasemble da função
    ou
    objdump -d a.out
    */
}

int ex_5(int argn, char*argc[], char* envc[]){
    printf("Exercício 5 a)\n");

    for(int i=1; i<argn; i++){
        printf("%s\n",argc[i]);
    }

    char* env;
    for(int i=0; (env=envc[i])!=NULL; i++){
        printf("%s\n",env);
    }

    printf("\nExercício 5 b)\n");
    for(int i=1; i<argn; i++){
        char* string=argc[i];
        int j=0;
        while(envc[j]!=NULL){
            char* s2=strtok(envc[j],"=");
            if(strcmp(string,s2)==0){
                printf("%s\n",strtok(NULL,"="));
                break;
            }
            j++;
        }
    }

    //Alínea c) usar export e nome da var=value, para criar uma environment variable.
    //Para apagar usar o unset seguido do nome da var.

    return 0;
}

int ex_6(char *file_name){
    //Alínea A
    //Flag O_CREAT cria se não existir;
    if(open(file_name, O_RDONLY)==-1) printf("Error openning in out.\n");

    //Alínea B
    if(open(file_name, O_RDONLY)==-1) fprintf(stderr, "%s", "Error openning in err.\n");

    /* 
    Correr o exe seguido dos argumentos > nome do file para stdout 2> nome do file para stderr, cria ficheiros para os outputs de cada tipo.
    
    ex: ./a.out file_searched > file_stdout 2> file_stderr
    Neste caso o "Error openning in out" vai para file_stdout e "Error openning in err" vai para file_stderr.
    */

    //Alínea C
    if(open(file_name, O_RDONLY)==-1) perror("Error");
    //Same as fprintf to stderr, contudo é apresentado ": " seguido do erro provocado.
}

int ex_10(){
    //Print hello world 100 000 times.
    //time ./a.out -> diz nos o tempo
    for(int i=0; i <100000; i++) printf("Hello world!");
    return 0;
}

int ex_11_a(char *argc[]){
    char *file_1=argc[1];
    char *file_2=argc[2];
    char buf[100];
    int fd = open(file_1, O_RDONLY);
    if(fd==-1) printf("Error openning in out.\n");
    else{
        int counter = read(fd, buf, 100);
        int fd2 = open(file_2, O_WRONLY);
        write(fd2, buf, counter);
        close(fd);
        close(fd2);
    }
    return 0;
}
int ex_11_b(char *argc[]){
    FILE* fp;
    FILE* fp2;
    char *file_1=argc[1];
    char *file_2=argc[2];
    int c;
    fp = fopen(file_1, "r");
    fp2=fopen(file_2,"w");
    if(fp){
        //Possível ler uma string e escreve-la toda.
        while((c=getc(fp))!= EOF){
            fputc(c, fp2);
        }
        fclose(fp);
        fclose(fp2);
    }
    return 0;
}
int ex_11_c(char *argc[]){
    FILE* fp;
    FILE* fp2;
    if(argc[2]==NULL){
        char *file_1=argc[1];
        char string[20];
        fp = fopen(file_1, "r");
        if(fp){
            //Lê uma string com max 255 char;
            fgets(string,255, fp);
            //Print to stdout;
            printf("%s", string);
            fclose(fp);
        }
        return 0;
    }
    char *file_1=argc[1];
    char *file_2=argc[2];
    int c;
    fp = fopen(file_1, "r");
    fp2=fopen(file_2,"w");
    if(fp){
        while((c=getc(fp))!= EOF){
            fputc(c, fp2);
        }
        fclose(fp);
        fclose(fp2);
    }
    return 0;
}

int ex_12_a(char* argc[]){
    DIR* d;
    struct stat statbuf;
    struct dirent *dir;
    d=opendir(argc[1]);
    if(d){
        while ((dir=readdir(d))!= NULL)
        {
            stat(dir->d_name,&statbuf);
            printf("File name/size: %s / %ld\n", dir->d_name,statbuf.st_size);
        }
        closedir(d);
    }
    return 0;
}
int ex_12_b(char* argc[]){
    DIR* d;
    FILE* fp;
    char *string;
    fp=fopen("Files_in_dir","w");
    struct stat statbuf;
    struct dirent *dir;
    d=opendir(argc[1]);
    if(d){
        while ((dir=readdir(d)) != NULL)
        {
            stat(dir->d_name,&statbuf);
            //Number of files is equal to the number of lines outputed to Files_in_dir
            string= strcat(dir->d_name,"\n");
            fputs(string, fp);
        }
        closedir(d);
        fclose(fp);
    }
    return 0;
}

int main(int argn, char*argc[], char* envc[]){
    
    //ex 4
    //ex_4();

    //ex 5
    //ex_5(argn, argc, envc);

    //ex 6
    //ex_6(argc[1]);

    //ex 10
    //ex_10();

    //ex 11
    //ex_11_a(argc);
    //ex_11_b(argc);
    ex_11_c(argc);

    //ex 12
    ex_12_a(argc);
    ex_12_b(argc);

    exit(EXIT_SUCCESS);
}