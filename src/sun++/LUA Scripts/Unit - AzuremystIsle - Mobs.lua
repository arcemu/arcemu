--[[ UNIT - AzuremistIsle  - Mobs.lua

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

function InjuredDraenei_OnSpawn(pUnit, event)
	local InjuredDraeneiChance = math.random(1, 2)
	if (InjuredDraeneiChance==1) then
		pUnit:SetStandState(3)
	else
		pUnit:SetStandState(1)
		pUnit:SetCombatCapable(0)
	end
end
RegisterUnitEvent(16971, 6, "InjuredDraenei_OnSpawn")

function DraeneiSurvivor_OnSpawn(pUnit, event)
	local SurvivorDraeneiChance = math.random(1, 3)
	if (SurvivorDraeneiChance==1) then
		pUnit:SetStandState(3)
	elseif SurvivorDraeneiChance == 2 then
		pUnit:CastSpell(28630)
		pUnit:SetStandState(1)
	elseif SurvivorDraeneiChance == 3 then
		pUnit:CastSpell(28630)
		pUnit:SetCombatCapable(0)
	end
end

RegisterUnitEvent(16483, 6, "DraeneiSurvivor_OnSpawn")