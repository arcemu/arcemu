/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
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

#ifdef USE_SPECIFIC_AIAGENTS

#ifndef MAX_INT
	#define MAX_INT ( 1 << 30 )
#endif 
#ifndef PI
	#define PI ( 3.14f )	//yeah i know this is low precision
#endif 

#define MAX(a,b) (a>b?a:b)

//will get bigger if it is better to cast this spell then some other version
float GetSpellEficiencyFactor(SpellEntry *sp)
{
	SpellCastTime *sd = dbcSpellCastTime.LookupEntry( sp->CastingTimeIndex );
	return (float)(sp->EffectBasePoints[0]) / ( GetCastTime( sd ) * 2 + sp->manaCost + 1) ;
}

void AiAgentHealSupport::Init(Unit *un, AIType at, MovementType mt, Unit *owner)
{
	m_Unit->SetUInt32Value( UNIT_FIELD_BASE_HEALTH , m_Unit->GetUInt32Value( UNIT_FIELD_HEALTH ));
	m_Unit->SetUInt32Value( UNIT_FIELD_BASE_MANA , m_Unit->GetUInt32Value( UNIT_FIELD_POWER1 ));

	AIInterface::Init(un,at,mt,owner);//run the original init and we add our custom stuff too

	m_fallowAngle = 2 * PI - PI / 6;
	FollowDistance = 5 ; //we are support, we stand behind our master 

	DifficultyLevel = m_PetOwner->GetUInt32Value(UNIT_FIELD_LEVEL) / HealSpellLevels;
	if( DifficultyLevel > HealSpellLevels - 2 )
		DifficultyLevel = HealSpellLevels - 2;
	//scale health and mana
	m_Unit->SetUInt32Value( UNIT_FIELD_MAXHEALTH , m_Unit->GetUInt32Value( UNIT_FIELD_BASE_HEALTH ) * DifficultyLevel * CREATURE_STATS_SCALE_WITH_DIFFICULTY );
	m_Unit->SetUInt32Value( UNIT_FIELD_MAXPOWER1 , m_Unit->GetUInt32Value( UNIT_FIELD_BASE_MANA ) * DifficultyLevel * CREATURE_STATS_SCALE_WITH_DIFFICULTY );
	m_Unit->SetUInt32Value( UNIT_FIELD_HEALTH , m_Unit->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );
	m_Unit->SetUInt32Value( UNIT_FIELD_POWER1 , m_Unit->GetUInt32Value( UNIT_FIELD_MAXPOWER1 ) );

	uint8 race = m_PetOwner->getRace();

	if( race == RACE_HUMAN || race == RACE_DWARF || race == RACE_NIGHTELF || race == RACE_GNOME || race == RACE_DRAENEI )
		Owner_side = OWNER_SIDE_ALIANCE;
	else
		Owner_side = OWNER_SIDE_HORDE;

	last_time_full_health = true; //before we start healing we should try to quick augment our master

	m_defend_self = dbcSpell.LookupEntry( 642 ); // Divine Shield 1

	uint32 local_itr,local_itr2;

	memset(m_HealSpells,NULL,HealSpellCount*HealSpellLevels*sizeof(SpellEntry*));
	memset(m_HealSpellsEficiency,NULL,HealSpellCount*HealSpellLevels*sizeof(float));

	//we are using const lists. You don't like it ? Write your own AI :P
	//owner : Lesser Heal, Flash Heal, heal, Healing Touch, Greater Heal, Great Heal, Heal Brethren, Debuff Uber Heal Over Time
	//group : Prayer of Healing, Chain Heal, Healing Aura
	//augment : Uber Heal Over Time, Spiritual Healing,Healing Light, renew
	//augment : renew, power word shield, Dampen Magic, Blessing of Sanctuary 
	local_itr = 0;
	local_itr2 = 0;
	m_PartySpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 596 ); // Prayer of Healing 1
	m_PartySpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 10960 ); // Prayer of Healing 1
	m_PartySpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 10961 ); // Prayer of Healing 1
	m_PartySpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 25308 ); // Prayer of Healing 1

	local_itr++;
	local_itr2 = 0;
	m_PartySpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 1064 ); // Chain Heal 1
	m_PartySpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 10623 ); // Chain Heal 1
	m_PartySpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 25423 ); // Chain Heal 1
	m_PartySpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 33642 ); // Chain Heal 1

	local_itr++;
	local_itr2 = 0;
	m_PartySpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 5607 ); // Healing Aura 1
	m_PartySpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 5016 ); // Healing Aura 1
	m_PartySpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 15870 ); // Healing Aura 1
	m_PartySpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 11900 ); // Healing Aura 1

	local_itr = 0;
	local_itr2 = 0;
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 139 ); // Renew 1
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 6076 ); // Renew 1
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 10927 ); // Renew 1
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 25221 ); // Renew 1
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 37260 ); // Renew 1
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 36969 ); // Renew 2

	local_itr++;
	local_itr2 = 0;
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 17 ); // Power Word: Shield 1
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 3747 ); // Power Word: Shield 1
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 10898 ); // Power Word: Shield 1
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 10901 ); // Power Word: Shield 1
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 25217 ); // Power Word: Shield 1
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 25218 ); // Power Word: Shield 2

	local_itr++;
	local_itr2 = 0;
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 604 ); // Dampen Magic 1
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 8450 ); // Dampen Magic 1
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 8451 ); // Dampen Magic 1
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 10173 ); // Dampen Magic 1
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 10174 ); // Dampen Magic 1
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 33944 ); // Dampen Magic 2

	local_itr++;
	local_itr2 = 0;
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 20911 ); // Blessing of Sanctuary 1
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 20913 ); // Blessing of Sanctuary 1
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 20914 ); // Blessing of Sanctuary 1
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 20914 ); // Blessing of Sanctuary 1
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 27168 ); // Blessing of Sanctuary 1
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 27169 ); // Blessing of Sanctuary 2

	local_itr = 0;
	local_itr2 = 0;
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 2050 ); // Lesser Heal 1
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 2050 ); // Lesser Heal 1
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 2050 ); // Lesser Heal 1
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 2050 ); // Lesser Heal 1
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 2050 ); // Lesser Heal 1
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 2052 ); // Lesser Heal 2
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 2052 ); // Lesser Heal 2
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 2052 ); // Lesser Heal 2
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 2052 ); // Lesser Heal 2
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 2052 ); // Lesser Heal 2
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 2053 ); // Lesser Heal 3
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 2053 ); // Lesser Heal 3
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 2053 ); // Lesser Heal 3
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 2053 ); // Lesser Heal 3
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 2053 ); // Lesser Heal 3
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);

	local_itr++;
	local_itr2 = 0;
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 2061 ); // Flash Heal 1
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 2061 ); // Flash Heal 1
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 9472 ); // Flash Heal 2
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 9472 ); // Flash Heal 2
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 9473 ); // Flash Heal 3
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 9473 ); // Flash Heal 3
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 10915 ); // Flash Heal 4
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 10915 ); // Flash Heal 4
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 10915 ); // Flash Heal 4
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 10916 ); // Flash Heal 5
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 10916 ); // Flash Heal 5
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 10916 ); // Flash Heal 5
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 10917 ); // Flash Heal 6
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 10917 ); // Flash Heal 6
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 10917 ); // Flash Heal 6
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);

	local_itr++;
	local_itr2 = 0;
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 2054 ); // Heal 1
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 2054 ); // Heal 1
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 2054 ); // Heal 1
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 2055 ); // Heal 2
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 2055 ); // Heal 2
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 2055 ); // Heal 2
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 6063 ); // Heal 3
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 6063 ); // Heal 3
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 6063 ); // Heal 3
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 6064 ); // Heal 4
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 6064 ); // Heal 4
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 6064 ); // Heal 4
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 8812 ); // Heal 5
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 8812 ); // Heal 5
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 8812 ); // Heal 5
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);

	local_itr++;
	local_itr2 = 0;
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 5185 ); // Healing Touch 1
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 5186 ); // Healing Touch 2
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 5187 ); // Healing Touch 3
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 5188 ); // Healing Touch 4
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 5189 ); // Healing Touch 5
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 6778 ); // Healing Touch 6
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 8903 ); // Healing Touch 7
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 9758 ); // Healing Touch 8
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 9888 ); // Healing Touch 9
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 9889 ); // Healing Touch 10
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 25297 ); // Healing Touch 11
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 26978 ); // Healing Touch 12
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 26979 ); // Healing Touch 13
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 27527 ); // Healing Touch 14
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 29339 ); // Healing Touch 15
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);

	local_itr++;
	local_itr2 = 0;
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 2060 ); // Greater Heal 1
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 10963 ); // Greater Heal 2
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 10964 ); // Greater Heal 3
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 10965 ); // Greater Heal 4
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 10965 ); // Greater Heal 4
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 25210 ); // Greater Heal 5
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 25210 ); // Greater Heal 5
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 25213 ); // Greater Heal 6
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 25213 ); // Greater Heal 6
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 34119 ); // Greater Heal 7
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 34119 ); // Greater Heal 7
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 35096 ); // Greater Heal 8
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 35096 ); // Greater Heal 8
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 38580 ); // Greater Heal 9
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 38580 ); // Greater Heal 9
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);

	local_itr++;
	local_itr2 = HealSpellLevels / 2;
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 28306 ); // Great Heal 1
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 28306 ); // Great Heal 1
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 28306 ); // Great Heal 1
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 28306 ); // Great Heal 1
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 28306 ); // Great Heal 1
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 33387 ); // Great Heal 2
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 33387 ); // Great Heal 2
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 33387 ); // Great Heal 2
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 33387 ); // Great Heal 2
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 33387 ); // Great Heal 2
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 24208 ); // Great Heal 3
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 24208 ); // Great Heal 3
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 24208 ); // Great Heal 3
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 24208 ); // Great Heal 3
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 24208 ); // Great Heal 3
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);

	local_itr++;
	local_itr2 = HealSpellLevels - 2; //kinda uber spells :P
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 26565 ); // Heal Brethren 1
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);

	local_itr++;
	local_itr2 = HealSpellLevels - 2; //kinda uber spells :P
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 30839 ); // Debuff Uber Heal Over Time 1
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);

