
void make_apk(int force)
{
  char *apkfile, *keystore;
  int i;
  target_t *target;

  apkfile= ini_getstr(cfg, "project", "apkfile",  DEFVAL_APKFILE);
  keystore= ini_getstr(cfg, "apksigner", "keystore",  DEFVAL_KEYSTORE);

  if (!force)
  {
    char *apkpath;
    apkpath= mkpath( "build" , apkfile, NULL );
    if (stale(apkpath, keystore)
        || stale(apkpath, aPT_RESOURCESZIP) 
        || stale(apkpath, aPT_CLASSESDEX)) goto doit;
    for(i=0;target=cvars.targets[i];i++)
    {
      if (stale(apkpath, mkpath(target->libdir, aFN_LIBMAINSO, NULL)))
         goto doit;
    }
    return ;
  }

doit:

  vexec("cp", aPT_RESOURCESZIP, aPT_UNALIGNEDAPK, NULL,
              "copying resources\n");

  vexec("zip", "-u", aPT_UNALIGNEDAPK, "-j", aPT_CLASSESDEX, NULL,
              "copying java code\n");
 
  scallenz(chdir("build"), "chdir(build)");
  for(i=0;target=cvars.targets[i];i++)
  {
    vexec("zip", "-u", aPT_UNALIGNEDAPK, "-p",
                 mkpath(target->libdir+strlen("build/"), aFN_LIBMAINSO, NULL),
          NULL, "copying native %s code\n", target->name);
  }
  scallenz(chdir(".."), "chdir(..)");

  vexec(tool_path("zipalign"), "-f", "4", aPT_UNALIGNEDAPK, aPT_UNSIGNEDAPK,
                               NULL, "aligning apk\n");
  
  vexec(tool_path("apksigner"),
        VXMA, seprep(ini_getstr(cfg, "apksigner", "flags",
                                      DEFVAL_APKSIGNER_FLAGS),
                     varset_putm(varset_cvars(),
                                 "apkfile", apkfile,
                                 "keystore", keystore,
                                 "storepass", ini_getstr(cfg, "apksigner",
                                                         "storepass",
                                                         DEFVAL_STOREPASS),
                                  NULL)),
        NULL, "signing apk\n");
}
