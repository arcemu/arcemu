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
Made by: Recon
-- ]]
--Highlord Bolvar Fordragon--

function highlordbolvarf_OnCombat(pUnit, Event)
	pUnit:RegisterEvent("cleavee",7000, 0)
	pUnit:RegisterEvent("justice",12000, 0)
	pUnit:RegisterEvent("shield",17500, 0)
end

function cleavee(pUnit, Event)
	print "highlordbolvarf_cleave"
	pUnit:CastSpellOnTarget(20684,pUnit:GetClosestPlayer(0))
end

function justice(pUnit, Event)
	print "highlordbolvarf_justice"
	pUnit:CastSpellOnTarget(20683,pUnit:GetClosestPlayer(4))
end

function shield(pUnit, Event)
	print "highlordbolvarf_shield"
	pUnit:FullCastSpell(15062)
end

function highlordbolvarf_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end


function highlordbolvarf_OnDied(pUnit, event, player)
pUnit:RemoveEvents()
end

RegisterUnitEvent(1748, 1, "highlordbolvarf_OnCombat")
RegisterUnitEvent(1748, 2, "highlordbolvarf_OnLeaveCombat")
RegisterUnitEvent(1748, 4, "highlordbolvarf_OnDied")

--Archbishop Benedictus--
function arcbishbened_OnCombat(pUnit, Event)
	pUnit:RegisterEvent("buff",1, 1)
	pUnit:RegisterEvent("bubi",10500, 0)
	pUnit:RegisterEvent("smite",1500, 0)
	pUnit:RegisterEvent("holynova",14500, 0)
	pUnit:RegisterEvent("asummon",20500, 0)
end

function greetings(pUnit, Event)

end

function buff(pUnit, Event)
	print "arcbishbened_buff"
	pUnit:SendChatMessage(12, 0,"Sacrilege! Defend the cathedral!")	
	pUnit:PlaySoundToSet(58863)
	pUnit:FullCastSpell(2791)
end

function bubi(pUnit, Event)
	print "arcbishbened_bubble"
	pUnit:FullCastSpell(9800)
end

function smite(pUnit, Event)
	print "arcbishbened_smite"
	pUnit:CastSpellOnTarget(25364,pUnit:GetClosestPlayer(0))
end

function holynova(pUnit, Event)
	print "arcbishbened_holynova"
	pUnit:CastSpell(25331)
end

function asummon(pUnit, Event)
	print "arcbishbened_summon"
	local x = pUnit:GetX()
	local y = pUnit:GetY()
	local z = pUnit:GetZ()
	local o = pUnit:GetO()
	pUnit:SpawnCreature(68, x, y, z, o, 84, 60000) -- Summon allyance guard   after 60 sec  autom. despawn	
	pUnit:SpawnCreature(68, x, y, z, o, 84, 60000)
end


function arcbishbened_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end


function arcbishbened_OnDied(pUnit, event, player)
pUnit:RemoveEvents()
end

RegisterUnitEvent(1284, 1, "arcbishbened_OnCombat")
RegisterUnitEvent(1284, 2, "arcbishbened_OnLeaveCombat")
RegisterUnitEvent(1284, 4, "arcbishbened_OnDied")

--Gelbin Mekkatorque--
--little bugy--
function gelbin_OnCombat(pUnit, Event)
	pUnit:RegisterEvent("hali",1, 1)
	pUnit:RegisterEvent("ShrinkRay",7500, 0)
	pUnit:RegisterEvent("bomb",11500, 0)
	pUnit:RegisterEvent("dargongun",15500, 0)
end

function hali(pUnit, Event)
	pUnit:PlaySoundToSet(5897)
	pUnit:SendChatMessage(12, 0,"I won't stand for that!!")
end

function ShrinkRay(pUnit, Event)
	print "gelbin_ShrinkRay"
	pUnit:CastSpellOnTarget(22742,pUnit:GetClosestPlayer(0))
end

function bomb(pUnit, Event)
	print "gelbin_bomb"
	pUnit:CastSpell(9143)
end

function dargongun(pUnit, Event)
	print "gelbin_dragongun"
	pUnit:CastSpellOnTarget(22739,pUnit:GetClosestPlayer(0))

end

