#!/usr/bin/env sh
# Ascent 2007, Aim

# WARNING:
#  Running this script will wipe all previous svn:ignore propsets

if [ ! -f configure ] ; then
  echo "You must run this script in the root of the trunk."
  exit 1
fi

echo "Updating eol-styles"

NEWLINE='
'
IFS=$NEWLINE

for i in `find . \
  -name "*.sh" -o \
  -name "*.h" -o \
  -name "*.c" -o \
  -name "*.cpp" -o \
  -name "Makefile.am" -o \
  -name "configure.ac" -o \
  -name "*.txt" -o \
  -name "*.sql" -o \
  -name "*.conf" -o \
  -name "*.gm" -o \
  -name "*.patch" -o \
  -name "*.html"`; do
        EOLSTYLE=$(svn pg svn:eol-style $i)
        if [ $? -eq 0 ] && [ -z "$EOLSTYLE" ] ; then
		# first remove the returns just incase. (fucking windows)
		tr -d '\r' < $i > ${i}.tmp 
		mv ${i}.tmp $i
		svn ps svn:eol-style native $i 
        fi
done

