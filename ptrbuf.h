#ifndef _djgkjdioweoif_ptrbuf_h_included
#define _djgkjdioweoif_ptrbuf_h_included

typedef struct ptrbuf ptrbuf_t;

#ifndef _PTRBUF_PROTO
#define _PTRBUF_PROTO 
#endif

_PTRBUF_PROTO ptrbuf_t *ptrbuf_new(size_t blksize);
_PTRBUF_PROTO void ptrbuf_free(ptrbuf_t *B, void ***Rd, size_t *Rl);

_PTRBUF_PROTO void ptrbuf_collect(ptrbuf_t *B, void ***Rd, size_t *Rl);
_PTRBUF_PROTO size_t ptrbuf_count(ptrbuf_t *B);

     // always puts to the end of the buffer
_PTRBUF_PROTO void ptrbuf_put(ptrbuf_t *B, void *d);

     // gets from the start of the buffer
_PTRBUF_PROTO void* ptrbuf_get(ptrbuf_t *B);

     // gets from the end of the buffer
_PTRBUF_PROTO void* ptrbuf_pop(ptrbuf_t *B);


#endif
