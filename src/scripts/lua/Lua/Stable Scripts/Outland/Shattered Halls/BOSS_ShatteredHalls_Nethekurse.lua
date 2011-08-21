local Didthat = 0

function Nethekurse_Dark_Spin(Unit)
	if Unit:GetHealthPct() < 20 and Didthat == 0 then
		print "Nethekurse Dark Spin"
		Unit:FullCastSpellOnTarget(30502,Unit:GetClosestPlayer())
		Unit:SendChatMessage(11, 0, "Now, you will die...")
		Didthat = 1
	else
	end
end

function Nethekurse_Void_Zones(Unit, event, miscunit, misc)
	print "Nethekurse Void Zones"
	Unit:FullCastSpellOnTarget(30533,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "Don't stay on the zone...")
end

function Nethekurse_Burning_Nova(Unit, event, miscunit, misc)
	print "Nethekurse Burning Nova"
	Unit:FullCastSpellOnTarget(30500,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "Adios...NOOB...")
end

function Nethekurse(unit, event, miscunit, misc)
	print "Nethekurse"
	unit:RegisterEvent("Nethekurse_Dark_Spin",1000,0)
	unit:RegisterEvent("Nethekurse_Void_Zones",10000,0)
	unit:RegisterEvent("Nethekurse_Burning_Nova",21000,0)
end

RegisterUnitEvent(16807,1,"Nethekurse")