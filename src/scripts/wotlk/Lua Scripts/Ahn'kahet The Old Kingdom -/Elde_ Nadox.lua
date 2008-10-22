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
function Elder_Nadox_OnEnterCombat(pUnit,Event)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(12, 0,"The secrets of the deep shall remain hidden.")
	pUnit:RegisterEvent("eldersummon",20000, 0)
	pUnit:RegisterEvent("elderspell",8000, 0)
end

function eldersummon (pUnit,Event)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(12, 0,"The young must not grow hungry.")
	local x = pUnit:GetX()
	local y = pUnit:GetY()
	local z = pUnit:GetZ()
	local o = pUnit:GetO()
	--pUnit:SpawnCreature(??? , x, y, z, o, 14, 60000)
end

function elderspell (pUnit,Event)
	pUnit:FullCastSpellOnTarget(56130,pUnit:GetClosestPlayer(0))
end


function Elder_Nadox_OnKilledTarget (pUnit, Event)
	local killchoice=math.random(1, 3)
	if killchoice==1 then
		--pUnit:PlaySoundToSet()
		pUnit:SendChatMessage(12, 0,"Sleep now, in the cold dark.")
	elseif killchoice==2 then	
		--pUnit:PlaySoundToSet()
		pUnit:SendChatMessage(12, 0,"For the Lich King!")
	elseif killchoice==3 then
		--pUnit:PlaySoundToSet()
		pUnit:SendChatMessage(12, 0,"Perhaps we will be allies soon. ")	
end		
end

function Elder_Nadox_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end

function Elder_Nadox_Died(pUnit, event, player)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(12, 0, "Master, is my service complete?")	
	pUnit:RemoveEvents()
end

RegisterUnitEvent(29309, 1, "Elder_Nadox_OnCombat")
RegisterUnitEvent(29309, 2, "Elder_Nadox_OnLeaveCombat")
RegisterUnitEvent(29309, 3, "Elder_Nadox_OnDied")
RegisterUnitEvent(29309, 4, "Elder_Nadox_OnKilledTarget")