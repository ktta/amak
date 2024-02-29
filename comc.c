

char *find_latest_bin(char *dir, char *tool)
{
  FILE *p;
  char buf[2000];
  char *best;
  char *line; size_t line_size; char *s;
  snprintf(buf,sizeof(buf), "find %s  -xtype f  -name %s", dir, tool);
  p= popen(buf, "r");
  line= NULL; line_size= 0;
  best= NULL;
  while(getline(&line, &line_size, p)>0)
  {
    if ((s= strchr(line, '\n'))) *s= 0;
    if (!best || strverscmp(best, line)<0) 
    {
      if (best) free(best);
      best= strdup(line);
    } 
  }
  if (line) free(line);
  return best;
}

varset_t *varset_target(target_t *target)
{
  varset_t *V;
  V= varset_cvars();
  if (!target->spec)
    target->spec=ini_getstr(cfg,target->name, "spec", target->def_spec);
  varset_put(V, "target_spec", target->spec);
  return V;
}

char *tool_names[]= { "cc", "cxx", "ld" };

// tool is cc or cxx..

char* get_native_compiler(int tool, target_t *target)
{
  char *S;
  char *bin, *ndk_search;
  varset_t *V;

  if ((S=target->tools[tool].bin)) return S;

  V= varset_target(target);

  // target specific compiler set in the [target] section
  S= cook_ini_var(target->name, tool_names[tool], NULL, V);
  if (S) goto found;

  // generic compiler set in the [native] section
  S= cook_ini_var("native", tool_names[tool], NULL, V);
  if (S) goto found;

  // if these two fail, we're going to look for compilers like this:
/*** 
~/Android/Sdk/ndk/25.1.8937393/toolchains/llvm/prebuilt/linux-x86_64/bin/clang
~/Android/Sdk/ndk/25.1.8937393/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++

  The whole thing is so complicated, I don't want to do all this stuff.
  I will simply select the latest version and be done with it. I will
  just use the 'find' tool. 
**/

  ndk_search= cook_ini_var("native", "ndk_search", DEFVAL_NDKSEARCH, V);
  switch(tool)
  {
  case CT_LD:
     bin= ini_getstr(cfg, "native", "bin_ld", DEFVAL_BINLD);
     break;
  case CT_CC:
     bin= ini_getstr(cfg, "native", "bin_cc", DEFVAL_BINCC);
     break;
  case CT_CXX:
     bin= ini_getstr(cfg, "native", "bin_cxx", DEFVAL_BINCXX);
     break;
  default:
     die("unknown compiler %d\n", tool);
  }

  S= find_latest_bin(ndk_search, bin);
  if (!S) die("can't find compiler %s\n", tool_names[tool]);

found:
  return target->tools[tool].bin= S; 
}

char *null2empty(char *x) { return x ? x : ""; }


char* get_native_flags(int tool, target_t *target)
{
  char *flags_name, *oflags_name;
  char *flags_a, *oflags_a;
  char *flags_c, *oflags_c;
  char *flags;
  char *tn;
  char *S;

  if ((S=target->tools[tool].flags)) return S;
  tn= tool_names[tool];

  flags_name= strcom(tn, "flags", NULL);
  oflags_name= strcom("override_", flags_name, NULL);

  oflags_a= ini_getstrn(cfg, target->name, oflags_name);
  oflags_c= ini_getstrn(cfg, "native", oflags_name);

  if (oflags_a || oflags_c)
    flags= strcom(null2empty(oflags_c), " ", null2empty(oflags_a) , NULL); 
  else
    flags= target->tools[tool].def_flags;

  flags_c= ini_getstrn(cfg, "native", flags_name);
  if (flags_c) flags= strcom(flags, " ", flags_c, NULL);

  flags_a= ini_getstrn(cfg, target->name, flags_name);
  if (flags_a) flags= strcom(flags, " ", flags_a, NULL);

  return target->tools[tool].flags= flags;
}

char **read_dep(char *fn)
{
  FILE *f;
  static char *line; size_t line_sz;
  ptrbuf_t *p;
  char *Z;
  char **K;
  int i;

  f= fopen(fn, "rb");
  if (!f) return NULL;
  p= ptrbuf_new(20);
  while(getline(&line, &line_sz, f)>0)
  {
    Z= strchr(line, ':');
    if (!Z) Z= line;
    K= sepstr(Z, NULL);
    for(i=0;K[i];i++)
      if (strcmp(K[i],"\\")) ptrbuf_put(p, K[i]);
  }
  fclose(f);
  ptrbuf_put(p, NULL);
  ptrbuf_free(p, (void***) &K, NULL);
  return K;
}

void compile_native_file
   (char *file, int tool, target_t *target, varset_t *V, int force)
{
  char *obj, *oname, *dep, *src;

  obj= mkpath(target->libdir, oname=repdotsfx(file, ".o"),NULL);
  src= mkpath(aPT_NATIVESRC, file, NULL);
  dep= strcom(target->depdir, "/", oname, ".dep", NULL);

  if (force || stale(obj, src) || stalea(obj, read_dep(dep)))
    vexec(get_native_compiler(tool, target),
          VXMA, seprep(get_native_flags(tool, target), 
                       varset_putm(V, "input", src,
                                      "output", obj,
                                      "depend", dep, NULL)),
          NULL, "compiling [%s] %s\n", target->name, file);
}

void compile_native(target_t *target,int force)
{
  file_iter_t file;
  char *sfx[]= { ".c", ".cc", ".cxx", ".cpp", ".C", ".c++", NULL };
  int i;
  varset_t *V;

  V= varset_target(target);

  file_iter_begin(&file, aPT_NATIVESRC, FT_FILE | FT_LINK);
  while(file_iter_get(&file))
    for(i=0;sfx[i];i++) 
      if (hasdotsfx(file.name, sfx[i]))
      {
        compile_native_file(file.name, i==0 ? CT_CC : CT_CXX, target, V, force);
        break;
      }
  file_iter_end(&file);
}

void link_native(target_t *target, int force)
{
  file_iter_t file;
  strbuf_t *B;
  int update;
  char *output;
 
  update= 0;
  B= strbuf_init();
  output= mkpath(target->libdir, aFN_LIBMAINSO, NULL);

  file_iter_begin(&file, target->libdir, FT_FILE);
  while(file_iter_get(&file))
    if (hasdotsfx(file.name, ".o"))
    {
      char *src;
      src=mkpath(target->libdir, file.name, NULL);
      if (force || reqisnewer(output, src)) update= 1;
      strbuf_puts(B, " ");
      strbuf_puts(B, src);
    }
  file_iter_end(&file);
  strbuf_puts(B, " ");

  if (update) 
    vexec(get_native_compiler(CT_LD, target),
          VXMA, seprep(get_native_flags(CT_LD, target),
                       varset_putm(varset_target(),
                                   "output", output,
                                   "inputs", strbuf_collect(B), 
                                   NULL)),
          NULL, "linking %s/libmain.so\n", target->libdir);
}
