#include <stdio.h>

#include <lzss.h>

int main(int argc, char *argv[])
{
	return lzss_encode(lzss_get(fgetc), stdin, lzss_put(fputc), stdout) < 0
		? 1
		: 0;
}
