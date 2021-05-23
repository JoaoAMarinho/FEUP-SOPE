#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void func(int signo) {
    printf("Hello\n"); 
    fflush(stdout);
    //signal(SIGUSR1, func);
}


int ex_5(){
    int id=fork();
    if(id==0){
        printf("Hello ");
    }else if(id>0){
        //wait(&id);  //Esperar pelo filho que acabe
        printf("World!");
    }else{
        printf("Error");
        return -1;
    }
    return 0;
}

int ex_8(){

    struct sigaction new, old;
	sigset_t smask;	// defines signals to block while func() is running

    // prepare struct sigaction
    if (sigemptyset(&smask)==-1)	// block no signal, -1 in casse of error
	    perror ("sigsetfunctions");     // sigemptyset-Initializes the signal set given by (smask) to  empty,  with all signals excluded from the set.
    new.sa_handler = func;
    new.sa_mask = smask; //sa_mask  specifies  a  mask  of signals which should be blocked
    new.sa_flags = 0;	// usually works, sa_flags  specifies  a  set  of flags which modify the behavior of the signal.

    if(sigaction(SIGUSR1, &new, &old) == -1)
	    perror ("sigaction");

    //printf("\nSIGUSR1 handler is ready...");
    //printf("\n\t Try: \"sh> kill -SIGUSR1 %d\".\n", getpid());

    int pid=fork();
    if(pid==0){
        pause();
        signal(SIGUSR1,func);
    }else if(pid>0){
        //sleep(3); alínea b.
        printf("\nFather: sending SIGUSR1\n\n");
        printf("World!"); 
        kill(pid, SIGUSR1);
    }else{
        printf("Error");
        return -1;
    } 
    //pause();	// wait, pause() causes the calling process to sleep until a signal
                        //is delivered that either terminates the process or causes the 
                        //invocation of a signal-catching function.

    return 0;
}

int main(int argn, char*argc[], char* envc[]){

    //ex 5
    //ex_5();

    //ex 8
    ex_8();


    exit(EXIT_SUCCESS);
}