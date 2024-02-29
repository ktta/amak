int file_exists(char *fn)
{
  struct stat sb;
  if (stat(fn, &sb))
  {
    if (errno==ENOENT) return 0;
    sie("stat(%s)", fn);
  }
  if ((sb.st_mode & S_IFMT) !=S_IFREG)
    die("%s is not a regular file\n", fn);
  return 1;
}


void write_file_blob(char *fn, const char *str, size_t sz)
{
  FILE *f;
  scallezp(f=fopen(fn,"wb"), "fopen(%s)", fn);
  fwrite(str,1, sz, f);
  fclose(f);
}

void write_file_string(char *fn, const char *str)
{
  write_file_blob(fn, str, strlen(str));
}


typedef struct
{
  char *name;
  DIR *dir;
  int types;
} file_iter_t;

enum { FT_FILE=1, FT_LINK= 2 };

void file_iter_begin(file_iter_t *F, char *dirname, int filter)
{
  scallezp(F->dir= opendir(dirname), "open(%s)", dirname); 
  F->types= filter;
}

int file_iter_get(file_iter_t *F)
{
  struct dirent *de;
again:
  de= readdir(F->dir);
  if (!de) return 0;
  switch(de->d_type)
  {
  case DT_REG: if (!(F->types & FT_FILE)) goto again; break;
  case DT_LNK: if (!(F->types & FT_LINK)) goto again; break;
  default:     goto again;
  }
  F->name= de->d_name;
  return 1;
}

void file_iter_end(file_iter_t *F)
{
  closedir(F->dir);
}

