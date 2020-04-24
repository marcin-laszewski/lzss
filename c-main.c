#include <lzss.h>

int main(int argc, char *argv[])
{
	lzss_encode(stdin, stdout);
	return 0;
}
