#ifndef UTILS_H_
#define UTILS_H_

#include <pthread.h>

/* Server Execution TIMEOUT  */
#define TIMEOUT(secs) (time(NULL) > (tm_tracker.end + secs))

/* Library TIMEOUT */
#define TIMEOUT_LIB (time(NULL) > tm_tracker.end)

typedef struct {
    time_t begin;           /* initial instant of execution */
    time_t end;             /* final instant of execution */
} tm_t;

typedef enum server_op {
    RECVD,  /* received request */
    TSKEX,  /* task executed */
    TSKDN,  /* task done */
    FAILD,  /* task failed */
    LATE    /* timeout - too late */
} server_op_t;

struct message {
    int rid;        /* request id */
    pid_t pid;      /* process id */
    pthread_t tid;  /* thread id */
    int task_load;  /* task load */
    int task_res;   /* task result */
};

#endif  // UTILS_H_
