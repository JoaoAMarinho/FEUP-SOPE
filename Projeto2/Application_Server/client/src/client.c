#include "../include/client.h"

tm_t tm_tracker;
int srv_open = 1;
pthread_mutex_t request_prep = PTHREAD_MUTEX_INITIALIZER;

time_t parse_time(char* argv[]) {
    const char* ERR_FORMAT = "[Client] Invalid Parameter <-t nsecs> -%s\n";

    if (strncmp(argv[1], "-t", strlen(argv[1])) != 0) {
        fprintf(stderr, ERR_FORMAT, argv[1]);
        exit(EXIT_FAILURE);
    }

#define DECIMAL 10
    char* p_garbage;
    const time_t end_time = tm_tracker.begin +
                            strtoul(argv[2], &p_garbage, DECIMAL);
#undef DECIMAL

    if (strncmp(p_garbage, "", strlen(p_garbage)) != 0) {
        fprintf(stderr, ERR_FORMAT, p_garbage);
        exit(EXIT_FAILURE);
    }

    return end_time;
}

int main(int argc, char* argv[]) {
    tm_tracker.begin = time(NULL);
    tm_tracker.rng_seed = tm_tracker.begin;

    if (argc != 4) {
        /* exit error - invalid number of arguments */
        fprintf(stderr, "Usage: c <-t nsecs> <fifoname>\n");
        exit(1);
    }

    tm_tracker.end = parse_time(argv);
    char* pub_path = argv[3];

    unsigned int num_threads = 1024;
    pthread_t* ids = (pthread_t*) malloc(num_threads * sizeof(pthread_t));

    int pub_fd;
    while ((pub_fd = open((const char *)pub_path, O_WRONLY)) < 0 &&
            !TIMEOUT) {}

    if (TIMEOUT) {
        fprintf(stderr, "[Client] Public fifo is closed.\n");
        exit(1);
    }

    unsigned int cnt_thread = 0;
    for (; !TIMEOUT && srv_open; cnt_thread++) {
        if (cnt_thread + 1 == num_threads) {
            num_threads *= 2;
            ids = (pthread_t*) realloc(ids, num_threads * sizeof(pthread_t));
        }

        int err;
        err = pthread_create(&ids[cnt_thread], NULL, request_handler, &pub_fd);
        if (err) {
            fprintf(stderr, "[Client] Error Creating Thread: %s - %d\n",
                            strerror(err), err);
        }

#define SLEEP_MAX (unsigned int)1e4
        /* random interval between 0 and 10 miliseconds */
        usleep(rand_r(&tm_tracker.rng_seed) % SLEEP_MAX);
#undef SLEEP_MAX
    }

    /* Free all the thread currently in busy waiting */
    unsigned int thread_col;
    if (srv_open) {
        for (thread_col = 0; thread_col < cnt_thread; ++thread_col) {
            pthread_cancel(ids[thread_col]);
        }
    }

    for (thread_col = 0; thread_col < cnt_thread; ++thread_col) {
        int err;
        err = pthread_join(ids[thread_col], NULL);

        if (err) {
            fprintf(stderr, "[Client] Error Joining Threads: %s\n",
                    strerror(err));
        }
    }

    close(pub_fd);
    pthread_mutex_destroy(&request_prep);

    free(ids);

    return 0;
}