/*	//group : Prayer of Healing, Chain Heal, Healing Aura, Powerful Healing Ward
	//augment : Uber Heal Over Time, Spiritual Healing,Healing Light, renew
	sp = dbcSpell.LookupEntry( 122 ); // frost nova 1
	m_DefendSpells.push_back( sp ); */

	revive_spell = dbcSpell.LookupEntry( 2006 ); // resurrection 1
}

bool AiAgentHealSupport::CheckCanCast(SpellEntry *sp, Unit *target)
{
	uint32 Time_Now = getMSTime();
	if ( !sp )
		return false;

	if ( !target )
		return false;

	if( m_Unit->GetUInt32Value(UNIT_FIELD_POWER1) < sp->manaCost )
		return false; //we do not have enough juice

	CooldownMap::iterator itr = spell_cooldown_map.find( sp->Id );
	if( itr != spell_cooldown_map.end() && itr->second >= Time_Now )
		return false; // this spell is still cooling down

	return true;
}

SpellEntry*	AiAgentHealSupport::Get_Best_Heal_Spell(Unit *for_unit)
{
	uint32 now_health = for_unit->GetUInt32Value(UNIT_FIELD_HEALTH);
	uint32 max_health = for_unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH);
	uint32 health_to_heal = max_health - now_health;
	//get best spell to match our needs :D
	float best_match= 1 << 30 ;
	uint32 selected_spell_index = MAX_INT;
	for(uint32 i=0;i<HealSpellCount-1;i++)
