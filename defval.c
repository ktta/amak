

#define DEFVAL_ANDROID_VERSION     "26"
#define DEFVAL_PACKAGE_NAME        "com.dodo.android_test"
#define DEFVAL_DISPLAY_NAME        "Android Test"
#define DEFVAL_ORIENTATION         "landscape"
#define DEFVAL_APKFILE             "AndroidTest.apk"
#define DEFVAL_BUILD_TOOL_SEARCH   "%sdk%/build-tools"

#define DEFVAL_JAVAC               "%studio%/jre/bin/javac"
#define DEFVAL_JAVACFLAGS          "-source %java_version% -target %java_version% -d " aPT_JAVAOBJ  " -bootclasspath %android_jar% %input% %Rjava%"
#define DEFVAL_JAVA_VERSION        "1.6"
#define DEFVAL_ANDROID_JAR         "%sdk%/platforms/android-%android_version%/android.jar"

#define DEFVAL_AAPT2_CFLAGS        "compile -o build/rescomp.zip --dir res"
#define DEFVAL_AAPT2_LFLAGS        "link -I %android_jar% build/rescomp.zip --manifest " aPT_MANIFEST " --java " aPT_GENJAVA " -o build/resources.zip"

#define DEFVAL_D8_FLAGS            "--classpath %android_jar% --output build"

#define DEFVAL_KEYSTORE            aPT_KEYSTORE 
#define DEFVAL_STOREPASS           "android"

#define DEFVAL_APKSIGNER_FLAGS    "sign -ks %keystore% -ks-pass pass:%storepass%  --out build/%apkfile% build/unsigned.apk"


#define DEFVAL_BINCXX             "clang++"
#define DEFVAL_BINCC              "clang"
#define DEFVAL_BINLD              "clang"

#define DEFVAL_NDKSEARCH          "%sdk%/ndk"

#define DEFVAL_ADB                "%sdk%/platform-tools/adb"

