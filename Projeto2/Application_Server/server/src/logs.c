#include "../include/logs.h"

static const char* SERVER_OPS[] = {
    "RECVD",
    "TSKEX",
    "TSKDN",
    "FAILD",
    "2LATE"
};

void print_op(const struct message* msg, server_op_t op) {
    const char* MSG_OUTPUT_FORMAT = "%ld ; %d ; %d ; %d ; %lu ; %d ; %s\n";
    time_t inst = time(NULL);

    fprintf(stdout, MSG_OUTPUT_FORMAT, inst,
                                       msg->rid,
                                       msg->task_load,
                                       getpid(),
                                       pthread_self(),
                                       msg->task_res,
                                       SERVER_OPS[op]);
}
