***************************
*      .-.                *
*      `-.. ..-. + +      *
*      `-''-'' '          *
*  OpenSource Scripting   *
*          Team           *
* http://sunplusplus.info *
*                         *
***************************
This software is provided as free and open source by the
staff of the Sun++ Scripts Project, in accordance with 
the GPL license. This means we provide the software we have 
created freely and it has been thoroughly tested to work for 
the developers, but NO GUARANTEE is made it will work for you 
as well. Please give credit where credit is due, if modifying,
redistributing and/or using this software. Thank you.

Sun++ Team, July 2008
~~End of License Agreement
--------------------------------------------------------------------------------

How to install Sun++ scripts:

1) Make sure to compile the LATEST ArcEmu revisions. Leave a copy of the
compiled emulator in the ArcEmu/bin/release folder. The scripts will need
these files to compile.

2) Create a directory called SunScripts or Sun++ inside your ArcEmu/src folder. 

3) Check out this folder (http://svn.assembla.com/svn/sunplusplus/Trunk) into the 
newly created SunScripts folder.

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
**ATTENTION, IF YOU HAVE CHECKED OUT OUR REPOSITY WITH THE ADDRESS ON OUR WEBPAGE,
"http://svn.assembla.com/svn/sunplusplus/" YOU MUST NOT PUT THE WHOLE FOLDER INTO
SRC, ONLY TRUNK. IF YOU HAVE DOWNLOADED FROM WEBSITE, MOVE TRUNK TO SRC.*********
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Windows:
4) Now open up one of the Visual Studio solutions and set the build configuration
to Release. Then compile. All the scripts should compile properly.

Linux:
4) Open ArcEmu\src\makefile.am and replace "scripts" with "SunScripts", like this:
SUBDIRS = ascent-shared ascent-logonserver ascent-world ascent-voicechat ascent-realmserver SunScripts
Then compile.

5) Apply sql\SunPlusPlus_full.sql to your world database.

(Note: C++ "scripts" can *NOT* be hot swapped, meaning you must restart the server
to use them).
