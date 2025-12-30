/*
 * queue.c
 *
 *  Created on: Dec 30, 2025
 *      Author: Hoseung Yoon
 */


#include "queue.h"

void Que_Init(que_t *que)
{
	que->head = 0;
	que->tail = 0;
	que-> indexCount = 0;
}
int Que_Enque(que_t *que, int data)
{
	 if (Que_isFull(que)) return -1;
	 que->queData[que->tail] = data;
	 que->tail = (que->tail + 1) % 4;
	 que->indexCount++;
}

int Que_Deque(que_t *que)
{
	if (Que_isEmpty(que)) return -1;
	int temp;
	temp = que->queData[que->head];
	que->head = (que->head + 1) % 4;
	que->indexCount--;
	return temp;
}

bool Que_isFull(que_t *que)
{
	return que->indexCount == 4;
}

bool Que_isEmpty(que_t *que)
{
	return que->indexCount == 0;
}
