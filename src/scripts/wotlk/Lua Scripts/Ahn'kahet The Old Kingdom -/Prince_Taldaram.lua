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

function Prince_Taldaram_OnCombat(pUnit, Event)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(12, 0, "I will feast on your remains.")
	pUnit:RegisterEvent("blood",17000, 0)
	pUnit:RegisterEvent("vasnih",24000, 0)
	pUnit:RegisterEvent("conjure",35000, 0)
	pUnit:RegisterEvent("vampyr",40000, 0)
end

function blood(pUnit,Event)
	pUnit:FullCastSpell(55968)
end

function vasnih(pUnit,Event)
	pUnit:FullCastSpell(55964)
	local vchoice=math.random(1, 2)
	if vchoice==1 then
		--pUnit:PlaySoundToSet()
		pUnit:SendChatMessage(12, 0,"Your heartbeat is music to my ears.")
	elseif vchoice==2
		--pUnit:PlaySoundToSet()
		pUnit:SendChatMessage(12, 0,"I am nowhere. I am everywhere. I am the watcher, unseen.")
	end
end

--Serverside script..  i think it's doesn't working..--
function conjure(pUnit,Event)
	pUnit:FullCastSpellOnTarget(55931,pUnit:GetClosestPlayer(0))
end

function vampyr(pUnit,Event)
	pUnit:FullCastSpellOnTarget(55959,pUnit:GetClosestPlayer(0))
	local vamchoice=math.random(1, 2)
	if vamchoice==1 then
		--pUnit:PlaySoundToSet()
		pUnit:SendChatMessage(12, 0,"So appetizing.")
	elseif vamchoice==2
		--pUnit:PlaySoundToSet()
		pUnit:SendChatMessage(12, 0,"Fresh, warm blood. It has been too long. ")
	end
end

function Prince_Taldaram_OnKilledTarget (pUnit, Event)
	local Choice=math.random(1, 2)
	if Choice==1 then
		--pUnit:PlaySoundToSet()
		pUnit:SendChatMessage(12, 0,"I will drink no blood before it's time.")
	elseif Choice==2 then	
		--pUnit:PlaySoundToSet()
		pUnit:SendChatMessage(12, 0,"One final embrace.")
end		
end

function Prince_Taldaram_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end

function Prince_Taldaram_Died(pUnit, event, player)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(12, 0, "Still I hunger, still I thirst.")	
	pUnit:RemoveEvents()
end

RegisterUnitEvent(29308, 1, "Prince_Taldaram_OnCombat")
RegisterUnitEvent(29308, 2, "Prince_Taldaram_OnLeaveCombat")
RegisterUnitEvent(29308, 3, "Prince_Taldaram_OnDied")
RegisterUnitEvent(29308, 4, "Prince_Taldaram_OnKilledTarget")