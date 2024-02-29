

void compile_resources(int force)
{
  char **resfiles;
  varset_t *V;

  if (!force && !stale(aPT_RESOURCESZIP, aPT_MANIFEST) 
             && (resfiles= find_files("res", NULL, 0))
             && !stalea(aPT_RESOURCESZIP, resfiles))
     return ;

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
