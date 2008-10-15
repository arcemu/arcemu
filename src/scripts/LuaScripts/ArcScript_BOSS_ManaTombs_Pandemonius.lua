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
--Rewrite: Recon
function Pandemonius_OnCombat(pUnit, Event)
	pUnit:RegisterEvent("speak",2, 1)
	pUnit:RegisterEvent("Pandemonius_Void_Blast",5000, 0)
	pUnit:RegisterEvent("Pandemonius_Dark_Shell",17000, 0) --i dont know how often :S
end

function speak(pUnit, Event)
	local eselyke = math.random(1,3)
	if(eselyke == 1) then
	pUnit:PlaySoundToSet(10561)
	pUnit:SendChatMessage(11, 0, "I will feed on your soul.")
	end
	if(eselyke == 2) then
	pUnit:PlaySoundToSet(10562)
	pUnit:SendChatMessage(11, 0, "So... full of life!")
	end
	if(eselyke == 3) then
	pUnit:PlaySoundToSet(10563)
	pUnit:SendChatMessage(11, 0, "Do not... resist.")
	end	
end

function Pandemonius_Void_Blast(Unit, event, miscunit, misc)
	print "Pandemonius Void Blast"
	Unit:FullCastSpellOnTarget(32325,Unit:GetClosestPlayer(0))

end

function Pandemonius_Dark_Shell(Unit, event, miscunit, misc)
	print "Pandemonius Dark Shell"
	pUnit:CastSpell(32358)
end

function Pandemonius_OnKilledTarget (pUnit, Event)
	local eselly = math.random(1,2)
	if(eselly == 1) then
	pUnit:PlaySoundToSet(10565)
	pUnit:SendChatMessage(11, 0, "More... I must have more!")
	end
	if(eselly == 2) then
	pUnit:PlaySoundToSet(10564)
	pUnit:SendChatMessage(11, 0, "Yes! I am... empowered!")
	end
end

function Pandemonius_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end


function Pandemonius_Died(Unit, event, player)
pUnit:PlaySoundToSet(10566)
pUnit:SendChatMessage(11, 0, "To the void... once... more.")
Unit:RemoveEvents()
end

RegisterUnitEvent(18341, 1, "Pandemonius_OnCombat")
RegisterUnitEvent(18341, 2, "Pandemonius_OnLeaveCombat")
RegisterUnitEvent(18341, 3, "Pandemonius_OnKilledTarget")
RegisterUnitEvent(18341, 4, "Pandemonius_OnDied")
