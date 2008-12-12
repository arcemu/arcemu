--[[ Dalronn and Skarvald (Second boss in Utgarde Keep)


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

 Script Made By FaKiE ]]

-- Skarvald Alive Part

function Skarvald_EnterCombat(pUnit, event)
SpawnSx=pUnit:GetX()
SpawnSy=pUnit:GetY()
SpawnSz=pUnit:GetZ()
SpawnSo=pUnit:GetO()
SYell=0
SGhost=0
pUnit:SendChatMessage(14, 0, "Dalronn! See if you can muster the nerve to join my attack!")
--pUnit:PlaySoundToSet(1337) -- Needs changing
pUnit:RegisterEvent("SCharge", 35000, 0)
pUnit:RegisterEvent("SDeathCheck", 100, 0)
end

function SCharge(pUnit, event)
pUnit:CastSpellOnTarget(48583, pUnit:GetMainTank())
pUnit:RegisterEvent("SCharge2", 1550, 1)
end

function SCharge2(pUnit, event)
pUnit:FullCastSpellOnTarget(43651, pUnit:GetRandomPlayer(0))
end

function SDeathCheck(pUnit, event)
if DGhost==1 and SYell==0 then
pUnit:SendChatMessage(14, 0, "Pagh! What sort of necromancer lets death stop him? I knew you were worthless!")
SYell=1
end
end

function Skarvald_Died(pUnit, event)
pUnit:RemoveEvents()
SGhost=1
if DGhost==0 then
pUnit:SendChatMessage(14, 0, "A warrior's death.")
--pUnit:PlaySoundToSet(1337) -- Needs changing
local Sx=pUnit:GetX()
local Sy=pUnit:GetY()
local Sz=pUnit:GetZ()
local So=pUnit:GetO()
pUnit:SpawnCreature(27390, Sx, Sy, Sz, So, 1045, 0)
pUnit:Despawn(50, 0)
else
pUnit:SendChatMessage(14, 0, "A warrior's death.")
--pUnit:PlaySoundToSet(1337) -- Needs changing
end
end

function Skarvald_LeavingCombat(pUnit, event)
pUnit:RemoveEvents()
end

RegisterUnitEvent (24200, 4, "Skarvald_Died")
RegisterUnitEvent (24200, 2, "Skarvald_LeavingCombat")
RegisterUnitEvent (24200, 1, "Skarvald_EnterCombat")

-- Dalronn Alive Part

function Dalronn_EnterCombat(pUnit, event)
SpawnDx=pUnit:GetX()
SpawnDy=pUnit:GetY()
SpawnDz=pUnit:GetZ()
SpawnDo=pUnit:GetO()
DYell=0
DGhost=0
pUnit:FullCastSpellOnTarget(43649, pUnit:GetRandomPlayer(0))
pUnit:RegisterEvent("DReply", 5500, 1)
pUnit:RegisterEvent("DShadowbolt", 2100, 6)
pUnit:RegisterEvent("DDebilitate", 15000, 1)
pUnit:RegisterEvent("DDeathCheck", 100, 0)
end

function DReply(pUnit, event)
pUnit:SendChatMessage(14, 0, "By all means, don't assess the situation, you halfwit! Just jump into the fray!")
end

function DDeathCheck(pUnit, event)
if SGhost==1 and DYell==0 then
DYell=1
pUnit:RegisterEvent("DYell", 2000, 1)
end
end

function DYell(pUnit, event)
pUnit:SendChatMessage(14, 0, "You may serve me yet.")
--pUnit:PlaySoundToSet(1337) -- Needs  changing
end

function DDebilitate(pUnit, event)
pUnit:CastSpellOnTarget(43650, pUnit:GetMainTank())
pUnit:RegisterEvent("DShadowbolt", 2100, 6)
pUnit:RegisterEvent("DDebilitate", 15000, 1)
end

function DShadowbolt(pUnit, event)
pUnit:FullCastSpellOnTarget(43649, pUnit:GetRandomPlayer(0))
end

