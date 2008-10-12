
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

function ArcScriptTavarok_Earthquake(Unit, event, miscunit, misc)
	print "Tavarok Earthquake"
	Unit:FullCastSpellOnTarget(33919,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Do you feel my Earthquake...")
end

function ArcScriptTavarok_Crystal_Poison(Unit, event, miscunit, misc)
	print "Tavarok Crystal Poison"
	Unit:FullCastSpellOnTarget(32361,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "Some poison...")
end

function ArcScriptTavarok(unit, event, miscunit, misc)
	print "Tavarok"
	unit:RegisterEvent("Tavarok_Earthquake",10000,0)
	unit:RegisterEvent("Tavarok_Crystal_Poison",13000,0)
end

RegisterUnitEvent(18343,1,"ArcScriptTavarok")