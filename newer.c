

int reqisnewer(char *tgt, char *req)
{
  struct stat stgt, sreq;
  struct timespec *T, *R;
  if (stat(tgt, &stgt))
  {
    if (errno==ENOENT) return 1;
    sie("stat(%s)", tgt);
    // unreachable
  }
  if (stat(req, &sreq))
  {
    if (errno==ENOENT) return 0;
    sie("stat(%s)", req);
    // unreachable
  }

  T= &stgt.st_mtim;
  R= &sreq.st_mtim;

  if (R->tv_sec > T->tv_sec) return 1;
  if (R->tv_sec < T->tv_sec) return 0;
  if (R->tv_nsec > T->tv_nsec ) return 1;
  return 0;
}

// a better name for reqisnewer
int stale(char *obj, char *src)
{
  return reqisnewer(obj, src); 
}

int stalea(char *obj, char **src)
{
  int i;
  if (!src) return 0;
  for(i=0;src[i];i++) if (reqisnewer(obj, src[i])) return 1;
  return 0;
}
