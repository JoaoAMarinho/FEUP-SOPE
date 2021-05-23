#include "../include/request.h"

static int rid_tracker = 0;

extern pthread_mutex_t request_prep; 
extern int srv_open;
extern tm_t tm_tracker;

/**
 * @brief CLEANUP_ROUTINE for threads, here all the resources taken from the 
 *        system are freed
 * 
 * @param arg clean_up_t with required info to close all the files opened by
 *            a given thread
 * @return void
 */
static void request_cleanup(void* arg) {
    int fd = ((clean_up_t *)arg)->fd;
    if (fd > 0)
        close(fd);

    const char* path = ((clean_up_t *)arg)->path;
    unlink(path);

    /* 
     * The fact that a task result is -1 at this point \
     * indicates that no output has been done to a given \
     * request.
     */
    struct message msg = ((clean_up_t *)arg)->msg;
    if (TIMEOUT && msg.task_res == -1)
        print_op(&msg, GAVUP);
}

void* request_handler(void* fifo_fd) {
    pthread_mutex_lock(&request_prep);
    ++rid_tracker;
    struct message msg = {
        .rid = rid_tracker,
        .pid = getpid(),
        .tid = pthread_self(),
        .task_load = rand_r(&tm_tracker.rng_seed) % 9 + 1,
        .task_res = -1
    };
    pthread_mutex_unlock(&request_prep);

#define PATH_MAX_LEN 256
    char prv_fifo_path[PATH_MAX_LEN];
    snprintf(prv_fifo_path, PATH_MAX_LEN, "/tmp/%d.%lu", msg.pid, msg.tid);
    mode_t prv_mod = S_IRUSR | S_IWUSR | S_IRWXG | S_IRGRP | S_IROTH | S_IWOTH;
#undef PATH_MAX_LEN

    /* Request Initiation */
    print_op(&msg, IWANT);

    if (write((*((int *)fifo_fd)), &msg, sizeof(struct message)) == -1) {
        fprintf(stderr, "[Client] Error writing to public FIFO\n");
    }

    if (mkfifo(prv_fifo_path, prv_mod) == -1) {
        fprintf(stderr, "[Client] Invalid private FIFO");
        pthread_exit(NULL);
    }

    clean_up_t clean_up = {
        .fd = -1,
        .path = prv_fifo_path,
        .msg = msg
    };
    pthread_cleanup_push(request_cleanup, &clean_up);

    struct message answer;
    int answer_fd;
    while ((answer_fd = open(prv_fifo_path, O_RDONLY)) < 0)  {}

    /* Update file descriptor value */
    clean_up.fd = answer_fd;

    if (read(answer_fd, &answer, sizeof(struct message)) == -1) {
        fprintf(stderr, "[Client] Error reading from private FIFO\n");
    }

    if (answer.task_res == -1) {
        srv_open = 0;
        print_op(&answer, CLOSD);
    } else {
        print_op(&answer, GOTRS);
    }

    clean_up.msg.task_res = 0;
    pthread_cleanup_pop(1);
    pthread_exit(NULL);
}
