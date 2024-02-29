

typedef struct
{
  int narg, sarg;
  char **data;
} args_t;

static void arg_alloc(args_t *A, int xtra)
{
  if (A->narg+xtra<=A->sarg) return ;
  A->sarg += (A->sarg + xtra)/2;
  A->data= realloc(A->data, A->sarg*sizeof(A->data[0]));
}

args_t *arg_init()
{
  args_t *A;
  A= malloc(sizeof(*A));
  A->narg= 0;
  A->sarg= 200;
  A->data= malloc(sizeof(*A->data)*A->sarg);
  return A;
}

void arg_put(args_t *A, char *a)
{
  arg_alloc(A,1);
  A->data[A->narg++]= a;
}

void arg_putn(args_t *A, char **val, int n)
{
  int i;
  arg_alloc(A, n);
  for(i=0;i<n;i++)
    A->data[A->narg++]= val[i];
}

void arg_put_split(args_t *A, char *a)
{
  char **s;
  int n;

  s= sepstr(a,&n);
  arg_putn(A, s, n);
}

char** arg_get(args_t *A, int *nA)
{
  char **R;
  arg_alloc(A, 1);
  A->data[A->narg]= NULL;
  if (nA) *nA= A->narg;
  R= A->data;
  free(A);
  return R;
}

void arg_putarray(args_t *A, char **val)
{
  int n;
  for(n=0;val[n];n++)  ;
  arg_putn(A, val, n);
}


int arg_exec(args_t *A, const char *fmt, ...)
{
  va_list ap;
  int retv;
  va_start(ap, fmt);
  retv= exec_programv(arg_get(A, NULL), fmt, ap);
  va_end(ap);
  return retv;
}

