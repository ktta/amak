static const char dfile0[]=
"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
"<!-- BEGIN_INCLUDE(manifest) -->\n"
"<manifest\n"
"        xmlns:android=\"http://schemas.android.com/apk/res/android\"\n"
"        package=\"%package_name%\"\n"
"        android:versionCode=\"1\"\n"
"        android:versionName=\"1.0\">\n"
"\n"
"    <uses-sdk android:minSdkVersion=\"%android_version%\" />\n"
"       %permissions%\n"
"\n"
"    <!-- This .apk has Java code, so set hasCode to true. -->\n"
"    <application android:label=\"%display_name%\" android:hasCode=\"true\">\n"
"\n"
"<!-- we don't want our application to restart on any configuration change.\n"
"     thus the huge configChanges declaration -->\n"
"        <activity\n"
"             android:name=\"%package_name%.MainActivity\"\n"
"             android:label=\"%display_name%\"\n"
"             android:screenOrientation=\"%orientation%\"\n"
"             android:configChanges=\"mcc|mnc|locale|touchscreen|keyboard|keyboardHidden|navigation|screenLayout|fontScale|uiMode|orientation|density|screenSize|smallestScreenSize\">\n"
"           <intent-filter>\n"
"                <action android:name=\"android.intent.action.MAIN\" />\n"
"                <category android:name=\"android.intent.category.LAUNCHER\" />\n"
"           </intent-filter>\n"
"        </activity>\n"
"    </application>\n"
"\n"
"</manifest> \n"
"<!-- END_INCLUDE(manifest) -->\n"
"\n"
;
static const char dfile1[]=
"#\n"
"# This is the configuration file for amak.\n"
"#\n"
"# Uncomment and set the values below. Almost all parameters are mandatory.\n"
"#\n"
"[project]\n"
"# name=com.dodo.android_test\n"
"# android_version= 26\n"
"# apkfile= AndroidTest.apk\n"
"#\n"
"# The following name will be put into the AndroidManifest.xml. It will\n"
"# be used to display the icon.\n"
"#\n"
"# display_name=Android Test\n"
"#\n"
"orientation=landscape\n"
"# \n"
"# The permissions required for the application. The identifiers used\n"
"# are the same ones that appear in AndroidManifest.xml \n"
"#\n"
"# permissions= WRITE_EXTERNAL_STORAGE INTERNET ACCESS_NETWORK_STATE\n"
"# permissions+= ACCESS_WIFI_STATE \n"
"#\n"
"# If you have native code, then you need to specify here for which\n"
"# targets you want it compiled.\n"
"#\n"
"# targets= x86_64 x86 arm64-v8a armeabi-v7a\n"
"#\n"
"\n"
"[paths]\n"
"#\n"
"# Path to Android Sdk\n"
"#\n"
"# sdk= /home/dodo/Android/Sdk\n"
"#\n"
"#\n"
"# Build tools are normally found automatically from the SDK \n"
"# distribution.\n"
"#\n"
"# However, if your SDK distribution's packaging differs from the\n"
"# one implemented in the program, you may use the following \n"
"# variables to set the required paths.\n"
"#\n"
"# The most direct approach is to set the paths to the required tools\n"
"# using the following variables. The defaults are shown here.\n"
"# \n"
"# aapt2=%sdk%/build-tools/%build_tool_ver%/aapt2\n"
"# d8=%sdk%/build-tools/%build_tool_ver%/d8\n"
"# zipalign=%sdk%/build-tools/%build_tool_ver%/zipalign\n"
"# apksigner=%sdk%/build-tools/%build_tool_ver%/apksigner\n"
"#\n"
"# The second most direct way of doing this is to set the directory\n"
"# for all the tools above:\n"
"#\n"
"# build_tool_dir=%sdk%/build-tools/%build_tool_ver%\n"
"#\n"
"# If you are a sysadmin and you want your users to find different\n"
"# versions of the tool set, you may set the following variable:\n"
"#\n"
"# build_tool_search=%sdk/build-tools\n"
"#\n"
"# Then, your users can select a particular version by setting \n"
"#\n"
"# build_tool_ver=33.0.1\n"
"#\n"
"# If this variable is missing, then the latest version of the toolset\n"
"# will be used.\n"
"#\n"
"# All in all, setting 'sdk', 'studio/javac' and 'android_version'\n"
"# should work by default.\n"
"#\n"
"\n"
"[java]\n"
"#\n"
"# The default javac installed on the system doesn't work. I use the\n"
"# one from Android Studio. If you are in the same situation, set this\n"
"# path.\n"
"#\n"
"# studio= /home/dodo/android-studio\n"
"#\n"
"# when given the 'studio' variable, the default path for javac is as\n"
"# follows:\n"
"#\n"
"# javac= %studio%/jre/bin/javac\n"
"#\n"
"# Alternatively, you may set the path to a known good javac here.\n"
"#\n"
"# javac=/path/to/javac\n"
"#\n"
"# If you want to use a version of java other than 1.6, you may set it\n"
"# here\n"
"#\n"
"# version=1.6\n"
"#\n"
"# The whole javac command line arguments are here:\n"
"#\n"
"# cflags= -source %java_version% -target %java_version% -d java-obj\n"
"# cflags+= -bootclasspath %android_version%\n"
"#\n"
"# The -d java-obj part is crucial.\n"
"#\n"
"# The android.jar file is normally found automatically within the \n"
"# SDK directory. However, you may set its path here if it's somewhere\n"
"# else. Here is the default value\n"
"#\n"
"# android_jar= %sdk%/platforms/android-%android_version%/android.jar\n"
"#\n"
"\n"
"\n"
"[compile]\n"
"#\n"
"# The program is also able to compile native C/C++ code. Here are the\n"
"# settings for that. \n"
"# \n"
"# The system normally detects the C/C++ compiler by itself. If you\n"
"# want to instruct the program to use a specific compiler, you may\n"
"# do so here.\n"
"#\n"
"# override_compiler=y\n"
"# \n"
"# Typically, only one compiler binary is used for all architectures,\n"
"# compiling code for each architecture by using the --target flag.\n"
"# If there is such a compiler, then it should be noted here. If there\n"
"# are separate binaries for each compiler, then they will be specified\n"
"# in their own sections.\n"
"#\n"
"# cc=%sdk%/ndk/%ndk_version%/toolchains/llvm/prebuilt/linux-%host%/bin/clang\n"
"# cc+= --target %arch%-linux-android%android_version%\n"
"# cxx=%sdk%/ndk/%ndk_version%/toolchains/llvm/prebuilt/linux-%host%/bin/clang++\n"
"# cxx+= --target %arch%-linux-android%android_version%\n"
"#\n"
"# common cflags will be given here, note that the system has default cflags\n"
"# suitable for each architecture. If you want to override them, put them\n"
"# here.\n"
"#\n"
"# cflags= \n"
"# cxxflags= \n"
"# ldflags=\n"
"#\n"
"# If you want to provide extra flags, you can do so using the following\n"
"#\n"
"# extra_cflags=\n"
"# extra_cxxflags=\n"
"# extra_ldflags=\n"
"#\n"
"# You may also set flags for specific architectures using the following\n"
"# section names. These override the settings in the common part.\n"
"#\n"
"# [x86_64]\n"
"# [aarch64]\n"
"# [armv7a]\n"
"# [i686]\n"
"#\n"
"# Again, the default values should work if you have a SDK compatible\n"
"# with the default values in this program.\n"
"#\n"
"# No settings should be necessary other than the SDK directory.\n"
"#\n"
"# Note that the system makes use of dependency information created by\n"
"# the compiler. If you override cflags or cxxflags, make sure that\n"
"# you have the following within those flags:\n"
"#\n"
"# -MFdepend/%output% -MMD\n"
"#\n"
;
static const char dfile2[]=
"\n"
"package %package_name%;\n"
"\n"
"import android.app.Activity;                                                    \n"
"import android.content.pm.ActivityInfo;                                         \n"
"import android.app.NativeActivity;                                              \n"
"import android.os.Environment;                                                  \n"
"import android.content.Intent;                                                  \n"
"import android.content.ClipData;                                                \n"
"import android.database.Cursor;                                                 \n"
"import android.graphics.BitmapFactory;                                          \n"
"import android.net.Uri;                                                         \n"
"import android.os.Bundle;                                                       \n"
"import android.provider.MediaStore;                                             \n"
"import android.util.DisplayMetrics;                                             \n"
"import android.content.ActivityNotFoundException;                               \n"
"import java.util.ArrayList;       \n"
"\n"
"public class MainActivity extends NativeActivity\n"
"{\n"
"/*  static { System.loadLibrary(\"main\"); } */\n"
"\n"
"  public static native void haveresult(int rcode);\n"
"  public int getDisplayDPI()\n"
"  {\n"
"    DisplayMetrics dm = new DisplayMetrics();\n"
"    getWindowManager().getDefaultDisplay().getMetrics(dm);\n"
"    float xDpi = dm.xdpi;\n"
"    float yDpi = dm.ydpi;\n"
"    if (xDpi>yDpi) return (int) xDpi; else return (int) yDpi;\n"
"  }\n"
"                                                         \n"
"  public int getStatusBarHeight() \n"
"  {\n"
"    int result= 0; \n"
"    int resourceId= getResources().getIdentifier             \n"
"                          (\"status_bar_height\", \"dimen\", \"android\");\n"
"    if (resourceId > 0)                                 \n"
"    {                                                          \n"
"        result= getResources().getDimensionPixelSize(resourceId); \n"
"    }                 \n"
"    return result;\n"
"  }\n"
"\n"
"                                    \n"
"  public int getNavigationBarHeight()                                           \n"
"  {                                                                             \n"
"    int result= 0;                                                              \n"
"    int resourceId= getResources().getIdentifier                                \n"
"                          (\"navigation_bar_height\", \"dimen\", \"android\");        \n"
"    if (resourceId > 0)                                                         \n"
"    {                                                                           \n"
"        result= getResources().getDimensionPixelSize(resourceId);               \n"
"    }                                                                           \n"
"    return result;                                                              \n"
"  }                                        \n"
"\n"
"  public String getDownloadDirectory()\n"
"  {\n"
"    return Environment\n"
"           .getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS)\n"
"           .getAbsolutePath(); \n"
"  }\n"
"}\n"
"\n"
;
static const char dfile3[]=
"<?xml version=\"1.0\" encoding=\"utf-8\"?>                                          \n"
"<resources>\n"
"</resources>    \n"
;
static const char dfile4[]=
"[project]\n"
"name=com.dodo.android_test\n"
"android_version= 26\n"
"apkfile= AndroidTest.apk\n"
"display_name=Android Test\n"
"orientation=landscape\n"
"permissions= WRITE_EXTERNAL_STORAGE INTERNET ACCESS_NETWORK_STATE\n"
"\n"
"[paths]\n"
"sdk= /home/dodo/Android/Sdk\n"
"studio= /home/dodo/android-studio\n"
"\n"
;
static const char dfile5[]=
{
48,130,10,214,2,1,3,48,130,10,128,6,9,42,134,72,134,247,13,1,7,1,160,130,10,
113,4,130,10,109,48,130,10,105,48,130,5,192,6,9,42,134,72,134,247,13,1,7,1,
160,130,5,177,4,130,5,173,48,130,5,169,48,130,5,165,6,11,42,134,72,134,247,13,
1,12,10,1,2,160,130,5,64,48,130,5,60,48,102,6,9,42,134,72,134,247,13,1,5,13,
48,89,48,56,6,9,42,134,72,134,247,13,1,5,12,48,43,4,20,138,112,10,60,42,179,
96,71,177,254,220,101,64,18,57,38,5,148,19,126,2,2,39,16,2,1,32,48,12,6,8,42,
134,72,134,247,13,2,9,5,0,48,29,6,9,96,134,72,1,101,3,4,1,42,4,16,216,177,68,
137,230,173,91,246,184,179,200,72,187,45,157,87,4,130,4,208,233,61,228,223,58,
230,214,167,48,25,137,175,224,238,131,228,78,14,21,1,94,240,230,218,208,46,
165,172,75,165,74,91,57,157,236,201,241,27,111,169,101,12,96,32,7,83,194,172,
73,24,96,8,179,255,5,86,121,80,57,34,52,61,209,207,201,206,96,133,160,59,255,
208,237,240,215,49,209,154,255,251,247,123,6,56,239,37,22,119,155,170,186,205,
122,15,79,177,49,120,252,57,43,164,121,131,49,66,248,172,170,108,237,96,85,34,
249,48,42,195,111,85,114,122,77,52,166,181,30,140,214,128,162,110,43,42,139,
129,104,12,4,241,101,252,87,162,93,255,185,220,113,187,143,44,79,39,205,221,
217,83,169,65,190,56,126,77,59,148,30,222,11,253,239,208,24,252,190,218,49,
178,75,68,250,70,126,22,98,73,219,158,78,78,25,119,205,64,17,58,42,199,222,
216,113,19,113,17,154,0,193,191,31,213,203,10,95,34,248,89,105,203,160,248,
189,204,114,58,0,136,94,8,48,113,26,186,128,231,121,33,230,132,118,43,154,233,
190,72,8,190,73,90,239,50,104,35,81,154,171,217,10,80,192,12,26,168,82,202,9,
20,166,87,23,98,29,127,124,206,202,112,155,91,146,49,22,161,38,155,189,135,12,
30,86,62,124,83,83,234,237,127,21,83,127,69,194,25,214,134,210,169,248,166,
234,61,74,91,63,51,201,72,170,174,164,130,83,123,106,6,12,239,0,127,252,30,
116,224,187,98,13,34,118,98,213,50,31,53,39,132,70,76,219,156,44,5,57,23,52,
170,9,79,192,186,186,8,44,219,168,178,200,108,4,198,73,139,85,77,16,25,227,47,
86,13,175,59,117,50,243,138,0,27,124,208,208,143,212,87,83,22,15,178,107,60,
54,110,222,189,242,35,42,40,38,41,193,33,213,104,253,173,191,25,235,237,250,
89,190,61,236,21,224,62,10,5,146,57,221,90,171,199,181,16,151,186,129,15,179,
164,163,83,233,244,243,111,185,20,55,206,107,154,146,64,141,153,232,81,217,54,
104,130,30,235,250,148,62,22,159,70,175,203,118,222,115,192,229,103,177,120,
25,217,206,125,255,12,75,159,226,205,220,141,188,87,9,195,235,7,119,250,94,
251,186,107,156,239,238,116,221,140,153,16,254,8,90,142,152,194,167,106,21,98,
189,190,158,116,251,145,229,21,185,173,75,59,9,228,178,125,186,115,108,62,104,
210,30,163,32,58,207,46,194,111,233,151,134,140,205,134,218,112,233,9,181,33,
144,53,175,64,100,221,89,30,49,92,197,90,123,211,242,204,29,24,55,184,39,12,
77,139,225,113,27,76,191,168,66,142,121,105,194,191,181,243,37,175,124,76,64,
182,32,194,66,121,199,222,2,33,74,66,231,139,35,61,1,60,174,207,215,129,55,
199,165,33,19,245,245,49,153,238,162,19,30,205,136,142,149,251,238,80,41,103,
106,20,156,70,142,41,95,41,244,35,137,207,167,6,103,214,144,206,121,76,82,158,
177,129,5,17,130,149,111,241,109,142,12,128,247,19,133,124,0,6,29,166,100,182,
102,184,57,236,114,195,201,3,252,21,50,44,184,245,89,43,196,89,16,15,83,30,47,
62,101,77,29,73,62,70,118,112,45,43,25,25,41,204,121,205,27,39,137,24,109,99,
19,25,246,180,36,100,228,233,61,107,36,232,53,92,63,39,81,231,82,126,138,18,
50,152,209,50,66,205,52,49,101,58,141,231,231,25,10,60,11,183,158,122,82,66,
188,134,215,124,178,95,211,8,69,211,238,177,178,148,27,79,100,72,45,84,201,
124,222,36,114,193,218,179,41,29,207,200,191,86,103,58,219,105,95,153,208,156,
93,85,60,7,196,210,144,195,224,155,98,203,17,141,184,128,212,147,68,224,139,
161,158,155,169,254,239,25,54,26,38,185,242,225,56,14,77,226,239,181,238,93,
165,246,153,47,171,88,115,126,55,42,187,31,27,136,178,3,242,206,87,3,141,232,
127,139,225,35,246,254,121,121,4,190,125,243,189,106,138,37,152,205,240,211,
231,71,67,234,122,253,202,3,134,3,223,184,35,26,108,55,56,132,54,179,111,176,
28,227,185,72,141,222,43,147,202,175,196,216,27,236,144,72,234,212,219,235,
124,234,106,57,230,147,186,42,71,177,23,173,42,209,43,110,12,28,132,202,57,
215,187,194,224,231,64,49,238,206,28,79,187,157,4,247,154,98,60,225,91,251,33,
12,151,241,144,193,155,100,82,59,29,187,33,244,119,120,118,67,143,75,116,28,
166,198,4,47,74,178,93,157,76,131,4,81,130,165,203,196,7,145,137,111,5,245,
237,91,120,3,117,109,98,215,185,233,197,104,123,9,83,120,178,1,90,159,254,224,
255,59,196,190,63,107,214,52,35,254,51,72,63,200,111,56,109,45,106,20,190,225,
123,115,229,30,70,7,78,187,117,74,227,18,228,41,192,161,225,156,128,65,8,36,
103,202,124,168,75,19,49,243,105,127,189,1,170,143,22,88,224,135,210,184,8,38,
216,192,151,49,27,249,226,95,100,217,170,1,198,136,181,34,248,33,43,92,103,
142,193,235,105,17,152,111,157,255,11,108,63,238,172,134,70,87,21,224,162,207,
30,235,198,37,106,121,95,234,19,99,31,117,253,35,195,112,9,167,12,140,253,73,
22,180,137,2,237,212,88,227,104,209,184,254,79,240,175,57,131,52,83,152,42,
154,155,15,122,72,57,217,28,74,92,32,142,53,29,99,163,42,206,134,49,82,48,45,
6,9,42,134,72,134,247,13,1,9,20,49,32,30,30,0,97,0,110,0,100,0,114,0,111,0,
105,0,100,0,100,0,101,0,98,0,117,0,103,0,107,0,101,0,121,48,33,6,9,42,134,72,
134,247,13,1,9,21,49,20,4,18,84,105,109,101,32,49,55,48,56,56,54,52,53,51,49,
52,52,54,48,130,4,161,6,9,42,134,72,134,247,13,1,7,6,160,130,4,146,48,130,4,
142,2,1,0,48,130,4,135,6,9,42,134,72,134,247,13,1,7,1,48,102,6,9,42,134,72,
134,247,13,1,5,13,48,89,48,56,6,9,42,134,72,134,247,13,1,5,12,48,43,4,20,59,
155,5,198,180,253,80,30,178,28,252,108,141,246,203,235,234,130,16,101,2,2,39,
16,2,1,32,48,12,6,8,42,134,72,134,247,13,2,9,5,0,48,29,6,9,96,134,72,1,101,3,
4,1,42,4,16,60,212,75,112,251,84,127,23,13,115,4,169,182,92,103,140,128,130,4,
16,174,224,52,205,211,195,60,109,56,234,78,244,79,12,57,224,224,72,55,250,203,
158,217,127,164,99,151,3,75,72,116,97,158,148,177,171,36,87,194,30,194,50,216,
142,45,249,66,103,227,26,80,201,137,233,11,220,159,29,246,136,90,252,58,55,
113,104,39,76,255,233,198,38,207,235,183,131,227,4,18,117,122,205,68,184,218,
224,47,233,4,78,27,4,29,163,111,227,67,59,155,62,79,245,113,247,120,236,217,
157,253,231,222,240,224,215,15,80,18,125,164,26,73,125,43,149,74,40,36,176,84,
232,113,28,241,192,222,179,139,236,222,167,92,175,39,144,95,81,118,92,116,18,
19,24,101,32,229,141,59,33,51,185,185,12,103,165,191,112,188,10,71,0,104,100,
154,150,31,119,6,54,45,128,212,162,215,156,113,24,230,221,125,109,212,148,206,
118,205,39,142,240,232,129,238,92,92,107,69,173,83,52,190,39,12,232,28,209,
181,88,42,54,58,154,195,147,212,179,100,205,72,0,145,26,219,80,237,215,63,3,
19,123,39,15,26,124,32,203,134,57,219,28,241,148,141,24,50,59,4,218,67,91,244,
112,111,1,86,62,95,56,83,159,10,64,190,53,151,182,251,150,114,85,209,232,149,
234,170,250,82,21,180,111,198,125,147,0,121,205,123,41,76,214,160,220,206,47,
49,254,252,60,149,190,255,140,210,43,134,85,233,74,206,19,101,216,110,91,191,
249,0,63,220,23,246,86,29,230,93,95,155,187,93,169,168,35,160,143,70,104,187,
66,166,109,207,12,178,155,96,219,66,77,57,106,174,113,180,239,119,185,77,108,
129,74,79,58,56,244,44,197,198,28,170,219,2,155,55,115,233,74,120,184,146,9,
127,107,140,147,190,109,188,5,209,175,11,231,187,164,234,115,249,109,174,50,
144,119,3,71,184,66,146,95,98,238,181,123,213,239,139,109,192,233,9,15,196,
172,105,139,87,151,157,230,233,71,251,94,151,19,169,82,160,206,100,85,131,14,
147,13,101,14,150,180,66,94,56,156,163,102,56,71,0,63,219,177,38,154,95,50,
197,100,28,210,32,147,202,141,182,62,143,219,104,175,210,46,35,127,80,43,239,
85,38,97,94,17,199,204,192,187,76,235,62,170,243,198,139,236,114,234,231,203,
101,46,236,146,162,215,211,67,98,63,89,95,176,113,107,175,35,231,119,77,149,
215,208,207,60,175,71,166,40,198,74,180,46,16,231,124,195,5,161,96,179,148,58,
107,88,167,170,205,111,206,96,239,119,220,33,125,166,69,172,235,113,161,59,
145,57,150,204,123,73,60,50,74,190,217,70,248,30,115,78,109,74,74,178,133,46,
253,224,187,111,106,68,194,225,193,41,249,94,93,218,104,248,136,50,130,133,
192,41,104,145,243,181,87,60,202,249,222,7,95,154,249,88,41,12,96,151,190,103,
81,25,55,55,61,200,92,48,198,89,43,64,231,225,151,104,58,131,5,18,64,92,126,
212,182,81,129,35,189,68,200,119,162,27,135,51,194,56,125,124,19,132,109,200,
76,92,146,199,245,192,208,228,97,96,213,110,173,39,91,4,72,91,134,72,128,63,
126,86,156,108,170,127,201,237,234,148,87,144,37,181,42,14,200,2,16,20,79,225,
84,68,85,62,14,228,141,3,214,133,200,144,38,4,162,86,135,149,89,244,174,167,
131,184,151,220,88,105,205,255,168,3,60,129,131,164,66,188,143,212,255,202,
155,68,188,76,32,254,115,27,161,92,8,169,194,1,192,48,99,148,105,210,59,219,
54,79,178,196,28,99,16,118,180,132,249,121,133,164,26,1,9,234,49,194,233,22,
32,115,86,108,124,122,88,38,83,211,52,192,192,211,174,77,30,212,7,72,209,196,
241,191,255,71,26,97,13,190,51,175,125,112,110,231,40,143,124,196,73,9,98,182,
183,187,40,239,220,79,32,194,142,214,186,7,49,179,239,184,197,176,240,20,220,
88,232,35,253,67,104,49,184,221,68,35,2,244,54,137,195,146,192,75,155,28,154,
20,49,221,80,193,241,211,176,193,210,221,64,38,141,245,252,39,92,138,71,28,39,
55,219,251,234,216,11,201,126,1,238,175,165,10,130,135,229,169,20,207,247,231,
22,164,174,5,192,136,135,212,136,255,183,112,39,59,156,7,44,112,86,53,232,80,
236,164,23,94,61,83,21,192,128,57,159,53,114,63,22,218,12,169,94,172,84,67,98,
255,35,109,183,7,14,182,41,58,87,126,18,116,6,118,49,118,63,51,133,119,149,18,
224,210,224,221,85,129,216,156,214,63,206,140,149,98,234,163,249,76,100,103,
56,193,224,74,37,177,0,48,77,48,49,48,13,6,9,96,134,72,1,101,3,4,2,1,5,0,4,32,
151,65,244,134,155,125,159,34,101,1,112,221,122,34,133,174,68,10,215,9,238,
155,18,130,142,22,85,169,252,41,252,249,4,20,109,210,169,79,35,74,129,61,25,
237,32,36,5,194,238,221,200,53,11,145,2,2,39,16,
}
;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const char *data_get(const char *fn, size_t *Rsize)
{
  static const struct {
    const char *name, *value;
    size_t size;
  } data[]= {
 { "AndroidManifest.xml", dfile0, 1230ull },
 { "amak.ini", dfile1, 4705ull },
 { "MainActivity.java", dfile2, 3080ull },
 { "strings.xml", dfile3, 111ull },
 { "minimal.ini", dfile4, 269ull },
 { "debug.keystore", dfile5, 2778ull },
  { NULL, NULL } };
  int i;
  for(i=0;data[i].name;i++)
    if (!strcmp(data[i].name,fn)) 
    {
       if (Rsize) *Rsize= data[i].size;
       return data[i].value;
    }
  return NULL;
}
