--[[ UNIT - Orgrimarr - Generic.lua

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

Staff of The Moon++ Scripting Project, November 2007
~~End of License Agreement

All credits for the files in this folder that are GameMonkey scripts
go to the GMScripts project, for all their hard work for the Ascent 
community.

-- Moon++ Project, November 2007 ]]

function Thrall_onCombat (pUnit, Event)
	pUnit:PlaySoundToSet (5880)
end

function OrgrimmarGruntOnCombat(pUnit, Event)
	pUnit:SendChatMessage(11, 0, "For the Warchief!");
end

RegisterUnitEvent(3296, 1, "OrgrimmarGruntOnCombat");
RegisterUnitEvent (4949, 1, "Thrall_onCombat") 