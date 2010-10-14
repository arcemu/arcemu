#!/bin/sh

# Created by devnull
echo "ArcScripts installer for ArcEmu"

# Convert all MS-DOS formated filesto unix format
find ./src/scripts/ -not -iname "*.svn*" -type f | sed 's/^/\"/g;s/$/\"/g' | xargs -0 | xargs dos2unix

line=`grep -n "src/arcemu-realmserver/Makefile" ../../configure.ac | cut -d: -f1`

echo "Removing scripts entries from config.ac ..."
cat ../../configure.ac | grep -v 'src/scripts' > ../../config.arc.new

echo "Inserting arc++ scripts on config file ..."
sed -e "$(( line + 1 ))"i'	src/scripts/src/Makefile' ../../config.arc.new > ../../config.arc.new1
sed -e "$(( line + 1 ))"i'	src/scripts/src/EventScripts/Makefile' ../../config.arc.new1 > ../../config.arc.new2
sed -e "$(( line + 1 ))"i'	src/scripts/src/ExtraScripts/Makefile' ../../config.arc.new2 > ../../config.arc.new3
sed -e "$(( line + 1 ))"i'	src/scripts/src/GossipScripts/Makefile' ../../config.arc.new3 > ../../config.arc.new4
sed -e "$(( line + 1 ))"i'	src/scripts/src/InstanceScripts/Makefile' ../../config.arc.new4 > ../../config.arc.new5
sed -e "$(( line + 1 ))"i'	src/scripts/src/LuaEngine/Makefile' ../../config.arc.new5 > ../../config.arc.new6
sed -e "$(( line + 1 ))"i'	src/scripts/src/MiscScripts/Makefile' ../../config.arc.new6 > ../../config.arc.new7
sed -e "$(( line + 1 ))"i'	src/scripts/src/QuestScripts/Makefile' ../../config.arc.new7 > ../../config.arc.new8
sed -e "$(( line + 1 ))"i'	src/scripts/src/ServerStatusPlugin/Makefile' ../../config.arc.new8 > ../../config.arc.new9
sed -e "$(( line + 1 ))"i'	src/scripts/src/SpellHandlers/Makefile' ../../config.arc.new9 > ../../config.arc.new10

# Doesn't compile with Arc (+ they're crap :) )
#sed -e "$(( line + 1 ))"i'	#src/scripts/src/WorldPvPScripts/Makefile' ../../config.arc.new10 #> ../../config.arc.new11

mv ../../config.arc.new10 ../../configure.ac
rm -f ../../config.arc.*

echo "Replacing the scripts entry in Makefile.am for scripts entry (you have to use scripts directory name for this to work) ..."
sed "s/scripts/arc++\/src/g" ../Makefile.am > makefile.arc.tmp && mv stylele.arc.tmp ../Makefile.am

# Convert all MS-DOS formated filesto unix format
find ./src/scripts/ -not -iname "*.svn*" -type f | sed 's/^/\"/g;s/$/\"/g' | xargs -0 | xargs dos2unix

echo "Finished"
echo "Now, just go to ArcEmu's root dir and run make && make to finish"
