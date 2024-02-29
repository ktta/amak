#include "sysheaders.h"
#include "fileop.c"
#include "latest.c"


int main(int argc, char **argv)
{
  char *L;
  L= latest(argv[1]);
  printf("latest is %s\n", L);
  return 0;
}
