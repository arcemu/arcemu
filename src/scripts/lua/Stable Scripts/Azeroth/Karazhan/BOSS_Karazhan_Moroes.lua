function Moroes_Enrage(Unit, event, miscunit, misc)
	if Unit:GetHealthPct() < 30 and Didthat == 0 then
		Unit:FullCastSpell(44779)
		Didthat = 1
	else
	end
end

function Moroes_Gouge(Unit, event, miscunit, misc)
	print "Moroes Gouge"
	Unit:FullCastSpellOnTarget(28456,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Gouge on you...")
end

function Moroes_Blind(Unit, event, miscunit, misc)
	print "Moroes Blind"
	Unit:FullCastSpellOnTarget(34654,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Ho, your blind...")
end

function Moroes_Vanish(Unit, event, miscunit, misc)
	print "Moroes Vanish"
	Unit:FullCastSpell(41476)
	Unit:SendChatMessage(11, 0, "You dont see me anymore...")
end

function Moroes_Garrote(Unit, event, miscunit, misc)
	print "Moroes Garrote"
	Unit:FullCastSpellOnTarget(37066,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "I choose you...")
end

function Moroes(unit, event, miscunit, misc)
	print "Moroes"
	unit:RegisterEvent("Moroes_Enrage",1000,1)
	unit:RegisterEvent("Moroes_Gouge",15000,0)
	unit:RegisterEvent("Moroes_Blind",20000,0)
	unit:RegisterEvent("Moroes_Vanish",60000,0)
	unit:RegisterEvent("Moroes_Garrote",75000,0)
end

RegisterUnitEvent(15687,1,"Moroes")