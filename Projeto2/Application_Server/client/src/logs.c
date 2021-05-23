#include "../include/logs.h"

static const char* CLIENT_OPS[] = {
    "IWANT",
    "GOTRS",
    "CLOSD",
    "GAVUP"
};

void print_op(const struct message* msg, request_client_op_t op) {
    const char* MSG_OUTPUT_FORMAT = "%ld ; %d ; %d ; %d ; %lu ; %d ; %s\n";
    time_t inst = time(NULL);

    fprintf(stdout, MSG_OUTPUT_FORMAT, inst,
                                       msg->rid,
                                       msg->task_load,
                                       msg->pid,
                                       msg->tid,
                                       msg->task_res,
                                       CLIENT_OPS[op]);
}
