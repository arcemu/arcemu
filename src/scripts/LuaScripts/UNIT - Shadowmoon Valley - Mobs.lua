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


function EclipsionSpawn(pUnit, Event)
	
	x = pUnit:GetX();
	y = pUnit:GetY();
	z = pUnit:GetZ();
	o = pUnit:GetO();
	
	pUnit:SpawnCreature(21627, x, y, z, o, 23000);
	
end

function EclipsionDespawn(pUnit, Event)

	pUnit:Despawn(1000, 0);
	
end

RegisterUnitEvent(19806, 1, "EclipsionSpawn");
RegisterUnitEvent(22018, 1, "EclipsionSpawn");
RegisterUnitEvent(21627, 2, "EclipsionDespawn");

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
unit:FullCastSpellOnTarget(30901, plr) 
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


function AcientShadowmoonSpirit_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("AcientShadowmoonSpirit_Haste", 40000, 0)
	pUnit:RegisterEvent("AcientShadowmoonSpirit_DeathAndDecay", 30000, 0)
	pUnit:RegisterEvent("AcientShadowmoonSpirit_TouchOfDarkness", 15000, 1)
	pUnit:RegisterEvent("AcientShadowmoonSpirit_UnholyArmor", 3000, 0)
end

function AcientShadowmoonSpirit_Haste(pUnit,Event)
	pUnit:CastSpell(37728)
end

function AcientShadowmoonSpirit_DeathAndDecay(pUnit,Event)
	pUnit:FullCastSpellOnTarget(37788,pUnit:GetClosestPlayer())
end

function AcientShadowmoonSpirit_TouchOfDarkness(pUnit,Event)
	pUnit:FullCastSpellOnTarget(37727,pUnit:GetClosestPlayer())
end

function AcientShadowmoonSpirit_UnholyArmor(pUnit,Event)
	pUnit:CastSpell(37729)
end

function AcientShadowmoonSpirit_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function AcientShadowmoonSpirit_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent (21797, 1, "AcientShadowmoonSpirit_OnEnterCombat")
RegisterUnitEvent (21797, 2, "AcientShadowmoonSpirit_OnLeaveCombat")
RegisterUnitEvent (21797, 4, "AcientShadowmoonSpirit_OnDied")

function Akama_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Akama_Lightning", 1540, 0)
end

function Akama_Lightning(pUnit,Event)
	pUnit:FullCastSpellOnTarget(39945,pUnit:GetClosestPlayer())
end

function Akama_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Akama_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent (21700, 1, "Akama_OnEnterCombat")
RegisterUnitEvent (21700, 2, "Akama_OnLeaveCombat")
RegisterUnitEvent (21700, 4, "Akama_OnDied")

function Alandien_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Alandien_ShadowFury", 3000, 0)
	pUnit:RegisterEvent("Alandien_ManaBurn", 3000, 0)
end

function Alandien_ShadowFury(pUnit,Event)
	pUnit:FullCastSpellOnTarget(39082,pUnit:GetClosestPlayer())
end

function Alandien_ManaBurn(pUnit,Event)
	pUnit:FullCastSpellOnTarget(39262,pUnit:GetClosestPlayer())
end

function Alandien_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Alandien_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent (21171, 1, "Alandien_OnEnterCombat")
RegisterUnitEvent (21171, 2, "Alandien_OnLeaveCombat")
RegisterUnitEvent (21171, 4, "Alandien_OnDied")

function AltarDefender_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("AltarDefender_Shoot", 11000, 0)
end

function AltarDefender_Shoot(pUnit,Event)
	pUnit:FullCastSpellOnTarget(41440,pUnit:GetClosestPlayer())
end

function AltarDefender_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function AltarDefender_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent (23453, 1, "Alandien_OnEnterCombat")
RegisterUnitEvent (23453, 2, "Alandien_OnLeaveCombat")
RegisterUnitEvent (23453, 4, "Alandien_OnDiedCombat")

function AltarOfShatar_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("AltarOfShatar_Net", 20000, 0)
end

function AltarofShatar_Net(pUnit,Event)
	pUnit:FullCastSpellOnTarget(12024,pUnit:GetClosestPlayer())
end

function AltarofShatar_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function AltarofShatar_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent (21986, 1, "Alandien_OnEnterCombat")
RegisterUnitEvent (21986, 2, "Alandien_OnLeaveCombat")
RegisterUnitEvent (21986, 4, "Alandien_OnDied")

function Jerrikar_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Jerrikar_DarkStrike", 4000, 0)
	pUnit:RegisterEvent("Jerrikar_DiplomaticImmunity", 40000, 0)
	pUnit:RegisterEvent("Jerrikar_Silence", 30000, 0)
end

function Jerrikar_Silence(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38913,GetClosestPlayer())
end

function Jerrikar_DarkStrike(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38926,GetClosestPlayer())
end

function Jerrikar_DiplomaticImmunity(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38916,GetClosestPlayer())
end

function Jerrikar_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Jerrikar_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent (18695, 1, "Alandien_OnEnterCombat")
RegisterUnitEvent (18695, 2, "Alandien_OnLeaveCombat")
RegisterUnitEvent (18695, 4, "Alandien_OnDied")

function AnchoriteCaalen_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("AnchoriteCaalen_HolySmite", 1540, 0)
	pUnit:RegisterEvent("AnchoriteCaalen_HolySmite", 5000, 0)
end

function AnchoriteCaalen_HolySmite(pUnit,Event)
	pUnit:FullCastSpellOnTarget(36176,pUnit:GetClosestPlayer())
end

function AnchoriteCaalen_GreaterHeal(pUnit,Event)
	pUnit:FullCastSpellOnTarget(35096,pUnit:GetRandomFriend())
end

function AnchoriteCaalen_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function AnchoriteCaalen_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent (22862, 1, "Alandien_OnEnterCombat")
RegisterUnitEvent (22862, 2, "Alandien_OnLeaveCombat")
RegisterUnitEvent (22862, 4, "Alandien_OnDied")

function ArcanoScorp_OnEnterCombat(pUnit,Event)
	pUnit:CastSpell(37917) 
	pUnit:CastSpell(37851)
	pUnit:RegisterEvent("ArcanoScorp_DisMantle", 3000, 0)
	pUnit:RegisterEvent("ArcanoScorp_Pince", 6600, 0)
end

function ArcanoScorp_Dismantle(pUnit,Event)
	pUnit:FullCastSpellOnTarget(37919,pUnit:GetClosestPlayer())
end

function ArcanoScorp_Pince(pUnit,Event)
	pUnit:FullCastSpellOnTarget(37918,pUnit:GetClosestPlayer())
end

RegisterUnitEvent (21909, 1, "ArcanoScorp_OnEnterCombat")
RegisterUnitEvent (21909, 2, "ArcanoScorp_OnLeaveCombat")
RegisterUnitEvent (21909, 4, "ArcanoScorp_OnDied")

function Arvoar_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Arvoar_BellowingRoar", 30000, 0)
	pUnit:RegisterEvent("Arvoar_Rend", 20000, 0)
	pUnit:RegisterEvent("Arvoar_RockShell", 33000, 0)
end

function Arvoar_BellowingRoar(pUnit,Event)
	pUnit:FullCastSpellOnTarget(40636,pUnit:GetClosestPlayer())
end

function Arvoar_Rend(pUnit,Event)
	pUnit:FullCastSpellOnTarget(13443,pUnit:GetClosestPlayer())
end

function Arvoar_RockShell(pUnit,Event)
	pUnit:CastSpell(33810)
end

function Arvoar_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Arvoar_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent (23267, 1, "Arvoar_OnEnterCombat")
RegisterUnitEvent (23267, 2, "Arvoar_OnLeaveCombat")
RegisterUnitEvent (23267, 4, "Arvoar_OnDied")

function Asghar_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Asghar_Cleave", 3300, 0)
	pUnit:RegisterEvent("Asghar_DarkMending", 15000, 0)
end

function Asghar_Cleave(pUnit,Event)
	pUnit:FullCastSpellOnTarget(15496,pUnit:GetClosestPlayer())
end

function Asghar_DarkMending(pUnit,Event)
	pUnit:CastSpell(16588)
end

function Asghar_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Asghar_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent (22025, 1, "Asgarhar_OnEnterCombat")
RegisterUnitEvent (22025, 2, "Asgarhar_OnLeaveCombat")
RegisterUnitEvent (22025, 4, "Asgarhar_OnDied")

function AshtongueShaman_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("AshtongueShaman_Bloodlust", 60000, 0)
	pUnit:RegisterEvent("AshtongueShaman_LightningShield", 60000, 0)
end

function AshtongueShaman_Bloodlust(pUnit,Event)
	pUnit:CastSpell(37067)
end

function AshtongueShaman_LightningShield(pUnit,Event)
	pUnit:CastSpell(12550)
end

function AshtongueShaman_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function AshtongueShaman_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent (21453, 1, "AshtongueShaman_OnEnterCombat")
RegisterUnitEvent (21453, 2, "AshtongueShaman_OnLeaveCombat")
RegisterUnitEvent (21453, 4, "AshtongueShaman_OnDied")

function AshtongueWorker_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("AshtongueWorker_PierceArmor", 45000, 0)
end

function AshtongueWorker_PierceArmor(pUnit,Event)
	pUnit:FullCastSpellOnTarget(6016,pUnit:GetClosestPlayer())
end

function AshtongueWorker_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function AshtongueWorker_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent (21455, 1, "AshtongueWorker_OnEnterCombat")
RegisterUnitEvent (21455, 2, "AshtongueWorker_OnLeaveCombat")
RegisterUnitEvent (21455, 4, "AshtongueWorker_OnDied")

function Azaloth_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Azaloth_RainOfFire", 30000, 0)
	pUnit:RegisterEvent("Azaloth_Cleave", 60000, 0)
	pUnit:RegisterEvent("Azaloth_Cripple", 20000, 0)
	pUnit:RegisterEvent("Azaloth_WarStomp", 13000, 0)
end

function Azaloth_RainOfFire(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38741,pUnit:GetClosestPlayer())
end

function Azaloth_Cleave(pUnit,Event)
	pUnit:FullCastSpellOnTarget(40504,pUnit:GetClosestPlayer())
end


function Azaloth_Cripple(pUnit,Event)
	pUnit:FullCastSpellOnTarget(11443,pUnit:GetClosestPlayer())
end


function Azaloth_WarStomp(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38750,pUnit:GetClosestPlayer())
end

function Azaloth_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Azaloth_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21506, 1, "Azaloth_OnEnterCombat")
RegisterUnitEvent(21506, 2, "Azaloth_OnLeaveCombat")
RegisterUnitEvent(21506, 4, "Azaloth_OnDied")

function Barash_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Barash_RockShell", 11000, 0)
	pUnit:RegisterEvent("Barash_BellowingRoar", 40000, 0)
	pUnit:RegisterEvent("Barash_Rend", 5000, 0)
end

function Barash_RockShell(pUnit,Event)
	pUnit:FullCastSpellOnTarget(33810,pUnit:GetClosestPlayer())
end

function Barash_BellowingRoar(pUnit,Event)
	pUnit:FullCastSpellOnTarget(40636,pUnit:GetClosestPlayer())
end

function Barash_Rend(pUnit,Event)
	pUnit:FullCastSpellOnTarget(13443,pUnit:GetClosestPlayer())
end

function Barash_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Barash_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(23269, 1, "Barash_OnEnterCombat")
RegisterUnitEvent(23269, 2, "Barash_OnLeaveCombat")
RegisterUnitEvent(23269, 4, "Barash_OnDied")

function BatRiderGuard_OnEnterCombat(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38066,pUnit:GetClosestPlayer())
	pUnit:RegisterEvent("BatRiderGuard_Spell", 25000, 0)
end

function BatRiderGuard_Spell(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38066,pUnit:GetClosestPlayer())
end

function BatRiderGuard_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function BatRiderGuard_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(15242, 1, "BatRiderGuard_OnEnterCombat")
RegisterUnitEvent(15242, 2, "BatRiderGuard_OnLeaveCombat")
RegisterUnitEvent(15242, 4, "BatRiderGuard_OnDied")

function BBOD_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("BBOD_Spell", 60000, 0)
end

function BBOD_Spell(pUnit,Event)
	pUnit:FullCastSpellOnTarget(7279,pUnit:GetClosestPlayer())
end

function BBOD_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function BBOD_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent (23286, 1, "BBOD_OnEnterCombat")
RegisterUnitEvent (23286, 2, "BBOD_OnLeaveCombat")
RegisterUnitEvent (23286, 4, "BBOD_OnDied")

function BlackCat_OnEnterCombat(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "Meow!")
end

function BlackCat_OnDeath(pUnit,Event)
	pUnit:FullCastSpellOnTarget(39477,pUnit:GetClosestPlayer())
end

RegisterUnitEvent(22816, 1, "BlackCat_OnEnterCombat")
RegisterUnitEvent(22816, 4, "BlackCat_OnDeath")

function BWM_OnEnterCombat(pUnit,Event)
	pUnit:RegisterUnitEvent("BWM_SnapKick", 10000, 0)
end

function BWM_SnapKick(pUnit,Event)
	pUnit:FullCastSpellOnTarget(39477,pUnit:GetClosestPlayer())
end

function BWM_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function BWM_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21245, 1, "BWM_OnEnterCombat")
RegisterUnitEvent(21245, 2, "BWM_OnLeaveCombat")
RegisterUnitEvent(21245, 4, "BWM_OnDied")

function BWMessenger_OnEnterCombat(pUnit,Event)
	pUnit:FullCastSpellOnTarget(35570,pUnit:GetClosestPlayer())
end

RegisterUnitEvent(21244, 1, "BWMessenger_OnEnterCombat")

function Borak_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Borak_SliceAndDice", 15000, 0)
	pUnit:RegisterEvent("Borak_Eviscerate", 5000, 0)
end

function Borak_SliceAndDice(pUnit,Event)
	pUnit:FullCastSpellOnTarget(30470,pUnit:GetClosestPlayer())
end

function Borak_Eviscerate(pUnit,Event)
	pUnit:FullCastSpellOnTarget(27611,pUnit:GetClosestPlayer())
end

function Borak_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Borak_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21293, 1, "Borak_OnEnterCombat")
RegisterUnitEvent(21293, 2, "Borak_OnLeaveCombat")
RegisterUnitEvent(21293, 4, "Borak_OnDied")

function Bron_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Bron_ThrowHammer", 15000, 0)
end

function Bron_ThrowHammer(pUnit,Event)
	pUnit:FullCastSpellOnTarget(33822,pUnit:GetClosestPlayer())
end

function Bron_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Bron_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(19395, 1, "Bron_OnEnterCombat")
RegisterUnitEvent(19395, 2, "Bron_OnLeaveCombat")
RegisterUnitEvent(19395, 4, "Bron_OnDied")

function WaterSpirit_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("WaterSpirit_Waterbolt", 4300, 0)
end

function Waterspirit_WaterBolt(pUnit,Event)
	pUnit:FullCastSpellOnTarget(31707,pUnit:GetClosestPlayer())
end

function WaterSpirit_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function WaterSpirit_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21029, 1, "WaterSpirit_OnEnterCombat")
RegisterUnitEvent(21029, 2, "WaterSpirit_OnLeaveCombat")
RegisterUnitEvent(21029, 4, "WaterSpirit_OnDied")

function ChainOfShadows_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("ChainOfShadows_Spell", 5500, 0)
end

function ChainOfShadows_Spell(pUnit,Event)
	pUnit:FullCastSPellOnTarget(37784,pUnit:GetClosestPlayer())
end

function ChainOfShadows_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function ChainOfShadows_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21876, 1, "ChainOfShadows_OnEnterCombat")
RegisterUnitEvent(21876, 2, "ChainOfShadows_OnLeaveCombat")
RegisterUnitEvent(21876, 4, "ChainOfShadows_OnDied")

function ChancellorBloodleaf_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("ChancellorBloodleaf_Spell1", 4000, 0)
	pUnit:RegisterEvent("ChancellorBloodleaf_Spell2", 10000, 0)
end

function ChancellorBloodleaf_Spell1(pUnit,Event)
	pUnit:FullCastSpellOnTarget(34517,pUnit:GetClosestPlayer())
end

function ChancellorBloodleaf_Spell2(pUnit,Event)
	pUnit:FullCastSpellOnTarget(15791,pUnit:GetClosestPlayer())
end

RegisterUnitEvent(22012, 1, "ChancellorBloodleaf_OnEnterCombat")
RegisterUnitEvent(22012, 2, "ChancellorBloodleaf_OnLeaveCombat")
RegisterUnitEvent(22012, 4, "ChancellorBloodleaf_OnDied")

