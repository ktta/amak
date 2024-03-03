

void make_activity(int force)
{
  varset_t *V;
  char *path;

  path= mkpath(aPT_JAVASRC, cvars.package_dir, aFN_ACTIVITY, NULL);

  if (!force && file_exists(path)) return ;

  V= varset_init(NULL);
  varset_put(V, "package_name", cvars.package_name);

  write_file_string(path, replace_vars(data_get(aFN_ACTIVITY,NULL), V));
}

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


void make_keystore()
{
  const char *d;
  size_t sz;
  d= data_get(aFN_KEYSTORE, &sz);
  write_file_blob(aPT_KEYSTORE, d, sz);
}


void encode_permissions(varset_t *S)
{
  char *perm;
  char **P;  int nP, i;
  strbuf_t *B;

  char *pskel= 
      "<uses-permission android:name=\"android.permission.%s\" />\n";
 
  perm= ini_getstrn(cfg,"project", "permissions");

  if (!perm)
  {
    varset_put(S, "permissions", "");
    return ;
  }
  P= sepstr(perm, &nP);
  B= strbuf_init();
  for(i=0;P[i];i++) strbuf_printf(B, pskel, P[i]);
  varset_put(S, "permissions", strbuf_collect(B));
}


void make_manifest(int force)
{
  varset_t *V;

  if (!force && file_exists(aPT_MANIFEST)) return ;

  V= varset_init(NULL);

  varset_put(V, "android_version", ini_getstr(cfg, "package",
                                    "android_version",  
                                    DEFVAL_ANDROID_VERSION));
  varset_put(V, "package_name", ini_getstr(cfg, "package", "name", 
                                     DEFVAL_PACKAGE_NAME));
  varset_put(V, "display_name", ini_getstr(cfg, "package", "display_name",
                                     DEFVAL_DISPLAY_NAME));
  varset_put(V, "orientation", ini_getstr(cfg, "package", "orientation", 
                                     DEFVAL_ORIENTATION));
  encode_permissions(V);

  write_file_string(aPT_MANIFEST, replace_vars(data_get(aFN_MANIFEST,NULL),V));
}

void make_strings(int force)
{
  if (!force && file_exists(aPT_STRINGSXML)) return ;
  write_file_string(aPT_STRINGSXML, data_get(aFN_STRINGSXML, NULL));
}

void make_styles(int force)
{
  if (force || !file_exists(aPT_STYLESXML)) 
     write_file_string(aPT_STYLESXML, data_get(aFN_STYLESXML, NULL));
}