#ifdef SPELL_EFF_PCT_SCALE_WITH_DIFFICULTY
		if( m_HealSpells[DifficultyLevel][i]
			&& SPELL_EFF_PCT_SCALE_WITH_DIFFICULTY * DifficultyLevel * m_HealSpells[DifficultyLevel][i]->EffectBasePoints[0] <= health_to_heal
			&& SPELL_EFF_PCT_SCALE_WITH_DIFFICULTY * DifficultyLevel * m_HealSpells[DifficultyLevel][i]->EffectBasePoints[0] >= health_to_heal)
#else
		if( m_HealSpells[DifficultyLevel][spell_slector]
			&& m_HealSpells[DifficultyLevel][i]->EffectBasePoints[0] <= health_to_heal
			&& m_HealSpells[DifficultyLevel][i+1]->EffectBasePoints[0] >= health_to_heal
#endif
		{
			uint32 spell_slector = ( i + 1 ) % HealSpellCount;
			if(	CheckCanCast ( m_HealSpells[DifficultyLevel][spell_slector] , for_unit) )
			{ 
#ifdef SPELL_EFF_PCT_SCALE_WITH_DIFFICULTY
				float cur_efficiency = ( SPELL_EFF_PCT_SCALE_WITH_DIFFICULTY * DifficultyLevel * m_HealSpellsEficiency[DifficultyLevel][spell_slector] * MAX( 1.0f,( health_to_heal - m_HealSpells[DifficultyLevel][spell_slector]->EffectBasePoints[0] ) ) );
#else
				float cur_efficiency = ( m_HealSpellsEficiency[DifficultyLevel][spell_slector] * max(1,( health_to_heal - m_HealSpells[DifficultyLevel][spell_slector]->EffectBasePoints[0] ) ) );
#endif
				if( cur_efficiency <  best_match)
				{
printf("found a better match with id %u , old price %f new price %f\n",m_HealSpells[DifficultyLevel][spell_slector]->Id,best_match,cur_efficiency);
					best_match = cur_efficiency;
					selected_spell_index = spell_slector;
				}
			}
		}
	if( selected_spell_index != MAX_INT )
	{
printf("selected spell with id %u and price %f\n",m_HealSpells[ DifficultyLevel ][ selected_spell_index ]->Id,best_match);
		return m_HealSpells[ DifficultyLevel ][ selected_spell_index ];
	}
	return NULL ;
}

