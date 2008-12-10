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
Made by: Tally/`Angel
-- ]]
motherPhase = 0
beamPhase = 0
beamTimes = 1
mother = 22947
multiIntro = 0
round = 1
repeatAllow = 0
beamAllow = 0
aura = 0
lashTime = math.random(30000,90000)
attAllow = 0

--------------



------Introduction------
function MotherIntro(pUnit, event)

print "[ANGELSCRIPT]: Mother Fight beginning!"
pUnit:SendChatMessage(12, 0, "Oh, my darlings! You wouldn't hurt your little old mother would you? Ah, I guess I'll have to teach you a lesson!")
pUnit:RegisterEvent("Mother_OnCombat",1000,0)
motherPhase = 0
end

-----COMBAT-------
function Mother_OnCombat(pUnit, event)
	if motherPhase == 0 then
	pUnit:FullCastSpellOnTarget(40879, pUnit)
	beamPhase = 1
	beamTimes = 1
	print "[ANGELSCRIPT]: Motherfight Entering round 1!"
	pUnit:RegisterEvent("Mother_BeamOne",10000,0)
	pUnit:RegisterEvent("Mother_Auras",10000,0)
	motherPhase = 1
	else
	end
end
----------------

--------ABILITIES----------
function Mother_BeamOne(pUnit, event)
	if beamPhase == 1 then
	beamAllow = 0
		pUnit:FullCastSpellOnTarget(40859, pUnit:GetMainTank())
		print "Firing Beam 1"
		if beamTimes == 1 then
			if beamAllow == 0 then
				beamTimes = 2
				beamAllow = 1
				print "1-1"
			end
		end
		if beamTimes == 2 then
			if beamAllow == 0 then
				beamTimes = 3
				beamAllow = 1
				print "1-2"
			end
		end	
		if beamTimes == 3 then
			if beamAllow == 0 then
			beamPhase = 2
			print "1-3"
			print "Going to BeamPhase2"
			pUnit:RemoveEvents()
			pUnit:RegisterEvent("Mother_BeamTwo",10000,0)
			beamAllow = 0
			pUnit:RegisterEvent("Mother_SaberLash",30000,0)
			end
		end
	end
end
	
function Mother_BeamTwo(pUnit, event)
	if beamPhase == 2 then
		pUnit:FullCastSpellOnTarget(40860, pUnit:GetMainTank())
		print "Firing Beam 2"
		beamAllow = 0
		if beamTimes == 3 then
			if beamAllow == 0 then
				beamTimes = 4
				beamAllow = 1
				print "2-1"
			end
		end
		if beamTimes == 4 then
			if beamAllow == 0 then
				beamTimes = 5
				beamAllow = 1
				print "2-2"
			end
		end
		if beamTimes == 5 then
			if beamAllow == 0 then
			beamPhase = 3
			print "2-3"
			pUnit:RemoveEvents()
			pUnit:RegisterEvent("Mother_BeamThree",10000,0)
						pUnit:RegisterEvent("Mother_SaberLash",30000,0)
							pUnit:RegisterEvent("Mother_FatalAttraction",10,0)
			beamAllow = 0
			print "Going to BeamPhase 3"
			end
		end
	end	
end
	
function Mother_BeamThree(pUnit, event)
	if beamPhase == 3 then
		beamAllow = 0
		pUnit:FullCastSpellOnTarget(40861, pUnit:GetMainTank())
		repeatAllow = 0
		print "Firing beam 3"
		if beamTimes == 5 then
			if beamAllow == 0 then
				beamTimes = 6
				beamAllow = 1
				print "3-1"
			end
		end
		if beamTimes == 6 then
			if beamAllow == 0 then
				beamTimes = 7
				beamAllow = 1
				print "3-2"
			end
		end
		if beamTimes == 7 then
			if beamAllow == 0 then
				pUnit:RemoveEvents()
				pUnit:RegisterEvent("Mother_BeamFour",10000,0)
							pUnit:RegisterEvent("Mother_SaberLash",30000,0)
				beamAllow = 0
				beamPhase = 4
				print "3-3"
				print "Going to BeamPhase 4"
			end
		end
	end	
end

function Mother_BeamFour(pUnit, event)
	if beamPhase == 4 then
		pUnit:FullCastSpellOnTarget(40827, pUnit:GetMainTank())
		repeatAllow = 0
		beamAllow = 0
		print "Firing beam 4"
		if beamTimes == 7 then
			if beamAllow == 0 then
				beamTimes = 8
				beamAllow = 1
				print "4-1"
			end
		end
		if beamTimes == 8 then
			if beamAllow == 0 then
				beamTimes = 9
				beamAllow = 1
				print "4-2"
			end
		end
		if beamTimes == 9 then
			if beamAllow == 0 then
			repeatAllow = 0
			print "4-3"
			pUnit:RemoveEvents()
			pUnit:RegisterEvent("Mother_RestartBeams",10,0)
			end
		end
	end	
end

function Mother_RestartBeams(pUnit, event)
	if repeatAllow == 0 then
		beamPhase = 1
		beamTimes = 1
		repeatAllow = 1
		print "[ANGELSCRIPT]: Repeating Beam Phase 1!"
		pUnit:RegisterEvent("Mother_BeamOne",10000,0)
	end
end
	



function Mother_Auras(pUnit, event)
aura = math.random(1, 6)
	if aura == 1 then
		pUnit:FullCastSpell(40880)
	end
	if aura == 2 then
		pUnit:FullCastSpell(40882)
	end
	if aura == 3 then
		pUnit:FullCastSpell(40883)
	end	
	if aura == 4 then
		pUnit:FullCastSpell(40891)
	end
	if aura == 5 then
		pUnit:FullCastSpell(40896)
	end
	if aura == 6 then
		pUnit:FullCastSpell(40897)
	end
end

function Mother_SaberLash(pUnit, event)
pUnit:FullCastSpellOnTarget(40810,pUnit:GetMainTank())
print "Mother Casting Lash"
end

function motherLeave(pUnit, event)
pUnit:RemoveEvents()
motherPhase = 0
beamPhase = 0
beamTimes = 0
end

function Mother_FatalAttraction(pUnit, event)
	if attAllow == 0 then
		pUnit:FullCastSpellOnTarget(40871,pUnit:GetMainTank())
		pUnit:FullCastSpellOnTarget(41001,pUnit:GetMainTank())
		print "Casting Fatal Attraction"
		attAllow = 1
	end
end

RegisterUnitEvent(22947, 2, "motherLeave")
RegisterUnitEvent(22947, 1, "MotherIntro")
RegisterUnitEvent(22947, 4, "motherLeave")



