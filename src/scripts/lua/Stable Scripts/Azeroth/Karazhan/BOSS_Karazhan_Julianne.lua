function Julianne_Eternal_Affection(Unit, event, miscunit, misc)
	print "Julianne Eternal Affection"
	Unit:FullCastSpell(30878)
	Unit:SendChatMessage(11, 0, "HEAL...")
end

function Julianne_Powerful_Attraction(Unit, event, miscunit, misc)
	print "Julianne Powerful Attraction"
	Unit:FullCastSpellOnTarget(30889,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "You love me...HOooo...")
end

function Julianne_Blinding_Passion(Unit, event, miscunit, misc)
	print "Julianne Blinding Passion"
	Unit:FullCastSpellOnTarget(30890,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Some passion...")
end

function Julianne_Devotion(Unit, event, miscunit, misc)
	print "Julianne Devotion"
	Unit:FullCastSpell(30887)
	Unit:SendChatMessage(11, 0, "I will kill you...")
end

function Julianne(unit, event, miscunit, misc)
	print "Julianne"
	unit:RegisterEvent("Julianne_Eternal_Affection",10000,0)
	unit:RegisterEvent("Julianne_Powerful_Attraction",15000,0)
	unit:RegisterEvent("Julianne_Blinding_Passion",20000,0)
	unit:RegisterEvent("Julianne_Devotion",25000,0)
end

RegisterUnitEvent(117534,1,"Julianne")