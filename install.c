

void install_apk()
{
  char *apkfile;

  apkfile= ini_getstr(cfg, "project", "apkfile", DEFVAL_APKFILE);
  
  vexec(replace_vars(ini_getstr(cfg, "adb", "path", DEFVAL_ADB),
                     varset_cvars()),
        "install", mkpath("build", apkfile, NULL),
         NULL, "installing %s\n", apkfile);
}
