#ifndef UTILS_H_
#define UTILS_H_

#include <pthread.h>

#define TIMEOUT (time(NULL) > tm_tracker.end)

typedef struct {
    unsigned int rng_seed;  /* random number generator seed */
    time_t begin;           /* initial instant of execution */
    time_t end;             /* final instant of execution */
} tm_t;

typedef enum request_client_op {
    IWANT, /* initial Request */
    GOTRS, /* got result */
    CLOSD, /* closed server */
    GAVUP  /* timeout */
} request_client_op_t;

struct message {
    int rid;        /* request id */
    pid_t pid;      /* process id */
    pthread_t tid;  /* thread id */
    int task_load;  /* task load */
    int task_res;   /* task result */
};

#endif  // UTILS_H_
