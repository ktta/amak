
typedef struct varset { struct varset *up; strmap_t map; } varset_t;
struct varstore { char *name, *value; };

void varset_put(varset_t  *set, char *name, char *value)
{
  struct varstore *V;
  V= strmap_find(&set->map, name);
  if (V) { V->value= value; return ; }
  V= malloc(sizeof(*V));
  V->name= strdup(name);
  V->value= strdup(value);
  strmap_insert(&set->map, V->name, V);
}

void varset_putif(varset_t  *set, char *name, char *value)
{
  if (value) varset_put(set, name, value);
}

char* varset_get(varset_t *set, char *name)
{
  struct varstore *V;
               while(1)
                  {
  V= strmap_find(&set->map, name);
  if (V) return V->value;
  if (!set->up) return NULL;
  set= set->up;
                  }
}

varset_t *varset_init(varset_t *parent)
{
  varset_t *V;
  V= calloc(1,sizeof(*V));
  V->up= parent;
  return V;
}

void var_from_ini(varset_t *V, char *name, char *sec, char *key)
{
  varset_putif(V, name, ini_getstrn(cfg, sec, key));
}

void var_from_inid(varset_t *V, char *name, char *sec, char *key,char *defv)
{
  varset_put(V, name, ini_getstr(cfg, sec, key, defv));
}

char *replace_vars(const char *input, varset_t *set)
{
  char *s, *e, *o;
  char buf[200];
  strbuf_t *B;

  B= strbuf_init();
  while(1)
  {
    if (!(s= strchr(input, '%')) || (!(e=strchr(s+1,'%')))) 
    {
       strbuf_puts(B, (char*) input);
       return strbuf_collect(B);
    }
    if (s!=input)
       strbuf_puts_l(B, (char*) input, s-input);

    memcpy(buf,s+1, e-s-1);
    buf[e-s-1]= 0;

    if ((o= varset_get(set, buf))) strbuf_puts(B, replace_vars(o, set));
                              else strbuf_puts_l(B, s, e-s+1);
    input= e+1;
  }
}

varset_t* varset_cvars()
{
  static varset_t *V;
  if (!V)
  {
    V= varset_init(NULL);
    varset_put(V, "android_jar", cvars.android_jar); 
    varset_put(V, "android_version", cvars.android_version);
    varset_put(V, "java_version", cvars.java_version);
    varset_put(V, "package_name", cvars.package_name);
    varset_put(V, "package_dir", cvars.package_dir);
    varset_put(V, "sdk", cvars.sdk); 
    if (cvars.studio) varset_put(V, "studio", cvars.studio);
  }
  return varset_init(V);
}

char **seprep(char *str, varset_t *V)
{
  return sepstr(replace_vars(str, V), NULL);
}

varset_t *varset_putm(varset_t *V, ...)
{
  va_list ap;
  char *name, *value;
  va_start(ap, V);
  while((name=va_arg(ap,char*))) varset_put(V, name, va_arg(ap,char*));
  va_end(ap);
  return V;
}
