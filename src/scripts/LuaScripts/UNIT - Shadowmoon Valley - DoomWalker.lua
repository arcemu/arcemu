--[[ UNIT - ShadowmoonValley - DoomWalker.lua

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

function DoomWalker_Earthquake(unit)
unit:CastSpell(32686)
unit:SendChatMessage(12, 0, "Magnitude set. Release.")
unit:PlaySoundToSet (11345)
end

function DoomWalker_overrun(unit)
unit:CastSpell(32637)
unit:SendChatMessage(12, 0, "Trajectory Locked.")
unit:PlaySoundToSet (11347)
end

function DoomWalker_ChainLightning(unit)
local plr = unit:GetClosestPlayer()
if (plr ~= nil) then
unit:CastSpellOnTarget(33665, plr) 
unit:PlaySoundToSet (11346)
end
end

function DoomWalker_SunderArmor(unit)
local plr = unit:GetClosestPlayer()
if (plr ~= nil) then
unit:CastSpellOnTarget(30901, plr) 
end
end

function DoomWalker_OnEnterCombat(unit)
unit:SendChatMessage(12, 0, "Do not proceed. You will be eliminated.")
unit:PlaySoundToSet (11344)
unit:RegisterEvent("DoomWalker_ChainLightning",23000, 0)
unit:RegisterEvent("DoomWalker_SunderArmor",10000, 0)
unit:RegisterEvent("DoomWalker_Earthquake",50000, 0)
unit:RegisterEvent("DoomWalker_overrun",120000, 0) 
end

function DoomWalker_OnLeaveCombat(unit)
unit:RemoveEvents()
end

function DoomWalker_KilledTarget(unit)
unit:SendChatMessage(12, 0, "Target Exterminated.")
unit:PlaySoundToSet (11351)
unit:RemoveEvents()
end

function DoomWalker_OnDied(unit)
unit:SendChatMessage(12, 0, "System failure in five....four....")
unit:PlaySoundToSet (11352)
unit:RemoveEvents()
end



RegisterUnitEvent(17711, 1, "DoomWalker_OnEnterCombat")
RegisterUnitEvent(17711, 2, "DoomWalker_OnLeaveCombat")
RegisterUnitEvent(17711, 3, "DoomWalker_OnKilledTarget")
RegisterUnitEvent(17711, 4, "DoomWalker_OnDied") 