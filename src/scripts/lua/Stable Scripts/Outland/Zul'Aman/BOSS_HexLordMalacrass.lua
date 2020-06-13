--[[***************************
*      .-.                *
*      `-.. ..-. + +      *
*      `-''-'' '          *
*  OpenSource Scripting   *
*          Team           *
* http://sunplusplus.info *
*                         *
***************************
This software is provided as free and open source by the
staff of the Sun++ Scripts Project, in accordance with 
the GPL license. This means we provide the software we have 
created freely and it has been thoroughly tested to work for 
the developers, but NO GUARANTEE is made it will work for you 
as well. Please give credit where credit is due, if modifying,
redistributing and/or using this software. Thank you.

Sun++ Team, July 2008
~~End of License Agreement]]
local slither = 24242;
local thurg = 24241
local alyson = 24240
local fenstalker =24245
local lord_raadan =24243
local hex_lord = 24239;
local koragg = 24247;
local kroth = 24244
local darkheart = 24246
local CHAT_MSG_MONSTER_YELL = 14
local LANG_UNIVERSAL = 0
----------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------
---------------------------------HEX LORDS AI-----------------------------------------------------------
----------------------------------------------------------------------------------------------------------------
function HexLord_OnCombat(Unit)
	Unit:RemoveEvents();
	setvars(Unit,{First =false,HexLord_AddsTable = {},HexLord_PossibleAdds = {}})
	Unit:SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Da shadow gonna fall on you...");
	Unit:PlaySoundToSet(12041);
	Unit:RegisterEvent("HexLord_SpiritBoltsFirst", 30000, 1);
	Unit:RegisterEvent("HexLord_DrainPowerCheck", 11000, 1);
	Unit:RegisterEvent("HexLord_RandomSpawns", 50, 4)
	Unit:RegisterAIUpdateEvent(3000)
	local args = getvars(Unit)
	table.insert(args.HexLord_PossibleAdds, thurg)
	table.insert(args.HexLord_PossibleAdds, slither)
	table.insert(args.HexLord_PossibleAdds, lord_raadan)
	table.insert(args.HexLord_PossibleAdds, fenstalker)
	table.insert(args.HexLord_PossibleAdds, koragg)
	table.insert(args.HexLord_PossibleAdds, kroth)
	table.insert(args.HexLord_PossibleAdds, darkheart)
	table.insert(args.HexLord_PossibleAdds, alyson)
end
function HexLord_RandomSpawns(Unit)
	local args = getvars(Unit)
	local chosenadd = args.HexLord_PossibleAdds[math.random(1,table.getn(args.HexLord_PossibleAdds))]
	table.insert(args.HexLord_AddsTable,(Unit:SpawnCreature(chosenadd,Unit:GetX()+math.random(1,5),Unit:GetY(),Unit:GetZ(),Unit:GetO(),1890,0)))
	table.remove(args.HexLord_PossibleAdds, chosenadd)
end
	
function HexLord_SpiritBoltsFirst(Unit)
	local args = getvars(Unit)
	args.First=true
	setvars(Unit, args)
	Unit:FullCastSpell(43383)
	HexLord_FirstCheck(Unit)
end

function HexLord_FirstCheck(Unit)
	local args = getvars(Unit)
	if (args.First == true) then
		Unit:RegisterEvent("HexLord_SoulDrain", 13000, 1);
	end
end

function HexLord_SpiritBolts(Unit)
	Unit:SetAttackTimer(10000,0)
	Unit:FullCastSpell(43383)
	Unit:SetCombatSpellCapable(1)
	Unit:RegisterEvent("HexLord_SpiritBoltsdeux",10000,1)
	Unit:RegisterEvent("HexLord_SpiritBolts", 32000, 1)
end
function HexLord_SpiritBoltsdeux(Unit)
	Unit:SetCombatSpellCapable(0)
end

function HexLord_DrainPowerCheck(Unit)
	if Unit:GetHealthPct() < 80 then
		HexLord_DrainPowerAI(Unit)
	else
		Unit:RegisterEvent("HexLord_DrainPowerCheck", 5000,1)
	end
end

function HexLord_DrainPowerAI(Unit)
	local plrs = Unit:GetInRangePlayers()
	Unit:FullCastSpell(44131)
	for k,v in pairs(plrs) do
		Unit:CastSpell(44132)
	end
	Unit:RegisterEvent("HexLord_DrainPowerAI", 60000, 1);
