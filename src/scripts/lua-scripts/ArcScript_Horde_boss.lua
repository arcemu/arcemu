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
--Thrall--

function thrall_OnCombat(pUnit, Event)
	pUnit:PlaySoundToSet(5880)
	pUnit:RegisterEvent("chain",12000, 0)
	pUnit:RegisterEvent("shock",17000, 0)
	pUnit:RegisterEvent("summonka",25000, 0)
end

function chain(pUnit, Event)
	print "Thrall_chainlight"
	local chainCheck = pUnit:GetClosestPlayer(0)
	if (chainCheck ~= nil) then
	pUnit:FullCastSpellOnTarget(16033,pUnit:GetClosestPlayer(0))
end
end

function shock(pUnit, Event)
	print "Thrall_shock"
	local shockCheck = pUnit:GetClosestPlayer(0)
	if (shockCheck ~= nil) then
		pUnit:FullCastSpellOnTarget(16034,pUnit:GetClosestPlayer(0))
	end
end

function summonka(pUnit, Event)
	print "Thrall_summon"
	local x = pUnit:GetX()
	local y = pUnit:GetY()
	local z = pUnit:GetZ()
	local o = pUnit:GetO()
	pUnit:SpawnCreature(3296, x, y, z, o, 83, 60000) -- Summon Grunt   after 60 sec grunt autom. despawn	
	pUnit:SpawnCreature(3296, x, y, z, o, 83, 60000)
end

function thrall_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end


function thrall_OnDied(pUnit, event, player)
pUnit:RemoveEvents()
end

RegisterUnitEvent(4949, 1, "thrall_OnCombat")
RegisterUnitEvent(4949, 2, "thrall_OnLeaveCombat")
RegisterUnitEvent(4949, 4, "thrall_OnDied")


-- Vol'jin  ai_script--

function vojin_OnCombat(pUnit, Event)
	pUnit:PlaySoundToSet(5881)
end

function vojin_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end

function vojin_OnDied(pUnit, event, player)
pUnit:RemoveEvents()
end

RegisterUnitEvent(10540, 1, "vojin_OnCombat")
RegisterUnitEvent(10540, 2, "vojin_OnLeaveCombat")
RegisterUnitEvent(10540, 4, "vojin_OnDied")

-- Lady Sylvanas Windrunner  ai_script--

function ladyswindr_OnCombat(pUnit, Event)
	pUnit:PlaySoundToSet(5886)
	pUnit:RegisterEvent("fade",35000, 3)
	pUnit:RegisterEvent("multishot",9000, 0)
	pUnit:RegisterEvent("shoot",13000, 0)
	pUnit:RegisterEvent("summonn",19000, 0)
end

function shoot(pUnit, Event)
	print "ladyswindr_shoot"
	local shootCheck = pUnit:GetClosestPlayer(0)
	if (shootCheck ~= nil) then
		pUnit:CastSpellOnTarget(20463,pUnit:GetClosestPlayer(0))
	end
end


function fade(pUnit, Event)
	print "ladyswindr_fade"
	pUnit:FullCastSpell(20672)
end

function multishot(pUnit, Event)
	print "ladyswindr_multishot"
	local multiCheck = pUnit:GetClosestPlayer(0)
	if (multiCheck ~= nil) then
		pUnit:CastSpellOnTarget(20735,pUnit:GetClosestPlayer(0))
	end
end

function summonn(pUnit, Event)
	print "ladyswindr_summon"
	pUnit:CastSpell(20464)
end

function ladyswindr_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end

function ladyswindr_OnDied(pUnit, event, player)
pUnit:RemoveEvents()
end

RegisterUnitEvent(10181, 1, "ladyswindr_OnCombat")
RegisterUnitEvent(10181, 2, "ladyswindr_OnLeaveCombat")
RegisterUnitEvent(10181, 4, "ladyswindr_OnDied")

--Varimathras--
function Varimathras_OnCombat(pUnit, Event)
	pUnit:PlaySoundToSet(5887)
	pUnit:RegisterEvent("drainlife",15000, 0)
	pUnit:RegisterEvent("shadoww",9000, 0)
end



function drainlife(pUnit, Event)
	print "Varimathras_drainlife"
	local drainCheck = pUnit:GetClosestPlayer(0)
	if (drainCheck ~= nil) then
		pUnit:CastSpellOnTarget(20743,pUnit:GetClosestPlayer(0))
	end
end

function shadoww(pUnit, Event)
	print "Varimathras_shadow"
	pUnit:CastSpellOnTarget(20741,pUnit:GetClosestPlayer(0))
