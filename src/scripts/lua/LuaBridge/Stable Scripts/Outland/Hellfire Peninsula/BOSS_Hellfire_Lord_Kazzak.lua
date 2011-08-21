local Didthat = 0
function Kazzak_Enrage(Unit, event, miscunit, misc)
	if Unit:GetHealthPct() < 30 and Didthat == 0 then
		Unit:FullCastSpell(34670)
		Didthat = 1
	else
	end
end

function Kazzak_Shadow_Bolt(Unit, event, miscunit, misc)
	print "Kazzak Shadow Bolt"
	Unit:FullCastSpellOnTarget(29924,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Shadow Bolt on you...")
end

function Kazzak_Reflection(Unit, event, miscunit, misc)
	print "Kazzak Reflection"
	Unit:FullCastSpellOnTarget(21063,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Now a reflection on you...")
end

function Kazzak_Kazzak(Unit, event, miscunit, misc)
	print "Kazzak Kazzak"
	Unit:FullCastSpellOnTarget(32960,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Mark Of Kazzak...")
end

function Kazzak_Cleave(Unit, event, miscunit, misc)
	print "Kazzak Cleave"
	Unit:FullCastSpell(31779)
	Unit:SendChatMessage(11, 0, "Cleave...")
end

function Kazzak_Thunderclap(Unit, event, miscunit, misc)
	print "Kazzak Thunderclap"
	Unit:FullCastSpellOnTarget(36706,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Thunderclap...")
end

function Kazzak_Void_Bolt(Unit, event, miscunit, misc)
	print "Kazzak Void Bolt"
	Unit:FullCastSpellOnTarget(39329,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Now a good Void Bolt...")
end

function Kazzak_Soul(Unit, event, miscunit, misc)
	print "Kazzak Soul"
	Unit:FullCastSpell(32966)
	Unit:SendChatMessage(11, 0, "Now you will die...")
end

function Kazzak(unit, event, miscunit, misc)
	print "Kazzak"
	unit:RegisterEvent("Kazzak_Enrage",1000,1)
	unit:RegisterEvent("Kazzak_Shadow_Bolt",10000,0)
	unit:RegisterEvent("Kazzak_Reflection",17000,0)
	unit:RegisterEvent("Kazzak_Kazzak",23000,0)
	unit:RegisterEvent("Kazzak_Cleave",27000,0)
	unit:RegisterEvent("Kazzak_Thunderclap",33000,0)
	unit:RegisterEvent("Kazzak_Void_Bolt",37000,0)
	unit:RegisterEvent("Kazzak_Soul",600000,1)
end

RegisterUnitEvent(18728,1,"Kazzak")