end

function HexLord_SoulDrain(Unit)
	Unit:RemoveEvents();
	local plr = Unit:GetRandomPlayer(0)
	if plr == nil then
		return
	end
	local Class = plr:GetPlayerClass()
	if (Class == "Warrior") then
		HexLord_WarriorDrain(Unit)
	elseif (Class == "Paladin") then
		HexLord_PaladinDrain(Unit)
	elseif (Class == "Hunter") then
		HexLord_HunterDrain(Unit)
	elseif (Class == "Rogue") then
		HexLord_RogueDrain(Unit)
	elseif (Class == "Priest") then
		HexLord_PriestDrain(Unit)
	elseif (Class == "Shaman") then
		HexLord_ShamanDrain(Unit)
	elseif (Class == "Mage") then
		HexLord_MageDrain(Unit)
	elseif (Class == "Warlock") then
		HexLord_WarlockDrain(Unit)
	elseif (Class == "Druid") then
		HexLord_DruidDrain(Unit)
	end
	Unit:RegisterEvent("HexLord_DrainPowerCheck", 1000 ,1);
	Unit:RegisterEvent("HexLord_SpiritBolts", 32000, 1);
	Unit:RegisterEvent("HexLord_SoulDrain", 60000, 1);
end
function HexLord_OnDeath(Unit)
	Unit:RemoveEvents()
	Unit:SendChatMessage(CHAT_MSG_MONSTER_YELL,LANG_UNIVERSAL, "Dis not da end for me...");
	Unit:PlaySoundToSet(12051)
	Unit:RemoveAIUpdateEvent()
end
function HexLord_OnKilledTarget(Unit)
	local deathcheck = math.random(1, 2);
	if (deathcheck == 1) then
		Unit:SendChatMessage(CHAT_MSG_MONSTER_YELL,LANG_UNIVERSAL,"Azzaga choogo zinn!");
		Unit:PlaySoundToSet(12044);
	else
		Unit:SendChatMessage(CHAT_MSG_MONSTER_YELL,LANG_UNIVERSAL, "Dis a nightmare ya don' wake up from!");
		Unit:PlaySoundToSet(12043);
	end
end
function HexLord_OnWipe(Unit)
	Unit:RemoveEvents()
	Unit:RemoveAIUpdateEvent()
	local args = getvars(Unit)
	for k,v in pairs(args.HexLord_AddsTable) do
		if v~= nil and v:IsInWorld() then
			v:RemoveFromWorld()
			k,v = nil, nil
		end
	end
end

RegisterUnitEvent(hex_lord, 1, "HexLord_OnCombat");
RegisterUnitEvent(hex_lord, 2, "HexLord_OnWipe");
RegisterUnitEvent(hex_lord, 3, "HexLord_OnKilledTarget");
RegisterUnitEvent(hex_lord, 4, "HexLord_OnDeath");
RegisterUnitEvent(hex_lord,21,"HexLord_FirstCheck");
----------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------
--------------------------DRAIN POWER AI------------------------------------------
----------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------
--Warrior Drain--
function HexLord_WarriorDrain(Unit)
	local Start = math.random(1,3);
	if (Start == 1) then
		HexLord_WarriorSpellReflect(Unit)
	elseif (Start == 2) then
		HexLord_WarriorWhirlWind(Unit)
	elseif (Start == 3) then
		HexLord_WarriorMortalStrike(Unit)
	end
	Unit:RegisterEvent("HexLord_WarriorDrain", 10000, 1);
end
function HexLord_WarriorSpellReflect(Unit)
	Unit:FullCastSpell(43443)
end
function HexLord_WarriorWhirlWind(Unit)
	Unit:SetAttackTimer(4000,0)
	Unit:FullCastSpell(43442)
end
function HexLord_WarriorMortalStrike(Unit)
	local tank = Unit:GetMainTank()
	if tank ~= nil then
		Unit:FullCastSpellOnTarget(43441,tank)
	end
end
-- Paladin Soul Drain --
function HexLord_PaladinDrain(Unit)
	local Start = math.random(1,3);
	if (Start == 1) then
		HexLord_PaladinAoE(Unit)
	elseif (Start == 2) then
		HexLord_PaladinHLight(Unit)
	elseif (Start == 3) then
		HexLord_PaladinAvgWrath(Unit)
	end
	Unit:RegisterEvent("HexLord_PaladinDrain", 10000, 1);
