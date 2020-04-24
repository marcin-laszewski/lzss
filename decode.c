#include <lzss.h>

void
lzss_decode_init(struct lzss_decode *l)
{
	l->mask = 0;
}

/* get n bits */
static int
getbit(struct lzss_decode *l, int (*get)(void *), void *gd, int n)
{
	int i, x;

	x = 0;
	for (i = 0; i < n; i++)
	{
		if (l->mask == 0)
		{
			if ((l->buf = get(gd)) < 0)
				return -1;

			l->mask = 128;
		}

		x <<= 1;

		if (l->buf & l->mask)
			x++;

		l->mask >>= 1;
	}

	return x;
}

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

	while ((c = getbit(l, get, gd, 1)) >= 0)
	{
		if (c)
		{
			if ((c = getbit(l, get, gd, 8)) < 0)
				break;

			put(c, pd);
			buffer[r++] = c;
			r &= (lzss_N - 1);
		}
		else
		{
			if ((i = getbit(l, get, gd, lzss_EI)) < 0)
				break;

			if ((j = getbit(l, get, gd, lzss_EJ)) < 0)
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
