#include <stdio.h>
#include <stdlib.h>

#include <lzss.h>

#define P   1			/* If match length <= P then output one character */

unsigned long codecount = 0;
unsigned long textcount = 0;
int bit_buffer = 0;
int bit_mask = 128;

static void
error(void)
{
	printf("Output error\n");
	exit(1);
}

static void
putbit1(int (* put)(int c, void *), void *pd)
{
	bit_buffer |= bit_mask;

	if ((bit_mask >>= 1) == 0)
	{
		if (put(bit_buffer, pd) == EOF)
			error();

		bit_buffer = 0;
		bit_mask = 128;
		codecount++;
	}
}

static void
putbit0(int (* put)(int c, void *), void *pd)
{
	if ((bit_mask >>= 1) == 0)
	{
		if (put(bit_buffer, pd) == EOF)
			error();

		bit_buffer = 0;
		bit_mask = 128;
		codecount++;
	}
}

static void
output1(int (* put)(int c, void *), void *pd, int c)
{
	int mask;

	putbit1(put, pd);
	mask = 256;

	while (mask >>= 1)
	{
		if (c & mask)
			putbit1(put, pd);
		else
			putbit0(put, pd);
	}
}

static void
output2(int (* put)(int c, void *), void *pd, int x, int y)
{
	int mask;

	putbit0(put, pd);
	mask = N;

	while (mask >>= 1)
	{
		if (x & mask)
			putbit1(put, pd);
		else
			putbit0(put, pd);
	}

	mask = (1 << EJ);

	while (mask >>= 1)
	{
		if (y & mask)
			putbit1(put, pd);
		else
			putbit0(put, pd);
	}
}

static void
flush_bit_buffer(int (* put)(int c, void *), void *pd)
{
	if (bit_mask != 128)
	{
		if (put(bit_buffer, pd) == EOF)
			error();

		codecount++;
	}
}

void
lzss_encode(int (*get)(void *), void *gd, int (* put)(int c, void *), void *pd)
{
	int i, j, f1, x, y, r, s, bufferend, c;

	for (i = 0; i < N - F; i++)
		buffer[i] = ' ';

	for (i = N - F; i < N * 2; i++)
	{
		if ((c = get(gd)) == EOF)
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
			output1(put, pd, c);
		}
		else
			output2(put, pd, x & (N - 1), y - 2);

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
				if ((c = get(gd)) == EOF)
					break;

				buffer[bufferend++] = c;
				textcount++;
			}
		}
	}

	flush_bit_buffer(put, pd);
}
