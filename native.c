

typedef struct
{
  char *def_flags;
  char *bin, *flags; 
} ctool_t;

enum { CT_CC, CT_CXX, CT_LD };

typedef struct
{
  char *name;
  char *depdir, *libdir;
  ctool_t  tools[3];
  char *def_spec;
  char *spec;
} target_t;



#define   CFLAGS_COMMON "-target %target_spec% " \
                        "-fdata-sections " \
                        "-ffunction-sections " \
                        "-fstack-protector-strong " \
                        "-funwind-tables " \
                        "-no-canonical-prefixes " \
                        "-Wformat -Werror=format-security " \
                        "-g " \
                        " -Wno-invalid-command-line-argument " \
                        " -Wno-unused-command-line-argument " \
                        " -D_FORTIFY_SOURCE=2 " \
                        " -DANDROID " \
                        " -I native " \
                        " -UNDEBUG -O2 " \
                        " -c %input% " \
                        " -o %output% " \
                        " -MMD -MF%depend% " 

#define   CXXFLAGS_COMMON  CFLAGS_COMMON " -fno-rtti -fno-exceptions " \
                           " -nostdinc++ "

#define   LDFLAGS_COMMON   " -target %target_spec% -no-canonical-prefixes " \
                           " -Wl,--no-undefined " \
                           " -Wl,--fatal-warnings " \
                           " -nostdlib++ " \
                           " -Wl,--gc-sections " \
                           " -shared " \
                           " -Wl,-soname,libmain.so " \
                           " -o %output% " \
                           " %inputs% -landroid -llog -lc -lm "

target_t known_targets[]=
{
  { "x86_64", aPT_DEPx86_64, aPT_LIBx86_64,
           {{ CFLAGS_COMMON " -fPIC -mstackrealign "   },
           {  CXXFLAGS_COMMON " -fPIC -mstackrealign " }, 
           {  LDFLAGS_COMMON                          }}, 
              "x86_64-linux-android%android_version%"   },
  { "x86",    aPT_DEPx86,    aPT_LIBx86, 
           {{ CFLAGS_COMMON   " -fPIC "                },
           {  CXXFLAGS_COMMON " -fPIC "                }, 
           {  LDFLAGS_COMMON                          }},
              "i686-linux-android%android_version%"      },
  { "aarch64",aPT_DEPaarch64, aPT_LIBaarch64,
/*** add  
        " -mbranch-protection=standard "
     etc in here. It's not strictly necessary, the android tools do it
     only if there is a user preference for it. however, I should be 
     able to present this option to the user.
 ***/
           {{ CFLAGS_COMMON " -fpic "                  },
           {  CXXFLAGS_COMMON " -fpic "                },
           {  LDFLAGS_COMMON                          }},
              "aarch64-linux-android%android_version%"   },
  { "armv7a", aPT_DEParmv7a, aPT_LIBarmv7a, 
           {{ CFLAGS_COMMON " -fpic "                  },
           {  CXXFLAGS_COMMON " -fpic "                },
           {  LDFLAGS_COMMON                          }},
              "armv7a-linux-androideabi%android_version%" },
  { NULL }
};

#undef CFLAGS_COMMON
#undef CXXFLAGS_COMMON
#undef LDFLAGS_COMMON

target_t *find_target(char *target)
{
  target_t *T;
  int i;
  char *s;
  for(T=known_targets;s=T->name;T++) if (!strcmp(s, target))  return T;
  die("unknown target %s\n", target);
  return NULL;
}

