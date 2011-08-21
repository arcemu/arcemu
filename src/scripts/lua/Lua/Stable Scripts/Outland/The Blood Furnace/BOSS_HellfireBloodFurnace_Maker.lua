function Maker_Domination(Unit)
	print "Maker_Domination"
	if Unit:GetRandomPlayer(0) ~= nil then
		Unit:FullCastSpellOnTarget(30923,Unit:GetRandomPlayer(0))
		Unit:SendChatMessage(CHAT_MSG_MONSTER_YELL,LANG_UNIVERSAL, "I will dominate you...")
	end
end

function Maker_Acid_Spray(Unit)
	print "Maker Acid Spray"
	if Unit:GetClosestPlayer() ~= nil then
		Unit:FullCastSpellOnTarget(38973,Unit:GetClosestPlayer())
		Unit:SendChatMessage(CHAT_MSG_MONSTER_YELL,LANG_UNIVERSAL, "Acid Spray...on you...")
	end
end

function Maker_Trow_Beaker(Unit)
	print "Maker_Trow_Beaker"
	if Unit:GetClosestPlayer() ~= nil then
		Unit:FullCastSpellOnTarget(30925,Unit:GetClosestPlayer())
		Unit:SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Catch that...")
	end
end

function Maker(Unit)
	print "Maker"
	Unit:RegisterEvent("Maker_Domination",10000,0)
	Unit:RegisterEvent("Maker_Acid_Spray",14000,0)
	Unit:RegisterEvent("Maker_Trow_Beaker",21000,0)
end

RegisterUnitEvent(17381,1,"Maker")