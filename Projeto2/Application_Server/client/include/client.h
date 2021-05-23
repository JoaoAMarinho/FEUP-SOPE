#ifndef CLIENT_H_
#define CLIENT_H_

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <errno.h>

#include "./logs.h"
#include "./request.h"

/* PARSING FUNCTIONS */

/**
 * @brief Parses the Time argument and returns the expected finish time
 * 
 * @param argc command line arguments count 
 * @param argv comand line arguments value
 * @return const time_t expected finish time
 */
time_t parse_time(char* argv[]);

/* CLIENT ENTRY POINT */

/**
 * @brief Main function that executes the fundamental operations for the client
 * 
 * @param argc command line arguments count 
 * @param argv comand line arguments value
 */
int main(int argc, char* argv[]);

#endif  // CLIENT_H_