function Cobra_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Cobra_Spell", 11000, 0)
end

function Cobra_Spell(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38030,pUnit:GetClosestPlayer())
end

function Cobra_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Cobra_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end
RegisterUnitEvent(19784, 1, "Cobra_OnEnterCombat")
RegisterUnitEvent(19784, 2, "Cobra_OnLeaveCombat")
RegisterUnitEvent(19784, 4, "Cobra_OnDied")

function CDefender_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("CDefender_Spell1", 7000, 0)
	pUnit:RegisterEvent("CDefender_Spell2", 16000, 0)
end

function CDefender_Spell1(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38233,pUnit:GetClosestPlayer())
end

function CDefender_Spell2(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38031,pUnit:GetClosestPlayer())
end

function CDefender_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function CDefender_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(19762, 1, "CDefender_OnEnterCombat")
RegisterUnitEvent(19762, 2, "CDefender_OnLeaveCombat")
RegisterUnitEvent(19762, 4, "CDefender_OnDied")

function CMuckwatcher_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("CMuckwatcher_BattleShout", 40000, 0)
end

function CMuckwatcher_BattleShout(pUnit,Event)
	pUnit:CastSpell(38232)
end

function CMuckwatcher_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function CMuckwatcher_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(19788, 1, "CMuckwatcher_OnEnterCombat")
RegisterUnitEvent(19788, 2, "CMuckwatcher_OnLeaveCombat")
RegisterUnitEvent(19788, 4, "CMuckwatcher_OnDied")

function CMyrmidon_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("CMyrmidon_Spell", 120000, 0)
end

function CMyrmidon_Spell(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38027,pUnit:GetClosestPlayer())
end

function CMyrmidon_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function CMyrmidon_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end


RegisterUnitEvent(19765, 1, "CMyrmidon_OnEnterCombat")
RegisterUnitEvent(19765, 2, "CMyrmidon_OnLeaveCombat")
RegisterUnitEvent(19765, 4, "CMyrmidon_OnDied")

function CSiren_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("CSiren_Spell1", 20000, 0)
	pUnit:RegisterEvent("CSiren_Spell2", 6000, 0)
end

function CSiren_Spell1(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38026,pUnit:GetClosestPlayer())
end

function CSiren_Spell2(pUnit,Event)
	pUnit:FullCastSpellOnTarget(32011,pUnit:GetClosestPlayer())
end

function CSiren_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function CSiren_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(19768,1,"CSiren_OnEnterCombat")
RegisterUnitEvent(19768,2,"CSiren_OnLeaveCombat")
RegisterUnitEvent(19768,4,"CSiren_OnDied")

function CSorceress_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("CSiren_Spell1", 20000, 0)
	pUnit:RegisterEvent("CSiren_Spell2", 6000, 0)
end

function CSorceress_Spell1(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38026,pUnit:GetClosestPlayer())
end

function CSorceress_Spell2(pUnit,Event)
	pUnit:FullCastSpellOnTarget(32011,pUnit:GetClosestPlayer())
end

function CSorceress_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function CSorceress_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(19767,1,"CSorceress_OnEnterCombat")
RegisterUnitEvent(19767,2,"CSorceress_OnLeaveCombat")
RegisterUnitEvent(19767,4,"CSorceress_OnDied")

function Collidus_OnEnterCombat(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38932,pUnit:GetClosestPlayer())
	pUnit:FullCastSpellOnTarget(36414,pUnit:GetClosestPlayer())
	pUnit:RegisterEvent("Collidus_FocusedBursts", 40000, 0)
	pUnit:RegisterEvent("Collidus_Scream", 40000, 0)
end

function Collidus_FocusedBursts(pUnit,Event)
	pUnit:FullCastSpellOnTarget(36414,pUnit:GetClosestPlayer())
end

function Collidus_Scream(pUnit,Event)
	pUnit:FullCastSpellOnTarget(34322,pUnit:GetClosestPlayer())
end

function Collidus_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Collidus_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(18694, 1, "Collidus_OnEnterCombat")
RegisterUnitEvent(18694, 2, "Collidus_OnLeaveCombat")
RegisterUnitEvent(18694, 4, "Collidus_OnDied")

function Arcus_OnEnterCombat(pUnit,Event)
	pUnit:FullCastSpellOnTarget(41440,pUnit:GetClosestPlayer())
	pUnit:RegisterEvent("Arcus_Spell1", 10000, 0)
	pUnit:RegisterEvent("Arcus_Spell2", 30000, 0)
end

function Arcus_Spell1(pUnit,Event)
	pUnit:FullCastSpellOnTarget(41448,pUnit:GetClosestPlayer())
end

function Arcus_Spell1(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38370,pUnit:GetClosestPlayer())
end

function Arcus_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Arcus_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(23452, 1, "Arcus_OnEnterCombat")
RegisterUnitEvent(23452, 2, "Arcus_OnLeaveCombat")
RegisterUnitEvent(23452, 4, "Arcus_OnDied")

function Hobb_OnEnterCombat(pUnit,Event)
	pUnit:FullCastSpellOnTarget(41440,pUnit:GetClosestPlayer())
	pUnit:RegisterEvent("Hobb_Spell1", 10000, 0)
	pUnit:RegisterEvent("Hobb_Spell2", 30000, 0)
end

function Hobb_Spell1(pUnit,Event)
	pUnit:FullCastSpellOnTarget(41448,pUnit:GetClosestPlayer())
end

function Hobb_Spell1(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38370,pUnit:GetClosestPlayer())
end

function Hobb_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Hobb_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(23434, 1, "Hobb_OnEnterCombat")
RegisterUnitEvent(23434, 2, "Hobb_OnLeaveCombat")
RegisterUnitEvent(23434, 4, "Hobb_OnDied")

function Corok_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Corok_Spell1", 11000, 0)
	pUnit:RegisterEvent("Corok_Spell2", 26000, 0)
end

function Corok_Spell1(pUnit,Event)
	pUnit:FullCastSpellOnTarget(12612,pUnit:GetClosestPlayer())
end

function Corok_Spell2(pUnit,Event)
	pUnit:FullCastSpellOnTarget(15550,pUnit:GetClosestPlayer())
end

function Corok_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Corok_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(22011, 1, "Corok_OnEnterCombat")
RegisterUnitEvent(22011, 2, "Corok_OnLeaveCombat")
RegisterUnitEvent(22011, 4, "Corok_OnDied")

function CAE_OnEnterCombat(pUnit,Event)
	pUnit:CastSpell(35194)
	pUnit:RegisterEvent("CAE_LBolt", 5000, 0)
end

function CAE_LBolt(pUnit,Event)
	pUnit:FullCastSpellOnTarget(9532,pUnit:GetClosestPlayer())
end


function CAE_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end


function CAE_OnDiedCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21707, 1, "CAE_OnEnterCombat")
RegisterUnitEvent(21707, 2, "CAE_OnLeaveCombat")
RegisterUnitEvent(21707, 4, "CAE_OnDied")

function CFE_OnEnterCombat(pUnit,Event)
	pUnit:CastSpell(35194)
	pUnit:RegisterEvent("CFE_Fball", 5000, 0)
end

function CFE_Fball(pUnit,Event)
	pUnit:FullCastSpellOnTarget(9053,pUnit:GetClosestPlayer())
end

function CFE_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function CFE_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21706, 1, "CFE_OnEnterCombat")
RegisterUnitEvent(21706, 2, "CFE_OnLeaveCombat")
RegisterUnitEvent(21706, 4, "CFE_OnDied")

function CWE_OnEnterCombat(pUnit,Event)
	pUnit:CastSpell(35194)
	pUnit:RegisterEvent("CWE_FBolt", 5000, 0)
end

function CWE_Fbolt(pUnit,Event)
	pUnit:FullCastSpellOnTarget(9672,pUnit:GetClosestPlayer())
end

function CWE_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function CWE_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21428, 1, "CWE_OnEnterCombat")
RegisterUnitEvent(21428, 2, "CWE_OnLeaveCombat")
RegisterUnitEvent(21428, 4, "CWE_OnDied")

function CMForeman_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("CMForeman_Enrage", 5000, 0)
end

function CMForeman_Enrage(pUnit,Event)
	pUnit:CastSpell(40743)
end

function CMForeman_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function CMForeman_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(23305, 1, "CMForeman_OnEnterCombat")
RegisterUnitEvent(23305, 2, "CMForeman_OnLeaveCombat")
RegisterUnitEvent(23305, 4, "CMForeman_OnDied")

function CMMiner_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("CMMiner_Enrage", 5000, 0)
end

function CMMiner_Enrage(pUnit,Event)
	pUnit:CastSpell(40743)
end

function CMMiner_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function CMMiner_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(23324, 1, "CMMiner_OnEnterCombat")
RegisterUnitEvent(23324, 2, "CMMiner_OnLeaveCombat")
RegisterUnitEvent(23324, 4, "CMMiner_OnDied")

function CShardling_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("CShardling_Enrage", 5000, 0)
end

function CShardling_Enrage(pUnit,Event)
	pUnit:CastSpell(40743)
end

function CShardling_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function CShardling_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21936, 1, "CMMiner_OnEnterCombat")
RegisterUnitEvent(21936, 2, "CMMiner_OnLeaveCombat")
RegisterUnitEvent(21936, 4, "CMMiner_OnDied")

function Cyrukh_OnEnterCombat(pUnit,Event)
	pUnit:GetMainTank()
	pUnit:GetAddTank()
	pUnit:RegisterEvent("Cyrukh_Final", 0001, 0)
	pUnit:RegisterEvent("Cyrukh_Trample", 10000, 0)
	pUnit:RegisterEvent("Cyrukh_KnockAway", 22000, 0)
end

function Cyrukh_Final(pUnit,Event)
 if pUnit:GetHealthPct() == 10 then
	pUnit:RegisterEvent("Cyrukh_FinalSpell",  20000, 0)
end
end

function Cyrukh_FinalSpell(pUnit,Event)
	pUnit:FullCastSpellOnTarget(39429,pUnit:GetMainTank())
end

function Cyrukh_Trample(pUnit,Event)
	pUnit:FullCastSpellOnTarget(39425,pUnit:GetMainTank())
end

function Cyrukh_KnockAway(pUnit,Event)
	pUnit:FullCastSpellOnTarget(18945,pUnit:GetMainTank())
end

function Cyrukh_Death(pUnit,Event)
	pUnit:RemoveEvents()
end

function Cyrukh_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21181, 1, "Cyrukh_OnEnterCombat")
RegisterUnitEvent(21181, 4, "Cyrukh_Death")
RegisterUnitEvent(21181, 2, "Cyrukh_OnLeaveCombat")

function DCHarbinger_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("DCHarbinger_Spell", 13000, 0)
	pUnit:RegisterEvent("DCHarbinger_Heal", 25000, 0)
end

function DCHarbinger_Spell(pUnit,Event)
	pUnit:FullCastSpelOnTarget(15496,pUnit:GetClosestPlayer())
end

function DCHarbinger_Heal(pUnit,Event)
	pUnit:FullCastSpelOnTarget(16588,pUnit:GetRandomFriend())
end

function DCHarbinger_Died(pUnit,Event)
	pUnit:RemoveEvents()
end

function DCHarbinger_LeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21384, 1, "DCHarbinger_OnEnterCombat")
RegisterUnitEvent(21384, 4, "DCHarbinger_Died")
RegisterUnitEvent(21384, 2, "DCHarbinger_LeaveCombat")

function DCHawkeye_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("DCHawkeye_Spell", 32000, 0)
end

function DCHawkeye_Spell(pUnit,Event)
	pUnit:FullCastSpellOnTarget(37974,pUnit:GetClosestPlayer())
end

function DCHawkeye_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function DCHawkeye_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21386, 1, "DCHawkeye_OnEnterCombat")
RegisterUnitEvent(21386, 2, "DCHawkeye_OnLeaveCombat")
RegisterUnitEvent(21386, 4, "DCHawkeye_OnDied")

function DCRavenguard_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("DCRavenguard_Enrage", 120000, 0)
	pUnit:RegisterEvent("DCRavenguard_Howl", 27000, 0)
end

function DCRavenguard_Enrage(pUnit,Event)
	pUnit:FullCastSpellOnTarget(8599,pUnit:GetClosestPlayer())
end

function DCRavenguard_Howl(pUnit,Event)
	pUnit:FullCastSpellOnTarget(23600,pUnit:GetClosestPlayer())
end

function DCRavenguard_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

function DCRavenguard_LeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(19827, 1, "DCRavenguard_OnEnterCombat")
RegisterUnitEvent(19827, 2, "DCRavenguard_LeaveCombat")
RegisterUnitEvent(19827, 4, "DCRavenguard_OnDied")

function DCScorncrow_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("DCScorncrow_FrostBolt", 12000, 0)
end

function DCScorncrow_Frostbolt(pUnit,Event)
	pUnit:FullCastSpellOnTarget(9672,pUnit:GetClosestPlayer())
end


function DCScorncrow_LeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function DCScorncrow_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21385, 1, "DCScorncrow_OnEnterCombat")
RegisterUnitEvent(21385, 2, "DCScorncrow_LeaveCombat")
RegisterUnitEvent(21385, 4, "DCScorncrow_OnDied")

function DCShadowmancer_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("DCShadowmancer_ShadowBolt", 13000, 0)
	pUnit:RegisterEvent("DCShadowmancer_DarkMending", 25000, 0)
end

function DCShadowmancer_ShadowBolt(pUnit,Event)
	pUnit:FullCastSpelOnTarget(9613,pUnit:GetClosestPlayer())
end

function DCShadowmancer_DarkMending(pUnit,Event)
	pUnit:FullCastSpelOnTarget(16588,pUnit:GetRandomFriend())
end

function DCShadowmancer_Died(pUnit,Event)
	pUnit:RemoveEvents()
end

function DCShadowmancer_LeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21384, 1, "DCShadowmancer_OnEnterCombat")
RegisterUnitEvent(21384, 4, "DCShadowmancer_Died")
RegisterUnitEvent(21384, 2, "DCShadowmancer_LeaveCombat")

function DCTalonite_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("DCTalonite_ColdTouch", 18000, 0)
	pUnit:RegisterEvent("DCTalonite_TalonOfJustice", 15000, 0)
end

function DCTalonite_ColdTouch(pUnit,Event)
	pUnit:FullCastSpellOnTarget(39230,pUnit:GetClosestPlayer())
end


function DCTalonite_TalonOfJustice(pUnit,Event)
	pUnit:FullCastSpellOnTarget(39229,pUnit:GetClosestPlayer())
end

function DCTalonite_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

function DCTalonite_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(19825, 1, "DCTalonite_OnEnterCombat")
RegisterUnitEvent(19825, 4, "DCTalonite_OnDied")
RegisterUnitEvent(19825, 2, "DCTalonite_OnLeaveCombat")

function DGuardian_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("DGuardian_Strike", 9000, 0)
	pUnit:RegisterEvent("DGuardian_Bash", 20000, 0)
end

function DGuardian_Strike(pUnit,Event)
	pUnit:FullCastSpellOnTarget(37998,pUnit:GetClosestPlayer())
end

function DGuardian_Bash(pUnit,Event)
	pUnit:FullCastSpellOnTarget(11972,pUnit:GetClosestPlayer())
end

function DGuardian_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

function DGuardian_LeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(20878, 1, "DGuardian_OnEnterCombat")
RegisterUnitEvent(20878, 1, "DGuardian_OnDied")
RegisterUnitEvent(20878, 1, "DGuardian_LeaveCombat")

function Dimp_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Dimp_Firebolt", 6000, 0)
end

function Dimp_Firebolt(pUnit,Event)
	pUnit:FullCastSpellOnTarget(36227,pUnit:GetClosestPlayer())
end

function Dimp_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

function Dimp_LeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(20887, 1, "Dimp_OnEnterCombat")
RegisterUnitEvent(20887, 2, "Dimp_LeaveCombat")
RegisterUnitEvent(20887, 4, "Dimp_OnDied")

function DInfernal_Yell(pUnit, Event)
	pUnit:RemoveEvents()
	pUnit:CastSpell(36658)
end

function DInfernal_Start(pUnit, Event)
	pUnit:RegisterEvent("DInfernal_Yell", 1000, 0)
end

RegisterUnitEvent(21316, 6, "DInfernal_Start")

function Dmine_OnEnterCombat(pUnit,Event)
	pUnit:GetMainTank()
	pUnit:CastSpell(5)
end

function Dmine_LeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Dmine_OnDied(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38108,pUnit:GetMainTank())
end

RegisterUnitEvent(22315, 2, "Dmine_OnEnterCombat")
RegisterUnitEvent(22315, 2, "Dmine_LeaveCombat")
RegisterUnitEvent(22315, 4, "Dmine_OnDied")

