/* Testing access issues to shared variables
 * - Race conditions!
 * JMMC - Out.2001
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include<unistd.h>

#define MAXCOUNT 100000
#define BUF_SIZE 20
#define MAX_ITER 10000

long count;
long iter=0;
void *thr_fun(void *);
pthread_mutex_t M;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER; 


void *thr_fun(void *arg) {
    while (count < MAXCOUNT) {
        //Lock mutex
        pthread_mutex_lock(&M); 
        printf("\n%ld",count);
        count++;
        //Unlock mutex
        pthread_mutex_unlock(&M); 
    }
    printf("\nEND thread %lu!\n", (unsigned long) pthread_self());
    return (NULL);
}

void *thr_inc(void *arg) {
    while (iter < MAX_ITER) {
        pthread_mutex_lock(&M);
        if(iter >= MAX_ITER) break;
        if(count<20){ 
        iter++;
        printf("\n%ld",count);
        count++;
        }
        pthread_mutex_unlock(&M); 
    }
    printf("\nEND thread %lu!\n", (unsigned long) pthread_self());
    return (NULL);
}
void *thr_decr(void *arg) {
    while (iter < MAX_ITER) {
        pthread_mutex_lock(&M); 
        if(iter >= MAX_ITER) break;
        if(count>0){
        iter++;
        printf("\n%ld",count);
        count--;
        }
        pthread_mutex_unlock(&M); 
            
    }
    printf("\nEND thread %lu!\n", (unsigned long) pthread_self());
    return (NULL);
}

int ex_7(int argc, char *argv[]){
    pthread_t *ptid;
	int nthreads;
	int i;

    setbuf(stdout,NULL);
    if (argc < 2) {
        printf("\nUsage: %s #threads\n", argv[0]);
        exit(1);
    }
    nthreads = atoi(argv[1]);
    ptid = (pthread_t *) malloc(nthreads * sizeof(pthread_t));
    if (ptid == NULL) {
        perror("malloc");
        exit(2);
    }
    for (i = 0; i< nthreads; i++) {
        if(pthread_create(&ptid[i],NULL,thr_fun,NULL) != 0) {
            perror("pthread_create");
            exit(3);
        }
    }
    for (i = 0; i< nthreads; i++)
        pthread_join(ptid[i],NULL);

    printf("\nEND!\n");

    return 0;
}

int ex_7_b(int argc, char *argv[]){
    pthread_t *ptid;
	int nthreads;
	int i;

    //Initialize mutex
    if (pthread_mutex_init(&M, NULL) != 0) { 
        printf("\n mutex init has failed\n"); 
        return 1; 
    } 

    setbuf(stdout,NULL);
    if (argc < 2) {
        printf("\nUsage: %s #threads\n", argv[0]);
        exit(1);
    }
    nthreads = atoi(argv[1]);
    ptid = (pthread_t *) malloc(nthreads * sizeof(pthread_t));
    if (ptid == NULL) {
        perror("malloc");
        exit(2);
    }
    for (i = 0; i< nthreads; i++) {
        if(pthread_create(&ptid[i],NULL,thr_fun,NULL) != 0) {
            perror("pthread_create");
            exit(3);
        }
    }
    for (i = 0; i< nthreads; i++)
        pthread_join(ptid[i],NULL);

    //Destroy mutex
    pthread_mutex_destroy(&M); 
    printf("\nEND!\n");
    printf("\n%ld\n", count);
    return 0;
}

int ex_11(int argc, char *argv[]){
    pthread_t *ptid;
	int nthreads;
	int i;

    //Initialize mutex
    if (pthread_mutex_init(&M, NULL) != 0) { 
        printf("\n mutex init has failed\n"); 
        return 1; 
    } 

    setbuf(stdout,NULL);
    if (argc < 2) {
        printf("\nUsage: %s #threads\n", argv[0]);
        exit(1);
    }
    nthreads = atoi(argv[1]);
    ptid = (pthread_t *) malloc(nthreads * sizeof(pthread_t));
    if (ptid == NULL) {
        perror("malloc");
        exit(2);
    }
    for (i = 0; i< nthreads; i++) {
        if(i%2==0){
            if(pthread_create(&ptid[i],NULL,thr_inc,NULL) != 0) {
            perror("pthread_create");
            exit(3);
        }
        }else{
            if(pthread_create(&ptid[i],NULL,thr_decr,NULL) != 0) {
            perror("pthread_create");
            exit(3);
        }
        }
    }
    for (i = 0; i< nthreads; i++)
        pthread_join(ptid[i],NULL);

    //Destroy mutex
    pthread_mutex_destroy(&M); 
    printf("\nEND!\n");
    printf("\n%ld\n", count);
    return 0;
}

int main(int argc, char *argv[]) {
	
    //ex_7
    //ex_7(argc,argv);
    //a) tp4cont | uniq -d ou -c
    //ex_7_b(argc,argv);

    //ex_11
    ex_11(argc,argv);

    return 0;
}


                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          