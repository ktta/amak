
void die(const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr,fmt, ap);
  va_end(ap);
  exit(1);
}

void sie(const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr,fmt, ap);
  va_end(ap);
  fprintf(stderr, ": %s\n", strerror(errno));
  exit(1);
}

void scallenz(int retv, const char *fmt, ...)
{
  va_list ap;
  if (!retv) return ;
  va_start(ap, fmt);
  fprintf(stderr, fmt, ap);
  va_end(ap);
  fprintf(stderr,": %s\n", strerror(errno));
  exit(1);
}

void scallezp(void *ptr, const char *fmt, ...)
{
  va_list ap;
  if (ptr) return ;
  va_start(ap, fmt);
  fprintf(stderr, fmt, ap);
  va_end(ap);
  fprintf(stderr,": %s\n", strerror(errno));
  exit(1);
}