end

function Varimathras_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end

function Varimathras_OnDied(pUnit, event, player)
pUnit:RemoveEvents()
end

RegisterUnitEvent(2425, 1, "Varimathras_OnCombat")
RegisterUnitEvent(2425, 2, "Varimathras_OnLeaveCombat")
RegisterUnitEvent(2425, 4, "Varimathras_OnDied")


--Cairne Bloodhoof --

function cairblood_OnCombat(pUnit, Event)
	pUnit:PlaySoundToSet(5884)
	pUnit:RegisterEvent("charge",1, 1)
	pUnit:RegisterEvent("cleave",7000, 0)
	pUnit:RegisterEvent("mortalStrike",12000, 0)
	pUnit:RegisterEvent("thunderclap",21000, 0)
	pUnit:RegisterEvent("uppercut",30000, 0)
	pUnit:RegisterEvent("warstomp",35000, 0)
end

function charge(pUnit, Event)
--	print "cairblood_charge" 
--	pUnit:castSpell(16636)
end

function cleave(pUnit, Event)
	print "cairblood_cleave"
	local cleaveCheck = pUnit:GetClosestPlayer(0)
	if (cleaveCheck ~= nil) then
		pUnit:CastSpellOnTarget(16044,pUnit:GetClosestPlayer(0))
	end
end

function mortalStrike(pUnit, Event)
	print "cairblood_MortalStrike"
	local mortalCheck = pUnit:GetClosestPlayer(0)
	if (mortalCheck ~= nil) then
		pUnit:CastSpellOnTarget(16856,pUnit:GetClosestPlayer(0))
	end
end

function thunderclap(pUnit, Event)
	print "cairblood_thunderclap"
	pUnit:CastSpell(23931)
end

function uppercut(pUnit, Event)
	print "cairblood_Uppercut"
	local upperCheck = pUnit:GetClosestPlayer(0)
	if (upperCheck ~= nil) then
		pUnit:CastSpellOnTarget(22916,pUnit:GetClosestPlayer(1))
	end
end

function warstomp(pUnit, Event)
	print "cairblood_warstomp"
	pUnit:CastSpell(15593)
end

function cairblood_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end


function cairblood_OnDied(pUnit, event, player)
pUnit:RemoveEvents()
end

RegisterUnitEvent(3057, 1, "cairblood_OnCombat")
RegisterUnitEvent(3057, 2, "cairblood_OnLeaveCombat")
RegisterUnitEvent(3057, 4, "cairblood_OnDied")

--Lor'themar Theron--

function lorthemar_OnCombat(pUnit, Event)
	--pUnit:PlaySoundToSet(i i  dont know :D )
	pUnit:RegisterEvent("cleavee",7000, 0)
	pUnit:RegisterEvent("mannaburn",12000, 0)
	pUnit:RegisterEvent("shock",17000, 0)
	pUnit:RegisterEvent("charm",30000, 0)
end

function cleavee(pUnit, Event)
	print "lorthemar_cleave"
	local cleaveCheck = pUnit:GetClosestPlayer(0)
	if (cleaveCheck ~= nil) then
		pUnit:CastSpellOnTarget(16044,pUnit:GetClosestPlayer(0))
	end
end

function mannaburn(pUnit, Event)
	print "lorthemar_mannaburn"
	local burnCheck = pUnit:GetClosestPlayer(0)
	if (burnCheck ~= nil) then
		pUnit:CastSpellOnTarget(33385,pUnit:GetClosestPlayer(4))
	end
end

function shock(pUnit, Event)
	print "lorthemar_shock"
	local shockCheck = pUnit:GetClosestPlayer(0)
	if (shockCheck ~= nil) then
		pUnit:FullCastSpellOnTarget(16034,pUnit:GetClosestPlayer(0))
	end
end

function charm(pUnit, Event)
	print "lorthemar_charm"
	local charmCheck = pUnit:GetRandomPlayer(1)
	if (charmCheck ~= nil) then
	pUnit:CastSpellOnTarget(33384,pUnit:GetRandomPlayer(1))
end
end


function lorthemar_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end


function lorthemar_OnDied(pUnit, event, player)
pUnit:RemoveEvents()
end

RegisterUnitEvent(16802, 1, "lorthemar_OnCombat")
RegisterUnitEvent(16802, 2, "lorthemar_OnLeaveCombat")
RegisterUnitEvent(16802, 4, "lorthemar_OnDied")