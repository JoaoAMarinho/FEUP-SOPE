#ifndef REQUEST_H_
#define REQUEST_H_

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>

#include "./queue.h"
#include "./logs.h"
#include "./utils.h"

#include "../lib/lib.h"

/**
 * @brief This is the entry function for the consumer thread
 *        Here is where the messages are dequeued after being processed
 *        by the producer threads and the result is sent back to the client
 * 
 * @param arg [NOT USED] 
 * @return void* [IGNORED]
 */
void* buffer_reader(void* arg);
/**
 * @brief This is the entry function for the producer thread
 *        Here is where the messages are processed and enqueued, to later
 *        being sent back to the client by the producer thread
 * 
 * @param message Points to the location in heap where the message is.
 *                In the end of execution this pointer will be freed and
 *                the contents of the message will remain in the queue
 * @return void* [IGNORED]
 */
void* request_handler(void* message);

#endif  // REQUEST_H_
