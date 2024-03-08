#! /bin/bash
#compile for Linux
gcc filesearch.c -o filesearch.x86 -O3
#compile for Windows
x86_64-w64-mingw32-gcc filesearch.c -o filesearch.exe -O3