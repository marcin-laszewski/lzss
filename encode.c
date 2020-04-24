#include <lzss.h>

#define P   1			/* If match length <= P then output one character */

void
lzss_encode_init(struct lzss_encode *l)
{
	l->codecount	= 0;
	l->textcount	= 0;
	l->bit_buffer	= 0;
	l->bit_mask		= 128;
}

static int
putbit1(struct lzss_encode *l, int (* put)(int c, void *), void *pd)
{
	l->bit_buffer |= l->bit_mask;

	if ((l->bit_mask >>= 1) == 0)
	{
		if (put(l->bit_buffer, pd) < 0)
			return lzss_OUTPUT;

		l->bit_buffer = 0;
		l->bit_mask = 128;
		l->codecount++;
	}

	return lzss_OK;
}

static int
putbit0(struct lzss_encode *l, int (* put)(int c, void *), void *pd)
{
	if ((l->bit_mask >>= 1) == 0)
	{
		if (put(l->bit_buffer, pd) < 0)
			return lzss_OUTPUT;

		l->bit_buffer = 0;
		l->bit_mask = 128;
		l->codecount++;
	}

	return lzss_OK;
}

static int
output1(struct lzss_encode *l, int (* put)(int c, void *), void *pd, int c)
{
	int mask;

	if (putbit1(l, put, pd) < 0)
		return lzss_OUTPUT;

	mask = 256;

	while (mask >>= 1)
	{
		if (c & mask)
		{
			if (putbit1(l, put, pd) < 0)
				return lzss_OUTPUT;
		}
		else
		{
			if (putbit0(l, put, pd) < 0)
				return lzss_OUTPUT;
		}
	}

	return lzss_OK;
}

static int
output2(struct lzss_encode *l, int (* put)(int c, void *), void *pd, int x, int y)
{
	int mask;

	if (putbit0(l, put, pd) < 0)
		return lzss_OUTPUT;

	mask = lzss_N;

	while (mask >>= 1)
	{
		if (x & mask)
		{
			if (putbit1(l, put, pd) < 0)
				return lzss_OUTPUT;
		}
		else
		{
			if (putbit0(l, put, pd) < 0)
				return lzss_OUTPUT;
		}
	}

	mask = (1 << lzss_EJ);

	while (mask >>= 1)
	{
		if (y & mask)
		{
			if (putbit1(l, put, pd) < 0)
				return lzss_OUTPUT;
		}
		else
		{
			if (putbit0(l, put, pd) < 0)
				return lzss_OUTPUT;
		}
	}

	return lzss_OK;
}

static int
flush_bit_buffer(struct lzss_encode *l, int (* put)(int c, void *), void *pd)
{
	if (l->bit_mask != 128)
	{
		if (put(l->bit_buffer, pd) < 0)
			return lzss_OUTPUT;

		l->codecount++;
	}

	return lzss_OK;
}

int
lzss_encode(struct lzss_encode *l,
	int (*get)(void *), void *gd,
	int (* put)(int c, void *), void *pd)
{
	unsigned char buffer[lzss_BUFLEN];
	int i, j, f1, x, y, r, s, bufferend, c;

	for (i = 0; i < lzss_N - lzss_F; i++)
		buffer[i] = ' ';

	for (i = lzss_N - lzss_F; i < lzss_N * 2; i++)
	{
		if ((c = get(gd)) < 0)
			break;
		buffer[i] = c;
		l->textcount++;
	}

	bufferend = i;
	r = lzss_N - lzss_F;
	s = 0;

	while (r < bufferend)
	{
		f1 = (lzss_F <= bufferend - r) ? lzss_F : bufferend - r;
		x = 0;
		y = 1;
		c = buffer[r];

		for (i = r - 1; i >= s; i--)
			if (buffer[i] == c)
			{
				for (j = 1; j < f1; j++)
					if (buffer[i + j] != buffer[r + j])
						break;

				if (j > y)
				{
					x = i;
					y = j;
				}
			}

		if (y <= P)
		{
			y = 1;
			if (output1(l, put, pd, c) < 0)
				return lzss_OUTPUT;
		}
		else
		{
			if (output2(l, put, pd, x & (lzss_N - 1), y - 2) < 0)
				return lzss_OUTPUT;
		}

		r += y;
		s += y;

		if (r >= lzss_N * 2 - lzss_F)
		{
			for (i = 0; i < lzss_N; i++)
				buffer[i] = buffer[i + lzss_N];

			bufferend -= lzss_N;

			r -= lzss_N;
			s -= lzss_N;

			while (bufferend < lzss_N * 2)
			{
				if ((c = get(gd)) < 0)
					break;

				buffer[bufferend++] = c;
				l->textcount++;
			}
		}
	}

	return flush_bit_buffer(l, put, pd);
}
