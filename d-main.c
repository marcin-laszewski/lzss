#include <lzss.h>

int main(int argc, char *argv[])
{
	lzss_decode(stdin, stdout);
	return 0;
}
