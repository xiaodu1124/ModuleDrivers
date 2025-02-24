#ifndef __RING_BUFFER_H
#define __RING_BUFFER_H

#define BUFFER_SIZE 512

typedef struct 
{
	unsigned char buffer[BUFFER_SIZE];
	volatile unsigned int pw;
	volatile unsigned int pr;
}ring_buffer;

void ring_buffer_init(ring_buffer *dst_buf);
void ring_buffer_write(unsigned char c, ring_buffer *dst_buf);
int ring_buffer_read(unsigned char *c, ring_buffer *dst_buf);

#endif

