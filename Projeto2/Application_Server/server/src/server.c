#include "../include/server.h"

tm_t tm_tracker;

queue q = QUEUE_INITIALIZER;

pthread_mutex_t m_enqueue = PTHREAD_MUTEX_INITIALIZER;
sem_t full_queue;
sem_t empty_queue;


time_t parse_time(char* argv[]) {
    const char* ERR_FORMAT = "[Server] Invalid Parameter <-t nsecs> -%s\n";

    if (strncmp(argv[1], "-t", strlen(argv[1])) != 0) {
        fprintf(stderr, ERR_FORMAT, argv[1]);
        exit(EXIT_FAILURE);
    }

#define DECIMAL 10
    char* p_garbage;
    const time_t end_time = tm_tracker.begin +
                            strtoul(argv[2], &p_garbage, DECIMAL);

    if (strncmp(p_garbage, "", strlen(p_garbage)) != 0) {
        fprintf(stderr, ERR_FORMAT, p_garbage);
        exit(EXIT_FAILURE);
    }

    return end_time;
}

unsigned int parse_bufsz(char* argv[]) {
    const char* ERR_FORMAT = "[Server] Invalid Parameter [-l bufsz] -%s\n";

    if (strncmp(argv[3], "-l", strlen(argv[3])) != 0) {
        fprintf(stderr, ERR_FORMAT, argv[1]);
        exit(EXIT_FAILURE);
    }

    char* p_garbage;
    const unsigned int buf_size = strtoul(argv[4], &p_garbage, DECIMAL);
#undef DECIMAL

    if (strncmp(p_garbage, "", strlen(p_garbage)) != 0) {
        fprintf(stderr, ERR_FORMAT, p_garbage);
        exit(EXIT_FAILURE);
    }

    return buf_size;
}


int main(int argc, char* argv[]) {
    if (argc != 4 && argc != 6) {
        /* exit error - invalid number of arguments */
        fprintf(stderr, "Usage: s <-t nsecs> [-l bufsz] <fifoname>\n");
        exit(1);
    }

    tm_tracker.begin = time(NULL);
    tm_tracker.end = parse_time(argv);
    unsigned int buf_size = (argc == 6) ? parse_bufsz(argv) : 1;

    sem_init(&full_queue, 0, 0);
    sem_init(&empty_queue, 0, buf_size);

    unsigned int fifo_index = (argc == 6) ? 5 : 3;
    const char* pub_path = argv[fifo_index];
    mode_t pub_mod = S_IRUSR | S_IWUSR | S_IRWXG | S_IRGRP | S_IROTH | S_IWOTH;
    if (mkfifo(pub_path, pub_mod) == -1 && errno != EEXIST) {
        fprintf(stderr, "[Server] Invalid public FIFO\n [Server] Exiting...");
        exit(EXIT_FAILURE);
    }

    /* Consumer Thread Instantiation */
    pthread_t consumer;
    pthread_create(&consumer, NULL, buffer_reader, NULL);

    /* Thread Pool */
    unsigned int num_threads = 1024;
    pthread_t* ids = (pthread_t*)malloc(num_threads * sizeof(pthread_t));

    int pub_fd;
    while ((pub_fd = open(pub_path, O_RDONLY)) < 0 && !TIMEOUT(1)) {}

    struct message* msg;
    unsigned int cnt_thread = 0;

    while (!TIMEOUT(1)) {
        msg = (struct message *)malloc(sizeof(struct message));
        /*
         * Brief explanation for this is that after syncinc if the client
         * stops to send requests then this will return 0 bytes meaning that
         * aren't more requests from the client side
         */
        if (read(pub_fd, msg, sizeof(struct message)) <= 0) {
            /* Avoiding memory leaks here */
            free(msg);

            /* Server must run until the end... */
            fprintf(stderr, "[Server] Waiting for messages to arrive...\n");
            continue;
        }

        print_op(msg, RECVD);

        if ((cnt_thread + 1) == num_threads) {
            num_threads *= 2;
            ids = (pthread_t*)realloc(ids, num_threads * sizeof(pthread_t));
        }

        int err;
        err = pthread_create(&ids[cnt_thread], NULL, request_handler, msg);
        cnt_thread += 1;

        if (err) {
            fprintf(stderr, "[Server] Error create producer thread: %s - %d\n",
                    strerror(err), err);
        }
    }

    close(pub_fd);

    for (unsigned int thread_col = 0; thread_col < cnt_thread; ++thread_col) {
        int err;
        err = pthread_join(ids[thread_col], NULL);

        if (err) {
            fprintf(stderr, "[Server] Error join producer threads: %s\n",
                    strerror(err));
        }
    }

    free(ids);

    int queue_not_empty = 1;
    while (queue_not_empty) {
        pthread_mutex_lock(&m_enqueue);
        sem_getvalue(&full_queue, &queue_not_empty);
        pthread_mutex_unlock(&m_enqueue);

#define SLEEP_TIME_BETWEEN_TRIES 500
        usleep(SLEEP_TIME_BETWEEN_TRIES);
#undef SLEEP_TIME_BETWEEN_TRIES
    }

    if (pthread_cancel(consumer))
        fprintf(stderr, "[Server] Error canceling consumer thread\n");

    if (pthread_join(consumer, NULL))
        fprintf(stderr, "[Server] Error join consumer thread\n");

    sem_destroy(&full_queue);
    sem_destroy(&empty_queue);
    pthread_mutex_destroy(&m_enqueue);

    unlink(pub_path);
    return 0;
}
