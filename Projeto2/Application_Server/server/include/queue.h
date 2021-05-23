#ifndef QUEUE_H_
#define QUEUE_H_

/**
 * @file queue.h
 * @brief Very Simple Impelementation of a FIFO like Structure
 *        This takes as basis a linked list in which messages are
 *        stored in order of their arrival.
 * @remarks This is a very poor implementation of FIFO, it takes a lot
 *          of shortcuts, keep in mind that there isn't no size nor count
 *          of the number of stored elements for example.
 *          That decision occurs from the fact that it isn't strictly necessary
 *          since semaphores do that tracking for us.
 * 
 * @author This implementation is based on the one made by Jacob Sorber in its
 *         Youtube channel - with a few tweaks.
 * @link https://www.youtube.com/watch?v=FcIubL92gaI&ab_channel=JacobSorber
 */

#include <stdlib.h>
#include "./utils.h"  // struct message definition

typedef struct node {
    struct message data;    /* message data for a given entry */
    struct node* next;      /* next queue entry */
} node;

typedef struct queue {
    node* head;             /* head - read only end */
    node* tail;             /* tail - write only end */
} queue;

#define QUEUE_INITIALIZER { .head = NULL, .tail = NULL }

/**
 * @brief enqueues a message in the FIFO
 * 
 * @param q FIFO reference
 * @param value message to be enqueued
 */
void enqueue(queue *q, struct message value);
/**
 * @brief retrieves and removes a message in the FIFO, 
 *        this function should only be called if we are
 *        certain that the FIFO isn't empty, otherwise
 *        its behaviour might be undefined
 * 
 * @param q FIFO reference
 * @return struct message filled with the data in the FIFO's Head
 */
struct message dequeue(queue *q);

#endif  // QUEUE_H_
