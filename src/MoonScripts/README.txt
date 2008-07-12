********************************
*                              *
*      The Moon Project        *
*                              *
********************************

This software is provided as free and open source by the
staff of The Moon Project, in accordance with 
the GPL license. This means we provide the software we have 
created freely and it has been thoroughly tested to work for 
the developers, but NO GUARANTEE is made it will work for you 
as well. Please give credit where credit is due, if modifying,
redistributing and/or using this software. Thank you.

Staff of Moon Project, Feb 2008
~~End of License Agreement

#############################################################

How to install Moon scripts:

1) Make sure to compile the LATEST Ascent revisions. Leave a copy of the
compiled emulator in the Ascent/bin/release folder. The scripts will need
these files to compile.

2) Create a directory called MoonScripts inside your Ascent/src folder. 

3) Check out this folder (http://mmoforge.org/svn/moon/trunk) into the 
newly created MoonScripts folder.

Windows:
4) Now open up one of the Visual Studio solutions and set the build configuration
to Release. Then compile. All the scripts should compile properly.

Linux:
4) Open ascent\src\makefile.am and replace "scripts" with "MoonScripts", like this:
SUBDIRS = ascent-shared ascent-logonserver ascent-world ascent-voicechat ascent-realmserver MoonScripts
Then compile.

5) Apply sql\moonplusplus_full.sql to your world database.

(Note: C++ "scripts" can *NOT* be hot swapped, meaning you must restart the server
to use them).