end
function HexLord_PaladinAoE(Unit)
	Unit:FullCastSpell(43429)
end
function HexLord_PaladinHLight(Unit)
	Unit:SetAttackTimer(2500,0)
	Unit:FullCastSpell(43451)
end
function HexLord_PaladinAvgWrath(Unit)
	Unit:FullCastSpell(43430)
end

-- Hunter Soul Drain ---
function HexLord_HunterDrain(Unit)
	local Start = math.random(1,3);
	if (Start == 1) then
		HexLord_HunterExpTrap(Unit)
	elseif (Start == 2) then
		HexLord_HunterFreezeTrap(Unit)
	elseif (Start == 3) then
		--HunterSnakeTrap(Hex,event)
	end
	Unit:RegisterEvent("HexLord_HunterDrain", 10000, 1);
end
function HexLord_HunterExpTrap(Unit)
	Unit:FullCastSpell(43444)
end
function HexLord_HunterFreezeTrap(Unit)
	Unit:FullCastSpell(43447)
end

--[[function HunterSnakeTrap(Hex, event)
	Hex:FullCastSpell(43449);
end]]
-- Rogue Soul Drain --
function HexLord_RogueDrain(Unit)
	local Start = math.random(1,3);
	if (Start == 1) then
		HexLord_RogueBlind(Unit)
	elseif (Start == 2) then
		HexLord_RogueSlicenDice(Unit)
	elseif (Start == 3) then
		HexLord_RogueWoundPoison(Unit)
	end
	Unit:RegisterEvent("HexLord_RogueDrain", 10000, 1);
end
function HexLord_RogueBlind(Unit)
	local plr = Unit:GetRandomPlayer(0)
	if plr ~= nil then
		Unit:FullCastSpellOnTarget(43433,plr)
	end
end
function HexLord_RogueSlicenDice(Unit)
	Unit:FullCastSpell(43547)
end
function HexLord_RogueWoundPoison(Unit)
	local plr = Unit:GetRandomPlayer(0)
	if plr ~= nil then
		Unit:FullCastSpellOnTarget(39665,plr)
	end
end
-- Priest Soul Drain --
function HexLord_PriestDrain(Unit)
	local Start = math.random(1,6);
	if (Start == 1) then
		HexLord_PriestHeal(Unit)
	--elseif (Start == 2) then
	elseif (Start == 3) then
		HexLord_PriestMindBlast(Unit)
	elseif (Start == 4) then
		HexLord_PriestSWD(Unit)
	elseif (Start == 5) then
		HexLord_PriestPsychicScream(Unit)
	elseif (Start == 6) then
		 HexLord_PriestPainSuppression(Unit)
	end
	Unit:RegisterEvent("HexLord_PriestDrain", 10000, 1);
end

function HexLord_PriestHeal(Unit)
	Unit:FullCastSpell(41372)
end

--[[function PriestMindControl(Hex, event)
	local plr = Hex:GetRandomPlayer(0);
	if (plr ~= nil) then
		Hex:FullCastSpellOnTarget(43550, plr);
	end
end]] -- Not Supported in Core yet.

function HexLord_PriestMindBlast(Unit)
	local plr = Unit:GetRandomPlayer(0)
	if plr ~= nil then
		Unit:SetAttackTimer(2500,0)
		Unit:FullCastSpellOnTarget(41374,plr)
	end
end

function HexLord_PriestSWD(Unit)
	local plr = Unit:GetRandomPlayer(0)
	if plr ~= nil then
		Unit:FullCastSpellOnTarget(41375,plr)
	end
end
function HexLord_PriestPsychicScream(Unit)
	local plr = Unit:GetRandomPlayer(0)
	if plr ~= nil then
		Unit:FullCastSpell(43442)
	end
end
function HexLord_PriestPainSuppression(Unit)
	Unit:FullCastSpell(44416)
end


--Shaman Soul Drain --
function HexLord_ShamanDrain(Unit)
	local Start = math.random(1,3);
	if (Start == 1) then
		HexLord_ShamanHealWave(Unit)
	elseif (Start == 2) then
		HexLord_ShamanChainLight(Unit)
	elseif (Start == 3) then
		HexLord_ShamanFireNova(Unit)
	end
	Unit:RegisterEvent("HexLord_ShamanDrain", 10000, 1);
