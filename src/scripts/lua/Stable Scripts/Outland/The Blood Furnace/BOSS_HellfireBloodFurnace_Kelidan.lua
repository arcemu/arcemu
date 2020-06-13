local CHAT_MSG_MONSTER_YELL = 14
local LANG_UNIVERSAL = 0
function Kelidan_Shadow_Bolt_Volley(Unit)
	print "Kelidan Shadow Bolt Volley"
	if Unit:GetClosestPlayer() ~= nil then
		Unit:FullCastSpellOnTarget(36736,Unit:GetClosestPlayer())
		Unit:SendChatMessage(CHAT_MSG_MONSTER_YELL,LANG_UNIVERSAL, "Rain of shadow Bolt...")
	end
end

function Kelidan_Burning_Nova(Unit)
	print "Kelidan Burning Nova"
	if Unit:GetClosestPlayer() ~= nil then
		Unit:FullCastSpellOnTarget(20229,Unit:GetClosestPlayer())
		Unit:SendChatMessage(CHAT_MSG_MONSTER_YELL,LANG_UNIVERSAL, "Fire...Fire...on you...")
	end
end
function Kelidan_OnWipe(Unit)
	Unit:RemoveEvents()
end
function Kelidan_OnCombat(Unit)
	print "Kelidan"
	Unit:RegisterEvent("Kelidan_Shadow_Bolt_Volley",9000,0)
	Unit:RegisterEvent("Kelidan_Burning_Nova",14000,0)
end
RegisterUnitEvent(17377,1,"Kelidan_OnCombat")
RegisterUnitEvent(17377,2,"Kelidan_OnWipe")
RegisterUnitEvent(17377,4,"Kelidan_OnWipe")