function DSmith_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("DSmith_ChaosNova", 6000, 0)
	pUnit:RegisterEvent("DSmith_DrillArmor", 6000, 0)
end

function DSmith_ChaosNova(pUnit,Event)
	pUnit:FullCastSpellOnTarget(36225,pUnit:GetClosestPlayer())
end

function DSmith_DrillArmor(pUnit,Event)
	pUnit:FullCastSpellOnTarget(37580,pUnit:GetClosestPlayer())
end

function DSmith_LeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function DSmith_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(19756, 1, "DSmith_OnEnterCombat")
RegisterUnitEvent(19756, 2, "DSmith_LeaveCombat")
RegisterUnitEvent(19756, 4, "DSmith_OnDied")

function DSummoner_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("DSummoner_ShadowBolt", 10000, 0)
	pUnit:RegisterEvent("DSummoner_ShadowBolt", 16000, 0)
end

function DSummoner_ShadowBolt(pUnit,Event)
	pUnit:FullCastSpellOnTarget(9613,pUnit:GetClosestPlayer())
end

function DSummoner_FelImmolate(pUnit,Event)
	pUnit:FullCastSpellOnTarget(37628,pUnit:GetClosestPlayer())
end

function DSummoner_OnDeath(pUnit,Event)
	pUnit:RemoveEvents()
end

function DSummoner_LeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(20872, 1, "DSummoner_OnEnterCombat")
RegisterUnitEvent(20872, 2, "DSummoner_LeaveCombat")
RegisterUnitEvent(20872, 4, "DSummoner_OnDeath")

function DTinkerer_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("DTinkerer_Spell", 23000, 0)
end

function DTinkerer_Spell(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38753,pUnit:GetClosestPlayer())
end

function DTinkerer_LeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function DTinkerer_OnDeath(pUnit,Event)
	pUnit:RemoveEvents()
	pUnit:CastSpell(38107)
end

RegisterUnitEvent(19754, 1, "DTinkerer_OnEnterCombat")
RegisterUnitEvent(19754, 2, "DTinkerer_LeaveCombat")
RegisterUnitEvent(19754, 4, "DTinkerer_OnDeath")


function DHInitiate_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("DHInitiate_SpellBreaker", 18000, 0)
end

function DHInitiate_SpellBreaker(pUnit,Event)
	pUnit:FullCastSpellOnTarget(35871,pUnit:GetClosestPlayer())
end

function DHInitiate_LeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function DHInitiate_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21180, 1, "DHInitiate_OnEnterCombat")
RegisterUnitEvent(21180, 2, "DHInitiate_LeaveCombat")
RegisterUnitEvent(21180, 4, "DHInitiate_OnDied")

function DHSupplicant_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("DHSupplicant_Spell", 40000, 0)
end

function DHSupplicant_Spell(pUnit,Event)
	pUnit:CastSpell(37683)
end

function DHSupplicant_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

function DHSupplicant_LeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21179, 1, "DHSupplicant_OnEnterCombat")
RegisterUnitEvent(21179, 2, "DHSupplicant_LeaveCombat")
RegisterUnitEvent(21179, 4, "DHSupplicant_OnDied")

function DDPeon_Spell(pUnit, Event)
	pUnit:RemoveEvents()
	pUnit:CastSpell(40732)
	pUnit:RegisterEvent("DDPeon_Spell2", 10000, 0)
end

function DDPeon_Spell2(pUnit, Event)
	pUnit:RemoveEvents()
	pUnit:CastSpell(40735)
	pUnit:RegisterEvent("DDPeon_Spell3", 20000, 0)
end

function DDPeon_Spell3(pUnit, Event)
	pUnit:RemoveEvents()
	pUnit:CastSpell(40714)
	pUnit:RegisterEvent("DDPeon_Spell1", 30000, 0)
end

function DDPeon_Start(pUnit, Event)
	pUnit:RegisterEvent("DDPeon_Spell", 1000, 0)
end

RegisterUnitEvent(21316, 6, "DDPeon_Start")

function DFelboar_OnEnterCombat(pUnit,Event)
	pUnit:CastSpell(36462)
	pUnit:FullCastSpellOnTarget(22120,pUnit:GetClosestPlayer())
end

function DFelboar_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

function DFelboar_LeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21195, 1, "DFelboar_OnEnterCombat")
RegisterUnitEvent(21195, 2, "DFelboar_LeaveCombat")
RegisterUnitEvent(21195, 4, "DFelboar_OnDied")

function DAscendant_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("DAscendant_Cleave", 40000, 0)
	pUnit:RegisterEvent("DAscendant_MortalStrike", 15000, 0)
	pUnit:RegisterEvent("DAscendant_Uppercut", 23000, 0)
end

function DAscendant_Cleave(pUnit,Event)
	pUnit:FullCastSpellOnTarget(15496,pUnit:GetClosestPlayer())
end

function DAscendant_MortalStrike(pUnit,Event)
	pUnit:FullCastSpellOnTarget(17547,pUnit:GetClosestPlayer())
end

function DAscendant_Uppercut(pUnit,Event)
	pUnit:FullCastSpellOnTarget(10966,pUnit:GetClosestPlayer())
end

function DAscendant_LeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function DAscendant_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(22253, 1, "DAscendant_OnEnterCombat")
RegisterUnitEvent(22253, 2, "DAscendant_LeaveCombat")
RegisterUnitEvent(22253, 4, "DAscendant_OnDied")

function DDrakeRider_OnEnterCombat(pUnit,Event)
	pUnit:FullCastSpellOnTarget(6660,pUnit:GetClosestPlayer())
end

function DDrakeRider_LeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function DDrakeRider_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21719, 1, "DDrakeRider_OnEnterCombat")
RegisterUnitEvent(21719, 2, "DDrakeRider_LeaveCombat")
RegisterUnitEvent(21719, 4, "DDrakeRider_OnDied")

function DElite_OnEnterCombat(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38858,pUnit:GetClosestPlayer())
	pUnit:RegisterEvent("DElite_Spell1", 36000, 0)
	pUnit:RegisterEvent("DElite_Spell2", 50000, 0)
end

function DElite_Spell1(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38861,pUnit:GetClosestPlayer())
end

function DElite_Spell2(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38859,pUnit:GetClosestPlayer())
end

function DElite_LeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function DElite_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(22331, 1, "DElite_OnEnterCombat")
RegisterUnitEvent(22331, 2, "DElite_LeaveCombat")
RegisterUnitEvent(22331, 4, "DElite_OnDied")

function DNetherDrake_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("DNetherDrake_ArcaneBlast", 11000, 0)
end

function DNetherDrake_ArcaneBlast(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38344,pUnit:GetClosestPlayer())
end

function DNetherDrake_LeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function DNetherDrake_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(22000, 1, "DNetherDrake_OnEnterCombat")
RegisterUnitEvent(22000, 2, "DNetherDrake_LeaveCombat")
RegisterUnitEvent(22000, 4, "DNetherDrake_OnDied")

function DNPeon_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("DNPeon_Spell", 46000, 0)
end

function DNPeon_Spell(pUnit,Event)
	pUnit:FullCastSpellOnTarget(15572,pUnit:GetClosestPlayer())
end

function DNPeon_LeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function DNPeon_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(22252, 1, "DNPeon_OnEnterCombat")
RegisterUnitEvent(22252, 2, "DNPeon_LeaveCombat")
RegisterUnitEvent(22252, 4, "DNPeon_OnDied")

local cry_delay = 100000
local announces = {}
local announcei = 3
local choice = 1

RegisterUnitEvent(22252, 6, "DragonmawPeon_Random_Setup")
announces[1] = "It put the mutton in the stomach!"
announces[2] = "WHY IT PUT DA BOOTERANG ON DA SKIN?! WHY?!"
announces[3] = "You is bad orc... baaad... or... argh!"

function DragonmawPeon_Random_Tick(pUnit, Event)
   choice = math.random(1, announcei)
   pUnit:SendChatMessage(12, 0, announces[choice])
end

function DragonmawPeon_Random_Setup(pUnit, Event)
   pUnit:RegisterEvent("DragonmawPeon_Random_Tick", cry_delay, 0)
end

function DShaman_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("DShaman_LShield", 20000, 0)
	pUnit:RegisterEvent("DShaman_Bloodlust", 33000, 0)
end

function DShaman_LShield(pUnit,Event)
	pUnit:CastSpell(12550)
end

function DShaman_Bloodlust(pUnit,Event)
	pUnit:CastSpell(6742)
end

function DShaman_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

function DShaman_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21720, 1, "DShaman_OnEnterCombat")
RegisterUnitEvent(21720, 2, "DShaman_OnLeaveCombat")
RegisterUnitEvent(21720, 4, "DShaman_OnDied")

function DSkybreaker_OnEnterCombat(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38858,pUnit:GetClosestPlayer())
	pUnit:RegisterEvent("DSkybreaker_Spell1", 24000, 0)
	pUnit:RegisterEvent("DSkybreaker_Spell2", 11000, 0)
end

function DSkybreaker_Spell1(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38861,pUnit:GetClosestPlayer())
end

function DSkybreaker_Spell2(pUnit,Event)
	pUnit:FullCastSpellOnTarget(41448,pUnit:GetClosestPlayer())
end

function DSkybreaker_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function DSkybreaker_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(22274, 1, "DSkybreaker_OnEnterCombat")
RegisterUnitEvent(22274, 2, "DSkybreaker_OnLeaveCombat")
RegisterUnitEvent(22274, 4, "DSkybreaker_OnDied")

function DSubjugator_OnEnterCombat(pUnit,Event)
	pUnit:FullCastSpellOnTarget(9613,pUnit:GetClosestPlayer())
	pUnit:RegisterEvent("DSubjugator_Shadowbolt", 9000, 0)
end

function DSubjugator_Shadowbolt(pUnit,Event)
	pUnit:FullCastSpellOnTarget(9613,pUnit:GetClosestPlayer())
end

function DSubjugator_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end
function DSubjugator_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21718, 1, "DSubjugator_OnEnterCombat")
RegisterUnitEvent(21718, 2, "DSubjugator_OnLeaveCombat")
RegisterUnitEvent(21718, 4, "DSubjugator_OnDied")

function DTransporter_OnEnterCombat(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38858,pUnit:GetClosestPlayer())
	pUnit:RegisterEvent("DTransporter_Spell1Shoot", 33000, 0)
	pUnit:RegisterEvent("DTransporter_Spell2Shoot", 20000, 0)
end

function DTransporter_Spell1Shoot(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38861,pUnit:GetClosestPlayer())
end

function DTransporter_Spell2Shoot(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38859,pUnit:GetClosestPlayer())
end

function DTransporter_OnDeath(pUnit,Event)
	pUnit:RemoveEvents()
end

function DTransporter_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(23188, 1, "DTransporter_OnEnterCombat")
RegisterUnitEvent(23188, 2, "DTransporter_OnLeaveCombat")
RegisterUnitEvent(23188, 4, "DTransporter_OnDeath")

function DWrangler_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("DWrangler_Net", 20000, 0)
	pUnit:RegisterEvent("DWrangler_Enrage", 1000, 0)
end

function DWrangler_Net(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38338,pUnit:GetClosestPlayer())
end

function DWrangler_Enrage(pUnit,Event)
 if pUnit:GetHealthPct() == 94 then
	pUnit:CastSpell(8599)
end
end

function DWrangler_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function DWrangler_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21717, 1, "DWrangler_OnEnterCombat")
RegisterUnitEvent(21717, 2, "DWrangler_OnLeaveCombat")
RegisterUnitEvent(21717, 4, "DWrangler_OnDied")

function Dreadwarden_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Dreadwarden_Spell1", 34000, 0)
	pUnit:RegisterEvent("Dreadwarden_Spell2", 21000, 0)
end

function Dreadwarden_Spell1(pUnit,Event)
	pUnit:FullCastSpellOnTarget(11443,pUnit:GetClosestPlayer())
end

function Dreadwarden_Spell2(pUnit,Event)
	pUnit:FullCastSpellOnTarget(32736,pUnit:GetClosestPlayer())
end

function Dreadwarden_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Dreadwarden_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(19744, 1, "Dreadwarden_OnEnterCombat")
RegisterUnitEvent(19744, 2, "Dreadwarden_OnLeaveCombat")
RegisterUnitEvent(19744, 4, "Dreadwarden_OnDied")

function EArchmage_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("EArchmage_Spell1", 14000, 0)
	pUnit:RegisterEvent("EArchmage_Spell2", 21000, 0)
	pUnit:RegisterEvent("EArchmage_Spell3", 10000, 0)
end

function EArchmage_Spell1(pUnit,Event)
	pUnit:FullCastSpellOnTarget(37986,pUnit:GetClosestPlayer())
end

function EArchmage_Spell2(pUnit,Event)
	pUnit:FullCastSpellOnTarget(11829,pUnit:GetClosestPlayer())
end

function EArchmage_Spell3(pUnit,Event)
	pUnit:FullCastSpellOnTarget(13878,pUnit:GetClosestPlayer())
end

function EArchmage_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function EArchmage_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(19796, 1, "EArchmage_OnEnterCombat")
RegisterUnitEvent(19796, 2, "EArchmage_OnLeaveCombat")
RegisterUnitEvent(19796, 4, "EArchmage_OnDied")

function EBloodwarder_OnEnterCombat(pUnit,Event)
	pUnit:CastSpell(38311)
	pUnit:RegisterEvent("EBloodwarder_BloodLeech", 9000, 0)
end

function EBloodwarder_BloodLeech(pUnit,Event)
	pUnit:FullCastSpellOnTarget(37838,pUnit:GetClosestPlayer())
end

function EBloodwarder_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function EBloodwarder_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(19806, 1, "EBloodwarder_OnEnterCombat")
RegisterUnitEvent(19806, 2, "EBloodwarder_OnLeaveCombat")
RegisterUnitEvent(19806, 4, "EBloodwarder_OnDied")

function ECavalier_OnEnterCombat(pUnit,Event)
	pUnit:CastSpell(38311)
	pUnit:RegisterEvent("ECavalier_SpellBreaker", 20000, 0)
	pUnit:RegisterEvent("ECavalier_BattleShout", 60000, 0)
end

function ECavalier_SpellBreaker(pUnit,Event)
	pUnit:CastSpell(35871)
end

function ECavalier_BattleShout(pUnit,Event)
	pUnit:CastSpell(30931)
end

function ECavalier_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function ECavalier_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(22018, 1, "ECavalier_OnEnterCombat")
RegisterUnitEvent(22018, 2, "ECavalier_OnLeaveCombat")
RegisterUnitEvent(22018, 4, "ECavalier_OnDied")

function ECenturion_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("ECenturion_SpellBreaker", 18000, 0)
	pUnit:RegisterEvent("ECenturion_Bloodheal", 1000, 0)
end


function ECenturion_SpellBreaker(pUnit,Event)
	pUnit:FullCastSpellOnTarget(35871,pUnit:GetClosestPlayer())
end

function ECenturion_Bloodheal(pUnit,Event)
 if pUnit:GetHealthPct() == 2 then
	pUnit:CastSpell(36476)
end
end

function ECenturion_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function ECenturion_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(19792, 1, "ECenturion_OnEnterCombat")
RegisterUnitEvent(19792, 2, "ECenturion_OnLeaveCombat")
RegisterUnitEvent(19792, 4, "ECenturion_OnDied")

function EDragonhawk_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("EDragonhawk_Firebreath", 18000, 0)
end

function EDragonhawk_Firebreath(pUnit,Event)
	pUnit:FullCastSpellOnTarget(37985,pUnit:GetClosestPlayer())
end

