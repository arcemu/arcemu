
function Aeranas_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Aeranas_Shock", 10000, 0)
end

function Aeranas_Shock(pUnit,Event)
	pUnit:FullCastSpellOnTarget(12553,pUnit:GetClosestPlayer())
end

function Aeranas_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Aeranas_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end


RegisterUnitEvent (17085, 1, "Aeranas_OnEnterCombat")
RegisterUnitEvent (17085, 2, "Aeranas_OnLeaveCombat")
RegisterUnitEvent (17085, 4, "Aeranas_OnDied")

function Aggonis_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Aggonis_Corruption", 24000, 0)
end

function Aggonis_Corruption(pUnit,Event)
	pUnit:FullCastSpellOnTarget(21068,pUnit:GetClosestPlayer())
end

function Aggonis_OnEnterCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Aggonis_OnEnterCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent (17000, 1, "Aggonis_OnEnterCombat")
RegisterUnitEvent (17000, 2, "Aggonis_OnLeaveCombat")
RegisterUnitEvent (17000, 4, "Aggonis_OnDied")

function Arazzius_OnEnterCombat(pUnit,Event)
	pUnit:CastSpell(34094)
	pUnit:RegisterEvent("Arazzius_FeebleWeapons", 120000, 0)
	pUnit:RegisterEvent("Arazzius_Inferno", 360000, 0)
	pUnit:RegisterEvent("Arazzius_Pyroblast", 24000, 0)
	pUnit:RegisterEvent("Arazzius_ShadowBoltVolley", 5000, 0)
end

function Arazzius_FeebleWeapons(pUnit,Event)
	pUnit:FullCastSpellOnTarget(34088,pUnit:GetClosestPlayer())
end

function Arazzius_Inferno(pUnit,Event)
	pUnit:FullCastSpellOnTarget(34249,pUnit:GetClosestPlayer())
end

function Arazzius_Pyroblast(pUnit,Event)
	pUnit:FullCastSpellOnTarget(33975,pUnit:GetClosestPlayer())
end

function Arazzius_ShadowBoltVolley(pUnit,Event)
	pUnit:FullCastSpellOnTarget(15245,pUnit:GetClosestPlayer())
end

function Arazzius_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Arazzius_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent (19191, 1, "Arazzius_OnEnterCombat")
RegisterUnitEvent (19191, 2, "Arazzius_OnLeaveCombat")
RegisterUnitEvent (19191, 4, "Arazzius_OnDied")

function Torseldori_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Torseldori_ArcaneMissiles", 15000 , 0)
	pUnit:RegisterEvent("Torseldori_Blizzard", 30000, 0)
	pUnit:RegisterEvent("Torseldori_FrostNova", 20000, 0)
	pUnit:RegisterEvent("Torseldori_Frostbolt", 3500, 0)
end

function Torseldori_ArcaneMissiles(pUnit,Event)
	pUnit:FullCastSpellOnTarget(22273,pUnit:GetClosestPlayer())
end

function Torseldori_Blizzard(pUnit,Event)
	pUnit:FullCastSpellOnTarget(33634,pUnit:GetClosestPlayer())
end

function Torseldori_FrostNova(pUnit,Event)
	pUnit:FullCastSpellOnTarget(12674,pUnit:GetClosestPlayer())
end

function Torseldori_Frostbolt(pUnit,Event)
	pUnit:FullCastSpellOnTarget(15530,pUnit:GetClosestPlayer())
end

function Torseldori_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Torseldori_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent (19257, 1, "Torseldori_OnEnterCombat")
RegisterUnitEvent (19257, 2, "Torseldori_OnLeaveCombat")
RegisterUnitEvent (19257, 4, "Torseldori_OnDied")

function Xintor_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Xintor_ArcaneMissiles", 10000 , 0)
	pUnit:RegisterEvent("Xintor_Fireball", 4000, 0)
	pUnit:RegisterEvent("Xintor_IceBarrier", 120000, 0) 
end

function Xintor_ArcaneMissiles(pUnit,Event)
	pUnit:FullCastSpellOnTarget(22273,pUnit:GetClosestPlayer())
end

function Xintor_Frostbolt(pUnit,Event)
	pUnit:FullCastSpellOnTarget(15530,pUnit:GetClosestPlayer())
end

function Xintor_IceBarrier(pUnit,Event)
	pUnit:FullCastSpellOnTarget(33245,pUnit:GetClosestPlayer())
end

