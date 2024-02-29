
int strfind
  (const char *what, const void *base,
   size_t elsz, size_t stroff, size_t intoff)
{
  const char * const *sptr;
  const int  *iptr;

  sptr= base + stroff;
  iptr= base + intoff;

  while(*sptr)
     if (!strcmp(*sptr, what)) return *iptr;
     else  { sptr= (const void*) sptr + elsz; 
             iptr= (const void*) iptr + elsz; }
  return -1;
}

int strindex(const char *what, ...)
{
  va_list ap;
  const char *str;
  int i;
  i= 0;
  va_start(ap, what);
  while((str=va_arg(ap,char*)))
  {
    if (!strcmp(str, what)) { va_end(ap); return i; }
    i++;
  }
  va_end(ap);
  return -1;
}

char *repsfx(char *str, char *sfx, char *nsf)
{
  int Ls, Lf, Ln;
  char *K;
  Lf= strlen(sfx);
  Ls= strlen(str);
  Ln= strlen(nsf);
  if (Ls<Lf) return strdup(str);
  
  K= malloc( Ls - Lf + Ln + 1);
  memcpy(K, str, Ls-Lf);
  strcpy(K+Ls-Lf, nsf);
  return K;
}

char *dotsfx(char *str)
{
  char *s;
  for(s=str+strlen(str)-1;s>str;s--) if (*s=='.') return s;
  return NULL;
}

int hasdotsfx(char *str, char *sfx)
{
  char *s;
  return (s=dotsfx(str)) ? !strcmp(s, sfx) : 0;
}

char *repdotsfx(char *str, char *nsf)
{
  char *s;
  return (s= dotsfx(str)) ? repsfx(str, s, nsf) : strdup(str);
}


static int sep_ispc(char *s)
{
  unsigned char k= *s;
  return (k > 0 && k<=32) || k==127;
}

char **sepstr(char *src, int *ns)
{
  int i;
  int al;
  char **R;
  int n;

  src= strdup(src);

  // 2= one value + one NULL
  for(i=0,al=2;src[i];i++) if (src[i]==' ') al++;
 
  R= malloc(sizeof(*R)*al);
  n= 0;
  while(*src)
  {
    while(sep_ispc(src)) src++;
    if (!*src) break;
    R[n++]= src;
    while(*src && !sep_ispc(src)) src++;
    if (*src) { *src= 0; src++; }
  }
  R[n]= NULL;
  if (ns) *ns= n;
  return R;
}

char *mkpath(const char *first, ...)
{
  va_list ap;
  char *K, *o;
  int L;
  L= strlen(first);
  va_start(ap, first);
  while((K=va_arg(ap,char*))) L+= 1+ strlen(K);
  va_end(ap);

  o= malloc(L+1);
  L= strlen(first);
  memcpy(o, first, L);
  va_start(ap, first);
  while((K=va_arg(ap,char*))) { o[L++]='/'; strcpy(o+L, K); L+= strlen(K); }
  va_end(ap);

  return o;
}

char *strcom(const char *first, ...)
{
  va_list ap;
  char *K, *o;
  int L;
  L= strlen(first);
  va_start(ap, first);
  while((K=va_arg(ap,char*))) L+= strlen(K);
  va_end(ap);

  o= malloc(L+1);
  L= strlen(first);
  memcpy(o, first, L);
  va_start(ap, first);
  while((K=va_arg(ap,char*))) { strcpy(o+L, K); L+= strlen(K); }
  va_end(ap);

  return o;
}

char *package_name_to_dir(char *package_name)
{
  char *s,*k;
  k= s= strdup(package_name);
  while((k=strchr(k,'.'))) *k= '/';
  return s;
}
