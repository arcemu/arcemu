

function Rousch_FriendsTribute(pUnit, event)
    pUnit:Emote(113)
end

function Rousch_OnSpawn(pUnit, event)
     pUnit:SetStandState(8)
	 pUnit:RegisterEvent("Rousch_FriendsTribute",5000, 0)
	 pUnit:RemoveEvents()
end

RegisterUnitEvent(16090, 6, "Rousch_OnSpawn")