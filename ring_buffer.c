#include "ring_buffer.h"

void ring_buffer_init(ring_buffer *dst_buf)
{
	dst_buf->pw = 0;
	dst_buf->pr = 0;
}

void ring_buffer_write(unsigned char c,ring_buffer *dst_buf)
{
	int i = (dst_buf->pw + 1) % BUFFER_SIZE;
	if(i != dst_buf->pr)
	{
		dst_buf->buffer[dst_buf->pw] = c;
		dst_buf->pw = i;
	}
}

int ring_buffer_read(unsigned char *c,ring_buffer *dst_buf)
{
	if(dst_buf->pr == dst_buf->pw)
	{
		return -1;
	}
	else
	{
		*c = dst_buf->buffer[dst_buf->pr];
		dst_buf->pr = (dst_buf->pr + 1) % BUFFER_SIZE;
		return 0;
	}
}
