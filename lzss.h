#if !defined(LZSS_H)
#define	LZSS_H

#if !defined(lzss_EI)
#	define lzss_EI 11			/* typically 10..13 */
#endif
#if !defined(lzss_EJ)
#	define lzss_EJ  4			/* typically 4..5 */
#endif

#define lzss_N  (1 << lzss_EI)			/* buffer size */
#define lzss_F ((1 << lzss_EJ) + 1)	/* lookahead buffer size */

#define	lzss_BUFLEN	(lzss_N << 1)

#define	lzss_OK	0
#define	lzss_EOF		(-1)
#define	lzss_OUTPUT	(-2)

#define	lzss_is_error(a)	((a) < 0)
#define	lzss_is_ok(a)			(!lzss_is_error(a))

#define	lzss_get(f)	((int(*)(void *))(f))
#define	lzss_put(f)	((int(*)(int, void*))(f))

struct lzss_decode
{
	int buf;
	int mask;
};

struct lzss_encode
{
	unsigned long codecount;
	unsigned long textcount;
	int bit_buffer;
	int bit_mask;
};

struct lzss_stream
{
	int r;
	int i;
	int j;
	int k;
#if defined(lzss_buf_INTERNAL)
	unsigned char buffer[lzss_BUFLEN];
#else
	unsigned char *buffer;
#endif
};

#if defined(__cplusplus)
extern "C" {
#endif

int lzss_decode(struct lzss_decode *l,
        int (*get)(void *), void *gd,
        int (*put)(int, void *), void *pd);
void lzss_decode_init(struct lzss_decode *l);

void lzss_stream_open(struct lzss_stream *f);
int lzss_stream_get(struct lzss_stream *f,
			struct lzss_decode *l,
			int (*get)(void *), void *gd);

int lzss_encode(struct lzss_encode * l,
        int (*get)(void *), void *gd,
        int (*put)(int, void *), void *pd);
void lzss_encode_init(struct lzss_encode * l);

int lzss_getbit(struct lzss_decode *l, int (*get)(void *), void *gd, int n);

#if defined(__cplusplus)
}
#endif

extern unsigned char buffer[lzss_N * 2];

#endif
