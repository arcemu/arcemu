function Thespia_Cloud(Unit, event, miscunit, misc)
	print "Thespia Cloud"
	Unit:FullCastSpellOnTarget(25033,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "This cloud will kill you...")
end

function Thespia_Winds(Unit, event, miscunit, misc)
	print "Thespia Winds"
	Unit:FullCastSpellOnTarget(31718,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "Let the wind immobilize you...")
end

function Thespia_Burst(Unit, event, miscunit, misc)
	print "Thespia Burst"
	Unit:FullCastSpellOnTarget(31481,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "A good burst for you...")
end

function Thespia(unit, event, miscunit, misc)
	print "Thespia"
	unit:RegisterEvent("Thespia_Cloud",8000,0)
	unit:RegisterEvent("Thespia_Winds",10000,0)
	unit:RegisterEvent("Thespia_Burst",13000,0)
end

RegisterUnitEvent(17797,1,"Thespia")