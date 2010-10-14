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

function SunbladeMageGuard_OnCombat(Unit, Event)
	Unit:RegisterAIUpdateEvent(7000)
end

function SunbladeMageGuard_GlaiveThrow(Unit)
	local FlipGlaive = math.random(2)
	if FlipGlaive ==1 and Unit:GetRandomPlayer(7) then
		Unit:CastSpellOnTarget(44478, Unit:GetRandomPlayer(7))
	else
		Unit:CastSpellOnTarget(46028, Unit:GetRandomPlayer(7))
	end
end

function SunbladeMageGuard_LeaveCombat(Unit, Event)
	Unit:RemoveAIUpdateEvent()
end

function SunbladeMageGuard_Died(Unit, Event)
	Unit:RemoveAIUpdateEvent()
end



RegisterUnitEvent(24683, 1, "SunbladeMageGuard_OnCombat")
RegisterUnitEvent(24683, 21, "SunbladeMageGuard_GlaiveThrow")
RegisterUnitEvent(24683, 2, "SunbladeMageGuard_LeaveCombat")
RegisterUnitEvent(24683, 4, "SunbladeMageGuard_Died")
