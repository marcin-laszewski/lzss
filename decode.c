#include <lzss.h>

/* get n bits */
static int
getbit(int (*get)(void *), void *gd, int n)
{
	int i, x;
	static int buf, mask = 0;

	x = 0;
	for (i = 0; i < n; i++)
	{
		if (mask == 0)
		{
			if ((buf = get(gd)) < 0)
				return -1;

			mask = 128;
		}

		x <<= 1;

		if (buf & mask)
			x++;

		mask >>= 1;
	}

	return x;
}

int
lzss_decode(int (*get)(void *), void * gd, int (*put)(int, void *), void *pd)
{
	int i, j, k, r, c;

	for (i = 0; i < N - F; i++)
		buffer[i] = ' ';

	r = N - F;

	while ((c = getbit(get, gd, 1)) >= 0)
	{
		if (c)
		{
			if ((c = getbit(get, gd, 8)) < 0)
				break;

			put(c, pd);
			buffer[r++] = c;
			r &= (N - 1);
		}
		else
		{
			if ((i = getbit(get, gd, EI)) < 0)
				break;

			if ((j = getbit(get, gd, EJ)) < 0)
				break;

			for (k = 0; k <= j + 1; k++)
			{
				c = buffer[(i + k) & (N - 1)];
				put(c, pd);
				buffer[r++] = c;
				r &= (N - 1);
			}
		}
	}

	return lzss_OK;
}
