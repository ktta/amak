

void link_java(int force)
{
  char **obj;

  obj= find_files(aPT_JAVAOBJ, "class", 0);

  if (!force)
  {
    int i;
    for(i=0;obj[i];i++)
      if (reqisnewer(aPT_CLASSESDEX, obj[i]))
        break;

    if (!obj[i]) return ;
  }

  vexec(tool_path("d8"), 
     VXMA, seprep(ini_getstr(cfg,"d8", "flags", DEFVAL_D8_FLAGS),
                  varset_cvars()),
     VXMA, obj,
     NULL, "linking java objects\n");
}
