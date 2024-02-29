


void compile_java(int force)
{
  char **cmd, **Input;
  char **java_sources, *S;
  int i;
  varset_t *V;
  args_t *A;
  int na;

  V= varset_cvars();
  A= arg_init();

  arg_put(A, replace_vars(ini_getstr(cfg,"java", "javac", DEFVAL_JAVAC), V));
  arg_put_split(A, replace_vars(ini_getstr(cfg, "java", "cflags",
                                           DEFVAL_JAVACFLAGS),
                                V));
  arg_put(A, "dummy.java");
  arg_put(A, mkpath( aPT_GENJAVA , cvars.package_dir, "R.java", NULL));
  cmd= arg_get(A, &na);
  Input= cmd+na-2;

  java_sources= find_files(aPT_JAVASRC, "java", 1);

  for(i=0;S=java_sources[i];i++)
  {
    *Input= mkpath(aPT_JAVASRC, S, NULL);
    if (force || 
        reqisnewer(mkpath(aPT_JAVAOBJ, repsfx(S, ".java", ".class"), NULL),
                   *Input))
      exec_program(cmd, "compiling %s\n", basename(strdup(S)));
  }
}
