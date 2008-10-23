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
Todo: 10543 sound?
-- ]]
--Rewrite: Recon--

function Shaffar(pUnit, Event)
	pUnit:RegisterEvent("shaffar_Say",68000, 0)
end

function shaffar_Say(pUnit, Event)
	pUnit:PlaySoundToSet(10539)
	pUnit:SendChatMessage(11, 0, "What is this? You must forgive me, but I was not expecting company. As you can see, we are somewhat preoccupied right now. But no matter. As I am a gracious host, I will tend to you... personally.")
end

function Shaffar_Blink(pUnit, Event)
	print "Shaffar_Blink"
	pUnit:FullCastSpell(14514)
end

function ScriptShaffar_Frost_Nova(pUnit, Event)
	print "Shaffar Frost Nova"
	pUnit:FullCastSpellOnTarget(39035,Unit:GetClosestPlayer(0))
end

function ScriptShaffar_sapwn(pUnit, Event)
	print "Shaffar sapwn"
	pUnit:SpawnCreature(18431, -212.7, -0.3, 16.9, 0, 18, 36000000);
	pUnit:PlaySoundToSet(10540)
	pUnit:SendChatMessage(11, 0, "I have such fascinating things to show you.")
end

function ScriptShaffar_FrostBolt(pUnit, Event)
	print "Shaffar FrostBolt"
	pUnit:FullCastSpellOnTarget(40429,Unit:GetClosestPlayer(0))
end

function ScriptShaffar_Fireball(pUnit, Event)
	print "Shaffar Fireball"
	pUnit:FullCastSpellOnTarget(20420,Unit:GetClosestPlayer(0))
end

function ScriptShaffar_say(pUnit, Event)
	local shafchance = math.random(1,6)
	if(shafchance == 1) then
	pUnit:PlaySoundToSet(10541)
	pUnit:SendChatMessage(11, 0, "We have not yet been properly introduced.")
	end
	if(shafchance == 2) then
	pUnit:PlaySoundToSet(10542)
	pUnit:SendChatMessage(11, 0, "An epic battle. How exciting!")
	end
end

function Shaffar_OnCombat(pUnit, Event)
	print "Shaffar"
	pUnit:RegisterEvent("ScriptShaffar_say",1,1)
	pUnit:RegisterEvent("ScriptShaffar_Blink",10000,0)
	pUnit:RegisterEvent("ScriptShaffar_Frost_Nova",13000,0)
	pUnit:RegisterEvent("ScriptShaffar_sapwn",17000,0)
	pUnit:RegisterEvent("ScriptShaffar_FrostBolt",23000,0)
	pUnit:RegisterEvent("ScriptShaffar_Fireball",27000,0)
end

function Shaffar_OnKilledTarget (pUnit, Event)
	local shachance = math.random(1,2)
	if(shachance == 1) then
	pUnit:PlaySoundToSet(10545)
	pUnit:SendChatMessage(12, 0,"It has been... entertaining.")
	end
	if(shachance == 2) then
	pUnit:PlaySoundToSet(10544)	
	pUnit:SendChatMessage(12, 0,"And now we part company.")
	end
end

function Shaffar_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end

function Shaffar_OnDied(pUnit, Event)
	pUnit:PlaySoundToSet(10546)
	pUnit:SendChatMessage(12, 0,"I must bid you... farewell.")
	pUnit:RemoveEvents()
end

RegisterUnitEvent(18344, 1, "Shaffar")
RegisterUnitEvent(18344, 2, "Shaffar_OnCombat")
RegisterUnitEvent(18344, 3, "Shaffar_OnLeaveCombat")
RegisterUnitEvent(18344, 4, "Shaffar_OnKilledTarget")
RegisterUnitEvent(18344, 5, "Shaffar_OnDied")