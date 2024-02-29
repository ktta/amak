

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
