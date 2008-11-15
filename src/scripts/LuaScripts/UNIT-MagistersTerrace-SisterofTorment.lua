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

function SunbladeSisterofTorment_OnCombat(Unit, Event)
Unit:RegisterEvent("SunbladeSisterofTorment_DeadlyEmbrace", 10000, 0)
Unit:RegisterEvent("SunbladeSisterofTorment_LashofPain",7000, 0) 
end

--Deadly Embrace
function SunbladeSisterofTorment_DeadlyEmbrace(Unit, Event)
Unit:FullCastSpellOnTarget(44547, Unit:GetRandomPlayer(1))
end

--44640 Lash of Pain
function SunbladeSisterofTorment_LashofPain(Unit, Event)
local Flip = math.random(1,2)
if Flip == 1 then
Unit:CastSpellOnTarget(44640, Unit:GetClosestPlayer())
end
end


function SunbladeSisterofTorment_LeaveCombat(Unit, Event)
Unit:RemoveEvents()
end

function SunbladeSisterofTorment_Died(Unit, Event)
Unit:RemoveEvents()
end


RegisterUnitEvent(24697, 1, "SunbladeSisterofTorment_OnCombat")
RegisterUnitEvent(24697, 2, "SunbladeSisterofTorment_LeaveCombat")
RegisterUnitEvent(24697, 4, "SunbladeSisterofTorment_Died")
