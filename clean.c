

void remove_build_dir()
{
  vexec("rm", "-Rf", "build", NULL, "removing build/\n");
}

void clean_up()
{
  vexec("find", "build", "-type", "f", "-delete", NULL, "cleaning build/\n");
}
