
void make_apk(int force)
{
  int i;
  varset_t *V;
  char *flags;
  target_t *target;
  char *apkfile, *keystore;
  char *apkpath;

  apkfile= ini_getstr(cfg, "project", "apkfile",  DEFVAL_APKFILE);
  keystore= ini_getstr(cfg, "apksigner", "keystore",  DEFVAL_KEYSTORE);

  if (!force)
  {
    apkpath= mkpath( "build" , apkfile, NULL );
    if (reqisnewer(apkpath, aPT_RESOURCESZIP)) goto doit;
    if (reqisnewer(apkpath, aPT_CLASSESDEX))   goto doit;
    for(i=0;target=cvars.targets[i];i++)
    {
      if (reqisnewer(apkpath, mkpath(target->libdir, aFN_LIBMAINSO, NULL)))
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
  
  V= varset_cvars();
  varset_put(V, "apkfile", apkfile);
  varset_put(V, "keystore", keystore);
  varset_put(V, "storepass", ini_getstr(cfg, "apksigner", "storepass",
                                         DEFVAL_STOREPASS));
  flags= ini_getstr(cfg, "apksigner", "flags", DEFVAL_APKSIGNER_FLAGS);
  vexec(tool_path("apksigner"), VXMA, seprep(flags,V), NULL, "signing apk\n");
}
