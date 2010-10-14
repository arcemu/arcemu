function Omor_Shadow_Bolt(Unit, event, miscunit, misc)
     print "Omor Shadow Bolt"
     Unit:FullCastSpellOnTarget(36987,Unit:GetClosestPlayer())
     Unit:SendChatMessage(11, 0, "A Good Shadow Bolt for you...")
end

function Omor_Spell_reflect(Unit)
     print "Omor Spell reflect"
     Unit:FullCastSpell(33961)
     Unit:SendChatMessage(11, 0, "I Will reflect all your spell's...")
end

function Omor_Felhounds(Unit)
     print "Omor Felhounds"
     Unit:SpawnCreature(18478, -1168.601, 1699.915, 91.477, 0, 18, 96000000);
     Unit:SpawnCreature(18478, -1160.796, 1689.295, 91.646, 0, 18, 96000000);
     Unit:SendChatMessage(11, 0, "Come help me my child...")
end

function Omor_Shadow_Whip(Unit, event, miscunit, misc)
     print "Omor Shadow Whip"
     Unit:FullCastSpellOnTarget(30638,Unit:GetRandomPlayer())
     Unit:SendChatMessage(11, 0, "Now it's your turn...")
end

function Omor_Treacherous_Aura(Unit, event, miscunit, misc)
     print "Omor Treacherous Aura"
     Unit:FullCastSpellOnTarget(30695,Unit:GetRandomPlayer())
     Unit:SendChatMessage(11, 0, "I choose you, you will die...")
end

function Omor_Bane_Treachery(Unit, event, miscunit, misc)
     print "Omor Bane Treachery"
     Unit:FullCastSpellOnTarget(37566,Unit:GetRandomPlayer())
     Unit:SendChatMessage(11, 0, "Let's do more dammage on you...")
end

function Omor(unit, event, miscunit, misc)
     print "Omor"
     unit:RegisterEvent("Omor_Shadow_Bolt",7000,0)
     unit:RegisterEvent("Omor_Spell_reflect",13000,0)
     unit:RegisterEvent("Omor_Felhounds",19000,0)
     unit:RegisterEvent("Omor_Shadow_Whip",27000,0)
     unit:RegisterEvent("Omor_Treacherous_Aura",33000,0)
     unit:RegisterEvent("Omor_Bane_Treachery",33000,0)
end

RegisterUnitEvent(17308,1,"Omor")