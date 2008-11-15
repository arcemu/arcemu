--[[

   /******************************************************************
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
   ******************************************************************/ 

********************************
*                                                            *
* The ArcScript Scripting Project        *
*                                                            *
********************************

This software is provided as free and open source by the
staff of The Moon++ Scripting Project, in accordance with
the GPL license. This means we provide the software we have
created freely and it has been thoroughly tested to work for
the developers, but NO GUARANTEE is made it will work for you
as well. Please give credit where credit is due, if modifying,
redistributing and/or using this software. Thank you.

Staff of The Moon++ Scripting Project, December 2007
~~End of License Agreement

All credits for the files in this folder that are GameMonkey scripts
go to the GMScripts project, for all their hard work for the Ascent
community.

-- Moon++ Project, February 2008 ]]
--[[ 
TO DO:
Do the entire 5 minute phase 1, did the after phases to get the bulk done first.
Spawn all the creatures for phase 1
Do the scripts for the spawned creatures that come in waves
Find a new way to define the boss as a unit.
NOT WORKING:
Detonate mana always casts on himself.
The guardians are not moving to the center
Blood Taps damage always hits himself, tried the if plr~=nil stuff, but the damage itself will always deal to him.
Replaced Blood tap with Growth, functions the same, but different spells.
Chains of KelThuzad--the mind control is not supported by ascent
-- ]]

function kt_OnCombat(Unit, Event)
	local phase3 = 0
	local agrochoice = math.random(1,3)
	local fbtime=math.random(15000,25000)
		if agrochoice==1 then
			Unit:PlaySoundToSet(8809)
			Unit:SendChatMessage(14,0,"Pray For Mercy!")
		end
		if agrochoice==3 then
			Unit:PlaySoundToSet(8811)
			Unit:SendChatMessage(14,0,"The end is Upon you!")
		end
		if agrochoice==2 then
			Unit:PlaySoundToSet(8810)
			Unit:SendChatMessage(14,0,"Scream your dying breath!")
		end
	Unit:RegisterEvent("Frostbolt1", 60000,0)
	Unit:RegisterEvent("Frostbolt2", fbtime,1)
	Unit:RegisterEvent("DetonateMana", 16000,0)
	Unit:RegisterEvent("Fissure", 30000,0)
	Unit:RegisterEvent("FrostBlast", 25000,0)
	Unit:RegisterEvent("Phase3",1000,0)
end

function kt_OnLeaveCombat(Unit)
	Unit:RemoveEvents()
end

function kt_OnKilledTarget(Unit)
	local KilledChoice = math.random(1,2)
		if KilledChoice==1 then
			Unit:PlaySoundToSet(8817)
			Unit:SendChatMessage(14,0,"The dark void awaits	you!")
		else
			Unit:PlaySoundToSet(8818)
		end	
end

function kt_OnDied(Unit)
	Unit:PlaySoundToSet(8814)
	Unit:SendChatMessage(14,0,"Do not rejoice... your victory is a hollow one... for I shall return	with powers beyond your	imagining!")
	Unit:RemoveEvents()	
end

function Frostbolt1(pUnit, event)
	pUnit:FullCastSpellOnTarget(28478, pUnit:GetMainTank())
end

function Frostbolt(pUnit, event)
	local fbtime=math.random(15000,45000)
	pUnit:CastSpell(28479)
	pUnit:RegisterEvent("Frostbolt2",fbtime,1)
end

function Frostbolt2(pUnit)
	local fbtime=math.random(15000,45000)
	pUnit:CastSpell(28479)
	pUnit:RegisterEvent("Frostbolt",fbtime,1)
end

function DetonateMana(pUnit, event)
	local mplr = pUnit:GetRandomPlayer(0)
		if mplr	~= nil then
			pUnit:CastSpellOnTarget(27819, mplr)
		end
end

function Fissure(pUnit)
	local FissureTarget=pUnit:GetRandomPlayer(0)
	local Fisx = FissureTarget:GetX()
	local Fisy = FissureTarget:GetY()
	local Fisz = FissureTarget:GetZ()
	local Fiso = FissureTarget:GetO()
	pUnit:SpawnCreature(16129,Fisx,Fisy,Fisz,Fiso,14,10000)
end

function Fissure_OnSpawn(pUnit, event)
	pUnit:RegisterEvent("VoidBlast",5000,0)
	pUnit:StopMovement(10000)
end

function VoidBlast(Unit, event)
	Unit:SendChatMessage(14,0,"Void Blast")
	Unit:CastSpell(27812)
end

function FrostBlast(pUnit)
	pUnit:PlaySoundToSet(8815)
	pUnit:SendChatMessage(14,0,"I shall freeze the blood in your veins!")
	pUnit:CastSpellOnTarget(27808, pUnit:GetRandomPlayer(0))
end

function Phase3(Unit, Event)
		if Unit:GetHealthPct() < 41 then
			if phase3==0 then
				Unit:PlaySoundToSet(8816)
				Unit:SendChatMessage(14,0, "Master! I require aid!")
				Unit:SpawnCreature(16441, 3755.697510, -5080.873535, 142.369598, 3.500572, 14,0)
				Unit:SpawnCreature(16441, 3739,	-5150, 143.17354, 2.149683, 14,0)
				Unit:SpawnCreature(16441, 3706,	-5154, 143, 2, 14,0)
				Unit:SpawnCreature(16441, 3726,	-5056, 143.17354, 4.5, 14,0)
				Unit:SpawnCreature(16441, 3673,	-5099, 143.17354, 6, 14,0)
				phase3 = 1
			else
			end
		end
end

function Icecrown(pUnit)	
	pUnit:MoveTo(3717.074951, -5106.559082,141.289780, 3.649795)
end

function Ice_Combat(pUnit, Event)
	ShackleAura = 0
	Target1	= pUnit:GetName()
	pUnit:CastSpell(36300)
	pUnit:RegisterEvent("BloodTap",1000,0)
	pUnit:RegisterEvent("ShackleStatus",1000,0)
end

function ShackleStatus(Unit, event)
		if Unit:HasAura(40135) then
			ShackleAura = ShackleAura + 1
			if ShackleAura >= 4 then
				Unit:RemoveAura(40135)
			end
		end
end

function BloodTap(pUnit, event)
	Target2	= pUnit:GetName()
		if Target2 ~= Target1 then
			pUnit:CastSpell(36300)
			Target1	= Target2
		end
end


function Ice_Death(pUnit)
	pUnit:RemoveEvents()
end


RegisterUnitEvent(15990,1,"kt_OnCombat")
RegisterUnitEvent(15990,2,"kt_OnLeaveCombat")
RegisterUnitEvent(15990,3,"kt_OnKilledTarget")
RegisterUnitEvent(15990,4,"kt_OnDied")
RegisterUnitEvent(16441,6,"Icecrown")
RegisterUnitEvent(16441,1,"Ice_Combat")
RegisterUnitEvent(16129,6,"Fissure_OnSpawn")
