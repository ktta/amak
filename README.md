# amak
Simple build system for creating Android applications

This program lets you create Android applications without using the NDK
Makefiles, Android Studio or any other complicated make system. 

The whole project is described in one INI file. Native sources live 
under native/. The system creates the initial Java sources and the
necessary source/build directory structure for you.

Usage
-----

- Clone this repo and run 'make' in the repo directory. 

$ git clone https://github.com/ktta/amak && cd amak && make

- Put the binary somewhere convenient.

$ sudo mv amak /usr/bin

- Find your Android SDK installation and Android Studio installation if
  you have it. If you don't, you will need a Java compiler from somewhere 
  else.

- Make a directory and create a new INI file in there. 

$ mkdir test && cd test && amak init

- Edit amak.ini to reflect your project.
- Generate everything and do a build.

$ amak gen && amak

- You will find the commands executed within build/amak.log
- Add your C/C++ into native/ and have a go.


