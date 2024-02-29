#include <stdlib.h>
#include <string.h>
#include "ptrbuf.h"


struct pbufnode { 
  struct pbufnode *next;
  size_t len, start;
  void* data[];
};

struct ptrbuf {
  struct pbufnode *first, *last;
  size_t blksize;
};


_PTRBUF_PROTO ptrbuf_t *ptrbuf_new(size_t blksize)
{
  ptrbuf_t *B;
  B= malloc(sizeof(*B));
  B->first= B->last= NULL;
  B->blksize= blksize;
  return B;
}


_PTRBUF_PROTO void ptrbuf_free(ptrbuf_t *B, void ***Rd, size_t *Rl)
{
  struct pbufnode *P,*N;
  if (Rd || Rl) ptrbuf_collect(B, Rd, Rl);
  for(P=B->first;P;P=N)
  {
    N= P->next;
    free(P);
  }
  free(B);
}

_PTRBUF_PROTO void ptrbuf_collect(ptrbuf_t *B, void ***Rd, size_t *Rl)
{
  size_t S;
  struct pbufnode *P;
  void **R;

  if (!Rd && !Rl) return ;

  S= 0;
  for(P=B->first;P;P=P->next) S+= P->len;

  if (S==0) { if (Rd) *Rd= NULL; if (Rl) *Rl= 0; return ; }
  if (!Rd) { if (Rl) *Rl= S; return ; }

  R= malloc(S*sizeof(void*));
  S= 0;
  for(P=B->first;P;P=P->next)
  {
    memcpy(R+S, P->data+P->start, P->len*sizeof(void*));
    S+= P->len;
  }

  *Rd= R;
  if (Rl) *Rl= S;
}

_PTRBUF_PROTO void ptrbuf_put(ptrbuf_t *B, void *d)
{
  struct pbufnode *P;

  P= B->last;
  if (!P || (P->len+P->start)==B->blksize)
  {
      P= malloc(sizeof(*P)+B->blksize*sizeof(void*));
      P->len= P->start= 0;
      P->next= NULL;
      if (!B->last) { B->last= B->first= P; }
      else { B->last->next= P; B->last= P; }
 }
 P->data[P->len+P->start]= d;
 P->len++;
}

_PTRBUF_PROTO void* ptrbuf_get(ptrbuf_t *B)
{
  struct pbufnode *P;
  void *R;

  P= B->first;
  if (!P) return NULL;
  R= P->data[P->start];
  P->start++;
  P->len--;
  if (P->len==0)
  {
    B->first= P->next;
    if (!B->first) B->last= NULL;
    free(P);
  }
  return R;
}

_PTRBUF_PROTO void* ptrbuf_pop(ptrbuf_t *B)
{
  struct pbufnode *P,**T,*Y;
  void *R;

  P= B->last;
  if (!P) return NULL;
  R= P->data[P->start+P->len-1];
  P->len--;
  if (P->len==0)
  {
    for(Y=B->first,T=&B->first;Y!=P;)
    {
      T= &Y->next;
      Y= Y->next;
    }
    *T= NULL;
    free(P);
    if (!B->first) B->last= NULL;
  }
  return R;
}

_PTRBUF_PROTO size_t ptrbuf_count(ptrbuf_t *B)
{
  size_t R;
  ptrbuf_collect(B, NULL, &R);
  return R;
}
