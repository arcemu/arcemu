function Betrayer_Healing(Unit)
	print "Betrayer Healing"
	Unit:FullCastSpell(34977)
	Unit:SendChatMessage(11, 0, "Some heal...")
end

function Betrayer_Earth(Unit, event, miscunit, misc)
	print "Betrayer Earth"
	Unit:FullCastSpellOnTarget(31981,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Totem...")
end

function Betrayer_Stoneskin(Unit, event, miscunit, misc)
	print "Betrayer Stoneskin"
	Unit:FullCastSpellOnTarget(31985,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "Totem...")
end

function Betrayer_Fire_Totem(Unit, event, miscunit, misc)
	print "Betrayer Fire Totem"
	Unit:FullCastSpellOnTarget(23419,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Totem fire...Good...")
end

function Betrayer_Light_Bolt(Unit, event, miscunit, misc)
	print "Betrayer Light Bolt"
	Unit:FullCastSpellOnTarget(36152,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Catch that...")
end

function Betrayer(unit, event, miscunit, misc)
	print "Betrayer"
	unit:RegisterEvent("Betrayer_Healing",5000,0)
	unit:RegisterEvent("Betrayer_Earth",7000,0)
	unit:RegisterEvent("Betrayer_Stoneskin",8000,0)
	unit:RegisterEvent("Betrayer_Fire_Totem",10000,0)
	unit:RegisterEvent("Betrayer_Light_Bolt",11000,0)
end

RegisterUnitEvent(17941,1,"Betrayer")