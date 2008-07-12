--[[ UNIT - Ashenvale - Generic.lua

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

function Talen_onSpawn (pUnit, Event)
	pUnit:Emote (13)
end
RegisterUnitEvent (3846, 6, "Talen_onSpawn")
RegisterUnitEvent (3846, 3, "Talen_onSpawn")
RegisterUnitEvent (3846, 2, "Talen_onSpawn")

function Talen_onCombat (pUnit, Event)
	pUnit:Emote (26)
end
RegisterUnitEvent (3846, 1, "Talen_onCombat")