#! /bin/sh
#
#  Converting text files from CR/LF to LF format.


echo "Converting files to Unix format..."

find . -type f "(" \
   -name "*.c" -o -name "*.cpp" -o -name "*.h" -o -name "*.s" -o \
   -name "*.asm" -o -name "*.inc" -o -name "*.vc" -o -name "*.ls" -o \
   -name "*.acs" -o -name "*.cfg" -o -name "*.txt" -o -name "*.vs" -o \
   -name "*.mak" -o -name "*.mgw" -o \
   -name "makefile.*" -o -name "makefile" -o -name "Makefile" \
   ")" \
   -exec sh -c "echo {};
                mv {} _tmpfile;
                tr -d \\\r < _tmpfile > {};
                touch -r _tmpfile {};
                rm _tmpfile" \;

echo "Done!"
