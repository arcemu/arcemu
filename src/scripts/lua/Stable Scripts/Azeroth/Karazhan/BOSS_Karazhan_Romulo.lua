function Romulo_Backward_Lunge(Unit, event, miscunit, misc)
	print "Romulo Backward Lunge"
	Unit:FullCastSpellOnTarget(30815,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "I choose you...")
end

function Romulo_Deadly_Swathe(Unit, event, miscunit, misc)
	print "Romulo Deadly Swathe"
	Unit:FullCastSpell(30817)
	Unit:SendChatMessage(11, 0, "You will pay...")
end

function Romulo_Daring(Unit, event, miscunit, misc)
	print "Romulo Daring"
	Unit:FullCastSpell(30841)
	Unit:SendChatMessage(11, 0, "Now it's my turn...")
end

function Romulo_Poisoned_Thrust(Unit, event, miscunit, misc)
	print "Romulo Poisoned Thrust"
	Unit:FullCastSpell(30822)
	Unit:SendChatMessage(11, 0, "Now, let's do more damage...")
end

function Romulo(unit, event, miscunit, misc)
	print "Romulo"
	unit:RegisterEvent("Romulo_Backward_Lunge",10000,0)
	unit:RegisterEvent("Romulo_Deadly_Swathe",15000,0)
	unit:RegisterEvent("Romulo_Daring",21000,0)
	unit:RegisterEvent("Romulo_Poisoned_Thrust",60000,0)
end

RegisterUnitEvent(17533,1,"Romulo")