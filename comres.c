

void compile_resources(int force)
{
  char **resfiles;
  int i;
  varset_t *V;

  if (!force)
  {
    resfiles= find_files("res", NULL, 0);
    for(i=0;resfiles[i];i++)
      if (reqisnewer(aPT_RESOURCESZIP, resfiles[i]))
         break;
    if (!resfiles[i] &&  !reqisnewer(aPT_RESOURCESZIP, aPT_MANIFEST))
       return ;
  }

  V= varset_cvars();

  vexec(tool_path("aapt2"),
        VXMA, seprep(ini_getstr(cfg, "aapt2", "cflags",
                                DEFVAL_AAPT2_CFLAGS),V),
        NULL, "compiling resources\n");

  vexec(tool_path("aapt2"),
        VXMA, seprep(ini_getstr(cfg, "aapt2", "lflags",
                                DEFVAL_AAPT2_LFLAGS), V),
        NULL, "linking resources\n");
}
