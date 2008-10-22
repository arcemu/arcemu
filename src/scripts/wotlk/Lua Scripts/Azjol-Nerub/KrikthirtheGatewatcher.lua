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

function Krikthir_OnEnterCombat(pUnit,Event)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(12, 0,"This kingdom belongs to the Scourge! Only the dead may enter.")
	pUnit:RegisterEvent("krik_MindFlay",7000, 0)
	pUnit:RegisterEvent("CurseofFatigue",10000, 0)--52592 , 59368 Maybie Heroic spell?--
	pUnit:RegisterEvent("kriksummon",20000, 0)	
	if pUnit:GetHealthPct() < 10 then
	pUnit:RegisterEvent("Frenzy",1,1)
	end
end


function krik_MindFlay (pUnit, Event)
	pUnit:FullCastSpellOnTarget(52586,pUnit:GetClosestPlayer(0))
end

function CurseofFatigue(pUnit,Event)
	pUnit:FullCastSpellOnTarget(52592,pUnit:GetClosestPlayer(0))
end

function Frenzy (pUnit,Event)
	pUnit:CastSpell(28747)
end



function kriksummon (pUnit,Event)
	local Kriksayy=math.random(1, 3)
	if Kriksayy==1 then
		--pUnit:PlaySoundToSet()
		pUnit:SendChatMessage(12, 0,"We are besieged. Strike out and bring back their corpses!")
	elseif Kriksayy==2 then	
		--pUnit:PlaySoundToSet()
		pUnit:SendChatMessage(12, 0,"We must hold the gate. Attack! Tear them limb from limb!")
	elseif Kriksayy==3 then	
		--pUnit:PlaySoundToSet()
		pUnit:SendChatMessage(12, 0,"The gate must be protected at all costs. Rip them to shreds!")



--Need more information.. http://www.youtube.com/watch?v=dbyPaztbdGg --
	--[[pUnit:SpawnCreature(mobID, x, y, z, o, 14, 60000)
	pUnit:SpawnCreature(mobID, x, y, z, o, 14, 60000)
	pUnit:SpawnCreature(mobID, x, y, z, o, 14, 60000)
	pUnit:SpawnCreature(mobID, x, y, z, o, 14, 60000)
	pUnit:SpawnCreature(mobID, x, y, z, o, 14, 60000)
	pUnit:SpawnCreature(mobID, x, y, z, o, 14, 60000)
	pUnit:SpawnCreature(mobID, x, y, z, o, 14, 60000)]]
	
	pUnit:Registerevent:("Say",8000,0)
end	

function Say (pUnit,Event)
	local Kriksummon=math.random(1, 3)
	if Kriksummon==1 then
		--pUnit:PlaySoundToSet()
		pUnit:SendChatMessage(12, 0,"Keep an eye on the tunnel. We must not let anyone through!")
	elseif Kriksummon==2 then	
		--pUnit:PlaySoundToSet()
		pUnit:SendChatMessage(12, 0,"I hear footsteps. Be on your guard.")
	elseif Kriksummon==3 then	
		--pUnit:PlaySoundToSet()
		pUnit:SendChatMessage(12, 0,"I sense the living. Be ready.")
end


function Krikthir_OnKilledTarget (pUnit, Event)
	local Krikthichoice=math.random(1, 2)
	if Krikthichoice==1 then
		--pUnit:PlaySoundToSet()
		pUnit:SendChatMessage(12, 0,"You were foolish to come.")
	elseif Krikthichoice==2 then	
		--pUnit:PlaySoundToSet()
		pUnit:SendChatMessage(12, 0,"As Anub'Arak commands!")
end		
end

function Krikthir_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end

function Krikthir_OnDied(pUnit, event, player)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(12, 0,"I should be grateful. But I long ago lost the capacity.")
	pUnit:RemoveEvents()
end

RegisterUnitEvent(28684, 1, "Krikthir_OnCombat")
RegisterUnitEvent(28684, 2, "Krikthir_OnLeaveCombat")
RegisterUnitEvent(28684, 3, "Krikthir_OnDied")
RegisterUnitEvent(28684, 4, "Krikthir_OnKilledTarget")

--########################################################--
--Krikthir Minon AI---


--[[
When unleashing the swarm:
    * "They hunger."
    * "Dinner time, my pets." 
	]]