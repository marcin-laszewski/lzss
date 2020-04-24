#include <lzss.h>

#define P   1			/* If match length <= P then output one character */

unsigned long codecount = 0;
unsigned long textcount = 0;
int bit_buffer = 0;
int bit_mask = 128;

static int
putbit1(int (* put)(int c, void *), void *pd)
{
	bit_buffer |= bit_mask;

	if ((bit_mask >>= 1) == 0)
	{
		if (put(bit_buffer, pd) < 0)
			return lzss_OUTPUT;

		bit_buffer = 0;
		bit_mask = 128;
		codecount++;
	}

	return lzss_OK;
}

static int
putbit0(int (* put)(int c, void *), void *pd)
{
	if ((bit_mask >>= 1) == 0)
	{
		if (put(bit_buffer, pd) < 0)
			return lzss_OUTPUT;

		bit_buffer = 0;
		bit_mask = 128;
		codecount++;
	}

	return lzss_OK;
}

static int
output1(int (* put)(int c, void *), void *pd, int c)
{
	int mask;

	if (putbit1(put, pd) < 0)
		return lzss_OUTPUT;

	mask = 256;

	while (mask >>= 1)
	{
		if (c & mask)
		{
			if (putbit1(put, pd) < 0)
				return lzss_OUTPUT;
		}
		else
		{
			if (putbit0(put, pd) < 0)
				return lzss_OUTPUT;
		}
	}

	return lzss_OK;
}

static int
output2(int (* put)(int c, void *), void *pd, int x, int y)
{
	int mask;

	if (putbit0(put, pd) < 0)
		return lzss_OUTPUT;

	mask = N;

	while (mask >>= 1)
	{
		if (x & mask)
		{
			if (putbit1(put, pd) < 0)
				return lzss_OUTPUT;
		}
		else
		{
			if (putbit0(put, pd) < 0)
				return lzss_OUTPUT;
		}
	}

	mask = (1 << EJ);

	while (mask >>= 1)
	{
		if (y & mask)
		{
			if (putbit1(put, pd) < 0)
				return lzss_OUTPUT;
		}
		else
		{
			if (putbit0(put, pd) < 0)
				return lzss_OUTPUT;
		}
	}

	return lzss_OK;
}

static int
flush_bit_buffer(int (* put)(int c, void *), void *pd)
{
	if (bit_mask != 128)
	{
		if (put(bit_buffer, pd) < 0)
			return lzss_OUTPUT;

		codecount++;
	}

	return lzss_OK;
}

int
lzss_encode(int (*get)(void *), void *gd, int (* put)(int c, void *), void *pd)
{
	int i, j, f1, x, y, r, s, bufferend, c;

	for (i = 0; i < N - F; i++)
		buffer[i] = ' ';

	for (i = N - F; i < N * 2; i++)
	{
		if ((c = get(gd)) < 0)
			break;
		buffer[i] = c;
		textcount++;
	}

	bufferend = i;
	r = N - F;
	s = 0;

	while (r < bufferend)
	{
		f1 = (F <= bufferend - r) ? F : bufferend - r;
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
			if (output1(put, pd, c) < 0)
				return lzss_OUTPUT;
		}
		else
		{
			if (output2(put, pd, x & (N - 1), y - 2) < 0)
				return lzss_OUTPUT;
		}

		r += y;
		s += y;

		if (r >= N * 2 - F)
		{
			for (i = 0; i < N; i++)
				buffer[i] = buffer[i + N];

			bufferend -= N;

			r -= N;
			s -= N;

			while (bufferend < N * 2)
			{
				if ((c = get(gd)) < 0)
					break;

				buffer[bufferend++] = c;
				textcount++;
			}
		}
	}

	return flush_bit_buffer(put, pd);
}
