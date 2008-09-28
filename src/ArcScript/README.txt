********************************
*                              *
*      The ArcScript Project        *
*                              *
********************************

This software is provided as free and open source by the
staff of The ArcScript Project, in accordance with 
the GPL license. This means we provide the software we have 
created freely and it has been thoroughly tested to work for 
the developers, but NO GUARANTEE is made it will work for you 
as well. Please give credit where credit is due, if modifying,
redistributing and/or using this software. Thank you.

Staff of ArcScript Project, Feb 2008
~~End of License Agreement

#############################################################

How to install ArcScript scripts:

1) Make sure to compile the LATEST Ascent revisions. Leave a copy of the
compiled emulator in the Ascent/bin/release folder. The scripts will need
these files to compile.

2) Create a directory called ArcScripts inside your Ascent/src folder. 


Windows:
3) Now open up one of the Visual Studio solutions and set the build configuration
to Release. Then compile. All the scripts should compile properly.

Linux:
3) Open ascent\src\makefile.am and replace "scripts" with "ArcScriptScripts", like this:
SUBDIRS = ascent-shared ascent-logonserver ascent-world ascent-voicechat ascent-realmserver ArcScriptScripts
Then compile.

4) Apply sql\ArcScript_full.sql to your world database.

(Note: C++ "scripts" can *NOT* be hot swapped, meaning you must restart the server
to use them).
