/*
 * queue.h
 *
 *  Created on: Dec 30, 2025
 *      Author: Hoseung Yoon
 */

#ifndef INC_QUEUE_H_
#define INC_QUEUE_H_

#include <stdbool.h>
#include <stdint.h>

// Queue Data Structure
typedef struct {
	int queData[4];
	int head;
	int tail;
	int indexCount;
}que_t;

void Que_Init(que_t *que);
int Que_Enque(que_t *que, int data);
int Que_Deque(que_t *que);
bool Que_isFull(que_t *que);
bool Que_isEmpty(que_t *que);

#endif /* INC_QUEUE_H_ */
