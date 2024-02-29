

char *cook_ini_var
  (char *sec, char *key, char *defvalue, varset_t *V)
{
  return replace_vars(ini_getstr(cfg, sec, key, defvalue),V);
}
