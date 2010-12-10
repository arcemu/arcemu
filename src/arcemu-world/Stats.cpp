/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2010 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "StdAfx.h"

uint32 getConColor(uint16 AttackerLvl, uint16 VictimLvl)
{

	//	const uint32 grayLevel[sWorld.LevelCap+1] = {0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,13,14,15,16,17,18,19,20,21,22,22,23,24,25,26,27,28,29,30,31,31,32,33,34,35,35,36,37,38,39,39,40,41,42,43,43,44,45,46,47,47,48,49,50,51,51,52,53,54,55,55};
	const uint32 grayLevel[PLAYER_LEVEL_CAP+1] = {0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,13,14,15,16,17,18,19,20,21,22,22,23,24,25,26,27,28,29,30,31,31,32,33,34,35,35,36,37,38,39,39,40,41,42,43,43,44,45,46,47,47,48,49,50,51,51,52,53,54,55,56,
#if PLAYER_LEVEL_CAP==80
		57,58,59,60,61,62,63,64,65,65
#endif
	};
	if(AttackerLvl + 5 <= VictimLvl)
	{
		if(AttackerLvl + 10 <= VictimLvl)
		{
			return 5;
		}
		return 4;
	}
	else
	{
		switch(VictimLvl - AttackerLvl)
		{
		case 4:
		case 3:
			return 3;
			break;
		case 2:
		case 1:
		case 0:
		case -1:
		case -2:
			return 2;
			break;
		default:
			// More adv formula for grey/green lvls:
			if(AttackerLvl <= 6)
			{
				return 1; //All others are green.
			}
			else
			{
				if(AttackerLvl > PLAYER_LEVEL_CAP)
					return 1;//gm
				if(AttackerLvl<PLAYER_LEVEL_CAP && VictimLvl <= grayLevel[AttackerLvl])
					return 0;
				else
					return 1;
			}
		}
	}
}

