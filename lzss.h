#if !defined(LZSS_H)
#define	LZSS_H

#define EI 11			/* typically 10..13 */
#define EJ  4			/* typically 4..5 */
#define N (1 << EI)				/* buffer size */
#define F ((1 << EJ) + 1)	/* lookahead buffer size */

#define	lzss_OK	0
#define	lzss_OUTPUT	(-2)

#define	lzss_is_error(a)	((a) < 0)
#define	lzss_is_ok(a)			(!lzss_is_error(a))

#define	lzss_get(f)	((int(*)(void *))(f))
#define	lzss_put(f)	((int(*)(int, void*))(f))

#if defined(__cplusplus)
extern "C" {
#endif

int lzss_decode(
        int (*get)(void *), void *gd,
        int (*put)(int, void *), void *pd);
int lzss_encode(
        int (*get)(void *), void *gd,
        int (*put)(int, void *), void *pd);

#if defined(__cplusplus)
}
#endif

extern unsigned char buffer[N * 2];

#endif
