--  [[ Coilfang Ambusher ]] --

function CoilfangAmbush_Multishot(Unit)
	local plr = Unit:GetRandomPlayer(0)
	if (plr ~= nil) then
		Unit:FullCastSpellOnTarget(27021,plr)
	end
end

function CoilfangAmbush_OnEnterCombat(Unit)
	Unit:RegisterEvent("CoilfangAmbush_Multishot",4500,99)
end
RegisterUnitEvent(21865, 1, "CoilfangAmbush_OnEnterCombat")

-- [[ Coilfang Fathom Witch ]] --

function CoilWitch_ShadowBolt(Unit)
	local plr = Unit:GetClosestPlayer()
	if (plr ~= nil) then
		Unit:FullCastSpellOnTarget(27209,plr)
	end
end

function CoilWitch_Knockback(Unit)
	local plr = Unit:GetRandomPlayer(1)
	if (plr ~= nil) then
		Unit:FullCastSpellOnTarget(34109,plr)
	end
end

function CoilWitch_OnEnterCombat(Unit)
	Unit:RegisterEvent("CoilWitch_ShadowBolt",15000,99)
	Unit:RegisterEvent("CoilWitch_Knockback",60000,99)
end

RegisterUnitEvent(21299, 1, "CoilWitch_OnEnterCombat")

-- [[ CoilFang Guardian ]] --

function CoilGuard_Cleave(Unit)
	Unit:FullCastSpellOnTarget(38260,Unit:GetClosestPlayer())
end

function CoilGuard_OnEnterCombat(Unit)
	Unit:RegisterEvent("CoilGuard_Cleave",40000,99)
end
RegisterUnitEvent(21873, 1, "CoilGuard_OnEnterCombat")

-- [[ Coilfang Priestess ]] --

function CoilPriest_Holynova(Unit)
	local plr = Unit:GetRandomPlayer(0)
	if (plr ~= nil) then
		Unit:FullCastSpellOnTarget(38589,plr)
	end
end

function CoilPriest_Smite(Unit)
	if Unit:GetClosetsPlayer() then
		Unit:FullCastSpellOnTarget(25364,Unit:GetClosestPlayer())
	end
end

function CoilPriest_OnEnterCombat(Unit)
	Unit:RegisterEvent("CoilPriest_Holynova",35000,10)
	Unit:RegisterEvent("CoilPriest_Smite",5000,0)
end

RegisterUnitEvent(21220, 1, "CoilPriest_OnEnterCombat")


-- [[ Coilfang Serpent Guard ]] --
function CoilSert_SpellReflect(Unit)
	Unit:FullCastSpell(36096)
end

function CoilSert_Cleave(Unit)
	if Unit:GetClosestPlayer() then
		Unit:FullCastSpellOnTarget(38260,Unit:GetClosestPlayer())
	end
end

function CoilSert_OnEnterCombat(Unit)
	Unit:RegisterEvent("CoilSert_SpellReflect",15000,10)
	Unit:RegisterEvent("CoilSert_Cleave",30000,0)
end

RegisterUnitEvent(21298, 1, "CoilSert_OnEnterCombat")

-- [[ Coilfang Shatterer ]] --

function CoilShat_ShatterArmor(Unit)
	if Unit:GetClosestPlayer() then
		Unit:FullCastSpellOnTarget(38591,Unit:GetClosestPlayer())
	end
end

function CoilShat_OnEnterCombat(Unit)
	Unit:RegisterEvent("CoilShat_ShatterArmor",30000,0)
end

RegisterUnitEvent(21301, 1, "CoilShat_OnEnterCombat")

-- [[ Coilfang Strider ]] --

function CoilStride_PsychicScream(Unit)
	Unit:FullCastSpell(36096)
end

function CoilStride_OnEnterCombat(Unit)
	Unit:RegisterEvent("CoilStride_PsychicScream",40000,2)
end

RegisterUnitEvent(22056, 1, "CoilStride_OnEnterCombat")

-- [[ Fathom Guard Caribdis ]] --

