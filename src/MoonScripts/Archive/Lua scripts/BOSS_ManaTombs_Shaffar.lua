function Shaffar_Blink(Unit)
	print "Shaffar_Blink"
	Unit:FullCastSpell(14514)
	Unit:SendChatMessage(11, 0, "Try to catch me...")
end

function Shaffar_Frost_Nova(Unit, event, miscunit, misc)
	print "Shaffar Frost Nova"
	Unit:FullCastSpellOnTarget(39035,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "I will turn you in a ice cube...")
end

function Shaffar_sapwn(Unit)
	print "Shaffar sapwn"
	Unit:SpawnCreature(18431, -212.7, -0.3, 16.9, 0, 18, 36000000);
	Unit:SendChatMessage(11, 0, "Come my child and help me...")
end

function Shaffar_FrostBolt(Unit, event, miscunit, misc)
	print "Shaffar FrostBolt"
	Unit:FullCastSpellOnTarget(40429,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Do you want a snow ball...")
end

function Shaffar_Fireball(Unit, event, miscunit, misc)
	print "Shaffar Fireball"
	Unit:FullCastSpellOnTarget(20420,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "I will give some heat...")
end

function Shaffar(unit, event, miscunit, misc)
	print "Shaffar"
	unit:RegisterEvent("Shaffar_Blink",10000,0)
	unit:RegisterEvent("Shaffar_Frost_Nova",13000,0)
	unit:RegisterEvent("Shaffar_sapwn",17000,0)
	unit:RegisterEvent("Shaffar_FrostBolt",23000,0)
	unit:RegisterEvent("Shaffar_Fireball",27000,0)
end

RegisterUnitEvent(18344,1,"Shaffar")