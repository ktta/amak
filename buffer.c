#include <stdlib.h>
#include <string.h>
#include "buffer.h"


struct buffer_node { 
  struct buffer_node *next;
  size_t len,size,start;
  unsigned char data[1];
};

struct buffer {
  struct buffer_node *first, *last;
  size_t blksize;
};



_BUFFER_PROTO buffer_t *buffer_new(size_t blksize)
{
  buffer_t *B;
  B= malloc(sizeof(*B));
  B->first= B->last= NULL;
  B->blksize= blksize;
  return B;
}


_BUFFER_PROTO void buffer_free(buffer_t *B, void **Rd, size_t *Rl)
{
  struct buffer_node *P,*N;
  if (Rd || Rl) buffer_collect(B, Rd, Rl);
  for(P=B->first;P;P=N)
  {
    N= P->next;
    free(P);
  }
  free(B);
}

_BUFFER_PROTO void buffer_collect(buffer_t *B, void **Rd, size_t *Rl)
{
  size_t S;
  struct buffer_node *P;
  unsigned char *R;

  if (!Rd && !Rl) return ;

  S= 0;
  for(P=B->first;P;P=P->next) S+= P->len;

  if (S==0) { if (Rd) *Rd= NULL; if (Rl) *Rl= 0; return ; }
  if (!Rd) { *Rl= S; return ; }

  R= malloc(S);
  S= 0;
  for(P=B->first;P;P=P->next)
  {
    memcpy(R+S, P->data+P->start, P->len);
    S+= P->len;
  }

  *Rd= (void*) R;
  if (Rl) *Rl= S;
}

_BUFFER_PROTO void buffer_write(buffer_t *B, void *d, size_t l)
{
  struct buffer_node *P;
  size_t cp;

  while(l)
  {
    P= B->last;
    if (!P || (P->start+P->len)==P->size)
    {
      P= malloc(sizeof(*P)+B->blksize-1);
      P->len= 0;
      P->next= NULL;
      P->size= B->blksize;
      P->start= 0;
      if (!B->last) { B->last= B->first= P; }
      else { B->last->next= P; B->last= P; }
    }

    cp= P->size-P->len-P->start;
    if (cp>l) cp= l; 

    memcpy(P->data+P->len+P->start, d, cp);
    P->len+= cp;
    l-= cp;
    d= (unsigned char*) d + cp;
  }
}

_BUFFER_PROTO void buffer_append(buffer_t *dst, buffer_t *src_f)
{
  if (src_f->first!=NULL)
  {
    if (dst->first==NULL)
    {
      dst->first= src_f->first;
      dst->last= src_f->last;
    }
    else
    {
      dst->last->next= src_f->first;
      dst->last= src_f->last;
    }
  }
  free(src_f);
}



_BUFFER_PROTO size_t buffer_read(buffer_t *B, void *d, size_t l)
{
  size_t total;
  struct buffer_node *P;
  size_t cp;
  total= 0;

  while(l)
  {
    P= B->first;
    if (!P) break;  
    if (P->len==0)
    {
      B->first= P->next;
      if (!B->first) B->last= NULL;
      free(P);
      continue;
    } 
    cp= P->len;
    if (cp>l) cp= l;
    memcpy(d, P->data+P->start, cp);
    P->start+= cp;
    P->len-= cp;
    total+= cp;
    d= (unsigned char*) d + cp;
  }
  return total;
}


