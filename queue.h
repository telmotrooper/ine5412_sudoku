// Adapted from Tutorials Point
// https://www.tutorialspoint.com/data_structures_algorithms/queue_program_in_c.htm

#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

#define QUEUE_SIZE 243 // number of tasks required for Sudoku

int queueIntArray[QUEUE_SIZE];
int queueFront = 0;
int queueRear = -1;
int queueItemCount = 0;

int queuePeek() { // returns the first value of the queue without removing it
    return queueIntArray[queueFront];
}

bool queueIsEmpty() {
    return queueItemCount == 0;
}

bool queueIsFull() {
    return queueItemCount == QUEUE_SIZE;
}

int queueSize() {
    return queueItemCount;
}

void queueInsert(int data) {

    if(!queueIsFull()) {

        if(queueRear == QUEUE_SIZE-1) {
            queueRear = -1;
        }

        queueIntArray[++queueRear] = data;
        queueItemCount++;
    }
}

int queueRemove() { // removes the first value of the queue
    int data = queueIntArray[queueFront++];

    if(queueFront == QUEUE_SIZE) {
        queueFront = 0;
    }

    queueItemCount--;
    return data;
}

#endif //QUEUE_H