end

function HexLord_ShamanHealWave(Unit)
	Unit:SetAttackTimer(2500,0)
	Unit:FullCastSpell(43548)
end

function HexLord_ShamanChainLight(Unit)
	local plr = Unit:GetRandomPlayer(0)
	if plr ~= nil then
		Unit:SetAttackTimer(2500,0)
		Unit:FullCastSpellOnTarget(43435,plr)
	end
end

function HexLord_ShamanFireNova(Unit)
	-- Unit:CastSpell(43436) Won't work, need a work around.
	Unit:SpawnCreature(5879,Unit:GetX(),Unit:GetY(),Unit:GetZ(),Unit:GetO(),14, 5500)
end
---------------------------------------------------------------------------------------------------------
function HexLord_FireNovaTotemDeath(Unit)
	Unit:RemoveAIUpdateEvent()
	Unit:RemoveFromWorld()
end
function HexLord_FireNovaTotem(Unit)
	local tbl = Unit:GetInRangeFriends()
	for k,v in pairs(tbl) do
		if v:GetEntry() == hex_lord then
			Unit:SetCombatCapable(1)
			Unit:WipeCurrentTarget()
			Unit:RegisterAIUpdateEvent(5000)
		end
		break
	end
end
function HexLord_FireNovaTotemBlast(Unit)
	Unit:FullCastSpell(23462)
	Unit:RemoveAIUpdateEvent()
	Unit:RemoveFromWorld()
end

RegisterUnitEvent(5879,18,"HexLord_FireNovaTotem")
RegisterUnitEvent(5879,21,"HexLord_FireNovaTotemBlast")
RegisterUnitEvent(5879,4,"HexLord_FireNovaTotemDeath")
-------------------------------------------------------------------------------
-- Mage Soul Drain --
function HexLord_MageDrain(Unit)
	local Start = math.random(1,3);
	if (Start == 1) then
		HexLord_MageFireBall(Unit)
	elseif (Start == 2) then
		HexLord_MageFrostbolt(Unit)
	elseif (Start == 3) then
		HexLord_MageFrostNova(Unit)
	end
	Unit:RegisterEvent("HexLord_MageDrain", 10000, 1);
end

function HexLord_MageFireBall(Unit)
	local plr = Unit:GetRandomPlayer(0)
	if plr ~= nil then
		Unit:FullCastSpellOnTarget(41383,plr)
	end
end

function HexLord_MageFrostbolt(Unit)
	local plr = Unit:GetRandomPlayer(0)
	if plr ~= nil then
		Unit:SetAttackTimer(2500,0)
		Unit:FullCastSpellOnTarget(43428,plr)
	end
end

function HexLord_MageFrostNova(Unit)
	local plr = Unit:GetRandomPlayer(0)
	if plr ~= nil then
		Unit:FullCastSpell(43426)
		local tbl = Unit:GetInRangePlayers()
		for k,v in pairs(tbl) do
			if v:HasAura(43426) ~= true then
				table.remove(tbl, k)
			end
		end
		local no = table.getn(tbl)
		if no ~= nil then
			local plr = tbl[math.random(1,no)]
			if plr ~= nil and math.random(0,1) == 1 then
				Unit:FullCastSpellOnTarget(43427, plr:GetGUID())
			end
		end
	end
end
-- Warlock Soul Drain --
function HexLord_WarlockDrain(Unit)
	local Start = math.random(1,3);
	if (Start == 1) then
		HexLord_WarlockCoD(Unit)
	elseif (Start == 2) then
		HexLord_WarlockRainOFire(Unit)
	elseif (Start == 3) then
		HexLord_WarlockUA(Unit)
	end
	Unit:RegisterEvent("HexLord_WarlockDrain", 10000, 1);
end

function HexLord_WarlockCoD(Unit)
	local plr = Unit:GetRandomPlayer(0)
	if plr ~= nil then
		Unit:FullCastSpellOnTarget(43439, plr)
	end
end

function HexLord_WarlockRainOFire(Unit)
	local plr = Unit:GetRandomPlayer(0)
	if plr ~= nil then
		Unit:CastSpellAoF(plr:GetX(),plr:GetY(),plr:GetZ(), 43440)
	end
