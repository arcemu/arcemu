--[[
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
--Moon April 2008]]

function SunbladeKeeper_OnCombat(Unit, Event)
	Unit:RegisterAIUpdateEvent(6000)
end

function SunbladeKeeper_ShadowBolt(Unit, Event)
	if Unit:GetRandomPlayer(7) then
		Unit:FullCastSpellOnTarget(15232, Unit:GetRandomPlayer(7))
	end
end


function SunbladeKeeper_LeaveCombat(Unit, Event)
	Unit:RegisterAIUpdateEvent()
end

function SunbladeKeeper_Died(Unit, Event)
	Unit:RegisterAIUpdateEvent()
end


RegisterUnitEvent(24762, 1, "SunbladeKeeper_OnCombat")
RegisterUnitEvent(24762, 21, "SunbladeKeeper_ShadowBolt")
RegisterUnitEvent(24762, 2, "SunbladeKeeper_LeaveCombat")
RegisterUnitEvent(24762, 4, "SunbladeKeeper_Died")
