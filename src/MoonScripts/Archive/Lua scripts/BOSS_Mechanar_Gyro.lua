function gyrokill_shadow_power_gkill(Unit, event, miscunit, misc)
	print "gyrokill shadow power gkill"
	Unit:FullCastSpellOnTarget(35322,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "NOOB...")
end

function gyrokill_steam_of_mahcine_flid_gkill(Unit, event, miscunit, misc)
	print "gyrokill steam of mahcine flid gkill"
	Unit:FullCastSpellOnTarget(35311,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "NOOB...")
end

function gyrokill_saw_blade(Unit)
	print "gyrokill saw blade"
	Unit:FullCastSpell(35318)
	Unit:SendChatMessage(11, 0, "If my devision is correct, you should be quiet dead!")
end

function gyrokill(unit, event, miscunit, misc)
	print "gyrokill"
	unit:RegisterEvent("gyrokill_shadow_power_gkill",20000,0)
	unit:RegisterEvent("gyrokill_steam_of_mahcine_flid_gkill",25000,0)
	unit:RegisterEvent("gyrokill_saw_blade",30000,0)
end

RegisterUnitEvent(19218,1,"gyrokill")