

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
