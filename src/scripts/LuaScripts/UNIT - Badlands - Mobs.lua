 --[[
   ******************************************************************
   *	 _____              ___                           _         *
   *	(  _  )            (  _`\               _        ( )_       *
   *	| (_) | _ __   ___ | (_(_)   ___  _ __ (_) _ _   | ,_)      *
   *	|  _  |( '__)/'___)`\__ \  /'___)( '__)| |( '_`\ | |        *
   *	| | | || |  ( (___ ( )_) |( (___ | |   | || (_) )| |_       *
   *	(_) (_)(_)  `\____)`\____)`\____)(_)   (_)| ,__/'`\__)      *
   *	                                          | |               *
   *	                                          (_)               *
   *	                                                            *
   *	               OpenSource Scripting Team                    *
   *	                <http://www.arcemu.org>                     *
   *	                                                            *
   ******************************************************************
  
   This software is provided as free and open source by the
staff of The ArcScript Project, in accordance with 
the GPL license. This means we provide the software we have 
created freely and it has been thoroughly tested to work for 
the developers, but NO GUARANTEE is made it will work for you 
as well. Please give credit where credit is due, if modifying,
redistributing and/or using this software. Thank you.

Staff of ArcScript Project, Feb 2008
~~End of License Agreement

#############################################################

-- ]]

-- http://www.wowhead.com/?zone=3
-- Total Units Complete = 9/90
function AmbassadorInfernus_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("AmbassadorInfernus_FireNova", 16000, 0)
	pUnit:RegisterEvent("AmbassadorInfernus_FireShieldIV", 24000, 0)
	pUnit:RegisterEvent("AmbassadorInfernus_SearingFlames", 13000, 0)
end

function AmbassadorInfernus_FireNova(pUnit,Event)
	pUnit:CastSpell(11970)
end

function AmbassadorInfernus_FireShieldIV(pUnit,Event)
	pUnit:CastSpell(2602)
end

function AmbassadorInfernus_SearingFlames(pUnit,Event)
	local searingCheck = pUnit:GetClosestPlayer()
	if (searingCheck ~= nil) then
	else
		pUnit:FullCastSpellOnTarget(9552,pUnit:GetClosestPlayer())
	end
end

function AmbassadorInfernus_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function AmbassadorInfernus_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(2745, 1, "AmbassadorInfernus_OnEnterCombat")
RegisterUnitEvent(2745, 2, "AmbassadorInfernus_OnLeaveCombat")
RegisterUnitEvent(2745, 4, "AmbassadorInfernus_OnDied")

function Blacklash_OnEnterCombat(pUnit,Event)
	pUnit:RegsiterEvent("Blacklash_FlameBreath", 4500, 0)
end

function Blacklash_FlameBreath(pUnit,Event)
	pUnit:FullCastSpellOnTarget(9573,pUnit:GetClosestPlayer())
end

function Blacklash_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Blacklash_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(2757, 1, "Blacklash_OnEnterCombat")
RegisterUnitEvent(2757, 2, "Blacklash_OnLeaveCombat")
RegisterUnitEvent(2757, 4, "Blacklash_OnDied")

function Buzzard_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Buzzard_InfectedWound", 38000, 0)
end

function Buzzard_InfectedWound(pUnit,Event)
	pUnit:FullCastSpellOnTarget(3427,pUnit:GetClosestPlayer())
end

function Buzzard_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Buzzard_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(2830, 1, "Buzzard_OnEnterCombat")
RegisterUnitEvent(2830, 2, "Buzzard_OnLeaveCombat")
RegisterUnitEvent(2830, 4, "Buzzard_OnDied")