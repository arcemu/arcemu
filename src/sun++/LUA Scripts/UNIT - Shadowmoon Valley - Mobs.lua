--[[ UNIT - ShadowmoonValley - Mobs.lua

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

-- Moon++ Project, December 2007 ]]


function EclipsionSpawn(pUnit, Event)
	local x = pUnit:GetX();
	local y = pUnit:GetY();
	local z = pUnit:GetZ();
	local o = pUnit:GetO();
	pUnit:SpawnCreature(21627, x, y, z, o, 23000);
end

function EclipsionDespawn(pUnit, Event)
	pUnit:Despawn(1000, 0);
end

RegisterUnitEvent(19806, 1, "EclipsionSpawn");
RegisterUnitEvent(22018, 1, "EclipsionSpawn");
RegisterUnitEvent(21627, 2, "EclipsionDespawn");