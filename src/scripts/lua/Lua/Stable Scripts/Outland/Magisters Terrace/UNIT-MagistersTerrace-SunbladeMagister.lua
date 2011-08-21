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

function SunbladeMagister_OnCombat(Unit, Event)
	Unit:RegisterAIUpdateEvent(3000)
end

function SunbladeMagister_Frostbolt(Unit, Event)
	local plr = Unit:GetRandomPlayer(1)
	if plr then
		Unit:FullCastSpellOnTarget(46035,plr)
	end
end

function SunbladeMagister_ArcaneNova(Unit)
	local arcaneflip = math.random(6)
	local plr = Unit:GetRandomPlayer(7)
	if arcaneflip == 1 and plr ~= nil then
		Unit:FullCastSpellOnTarget(46036,plr)
	else
	end
end

function SunbladeMagister_LeaveCombat(Unit)
	Unit:RemoveEvents()
	Unit:RemoveAIUpdateEvent()
end

function SunbladeMagister_Died(Unit)
	Unit:RemoveEvents()
	Unit:RemoveAIUpdateEvent()
end


RegisterUnitEvent(24685, 1, "SunbladeMagister_OnCombat")
RegisterUnitEvent(24685, 21,"SunbladeMagister_Frostbolt")
RegisterUnitEvent(24685, 21,"SunbladeMagister_ArcaneNova")
RegisterUnitEvent(24685, 2, "SunbladeMagister_LeaveCombat")
RegisterUnitEvent(24685, 4, "SunbladeMagister_Died")
