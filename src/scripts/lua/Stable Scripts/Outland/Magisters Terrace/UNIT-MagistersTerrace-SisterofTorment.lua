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

function SunbladeSisterofTorment_OnCombat(Unit, Event)
	Unit:RegisterEvent("SunbladeSisterofTorment_DeadlyEmbrace", 10000, 0)
	Unit:RegisterEvent("SunbladeSisterofTorment_LashofPain",7000, 0) 
end

--Deadly Embrace
function SunbladeSisterofTorment_DeadlyEmbrace(Unit, Event)
	if Unit:GetRandomPlayer(1) then
		Unit:FullCastSpellOnTarget(44547, Unit:GetRandomPlayer(1))
	end
end

--44640 Lash of Pain
function SunbladeSisterofTorment_LashofPain(Unit, Event)
	local Flip = math.random(2)
	if Flip == 1 and Unit:GetClosestPlayer() then
		Unit:CastSpellOnTarget(44640, Unit:GetClosestPlayer())
	end
end


function SunbladeSisterofTorment_LeaveCombat(Unit, Event)
	Unit:RemoveEvents()
end

function SunbladeSisterofTorment_Died(Unit, Event)
	Unit:RemoveEvents()
end


RegisterUnitEvent(24697, 1, "SunbladeSisterofTorment_OnCombat")
RegisterUnitEvent(24697, 2, "SunbladeSisterofTorment_LeaveCombat")
RegisterUnitEvent(24697, 4, "SunbladeSisterofTorment_Died")
