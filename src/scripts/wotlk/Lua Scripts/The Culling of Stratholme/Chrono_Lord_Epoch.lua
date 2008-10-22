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



function ChronoLordep_TimeWarp(pUnit,Event)
	pUnit:FullCastSpellOnTarget(52766,pUnit:GetClosestPlayer(0))
	local warpchoice=math.random(1, 3)
		if warpchoice==1 then
			pUnit:SendChatMessage(12, 0, "Tick tock, tick tock...")
		elseif warpchoice==2 then
			pUnit:SendChatMessage(12, 0, "Not quick enough!")
		elseif warpchoice==3 then
			pUnit:SendChatMessage(12, 0, "Let's get this over with.")
end
end

function ChronoLordep_CurseOe(pUnit,Event)
	pUnit:FullCastSpellOnTarget(52772,pUnit:GetClosestPlayer(0))
end

function ChronoLordep(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "Prince Arthas Menethil, on this day, a powerful darkness has taken hold of your soul. The death you are destined to visit upon others will this day be your own.")
end

function ChronoLordep_WoundingStrike(pUnit,Event)
	pUnit:FullCastSpellOnTarget(52771,pUnit:GetClosestPlayer(0))
end

function ChronoLordep_OnKilledTarget(pUnit,Event)
	local killChoice=math.random(1, 3)
		if killChoice==1 then
			pUnit:SendChatMessage(12, 0, "There is no future for you.")
		elseif killChoice==2 then
			pUnit:SendChatMessage(12, 0, "This is the hour of our greatest triumph!")
		elseif killChoice==3 then
			pUnit:SendChatMessage(12, 0, "You would destined to fail.")
end
end

function ChronoLordep_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

function ChronoLordep_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function ChronoLordep_OnEnterCombat(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "We'll see about that, young prince.")
	pUnit:RegisterEvent("ChronoLordep_TimeWarp", 18000, 0)
	pUnit:RegisterEvent("ChronoLordep_CurseOe", 16000, 0)
	pUnit:RegisterEvent("ChronoLordep_WoundingStrike", 8000, 0)
end

RegisterUnitEvent(26532, 1, "ChronoLordep_OnEnterCombat")
RegisterUnitEvent(26532, 2, "ChronoLordep")
RegisterUnitEvent(26532, 3, "ChronoLordep_OnKilledTarget")
RegisterUnitEvent(26532, 4, "ChronoLordep_OnDied")
RegisterUnitEvent(26532, 5, "ChronoLordep_OnLeaveCombat")