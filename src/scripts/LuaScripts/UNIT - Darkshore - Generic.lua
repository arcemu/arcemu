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

-- Cerellean Whiteclaw
function Cerellean_Whiteclaw_onSpawn (pUnit, Event)
	pUnit:Emote (68)
end
RegisterUnitEvent (3644, 6, "Cerellean_Whiteclaw_onSpawn")
RegisterUnitEvent (3644, 3, "Cerellean_Whiteclaw_onSpawn")
RegisterUnitEvent (3644, 2, "Cerellean_Whiteclaw_onSpawn")

function Cerellean_Whiteclaw_onCombat (pUnit, Event)
	pUnit:Emote (26)
end
RegisterUnitEvent (3644, 1, "Cerellean_Whiteclaw_onCombat")

-- Sentinel Tysha Moonblade
function Tysha_Moonblade_onSpawn (pUnit, Event)
	pUnit:Emote (68)
end
RegisterUnitEvent (3639, 6, "Tysha_Moonblade_onSpawn")
RegisterUnitEvent (3639, 3, "Tysha_Moonblade_onSpawn")
RegisterUnitEvent (3639, 2, "Tysha_Moonblade_onSpawn")

function Tysha_Moonblade_onCombat (pUnit, Event)
	pUnit:Emote (26)
end
RegisterUnitEvent (3639, 1, "Tysha_Moonblade_onCombat")

-- Asterion
function Asterion_onSpawn (pUnit, Event)
	pUnit:Emote (68)
	pUnit:SetCombatCapable (0)
end
RegisterUnitEvent (3650, 6, "Asterion_onSpawn")

-- Anaya Dawnrunner
function Anaya_Dawnrunner_onDeath (pUnit, Event)
	pUnit:SendChatMessage (0, 0, "Finally, my soul may rest... Oh, dearest Cerellean...")
end
RegisterUnitEvent (3667, 4, "Anaya_Dawnrunner_onDeath")