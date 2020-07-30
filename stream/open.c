#include <lzss.h>

void
lzss_stream_open(struct lzss_stream *f)
{
	int i;

	for (i = 0; i < lzss_N - lzss_F; i++)
		f->buffer[i] = (unsigned char)' ';

	f->r = lzss_N - lzss_F;
	f->j = 0;
	f->k = 2;
}