function Xintor_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Xintor_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent (16977, 1, "Xintor_OnEnterCombat")
RegisterUnitEvent (16977, 2, "Xintor_OnLeaveCombat")
RegisterUnitEvent (16977, 4, "Xintor_OnDied")


function Arzeth_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Arzeth_MortalStrike", 60000 , 0)
	pUnit:RegisterEvent("Arzeth_ShadowBoltVolley", 10000, 0)
end

function Arzeth_ShadowBoltVolley(pUnit,Event)
	pUnit:FullCastSpellOnTarget(15245,pUnit:GetClosestPlayer())
end

function Arzeth_MortalStrike(pUnit,Event)
	pUnit:FullCastSpellOnTarget(16856,pUnit:GetClosestPlayer())
end

function Arzeth_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Arzeth_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent (19354, 1, "Arzeth_OnEnterCombat")
RegisterUnitEvent (19354, 2, "Arzeth_OnLeaveCombat")
RegisterUnitEvent (19354, 4, "Arzeth_OnDied")

function Avruu_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Avruu_Darkness", 18000, 0)
end

function Avruu_Darkness(pUnit,Event)
	pUnit:FullCastSpellOnTarget(34112,pUnit:GetClosestPlayer())
end

function Avruu_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Avruu_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent (17084, 1, "Avruu_OnEnterCombat")
RegisterUnitEvent (17084, 2, "Avruu_OnLeaveCombat")
RegisterUnitEvent (17084, 4, "Avruu_OnDied")

function BatRiderGuard_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("BatRiderGuard_UnstableConcoction", 22000, 0)
end

function BatRiderGuard_UnstableConcoction(pUnit,Event)
	pUnit:FullCastSpellOnTarget(38066,pUnit:GetClosestPlayer())
end

function BatRiderGuard_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function BatRiderGuard_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent (15242, 1, "BatRiderGuard_OnEnterCombat")
RegisterUnitEvent (15242, 2, "BatRiderGuard_OnLeaveCombat")
RegisterUnitEvent (15242, 4, "BatRiderGuard_OnDied")

function BHDS_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("BHDS_FireNova", 20000, 0)
	pUnit:RegisterEvent("BHDS_Bloodlust", 60000, 0)
	pUnit:RegisterEvent("BHDS_LightningShield", 30000, 0)
end

function BHDS_FireNova(pUnit,Event)
	pUnit:FullCastSpellOnTarget(32062,pUnit:GetClosestPlayer())
end

function BHDS_Bloodlust(pUnit,Event)
	pUnit:CastSpell(6742)
end

function BHDS_LightningShield(pUnit,Event)
	pUnit:CastSpell(12550)
end

function BHDS_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function BHDS_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent (16873, 1, "BHDS_OnEnterCombat")
RegisterUnitEvent (16873, 2, "BHDS_OnLeaveCombat")
RegisterUnitEvent (16873, 4, "BHDS_OnDied")

function BHGrunt_OnEnterCombat(pUnit,Event)
	pUnit:CastSpell(8599)
	pUnit:RegisterEvent("BHGrunt_Strike", 6000, 0)
end

function BHGrunt_Strike(pUnit,Event)
	pUnit:FullCastSpellOnTarget(11976,pUnit:GetClosestPlayer())
end

function BHGrunt_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function BHGrunt_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(16871, 1, "BHGrunt_OnEnterCombat")
RegisterUnitEvent(16871, 2, "BHGrunt_OnLeaveCombat")
RegisterUnitEvent(16871, 4, "BHGrunt_OnDied")

function BHN_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("BHN_Fireball", 7000, 0)
	pUnit:RegisterEvent("BHN_Spell", 240000, 0)
end

function BHN_Spell(pUnit,Event)
	pUnit:FullCastSpellOnTarget(34073,pUnit,Event)
end

function BHN_Fireball(pUnit,Event)
	pUnit:FullCastSpellOnTarget(9053,pUnit:GetClosestPlayer())
end

function BHN_Phase(pUnit,Event)
 if pUnit:GetHealthPct() == 75 then 
	pUnit:CastSpell(34019)
end
end

function BHN_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function BHN_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(19422, 1, "BHN_OnEnterCombat")
RegisterUnitEvent(19422, 1, "BHN_Phase")
RegisterUnitEvent(19422, 2, "BHN_OnLeaveCombat")
RegisterUnitEvent(19422, 4, "BHN_OnDied")

