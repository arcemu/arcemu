function Hyakiss_Hyakiss_Web(Unit, event, miscunit, misc)
	print "Hyakiss Hyakiss Web"
	Unit:FullCastSpellOnTarget(29896,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "I will show you my Web design...")
end

function Hyakiss_Acidic_Wound(Unit, event, miscunit, misc)
	print "Hyakiss Acidic Wound"
	Unit:FullCastSpellOnTarget(40481,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "HO, a bad wound...")
end
function Hyakiss(unit, event, miscunit, misc)
	print "Hyakiss"
	unit:RegisterEvent("Hyakiss_Hyakiss_Web",15000,0)
	unit:RegisterEvent("Hyakiss_Acidic_Wound",25000,0)
end

RegisterUnitEvent(16179,1,"Hyakiss")