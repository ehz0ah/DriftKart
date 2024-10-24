#include "MKL25Z4.h" // Device header
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
	if ((q->size == 0) && (q->head == q->tail)) {
		return 1;
	}
	return 0;
}

int isFull(CircularBuffer *q)
{
	if ((q->size == BUFFER_SIZE) && (q->head == q->tail)) {
		return 1;
	}
	return 0;
}

void buffer_put(CircularBuffer *q, uint8_t data)
{
	if (!isFull(q))
	{
		q->buffer[q->head] = data;
		q->head = (q->head + 1) % BUFFER_SIZE;
		q->size++;
	}
	// Can return 1 for success or 0 for failure in the future
}

uint8_t buffer_get(CircularBuffer *q)
{
	uint8_t data = 0;
	if (!isEmpty(q))
	{
		data = q->buffer[q->tail];
		q->buffer[q->tail] = 0;
		q->tail = (q->tail + 1) % BUFFER_SIZE;
		q->size--;
	}
	return data;
}

void parseData(CircularBuffer* queue, int* speed, uint8_t* command, uint8_t* spin, osEventFlagsId_t runEndEvent) {
	uint8_t data = buffer_get(queue);
	*command = (data >> 4) & 0x0F;
	uint8_t throttle = (data >> 3) & 0x01;
	uint8_t run = (data >> 2) & 0x01;
	*spin = (data >> 1) & 0x01;
	if (throttle) {
		*speed = FULL_SPEED;
	} else {
		*speed = SLOW_SPEED;  // 60%
	}
	if (run) {
		osEventFlagsClear(runEndEvent, 0x02);
		osEventFlagsSet(runEndEvent, 0x01);  // bit 0 controls playing main music
	} else {
		osEventFlagsClear(runEndEvent, 0x01);
		osEventFlagsSet(runEndEvent, 0x02);  // bit 1 controls playing of end music
	}
	
}
