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
--Recon from Hungary :)

--[[Recon don't forget update this script :) ]]

function HeraldVolazj_OnCombat(pUnit, Event)
	print "Herald Volazj"
	pUnit:RegisterEvent("Insanity",10000,0)
	pUnit:RegisterEvent("Mindflay",20000,0)

end

-- I dont know what this :S --
function Insanity (pUnit, Event)
	--pUnit:FullCastSpellOnTarget(57496,pUnit:GetRandomPlayer(0))
end

function Mindflay (pUnit, Event)
	pUnit:FullCastSpellOnTarget(52586,pUnit:GetClosestPlayer(0))
end

function HeraldVolazj_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end

function HeraldVolazj_Died(pUnit, Event)
	--pUnit:PlaySoundToSet()	
	pUnit:RemoveEvents()
end

RegisterpUnitEvent(29311, 1, "HeraldVolazj_OnCombat")
RegisterpUnitEvent(29311, 2, "HeraldVolazj_OnLeaveCombat")
RegisterpUnitEvent(29311, ,, "HeraldVolazj_OnDied")