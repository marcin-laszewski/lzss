#include <lzss.h>

/* get n bits */
static int
getbit(FILE * infile, int n)
{
	int i, x;
	static int buf, mask = 0;

	x = 0;
	for (i = 0; i < n; i++)
	{
		if (mask == 0)
		{
			if ((buf = fgetc(infile)) == EOF)
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
lzss_decode(FILE *infile, FILE *outfile)
{
	int i, j, k, r, c;

	for (i = 0; i < N - F; i++)
		buffer[i] = ' ';

	r = N - F;

	while ((c = getbit(infile, 1)) != EOF)
	{
		if (c)
		{
			if ((c = getbit(infile, 8)) == EOF)
				break;

			fputc(c, outfile);
			buffer[r++] = c;
			r &= (N - 1);
		}
		else
		{
			if ((i = getbit(infile, EI)) == EOF)
				break;

			if ((j = getbit(infile, EJ)) == EOF)
				break;

			for (k = 0; k <= j + 1; k++)
			{
				c = buffer[(i + k) & (N - 1)];
				fputc(c, outfile);
				buffer[r++] = c;
				r &= (N - 1);
			}
		}
	}
}
