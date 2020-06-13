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

function SunbladeBloodKnight_OnCombat(Unit, Event)
	Unit:RegisterAIUpdateEvent(10000)
end

--[[-- I dont't know how often he does this just set it to 6, 3 sec--]]
function SunbladeBloodKnight_HolyLight(Unit, Event)
	Unit:FullCastSpell(27136)
end

function SunbladeBloodKnight_LeaveCombat(Unit, Event)
	Unit:RemoveAIUpdateEvent()
end

function SunbladeBloodKnight_Died(Unit, Event)
	Unit:RemoveAIUpdateEvent()
end


RegisterUnitEvent(27136, 1, "SunbladeBloodKnight_OnCombat")
RegisterUnitEvent(27136, 21, "SunbladeBloodKnight_HolyLight")
RegisterUnitEvent(27136, 2, "SunbladeBloodKnight_LeaveCombat")
RegisterUnitEvent(27136, 4, "SunbladeBloodKnight_Died")