void AiAgentHealSupport::_UpdateCombat(uint32 p_time)
{
	m_nextTarget = m_PetOwner;

printf("we have just overwritten updatecombat\n");

	if( !m_nextTarget )
		return; //oh noez, our master has abandoned us ! Where is te luv ?

printf("we do have a target\n");

	if( m_Unit->isCasting() )
		return; // we are already supporting someone ...get back later

	//for fun : mimic master standstate. Note that this might give strange results
	if( m_PetOwner->GetStandState() != m_Unit->GetStandState() )
		m_Unit->SetStandState( m_PetOwner->GetStandState() );

	//we should be at same level at owner so we profit of fighting formulas same as owner
	if(	m_Unit->GetUInt32Value( UNIT_FIELD_LEVEL ) != m_PetOwner->GetUInt32Value( UNIT_FIELD_LEVEL ) )
		m_Unit->SetUInt32Value( UNIT_FIELD_LEVEL, m_PetOwner->GetUInt32Value( UNIT_FIELD_LEVEL ) );

printf("we are not casting\n");

	uint32 new_DifficultyLevel = m_PetOwner->GetUInt32Value(UNIT_FIELD_LEVEL) / HealSpellLevels;
	if( new_DifficultyLevel != new_DifficultyLevel)
	{
		if( new_DifficultyLevel > HealSpellLevels - 2 )
			new_DifficultyLevel = HealSpellLevels - 2;
		DifficultyLevel = new_DifficultyLevel;
		//scale health and mana.when we level we max out our stats
		m_Unit->SetUInt32Value( UNIT_FIELD_MAXHEALTH , m_Unit->GetUInt32Value( UNIT_FIELD_BASE_HEALTH ) * new_DifficultyLevel * CREATURE_STATS_SCALE_WITH_DIFFICULTY );
		m_Unit->SetUInt32Value( UNIT_FIELD_MAXPOWER1 , m_Unit->GetUInt32Value( UNIT_FIELD_BASE_MANA ) * new_DifficultyLevel * CREATURE_STATS_SCALE_WITH_DIFFICULTY );
		m_Unit->SetUInt32Value( UNIT_FIELD_HEALTH , m_Unit->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );
		m_Unit->SetUInt32Value( UNIT_FIELD_POWER1 , m_Unit->GetUInt32Value( UNIT_FIELD_MAXPOWER1 ) );
	}

	//if owner is mounted then we mount too. Speed is not set though
	if( m_PetOwner->GetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID ) && m_Unit->GetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID ) == 0 )
	{
		if( Owner_side == OWNER_SIDE_ALIANCE )
			m_Unit->SetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID, HELPER_MOUNT_A_DISPLAY );
		else
			m_Unit->SetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID, OWNER_SIDE_HORDE );
		m_moveSprint =  true;
	}
	else if( m_PetOwner->GetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID ) == 0 && m_Unit->GetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID ) != 0 )
	{
		m_Unit->SetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID, 0 );
		m_moveSprint = false;
	}

	if ( m_PetOwner->GetHealthPct() == 100 )
		last_time_full_health = true;
	
	uint32 Time_Now = getMSTime();

	SpellCastTargets targets;
	m_castingSpell = NULL;

	//poor thing died. Res him
	if( !m_nextTarget->isAlive() 
		&& m_Unit->GetUInt32Value(UNIT_FIELD_POWER1) >= revive_spell->manaCost
		&& spell_cooldown_map[ revive_spell->Id ] < Time_Now
		)
	{
		m_castingSpell = revive_spell;
		setSpellTargets(m_castingSpell, m_PetOwner);
printf("master died, we are going to resurect him\n");
	}

	if ( last_time_full_health == true && m_PetOwner->GetHealthPct() != 100 && m_castingSpell == NULL )
	{
		uint32 augment_DifficultyLevel = m_PetOwner->GetUInt32Value(UNIT_FIELD_LEVEL) / AugmentSpellCount;
		//pick 1 random augment spell
		uint32 augment_spell = RandomUInt( AugmentSpellLevels );
		if( m_AugmentSpells [ augment_DifficultyLevel ] [ augment_spell ] )
		{
			m_castingSpell = m_AugmentSpells [ augment_DifficultyLevel ] [ augment_spell ];
			setSpellTargets( m_castingSpell, m_PetOwner );
		}
	}

	if ( m_PetOwner->GetHealthPct() < 100 && m_castingSpell == NULL )
	{
printf("master is injured, will try to heal him\n");
		m_castingSpell = Get_Best_Heal_Spell( m_PetOwner );
		if ( m_castingSpell ) 
			setSpellTargets( m_castingSpell, m_PetOwner );
else printf("we were not able to select any heal spells due to power %u or cooldown issues\n",m_Unit->GetUInt32Value(UNIT_FIELD_POWER1));
	}

	if ( m_Unit->GetHealthPct() < 100 && m_castingSpell == NULL )
	{
		if(	!Protect_self() ) //first we try to escape combat
		{
printf("we are injured, will try to heal us\n");
			m_castingSpell = Get_Best_Heal_Spell( m_Unit );
			if ( m_castingSpell ) 
				setSpellTargets( m_castingSpell, m_Unit );
		}
else printf("we were not able to select any heal spells due to power %u or cooldown issues\n",m_Unit->GetUInt32Value(UNIT_FIELD_POWER1));
	}

	if ( m_PetOwner->GetHealthPct() == 100 && m_castingSpell == NULL && m_PetOwner->IsPlayer() && static_cast< Player*>( m_PetOwner )->InGroup())
	{
		uint32 party_DifficultyLevel = m_PetOwner->GetUInt32Value(UNIT_FIELD_LEVEL) / PartySpellCount;
		//pick 1 random augment spell
		uint32 party_spell = RandomUInt( PartySpellLevels );
		if( m_PartySpells [ party_DifficultyLevel ] [ party_spell ] )
		{
			m_castingSpell = m_PartySpells [ party_DifficultyLevel ] [ party_spell ];
			setSpellTargets( m_castingSpell, m_PetOwner );
		}
	}

	if( !m_castingSpell )
		return; //sorry but we are out of juice

