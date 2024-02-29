

struct 
{
  char *android_jar;
  char *android_version;
  char *package_name;
  char *package_dir;
  char *studio;
  char *sdk;
  char *java_version;
  target_t **targets;
} cvars;


void init_cvars(int release)
{
  cvars.android_jar= ini_getstr(cfg, "java", "android_jar",
                                DEFVAL_ANDROID_JAR);
  cvars.package_name= ini_getstr(cfg, "project", "name",
                                 DEFVAL_PACKAGE_NAME);
  cvars.package_dir= package_name_to_dir(cvars.package_name);
  cvars.studio= ini_getstrn(cfg, "paths", "studio");
  cvars.sdk= ini_getstrn(cfg, "paths", "sdk");
  if (!cvars.sdk)
     die("Path to Android SDK has to be set.\n");
   
  cvars.android_version= ini_getstr(cfg, "project", "android_version",
                                    DEFVAL_ANDROID_VERSION);
  cvars.java_version= ini_getstr(cfg, "java", "version", 
                                 DEFVAL_JAVA_VERSION);
  char *tgt;
  tgt= ini_getstrn(cfg, "native", release ? "release_targets" : "targets");
                        
  if (tgt)
  {
    char **t;
    int nt, i;
    t= sepstr(tgt, &nt);
    cvars.targets= calloc(nt, sizeof(cvars.targets[0]));
    for(i=0;i<nt;i++) cvars.targets[i]= find_target(t[i]);
  }
  else
  {
    cvars.targets= calloc(1, sizeof(cvars.targets[0]));
  }
}
