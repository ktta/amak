

static void compile_natives(int force)
{
  int i;
  target_t *tgt;
  for(i=0;tgt=cvars.targets[i];i++)
  {
    compile_native(tgt, force);
    link_native(tgt, force);
  }
}

void iniload(char *fn, char *env, int help)
{
  if (!ini_load_file(cfg, fn)) return ;
  if (env)
    fprintf(stderr,"amak[%s]: %s\n", env, ini_get_error(cfg));
  else
    fprintf(stderr,"amak: %s\n", ini_get_error(cfg));
  if (help && ini_get_errno(cfg)==ENOENT)
       fprintf(stderr, "try running\n\n"    " amak init\n\n"
                       "or\n\n"         " amak minit\n\n"
                       "to create an initial amak.ini file\n");
  exit(1);
}

void tryloadenv(char* vn)
{
  char *fn;
  if ((fn=getenv(vn))) iniload(fn, vn, 0);
}

void init(int release)
{
  cfg= ini_new();
  tryloadenv("AMAKSYS");
  tryloadenv("AMAKUSR");
  iniload("amak.ini", NULL, 1);
  init_cvars(release);
}


int main(int argc,char **argv)
{
  int i;
struct mcom {
  char *name;
  int cmdnum;
  char *desc;
  int has_file_arg;
};

enum {
   mcBUILD,       mcCLEAN,     mcGEN,       mcGENF,      mcINSTALLF,
   mcHELP,         mcINIT,  mcINSTALL,      mcJAVA,      mcMANIFEST,
   mcMINIT,      mcNATIVE,  mcREBUILD,    mcREMOVE,           mcRES,
   mcVERSION,  mcNATIVEF,     mcJAVAF,   mcRELEASE,      mcRELEASEF,
};

static const struct mcom com[]=
{
{ "build",      mcBUILD,  "Build the project, without checking for"
                          " dependencies."                               },
{ "clean",      mcCLEAN,  "Clean up the build."                          },
{ "gen",          mcGEN,  "Create necessary files and directories."      },
{ "genf",        mcGENF,  "Generate files, but overrides them even "
                          "if they are present."                         },
{ "help",        mcHELP,  "Print this help message."                     }, 
{ "init",        mcINIT,  "Create an initial amak.ini file."             },
{ "install",  mcINSTALL,  "Rebuild and install the generated package "
                          "using adb."                                   },
{ "installf", mcINSTALLF, "Build and install the generated package "
                          "using adb."                                   },
{ "java",        mcJAVA,  "Rebuild Java code."                           },
{ "javaf",      mcJAVAF,  "Build Java code."                             },
{ "manifest",mcMANIFEST,  "Build the AndroidManifest.xml file."          },
{ "minit",      mcMINIT,  "Create a minimal amak.ini file. "             },
{ "native",    mcNATIVE,  "Rebuild native libraries."                    },
{ "nativef",  mcNATIVEF,  "Build native libraries."                      },
{ "remove",    mcREMOVE,  "Remove the build directory."                  },
{ "release",  mcRELEASE,  "Make a release build."                        },
{ "releasef",mcRELEASEF,  "Make a release build, without checking "
                          "dependencies."                                },
{ "res",          mcRES,  "Build the resources."                         },
{ "version", mcVERSION,   "Print version info."                          },
{ NULL, -1, NULL }
};
  int cno;
  int force, release, install;

  if (argc==1) // rebuild
    cno= mcREBUILD;
  else
    cno= strfind(argv[1], com, sizeof(com[0]), offsetof(struct mcom,name),
                                               offsetof(struct mcom,cmdnum));

  force= 0;
  release= 0;
  install= 0;

  switch(cno)
  {
  case mcRELEASEF: force= 1; cno= mcREBUILD; release= 1; break;
  case  mcRELEASE:           cno= mcREBUILD; release= 1; break;
  case    mcJAVAF: force= 1; cno= mcJAVA;                break;
  case  mcNATIVEF: force= 1; cno= mcNATIVE;              break;
  case     mcGENF: force= 1; cno= mcGEN;                 break;
  case    mcBUILD: force= 1; cno= mcREBUILD;             break;
  case  mcINSTALL:           cno= mcREBUILD; install= 1; break;
  case mcINSTALLF: force= 1; cno= mcREBUILD; install= 1; break;
  }

  openlog();
  switch(cno)
  {
  case mcHELP:
  default:
    printf("usage: amak <command> where command is one of:\n");
    for(i=0;com[i].name;i++)
    {
      int s;
      if (com[i].has_file_arg)
         s= printf("%s <file>", com[i].name);
      else
         s= printf("%s", com[i].name);
      for(;s<16;s++) printf(" ");
      printf("%s\n", com[i].desc);
    }
    return cno!=mcHELP;
  case mcINIT:
    write_file_string("amak.ini", data_get("amak.ini", NULL));
    break;
  case mcMINIT:
    write_file_string("amak.ini", data_get("minimal.ini", NULL));
    break;
  case mcGEN:
    init(release);
    make_dirs();
    make_manifest(force);
    make_strings(force);
    make_activity(force);
    make_keystore();
    break;
  case mcMANIFEST:
    init(release);
    make_manifest(1);
    break;
  case mcRES:
    init(release);
    compile_resources(1);
    break;
  case mcJAVA:
    init(release);
    compile_resources(0); // force is for the java thing.
    compile_java(force);
    break;
  case mcNATIVE:
    init(release);
    compile_natives(force);
    break;
  case mcREBUILD:
    init(release);
    compile_resources(force);
    compile_java(force);
    compile_natives(force);
    link_java(force);
    make_apk(force);
    if (install) install_apk();
    break;
  case mcREMOVE:
    remove_build_dir();
    break;
  case mcCLEAN:
    clean_up();
    break;
  case mcVERSION:
    printf("amak 240227\n");
    break;
  }
  closelog();
 
  return 0;
}