uint32 CalculateXpToGive(Unit *pVictim, Unit *pAttacker)
{
	if(pVictim->IsPlayer())
		return 0;

	if(TO_CREATURE(pVictim)->IsTotem())
		return 0;

	CreatureInfo *victimI;
	victimI = TO_CREATURE(pVictim)->GetCreatureInfo();

	if(victimI->Type == UNIT_TYPE_CRITTER)
		return 0;
	uint32 VictimLvl = pVictim->getLevel();
	uint32 AttackerLvl = pAttacker->getLevel();

	if( pAttacker->IsPet() && TO< Pet* >( pAttacker )->GetPetOwner() )
	{
		// based on: http://www.wowwiki.com/Talk:Formulas:Mob_XP#Hunter.27s_pet_XP (2008/01/12)
		uint32 ownerLvl = TO< Pet* >( pAttacker )->GetPetOwner()->getLevel();
		VictimLvl += ownerLvl - AttackerLvl;
		AttackerLvl = ownerLvl;
	}
	else if( (int32)VictimLvl - (int32)AttackerLvl > 10 ) //not wowwikilike but more balanced
		return 0;

	// Partha: this screws things up for pets and groups
	// No need for it here - it does this later in Player::GiveXP 
	/*
	uint32 max_level = 70;
	if(pAttacker->IsPlayer())
	max_level = pAttacker->GetUInt32Value(PLAYER_FIELD_MAX_LEVEL);
	else if(pAttacker->IsPet())
	max_level = TO< Pet* >( pAttacker )->GetPetOwner()->GetUInt32Value(PLAYER_FIELD_MAX_LEVEL);

	if(pAttacker->getLevel() >= max_level)
	return 0;
	*/


	/*if(VictimLvl+7>AttackerLvl)
	VictimLvl = AttackerLvl + 7;*/

	float zd = 5;
	float g = 5;

	// get zero diff
	if(AttackerLvl >= PLAYER_LEVEL_CAP)
	{
		g = 15;
		zd = 19;
	}
	else if(AttackerLvl >= 65)
	{
		zd = 18;
		g = 14;
	}
	else if(AttackerLvl >= 60)
	{
		zd = 17;
		g = 13;
	}
	else if(AttackerLvl >= 55)
	{
		zd = 16;
		g = 12;
	}
	else if(AttackerLvl >= 50)
	{
		zd = 15;
		g = 11;
	}
	else if(AttackerLvl >= 45)
	{
		zd = 14;
		g = 10;
	}
	else if(AttackerLvl >= 40)
	{
		zd = 13;
		g = 9;
	}
	else if(AttackerLvl >= 30)
	{
		zd = 12;
		g = 8;
	}
	else if(AttackerLvl >= 20)
	{
		zd = 11;
		g = 7;
	}
	else if(AttackerLvl >= 16)
	{
		zd = 9;
		g = 6;
	}
	else if(AttackerLvl >= 12)
	{
		zd = 8;
		g = 6;
	}
	else if(AttackerLvl >= 10)
	{
		zd = 7;
		g = 6;
	}
	else if(AttackerLvl >= 8)
	{
		zd = 6;
		g = 5;
	}
	else
	{
		zd = 5;
		g = 5;
	}
	// get grey diff

	float xp = 0.0f;
	float fVictim = float(VictimLvl);
	float fAttacker = float(AttackerLvl);

	if(VictimLvl == AttackerLvl)
		xp = ((fVictim * 5.0f) + 45.0f);
	else if(VictimLvl > AttackerLvl)
	{
		float j = 1.0f + (0.25f * (fVictim - fAttacker));
		xp = ((AttackerLvl * 5.0f) + 45.0f) * j;
	}
	else
	{
		if((AttackerLvl - VictimLvl) < g)
		{
			float j = (1.0f - ((fAttacker - fVictim) / zd));
			xp = (AttackerLvl * 5.0f + 45.0f) * j;
		}
	}

	// multiply by global XP rate
	if(xp == 0.0f)
		return 0;

	xp *= sWorld.getRate(RATE_XP);

	// elite boss multiplier
	if(victimI)
	{
		switch(victimI->Rank)
		{
		case 0: // normal mob
			break;
		case 1: // elite
			xp *= 2.0f;
			break;
		case 2: // rare elite
			xp *= 2.0f;
			break;
		case 3: // world boss
			xp *= 2.5f;
			break;
		default:	// rare or higher
			//			xp *= 7.0f;
			break;
		}
	}
	return (uint32)xp;
	/*const float ZD[PLAYER_LEVEL_CAP+1] = {1,5,5,5,5,5,5,5,6,6,7,7,8,8,8,9,9,9,9,9,11,11,11,11,11,11,11,11,11,11,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,14,14,14,14,14,15,15,15,15,15,16,16,16,16,16,17,17,17,17,17,17,17,17,17,17,17};
	float temp = 0;
	float tempcap = 0;
	float xp = 0;

	if(VictimLvl >= AttackerLvl)
	{
	temp = ((AttackerLvl * 5) + 45) * (1 + 0.05 * (VictimLvl - AttackerLvl));
	tempcap = ((AttackerLvl * 5) + 45) * 1.2;
	if(temp > tempcap)
	{
	if( tempcap < 0 )
	tempcap = 0;
	else
	tempcap *= sWorld.getRate(RATE_XP);

	xp = tempcap;
	}
	else
	{
	if( temp < 0 )
	temp = 0;
	else
	temp *= sWorld.getRate(RATE_XP);

	xp = temp;
	}
	}
	else
	{
	if(getConColor(AttackerLvl, VictimLvl) == 0)
	{
	return (uint32)0;
	}
	else
	{
	if(AttackerLvl < PLAYER_LEVEL_CAP)
	temp = (((AttackerLvl * 5) + 45) * (1 - (AttackerLvl - VictimLvl)/ZD[AttackerLvl]));
	else
	temp = (((AttackerLvl * 5) + 45) * (1 - (AttackerLvl - VictimLvl)/17));
	if( temp < 0 )
	temp = 0;
	else
	temp *= sWorld.getRate(RATE_XP);

	xp = temp;
	}
	}

	if(victimI)
	{
	switch(victimI->Rank)
	{
	case 0: // normal mob
	break;
	case 1: // elite
	xp *= 1.5f;
	break;
	case 2: // rare elite
	xp *= 3.0f;
	break;
	case 3: // world boss
	xp *= 10.0f;
	break;
	default:	// rare or higher
	xp *= 7.0f;
	break;
	}
	}

	return (uint32)(xp);*/
}

