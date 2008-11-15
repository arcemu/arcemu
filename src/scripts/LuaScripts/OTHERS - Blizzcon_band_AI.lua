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


function spellflare(unit)
unit:CastSpell(42505) 
end

function Banddespawn(unit, Event)
unit:Delete()
end



function despawnband(unit, Event)
unit:RegisterEvent(500, spellflare, 1)
unit:RemoveEvents() 
unit:Despawn(1000,2000000)  
end

function despawnband2(unit, Event)
unit:RegisterEvent(500, spellflare, 2)
unit:RemoveEvents() 
unit:Despawn(1000,2000000)
end


function spellfire(unit)
unit:CastSpell(42501)
end

function UDemotes(unit)
unit:RegisterEvent(500, spellflare, 2)
unit:RegisterEvent(280000, despawnband, 0)
unit:RegisterEvent(10000, EmoteSpinGuiUD, 1)
unit:RegisterEvent(30000, EmoteStrumGuiUD, 1)
unit:RegisterEvent(34000, EmoteStrumGuiUD, 1)
unit:RegisterEvent(38000, EmoteSpinGuiUD, 1)
unit:RegisterEvent(45000, Normalstate, 1)
unit:RegisterEvent(104000, EmoteJumpGuiUD, 1)
unit:RegisterEvent(70000, EmoteStepGuiUD, 1)
unit:RegisterEvent(85000, Normalstate, 1)
unit:RegisterEvent(123000, EmoteJumpGuiUD, 1)
unit:RegisterEvent(140000, Spotlight, 1)
unit:RegisterEvent(145000, EmoteSpinGuiUD, 1) 
unit:RegisterEvent(168000, EmoteJumpGuiUD, 1)
unit:RegisterEvent(200000, Normalstate, 1)
unit:RegisterEvent(229000, EmoteStepGuiUD, 1)
unit:RegisterEvent(203000, Normalstate, 1)
end

function EmoteStrumGuiUD(unit)
unit:Emote(405)
end

function EmoteStepGuiUD(unit)
unit:Emote(403)
end

function EmoteSpinGuiUD(unit)
unit:Emote(404)
end
function EmoteJumpGuiUD(unit)
unit:Emote(402)
end

function Drumemotes(unit)
unit:RegisterEvent(500, spellflare, 2)
unit:RegisterEvent(280000, despawnband, 0)
unit:SetCombatCapable(0)
unit:RegisterEvent(30000, Emotetwodrum, 1)
unit:RegisterEvent(34000, Emotetwodrum, 1)
unit:RegisterEvent(38000, Emotetwodrum, 1)
unit:RegisterEvent(45000, Normalstate, 1)
unit:RegisterEvent(58000, Emotetwodrum, 1)
unit:RegisterEvent(68000, Emotejumpdrum, 1)
unit:RegisterEvent(88000, Emoterolldrum, 1)
unit:RegisterEvent(100000, Emotejumpdrum, 1)
unit:RegisterEvent(138000, Emotetwodrum, 1)
unit:RegisterEvent(210000, Emoterolldrum, 1)
unit:RegisterEvent(203000, Normalstate, 1)
end


function Emotejumpdrum(unit)
unit:Emote(405)
end

function Emoterolldrum(unit)
unit:Emote(402)
end

function Emotetwodrum(unit)
unit:Emote(403)
end

function Emotespindrum(unit)
unit:Emote(404)
end

function Orcemotes(unit)
unit:RegisterEvent(500, spellflare, 2)
unit:RegisterEvent(280000, despawnband, 0)
unit:SetCombatCapable(0)
unit:RegisterEvent(0500, EmoteRight, 1)
unit:RegisterEvent(19000, EmoteGrabclose, 1)
unit:RegisterEvent(30000, Emoteleft, 1)
unit:RegisterEvent(45000, Normalstate, 1)
unit:RegisterEvent(65000, EmoteGrab, 1)
unit:RegisterEvent(70000, Emoteleft, 1)
unit:RegisterEvent(84000, EmoteScream, 1)
unit:RegisterEvent(87000, Normalstate, 1)
unit:RegisterEvent(115000, OrcLight, 1)
unit:RegisterEvent(123042, Emoteleft, 1)
unit:RegisterEvent(137000, EmoteScream, 1)
unit:RegisterEvent(142000, Emotestop, 1)
unit:RegisterEvent(180000, Emotestop, 1)
unit:RegisterEvent(229000, EmoteRight, 1)
unit:RegisterEvent(239000, EmoteGrab, 1)
unit:RegisterEvent(259000, EmoteScream, 1)
end

function EmoteRight(unit)
unit:Emote(403)
end

