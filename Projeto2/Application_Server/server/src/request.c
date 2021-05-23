#include "../include/request.h"

extern tm_t tm_tracker;

extern queue q;

extern pthread_mutex_t m_enqueue;
extern sem_t full_queue;
extern sem_t empty_queue;


void* buffer_reader(__attribute__((unused)) void* arg) {
    struct message msg;

    while (1) {
        sem_wait(&full_queue);
        pthread_mutex_lock(&m_enqueue);

        msg = dequeue(&q);

        pthread_mutex_unlock(&m_enqueue);
        sem_post(&empty_queue);

#define PATH_MAX_LEN 256
        char prv_fifo_path[PATH_MAX_LEN];
        snprintf(prv_fifo_path, PATH_MAX_LEN, "/tmp/%d.%lu", msg.pid, msg.tid);
#undef PATH_MAX_LEN

        /* 
         * Is this a good approach?
         * If client doens't have the private fifo prepared we just ignore? 
         */
        int answer_fd;
        if ((answer_fd = open(prv_fifo_path, O_WRONLY | O_NONBLOCK)) < 0) {
            fprintf(stderr, "[Server] Error opening private FIFO\n");
            print_op(&msg, FAILD);
            continue;
        }


        msg.pid = getpid();
        msg.tid = pthread_self();
        if (write(answer_fd, &msg, sizeof(struct message)) == -1) {
            fprintf(stderr, "[Server] Error writing to private FIFO\n");
            print_op(&msg, FAILD);
        }

        msg.task_res == -1 ? print_op(&msg, LATE) : print_op(&msg, TSKDN);
    }

    pthread_exit(NULL);
}


void* request_handler(void* message) {
    struct message* msg = (struct message*)message;

    if (!TIMEOUT_LIB) {
        msg->task_res = task(msg->task_load);
        print_op(msg, TSKEX);
    }

    sem_wait(&empty_queue);
    pthread_mutex_lock(&m_enqueue);

    enqueue(&q, *msg);

    pthread_mutex_unlock(&m_enqueue);
    sem_post(&full_queue);

    /* 
     * This thread must be responsible for free this malloc'd
     * struct. This happens because the main thread offers no
     * guarantee that it will not remove it from its stack
     * before the message being enqueued   
     */
    free(msg);
    pthread_exit(NULL);
}
