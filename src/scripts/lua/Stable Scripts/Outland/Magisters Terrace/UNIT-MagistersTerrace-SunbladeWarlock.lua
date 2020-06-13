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

function SunbladeWarlock_OnCombat(Unit, Event)
--He summons the Imp
	Unit:CastSpell(44517)
--Cast Events don't know how long the cooldown is.
	Unit:RegisterEvent("SunbladeWarlock_Incinerate", 5000, 0)
--46042 Immolate
	Unit:RegisterEvent("SunbladeWarlock_Immolate", 3500, 0)
end

function SunbladeWarlock_Incinerate(Unit, Event)
	if Unit:GetClosestPlayer() then
		Unit:FullCastSpellOnTarget(46043, Unit:GetClosestPlayer())
	end
end

function SunbladeWarlock_Immolate(Unit, Event)
	local Flip = math.random(3)
	if Flip == 1 and Unit:GetMainTank() then
		Unit:FullCastSpellOnTarget(46042, Unit:GetMainTank())
	end
end

function SunbladeWarlock_LeaveCombat(Unit, Event)
	Unit:RemoveEvents()
end

function SunbladeWarlock_Died(Unit, Event)
	Unit:RemoveEvents()
end


RegisterUnitEvent(24686, 1, "SunbladeWarlock_OnCombat")
RegisterUnitEvent(24686, 2, "SunbladeWarlock_LeaveCombat")
RegisterUnitEvent(24686, 4, "SunbladeWarlock_Died")