function BHT_OnEnterCombat(pUnit,Event)
	pUnit:CastSpell(34368)
	pUnit:FullCastSpellOnTarget(33924,pUnit:GetRandomPlayer(3))
end

function BHT_Phase1(pUnit,Event)
 if pUnit:GetHealthPct() == 15 then
	pUnit:FullCastSpellOnTarget(31553,pUnit:GetClosestPlayer())

end
end

function BHT_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function BHT_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(19424, 1, "BHT_OnEnterCombat")
RegisterUnitEvent(19424, 1, "BHT_Phase1")
RegisterUnitEvent(19424, 2, "BHT_OnLeaveCombat")
RegisterUnitEvent(19424, 4, "BHT_OnDied")

function BR_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("BR_Spell", 120000, 0)
end

function BR_Spell(pUnit,Event)
	pUnit:FullCastSpellOnTarget(32722,pUnit:GetClosestPlayer())
end

function BR_OnEnterCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function BR_OnEnterCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(16901, 1, "BR_OnEnterCombat")
RegisterUnitEvent(16901, 2, "BR_OnLeaveCombat")
RegisterUnitEvent(16901, 4, "BR_OnDied")

function Bloodmage_OnEnterCombat(punit,Event)
	pUnit:RegisterEvent("Bloodmage_Blizzard", 40000, 0)
	pUnit:RegisterEvent("Bloodmage_FrostNova", 13000, 0)
	pUnit:RegisterEvent("Bloodmage_Frostbolt", 7000, 0)
	pUnit:RegisterEvent("Bloodmage_ArcaneMissle", 8000, 0)
end

function Bloodmage_FrostNova(pUnit,Event)
	pUnit:FullCastSpellOnTarget(12674,pUnit:GetClosestPlayer())
end

function Bloodmage_Frostbolt(pUnit,Event)
	pUnit:FullCastSpellOnTarget(15530,pUnit:GetClosestPlayer())
end

function Bloodmage_Blizzard(pUnit,Event)
	pUnit:FullCastSpellOnTarget(33634,pUnit:GetClosestPlayer())
end

function Bloodmage_ArcaneMissle(pUnit,Event)
 if pUnit:GetHealthPct () == 30 then
	pUnit:FullCastSpellOnTarget(22273,pUnit:GetClosestPlayer())
end
end

function Bloodmage_OnLeaveCombat(punit,Event)
	pUnit:RemoveEvents()
end

