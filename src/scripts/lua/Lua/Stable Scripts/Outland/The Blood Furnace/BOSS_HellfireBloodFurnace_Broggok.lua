local CHAT_MSG_MONSTER_YELL = 14;
local LANG_UNIVERSAL = 0;

function Broggok_Ring_Poison(Unit)
	print "Broggok Ring Poison"
	if Unit:GetClosestPlayer() ~= nil then 
		Unit:FullCastSpellOnTarget(29865,Unit:GetClosestPlayer())
		Unit:SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Ring of Poison...")
	end
	Unit:RegisterEvent("Broggok_Ring_Poison",9000,1)
end
function Broggok_Poison_Bolt(Unit)
	print "Broggok Poison Bolt"
	if Unit:GetClosestPlayer() ~= nil then
		Unit:FullCastSpellOnTarget(37862,Unit:GetClosestPlayer())
		Unit:SendChatMessage(CHAT_MSG_MONSTER_YELL,LANG_UNIVERSAL, "Rain of Poison Bolt...")
	end
	Unit:RegisterEvent("Broggok_Poison_Bolt",17000,1)
end
function Broggok_OnCombat(Unit)
	print "Broggok"
	Unit:RegisterEvent("Broggok_Ring_Poison",9000,1)
	Unit:RegisterEvent("Broggok_Poison_Bolt",17000,1)
end
function Broggok_OnWipe(Unit)
	Unit:RemoveEvents()
end
RegisterUnitEvent(17380,4,"Broggok_OnWipe")
RegisterUnitEvent(17380,2,"Broggok_OnWipe")
RegisterUnitEvent(17380,1,"Broggok_OnCombat")