#include "MKL25Z4.h"                    // Device header

#define BUFFER_SIZE 64

typedef struct {
	volatile uint8_t buffer[BUFFER_SIZE];
	volatile uint16_t head;
	volatile uint16_t tail;
	volatile uint16_t size;
} CircularBuffer;

void initQueue(CircularBuffer *q);

int isEmpty(CircularBuffer *q);

int isFull(CircularBuffer *q);

void buffer_put(CircularBuffer *q, uint8_t data);

uint8_t buffer_get(CircularBuffer *q);