//Taken from WoWWoW Source
/*
Author: pionere

Calculate the stat increase. Using 3rd grade polynome.

Parameter level The level the character reached.
Parameter a3 The factor for x^3.
Parameter a2 The factor for x^2.
Parameter a1 The factor for x^1.
Parameter a0 The constant factor for the polynome.
Return stat gain
*/
uint32 CalculateStat(uint16 level,double a3, double a2, double a1, double a0)
{
	int result1;
	int result2;
	int diff;

	result1 =	(int)(a3*level*level*level +
		a2*level*level +
		a1*level +
		a0);

	result2 =	(int)(a3*(level-1)*(level-1)*(level-1) +
		a2*(level-1)*(level-1) +
		a1*(level-1) +
		a0);

	//get difference
	diff = result1-result2;
	return diff;
}
//Partialy taken from WoWWoW Source
uint32 GainStat(uint16 level, uint8 playerclass,uint8 Stat)
{
	uint32 gain = 0;
	switch(playerclass)
	{
	case WARRIOR:
		{
			switch(Stat)
			{
			case STAT_STRENGTH:  { gain = CalculateStat(level, 0.000039, 0.006902, 1.080040, -1.051701); }break;
			case STAT_AGILITY:   { gain = CalculateStat(level, 0.000022, 0.004600, 0.655333, -0.600356); }break;
			case STAT_STAMINA:   { gain = CalculateStat(level, 0.000059, 0.004044, 1.040000, -1.488504); }break;
			case STAT_INTELLECT: { gain = CalculateStat(level, 0.000002, 0.001003, 0.100890, -0.076055); }break;
			case STAT_SPIRIT:	{ gain = CalculateStat(level, 0.000006, 0.002031, 0.278360, -0.340077); }break;
			}

		}break;

	case PALADIN:
		{
			switch(Stat)
			{
			case STAT_STRENGTH:  { gain = CalculateStat(level, 0.000037, 0.005455, 0.940039, -1.000090); }break;
			case STAT_AGILITY:   { gain = CalculateStat(level, 0.000020, 0.003007, 0.505215, -0.500642); }break;
			case STAT_STAMINA:   { gain = CalculateStat(level, 0.000038, 0.005145, 0.871006, -0.832029); }break;
			case STAT_INTELLECT: { gain = CalculateStat(level, 0.000023, 0.003345, 0.560050, -0.562058); }break;
			case STAT_SPIRIT:	{ gain = CalculateStat(level, 0.000032, 0.003025, 0.615890, -0.640307); }break;
			}
		}break;

	case HUNTER:
		{
			switch(Stat)
			{
			case STAT_STRENGTH:  { gain = CalculateStat(level, 0.000022, 0.001800, 0.407867, -0.550889); }break;
			case STAT_AGILITY:   { gain = CalculateStat(level, 0.000040, 0.007416, 1.125108, -1.003045); }break;
			case STAT_STAMINA:   { gain = CalculateStat(level, 0.000031, 0.004480, 0.780040, -0.800471); }break;
			case STAT_INTELLECT: { gain = CalculateStat(level, 0.000020, 0.003007, 0.505215, -0.500642); }break;
			case STAT_SPIRIT:	{ gain = CalculateStat(level, 0.000017, 0.003803, 0.536846, -0.490026); }break;
			}
		}break;

	case ROGUE:
		{
			switch(Stat)
			{
			case STAT_STRENGTH:  { gain = CalculateStat(level, 0.000025, 0.004170, 0.654096, -0.601491); }break;
			case STAT_AGILITY:   { gain = CalculateStat(level, 0.000038, 0.007834, 1.191028, -1.203940); }break;
			case STAT_STAMINA:   { gain = CalculateStat(level, 0.000032, 0.003025, 0.615890, -0.640307); }break;
			case STAT_INTELLECT: { gain = CalculateStat(level, 0.000008, 0.001001, 0.163190, -0.064280); }break;
			case STAT_SPIRIT:	{ gain = CalculateStat(level, 0.000024, 0.000981, 0.364935, -0.570900); }break;
			}
		}break;

	case PRIEST:
		{
			switch(Stat)
			{
			case STAT_STRENGTH:  { gain = CalculateStat(level, 0.000008, 0.001001, 0.163190, -0.064280); }break;
			case STAT_AGILITY:   { gain = CalculateStat(level, 0.000022, 0.000022, 0.260756, -0.494000); }break;
			case STAT_STAMINA:   { gain = CalculateStat(level, 0.000024, 0.000981, 0.364935, -0.570900); }break;
			case STAT_INTELLECT: { gain = CalculateStat(level, 0.000039, 0.006981, 1.090090, -1.006070); }break;
			case STAT_SPIRIT:	{ gain = CalculateStat(level, 0.000040, 0.007416, 1.125108, -1.003045); }break;
			}
		}break;

	case SHAMAN:
		{
			switch(Stat)
			{
			case STAT_STRENGTH:  { gain = CalculateStat(level, 0.000035, 0.003641, 0.734310, -0.800626); }break;
			case STAT_AGILITY:   { gain = CalculateStat(level, 0.000022, 0.001800, 0.407867, -0.550889); }break;
			case STAT_STAMINA:   { gain = CalculateStat(level, 0.000020, 0.006030, 0.809570, -0.809220); }break;
			case STAT_INTELLECT: { gain = CalculateStat(level, 0.000031, 0.004480, 0.780040, -0.800471); }break;
			case STAT_SPIRIT:	{ gain = CalculateStat(level, 0.000038, 0.005145, 0.871006, -0.832029); }break;
			}
		}break;

	case MAGE:
		{
			switch(Stat)
			{
			case STAT_STRENGTH:  { gain = CalculateStat(level, 0.000002, 0.001003, 0.100890, -0.076055); }break;
			case STAT_AGILITY:   { gain = CalculateStat(level, 0.000008, 0.001001, 0.163190, -0.064280); }break;
			case STAT_STAMINA:   { gain = CalculateStat(level, 0.000006, 0.002031, 0.278360, -0.340077); }break;
			case STAT_INTELLECT: { gain = CalculateStat(level, 0.000040, 0.007416, 1.125108, -1.003045); }break;
			case STAT_SPIRIT:	{ gain = CalculateStat(level, 0.000039, 0.006981, 1.090090, -1.006070); }break;
			}
		}break;

	case WARLOCK:
		{
			switch(Stat)
			{
			case STAT_STRENGTH:  { gain = CalculateStat(level, 0.000006, 0.002031, 0.278360, -0.340077); }break;
			case STAT_AGILITY:   { gain = CalculateStat(level, 0.000024, 0.000981, 0.364935, -0.570900); }break;
			case STAT_STAMINA:   { gain = CalculateStat(level, 0.000021, 0.003009, 0.486493, -0.400003); }break;
			case STAT_INTELLECT: { gain = CalculateStat(level, 0.000059, 0.004044, 1.040000, -1.488504); }break;
			case STAT_SPIRIT:	{ gain = CalculateStat(level, 0.000040, 0.006404, 1.038791, -1.039076); }break;
			}
		}break;

	case DRUID:
		{
			switch(Stat)
			{
			case STAT_STRENGTH:  { gain = CalculateStat(level, 0.000021, 0.003009, 0.486493, -0.400003); }break;
			case STAT_AGILITY:   { gain = CalculateStat(level, 0.000041, 0.000440, 0.512076, -1.000317); }break;
			case STAT_STAMINA:   { gain = CalculateStat(level, 0.000023, 0.003345, 0.560050, -0.562058); }break;
			case STAT_INTELLECT: { gain = CalculateStat(level, 0.000038, 0.005145, 0.871006, -0.832029); }break;
			case STAT_SPIRIT:	{ gain = CalculateStat(level, 0.000059, 0.004044, 1.040000, -1.488504); }break;
			}
		}break;

	case DEATHKNIGHT:
		{
			switch(Stat)
			{
			case STAT_STRENGTH:  { gain = CalculateStat(level, 0.000039, 0.006902, 1.080040, -1.051701); }break;
			case STAT_AGILITY:   { gain = CalculateStat(level, 0.000022, 0.004600, 0.655333, -0.600356); }break;
			case STAT_STAMINA:   { gain = CalculateStat(level, 0.000059, 0.004044, 1.040000, -1.488504); }break;
			case STAT_INTELLECT: { gain = CalculateStat(level, 0.000002, 0.001003, 0.100890, -0.076055); }break;
			case STAT_SPIRIT:	{ gain = CalculateStat(level, 0.000006, 0.002031, 0.278360, -0.340077); }break;
			}

		}break;
	}
	return gain;
}

