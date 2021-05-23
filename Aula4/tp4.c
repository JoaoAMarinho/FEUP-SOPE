#include <stdlib.h>
#include <stdio.h> 
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <signal.h>

void handler(){

}



int ex_12_cont(char* argc){
    DIR* d;
    struct stat statbuf;
    struct dirent *dir;
    d=opendir(argc);
    if(d){
        while ((dir=readdir(d))!= NULL)
        {
            chdir(argc);
            stat(dir->d_name,&statbuf);
            if(S_ISREG(statbuf.st_mode)){
                printf("File name/size: %s / %ld\n", dir->d_name,statbuf.st_size);
            }
            else if(S_ISDIR(statbuf.st_mode) && (strcmp(dir->d_name,"..")!=0) && (strcmp(dir->d_name,".")!=0) ){
                int pid = fork();
                if(pid==0){
                    char * argc1=(char *)malloc(sizeof(char)*strlen(argc));
                    argc1=strcpy(argc1,argc);
                    strcat(argc1, "/");
                    strcat(argc1,dir->d_name);
                    ex_12_cont(argc1);
                    exit(0);
                }else if(pid>0){
                    wait(&pid);
                }else{
                    perror("Fork()");
                    exit(1);
                }
            }
        }
        closedir(d);
    }
    return 0;
}

char *cmdline[] = { "sleep", "10", (void *)0 };
char *newenviron[] = { "USER=Linus", NULL };
int ex_2(){
    pid_t id;
    setbuf(stdout, NULL);	// to make sure printf() have no buffer so, nodelay!

    printf("\nParent: %d. USER=%s", getpid(), getenv("USER"));

    id = fork();
    switch (id) {
        case -1:
            perror ("fork()");
            exit (1);
        case 0:	// child
            printf("\nChild: %d. USER=%s", getpid(), getenv("USER"));
            printf("\nChild: check that following exec was succeful with: \"ps x | grep sleep\"\n");
            if (execve("/bin/sleep", cmdline, newenviron) == -1)
                perror("execve");
            printf("\nYou will not see this unless execve() fails! Know why?...\n");
            break;	// if execve fails...
        default:
            printf("\nParent still: %d. USER=%s", getpid(), getenv("USER"));
            wait(&id);
        }
    return 0;
}
int ex_2_a(){
    pid_t id;
    setbuf(stdout, NULL);	// to make sure printf() have no buffer so, nodelay!

    printf("\nParent: %d. USER=%s", getpid(), getenv("USER"));

    id = fork();
    switch (id) {
        case -1:
            perror ("fork()");
            exit (1);
        case 0:	// child
            printf("\nChild: %d. USER=%s", getpid(), getenv("USER"));
            printf("\nChild: check that following exec was succeful with: \"ps x | grep sleep\"\n");
            if (execv("/bin/sleep", cmdline) == -1)
                perror("execve");
            printf("\nYou will not see this unless execve() fails! Know why?...\n");
            break;	// if execve fails...
        default:
            printf("\nParent still: %d. USER=%s", getpid(), getenv("USER"));
            wait(&id);
        }
    return 0;
}

int ex_2_b(){
    pid_t id;
    setbuf(stdout, NULL);	// to make sure printf() have no buffer so, nodelay!

    printf("\nParent: %d. USER=%s", getpid(), getenv("USER"));

    id = fork();
    switch (id) {
        case -1:
            perror ("fork()");
            exit (1);
        case 0:	// child
            printf("\nChild: %d. USER=%s", getpid(), getenv("USER"));
            printf("\nChild: check that following exec was succeful with: \"ps x | grep sleep\"\n");
            if (execv("/bin/sleep", cmdline) == -1)
                perror("execve");
            printf("\nYou will not see this unless execve() fails! Know why?...\n");
            break;	// if execve fails...
        default:
            printf("\nParent still: %d. USER=%s", getpid(), getenv("USER"));
            wait(&id);
            printf("\nStatus: %d", id);
        }
    return 0;
}

int ex_2_c(){
    pid_t id;
    setbuf(stdout, NULL);	// to make sure printf() have no buffer so, nodelay!

    printf("\nParent: %d. USER=%s", getpid(), getenv("USER"));

    id = fork();
    switch (id) {
        case -1:
            perror ("fork()");
            exit (1);
        case 0:	// child
            printf("\nChild: %d. USER=%s", getpid(), getenv("USER"));
            printf("\nChild: check that following exec was succeful with: \"ps x | grep sleep\"\n");
            if (execve("./call-sleep", cmdline, newenviron) == -1)
                perror("execve");
            printf("\nYou will not see this unless execve() fails! Know why?...\n");
            break;	// if execve fails...
        default:
            printf("\nParent still: %d. USER=%s", getpid(), getenv("USER"));
            wait(&id);
            printf("\nStatus: %d", id);
        }
    return 0;
}

int ex_3(){
    int pid1 =fork();

    if(pid1==0){
        //printf("IM CHILD!");
        int pid2=fork();
        if(pid2==0){
            while(1){
                signal(SIGINT,handler);
            }
            printf("IM GRANDCHILD! Group %d, GroupLeader %d\n", getgid(), getpgid(getpid()));
        }else if(pid2>0){
             while(1){
                signal(SIGINT,handler);
            }
            printf("IM CHILD! Group %d, GroupLeader %d\n", getgid(), getpgid(getpid()));
            wait(&pid2);
        }else{
            perror("Fork");
            exit(1);
        }
    } else if (pid1>0){
        printf("IM PAPA! Group %d, GroupLeader %d\n", getgid(), getpgid(getpid()));
        wait(&pid1);
    } else {
        perror("Fork");
        exit(1);
    }

    return 0;
}


int main(int argn, char*argc[], char* envc[]){

    //ex_12 cont
    //ex_12_cont(argc[1]);

    //ex_2
    //ex_2();
    //ex_2_a();
    //ex_2_b();
    //ex_2_c();
    //ex_3();

    exit(EXIT_SUCCESS);
}