function EDragonhawk_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function EDragonhawk_OnDeath(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(20502, 1, "EDragonhawk_OnEnterCombat")
RegisterUnitEvent(20502, 2, "EDragonhawk_OnLeaveCombat")
RegisterUnitEvent(20502, 4, "EDragonhawk_OnDeath")

function ESoldier_OnEnterCombat(pUnit,Event)
 if pUnit:GetHealthPct() == 3 then
	pUnit:CastSpell(36476)
end
end

function ESoldier_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function ESoldier_OnDeath(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(22016, 1, "ESoldier_OnEnterCombat")
RegisterUnitEvent(22016, 2, "ESoldier_OnLeaveCombat")
RegisterUnitEvent(22016, 4, "ESoldier_OnDeath")

function ESpellbinder_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("ESpellbinder_SummonArcane", 11000, (1))
	pUnit:RegisterEvent("ESpellbinder_ArcaneMissle", 16000, 0)
end

function ESpellbinder_SummonArcane(pUnit,Event)
	pUnit:CastSpell(38171)
end

function ESpellbinder_ArcaneMissle(pUnit,Event)
	pUnit:FullCastSpellOnTarget(34447,pUnit:GetClosestPlayer())
end

function ESpellbinder_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function ESpellbinder_OnDeath(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(22017, 1, "ESpellbinder_OnEnterCombat")
RegisterUnitEvent(22017, 2, "ESpellbinder_OnLeaveCombat")
RegisterUnitEvent(22017, 4, "ESpellbinder_OnDeath")

function EDemolisher_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("EDemolisher_Stomp", 19000, 0)
end

function EDemolisher_Stomp(pUnit,Event)
	pUnit:CastSpell(38045)
end

function EDemolisher_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function EDemolisher_OnDeath(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21802, 1, "EDemolisher_OnEnterCombat")
RegisterUnitEvent(21802, 2, "EDemolisher_OnLeaveCombat")
RegisterUnitEvent(21802, 4, "EDemolisher_OnDeath")

function AirSpirit_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("AirSpirit_Spell1", 11000, 0)
	pUnit:RegisterEvent("AirSpirit_Spell2", 25000, 0)
end

function AirSpirit_Spell1(pUnit,Event)
	pUnit:FullCastSpellOnTarget(12058,pUnit:GetClosestPlayer())
end

function AirSpirit_Spell2(punit,Event)
	pUnit:FullCastSpellOnTarget(32717,pUnit:GetClosestPlayer())
end

function AirSpirit_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function AirSpirit_OnDeath(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21060, 1, "AirSpirit_OnEnterCombat")
RegisterUnitEvent(21060, 2, "AirSpirit_OnLeaveCombat")
RegisterUnitEvent(21060, 4, "AirSpirit_OnDeath")

function EarthSpirit_OnEnterCombat(pUnit,Event)
	pUnit:CastSpell(38365)
	pUnit:RegisterEvent("EarthSpirit_Boulder", 8500, 0)
end

function EarthSpirit_Boulder(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38498,pUnit:GetClosestPlayer())
end

function EarthSpirit_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function EarthSpirit_OnDeath(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21050, 1, "EarthSpirit_OnEnterCombat")
RegisterUnitEvent(21050, 2, "EarthSpirit_OnLeaveCombat")
RegisterUnitEvent(21050, 4, "EarthSpirit_OnDeath")

function FireSpirit_OnEnterCombat(pUnit,Event)
	pUnit:CastSpell(36006)
	pUnit:RegisterEvent("FireSpirit_Enrage", 1000, 0)
	pUnit:RegisterEvent("FireSpirit_FelFireball", 3000, 0)
end

function FireSpirit_Enrage(pUnit,Event)
 if pUnit:GetHealthPct() == 97 then
	pUnit:CastSpell(8599)
end
end

function FireSpirit_FelFireball(pUnit,Event)
	pUnit:FullCastSpellOnTarget(36247,pUnit:GetClosestPlayer())
end

function FireSpirit_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function FireSpirit_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21061, 1, "FireSpirit_OnEnterCombat")
RegisterUnitEvent(21061, 2, "FireSpirit_OnLeaveCombat")
RegisterUnitEvent(21061, 4, "FireSpirit_OnDied")

function WaterSoul_OnEnterCombat(pUnit,Event)
	pUnit:CastSpell(35923)
end	

RegisterUnitEvent(21109, 1, "WaterSoul_OnEnterCombat")

function WaterSpirit_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("WaterSpirit_Stormbolt", 12000, 0)
end

function WaterSpirit_Stormbolt(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38032,pUnit:GetClosestPlayer())
end

function WaterSpirit_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function WaterSpirit_OnDeath(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21059, 1, "WaterSpirit_OnEnterCombat")
RegisterUnitEvent(21059, 1, "WaterSpirit_OnLeaveCombat")
RegisterUnitEvent(21059, 1, "WaterSpirit_OnDeath")

function ENDrake_OnEnterCombat(pUnit,Event)
	pUnit:CastSpell(38775)
	pUnit:RegisterEvent("ENDrake_Spell", 13000, 0)
end

function ENDrake_Spell(pUnit,Event)
	pUnit:FullCastSpellOnTarget(36513,pUnit:GetClosestPlayer())
end

function ENDrake_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function ENDrake_OnDeath(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21722, 1, "ENDrake_OnEnterCombat")
RegisterUnitEvent(21722, 2, "ENDrake_OnLeaveCombat")
RegisterUnitEvent(21722, 4, "ENDrake_OnDeath")

function ENWhelp_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("ENWhelp_Spell", 4000, 0)
end

function ENWhelp_Spell(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38340,pUnit:GetClosestPlayer())
end

function ENWhelp_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function ENWhelp_OnDeath(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21721, 1, "ENWhelp_OnEnterCombat")
RegisterUnitEvent(21721, 1, "ENWhelp_OnLeaveCombat")
RegisterUnitEvent(21721, 1, "ENWhelp_OnDeath")

function Eykenen_OnEnterCombat(pUnit,Event)
	pUnit:CastSpell(32734)
	pUnit:RegisterEvent("Eykenen_EarthShield", 43000, 0)
	pUnit:RegisterEvent("Eykenen_EarthShock", 10000, 0)
end

function Eykenen_EarthShield(pUnit,Event)
	pUnit:CastSpell(32734)
end

function Eykenen_EarthShock(pUnit,Event)
	pUnit:FullCastSpellOnTarget(13281,pUnit:GetClosestPlayer())
end

function Eykenen_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Eykenen_OnDeath(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21709, 1, "Eykenen_OnEnterCombat")
RegisterUnitEvent(21709, 1, "Eykenen_OnLeaveCombat")
RegisterUnitEvent(21709, 1, "Eykenen_OnDeath")

function FRSentinel_OnEnterCombat(pUnit,Event)
	pUnit:CastSpell(38022)
	pUnit:RegisterEvent("FRSentinel_Boom", 000, 0)
	pUnit:RegisterEvent("FRSentinel_WorldBreaker", 16000, 0)
end

function FRSentinel_Boom(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38052,pUnit:GetClosestPlayer())
end

function FRSentinel_WorldBreaker(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38052,pUnit:GetClosestPlayer())
end

function FRSentinel_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function FRSentinel_OnDeath(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21949, 1, "FRSentinel_OnEnterCombat")
RegisterUnitEvent(21949, 1, "FRSentinel_OnLeaveCombat")
RegisterUnitEvent(21949, 1, "FRSentinel_OnDeath")

function FelBoar_OnEnterCombat(pUnit,Event)
	pUnit:FullCastSpellOnTarget(35570,pUnit:GetClosestPlayer())
end

RegisterUnitEvent(21878, 1, "FelBoar_OnEnterCombat")

function FDiemetradon_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("FDiemetradon_Spell", 40000, 0)
end

function FDiemetradon_Spell(pUnit,Event)
	pUnit:FullCastSpellOnTarget(37941,pUnit:GetClosestPlayer())
end

function FDiemetradon_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function FDiemetradon_OnDeath(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21408, 1, "FDiemetradon_OnEnterCombat")
RegisterUnitEvent(21408, 2, "FDiemetradon_OnLeaveCombat")
RegisterUnitEvent(21408, 4, "FDiemetradon_OnDeath")

function Felspine_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Felspine_FelFlames", 11000, 0)
	pUnit:RegisterEvent("Felspine_FlamingWound", 46000, 0)
end

function Felspine_FelFlames(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38356,pUnit:GetClosestPlayer())
end

function Felspine_FlamingWound(pUnit,Event)
	pUnit:FullCastSpellOnTarget(37941,pUnit:GetClosestPlayer())
end

function Felspine_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Felspine_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end


RegisterUnitEvent(21897, 1, "Felspine_OnEnterCombat")
RegisterUnitEvent(21897, 2, "Felspine_OnLeaveCombat")
RegisterUnitEvent(21897, 4, "Felspine_OnDied")

function GFDiemetradon_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("GFDiemetradon_FelFireball", 11000, 0)
	pUnit:RegisterEvent("GFDiemetradon_FlamingWound", 51000, 0)
end

function GFDiemetradon_FelFireball(pUnit,Event)
	pUnit:FullCastSpellOnTarget(37945,pUnit:GetClosestPlayer())
end

function GFDiemetradon_FlamingWound(pUnit,Event)
	pUnit:FullCastSpellOnTarget(37941,pUnit:GetClosestPlayer())
end

function GFDiemetradon_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function GFDiemetradon_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21462, 1, "GFDiemetradon_OnEnterCombat")
RegisterUnitEvent(21462, 2, "GFDiemetradon_OnLeaveCombat")
RegisterUnitEvent(21462, 4, "GFDiemetradon_OnDied")

function Gromtor_OnEnterCombat(pUnit,Event)
	pUnit:FullCastSpellOnTarget(26281,pUnit:GetClosestPlayer())
	pUnit:RegisterEvent("Gromtor_SunderArmor", 120000, 0)
	pUnit:RegisterEvent("Gromtor_ShieldWall", 22000, 0)
	pUnit:RegisterEvent("Gromtor_ShieldBlock", 40000, 0)
	pUnit:RegisterEvent("Gromtor_HeroicStrike", 4000, 0)
	pUnit:RegisterEvent("Gromtor_BattleShout", 240000, 0)
end

function Gromtor_SunderArmor(pUnit,Event)
	pUnit:FullCastSpellOnTarget(16145,pUnit:GetClosestPlayer())
end

function Gromtor_ShieldWall(pUnit,Event)
	pUnit:FullCastSpellOnTarget(15062,pUnit:GetClosestPlayer())
end

function Gromtor_ShieldBlock(pUnit,Event)
	pUnit:FullCastSpellOnTarget(12169,pUnit:GetClosestPlayer())
end

function Gromtor_HeroicStrike(pUnit,Event)
	pUnit:FullCastSpellOnTarget(29426,pUnit:GetClosestPlayer())
end

function Gromtor_BattleShout(pUnit,Event)
	pUnit:FullCastSpellOnTarget(31403,pUnit:GetClosestPlayer())
end

function Gromtor_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Gromtor_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21291, 1, "Gromtor_OnEnterCombat")
RegisterUnitEvent(21291, 2, "Gromtor_OnLeaveCombat")
RegisterUnitEvent(21291, 4, "Gromtor_OnDied")

function GRGuard_OnEnterCombat(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38182,pUnit:GetClosestPlayer())
end

RegisterUnitEvent(15241, 1, "GRGuard_OnEnterCombat")

function Guldan_Channel(pUnit, Event)
	pUnit:RemoveEvents()
	pUnit:CastSpell(35996)
end

function Guldan_Start(pUnit, Event)
	pUnit:RegisterEvent("Guldan_Channel", 1000, 0)
end

RegisterUnitEvent(17008, 6, "Guldan_Start")

function Haalum_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Haalum_Chain", 15000, 0)
	pUnit:RegisterEvent("Haalum_LBolt", 9000, 0)
end

function Haalum_Chain(pUnit,Event)
	pUnit:FullCastSpellOnTarget(12058,pUnit:GetClosestPlayer())
end

function Haalum_LBolt(pUnit,Event)
	pUnit:FullCastSpellOnTarget(9532,pUnit:GetClosestPlayer())
end

function Haalum_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Haalum_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21711, 1, "Haalum_OnEnterCombat")
RegisterUnitEvent(21711, 2, "Haalum_OnLeaveCombat")
RegisterUnitEvent(21711, 4, "Haalum_OnDied")

function IAgonizer_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("IAgonizer_Firebolt", 6000, 0)
end

function IAgonizer_Firebolt(pUnit,Event)
	pUnit:FullCastSpellOnTarget(36227,pUnit:GetClosestPlayer())
end

function IAgonizer_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function IAgonizer_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(19801, 1, "IAgonizer_OnEnterCombat")
RegisterUnitEvent(19801, 2, "IAgonizer_OnLeaveCombat")
RegisterUnitEvent(19801, 4, "IAgonizer_OnDied")

function IDreadbringer_OnEnterCombat(pUnit,Event)
 if pUnit:GetHealthPct() == 91 then
	pUnit:FullCastSpellOnTarget(38167,pUnit:GetClosestPlayer())
end
end

RegisterUnitEvent(19799, 1, "IDreadbringer_OnEnterCombat")

function IDreadlord_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("IDreadlord_PsychicScream", 23000, 0)
	pUnit:RegisterEvent("IDreadlord_MindBlast", 7000, 0)
	pUnit:RegisterEvent("IDreadlord_Sleep", 50000, 0)
end

function IDreadlord_PsychicScream(pUnit,Event)
	pUnit:FullCastSpellOnTarget(13704,pUnit:GetClosestPlayer())
end

function IDreadlord_MindBlast(pUnit,Event)
	pUnit:FullCastSpellOnTarget(17287,pUnit:GetClosestPlayer())
end

function IDreadlord_Sleep(pUnit,Event)
	pUnit:FullCastSpellOnTarget(12098,pUnit:GetClosestPlayer())
end

function IDreadlord_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function IDreadlord_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21166, 1, "IDreadlord_OnEnterCombat")
RegisterUnitEvent(21166, 2, "IDreadlord_OnLeaveCombat")
RegisterUnitEvent(21166, 4, "IDreadlord_OnDied")

function IHighlord_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("IHighlord_Curseofflames", 240000, 0)
	pUnit:RegisterEvent("IHighlord_Flamestrike", 10000, 0)
	pUnit:RegisterEvent("IHighlord_Spell", 0001, 0)
end

function IHighlord_Curseofflames(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38010,pUnit:GetClosestPlayer())
end

function IHighlord_Spell(pUnit,Event)
 if pUnit:GetHealthPct() == 92 then 
	pUnit:FullCastSpellOnTarget(38010,pUnit:GetClosestPlayer())
end
end

function IHighlord_Flamestrike(pUnit,Event)
	pUnit:FullCastSpellOnTarget(16102,pUnit:GetClosestPlayer())
end

function IHighlord_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function IHighlord_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(19797, 1, "IHighlord_OnEnterCombat")
RegisterUnitEvent(19797, 2, "IHighlord_OnLeaveCombat")
RegisterUnitEvent(19797, 4, "IHighlord_OnDied")

function IJailor_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("IJailor_FelShackles", 24000, 0)
end

function IJailor_FelShackles(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38051,pUnit:GetClosestPlayer())
end

function IJailor_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function IJailor_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21520, 1, "IJailor_OnEnterCombat")
RegisterUnitEvent(21520, 2, "IJailor_OnLeaveCombat")
RegisterUnitEvent(21520, 4, "IJailor_OnDied")

function IMBreaker_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("IMBreaker_Spell1", 34000, 0)
	pUnit:RegisterEvent("IMBreaker_Spell2", 10000, 0)
	pUnit:RegisterEvent("IMBreaker_Spell3", 41000, 0)

end

function IMBreaker_Spell1(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38985,pUnit:GetClosestPlayer())
end

function IMBreaker_Spell2(pUnit,Event)
	pUnit:FullCastSpellOnTarget(17194,pUnit:GetClosestPlayer())
end

function IMBreaker_Spell3(pUnit,Event)
	pUnit:FullCastSpellOnTarget(22884,pUnit:GetClosestPlayer())
end

function IMBreaker_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function IMBreaker_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(22074, 1, "IMBreaker_OnEnterCombat")
RegisterUnitEvent(22074, 2, "IMBreaker_OnLeaveCombat")
RegisterUnitEvent(22074, 4, "IMBreaker_OnDied")

function IOverseer_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("IOverseer_MortalStrike", 20000, 0)
	pUnit:RegisterEvent("IOverseer_Rend", 50000, 0)
end

function IOverseer_MortalStrike(pUnit,Event)
	pUnit:FullCastSpellOnTarget(32736,pUnit:GetClosestPlayer())
end

function IOverseer_Rend(pUnit,Event)
	pUnit:FullCastSpellOnTarget(11977,pUnit:GetClosestPlayer())
end

function IOverseer_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function IOverseer_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21808, 1, "IOverseer_OnEnterCombat")
RegisterUnitEvent(21808, 2, "IOverseer_OnLeaveCombat")
RegisterUnitEvent(21808, 4, "IOverseer_OnDied")

function IPainlasher_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("IPainlasher_LashOfPain", 4000, 0)
end

function IPainlasher_LashOfPain(pUnit,Event)
	pUnit:FullCastSpellOnTarget(15968,pUnit:GetClosestPlayer())
end

function IPainlasher_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function IPainlasher_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(19800, 1, "IPainlasher_OnEnterCombat")
RegisterUnitEvent(19800, 2, "IPainlasher_OnLeaveCombat")
RegisterUnitEvent(19800, 4, "IPainlasher_OnDied")

