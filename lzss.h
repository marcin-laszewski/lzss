#if !defined(LZSS_H)
#define	LZSS_H

#include <stdio.h>

#define EI 11			/* typically 10..13 */
#define EJ  4			/* typically 4..5 */
#define N (1 << EI)				/* buffer size */
#define F ((1 << EJ) + 1)	/* lookahead buffer size */

#if defined(__cplusplus)
extern "C" {
#endif

void lzss_decode(FILE *infile, FILE *outfile);
void lzss_encode(FILE *infile, FILE *outfile);

#if defined(__cplusplus)
}
#endif

extern unsigned char buffer[N * 2];

#endif
