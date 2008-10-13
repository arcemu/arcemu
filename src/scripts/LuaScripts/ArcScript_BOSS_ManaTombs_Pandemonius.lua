
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


function ArcscriptPandemonius_Void_Blast(Unit, event, miscunit, misc)
	print "Pandemonius Void Blast"
	Unit:FullCastSpellOnTarget(32325,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "A good Void Blast...")
end

function ArcscriptPandemonius_Dark_Shell(Unit, event, miscunit, misc)
	print "Pandemonius Dark Shell"
	Unit:FullCastSpellOnTarget(32358,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Good move...For me...srry...")
end

function ArcscriptPandemonius(unit, event, miscunit, misc)
	print "Pandemonius"
	unit:RegisterEvent("ArcscriptPandemonius_Void_Blast",9000,0)
	unit:RegisterEvent("ArcscriptPandemonius_Dark_Shell",13000,0)
end

RegisterUnitEvent(18341,1,"ArcscriptPandemonius")