end

function HexLord_WarlockUA(Unit)
	local plr = Unit:GetRandomPlayer(0)
	if plr ~= nil then
		Unit:FullCastSpellOnTarget(35183,plr)
	end
end

-- Druid Soul Drain --
function HexLord_DruidDrain(Unit)
	local Start = math.random(1,3);
	if (Start == 1) then
		HexLord_DruidLifeBloom(Unit)
	elseif (Start == 2) then
		HexLord_DruidThorns(Unit)
	elseif (Start == 3) then
		HexLord_DruidMoonFire(Unit)
	end
	Unit:RegisterEvent("HexLord_DruidDrain", 10000, 1);
end

function HexLord_DruidLifeBloom(Unit, event)
	Unit:FullCastSpell(43421)
end

function HexLord_DruidThorns(Unit)
	Unit:FullCastSpell(43420);
end

function HexLord_DruidMoonFire(Unit)
	local plr = Unit:GetRandomPlayer(0)
	if plr ~= nil then
		Unit:FullCastSpellOnTarget(43545,plr)
	end
end
---------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------
--------------------------------SLITHER'S AI------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------
function Slither_OnCombat(Unit)
	Unit:RegisterAIUpdateEvent(7000)
	local plr = Unit:GetRandomPlayer(0)
	if plr ~= nil then
		plr:AttackReaction(Unit, 1, 0)
	end
end
function Slither_AIUpdate(Unit)
	local plr = Unit:GetRandomPlayer(0)
	if plr ~= nil then
		Unit:FullCastSpellOnTarget(43579,plr)
	end
end
function Slither_OnDeath(Unit)
	Unit:RemoveAIUpdateEvent()
	local args = getvars(Unit)
	for k,v in pairs(args.HexLord_AddsTable) do
		if v ~= nil and v == Unit then
			table.remove(args.HexLord_AddsTable, k)
		end
	end
end
RegisterUnitEvent(slither, 18, "Slither_OnCombat")
RegisterUnitEvent(slither, 21, "Slither_AIUpdate")
RegisterUnitEvent(slither, 4, "Slither_OnDeath")
----------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------
-----------------------------------FEN STALKER AI---------------------------------------------
----------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------
function FenStalker_OnSpawn(Unit)
	Unit:RegisterAIUpdateEvent(10000)
	local plr = Unit:GetRandomPlayer(0)
	if plr ~= nil then
		plr:AttackReaction(Unit, 1, 0)
	end
end
function FenStalker_AIUpdate(Unit)
	local plr = Unit:GetRandomPlayer(0)
	if plr ~= nil then
		Unit:FullCastSpellOnTarget(43586,plr)
	end
end
function FenStalker_OnDeath(Unit,event)
	Unit:RemoveAIUpdateEvent()
	local args = getvars(Unit)
	for k,v in pairs(args.HexLord_AddsTable) do
		if v~= nil and v == Unit then
			table.remove(args.HexLord_AddsTable, k)
		end
	end
end
RegisterUnitEvent(fenstalker, 18, "FenStalker_OnSpawn")
RegisterUnitEvent(fenstalker, 4, "FenStalker_OnDeath")
RegisterUnitEvent(fenstalker, 21, "FenStalker_AIUpdate")
----------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------
-----------------------------KORAGG-------------------------------------------------
----------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------
function KoraggThurg_OnLoad(Unit)
	local plr = Unit:GetRandomPlayer(0)
	if plr ~= nil then
		plr:AttackReaction(Unit, 1, 0)
	end
end
function KoraggThurg_OnDeath(Unit)
	local args = getvars(Unit)
	for k, v in pairs(args.HexLord_AddsTable) do
		if v ~= nil and v == Unit then
			table.remove(args.HexLord_AddsTable, k)
		end
	end
end
RegisterUnitEvent(thurg, 18, "KoraggThurg_OnLoad")
RegisterUnitEvent(koragg, 18, "KoraggThurg_OnLoad")
RegisterUnitEvent(thurg, 4,"KoraggThurg_OnDeath")
RegisterUnitEvent(koragg,4,"KoraggThurg_OnDeath")
----------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------
------------------------GAZAKROTH AI---------------------------------------------------------
----------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------
function Gazakroth_OnLoad(Unit,event)
	Unit:SetCombatCapable(1)
	Unit:RegisterAIUpdateEvent(2000)
	local plr = Unit:GetRandomPlayer(0)
	if plr ~= nil then
		plr:AttackReaction(Unit, 1, 0)
	end