uint32 CalculateDamage( Unit* pAttacker, Unit* pVictim, uint32 weapon_damage_type, uint32 *spellgroup, SpellEntry* ability ) // spellid is used only for 2-3 spells, that have AP bonus
{
	//TODO: Some awesome formula to determine how much damage to deal
	//consider this is melee damage
	// weapon_damage_type: 0 = melee, 1 = offhand(dualwield), 2 = ranged

	// Attack Power increases your base damage-per-second (DPS) by 1 for every 14 attack power.
	// (c) wowwiki

	//type of this UNIT_FIELD_ATTACK_POWER_MODS is unknown, not even uint32 disabled for now.

	uint32 offset;
	Item *it = NULL;

if(ability && ability->NameHash == SPELL_HASH_FLAMETONGUE_WEAPON)
                return 0;

	if(pAttacker->disarmed && pAttacker->IsPlayer())
	{
		offset=UNIT_FIELD_MINDAMAGE;
		it = TO< Player* >(pAttacker)->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
	}
	else if( weapon_damage_type == MELEE )
		offset = UNIT_FIELD_MINDAMAGE;
	else if( weapon_damage_type == OFFHAND )
		offset = UNIT_FIELD_MINOFFHANDDAMAGE;
	else  // weapon_damage_type == RANGED
		offset = UNIT_FIELD_MINRANGEDDAMAGE;

	float min_damage = pAttacker->GetFloatValue(offset);
	float max_damage = pAttacker->GetFloatValue(offset+1);
	if(it)
	{
		min_damage -= it->GetProto()->Damage[0].Min;
		max_damage -= it->GetProto()->Damage[0].Max;
	}

	float ap = 0;
	float bonus;
	float wspeed;

	if(offset == UNIT_FIELD_MINRANGEDDAMAGE)
	{
		//starting from base attack power then we apply mods on it
		//ap += pAttacker->GetRAP();
		ap += pVictim->RAPvModifier;

		if(!pVictim->IsPlayer())
		{
			uint32 creatType = TO_CREATURE(pVictim)->GetCreatureInfo()->Type;
			ap += (float)pAttacker->CreatureRangedAttackPowerMod[creatType];

			if(pAttacker->IsPlayer())
			{
				min_damage = (min_damage + TO< Player* >(pAttacker)->IncreaseDamageByType[creatType]) * (1 + TO< Player* >(pAttacker)->IncreaseDamageByTypePCT[creatType]);
				max_damage = (max_damage + TO< Player* >(pAttacker)->IncreaseDamageByType[creatType]) * (1 + TO< Player* >(pAttacker)->IncreaseDamageByTypePCT[creatType]);
			}
		}

		if(pAttacker->IsPlayer())
		{
			if(!pAttacker->disarmed)
			{
				it = TO< Player* >(pAttacker)->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED);
				if(it)
					wspeed = (float)it->GetProto()->Delay;
				else
					wspeed = 2000;
			}
			else
				wspeed = (float)pAttacker->GetBaseAttackTime(RANGED);
		}
		else
		{
			wspeed = (float)pAttacker->GetBaseAttackTime(MELEE);
		}

		//ranged weapon normalization.
		if(pAttacker->IsPlayer() && ability)
		{
			if(ability->Effect[0] == SPELL_EFFECT_DUMMYMELEE || ability->Effect[1] == SPELL_EFFECT_DUMMYMELEE || ability->Effect[2] == SPELL_EFFECT_DUMMYMELEE)
			{
				wspeed = 2800;
			}
		}

		//Weapon speed constant in feral forms
		if(pAttacker->IsPlayer())
		{
			if(TO< Player* >(pAttacker)->IsInFeralForm())
			{
				uint8 ss = TO< Player* >(pAttacker)->GetShapeShift();

				if(ss == FORM_CAT)
					wspeed = 1000.0;
				else if(ss == FORM_DIREBEAR || ss == FORM_BEAR)
					wspeed = 2500.0;
			}
		}

		bonus = (wspeed-pAttacker->GetBaseAttackTime(RANGED))/14000.0f*ap;
		min_damage += bonus;
		max_damage += bonus;
	}
	else
	{
		//MinD = AP(28AS-(WS/7))-MaxD
		//starting from base attack power then we apply mods on it
		//ap += pAttacker->GetAP();
		ap += pVictim->APvModifier;

		if(!pVictim->IsPlayer())
		{
			uint32 creatType = TO_CREATURE(pVictim)->GetCreatureInfo()->Type;
			ap += (float)pAttacker->CreatureAttackPowerMod[creatType];

			if(pAttacker->IsPlayer())
			{
				min_damage = (min_damage + TO< Player* >(pAttacker)->IncreaseDamageByType[creatType]) * (1 + TO< Player* >(pAttacker)->IncreaseDamageByTypePCT[creatType]);
				max_damage = (max_damage + TO< Player* >(pAttacker)->IncreaseDamageByType[creatType]) * (1 + TO< Player* >(pAttacker)->IncreaseDamageByTypePCT[creatType]);
			}
		}

		if(pAttacker->IsPlayer())
		{
			if(!pAttacker->disarmed)
			{
				it = TO< Player* >(pAttacker)->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);

				if(it)
					wspeed = (float)it->GetProto()->Delay;
				else
					wspeed = 2000;
			}
			else
				wspeed = (float)pAttacker->GetBaseAttackTime(MELEE);
		}
		else
		{
			wspeed = (float)pAttacker->GetBaseAttackTime(MELEE);
		}

		//Normalized weapon damage checks.
		if(pAttacker->IsPlayer() && ability)
		{
			if(ability->Effect[0] == SPELL_EFFECT_DUMMYMELEE || ability->Effect[1] == SPELL_EFFECT_DUMMYMELEE || ability->Effect[2] == SPELL_EFFECT_DUMMYMELEE)
			{
				it = TO< Player* >(pAttacker)->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);

				if(it)
				{
					if(it->GetProto()->Class == 2) //weapon
					{
						if(it->GetProto()->InventoryType == INVTYPE_2HWEAPON) wspeed = 3300;
						else if(it->GetProto()->SubClass == 15) wspeed = 1700;
						else wspeed = 2400;
					}
				}
			}
		}

		//Weapon speed constant in feral forms
		if(pAttacker->IsPlayer())
		{
			if(TO< Player* >(pAttacker)->IsInFeralForm())
			{
				uint8 ss = TO< Player* >(pAttacker)->GetShapeShift();

				if(ss == FORM_CAT)
					wspeed = 1000.0;
				else if(ss == FORM_DIREBEAR || ss == FORM_BEAR)
					wspeed = 2500.0;
			}
		}

		if (offset == UNIT_FIELD_MINDAMAGE)
			bonus = (wspeed-pAttacker->GetBaseAttackTime(MELEE))/14000.0f*ap;
		else
			bonus = (wspeed-pAttacker->GetBaseAttackTime(OFFHAND))/14000.0f*ap;
		min_damage += bonus;
		max_damage += bonus;
	}
	float diff = fabs(max_damage - min_damage);
	float result = min_damage;

	if(diff >= 1)
		result += RandomFloat(diff);

	if(result >= 0)
	{
		if( pAttacker->IsPlayer() && TO_PLAYER(pAttacker)->m_outStealthDamageBonusTimer )
		{
			if( (uint32)UNIXTIME >= TO_PLAYER(pAttacker)->m_outStealthDamageBonusTimer )
				TO_PLAYER(pAttacker)->m_outStealthDamageBonusTimer = 0;
			else
				result *= ((TO_PLAYER(pAttacker)->m_outStealthDamageBonusPct) / 100.0f) + 1.0f;
		}

		return float2int32(result);
	}

	return 0;
}

bool isEven (int num)
{
	if ((num%2)== 0)
	{
		return true;
	}

	return false;
}



