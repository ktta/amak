


char *tool_path(char *tool_name)
{
  char *user_defined;
  char *bt_ver, *bt_dir, *bt_search;
  char *lv;
  static varset_t *td;

  if (!td)
  {
    td= varset_cvars();
 
    var_from_ini(td, "build_tool_ver", "paths", "build_tool_ver");
    var_from_inid(td, "build_tool_search",  "paths", "build_tool_search",
                                 DEFVAL_BUILD_TOOL_SEARCH);
    var_from_ini(td, "build_tool_dir", "paths", "build_tool_dir");
  }

  user_defined= ini_getstrn(cfg, tool_name, "path");
  if (user_defined) 
     return replace_vars(user_defined, td);

  bt_dir= varset_get(td, "build_tool_dir");
  if (bt_dir)
     return replace_vars(mkpath(bt_dir, tool_name, NULL), td);
 
  bt_ver= varset_get(td, "build_tool_ver");
  bt_search= varset_get(td, "build_tool_search");

  if (bt_ver)
    return replace_vars(mkpath(bt_search, bt_ver, tool_name, NULL), td);

  if (!(lv=latest(bt_search=replace_vars(bt_search,td))))
     die("can not find tool directory. \n"
         "consider setting build_tool_search and/or build_tool_ver\n");

  return mkpath(bt_search, lv, tool_name, NULL);
}
