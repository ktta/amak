




void make_dirs()
{
  char buf[2000];
  int i;
  char *package_name, *package_dir;

  static const char *dirs[]=
  {
   "!" aPT_GENJAVA,
   "!" aPT_JAVAOBJ,
   "!" aPT_JAVASRC,
   aPT_NATIVESRC,
   aPT_DEPx86,
   aPT_DEPx86_64,
   aPT_DEPaarch64,
   aPT_DEParmv7a,
   aPT_LIBx86,
   aPT_LIBx86_64,
   aPT_LIBaarch64,
   aPT_LIBarmv7a,
   aPT_RESVALUES,
   NULL
  };
  char *argv[4];
  int ai;
  printf("making directories\n");
  for(i=0;dirs[i];i++)
  {
    ai= 0;
    argv[ai++]= "mkdir";
    argv[ai++]= "-p";
    if (dirs[i][0]=='!')
      argv[ai++]= mkpath(dirs[i]+1, cvars.package_dir, NULL);
    else
      argv[ai++]= (char*) dirs[i];
    argv[ai]= 0;
    exec_program(argv,NULL);
  }
}
