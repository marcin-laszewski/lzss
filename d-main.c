#include <stdio.h>

#include <lzss.h>

int main(int argc, char *argv[])
{
	lzss_decode(lzss_get(fgetc), stdin, lzss_put(fputc), stdout);
	return 0;
}