function gelbin_OnKilledTarget (pUnit, Event)
	pUnit:PlaySoundToSet(7255)
	pUnit:SendChatMessage(12, 0,"Pick on someone your own size!")
end

function gelbin_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end

function gelbin_OnDied(pUnit, event, player)
pUnit:RemoveEvents()
end

RegisterUnitEvent(7937, 1, "gelbin_OnCombat")
RegisterUnitEvent(7937, 2, "gelbin_OnLeaveCombat")
RegisterUnitEvent(7937, 3, "gelbin_OnKilledTarget")
RegisterUnitEvent(7937, 4, "gelbin_OnDied")

--Magni Bronzebeard--

function magni_OnCombat(pUnit, Event)
	pUnit:PlaySoundToSet(5896)
	pUnit:RegisterEvent("avatar",15000, 0)
	pUnit:RegisterEvent("knock",7500, 0)
	pUnit:RegisterEvent("thunderclapp",20000, 0)
	pUnit:RegisterEvent("summoniron",25000, 0)
	if pUnit:GetHealthPct() < 10 then
	pUnit:SetHealth(17000)
	end
end

function thunderclapp(pUnit, Event)
	print "magni_thunderclap"
	pUnit:CastSpell(23931)
end

function knock(pUnit, Event)
	print "magni_knock"
	pUnit:CastSpellOnTarget(20686,pUnit:GetClosestPlayer(0))
end

function summoniron(pUnit, Event)
	print "magni_summon"
	local x = pUnit:GetX()
	local y = pUnit:GetY()
	local z = pUnit:GetZ()
	local o = pUnit:GetO()
	pUnit:SpawnCreature(5595, x, y, z, o, 84, 60000) -- Summon ironforge   after 60 sec  autom. despawn	
	pUnit:SpawnCreature(5595, x, y, z, o, 84, 60000)
end

function avatar(pUnit, Event)
	print "magni_avatar"
	pUnit:CastSpell(19135)
end

function magni_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end

function magni_OnDied(pUnit, event, player)
pUnit:RemoveEvents()
end

RegisterUnitEvent(2784, 1, "magni_OnCombat")
RegisterUnitEvent(2784, 2, "magni_OnLeaveCombat")
RegisterUnitEvent(2784, 4, "magni_OnDied")

--Tyrande Whisperwind --

function tyrande_OnCombat(pUnit, Event)
	pUnit:PlaySoundToSet(5885)
	pUnit:RegisterEvent("starfall",3000, 0)
end

function starfall(pUnit, Event)
	print "tyrande_starfall"
	pUnit:CastSpellOnTarget(20687,pUnit:GetrandomPlayer(0))
end


function tyrande_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end


function tyrande_OnOnDied(pUnit, event, player)
pUnit:RemoveEvents()
end

RegisterUnitEvent(7999, 1, "tyrande_OnCombat")
RegisterUnitEvent(7999, 2, "tyrande_OnLeaveCombat")
RegisterUnitEvent(7999, 4, "tyrande_OnDied")

--Prophet Velen--

function velen_OnCombat(pUnit, Event)
	pUnit:PlaySoundToSet(10154)
	pUnit:RegisterEvent("prsmite",4000, 0)
	pUnit:RegisterEvent("summonguard",35000, 0)
end

function prsmite(pUnit, Event)
	print "velen_smite"
	pUnit:CastSpellOnTarget(36176,pUnit:GetrandomPlayer(0))
end

function summonguard(pUnit, Event)
	print "velen_summon"
	local x = pUnit:GetX()
	local y = pUnit:GetY()
	local z = pUnit:GetZ()
	local o = pUnit:GetO()
	pUnit:SpawnCreature(20674, x, y, z, o, 84, 60000) -- Summon guard   after 60 sec  autom. despawn	
	pUnit:SpawnCreature(20674, x, y, z, o, 84, 60000)
	pUnit:SpawnCreature(20674, x, y, z, o, 84, 60000)
end

function velen_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end


function velen_OnOnDied(pUnit, event, player)
pUnit:RemoveEvents()
end

RegisterUnitEvent(17468, 1, "velen_OnCombat")
RegisterUnitEvent(17468, 2, "velen_OnLeaveCombat")
RegisterUnitEvent(17468, 4, "velen_OnDied")