function IRavager_OnEnterCombat(pUnit,Event)
	pUnit:FullCastSpellOnTarget(33645,pUnit:GetClosestPlayer())
	pUnit:RegisterEvent("IRavager_Cleave", 22000, 0)
	pUnit:RegisterEvent("IRavager_Cutdown", 9000, 0)
	pUnit:RegisterEvent("IRavager_Shout", 60000, 0)
end

function IRavager_Cleave(pUnit,Event)
	pUnit:FullCastSpellOnTarget(15496,pUnit:GetClosestPlayer())
end

function IRavager_Cutdown(pUnit,Event)
	pUnit:FullCastSpellOnTarget(32009,pUnit:GetClosestPlayer())
end

function IRavager_Shout(pUnit,Event)
	pUnit:CastSpell(16244)
end

function IRavager_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function IRavager_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(22857, 1, "IRavager_OnEnterCombat")
RegisterUnitEvent(22857, 2, "IRavager_OnLeaveCombat")
RegisterUnitEvent(22857, 4, "IRavager_OnDied")

function ISatyr_OnEnterCombat(pUnit,Event)
 if pUnit:GetHealthPct() == 97 then
	pUnit:FullCastSpellOnTarget(38048,pUnit:GetClosestPlayer())
end
end

RegisterUnitEvent(21656, 1, "ISatyr_OnEnterCombat")

function IShadowlord_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("IShadowlord_Sleep", 000, 0)
	pUnit:RegisterEvent("IShadowlord_CarrionSwarm", 000, 0)
	pUnit:RegisterEvent("IShadowlord_Inferno", 000, 0)
end

function IShadowlord_Sleep(pUnit,Event)
	pUnit:FullCastSpellOnTarget(12098,pUnit:GetClosestPlayer())
end

function IShadowlord_CarrionSwarm(pUnit,Event)
	pUnit:FullCastSpellOnTarget(39942,pUnit:GetClosestPlayer())
end

function IShadowlord_Inferno(pUnit,Event)
 if pUnit:GetHealthPct() == 64 then
	pUnit:FullCastSpellOnTarget(39942,pUnit:GetClosestPlayer())
end
end

function IShadowlord_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function IShadowlord_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(22988, 1, "IShadowlord_OnEnterCombat")
RegisterUnitEvent(22988, 2, "IShadowlord_OnLeaveCombat")
RegisterUnitEvent(22988, 4, "IShadowlord_OnDied")

function IShadowstalker_OnEnterCombat(pUnit,Event)
	pUnit:FullCastSpellOnTarget(7159,pUnit:GetClosestPlayer())
	pUnit:RegisterEvent("IShadowstalker_Backstab", 8000, 0)
end

function IShadowstalker_Backstab(pUnit,Event)
	pUnit:FullCastSpellOnTarget(7159,pUnit:GetClosestPlayer())
end

function IShadowstalker_Stealth(pUnit,Event)
	pUnit:CastSpell(5916)
end

function IShadowstalker_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function IShadowstalker_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21337, 1, "IShadowstalker_OnEnterCombat")
RegisterUnitEvent(21337, 6, "IShadowstalker_Stealth")
RegisterUnitEvent(21337, 2, "IShadowstalker_OnLeaveCombat")
RegisterUnitEvent(21337, 4, "IShadowstalker_OnDied")

function IShocktrooper_OnEnterCombat(pUnit,Event)
	pUnit:FullCastSpellOnTarget(22120,pUnit:GetClosestPlayer())
	pUnit:RegisterEvent("IShocktrooper_Cleave", 31000, 0)
end

function IShocktrooper_Cleave(pUnit,Event)
	pUnit:FullCastSpellOnTarget(15496,pUnit:GetClosestPlayer())
end

function IShocktrooper_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function IShocktrooper_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(19802, 1, "IShocktrooper_OnEnterCombat")
RegisterUnitEvent(19802, 2, "IShocktrooper_OnLeaveCombat")
RegisterUnitEvent(19802, 4, "IShocktrooper_OnDied")

function ISlayer_OnEnterCombat(pUnit,Event)
	pUnit:FullCastSpellOnTarget(35570,pUnit:GetClosestPlayer())
end

RegisterUnitEvent(21639, 1, "ISlayer_OnEnterCombat")

function ISoldier_OnEnterCombat(pUnit,Event)
 if pUnit:GetHealthPct() == 92 then
	pUnit:FullCastSpellOnTarget(35871,pUnit:GetClosestPlayer())
end
end

RegisterUnitEvent(22075, 1, "ISoldier_OnEnterCombat")

function IWatcher_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("IWatcher_MStrike", 35000, 0)
end

function IWatcher_MStrike(pUnit,Event)
	pUnit:FullCastSpellOnTarget(32736,pUnit:GetClosestPlayer())
end

function IWatcher_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function IWatcher_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(22093, 1, "IWatcher_OnEnterCombat")
RegisterUnitEvent(22093, 2, "IWatcher_OnLeaveCombat")
RegisterUnitEvent(22093, 4, "IWatcher_OnDied")

function ISoul_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("ISoul_Totem", 11000, 0)
end

function ISoul_Totem(pUnit,Event)
	pUnit:CastSpell(11969)
end

function ISoul_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function ISoul_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(19757, 1, "ISoul_OnEnterCombat")
RegisterUnitEvent(19757, 2, "ISoul_OnLeaveCombat")
RegisterUnitEvent(19757, 4, "ISoul_OnDied")

function Karsius_OnEnterCombat(pUnit,Event)
	pUnit:FullCastSpellOnTarget(37789,pUnit:GetClosestPlayer())
end

function Karsius_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Karsius_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21877, 1, "Karsius_OnEnterCombat")
RegisterUnitEvent(21877, 2, "Karsius_OnLeaveCombat")
RegisterUnitEvent(21877, 4, "Karsius_OnDied")

function KotCistern_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("KotCistern_Waterbolt", 4000, 0)
end

function KotCistern_Waterbolt(pUnit,Event)
	pUnit:FullCastSpellOnTarget(32011,pUnit:GetClosestPlayer())
end

function KotCistern_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function KotCistern_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(20795, 1, "KotCistern_OnEnterCombat")
RegisterUnitEvent(20795, 2, "KotCistern_OnLeaveCombat")
RegisterUnitEvent(20795, 4, "KotCistern_OnDied")

function KDefender_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("KDefender_Cleave", 11000, 0)
end

function KDefender_Cleave(pUnit,Event)
	pUnit:FullCastSpellOnTarget(15496,pUnit:GetMainTank())
end

function KDefender_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function KDefender_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(19362, 1, "KDefender_OnEnterCombat")
RegisterUnitEvent(19362, 2, "KDefender_OnLeaveCombat")
RegisterUnitEvent(19362, 4, "KDefender_OnDied")

function KRider_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("KRider_SnapKick", 6000, 0)
	pUnit:RegisterEvent("KRider_MortalStrike", 13000, 0)
end

function KRider_SnapKick(pUnit,Event)
	pUnit:FullCastSpellOnTarget(15618,pUnit:GetClosestPlayer())
end

function KRider_MortalStrike(pUnit,Event)
	pUnit:FullCastSpellOnTarget(19643,pUnit:GetClosestPlayer())
end

function KRider_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function KRider_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(19364, 1, "KRider_OnEnterCombat")
RegisterUnitEvent(19364, 2, "KRider_OnLeaveCombat")
RegisterUnitEvent(19364, 4, "KRider_OnDied")

function LadyShavRar_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("LadyShavRar_ArcaneBolt", 8500, 0)
	pUnit:RegisterEvent("LadyShavRar_Freeze", 43000, 0)
end

function LadyShavRar_ArcaneBolt(pUnit,Event)
	pUnit:FullCastSpellOnTarget(13901,pUnit:GetClosestPlayer())
end

function LadyShavRar_Freeze(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38035,pUnit:GetClosestPlayer())
end

function LadyShavRar_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function LadyShavRar_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(20684, 1, "LadyShavRar_OnEnterCombat")
RegisterUnitEvent(20684, 2, "LadyShavRar_OnLeaveCombat")
RegisterUnitEvent(20684, 4, "LadyShavRar_OnDied")

function Lakaan_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Lakaan_WaterBolt", 4000, 0)
end

function Lakaan_WaterBolt(pUnit,Event)
	pUnit:FullCastSpellOnTarget(32011,pUnit:GetClosestPlayer())
end

function Lakaan_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Lakaan_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21416, 1, "Lakaan_OnEnterCombat")
RegisterUnitEvent(21416, 2, "Lakaan_OnLeaveCombat")
RegisterUnitEvent(21416, 4, "Lakaan_OnDied")

function LFCannon_OnEnterCombat(pUnit,Event)
 if pUnit:GetHealthPct() == 70 then
	pUnit:FullCastSpellOnTarget(36238,pUnit:GetClosestPlayer())
end
end

RegisterUnitEvent(21233, 1, "LFCannon_OnEnterCombat")

function LERider_OnEnterCombat(pUnit,Event)
	pUnit:CastSpell(39782)
	pUnit:CastSpellOnTarget(31888,pUnit:GetClosestPlayer())
end

RegisterUnitEvent(22966, 1, "LERider_OnEnterCombat")

function LVindicator_OnEnterCombat(pUnit,Event)
	pUnit:FullCastSpellOnTarget(13005,pUnit:GetClosestPlayer())
	pUnit:RegisterEvent("LVindicator_Exorcism", 11000, 0)
	pUnit:RegisterEvent("LVindicator_HolyLight", 16000, 0)
end

function LVindicator_Exorcism(pUnit,Event)
	pUnit:FullCastSpellOnTarget(33632,pUnit:GetClosestPlayer())
end

function LVindicator_HolyLight(pUnit,Event)
	pUnit:FullCastSpellOnTarget(13952,pUnit:GetRandomFriend(0))
end

function LVindicator_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function LVindicator_OnDied(pUnit,Event)
	pUnit:FullCastSpellOnTarget(13903,pUnit:GetRandomFriend(0))
end

RegisterUnitEvent(22861, 1, "LVindicator_OnEnterCombat")
RegisterUnitEvent(22861, 2, "LVindicator_OnLeaveCombat")
RegisterUnitEvent(22861, 4, "LVindicator_OnDied")

function Lothros_OnEnterCombat(pUnit,Event)
	pUnit:RegisterUnitEvent("Lothros_Spell", 60000, 0)
end

function Lothros_Spell(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38167,pUnit:GetClosestPlayer())
end

function Lothros_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Lothros_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21928, 1, "Lothros_OnEnterCombat")
RegisterUnitEvent(21928, 2, "Lothros_OnLeaveCombat")
RegisterUnitEvent(21928, 4, "Lothros_OnDied")

function Makazradon_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Makazradon_Cripple", 45000, 0)
	pUnit:RegisterEvent("Makazradon_FelCleave", 19000, 0)
	pUnit:RegisterEvent("Makazradon_RainOfFire", 18000, 0)
end

function Makazradon_Cripple(pUnit,Event)
	pUnit:FullCastSpellOnTarget(11443,pUnit:GetClosestPlayer())
end

function Makazradon_FelCleave(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38742,pUnit:GetClosestPlayer())
end

function Makazradon_RainOfFire(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38741,pUnit:GetClosestPlayer())
end

function Makazradon_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Makazradon_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21501, 1, "Makazradon_OnEnterCombat")
RegisterUnitEvent(21501, 2, "Makazradon_OnLeaveCombat")
RegisterUnitEvent(21501, 4, "Makazradon_OnDied")

function MatureNetherwingDrake_OnEnterCombat(pUnit,Event)
	pUnit:CastSpell(38502)
	pUnit:RegisterEvent("MatureNetherwingDrake_IntangiblePresence", 16000, 0)
	pUnit:RegisterEvent("MatureNetherwingDrake_Netherbreath", 5000, 0)
end

function MatureNetherwingDrake_IntangiblePresence(pUnit,Event)
	pUnit:FullCastSpellOnTarget(36513,pUnit:GetClosestPlayer())
end

function MatureNetherwingDrake_Netherbreath(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38467,pUnit:GetClosestPlayer())
end

function MatureNetherwingDrake_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function MatureNetherwingDrake_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21648, 1, "MatureNetherwingDrake_OnEnterCombat")
RegisterUnitEvent(21648, 2, "MatureNetherwingDrake_OnLeaveCombat")
RegisterUnitEvent(21648, 4, "MatureNetherwingDrake_OnDied")

function MoArgWeaponsmith_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("MoArgWeaponsmith_ChemicalFlames", 13000, 0)
	pUnit:RegisterEvent("MoArgWeaponsmith_DrillArmor", 18000, 0)
end

function MoArgWeaponsmith_ChemicalFlames(pUnit,Event)
	pUnit:FullCastSpellOnTarget(36253,pUnit:GetClosestPlayer())
end

function MoArgWeaponsmith_DrillArmor(pUnit,Event)
	pUnit:FullCastSpellOnTarget(37580,pUnit:GetClosestPlayer())
end

function MoArgWeaponsmith_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function MoArgWeaponsmith_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(19755, 1, "MoArgWeaponsmith_OnEnterCombat")
RegisterUnitEvent(19755, 2, "MoArgWeaponsmith_OnLeaveCombat")
RegisterUnitEvent(19755, 4, "MoArgWeaponsmith_OnDied")

function Mordenai_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Mordenai_Shoot", 2000, 0)
	pUnit:RegisterEvent("Mordenai_Start", 2000, 0)
end

function Mordenai_Shoot(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38372,pUnit:GetClosestPlayer())
end

function Mordenai_Start(pUnit,Eevnt)
 if pUnit:GetHealthPct() == 98 then
	pUnit:RemoveEvents()
	pUnit:RegisterEvent("Mordenai_AimedShot", 16000, 0)
	pUnit:RegisterEvent("Mordenai_ArcaneShot", 6000, 0)
end
end

function Mordenai_AimedShot(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38370,pUnit:GetClosestPlayer())
end

function Mordenai_ArcaneShot(pUnit,Event)
	pUnit:FullCastSpellOnTarget(36623,pUnit:GetClosestPlayer())
end

function Mordenai_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Mordenai_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(22113, 1, "Mordenai_OnEnterCombat")
RegisterUnitEvent(22113, 2, "Mordenai_OnLeaveCombat")
RegisterUnitEvent(22113, 4, "Mordenai_OnDied")

function Morgroron_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Morgroron_MortalCleave", 12000, 0)
	pUnit:RegisterEvent("Morgroron_RainOfFire", 18000, 0)
	pUnit:RegisterEvent("Morgroron_WarStomp", 7000, 0)
end

function Morgroron_MortalCleave(pUnit,Event)
	pUnit:FullCastSpellOnTarget(22859,pUnit:GetClosestPlayer())
end

function Morgroron_RainOfFire(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38741,pUnit:GetClosestPlayer())
end

function Morgroron_WarStomp(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38750,pUnit:GetClosestPlayer())
end

function Morgroron_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Morgroron_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21500, 1, "Morgroron_OnEnterCombat")
RegisterUnitEvent(21500, 2, "Morgroron_OnLeaveCombat")
RegisterUnitEvent(21500, 4, "Morgroron_OnDied")

function MutantHorror_OnEnterCombat(pUnit,Event)
	pUnit:CastSpell(8599)
	pUnit:RegisterEvent("MutantHorror_MutatedBlood", 45000, 0)
end

function MutantHorror_MutatedBlood(pUnit,Event)
	pUnit:FullCastSpellOnTarget(37950,pUnit:GetClosestPlayer())
end

function MutantHorror_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function MutantHorror_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21305, 1, "MutantHorror_OnEnterCombat")
RegisterUnitEvent(21305, 2, "MutantHorror_OnLeaveCombat")
RegisterUnitEvent(21305, 4, "MutantHorror_OnDied")

function Netharel_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Netharel_DebilitatingStrike", 15000, 0)
	pUnit:RegisterEvent("Netharel_Evasion", 30000, 0)
	pUnit:RegisterEvent("Netharel_ManaBurn", 7000, 0)
	pUnit:RegisterEvent("Netharel_Metamorphosis", 1000, 1)
end

function Netharel_DebilitatingStrike(pUnit,Event)
	pUnit:FullCastSpellOnTarget(39135,pUnit:GetClosestPlayer())
end

function Netharel_Evasion(pUnit,Event)
	pUnit:FullCastSpellOnTarget(37683,pUnit:GetClosestPlayer())
end

function Netharel_ManaBurn(pUnit,Event)
	pUnit:FullCastSpellOnTarget(39262,pUnit:GetClosestPlayer())
end

function Netharel_Metamorphosis(pUnit,Event)
 if pUnit:GetHealthPct() == 10 then
	pUnit:CastSpell(36298)
end
end