printf("we have a spell to cast\n");

	SpellCastTime *sd = dbcSpellCastTime.LookupEntry(m_castingSpell->CastingTimeIndex);

	//do not stop for instant casts
	if(GetCastTime(sd) != 0)
	{
		StopMovement(0);
printf("spell is not instant so we are going to stop movement \n");
	}

	float distance = m_Unit->GetDistanceSq(m_nextTarget);
	if(	distance <= m_castingSpell->base_range_or_radius_sqr || m_castingSpell->base_range_or_radius_sqr == 0 )
	{

printf("we are in range and going to cast spell \n");
		m_AIState = STATE_CASTING;
		
		Spell *nspell = new Spell(m_Unit, m_castingSpell, false, NULL);

#ifdef SPELL_EFF_PCT_SCALE_WITH_DIFFICULTY
		nspell->forced_basepoints[ 0 ] = (uint32)( m_castingSpell->EffectBasePoints[0] * SPELL_EFF_PCT_SCALE_WITH_DIFFICULTY * DifficultyLevel );
#endif

		nspell->prepare( &targets );

		CastSpell( m_Unit, m_castingSpell, targets );

		SetSpellDuration( m_castingSpell );

	}
	else // Target out of Range -> Run to it
	{
printf("we are going to move closer \n");
		m_moveRun = true;
		_CalcDestinationAndMove(m_nextTarget, sqrt( m_castingSpell->base_range_or_radius_sqr ) );
	}

