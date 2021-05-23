#include "../include/queue.h"

void enqueue(queue *q, struct message value) {
    node* new_node = (node*)malloc(sizeof(node));
    if (new_node == NULL)
        return;

    new_node->data = value;
    new_node->next = NULL;

    if (q->tail != NULL)
        q->tail->next = new_node;

    q->tail = new_node;

    if (q->head == NULL)
        q->head = new_node;
}

struct message dequeue(queue *q) {
    node* tmp = q->head;
    struct message msg = q->head->data;

    q->head = q->head->next;
    if (q->head == NULL)
        q->tail = NULL;

    free(tmp);
    return msg;
}