function FathGuardCarb_WaterBolt(Unit)
	if Unit:GetClosestPlayer() then
		Unit:FullCastSpellOnTarget(38335,Unit:GetClosestPlayer())
	end
end

function FathGuardCarb_Heal(Unit)
	Unit:FullCastSpell(6064)
end

function FathGuardCarb_OnEnterCombat(Unit)
	Unit:RegisterEvent("FathGuardCarb_Heal",10000,0)
	Unit:RegisterEvent("FathGuardCarb_WaterBolt",40000,0)
end

RegisterUnitEvent(21964, 1, "FathGuardCarb_OnEnterCombat")

-- [[ Fathom Guard Sharkkis ]] --

function FathGuardShark_Multishot(Unit)
	if Unit:GetClosestPlayer() then
		Unit:FullCastSpellOnTarget(27021,Unit:GetClosestPlayer())
	end
end

function FathGuardShark_Vipersting(Unit)
	if Unit:GetClosestPlayer() then
		Unit:FullCastSpellOnTarget(37551,Unit:GetClosestPlayer())
	end
end

function FathGuardShark_OnEnterCombat(Unit)
	local FathGuardSharkSpawn = math.random(1, 2)
	if (FathGuardSharkSpawn==1) then
		local x=Unit:GetX();
		local y=Unit:GetY();
		local z=Unit:GetZ();
		Unit:SpawnCreature(21260, x, y, z, 0, 14, 360000);
	end
	if (FathGuardSharkSpawn==2) then
		local x=Unit:GetX();
		local y=Unit:GetY();
		local z=Unit:GetZ();
		Unit:SpawnCreature(21246, x, y, z, 0, 14, 360000);
	end
	Unit:RegisterEvent("FathGuardShark_Multishot",8000,0)
	Unit:RegisterEvent("FathGuardShark_Vipersting",20000,0)
end

function FathGuardShark_OnLeaveCombat(Unit)
	Unit:Despawn(1000, 0);
-- Unit:RemoveEvents()
end

RegisterUnitEvent(21966, 1, "FathGuardShark_OnEnterCombat")
RegisterUnitEvent(21966, 2, "FathGuardShark_OLeaveCombat")

-- [[ Fathom Guard Tidalves ]] --

function FathGuardTidal_Spitfire(Unit)
	Unit:FullCastSpell(38236)
end

function FathGuardTidal_Cleansing(Unit)
	Unit:FullCastSpell(8170)
end

function FathGuardTidal_Earthbind(Unit)
	Unit:FullCastSpell(2484)
end

function FathGuardTidal_OnEnterCombat(Unit)
	Unit:RegisterEvent("FathGuardTidal_Earthbind",20000,0)
	Unit:RegisterEvent("FathGuardTidal_Spitfire",50000,0)
	Unit:RegisterEvent("FathGuardTidal_Cleansing",30000,0)
end
RegisterUnitEvent(21965, 1, "FathGuardTidal_OnEnterCombat")

-- [[ GreyHeart Nether Mage ]] --

function GreyNetherMage_RainFire(Unit)
	local plr = Unit:GetRandomPlayer(0)
	if (plr ~= nil) then
		Unit:FullCastSpellOnTarget(27212,plr)
	end
end

function GreyNetherMage_Fireball(Unit)
	if Unit:GetClosestPlayer() then
		Unit:FullCastSpellOnTarget(38836,Unit:GetClosestPlayer())
	end
end

function GreyNetherMage_ConeOfCold(Unit)
	if Unit:GetClosestPlayer() then
		Unit:FullCastSpellOnTarget(27087,Unit:GetClosestPlayer())
	end
end

function GreyNetherMage_Frostbolt(Unit)
	if Unit:GetClosestPlayer() then
		Unit:FullCastSpellOnTarget(37262,Unit:GetClosestPlayer())
	end
end

function GreyNetherMage_Lightning(Unit)
	if Unit:GetClosestPlayer() then
		Unit:FullCastSpellOnTarget(38146,Unit:GetClosestPlayer())
	end
end