// check if pets regenrate mana, If not then we should implement that too
}

void AiAgentHealSupport::SetSpellDuration(SpellEntry *sp)
{
	if ( !sp )
		return ;

	uint32 Time_Now = getMSTime();
	uint32 cooldowntime;

#ifdef SPELL_COOLD_PCT_SCALE_WITH_DIFFICULTY
	cooldowntime = (uint32) (m_castingSpell->RecoveryTime / ( SPELL_COOLD_PCT_SCALE_WITH_DIFFICULTY * DifficultyLevel ) );
#else
	cooldowntime = m_castingSpell->RecoveryTime;
#endif

    if(sp->DurationIndex)
    {
        SpellDuration *sd=dbcSpellDuration.LookupEntry(sp->DurationIndex);
		uint32 maxduration;
		maxduration = MAX ( sd->Duration1, sd->Duration2 ) ;
		maxduration = MAX ( maxduration , sd->Duration3 );

		if( maxduration > cooldowntime )
			cooldowntime = maxduration; //do not recast aura or heal over time spells on target
	}

	if( cooldowntime )
		spell_cooldown_map[ m_castingSpell->Id ] = Time_Now + cooldowntime;
}

bool AiAgentHealSupport::Protect_self()
{
	if ( CheckCanCast ( m_defend_self , m_Unit ) )
		return true;
	return false;
}

#endif