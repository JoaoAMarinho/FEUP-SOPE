#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

#define NTHREADS 4

void func(int signo) {
    printf("Hello\n");
}

void *rot(void *a) {
    printf("\n\t In thread PID: %d ; TID: %lu.", getpid(), (unsigned long) pthread_self());
    int* value1=malloc(sizeof(int));
    *value1=*(int*)a;
    *value1*=(*value1);
	pthread_exit((void*)value1);	// no termination code
}
void *rot2(void *a){
    printf("\n\t In thread PID: %d ; TID: %lu.", getpid(), (unsigned long) pthread_self());
    pthread_exit(a);	// no termination code
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
        printf("Hello\n");
        //signal(SIGUSR1,func);
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

int ex_9(){

    int i;	// thread counter
	pthread_t ids[NTHREADS];	// storage of (system) Thread Identifiers

    setbuf (stdout, NULL);	// only for debugging
    printf("\nMain thread PID: %d ; TID: %lu.\n", getpid(), (unsigned long) pthread_self());

    // new threads creation
    for(i=0; i<NTHREADS; i++) {
        int* value=malloc(sizeof(int));
        *value=i;
        if (pthread_create(&ids[i], NULL, rot, value) != 0)
            exit(-1);	// here, we decided to end process
        else
            printf("\nNew thread %d ; TID: %lu.", i, (unsigned long) ids[i]);
        }
    // wait for finishing of created threads
    for(i=0; i<NTHREADS; i++) {
        void* x;
        pthread_join(ids[i], &x);	// Note: threads give no termination code
        
        printf("\nTermination of thread %d: %lu.", *(int*)x, (unsigned long)ids[*(int*)x]);
        }

    printf("\n");
    pthread_exit(NULL);	// here, not really necessary...

    return 0;
}

int ex_10(char*argc[]){

    int i;	            // thread counter
	pthread_t ids[2];	// storage of (system) Thread Identifiers

    setbuf (stdout, NULL);	// only for debugging
    printf("\nMain thread PID: %d ; TID: %lu.\n", getpid(), (unsigned long) pthread_self());

    // new threads creation
    for(i=0; i<2; i++) {
    
        if (pthread_create(&ids[i], NULL, rot2, argc[i+1]) != 0)
            exit(-1);	// here, we decided to end process
        else
            printf("\nNew thread %d ; TID: %lu.", i, (unsigned long) ids[i]);
        }
    // wait for finishing of created threads
    for(i=0; i<2; i++) {
        void* x;
        pthread_join(ids[i], &x);	// Note: threads give no termination code
        printf("\n");
        printf("%s",(char*)x);
        }

    printf("\n");
    pthread_exit(NULL);	// here, not really necessary...

    return 0;
}

int pipes_example(){
    int proc;
    int pp[2];
    if (pipe(pp) == -1){ //Pipe error
        perror("pipe()");
        exit(1); 
    }
    if ((proc = fork()) == -1){ 
        perror("fork()");
        exit(2);
    }
    if (proc == 0) {
        close(pp[0]);
        write ( pp[1], "Hi, parent!", 1+strlen("Hi, parent!")); //write 
        close(pp[1]);
    }
    else { 
        char msg[1024];
        close(pp[1]);
        read(pp[0], msg, 1024); // waits...
        printf("Child said: «%s»\n", msg);
        close(pp[0]);
    }
    return 0;
}

int ex_13_a(){
    int proc;
    int pp[2];
    if (pipe(pp) == -1){ //Pipe error
        perror("pipe()");
        exit(1); 
    }
    if ((proc = fork()) == -1){ 
        perror("fork()");
        exit(2);
    }
    if (proc == 0) {
        char msg[1024];
        close(pp[1]);
        read(pp[0], msg, 1024); // waits...
        strcat(msg,"Systems");
        printf("%s\n", msg);
        close(pp[0]);
    }
    else { 
        close(pp[0]);
        write ( pp[1], "Operating ", 1+strlen("Operating ")); //write 
        close(pp[1]);
    }
    return 0;
}

void named_pipes_reader(char* str){
    //READER:
    int np;
    char msg[1024];
    //if (mkfifo("/tmp/np",0666) < 0) perror ("mkfifo");
    while ((np = open ("/tmp/np", O_RDONLY)) < 0)
        ; // synchronization...
    read(np, msg, 1024); // waits...
    strcat(msg,str);
    printf("%s\n", msg);
    close(np);
}
void named_pipes_writer(char * str){
    int np;
    if (mkfifo("/tmp/np",0666) < 0) perror ("mkfifo");
    while ((np = open ("/tmp/np", O_WRONLY)) < 0)
    ; // synchronization...
    write (np, str, 1+strlen(str));
    close(np);
}
int ex_13_b(){
    int proc;
    if ((proc = fork()) == -1){ 
        perror("fork()");
        exit(2);
    }
    if (proc == 0) {
        named_pipes_reader("Systems");
    }
    else { 
        named_pipes_writer("Operating ");
    }
    return 0;
}

int ex_13_c(){
     int proc;
    int pp[2];
    if (pipe(pp) == -1){ //Pipe error
        perror("pipe()");
        exit(1); 
    }
    if ((proc = fork()) == -1){ 
        perror("fork()");
        exit(2);
    }
    if (proc == 0) {
        close(pp[0]);
        write ( pp[1], "Systems", 1+strlen("Systems")); //write 
        close(pp[1]);
    }
    else { 
        char str[1024]="Operating ";
        char msg[1024];
        close(pp[1]);
        read(pp[0], msg, 1024); // waits...
        strcat(str, msg);
        printf("%s\n", str);
        close(pp[0]);

    }
    return 0;
}

void ex_13_d(){
    int proc;
    if ((proc = fork()) == -1){ 
        perror("fork()");
        exit(2);
    }
    if (proc == 0) {
        named_pipes_writer("Systems");
    }
    else {
        named_pipes_reader("Operating ");
    }
}

int main(int argn, char*argc[], char* envc[]){

    //ex 8
    //ex_8();

    //ex 9
    //ex_9();

    //ex 10
    //ex_10(argc);

    //ex_13
    //ex_13_a();
    //ex_13_b();
    //ex_13_c();
    ex_13_d();

    exit(EXIT_SUCCESS);
}
