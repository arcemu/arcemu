--[[ QUEST - Hillsbrad Foothills - Wanted Baron Vardus.lua

********************************
*                                                            *
* The Moon++ Scripting Project        *
*                                                            *
********************************

This software is provided as free and open source by the
staff of The Moon++ Scripting Project, in accordance with
the GPL license. This means we provide the software we have
created freely and it has been thoroughly tested to work for
the developers, but NO GUARANTEE is made it will work for you
as well. Please give credit where credit is due, if modifying,
redistributing and/or using this software. Thank you.

Staff of The Moon++ Scripting Project, December 2007
~~End of License Agreement

All credits for the files in this folder that are GameMonkey scripts
go to the GMScripts project, for all their hard work for the Ascent
community.

-- Moon++ Project, December 2007 ]]

AllowSpawn = 1

function Baron_Vardus_Quest_onAccept (pUnit, Event)
   if (AllowSpawn == 1) then
      spin = math.random (1, 4)
      
      if (spin == 1) then
         pUnit:SpawnCreature (2306, 692.64, -904.74, 157.79, 1.17511, 14, 1800000)
         AllowSpawn = 0
      elseif (spin == 2) then
         pUnit:SpawnCreature (2306, 939.0, -852.46, 114.644, 1.17511, 14, 1800000)
         AllowSpawn = 0
      elseif (spin == 3) then
         pUnit:SpawnCreature (2306, 1184.07, -553.43, 71.3346, 1.17511, 14, 1800000)
         AllowSpawn = 0
      elseif (spin == 4) then
         pUnit:SpawnCreature (2306, 1001.20, -793.93, 108.65, 1.17511, 14, 1800000)
         AllowSpawn = 0
      else
         print ("Error: QUEST - Hillsbrad Foothills - Wanted Baron Vardus.lua: function block Baron_Vardus_Quest_onAccept() - invalid number rolled")
      end
   end
end

function Baron_Vardus_onDead (pUnit, Event)
   AllowSpawn = 1
end

--RegisterQuestEvent(566, 2,"BaronVardus_OnAcceptQuest");  <<<<<<<<<<<<<<<<< Unimplemented by Ascent. Therefore, no way to spawn him on quest accept
RegisterUnitEvent (2306, 6, "Baron_Vardus_Quest_onAccept") --would be similar to how the quest accept event would look like, except as a unit event. Needs ascent work.
RegisterUnitEvent (2306, 4, "Baron_Vardus_onDead") 