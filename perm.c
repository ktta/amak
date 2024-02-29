
void encode_permissions(varset_t *S)
{
  char *perm;
  char **P;  int nP, i;
  strbuf_t *B;

  char *pskel= 
      "<uses-permission android:name=\"android.permission.%s\" />\n";
 
  perm= ini_getstrn(cfg,"project", "permissions");

  if (!perm)
  {
    varset_put(S, "permissions", "");
    return ;
  }
  P= sepstr(perm, &nP);
  B= strbuf_init();
  for(i=0;P[i];i++) strbuf_printf(B, pskel, P[i]);
  varset_put(S, "permissions", strbuf_collect(B));
}