function Netharel_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Netharel_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21164, 1, "Netharel_OnEnterCombat")
RegisterUnitEvent(21164, 2, "Netharel_OnLeaveCombat")
RegisterUnitEvent(21164, 4, "Netharel_OnDied")

function NethermineBurster_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("NethermineBurster_Poison", 2000, 0)
end

function NethermineBurster_Poison(pUnit,Event)
	pUnit:FullCastSpellOnTarget(31747,pUnit:GetClosestPlayer())
end

function NethermineBurster_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end


function NethermineBurster_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(23285, 1, "NethermineBurster_OnEnterCombat")
RegisterUnitEvent(23285, 2, "NethermineBurster_OnLeaveCombat")
RegisterUnitEvent(23285, 4, "NethermineBurster_OnDied")

function NethermineFlayer_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("NethermineFlayer_Rend", 30000, 0)
	pUnit:RegisterEvent("NethermineFlayer_ShredArmor", 31000, 0)
end

function NethermineFlayer_Rend(pUnit,Event)
	pUnit:FullCastSpellOnTarget(13443,pUnit:GetClosestPlayer())
end

function NethermineFlayer_ShredArmor(pUnit,Event)
	pUnit:FullCastSpellOnTarget(40770,pUnit:GetClosestPlayer())
end

function NethermineFlayer_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function NethermineFlayer_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(23169, 1, "NethermineFlayer_OnEnterCombat")
RegisterUnitEvent(23169, 2, "NethermineFlayer_OnLeaveCombat")
RegisterUnitEvent(23169, 4, "NethermineFlayer_OnDied")

function NethermineRavager_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("NethermineRavager_Rend", 15000, 0)
	pUnit:RegisterEvent("NethermineRavager_RockShell", 18000, 0)
end

function NethermineRavager_Rend(pUnit,Event)
	pUnit:FullCastSpellOnTarget(13443,pUnit:GetClosestPlayer())
end

function NethermineRavager_RockShell(pUnit,Event)
	pUnit:FullCastSpellOnTarget(33810,pUnit:GetClosestPlayer())
end

function NethermineRavager_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function NethermineRavager_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(23326, 1, "NethermineRavager_OnEnterCombat")
RegisterUnitEvent(23326, 2, "NethermineRavager_OnLeaveCombat")
RegisterUnitEvent(23326, 4, "NethermineRavager_OnDied")

function Netherskate_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Netherskate_DrainMana", 1000, 0)
	pUnit:RegisterEvent("Netherskate_TailSting", 32000, 0)
end

function Netherskate_DrainMana(pUnit,Event)
 if pUnit:GetManaPct() == 92 then
	pUnit:FullCastSpellOnTarget(17008,pUnit:GetRandomPlayer(4))
end
end

function Netherskate_TailSting(pUnit,Event)
	pUnit:FullCastSpellOnTarget(36659,pUnit:GetClosestPlayer())
end

function Netherskate_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Netherskate_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21901, 1, "Netherskate_OnEnterCombat")
RegisterUnitEvent(21901, 2, "Netherskate_OnLeaveCombat")
RegisterUnitEvent(21901, 4, "Netherskate_OnDied")

function NetherwingRay_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("NetherwingRay_DrainMana", 1000, 0)
	pUnit:RegisterEvent("NetherwingRay_TailSting", 32000, 0)
end

function NetherwingRay_DrainMana(pUnit,Event)
 if pUnit:GetManaPct() == 92 then
	pUnit:FullCastSpellOnTarget(17008,pUnit:GetRandomPlayer(4))
end
end

function NetherwingRay_TailSting(pUnit,Event)
	pUnit:FullCastSpellOnTarget(36659,pUnit:GetClosestPlayer())
end

function NetherwingRay_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function NetherwingRay_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(23501, 1, "NetherwingRay_OnEnterCombat")
RegisterUnitEvent(23501, 2, "NetherwingRay_OnLeaveCombsat")
RegisterUnitEvent(23501, 4, "NetherwingRay_OnDied")

function OrkaosTheInsane_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("OrkaosTheInsane_Cleave", 14000, 0)
	pUnit:RegisterEvent("OrkaosTheInsane_MortalStrike", 16000, 0)
	pUnit:RegisterEvent("OrkaosTheInsane_Uppercut", 8000, 0)
end

function OrkaosTheInsane_Cleave(pUnit,Event)
	pUnit:FullCastSpellOnTarget(15496,pUnit:GetClosestPlayer())
end

function OrkaosTheInsane_MortalStrike(pUnit,Event)
	pUnit:FullCastSpellOnTarget(17547,pUnit:GetClosestPlayer())
end

function OrkaosTheInsane_Uppercut(pUnit,Event)
	pUnit:FullCastSpellOnTarget(10966,pUnit:GetClosestPlayer())
end

function OrkaosTheInsane_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function OrkaosTheInsane_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(23168, 1, "OrkaosTheInsane_OnEnterCombat")
RegisterUnitEvent(23168, 2, "OrkaosTheInsane_OnLeaveCombsat")
RegisterUnitEvent(23168, 4, "OrkaosTheInsane_OnDied")

function OronokTornheart_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("OronokTornheart_ChainLightning", 1200, 0)
	pUnit:RegisterEvent("OronokTornheart_FrostShock", 16000, 0)
	pUnit:RegisterEvent("OronokTornheart_HealingWave", 5000, 0)
end

function OronokTornheart_ChainLightning(pUnit,Event)
	pUnit:FullCastSpellOnTarget(16006,pUnit:GetClosestPlayer())
end

function OronokTornheart_FrostShock(pUnit,Event)
	pUnit:FullCastSpellOnTarget(12548,pUnit:GetClosestPlayer())
end

function OronokTornheart_HealingWave(pUnit,Event)
	pUnit:FullCastSpellOnTarget(12491,pUnit:GetRandomFriend())
end

function OronokTornheart_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function OronokTornheart_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21183, 1, "OronokTornheart_OnEnterCombat")
RegisterUnitEvent(21183, 2, "OronokTornheart_OnLeaveCombsat")
RegisterUnitEvent(21183, 4, "OronokTornheart_OnDied")

function OronuTheElder_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("OronuTheElder_GroundingTotem", 45000, 0)
	pUnit:RegisterEvent("OronuTheElder_HealingWave", 11000, 0)
	pUnit:RegisterEvent("OronuTheElder_LightningShield", 30000, 0)
end

function OronuTheElder_GroundingTotem(pUnit,Event)
	pUnit:CastSpell(34079)
end

function OronuTheElder_HealingWave(pUnit,Event)
	pUnit:FullCastSpellOnTarget(11986,pUnit:GetRandomFriend())
end

function OronuTheElder_LightningShield(pUnit,Event)
	pUnit:CastSpell(12550)
end

function OronuTheElder_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function OronuTheElder_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21663, 1, "OronuTheElder_OnEnterCombat")
RegisterUnitEvent(21663, 2, "OronuTheElder_OnLeaveCombsat")
RegisterUnitEvent(21663, 4, "OronuTheElder_OnDied")

function OvermineFlayer_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("OvermineFlayer_Rend", 30000, 0)
	pUnit:RegisterEvent("OvermineFlayer_RockShell", 18000, 0)
end

function OvermineFlayer_Rend(pUnit,Event)
	pUnit:FullCastSpellOnTarget(13443,pUnit:GetClosestPlayer())
end

function OvermineFlayer_RockShell(pUnit,Event)
	pUnit:CastSpell(33810)
end

function OronuTheElder_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function OronuTheElder_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(23264, 1, "OvermineFlayer_OnEnterCombat")
RegisterUnitEvent(23264, 2, "OvermineFlayer_OnLeaveCombsat")
RegisterUnitEvent(23264, 4, "OvermineFlayer_OnDied")

function OverseerRipsaw_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("OverseerRipsaw_GushingWound", 25000, 0)
	pUnit:RegisterEvent("OverseerRipsaw_SawBlade", 2500, 0)
end

function OverseerRipsaw_GushingWound(pUnit,Event)
	pUnit:FullCastSpellOnTarget(35321,pUnit:GetClosestPlayer())
end

function OverseerRipsaw_SawBlade(pUnit,Event)
	pUnit:FullCastSpellOnTarget(32735,pUnit:GetClosestPlayer())
end

function OverseerRipsaw_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function OverseerRipsaw_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21499, 1, "OverseerRipsaw_OnEnterCombat")
RegisterUnitEvent(21499, 2, "OverseerRipsaw_OnLeaveCombsat")
RegisterUnitEvent(21499, 4, "OverseerRipsaw_OnDied")

function PainmistressGabrissa_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("PainmistressGabrissa_CurseOfPain", 120000, 0)
end

function PainmistressGabrissa_CurseOfPain(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38048,pUnit:GetClosestPlayer())
 if pUnit:GetHealthPct() == 50 then 
	pUnit:RemoveEvents()
end
end

function PainmistressGabrissa_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function PainmistressGabrissa_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21309, 1, "PainmistressGabrissa_OnEnterCombat")
RegisterUnitEvent(21309, 2, "PainmistressGabrissa_OnLeaveCombsat")
RegisterUnitEvent(21309, 4, "PainmistressGabrissa_OnDied")

function ProphetessCavrylin_OnEnterCombat(pUnit,Event)
	pUnit:FullCastSpellOnTarget(36656,pUnit:GetClosestPlayer())
	pUnit:CastSpell(37997)
	pUnit:RegisterEvent("ProphetessCavrylin_MeltFlesh", 15000, 0)
end

function ProphetessCavrylin_MeltFlesh(pUnit,Event)
	pUnit:FullCastSpellOnTarget(37629,pUnit:GetClosestPlayer())
end

function ProphetessCavrylin_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function ProphetessCavrylin_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(20683, 1, "ProphetessCavrylin_OnEnterCombat")
RegisterUnitEvent(20683, 2, "ProphetessCavrylin_OnLeaveCombsat")
RegisterUnitEvent(20683, 4, "ProphetessCavrylin_OnDied")

function RavenousFlayerMatriarch_OnEnterCombat(pUnit,Event)
	pUnit:CastSpell(36464)
	pUnit:RegisterEvent("RavenousFlayerMatriarch_GushingWound", 25000, 0)
end

function RavenousFlayerMatriarch_GushingWound(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38363,pUnit:GetClosestPlayer())
end

function RavenousFlayerMatriarch_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function RavenousFlayerMatriarch_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21205, 1, "RavenousFlayerMatriarch_OnEnterCombat")
RegisterUnitEvent(21205, 2, "RavenousFlayerMatriarch_OnLeaveCombsat")
RegisterUnitEvent(21205, 4, "RavenousFlayerMatriarch_OnDied")

function RemnantofCorruption_OnSpawn(pUnit,Event)
	pUnit:CastSpell(39169)
end

RegisterUnitEvent(22439, 6, "RemnantofCorruption_OnSpawn")

function RemnantofGreed_OnSpawn(pUnit,Event)
	pUnit:CastSpell(39168)
end

RegisterUnitEvent(22438, 6, "RemnantofGreed_OnSpawn")

function RemnantofHate_OnSpawn(pUnit,Event)
	pUnit:CastSpell(38696)
end

RegisterUnitEvent(22094, 6, "RemnantofHate_OnSpawn")

function RemnantofMalice_OnSpawn(pUnit,Event)
	pUnit:CastSpell(38695)
end

RegisterUnitEvent(22437, 6, "RemnantofMalice_OnSpawn")

function RocknailFlayer_OnEnterCombat(pUnit,Event)
	pUnit:RegisterUnitEvent("RocknailFlayer_FlayedFlesh", 30000, 0)
end

function RocknailFlayer_FlayedFlesh(pUnit,Event)
	pUnit:FullCastSpellOnTarget(37937,pUnit:GetClosestPlayer())
end

function RocknailFlayer_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function RocknailFlayer_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21477, 1, "RocknailFlayer_OnEnterCombat")
RegisterUnitEvent(21477, 2, "RocknailFlayer_OnLeaveCombat")
RegisterUnitEvent(21477, 4, "RocknailFlayer_OnDied")

function RocknailRipper_OnEnterCombat(pUnit,Event)
	pUnit:RegisterUnitEvent("RocknailRipper_Rip", 11000, 0)
end

function RocknailRipper_Rip(pUnit,Event)
	pUnit:FullCastSpellOnTarget(37937,pUnit:GetClosestPlayer())
end

function RocknailRipper_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function RocknailRipper_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21478, 1, "RocknailRipper_OnEnterCombat")
RegisterUnitEvent(21478, 2, "RocknailRipper_OnLeaveCombat")
RegisterUnitEvent(21478, 4, "RocknailRipper_OnDied")

function RuulTheDarkener_OnEnterCombat(pUnit,Event)
	pUnit:RegisterUnitEvent("RuulTheDarkener_Cleave", 15000, 0)
	pUnit:RegisterUnitEvent("RuulTheDarkener_Spellbreaker", 18000, 0)
	local Choice=math.random(1,2)
	if Choice==1 then
		pUnit:SendChatMessage(14, 0, "Your world is at an end.")
	elseif Choice==2 then
		pUnit:SendChatMessage(14, 0, "The skies will darken and all will go quiet. Only then will you know the sweet serenity of death.")
end
end

function RuulTheDarkener_Cleave(pUnit,Event)
	pUnit:FullCastSpellOnTarget(15284,pUnit:GetClosestPlayer())
end

function RuulTheDarkener_Spellbreaker(pUnit,Event)
	pUnit:FullCastSpellOnTarget(35871,pUnit:GetClosestPlayer())
end

function RuulTheDarkener_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function RuulTheDarkener_OnDied(pUnit,Event)
	pUnit:SpawnCreature(22106)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21315, 1, "RuulTheDarkener_OnEnterCombat")
RegisterUnitEvent(21315, 2, "RuulTheDarkener_OnLeaveCombat")
RegisterUnitEvent(21315, 4, "RuulTheDarkener_OnDied")

function RuulsNetherdrake_OnEnterCombat(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38344,pUnit:GetClosestPlayer())
	pUnit:FullCastSpellOnTarget(36513,pUnit:GetClosestPlayer())
	pUnit:RegisterEvent("RuulsNetherdrake_ArcaneBlast", 000, 0)
end

function RuulsNetherdrake_ArcaneBlast(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38344,pUnit:GetClosestPlayer())
end

function RuulsNetherdrake_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

function RuulsNetherdrake_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(22106, 1, "RuulsNetherdrake_OnEnterCombat")
RegisterUnitEvent(22106, 2, "RuulsNetherdrake_OnLeaveCombat")
RegisterUnitEvent(22106, 4, "RuulsNetherdrake_OnDied")

function SanctumDefender_OnEnterCombat(pUnit,Event)
	pUnit:FullCastSpellOnTarget(41440,pUnit:GetClosestPlayer())
end

RegisterUnitEvent(23435, 1, "SanctumDefender_OnEnterCombat")

function ScorchshellPincer_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("ScorchshellPincer_BurningPoison", 000, 0)
end

function ScorchshellPincer_BurningPoison(pUnit,Event)
	pUnit:FullCastSpellOnTarget(15284,pUnit:GetClosestPlayer())
end

function ScorchshellPincer_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function ScorchshellPincer_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21864, 1, "ScorchshellPincer_OnEnterCombat")
RegisterUnitEvent(21864, 2, "ScorchshellPincer_OnLeaveCombat")
RegisterUnitEvent(21864, 4, "ScorchshellPincer_OnDied")

function ScornedSpiritofAir_OnSpawn(pUnit,Event)
	pUnit:CastSpell(36206)
end

RegisterUnitEvent(21132, 6, "ScornedSpiritofAir_OnSpawn")

function ScornedSpiritofEarth_OnSpawn(pUnit,Event)
	pUnit:CastSpell(36206)
end

RegisterUnitEvent(21129, 6, "ScornedSpiritofEarth_OnSpawn")

function ScornedSpiritofFire_OnSpawn(pUnit,Event)
	pUnit:CastSpell(36206)
end

RegisterUnitEvent(21130, 6, "ScornedSpiritofFire_OnSpawn")

function ScornedSpiritofWater_OnSpawn(pUnit,Event)
	pUnit:CastSpell(36206)
end

RegisterUnitEvent(21131, 6, "ScornedSpiritofWater_OnSpawn")

function ScryerCavalier_OnEnterCombat(pUnit,Event)
	pUnit:CastSpell(30931)
	pUnit:registerEvent("ScryerCavalier_Spellbreaker", 24000, 0)
end

function ScryerCavalier_Spellbreaker(pUnit,Event)
	pUnit:FullCastSpellOnTarget(35871,pUnit:GetClosestPlayer())
end