end

function Gazakroth_Fireball(Unit)
	local plr = Unit:GetRandomPlayer(0)
	if plr ~= nil then
		Unit:FullCastSpellOnTarget(43584,plr)
	end
end

function Gazakroth_OnDeath(Unit)
	Unit:RemoveAIUpdateEvent()
	local args = getvars(Unit)
	for k,v in pairs(args.HexLord_AddsTable) do
		if v ~= nil and v == Unit then	
			table.remove(args.HexLord_AddsTable, k)
		end
	end
end

RegisterUnitEvent(kroth, 4, "Gazakroth_OnDeath")
RegisterUnitEvent(kroth, 18, "Gazakroth_OnLoad")
RegisterUnitEvent(kroth, 21, "Gazakroth_Fireball")


----------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------
----------------------------LORD RAADAN AI------------------------------------------
----------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------
function LordRaadan_OnLoad(Unit, event)
	Unit:RegisterAIUpdateEvent(7000)
	local plr = Unit:GetRandomPlayer(0)
	if plr ~= nil then
		plr:AttackReaction(Unit, 1, 0)
	end
end

function LordRaadan_Spells(Unit, event)
	local plr = Unit:GetRandomPlayer(0)
	if plr ~= nil then
		if math.random(0,1) < 0.5 then
			Unit:CastSpell(43582)
		else
			Unit:CastSpell(43583)
		end
	end
end

function LordRaadan_OnDeath(Unit)
	Unit:RemoveAIUpdateEvent()
	local args = getvars(Unit)
	for k,v  in pairs(args.HexLord_AddsTable) do
		if v == Unit then
			table.remove(args.HexLord_AddsTable, k)
		end
	end
end
RegisterUnitEvent(lord_raadan, 4, "LordRaadan_OnDeath");
RegisterUnitEvent(lord_raadan, 18, "LordRaadan_OnLoad")
RegisterUnitEvent(lord_raadan, 21, "LordRaadan_Spells")


----------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------
-------------------------------DARK HEART AI------------------------------------------------
----------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------
function Darkheart_OnSpawn(Unit, event)
	Unit:RegisterAIUpdateEvent(10000)
	local plr = Unit:GetRandomPlayer(0)
	if plr ~= nil then
		plr:AttackReaction(Unit, 1, 0)
	end
end

function Darkheartfear(Unit)
	local plr = Unit:GetRandomPlayer(0)
	if plr ~= nil then
		Unit:FullCastSpell(43590)
	end
end

function Darkheart_OnDeath(Unit)
	local args = getvars(Unit)
	for k,v in pairs(args.HexLord_AddsTable) do
		if v == Unit then
			table.remove(args.HexLord_AddsTable, k)
		end
	end
end
RegisterUnitEvent(darkheart, 18, "Darkheart_OnSpawn")
RegisterUnitEvent(darkheart, 21, "Darkheartfear")
RegisterUnitEvent(darkheart, 4, "Darkheart_OnDeath")


----------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------
-------------------------ALYSON AI----------------------------------------------
----------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------
function Alyson(Unit,event)
	Unit:SetCombatCapable(1)
	Unit:WipeCurrentTarget()
	Unit:RegisterAIUpdateEvent(3000)
end
function Alyson_AIUpdate(Unit,event)
	local tbl = Unit:GetInRangeFriends()
	local no = table.getn(tbl)
	if no ~= 0 then
		local healtar = tbl[math.random(1,no)]
		if healtar ~= nil and healtar:IsAlive() then
			Unit:SetNextTarget(healtar)
			Unit:FullCastSpellOnTarget(43575, healtar:GetGUID());
		end
	end
end
function AlysonStop(Unit)
	Unit:RemoveAIUpdateEvent()
	local args = getvars(Unit)
	for k,v in pairs(args.HexLord_AddsTable) do
		if v ~= nil and v == Unit then
			table.remove(args.HexLord_AddsTable, k)
		end
	end
end
			
RegisterUnitEvent(alyson, 4, "AlysonStop")
RegisterUnitEvent(alyson, 18, "Alyson")
RegisterUnitEvent(alyson, 21, "Alyson_AIUpdate")
