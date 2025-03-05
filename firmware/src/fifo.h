#ifndef __bfifo_H
#define __bfifo_H
#include <stdint.h>

typedef struct {
	uint8_t *pIn;
	uint8_t *pOut;
	uint8_t *pStart;
	uint8_t *pEnd;
} BFIFO;

extern void bfifo_init(BFIFO *fifo, uint8_t *buffer, unsigned int elemno);
extern int bfifo_put(BFIFO *fifo, uint8_t src);
extern int bfifo_get(BFIFO *fifo);
extern int bfifo_full(BFIFO *fifo);
extern int bfifo_empty(BFIFO *fifo);

#endif