function GreyNetherMage_OnEnterCombat(Unit)
	Unit:RegisterEvent("GreyNetherMage_RainFire",10000,1)
	Unit:RegisterEvent("GreyNetherMage_Fireball",20000,1)
	Unit:RegisterEvent("GreyNetherMage_ConeOfCold",30000,3)
	Unit:RegisterEvent("GreyNetherMage_Frostbolt",40000,3)
	Unit:RegisterEvent("GreyNetherMage_Lightning",50000,3)
end
RegisterUnitEvent(21230, 1, "GreyNetherMage_OnEnterCombat")

-- [[ Greyheart Shield Bearer ]]--

function GreyShieldBearer_AvengerShield(Unit)
	if Unit:GetClosestPlayer() then
		Unit:FullCastSpellOnTarget(32700,Unit:GetClosestPlayer())
	end
end

function GreyShieldBearer_Charge(Unit)
	if Unit:GetClosestPlayer() then
		Unit:FullCastSpellOnTarget(11578,Unit:GetClosestPlayer())
	end
end

function GreyShieldBearer_OnEnterCombat(Unit)
	Unit:RegisterEvent("GreyShieldBearer_AvengerShield",20000,0)
	Unit:RegisterEvent("GreyShieldBearer_Charge",1000,0)
end
RegisterUnitEvent(21231, 1, "GreyShieldBearer_OnEnterCombat")

-- [[ Greyheart TideCaller ]] --

function GreyTide_PoisonShield(Unit)
	Unit:FullCastSpell(39027)
end

function GreyTide_ChainLightning(Unit)
	if Unit:GetClosestPlayer() then
		Unit:FullCastSpellOnTarget(25442,Unit:GetClosestPlayer())
	end
end

function GreyTide_WaterTotem(Unit)
	Unit:FullCastSpell(38624)
end

function GreyTide_OnEnterCombat(Unit)
	Unit:RegisterEvent("GreyTide_PoisonShield",40000,0)
	Unit:RegisterEvent("GreyTide_ChainLightning",20000,0)
	Unit:RegisterEvent("GreyTide_WaterTotem",230000,0)
end

RegisterUnitEvent(21229, 1, "GreyTide_OnEnterCombat")

-- [[ Tidewalker Depth-Seer]] --

function TideWalkerSeer_Tranquility(Unit)
	Unit:FullCastSpell(26983)
end

function TideWalkerSeer_OnEnterCombat(Unit)
	Unit:RegisterEvent("TideWalkerSeer_Tranquility",40000,0)
end

RegisterUnitEvent(21224, 1, "TideWalkerSeer_OnEnterCombat")


--[[ Tidewalker Harpooner ]] --

function TideWalkerHarp_Net(Unit)
	if Unit:GetClosestPlayer() then
		Unit:FullCastSpellOnTarget(38661,Unit:GetClosestPlayer())
	end
end

function TideWalkerHarp_OnEnterCombat(Unit)
	Unit:RegisterEvent("TideWalkerHarp_Net",25000,0)
end

RegisterUnitEvent(21227, 1, "TideWalkerHarp_OnEnterCombat")

--[[ Tidewalker Hydromancer ]] --

function TideWalkerHydro_Frostbolt(Unit)
	if Unit:GetClosestPlayer() then
		Unit:FullCastSpellOnTarget(38697,Unit:GetClosestPlayer())
	end
end

function TideWalkerHydro_FrostNova(Unit)
	Unit:FullCastSpell(27088)
end

function TideWalkerHydro_FrostShock(Unit)
	if Unit:GetClosestPlayer() then
		Unit:FullCastSpellOnTarget(25464,Unit:GetClosestPlayer())
	end
end

function TideWalkerHydro_OnEnterCombat(Unit)
	Unit:RegisterEvent("TideWalkerHydro_Frostbolt",20000,0)
	Unit:RegisterEvent("TideWalkerHydro_FrostNova",35000,0)
	Unit:RegisterEvent("TideWalkerHydro_FrostShock",50000,0)
end
RegisterUnitEvent(21228, 1, "TideWalkerHydro_OnEnterCombat")

