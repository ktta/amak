
void make_dirs()
{
  int i;

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
  for(i=0;dirs[i];i++)
    vexec( "mkdir", "-p", dirs[i][0]=='!' ?
                            mkpath(dirs[i]+1, cvars.package_dir, NULL) : 
                            (char*) dirs[i],
           NULL, "making dir %s\n", dirs[i]);
}