function ScryerCavalier_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function ScryerCavalier_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(22967, 1, "ScryerCavalier_OnEnterCombat")
RegisterUnitEvent(22967, 2, "ScryerCavalier_OnLeaveCombat")
RegisterUnitEvent(22967, 4, "ScryerCavalier_OnDied")

function ScryerGuardian_OnEnterCombat(pUnit,Event)
	pUnit:registerEvent("ScryerGuardian_Fireball", 7000, 0)
	pUnit:registerEvent("ScryerGuardian_Scorch", 5000, 0)
	pUnit:registerEvent("ScryerGuardian_Slow", 40000, 0)
end

function ScryerGuardian_Fireball(pUnit,Event)
	pUnit:FullCastSpellOnTarget(15228,pUnit:GetClosestPlayer())
end

function ScryerGuardian_Scorch(pUnit,Event)
	pUnit:FullCastSpellOnTarget(17195,pUnit:GetClosestPlayer())
end

function ScryerGuardian_Slow(pUnit,Event)
	pUnit:FullCastSpellOnTarget(11436,pUnit:GetClosestPlayer())
end

function ScryerGuardian_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function ScryerGuardian_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(19504, 1, "ScryerGuardian_OnEnterCombat")
RegisterUnitEvent(19504, 2, "ScryerGuardian_OnLeaveCombat")
RegisterUnitEvent(19504, 4, "ScryerGuardian_OnDied")

function SeasonedMagister_OnEnterCombat(pUnit,Event)
	pUnit:registerEvent("SeasonedMagister_Fireball", 3000, 0)
end

function SeasonedMagister_Fireball(pUnit,Event)
	pUnit:FullCastSpellOnTarget(15228,pUnit:GetClosestPlayer())
end

function SeasonedMagister_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function SeasonedMagister_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(22863, 1, "SeasonedMagister_OnEnterCombat")
RegisterUnitEvent(22863, 2, "SeasonedMagister_OnLeaveCombat")
RegisterUnitEvent(22863, 4, "SeasonedMagister_OnDied")

function ShadowCouncilWarlock_OnEnterCombat(pUnit,Event)
	pUnit:SetCombatMeleeCapable(1)
	pUnit:registerEvent("ShadowCouncilWarlock_DrainLife", 24000, 0)
	pUnit:registerEvent("ShadowCouncilWarlock_ShadowBolt", 2500, 0)
end

function ShadowCouncilWarlock_DrainLife(pUnit,Event)
	pUnit:FullCastSpellOnTarget(37992,pUnit:GetClosestPlayer())
end

function ShadowCouncilWarlock_ShadowBolt(pUnit,Event)
	pUnit:FullCastSpellOnTarget(9613,pUnit:GetClosestPlayer())
end

function ShadowCouncilWarlock_OnLeaveCombat(pUnit,Event)
	pUnit:SetCombatMeleeCapable(0)
	pUnit:RemoveEvents()
end

function ShadowCouncilWarlock_OnDied(pUnit,Event)
	pUnit:SetCombatMeleeCapable(0)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21302, 1, "ShadowCouncilWarlock_OnEnterCombat")
RegisterUnitEvent(21302, 2, "ShadowCouncilWarlock_OnLeaveCombat")
RegisterUnitEvent(21302, 4, "ShadowCouncilWarlock_OnDied")

function ShadowhoofAssassin_OnEnterCombat(pUnit,Event)
	pUnit:registerEvent("ShadowhoofAssassin_DebilitatingStrike", 15000, 0)
	pUnit:registerEvent("ShadowhoofAssassin_SinisterStrike", 6000, 0)
end

function ShadowhoofAssassin_DebilitatingStrike(pUnit,Event)
	pUnit:FullCastSpellOnTarget(37577,pUnit:GetClosestPlayer())
end

function ShadowhoofAssassin_SinisterStrike(pUnit,Event)
	pUnit:FullCastSpellOnTarget(14873,pUnit:GetClosestPlayer())
end

function ShadowhoofAssassin_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function ShadowhoofAssassin_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(22858, 1, "ShadowhoofAssassin_OnEnterCombat")
RegisterUnitEvent(22858, 2, "ShadowhoofAssassin_OnLeaveCombat")
RegisterUnitEvent(22858, 4, "ShadowhoofAssassin_OnDied")

function ShadowhoofSummoner_OnEnterCombat(pUnit,Event)
	pUnit:SetCombatMeleeCapable(1)
	pUnit:registerEvent("ShadowhoofSummoner_CurseOfTongues", 30000, 0)
	pUnit:registerEvent("ShadowhoofSummoner_ShadowBolt", 3000, 0)
end

function ShadowhoofSummoner_CurseOfTongues(pUnit,Event)
	pUnit:FullCastSpellOnTarget(13338,pUnit:GetClosestPlayer())
end

function ShadowhoofSummoner_ShadowBolt(pUnit,Event)
	pUnit:FullCastSpellOnTarget(9613,pUnit:GetClosestPlayer())
end

function ShadowhoofSummoner_OnLeaveCombat(pUnit,Event)
	pUnit:SetCombatMeleeCapable(0)
	pUnit:RemoveEvents()
end

function ShadowhoofSummoner_OnDied(pUnit,Event)
	pUnit:SetCombatMeleeCapable(0)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(22859, 1, "ShadowhoofSummoner_OnEnterCombat")
RegisterUnitEvent(22859, 2, "ShadowhoofSummoner_OnLeaveCombat")
RegisterUnitEvent(22859, 4, "ShadowhoofSummoner_OnDied")

function ShadowlordDeathwail_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("ShadowlordDeathwail_DeathCoil", 18000, 0)
	pUnit:RegisterEvent("ShadowlordDeathwail_Fear", 24000, 0)
	pUnit:RegisterEvent("ShadowlordDeathwail_FelFireball", 11000, 0)
	pUnit:RegisterEvent("ShadowlordDeathwail_ShadowBolt", 5000, 0)
	pUnit:RegisterEvent("ShadowlordDeathwail_ShadowBoltVolley", 7000, 0)
end

function ShadowlordDeathwail_DeathCoil(pUnit,Event)
	pUnit:FullCastSpellOnTarget(32709,pUnit:GetClosestPlayer())
end

function ShadowlordDeathwail_Fear(pUnit,Event)
	pUnit:FullCastSpellOnTarget(27641,pUnit:GetClosestPlayer())
end

function ShadowlordDeathwail_FelFireball(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38312,pUnit:GetClosestPlayer())
end

function ShadowlordDeathwail_ShadowBolt(pUnit,Event)
	pUnit:FullCastSpellOnTarget(12471,pUnit:GetClosestPlayer())
end

function ShadowlordDeathwail_ShadowBoltVolley(pUnit,Event)
	pUnit:FullCastSpellOnTarget(15245,pUnit:GetClosestPlayer())
end

function ShadowlordDeathwail_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function ShadowlordDeathwail_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(22006, 1, "ShadowlordDeathwail_OnEnterCombat")
RegisterUnitEvent(22006, 2, "ShadowlordDeathwail_OnLeaveCombat")
RegisterUnitEvent(22006, 4, "ShadowlordDeathwail_OnDied")

function ShadowmoonChosen_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("ShadowmoonChosen_Uppercut", 5000, 0)
	pUnit:RegisterEvent("ShadowmoonChosen_Whirlwind", 11000, 0)
end

function ShadowmoonChosen_Uppercut(pUnit,Event)
	pUnit:FullCastSpellOnTarget(10966,pUnit:GetClosestPlayer())
end

function ShadowmoonChosen_Whirlwind(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38619,pUnit:GetClosestPlayer())
	pUnit:FullCastSpellOnTarget(38618,pUnit:GetClosestPlayer())
end

function ShadowmoonChosen_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function ShadowmoonChosen_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(22084, 1, "ShadowmoonChosen_OnEnterCombat")
RegisterUnitEvent(22084, 2, "ShadowmoonChosen_OnLeaveCombat")
RegisterUnitEvent(22084, 4, "ShadowmoonChosen_OnDied")

function ShadowmoonDarkweaver_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("ShadowmoonDarkweaver_Immolate", 4000, 0)
	pUnit:RegisterEvent("ShadowmoonDarkweaver_NetherInfusion", 35000, 3)
	pUnit:RegisterEvent("ShadowmoonDarkweaver_ShadowBolt", 2500, 0)
	pUnit:RegisterEvent("ShadowmoonDarkweaver_Shadowfury", 9000, 0)
end

function ShadowmoonDarkweaver_Immolate(pUnit,Event)
	pUnit:FullCastSpellOnTarget(11962,pUnit:GetClosestPlayer())
end

function ShadowmoonDarkweaver_NetherInfusion(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38446,pUnit:GetClosestPlayer())
end

function ShadowmoonDarkweaver_ShadowBolt(pUnit,Event)
	pUnit:FullCastSpellOnTarget(9613,pUnit:GetClosestPlayer())
end

function ShadowmoonDarkweaver_Shadowfury(pUnit,Event)
	pUnit:FullCastSpellOnTarget(35373,pUnit:GetClosestPlayer())
end

function ShadowmoonDarkweaver_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function ShadowmoonDarkweaver_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

function ShadowmoonDarkweaver_OnSpawn(pUnit,Event)
	pUnit:CastSpell(38442)
end

RegisterUnitEvent(22081, 1, "ShadowmoonDarkweaver_OnEnterCombat")
RegisterUnitEvent(22081, 2, "ShadowmoonDarkweaver_OnLeaveCombat")
RegisterUnitEvent(22081, 4, "ShadowmoonDarkweaver_OnDied")
RegisterUnitEvent(22081, 6, "ShadowmoonDarkweaver_OnSpawn")

function ShadowmoonRetainer_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("ShadowmoonRetainer_Shoot", 4000, 0)
end

function ShadowmoonRetainer_Shoot(pUnit,Event)
	pUnit:FullCastSpellOnTarget(15547,pUnit:GetRandomPlayer(3))
end

function ShadowmoonRetainer_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function ShadowmoonRetainer_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(22102, 1, "ShadowmoonRetainer_OnEnterCombat")
RegisterUnitEvent(22102, 2, "ShadowmoonRetainer_OnLeaveCombat")
RegisterUnitEvent(22102, 4, "ShadowmoonRetainer_OnDied")

function ShadowmoonSlayer_OnEnterCombat(pUnit,Event)
	pUnit:FullCastSpellOnTarget(3019,pUnit:GetClosestPlayer())
	pUnit:RegisterEvent("ShadowmoonSlayer_DebilitatingStrike", 20000, 0)
end

function ShadowmoonSlayer_DebilitatingStrike(pUnit,Event)
	pUnit:FullCastSpellOnTarget(37577,pUnit:GetClosestPlayer())
end

function ShadowmoonSlayer_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function ShadowmoonSlayer_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(22082, 1, "ShadowmoonSlayer_OnEnterCombat")
RegisterUnitEvent(22082, 2, "ShadowmoonSlayer_OnLeaveCombat")
RegisterUnitEvent(22082, 4, "ShadowmoonSlayer_OnDied")

function ShadowswornDrakonid_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("ShadowswornDrakonid_Cleave", 11000, 0)
	pUnit:RegisterEvent("ShadowswornDrakonid_MortalStrike", 15000, 0)
	pUnit:RegisterEvent("ShadowswornDrakonid_SunderArmor", 30000, 0)
end

function ShadowswornDrakonid_Cleave(pUnit,Event)
	pUnit:FullCastSpellOnTarget(15496,pUnit:GetClosestPlayer())
end

function ShadowswornDrakonid_MortalStrike(pUnit,Event)
	pUnit:FullCastSpellOnTarget(17547,pUnit:GetClosestPlayer())
end

function ShadowswornDrakonid_SunderArmor(pUnit,Event)
	pUnit:FullCastSpellOnTarget(16145,pUnit:GetClosestPlayer())
end

function ShadowswornDrakonid_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function ShadowswornDrakonid_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(22072, 1, "ShadowswornDrakonid_OnEnterCombat")
RegisterUnitEvent(22072, 2, "ShadowswornDrakonid_OnLeaveCombat")
RegisterUnitEvent(22072, 4, "ShadowswornDrakonid_OnDied")

function ShadowwingOwl_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("ShadowwingOwl_EagleClaw", 30000, 0)
end

function ShadowwingOwl_EagleClaw(pUnit,Event)
	pUnit:FullCastSpellOnTarget(15496,pUnit:GetClosestPlayer())
end

function ShadowwingOwl_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function ShadowwingOwl_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(22265, 1, "ShadowwingOwl_OnEnterCombat")
RegisterUnitEvent(22265, 2, "ShadowwingOwl_OnLeaveCombat")
RegisterUnitEvent(22265, 4, "ShadowwingOwl_OnDied")

function SkethylOwl_OnEnterCombat(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38254,pUnit:GetClosestPlayer())
	pUnit:RegisterEvent("SkethylOwl_TerrifyingScreech", 14000, 0)
end

function SkethylOwl_TerrifyingScreech(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38021,pUnit:GetClosestPlayer())
end

function SkethylOwl_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function SkethylOwl_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21450, 1, "SkethylOwl_OnEnterCombat")
RegisterUnitEvent(21450, 2, "SkethylOwl_OnLeaveCombat")
RegisterUnitEvent(21450, 4, "SkethylOwl_OnDied")

function SmithGorlunk_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("SmithGorlunk_Rend", 15000, 0)
	pUnit:RegisterEvent("SmithGorlunk_SunderArmor", 12000, 0)
end

function SmithGorlunk_Rend(pUnit,Event)
	pUnit:FullCastSpellOnTarget(13445,pUnit:GetClosestPlayer())
end

function SmithGorlunk_SunderArmor(pUnit,Event)
	pUnit:FullCastSpellOnTarget(13444,pUnit:GetClosestPlayer())
end

function SmithGorlunk_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function SmithGorlunk_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(22037, 1, "SmithGorlunk_OnEnterCombat")
RegisterUnitEvent(22037, 2, "SmithGorlunk_OnLeaveCombat")
RegisterUnitEvent(22037, 4, "SmithGorlunk_OnDied")

function SonOfCorok_OnEnterCombat(pUnit,Event)
	pUnit:FullCastSpellOnTarget(12612,pUnit:GetClosestPlayer())
	pUnit:RegisterEvent("SonOfCorok_Stomp", 15000, 0)
end

function SonOfCorok_Stomp(pUnit,Event)
	pUnit:FullCastSpellOnTarget(12612,pUnit:GetClosestPlayer())
end

function SonOfCorok_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function SonOfCorok_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(19824, 1, "SonOfCorok_OnEnterCombat")
RegisterUnitEvent(19824, 2, "SonOfCorok_OnLeaveCombat")
RegisterUnitEvent(19824, 4, "SonOfCorok_OnDied")

function SpawnOfUvuros_OnEnterCombat(pUnit,Event)
	pUnit:FullCastSpellOnTarget(36405,pUnit:GetClosestPlayer())
	pUnit:RegisterEvent("SpawnOfUvuros_Bite", 4000, 0)
	pUnit:RegisterEvent("SpawnOfUvuros_DoubleBreath", 15000, 0)
	pUnit:RegisterEvent("SpawnOfUvuros_Growl", 1000, 1)
	pUnit:RegisterEvent("SpawnOfUvuros_LavaBreath", 15000, 0)
	pUnit:RegisterEvent("SpawnOfUvuros_Stomp", 9000, 0)
end

function SpawnOfUvuros_Bite(pUnit,Event)
	pUnit:FullCastSpellOnTarget(27050,pUnit:GetClosestPlayer())
end

function SpawnOfUvuros_DoubleBreath(pUnit,Event)
	pUnit:FullCastSpellOnTarget(36406,pUnit:GetClosestPlayer())
end

function SpawnOfUvuros_Growl(pUnit,Event)
	pUnit:FullCastSpellOnTarget(14921,pUnit:GetClosestPlayer())
end

function SpawnOfUvuros_LavaBreath(pUnit,Event)
	pUnit:FullCastSpellOnTarget(58610,pUnit:GetClosestPlayer())
end

function SpawnOfUvuros_Stomp(pUnit,Event)
	pUnit:FullCastSpellOnTarget(36405,pUnit:GetClosestPlayer())
end

function SpawnOfUvuros_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function SpawnOfUvuros_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21108, 1, "SpawnOfUvuros_OnEnterCombat")
RegisterUnitEvent(21108, 2, "SpawnOfUvuros_OnLeaveCombat")
RegisterUnitEvent(21108, 4, "SpawnOfUvuros_OnDied")

function SpellboundTerrorguard_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("SpellboundTerrorguard_FelFlames", 8000, 0)
	pUnit:RegisterEvent("SpellboundTerrorguard_Hamstring", 20000, 3)
end