function Bloodmage_OnDied(punit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(19258, 1, "Bloodmage_OnEnterCombat")
RegisterUnitEvent(19258, 2, "Bloodmage_OnLeaveCombat")
RegisterUnitEvent(19258, 4, "Bloodmage_OnDied")

function BCE_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("BCE_Fireball", 10000, 0)
end

function BCE_Fireball(pUnit,Event)
	pUnit:FullCastSpellOnTarget(9053,pUnit:GetClosestPlayer())
end

function BCE_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function BCE_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(19701, 1, "BCE_OnEnterCombat")
RegisterUnitEvent(19701, 2, "BCE_OnLeaveCombat")
RegisterUnitEvent(19701, 4, "BCE_OnDied")

function BWM_OnEnterCombat(pUnit,Event)
	pUnit:FullCastSpellOnTarget(34113,pUnit:GetClosestPlayer())
	pUnit:RegisterEvent("BWM_Enrage", 120000, 0)
	pUnit:RegisterEvent("BWM_Bite", 240000, 0)
end

function BWM_Bite(pUnit,Event)
	pUnit:FullCastSpellOnTarget(34113,pUnit:GetClosestPlayer())
end

function BWM_Enrage(pUnit,Event)
	pUnit:FullCastSpellOnTarget(8599,pUnit:GetClosestPlayer())
end

function BWM_OnNearDeath(pUnit,Event)
 if pUnit:GetHealthPct() == 15 then
	pUnit:CastSpell(34114)
end
end

function BWM_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function BWM_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(16876, 1, "BWM_OnEnterCombat")
RegisterUnitEvent(16876, 1, "BWM_OnNearDeath")
RegisterUnitEvent(16876, 2, "BWM_OnLeaveCombat")
RegisterUnitEvent(16876, 4, "BWM_OnDied")

function BCR_OnEnterCombat(pUnit,Event)
	pUnit:CastSpell(32723)
end

RegisterUnitEvent(16925, 1, "BCR_OnEnterCombat")

function BCRW_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("BCRW_Howl", 30000, 0)
	pUnit:RegisterEvent("BCRW_Bite", 120000, 0)
end

function BCRW_Howl(pUnit,Event)
	pUnit:FullCastSpellOnTarget(3149,pUnit:GetClosestPlayer())
end

function BCRW_Bite(pUnit,Event)
	pUnit:FullCastSpellOnTarget(16460,pUnit:GetClosestPlayer())
end

function BCRW_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function BCRW_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(18706, 1, "BCRW_OnEnterCombat")
RegisterUnitEvent(18706, 2, "BCRW_OnLeaveCombat")
RegisterUnitEvent(18706, 4, "BCRW_OnDied")

function Scavenger_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Scavenger_Spell", 5000, 0)
end

function Scavenger_Spell(pUnit,Event)
	pUnit:FullCastSpellOnTarget(13398,pUnit:GetClosestPlayer())
end

function Scavenger_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Scavenger_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(18952, 1, "Scavenger_OnEnterCombat")
RegisterUnitEvent(18952, 2, "Scavenger_OnLeaveCombat")
RegisterUnitEvent(18952, 4, "Scavenger_OnDied")

function Buzzard_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Buzzard_Spell", 5000, 0)
end

function Buzzard_Spell(pUnit,Event)
	pUnit:FullCastSpellOnTarget(37012,pUnit:GetClosestPlayer())
end

function Buzzard_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Buzzard_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
	pUnit:CastSpell(33985)
end

RegisterUnitEvent(16972, 1, "Buzzard_OnEnterCombat")
RegisterUnitEvent(16972, 2, "Buzzard_OnLeaveCombat")
RegisterUnitEvent(16972, 4, "Buzzard_Death")

function CollapsingVoidwalker_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("CollapsingVoidwalker_Collapse", 1000, 1)
	pUnit:RegisterEvent("CollapsingVoidwalker_Fear", 12000, 0)
end

function CollapsingVoidwalker_Collapse(pUnit,Event)
 if pUnit:GetHealthPct() == 10 then
	pUnit:CastSpell(34302)
	pUnit:CastSpell(34233)
	pUnit:CastSpell(34234)
	pUnit:CastSpell(34842)
end
end

function CollapsingVoidwalker_Fear(pUnit,Event)
	pUnit:FullCastSpellOnTarget(34259,pUnit:GetClosestPlayer())
end

function CollapsingVoidwalker_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function CollapsingVoidwalker_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(17014, 1, "CollapsingVoidwalker_OnEnterCombat")
RegisterUnitEvent(17014, 2, "CollapsingVoidwalker_OnLeaveCombat")
RegisterUnitEvent(17014, 4, "CollapsingVoidwalker_OnDied")

function CorporalIronridge_OnSpawn(pUnit,Event)
	pUnit:CastSpell(35998)
end

RegisterUnitEvent(21133, 6, "CorporalIronridge_OnSpawn")

function CrustBurster_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("CrustBurster_Bore", 35000, 0)
	pUnit:RegisterEvent("CrustBurster_Poison", 4000, 0)
	pUnit:RegisterEvent("CrustBurster_Submerged", 50000, 0)
end

function CrustBurster_Bore(pUnit,Event)
	pUnit:FullCastSpellOnTarget(32738,pUnit:GetClosestPlayer())
end

function CrustBurster_Poison(pUnit,Event)
	pUnit:FullCastSpellOnTarget(31747,pUnit:GetClosestPlayer())
end

function CrustBurster_Submerged(pUnit,Event)
	pUnit:RemoveEvents()
	pUnit:CastSpell(37751)
	pUnit:RegsiterEvent("CrustBurster_Stand", 11000, 1)
end

function CrustBurster_Stand(pUnit,Event)
	pUnit:RemoveEvents()
	pUnit:CastSpell(37752)
	pUnit:RegisterEvent("CrustBurster_Bore", 35000, 0)
	pUnit:RegisterEvent("CrustBurster_Poison", 4000, 0)
	pUnit:RegisterEvent("CrustBurster_Submerged", 50000, 0)
end

function CrustBurster_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function CrustBurster_OnDied(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(16844, 1, "CrustBurster_OnEnterCombat")
RegisterUnitEvent(16844, 2, "CrustBurster_OnLeaveCombat")
RegisterUnitEvent(16844, 4, "CrustBurster_OnDied")

function CursedScarab_OnEnterCombat(pUnit,Event)
	pUnit:FullCastSpellOnTarget(3256,pUnit:GetClosestPlayer())
end

RegisterUnitEvent(21306, 1, "CursedScarab_OnEnterCombat")

