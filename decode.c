#include <stdio.h>

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
			if ((buf = get(gd)) == EOF)
				return EOF;

			mask = 128;
		}

		x <<= 1;

		if (buf & mask)
			x++;

		mask >>= 1;
	}

	return x;
}

void
lzss_decode(int (*get)(void *), void * gd, int (*put)(int, void *), void *pd)
{
	int i, j, k, r, c;

	for (i = 0; i < N - F; i++)
		buffer[i] = ' ';

	r = N - F;

	while ((c = getbit(get, gd, 1)) != EOF)
	{
		if (c)
		{
			if ((c = getbit(get, gd, 8)) == EOF)
				break;

			put(c, pd);
			buffer[r++] = c;
			r &= (N - 1);
		}
		else
		{
			if ((i = getbit(get, gd, EI)) == EOF)
				break;

			if ((j = getbit(get, gd, EJ)) == EOF)
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
}
