function Sulfuron_FireBolt(Unit, event, miscunit, misc)
	print "Sulfuron FireBolt"
	Unit:FullCastSpellOnTarget(23331,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "This wave will send you in ValHalla...")
end

function Sulfuron_FireShield(Unit, event, miscunit, misc)
	print "Sulfuron FireShield"
	Unit:FullCastSpellOnTarget(24573,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "My strike will crash your head...")
end

function Sulfuron_Charge(Unit, event, miscunit, misc)
	print "Sulfuron Charge"
	Unit:FullCastSpellOnTarget(36058,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "I will send you over the wall...")
end

function Sulfuron(unit, event, miscunit, misc)
	print "Sulfuron"
	unit:RegisterEvent("Sulfuron_FireBolt",9000,0)
	unit:RegisterEvent("Sulfuron_FireShield",13000,0)
	unit:RegisterEvent("Sulfuron_Charge",21000,0)
end

RegisterUnitEvent(20909,1,"Sulfuron")