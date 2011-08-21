function Nightbane_Cleave(Unit, event, miscunit, misc)
	print "Nightbane Cleave"
	Unit:FullCastSpellOnTarget(42587,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Cleave on you...")
end

function Nightbane_Tail_Sweep(Unit, event, miscunit, misc)
	print "Nightbane Tail Sweep"
	Unit:FullCastSpellOnTarget(25653,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "Take that...")
end

function Nightbane_Bone_Shards(Unit, event, miscunit, misc)
	print "Nightbane Bone Shards"
	Unit:FullCastSpell(17014)
	Unit:SendChatMessage(11, 0, "Bones protect me...")
end

function Nightbane_Distracting_Ash(Unit, event, miscunit, misc)
	print "Nightbane Distracting Ash"
	Unit:FullCastSpellOnTarget(30130,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "Ho, your are distracting...")
end

function Nightbane_Bellowing_Roar(Unit, event, miscunit, misc)
	print "Nightbane Bellowing Roar"
	Unit:FullCastSpellOnTarget(37066,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "FEAR on you guys...")
end

function Nightbane_Charred_Earth(Unit, event, miscunit, misc)
	print "Nightbane Charred Earth"
	Unit:FullCastSpellOnTarget(30129,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "FIRE...")
end

function Nightbane_Smoldering_Breath(Unit, event, miscunit, misc)
	print "Nightbane Smoldering Breath"
	Unit:FullCastSpellOnTarget(39385,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "A Good breath...")
end

function Nightbane(unit, event, miscunit, misc)
	print "Nightbane"
	unit:RegisterEvent("Nightbane_Cleave",10000,0)
	unit:RegisterEvent("Nightbane_Tail_Sweep",13000,0)
	unit:RegisterEvent("Nightbane_Bone_Shards",17000,0)
	unit:RegisterEvent("Nightbane_Distracting_Ash",23000,0)
	unit:RegisterEvent("Nightbane_Bellowing_Roar",31000,0)
	unit:RegisterEvent("Nightbane_Charred_Earth",37000,0)
	unit:RegisterEvent("Nightbane_Smoldering_Breath",43000,0)
end

RegisterUnitEvent(17225,1,"Nightbane")