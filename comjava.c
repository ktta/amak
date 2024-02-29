



void compile_java(int force)
{
  char **java_sources, *S,*src;
  int i;
  varset_t *V;

  V= varset_cvars();

  varset_put(V, "Rjava", 
             mkpath( aPT_GENJAVA , cvars.package_dir, "R.java", NULL));

  java_sources= find_files(aPT_JAVASRC, "java", 1);

  for(i=0;(S=java_sources[i]);i++)
  {
    src= mkpath(aPT_JAVASRC, S, NULL); // put the directory back in
    if (force ||  stale(mkpath(aPT_JAVAOBJ, 
                               repsfx(S, ".java", ".class"), NULL),
                        src))
      vexec(replace_vars(ini_getstr(cfg,"java", "javac", DEFVAL_JAVAC), V),
            VXMA, seprep(ini_getstr(cfg, "java", "cflags", DEFVAL_JAVACFLAGS),
                         varset_put(V, "input", src)),
            NULL, "compiling %s\n", basename(strdup(S)));
  }
}
