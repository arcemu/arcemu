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
-- Scripted By:  n4xD

local AddsDead = 0

function Prince_Keleseth_OnCombat(pUnit, Event)
pUnit:SendChatMessage(14, 0, "Your blood is mine!")
pUnit:RegisterEvent("Prince_Keleseth_Shadow_Bolt", math.random(13000, 15000), 0)
pUnit:RegisterEvent("Prince_Keleseth_Frost_Tomb", math.random(35000, 37000), 0)
pUnit:RegisterEvent("Prince_Keleseth_Vrykul_Skeleton", math.random(20000, 22000), 1)
pUnit:RegisterEvent("Prince_Keleseth_SpawnPhase", 100, 0)
end

function Prince_Keleseth_OnLeaveCombat(pUnit, Event)
pUnit:RemoveEvents()
end

function Prince_Keleseth_OnDeath(pUnit, Event)
pUnit:RemoveEvents()
pUnit:SendChatMessage(14, 0, "I join... the night.")
end

RegisterUnitEvent(23953, 1, "Prince_Keleseth_OnCombat")
RegisterUnitEvent(23953, 2, "Prince_Keleseth_OnLeaveCombat")
RegisterUnitEvent(23953, 4, "Prince_Keleseth_OnDeath")

function Prince_Keleseth_Shadow_Bolt(pUnit, Event)
pUnit:FullCastSpellOnTarget(43667, pUnit:GetMainTank())
end

function Prince_Keleseth_Frost_Tomb(pUnit, Event)
pUnit:FullCastSpellOnTarget(48400, pUnit:GetRandomPlayer(0))
end

function Prince_Keleseth_Vrykul_Skeleton(pUnit, Event)
local X = pUnit:GetX()
local Y = pUnit:GetY()
local Z = pUnit:GetZ()
local O = pUnit:GetO()
pUnit:SpawnCreature(23970, X, Y, Z, O, 18, 0)
pUnit:SpawnCreature(23970, X, Y, Z, O, 18, 0)
pUnit:SpawnCreature(23970, X, Y, Z, O, 18, 0)
pUnit:SpawnCreature(23970, X, Y, Z, O, 18, 0)
pUnit:SpawnCreature(23970, X, Y, Z, O, 18, 0)
pUnit:SendChatMessage(14, 0, "Aranal, lidel! Their fate shall be yours!")
end

function Prince_Keleseth_SpawnPhase(pUnit, Event)
if AddsDead >= 5 then
pUnit:RemoveEvents()
pUnit:RegisterEvent("Prince_Keleseth_Vrykul_Skeleton", 100, 1)
pUnit:RegisterEvent("Prince_Keleseth_Shadow_Bolt", math.random(13000, 15000), 0)
pUnit:RegisterEvent("Prince_Keleseth_Frost_Tomb", math.random(35000, 37000), 0)
AddsDead = 0
pUnit:RegisterEvent("Prince_Keleseth_RegularPhase", 100, 0)
end
end

function Prince_Keleseth_RegularPhase(pUnit, Event)
if AddsDead == 0  then
pUnit:RemoveEvents()
pUnit:RegisterEvent("Prince_Keleseth_SpawnPhase", 100, 0)
pUnit:RegisterEvent("Prince_Keleseth_Shadow_Bolt", math.random(13000, 15000), 0)
pUnit:RegisterEvent("Prince_Keleseth_Frost_Tomb", math.random(35000, 37000), 0)
end
end

function Vrykul_Skeleton_OnCombat(pUnit, Event)
pUnit:RegisterEvent("Vrykul_Skeleton_Decrepify", 15000, 0)
end

function Vrykul_Skeleton_OnLeaveCombat(pUnit, Event)
pUnit:RemoveEvents()
end

function Vrykul_Skeleton_OnDeath(pUnit, Event)
pUnit:RemoveEvents()
AddsDead = AddsDead + 1
end


RegisterUnitEvent(23970, 1, "Vrykul_Skeleton_OnCombat")
RegisterUnitEvent(23970, 2, "Vrykul_Skeleton_OnLeaveCombat")
RegisterUnitEvent(23970, 4, "Vrykul_Skeleton_OnDeath")

function Vrykul_Skeleton_Decrepify(pUnit, Event)
pUnit:FullCastSpellOnTarget(42702, pUnit:GetMainTank())
end