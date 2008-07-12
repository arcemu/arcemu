--[[ QUEST - Blades Edge Mountains - Into the Soulgrinder.lua

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

function Into_the_Soulgrinder_Quest_onComplete (pUnit, Event)
	pUnit:SpawnCreature (23053, 2794.978271, 5842.185547, 35.911819, 3.61522, 35, 540000)
end

--RegisterQuestEvent(11000, 1, "IntotheSoulgrinder_OnQuestComplete") <<<<<<<<<<<<<<<<<<<< Needs Ascent Functionality