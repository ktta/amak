

char *latest(char *dirname)
{
  DIR *dir;
  struct dirent *de;
  char *latest;
  
  latest= NULL;
  scallezp(dir= opendir(dirname), "open(%s)", dirname);

  while((de=readdir(dir)))
  {
    if (de->d_type!=DT_DIR || de->d_name[0]=='.') continue;
    if (!latest || strverscmp(de->d_name, latest)>0)
    {
      if (latest) free(latest);
      latest= strdup(de->d_name);
    }
  }
  closedir(dir);
  errno= ENOENT;
  scallezp(latest, "search_latest(%s)", dirname);
  return latest;
}
