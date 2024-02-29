
FILE *logfile;

void openlog()
{
  if (!logfile) logfile= fopen("build/amak.log", "wb");
  if (!logfile) logfile= stdout;
}

void closelog()
{
  if (logfile && logfile!=stdout) fclose(logfile);
  logfile= NULL;
}
