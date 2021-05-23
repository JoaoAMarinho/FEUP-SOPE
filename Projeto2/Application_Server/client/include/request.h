#ifndef REQUEST_H_
#define REQUEST_H_

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>

#include "./utils.h"
#include "./logs.h"

typedef struct {
    int fd;             /* file descriptor */
    char* path;         /* fifo file path */
    struct message msg; /* message to be printed */
} clean_up_t;

/* CLIENT REQUESTS */

/**
 * @brief START_ROUTINE for threads, here the request is sent and received the
 *        the corresponding answer.
 * 
 * @param fifo_fd File descriptor to the public FIFO required to send the requests
 * @return void* Return value is ignored
 */
void* request_handler(void* fifo_fd);

#endif  // REQUEST_H_