function SpellboundTerrorguard_FelFlames(pUnit,Event)
	pUnit:FullCastSpellOnTarget(37488,pUnit:GetClosestPlayer())
end

function SpellboundTerrorguard_Hamstring(pUnit,Event)
	pUnit:FullCastSpellOnTarget(31553,pUnit:GetClosestPlayer())
end

function SpellboundTerrorguard_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function SpellboundTerrorguard_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21908, 1, "SpellboundTerrorguard_OnEnterCombat")
RegisterUnitEvent(21908, 2, "SpellboundTerrorguard_OnLeaveCombat")
RegisterUnitEvent(21908, 4, "SpellboundTerrorguard_OnDied")

function SummonerSkartax_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("SummonerSkartax_Incinerate", 5000, 0)
	pUnit:RegisterEvent("SummonerSkartax_ShadowBolt", 9000, 0)
end

function SummonerSkartax_Incinerate(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38401,pUnit:GetClosestPlayer())
end

function SummonerSkartax_ShadowBolt(pUnit,Event)
	pUnit:FullCastSpellOnTarget(12471,pUnit:GetClosestPlayer())
end

function SummonerSkartax_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function SummonerSkartax_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21207, 1, "SummonerSkartax_OnEnterCombat")
RegisterUnitEvent(21207, 2, "SummonerSkartax_OnLeaveCombat")
RegisterUnitEvent(21207, 4, "SummonerSkartax_OnDied")

function SunfuryBloodLord_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("SunfuryBloodLord_DebilitatingStrike", 15000, 0)
	pUnit:RegisterEvent("SunfuryBloodLord_TorrentOfFlames", 5000, 0)
end

function SunfuryBloodLord_DebilitatingStrike(pUnit,Event)
	pUnit:FullCastSpellOnTarget(37577,pUnit:GetClosestPlayer())
end

function SunfuryBloodLord_TorrentOfFlames(pUnit,Event)
	pUnit:FullCastSpellOnTarget(36104,pUnit:GetClosestPlayer())
end

function SunfuryBloodLord_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function SunfuryBloodLord_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21743, 1, "SunfuryBloodLord_OnEnterCombat")
RegisterUnitEvent(21743, 2, "SunfuryBloodLord_OnLeaveCombat")
RegisterUnitEvent(21743, 4, "SunfuryBloodLord_OnDied")

function SunfuryEradicator_OnEnterCombat(pUnit,Event)
	pUnit:CastSpell(38008)
	pUnit:RegisterEvent("SunfuryEradicator_FlashHeal", 15000, 0)
end

function SunfuryEradicator_FlashHeal(pUnit,Event)
	pUnit:FullCastSpellOnTarget(17137,pUnit:GetRandomFriend())
end

function SunfuryEradicator_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function SunfuryEradicator_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21742, 1, "SunfuryEradicator_OnEnterCombat")
RegisterUnitEvent(21742, 2, "SunfuryEradicator_OnLeaveCombat")
RegisterUnitEvent(21742, 4, "SunfuryEradicator_OnDied")

function SunfurySummoner_OnEnterCombat(pUnit,Event)
	pUnit:CastSpell(13901)
	pUnit:RegisterEvent("SunfurySummoner_ArcaneBolt", 5000, 0)
	pUnit:RegisterEvent("SunfurySummoner_Blink", 1000, 1)
end

function SunfurySummoner_ArcaneBolt(pUnit,Event)
	pUnit:FullCastSpellOnTarget(13901,pUnit:GetRandomFriend())
end

function SunfurySummoner_Blink(pUnit,Event)
 if pUnit:GetHealthPct() == 4 then
	pUnit:CastSpell(36994)
end
end

function SunfurySummoner_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function SunfurySummoner_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21505, 1, "SunfurySummoner_OnEnterCombat")
RegisterUnitEvent(21505, 2, "SunfurySummoner_OnLeaveCombat")
RegisterUnitEvent(21505, 4, "SunfurySummoner_OnDied")

function SunfuryWarlock_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("SunfuryWarlock_Incinerate", 3000, 0)
	pUnit:RegisterEvent("SunfuryWarlock_ShadowBolt", 9000, 0)
end

function SunfuryWarlock_Incinerate(pUnit,Event)
	pUnit:FullCastSpellOnTarget(32707,pUnit:GetClosestPlayer())
end

function SunfuryWarlock_ShadowBolt(pUnit,Event)
	pUnit:FullCastSpellOnTarget(9613,pUnit:GetClosestPlayer())
end

function SunfuryWarlock_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function SunfuryWarlock_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21503, 1, "SunfuryWarlock_OnEnterCombat")
RegisterUnitEvent(21503, 2, "SunfuryWarlock_OnLeaveCombat")
RegisterUnitEvent(21503, 4, "SunfuryWarlock_OnDied")

function Terrormaster_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Terrormaster_Cleave", 9000, 0)
	pUnit:RegisterEvent("Terrormaster_Fear", 24000, 0)
end

function Terrormaster_Cleave(pUnit,Event)
	pUnit:FullCastSpellOnTarget(15496,pUnit:GetClosestPlayer())
end

function Terrormaster_Fear(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38154,pUnit:GetClosestPlayer())
end

function Terrormaster_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Terrormaster_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21314, 1, "Terrormaster_OnEnterCombat")
RegisterUnitEvent(21314, 2, "Terrormaster_OnLeaveCombat")
RegisterUnitEvent(21314, 4, "Terrormaster_OnDied")

function Theras_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Terrormaster_ManaBurn", 20000, 0)
	pUnit:RegisterEvent("Terrormaster_Metamorphosis", 1000, 1)
	pUnit:RegisterEvent("Terrormaster_Spellbreaker", 12000, 0)
end

function Theras_ManaBurn(pUnit,Event)
	pUnit:FullCastSpellOnTarget(39262,pUnit:GetClosestPlayer())
end

function Theras_Metamorphosis(pUnit,Event)
 if pUnit:GetHealthPct() == 49 then
	pUnit:CastSpell(36298)
end
end

function Theras_Spellbreaker(pUnit,Event)
	pUnit:FullCastSpellOnTarget(35871,pUnit:GetClosestPlayer())
end

function Theras_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Theras_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21168, 1, "Theras_OnEnterCombat")
RegisterUnitEvent(21168, 2, "Theras_OnLeaveCombat")
RegisterUnitEvent(21168, 4, "Theras_OnDied")

function TorlothTheMagnificent_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("TorlothTheMagnificent_Cleave", 10000, 0)
	pUnit:RegisterEvent("TorlothTheMagnificent_Shadowfury", 9000, 0)
	pUnit:RegisterEvent("TorlothTheMagnificent_SpellReflection", 12000, 0)
end

function TorlothTheMagnificent_Cleave(pUnit,Event)
	pUnit:FullCastSpellOnTarget(15284,pUnit:GetClosestPlayer())
end

function TorlothTheMagnificent_Shadowfury(pUnit,Event)
	pUnit:FullCastSpellOnTarget(39082,pUnit:GetClosestPlayer())
end

function TorlothTheMagnificent_SpellReflection(pUnit,Event)
	pUnit:CastSpell(33961)
end

function TorlothTheMagnificent_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function TorlothTheMagnificent_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(22076, 1, "TorlothTheMagnificent_OnEnterCombat")
RegisterUnitEvent(22076, 2, "TorlothTheMagnificent_OnLeaveCombat")
RegisterUnitEvent(22076, 4, "TorlothTheMagnificent_OnDied")

function Umberhowl_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Umberhowl_ChillingHowl", 30000, 0)
	pUnit:RegisterEvent("Umberhowl_Snarl", 7000, 0)
end

function Umberhowl_ChillingHowl(pUnit,Event)
	pUnit:FullCastSpellOnTarget(32918,pUnit:GetClosestPlayer())
end

function Umberhowl_Snarl(pUnit,Event)
	pUnit:FullCastSpellOnTarget(32919,pUnit:GetClosestPlayer())
end

function Umberhowl_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Umberhowl_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21255, 1, "Umberhowl_OnEnterCombat")
RegisterUnitEvent(21255, 2, "Umberhowl_OnLeaveCombat")
RegisterUnitEvent(21255, 4, "Umberhowl_OnDied")

function Uvuros_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Uvuros_Bite", 4000, 0)
	pUnit:RegisterEvent("Uvuros_DoubleBreath", 8000, 0)
	pUnit:RegisterEvent("Uvuros_Growl", 20000, 0)
	pUnit:RegisterEvent("Uvuros_LavaBreath", 10000, 0)
	pUnit:RegisterEvent("Uvuros_TerrifyingRoar", 27000, 0)
end

function Uvuros_Bite(pUnit,Event)
	pUnit:FullCastSpellOnTarget(27050,pUnit:GetClosestPlayer())
end

function Uvuros_DoubleBreath(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38361,pUnit:GetClosestPlayer())
end

function Uvuros_Growl(pUnit,Event)
	pUnit:FullCastSpellOnTarget(27047,pUnit:GetClosestPlayer())
end

function Uvuros_LavaBreath(pUnit,Event)
	pUnit:FullCastSpellOnTarget(58610,pUnit:GetClosestPlayer())
end

function Uvuros_TerrifyingRoar(pUnit,Event)
	pUnit:FullCastSpellOnTarget(37939,pUnit:GetClosestPlayer())
end

function Uvuros_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Uvuros_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21102, 1, "Uvuros_OnEnterCombat")
RegisterUnitEvent(21102, 2, "Uvuros_OnLeaveCombat")
RegisterUnitEvent(21102, 4, "Uvuros_OnDied")

function Uylaru_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Uylaru_CorruptedSearingTotem", 20000, 0)
	pUnit:RegisterEvent("Uylaru_FlameShock", 16000, 0)
end

function Uylaru_CorruptedSearingTotem(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38581,pUnit:GetClosestPlayer())
end

function Uylaru_FlameShock(pUnit,Event)
	pUnit:FullCastSpellOnTarget(15039,pUnit:GetClosestPlayer())
end

function Uylaru_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Uylaru_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21710, 1, "Uylaru_OnEnterCombat")
RegisterUnitEvent(21710, 2, "Uylaru_OnLeaveCombat")
RegisterUnitEvent(21710, 4, "Uylaru_OnDied")

function ValzareqTheConqueror_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Uylaru_Shoot", 2000, 3)
end

function ValzareqTheConqueror_Shoot(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38094,pUnit:GetClosestPlayer())
end

function ValzareqTheConqueror_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function ValzareqTheConqueror_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21979, 1, "ValzareqTheConqueror_OnEnterCombat")
RegisterUnitEvent(21979, 2, "ValzareqTheConqueror_OnLeaveCombat")
RegisterUnitEvent(21979, 4, "ValzareqTheConqueror_OnDied")

function Varedis_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Varedis_CurseOfFlames", 120000, 0)
	pUnit:RegisterEvent("Varedis_Evasion", 24000, 0)
	pUnit:RegisterEvent("Varedis_ManaBurn", 9000, 0)
end

function Varedis_CurseOfFlames(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38010,pUnit:GetClosestPlayer())
end

function Varedis_Evasion(pUnit,Event)
	pUnit:CastSpell(37683)
end

function Varedis_ManaBurn(pUnit,Event)
	pUnit:FullCastSpellOnTarget(39262,pUnit:GetClosestPlayer())
end

function Varedis_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Varedis_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21178, 1, "Varedis_OnEnterCombat")
RegisterUnitEvent(21178, 2, "Varedis_OnLeaveCombat")
RegisterUnitEvent(21178, 4, "Varedis_OnDied")

function VhelKur_OnSpawn(pUnit,Event)
	pUnit:CastSpell(36553)
end

RegisterUnitEvent(21801, 6, "VhelKur_OnSpawn")

function VilewingChimaera_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("VilewingChimaera_FroststormBreath", 5000, 0)
	pUnit:RegisterEvent("VilewingChimaera_VenomSpit", 15000, 0)
end

function VilewingChimaera_FroststormBreath(pUnit,Event)
	pUnit:FullCastSpellOnTarget(55491,pUnit:GetClosestPlayer())
end

function VilewingChimaera_VenomSpit(pUnit,Event)
	pUnit:FullCastSpellOnTarget(16552,pUnit:GetClosestPlayer())
end

function VilewingChimaera_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function VilewingChimaera_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21879, 1, "VilewingChimaera_OnEnterCombat")
RegisterUnitEvent(21879, 2, "VilewingChimaera_OnLeaveCombat")
RegisterUnitEvent(21879, 4, "VilewingChimaera_OnDied")

function WarbringerRazuun_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("WarbringerRazuun_FelFireball", 3800, 0)
	pUnit:RegisterEvent("WarbringerRazuun_MindWarp", 45000, 1)
end

function WarbringerRazuun_FelFireball(pUnit,Event)
	pUnit:FullCastSpellOnTarget(35913,pUnit:GetClosestPlayer())
end

function WarbringerRazuun_MindWarp(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38047,pUnit:GetClosestPlayer())
end

function WarbringerRazuun_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function WarbringerRazuun_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21287, 1, "WarbringerRazuun_OnEnterCombat")
RegisterUnitEvent(21287, 2, "WarbringerRazuun_OnLeaveCombat")
RegisterUnitEvent(21287, 4, "WarbringerRazuun_OnDied")

function WildhammerScout_OnEnterCombat(pUnit,Event)
	pUnit:FullCastSpellOnTarget(33808,pUnit:GetClosestPlayer())
	pUnit:RegisterEvent("WildhammerScout_ThrowHammer", 6000, 0)
end

function WildhammerScout_ThrowHammer(pUnit,Event)
	pUnit:FullCastSpellOnTarget(33805,pUnit:GetClosestPlayer())
	pUnit:FullCastSpellOnTarget(33806,pUnit:GetClosestPlayer())
end

function WildhammerScout_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function WildhammerScout_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(19384, 1, "WildhammerScout_OnEnterCombat")
RegisterUnitEvent(19384, 2, "WildhammerScout_OnLeaveCombat")
RegisterUnitEvent(19384, 4, "WildhammerScout_OnDied")

function Wrathstalker_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Wrathstalker_Cleave", 11000, 0)
end

function Wrathstalker_Cleave(pUnit,Event)
	pUnit:FullCastSpellOnTarget(33805,pUnit:GetClosestPlayer())
end

function Wrathstalker_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Wrathstalker_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21249, 1, "Wrathstalker_OnEnterCombat")
RegisterUnitEvent(21249, 2, "Wrathstalker_OnLeaveCombat")
RegisterUnitEvent(21249, 4, "Wrathstalker_OnDied")

function Xiri_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Xiri_BlindingLight", 24000, 0)
end

function Xiri_OnSpawn(pUnit,Event)
	pUnit:RegisterEvent("Xiri_LightOfTheNaaru1", 20000, 0)
	pUnit:RegisterEvent("Xiri_LightOfTheNaaru2", 20000, 0)
	pUnit:RegisterEvent("Xiri_LightOfTheNaaru3", 20000, 0)
end

function Xiri_BlindingLight(pUnit,Event)
	pUnit:FullCastSpellOnTarget(33805)
end

function Xiri_LightOfTheNaaru1(pUnit,Event)
	pUnit:CastSpell(39828)
end

function Xiri_LightOfTheNaaru2(pUnit,Event)
	pUnit:CastSpell(39831)
end

function Xiri_LightOfTheNaaru3(pUnit,Event)
	pUnit:CastSpell(39832)
end

function Xiri_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(18528, 1, "Xiri_OnEnterCombat")
RegisterUnitEvent(18528, 1, "Xiri_OnLeaveCombat")
RegisterUnitEvent(18528, 4, "Xiri_OnDied")
RegisterUnitEvent(18528, 6, "Xiri_OnSpawn")

function Zandras_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Zandras_FelShackles", 11000, 0)
end

function Zandras_FelShackles(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38051,pUnit:GetClosestPlayer())
end

function Zandras_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Zandras_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(21249, 1, "Zandras_OnEnterCombat")
RegisterUnitEvent(21249, 2, "Zandras_OnLeaveCombat")
RegisterUnitEvent(21249, 4, "Zandras_OnDied")

function ZuluhedTheWhacked_OnEnterCombat(pUnit,Event)
	pUnit:CastSpell(38853)
	pUnit:RegisterEvent("ZuluhedTheWhacked_DemonPortal", 35000, 0)
end

function ZuluhedTheWhacked_DemonPortal(pUnit,Event)
	pUnit:CastSpell(38876)
end

function ZuluhedTheWhacked_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function ZuluhedTheWhacked_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(11980, 1, "ZuluhedTheWhacked_OnEnterCombat")
RegisterUnitEvent(11980, 2, "ZuluhedTheWhacked_OnLeaveCombat")
RegisterUnitEvent(11980, 4, "ZuluhedTheWhacked_OnDied")