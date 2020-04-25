#include <lzss.h>

int
lzss_decode(struct lzss_decode * l,
	int (*get)(void *), void * gd,
	int (*put)(int, void *), void *pd)
{
	unsigned char buffer[lzss_BUFLEN];
	int i, j, k, r, c;

	for (i = 0; i < lzss_N - lzss_F; i++)
		buffer[i] = ' ';

	r = lzss_N - lzss_F;

	while ((c = lzss_getbit(l, get, gd, 1)) >= 0)
	{
		if (c)
		{
			if ((c = lzss_getbit(l, get, gd, 8)) < 0)
				break;

			put(c, pd);
			buffer[r++] = c;
			r &= (lzss_N - 1);
		}
		else
		{
			if ((i = lzss_getbit(l, get, gd, lzss_EI)) < 0)
				break;

			if ((j = lzss_getbit(l, get, gd, lzss_EJ)) < 0)
				break;

			for (k = 0; k <= j + 1; k++)
			{
				c = buffer[(i + k) & (lzss_N - 1)];
				put(c, pd);
				buffer[r++] = c;
				r &= (lzss_N - 1);
			}
		}
	}

	return lzss_OK;
}
