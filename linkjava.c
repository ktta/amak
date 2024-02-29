

void link_java(int force)
{
  char **obj;
  obj= find_files(aPT_JAVAOBJ, "class", 0);
  if (!force && !stalea(aPT_CLASSESDEX,obj)) return ;
  vexec(tool_path("d8"), 
     VXMA, seprep(ini_getstr(cfg,"d8", "flags", DEFVAL_D8_FLAGS),
                  varset_cvars()),
     VXMA, obj,
     NULL, "linking java objects\n");
}
