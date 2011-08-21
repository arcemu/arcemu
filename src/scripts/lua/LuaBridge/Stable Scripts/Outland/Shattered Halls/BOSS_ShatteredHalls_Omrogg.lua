function Omrogg_Fear(Unit, event, miscunit, misc)
	print "Omrogg Fear"
	Unit:FullCastSpellOnTarget(33547,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "You are afraid...")
end

function Omrogg_Thunderclap(Unit, event, miscunit, misc)
	print "Omrogg Thunderclap"
	Unit:FullCastSpellOnTarget(30633,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Good hit, i am supprise...")
end

function Omrogg_Blast_Wave(Unit, event, miscunit, misc)
	print "Omrogg Blast Wave"
	Unit:FullCastSpellOnTarget(36278,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Let's wave...do you understand...NOOB...")
end

function Omrogg_Aggro_Switch(Unit)
	print "Omrogg Aggro Switch"
	Unit:GetAddTank()
	Unit:SendChatMessage(11, 0, "I am coming to you...")
end

function Omrogg(unit, event, miscunit, misc)
	print "Omrogg"
	unit:RegisterEvent("Omrogg_Fear",10000,0)
	unit:RegisterEvent("Omrogg_Thunderclap",15000,0)
	unit:RegisterEvent("Omrogg_Blast_Wave",21000,0)
	unit:RegisterEvent("Omrogg_Aggro_Switch",25000,0)
end

RegisterUnitEvent(16809,1,"Omrogg")