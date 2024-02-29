

void make_keystore()
{
  const char *d;
  size_t sz;
  d= data_get(aFN_KEYSTORE, &sz);
  write_file_blob(aPT_KEYSTORE, d, sz);
}
