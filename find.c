
void strmove(char *dst, char *src)
{
  while(*src) *dst++= *src++;
  *dst++= 0;
}

char **find_files(char *dir, char *suffix, int strip_dir)
{
  ptrbuf_t *P;
  char **R;
  char *line; size_t line_size;
  FILE *inp;
  char buf[2000];
  int Ldir;

  P= ptrbuf_new(1024);
  Ldir= strip_dir ? (strlen(dir) + 1) : 0;

  if (suffix)
    snprintf(buf,sizeof(buf),"find %s -name '*.%s' -type f",dir, suffix);
  else
    snprintf(buf,sizeof(buf),"find %s -type f", dir);
  inp= popen(buf, "r");
  if (!inp) sie("popen(%s)", buf);
  line= NULL; line_size= 0;
  while(getline(&line, &line_size, inp)>0)
  {
    char *k;
    if ((k=strchr(line,'\n'))) *k= 0;
    ptrbuf_put(P, strdup(line+Ldir));
  }
  pclose(inp);
  ptrbuf_put(P, NULL);
  ptrbuf_free(P, (void***) &R, NULL);
  return R;
}
