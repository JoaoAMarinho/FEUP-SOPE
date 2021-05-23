#ifndef LOGS_H_
#define LOGS_H_

#include <stdio.h>
#include <time.h>

#include "./utils.h"

/* OUTPUT FUNCTIONS */

/**
 * @brief Prints an operation to the standard output (stdout)
 * 
 * @param msg struct with elements required for a message
 * @param op operation type identifier
 */
void print_op(const struct message* msg, request_client_op_t op);

#endif  // LOGS_H_