-- [[ TideWalker Warrior]] --
function TideWalkerWarrior_Cleave(Unit)
	if Unit:GetClosestPlayer() then
		Unit:FullCastSpellOnTarget(38260,Unit:GetClosestPlayer())
	end
end

function TideWalkerWarrior_BloodThirst(Unit)
	if Unit:GetClosestPlayer() then
		Unit:FullCastSpellOnTarget(30335,Unit:GetClosestPlayer())
	end
end

function TideWalkerWarrior_Frenzy(Unit)
	Unit:FullCastSpell(37605)
end

function TideWalkerWarrior_OnEnterCombat(Unit)
	Unit:RegisterEvent("TideWalkerWarrior_Cleave",30000,0)
	Unit:RegisterEvent("TideWalkerWarrior_BloodThirst",45000,0)
	Unit:RegisterEvent("TideWalkerWarrior_Frenzy",20000,0)
end
RegisterUnitEvent(21225, 1, "TideWalkerHydro_OnEnterCombat")

-- [[ Underbog Collosus ]] --

function UnderCollosus_Geyser(Unit)
	if Unit:GetClosestPlayer() then
		Unit:FullCastSpellOnTarget(37959,Unit:GetClosestPlayer())
	end
end

function UnderCollosus_AtropicBlow(Unit)
	if Unit:GetClosestPlayer() then
		Unit:FullCastSpellOnTarget(39015,Unit:GetClosestPlayer())
	end
end

function UnderCollosus_SporeQuake(Unit)
	Unit:FullCastSpell(38976)
end

function UnderCollosus_ToxicPool(Unit)
	Unit:FullCastSpell(38718)
end

function UnderCollosus_Frenzy(Unit)
	Unit:FullCastSpell(37605)
end

function UnderCollosus_RampInfection(Unit)
	if Unit:GetClosestPlayer() then
		Unit:FullCastSpellOnTarget(39042,Unit:GetClosestPlayer())
	end
end

function UnderCollosus_OnEnterCombat(Unit)
	Unit:RegisterEvent("UnderCollosus_Geyser",50000,2)
	Unit:RegisterEvent("UnderCollosus_AtropicBlow",20000,0)
	Unit:RegisterEvent("UnderCollosus_SporeQuake",65000,2)
	Unit:RegisterEvent("UnderCollosus_ToxicPool",55000,0)
	Unit:RegisterEvent("UnderCollosus_Frenzy",45000,4)
	Unit:RegisterEvent("UnderCollosus_RampInfection",30000,0)
end

function UnderCollosus_OnDied(Unit)
	local UnderCollosusSpawn = math.random(1, 4)
	if (UnderCollosusSpawn==1) then
		local x=Unit:GetX();
		local y=Unit:GetY();
		local z=Unit:GetZ();
		Unit:SpawnCreature(22352, x, y, z, 0, 14, 360000);
		Unit:SpawnCreature(22352, x, y, z, 0, 14, 360000);
		Unit:SpawnCreature(22352, x, y, z, 0, 14, 360000);
		Unit:SpawnCreature(22352, x, y, z, 0, 14, 360000);
		Unit:SpawnCreature(22352, x, y, z, 0, 14, 360000);
		Unit:SpawnCreature(22352, x, y, z, 0, 14, 360000);
		Unit:SpawnCreature(22352, x, y, z, 0, 14, 360000);
	end
	if (UnderCollosusSpawn==2) then
		Unit:FullCastSpell(38718)
	end
	if (UnderCollosusSpawn==3) then
		local x=Unit:GetX();
		local y=Unit:GetY();
		local z=Unit:GetZ();
		Unit:SpawnCreature(22347, x, y, z, 0, 14, 360000);
		Unit:SpawnCreature(22347, x, y, z, 0, 14, 360000);
	end
	if (UnderCollosusSpawn==4) then
		Unit:FullCastSpell(38730)
	end
end
RegisterUnitEvent(21251, 1, "UnderCollosus_OnEnterCombat")
RegisterUnitEvent(21251, 4, "UnderCollosus_OnDied")