function Dalronn_Died(pUnit, event)
pUnit:RemoveEvents()
DGhost=1
if SGhost==0 then
pUnit:SendChatMessage(14, 0, "See... you... soon.")
--pUnit:PlaySoundToSet(1337) -- Needs changing
local Dx=pUnit:GetX()
local Dy=pUnit:GetY()
local Dz=pUnit:GetZ()
local Do=pUnit:GetO()
pUnit:SpawnCreature(27389, Dx, Dy, Dz, Do, 1045, 0)
pUnit:Despawn(50, 0)
end
end

function Dalronn_LeavingCombat(pUnit, event)
pUnit:RemoveEvents()
end

RegisterUnitEvent (24201, 4, "Dalronn_Died")
RegisterUnitEvent (24201, 2, "Dalronn_LeavingCombat")
RegisterUnitEvent (24201, 1, "Dalronn_EnterCombat")

-- Ghost Part

function AttackRandom(pUnit, event)
local droptimer2=math.random(3500, 12500)
pUnit:ClearThreatList()
pUnit:RegisterEvent("AttackRandom2", droptimer2, 1)
end

function AttackRandom2(pUnit, event)
local droptimer3=math.random(3500, 12500)
pUnit:ClearThreatList()
pUnit:RegisterEvent("AttackRandom", droptimer3, 1)
end

-- Skarvald

function SkarvaldGhost_EnterCombat(pUnit, event)
local droptimer1=math.random(3500, 12500)
pUnit:RegisterEvent("AttackRandom", droptimer1, 1)
pUnit:RegisterEvent("SCharge", 35000, 0)
pUnit:RegisterEvent("SDeathCheck2", 100, 0)
end

function SDeathCheck2(pUnit, event)
if DGhost==1 then
pUnit:Despawn(50, 0)
end
end

function SkarvaldGhost_LeavingCombat(pUnit, event)
pUnit:RemoveEvents()
pUnit:SpawnCreature(24200, SpawnSx, SpawnSy, SpawnSz, SpawnSo, 1045, 0)
pUnit:Despawn(2000, 0)
end

RegisterUnitEvent (27390, 2, "SkarvaldGhost_LeavingCombat")
RegisterUnitEvent (27390, 1, "SkarvaldGhost_EnterCombat")

-- Dalronn

function DalronnGhost_EnterCombat(pUnit, event)
local droptimer1=math.random(3500, 12500)
pUnit:RegisterEvent("DShadowbolt", 2100, 6)
pUnit:RegisterEvent("DDebilitate", 15000, 1)
pUnit:RegisterEvent("AttackRandom", droptimer1, 1)
pUnit:RegisterEvent("DDeathCheck2", 100, 0)
end

function DDeathCheck2(pUnit, event)
if SGhost==1 then
pUnit:Despawn(50, 0)
end
end

function DalronnGhost_LeavingCombat(pUnit, event)
pUnit:RemoveEvents()
pUnit:SpawnCreature(24201, SpawnDx, SpawnDy, SpawnDz, SpawnDo, 1045, 0)
pUnit:Despawn(2000, 0)
end

RegisterUnitEvent (27389, 2, "DalronnGhost_LeavingCombat")
RegisterUnitEvent (27389, 1, "DalronnGhost_EnterCombat")

--------------------------------------------------------------------------------
-- Soul Linking, so that they both aggro when you aggro one of them.

function Skarvald_OnSpawn(pUnit, event)
Skarvald = pUnit
pUnit:RegisterEvent("SSoulLink", 1500, 1)
end

function Dalronn_OnSpawn(pUnit, event)
Dalronn = pUnit
pUnit:RegisterEvent("DSoulLink", 1500, 1)
end

function SSoulLink(pUnit, event)
pUnit:SetSoulLinkedWith(Dalronn)
end

function DSoulLink(pUnit, event)
pUnit:SetSoulLinkedWith(Skarvald)
end

RegisterUnitEvent (24200, 18, "Skarvald_OnSpawn")
RegisterUnitEvent (24201, 18, "Dalronn_OnSpawn")