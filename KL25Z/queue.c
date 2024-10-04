#include "MKL25Z4.h"                    // Device header
#include "queue.h"

void initQueue(CircularBuffer *q)
{
	unsigned int i;
	for (i = 0; i < BUFFER_SIZE; i++)
	{
		q->buffer[i] = 0; // to simplify debugging
	}
	q->head = 0;
	q->tail = 0;
	q->size = 0;
}

int isEmpty(CircularBuffer *q)
{
	return q->size == 0;
}

int isFull(CircularBuffer *q)
{
	return q->size == BUFFER_SIZE;
}

void buffer_put(CircularBuffer *q, uint8_t data) {
	if (!isFull(q)) {
		q->buffer[q->head] = data;
		q->head = (q->head + 1) % BUFFER_SIZE;
		q->size++;
	}
	// Can return 1 for success or 0 for failure in the future
}

uint8_t buffer_get(CircularBuffer *q) {
	uint8_t data;
	if (!isEmpty(q)) {
		data = q->buffer[q->tail];
		q->buffer[q->tail] = 0;
		q->tail = (q->tail + 1) % BUFFER_SIZE;
		q->size--;
	}
	return data;
}
