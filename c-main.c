#include <stdio.h>

#include <lzss.h>

int main(int argc, char *argv[])
{
	struct lzss_encode l;

	lzss_encode_init(&l);
	return lzss_encode(&l, lzss_get(fgetc), stdin, lzss_put(fputc), stdout) < 0
		? 1
		: 0;
}
