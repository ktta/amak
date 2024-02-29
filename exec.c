

int fork_exec(char **argv)
{
  pid_t child;
  siginfo_t info;

  switch((child=fork()))
  {
  case 0:
    execvp(argv[0], argv);
    sie("execvp(%s)", argv[0]);
    break;
  case (pid_t) -1:
    sie("fork"); 
    break;
  }

  while(waitid(P_PID, child, &info, WEXITED))
    if (errno!=EINTR)
       sie("waitid(%d:%s)", (int) child, argv[0]);
 
  switch(info.si_code)
  {
  case CLD_KILLED:
  case CLD_DUMPED:
    die("%s got killed by signal %d\n", argv[0], info.si_status);
    break;
  case CLD_EXITED:
    if (info.si_status)
       die("%s exited with code %d\n", argv[0], info.si_status);
    break;
  }
  return 0;
}

static void print_args(char **argv, FILE *out)
{
  int i;
  for(i=0;argv[i];i++) fprintf(out,"%s%s", i? " ": "", argv[i]); 
  fprintf(out,"\n");
}

int exec_programv(char **argv, const char *fmt, va_list ap) 
{
  if (fmt) vfprintf(stdout, fmt, ap);
  print_args(argv, logfile);
  return fork_exec(argv);
}

int exec_program(char **argv, const char *fmt, ...)
{
  va_list ap;
  int retv;
  va_start(ap, fmt);
  retv= exec_programv(argv, fmt, ap);
  va_end(ap);
  return retv;
}


int put_array(char **dst, char **src)
{
  int i;
  for(i=0;src[i];i++) dst[i]= src[i];
  return i;
}

int count_array(char **src)
{
  int i;
  for(i=0;src[i];i++) ;
  return i;
}

/**
 **/
const char VXMA[]= "Heyoo";
int vexec(char *first, ...)
{
  char **args;
  char *arg, *fmt;
  int nargs;
  va_list ap;

  va_start(ap, first);
  for(nargs=0,arg=first;arg;arg=va_arg(ap,char*))
    if (arg==VXMA) nargs += count_array(va_arg(ap, char**));
              else nargs++;
  va_end(ap);

  args= calloc(nargs+1, sizeof(args[0]));

  va_start(ap, first);
  for(nargs=0,arg=first;arg;arg=va_arg(ap,char*))
    if (arg==VXMA) nargs += put_array(args+nargs, va_arg(ap, char**));
              else args[nargs++]= arg;
  args[nargs]= 0;
 
  fmt= va_arg(ap, char*);
  if (fmt)
     vfprintf(stdout, fmt, ap);
  va_end(ap);

  print_args(args, logfile);
  return fork_exec(args);
}
