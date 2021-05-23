#ifndef SERVER_H_
#define SERVER_H_

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <errno.h>

#include "./queue.h"

#include "./logs.h"
#include "./request.h"

/* PARSING FUNCTIONS */

/**
 * @brief Parses the Time argument and returns the expected finish time
 *
 * @param argv comand line arguments value
 * @return const time_t expected finish time
 */
time_t parse_time(char* argv[]);
/**
 * @brief Parses the optional Buffer size argument and returns its value
 *
 * @param argv @param argv comand line arguments value
 * @return const unsigned int with buffer size value
 */
unsigned int parse_bufsz(char* argv[]);

/* SERVER ENTRY POINT */

/**
 * @brief Main function that executes the fundamental operations for the server
 * 
 * @param argc command line arguments count 
 * @param argv comand line arguments value
 */
int main(int argc, char* argv[]);

#endif  // SERVER_H_