function Emoteleft(unit)
unit:Emote(404)
end

function EmoteScream(unit)
unit:Emote(407)
end

function EmoteGrab(unit)
unit:Emote(405)
end

function Emotestop(unit)
unit:Emote(402)
end

function EmoteGrabclose(unit)
unit:Emote(406)
end

function OrcLight(unit)
unit:CastSpell(42510)
end

function Trollemotes(unit)
unit:RegisterEvent(500, spellflare, 2)
unit:SetCombatCapable(0)
unit:RegisterEvent(10000, EmoteStrumbass, 1)
unit:RegisterEvent(30000, EmoteStrumbass, 1)
unit:RegisterEvent(45000, Normalstate, 1)
unit:RegisterEvent(70350, EmoteStepbass, 1)
unit:RegisterEvent(85000, Normalstate, 1)
unit:RegisterEvent(95000, EmoteJumpbass, 1)
unit:RegisterEvent(102000, EmoteStrumbass, 1) 
unit:RegisterEvent(115000, EmoteStepbass, 1)
unit:RegisterEvent(123500, EmoteStepbass, 1)
unit:RegisterEvent(165000, EmoteStepbass, 1)
unit:RegisterEvent(192300, EmoteJumpbass, 1)
unit:RegisterEvent(203000, Normalstate, 1)
unit:RegisterEvent(229550, EmoteStepbass, 1)
unit:RegisterEvent(280000, despawnband, 1)
end


function EmoteStrumbass(unit)
unit:Emote(404)
end

function EmoteJumpbass(unit)
unit:Emote(403)
end

function EmoteStepbass(unit)
unit:Emote(402)
end

function BEmotes(unit)
unit:RegisterEvent(500, spellflare, 2)
unit:RegisterEvent(280000, despawnband, 0)
unit:RegisterEvent(10000, EmoteStrumGui, 1)
unit:RegisterEvent(30000, EmoteStrumGui, 1)
unit:RegisterEvent(34000, EmoteStrumGui, 1)
unit:RegisterEvent(38000, EmoteStrumGui, 1)
unit:RegisterEvent(45000, Normalstate, 1)
unit:RegisterEvent(70061, EmoteStrumGui, 1)
unit:RegisterEvent(85000, Normalstate, 1)
unit:RegisterEvent(104000, EmotekickGui, 1)
unit:RegisterEvent(123042, EmoteStrumGui, 1)
unit:RegisterEvent(140000, EmoteStrumGui, 1)
unit:RegisterEvent(166000, EmotekickGui, 1)
unit:RegisterEvent(168000, EmotepluckGui, 1)
unit:RegisterEvent(180000, EmotepluckGui, 1)
unit:RegisterEvent(193000, EmoteSpinGui, 1)
unit:RegisterEvent(203000, Normalstate, 1)
unit:RegisterEvent(229240, EmoteStrumGui, 1)
unit:RegisterEvent(257000, Normalstate, 1)
unit:RegisterEvent(259000, EmoteSpinGui, 1)
end


function EmoteStrumGui(unit)
unit:Emote(402)
end

function EmotepluckGui(unit)
unit:Emote(404)
end

function EmotekickGui(unit)
unit:Emote(403)
end

function EmoteSpinGui(unit)
unit:Emote(405)
end

function Normalstate(unit)
unit:Emote(401)
end

function Spotlight(unit)
unit:CastSpell(39312)
end




function spellearth(unit)
unit:CastSpell(42499)
unit:RegisterEvent(2000, spellfire, 1)
end

function spelllightning(unit)

unit:CastSpell(42510)
unit:CastSpell(42507)
end
 
function spellstorm(unit)
unit:CastSpell(42500)
unit:RegisterEvent(1000, spelllightning, 1)
unit:RegisterEvent(4000, spellearth, 1)
end

function Onspawnband4(unit)
unit:PlaySoundToSet(11803) 
unit:SetCombatCapable(0)
unit:RegisterEvent(1000, spellstorm, 1)  
unit:RegisterEvent(70000, spellstorm, 1)  
unit:RegisterEvent(123000, spellstorm, 1) 
unit:RegisterEvent(230000, spellstorm, 1)  
unit:RegisterEvent(243000, spellstorm, 1) 
unit:RegisterEvent(280000, despawnband, 0)  
end


RegisterUnitEvent(23624, 6, "Trollemotes") 
RegisterUnitEvent(23619, 6, "UDemotes")  
RegisterUnitEvent(23626, 6, "BEmotes")
RegisterUnitEvent(23625, 6, "Orcemotes")
RegisterUnitEvent(23623, 6, "Drumemotes")
RegisterUnitEvent(49000, 6, "Onspawnband4")