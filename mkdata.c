#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "filenames.c"

struct dfile
{
  char *name; int as_string; unsigned long long int size; char *vname;
} data_files[] =
{
  { aFN_MANIFEST,          1 },
  { "amak.ini",            1 },
  { aFN_ACTIVITY,          1 },
  { "strings.xml",         1 },
  { "minimal.ini",         1 },
  { aFN_KEYSTORE,          0 },
  { aFN_STYLESXML,         1 },
  { NULL                     }
};

static char *spec[256];

void init_spec()
{
  int i;
  char *punc;
  char buf[20];

  for(i=1;i<32;i++)
  {
    sprintf(buf,"\\%03o", (int) i);
    spec[i]= strdup(buf);
  }
  spec['\n']= "\\n"; spec['\t']= "\\t";
  spec['"']= "\\\""; spec['\\']= "\\\\";
}

char *header=
"#include <stdio.h>\n"
"#include <stdlib.h>\n"
"#include <string.h>\n"
"const char *data_get(const char *fn, size_t *Rsize)\n"
"{\n"
"  static const struct {\n"
"    const char *name, *value;\n"
"    size_t size;\n"
"  } data[]= {\n";
char *footer=
"  { NULL, NULL } };\n"
"  int i;\n"
"  for(i=0;data[i].name;i++)\n"
"    if (!strcmp(data[i].name,fn)) \n"
"    {\n"
"       if (Rsize) *Rsize= data[i].size;\n"
"       return data[i].value;\n"
"    }\n"
"  return NULL;\n"
"}\n";

static FILE *open_file(char *fn)
{
  char buf[200];
  FILE *f;
  sprintf(buf,"data/%s", fn);
  f= fopen(buf, "rb");
  if (!f) exit(fprintf(stderr,"fopen(%s): %s\n", buf, strerror(errno))); 
  return f;
}

size_t p_as_string(FILE *o, FILE *f)
{
  ssize_t line_len, i;
  static size_t line_size;
  static unsigned char *line;
  size_t total;
  char *s;

  total= 0;

  while((line_len=getline((char**) &line, &line_size, f))>0)
  {
    total += line_len;
    fprintf(o, "\"");
    for(i=0;i<line_len;i++)
      if ((s=spec[line[i]])) fprintf(o, "%s", s);
                        else fprintf(o, "%c", line[i]);
    fprintf(o, "\"\n");
  }
  return total;
}

int plen(unsigned char a)
{
  return a < 10 ? 2 : (a<100 ? 3 : 4 );
}

size_t p_as_binary(FILE *o, FILE *f)
{
  unsigned char buf[2000];
  size_t total, len, i;
  int line_len;
  int clen;

  total= 0;
  line_len= 0;
  while((len=fread(buf, 1, sizeof(buf), f))>0)
  {
    total += len;
    for(i=0;i<len;i++)
    {
      clen= plen(buf[i]);
      if (clen+line_len>78) { fprintf(o,"\n"); line_len= 0; }
      fprintf(o,"%d,", buf[i]);
      line_len+= clen;
    }
  }
  fprintf(o,"\n");
  return total;
}

int main(int argc,char **argv)
{
  FILE *o, *f;
  int i; 

  init_spec();
  o= fopen("data.c", "wb");
  for(i=0;data_files[i].name;i++)
  {
    f= open_file(data_files[i].name);
    fprintf(o, "static const char dfile%d[]=\n", i);
    if (data_files[i].as_string)
    {
      data_files[i].size= 1 + p_as_string(o, f);
    }
    else
    {
      fprintf(o, "{\n");
      data_files[i].size= p_as_binary(o, f);
      fprintf(o, "}\n");
    }
    fprintf(o, ";\n");
    fclose(f);
  }
  fputs(header, o);
  for(i=0;data_files[i].name;i++)
  {
    fprintf(o, " { \"%s\", dfile%d, %lluull },\n",
                    data_files[i].name, i, data_files[i].size);
  }
  fputs(footer,o);
  fclose(o);

  return 0;
}
