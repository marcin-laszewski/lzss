#include <stdio.h>

#include <lzss.h>

int main()
{
	struct lzss_decode_stream f;
	struct lzss_decode l;
	int c;

	lzss_decode_init(&l);
	lzss_decode_open(&f);

	while ((c = lzss_decode_get(&f, &l, lzss_get(fgetc), stdin)) >= 0)
		putchar(c);

	return 0;
}
