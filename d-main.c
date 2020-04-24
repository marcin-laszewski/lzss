#include <stdio.h>

#include <lzss.h>

int main(int argc, char *argv[])
{
	struct lzss_decode l;

	lzss_decode_init(&l);
	return lzss_decode(&l, lzss_get(fgetc), stdin, lzss_put(fputc), stdout) < 0
		? 1
		: 0;
}
