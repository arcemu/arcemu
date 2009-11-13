/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2009 <http://www.ArcEmu.org/>
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

void CreateDummySpell(uint32 id)
{
	const char * name = "Dummy Trigger";
	SpellEntry * sp = new SpellEntry;
	memset( sp, 0, sizeof(SpellEntry) );
	sp->Id = id;
	sp->Attributes = 384;
	sp->AttributesEx = 268435456;
	sp->AttributesExB = 4;
	sp->CastingTimeIndex = 1;
	sp->procChance = 75;
	sp->rangeIndex = 13;
	sp->EquippedItemClass = uint32(-1);
	sp->Effect[0] = 3;
	sp->EffectImplicitTargetA[0] = 25;
	sp->NameHash = crc32( (const unsigned char*)name, (unsigned int)strlen(name) );
	sp->dmg_multiplier[0] = 1.0f;
	sp->StanceBarOrder = -1;
	dbcSpell.SetRow(id,sp);
	sWorld.dummyspells.push_back(sp);
}

void ApplyNormalFixes()
{
	//Updating spell.dbc
	Log.Notice("World", "Processing %u spells...", dbcSpell.GetNumRows());

	uint32 cnt = dbcSpell.GetNumRows();

	map<uint32, uint32> talentSpells;
	map<uint32,uint32>::iterator talentSpellIterator;
	unsigned int i,j;
	for(i = 0; i < dbcTalent.GetNumRows(); ++i)
	{
		TalentEntry * tal = dbcTalent.LookupRow(i);
		for(j = 0; j < 5; ++j)
			if(tal->RankID[j] != 0)
				talentSpells.insert(make_pair(tal->RankID[j], tal->TalentTree));

	}

	for(uint32 x= 0; x < cnt; x++)
	{
		// Read every SpellEntry row
		SpellEntry * sp = dbcSpell.LookupRow(x);

		uint32 rank = 0;
		uint32 namehash = 0;

		sp->spell_can_crit = true;
		sp->self_cast_only = false;
		sp->apply_on_shapeshift_change = false;
		sp->always_apply = false;
		sp->noproc = false;
		sp->procFlagExtra = 0;

		// hash the name
		//!!!!!!! representing all strings on 32 bits is dangerous. There is a chance to get same hash for a lot of strings ;)
        namehash = crc32((const unsigned char*)sp->Name, (unsigned int)strlen(sp->Name));
		sp->NameHash = namehash; //need these set before we start processing spells

		float radius = std::max(::GetRadius(dbcSpellRadius.LookupEntry(sp->EffectRadiusIndex[0])),::GetRadius(dbcSpellRadius.LookupEntry(sp->EffectRadiusIndex[1])));
		radius = std::max(::GetRadius(dbcSpellRadius.LookupEntry(sp->EffectRadiusIndex[2])),radius);
		radius = std::max(GetMaxRange(dbcSpellRange.LookupEntry(sp->rangeIndex)),radius);
		sp->base_range_or_radius_sqr = radius*radius;

		sp->ai_target_type = GetAiTargetType( sp );
		// NEW SCHOOLS AS OF 2.4.0:
		/* (bitwise)
		SCHOOL_NORMAL = 1,
		SCHOOL_HOLY   = 2,
		SCHOOL_FIRE   = 4,
		SCHOOL_NATURE = 8,
		SCHOOL_FROST  = 16,
		SCHOOL_SHADOW = 32,
		SCHOOL_ARCANE = 64
		*/

		// Save School as SchoolMask, and set School as an index
		sp->SchoolMask = sp->School;
		for (i= 0; i<8; i++)
		{
			if (sp->School & (1<<i))
			{
				sp->School = i;
				break;
			}
		}

		// correct caster/target aura states
		if( sp->CasterAuraState > 1 )
			sp->CasterAuraState = 1 << ( sp->CasterAuraState - 1 );

		if( sp->TargetAuraState > 1 )
			sp->TargetAuraState = 1 << ( sp->TargetAuraState - 1 );

		// apply on shapeshift change
		if( sp->NameHash == SPELL_HASH_TRACK_HUMANOIDS )
			sp->apply_on_shapeshift_change = true;

		if( sp->NameHash == SPELL_HASH_BLOOD_FURY || sp->NameHash == SPELL_HASH_SHADOWSTEP )
			sp->always_apply = true;

		//there are some spells that change the "damage" value of 1 effect to another : devastate = bonus first then damage
		//this is a total bullshit so remove it when spell system supports effect overwriting
		for( uint32 col1_swap = 0; col1_swap < 2 ; col1_swap++ )
			for( uint32 col2_swap = col1_swap ; col2_swap < 3 ; col2_swap++ )
				if( sp->Effect[col1_swap] == SPELL_EFFECT_WEAPON_PERCENT_DAMAGE && sp->Effect[col2_swap] == SPELL_EFFECT_DUMMYMELEE )
				{
					uint32 temp;
					float ftemp;
					temp = sp->Effect[col1_swap];			sp->Effect[col1_swap] = sp->Effect[col2_swap] ;						sp->Effect[col2_swap] = temp;
					temp = sp->EffectDieSides[col1_swap];	sp->EffectDieSides[col1_swap] = sp->EffectDieSides[col2_swap] ;		sp->EffectDieSides[col2_swap] = temp;
					temp = sp->EffectBaseDice[col1_swap];	sp->EffectBaseDice[col1_swap] = sp->EffectBaseDice[col2_swap] ;		sp->EffectBaseDice[col2_swap] = temp;
					ftemp = sp->EffectDicePerLevel[col1_swap];			sp->EffectDicePerLevel[col1_swap] = sp->EffectDicePerLevel[col2_swap] ;				sp->EffectDicePerLevel[col2_swap] = ftemp;
					ftemp = sp->EffectRealPointsPerLevel[col1_swap];	sp->EffectRealPointsPerLevel[col1_swap] = sp->EffectRealPointsPerLevel[col2_swap] ;	sp->EffectRealPointsPerLevel[col2_swap] = ftemp;
					temp = sp->EffectBasePoints[col1_swap];	sp->EffectBasePoints[col1_swap] = sp->EffectBasePoints[col2_swap] ;	sp->EffectBasePoints[col2_swap] = temp;
					temp = sp->EffectMechanic[col1_swap];	sp->EffectMechanic[col1_swap] = sp->EffectMechanic[col2_swap] ;	sp->EffectMechanic[col2_swap] = temp;
					temp = sp->EffectImplicitTargetA[col1_swap];	sp->EffectImplicitTargetA[col1_swap] = sp->EffectImplicitTargetA[col2_swap] ;	sp->EffectImplicitTargetA[col2_swap] = temp;
					temp = sp->EffectImplicitTargetB[col1_swap];	sp->EffectImplicitTargetB[col1_swap] = sp->EffectImplicitTargetB[col2_swap] ;	sp->EffectImplicitTargetB[col2_swap] = temp;
					temp = sp->EffectRadiusIndex[col1_swap];	sp->EffectRadiusIndex[col1_swap] = sp->EffectRadiusIndex[col2_swap] ;	sp->EffectRadiusIndex[col2_swap] = temp;
					temp = sp->EffectApplyAuraName[col1_swap];	sp->EffectApplyAuraName[col1_swap] = sp->EffectApplyAuraName[col2_swap] ;	sp->EffectApplyAuraName[col2_swap] = temp;
					temp = sp->EffectAmplitude[col1_swap];		sp->EffectAmplitude[col1_swap] = sp->EffectAmplitude[col2_swap] ;	sp->EffectAmplitude[col2_swap] = temp;
					ftemp = sp->EffectMultipleValue[col1_swap];		sp->EffectMultipleValue[col1_swap] = sp->EffectMultipleValue[col2_swap] ;	sp->EffectMultipleValue[col2_swap] = ftemp;
					temp = sp->EffectChainTarget[col1_swap];	sp->EffectChainTarget[col1_swap] = sp->EffectChainTarget[col2_swap] ;	sp->EffectChainTarget[col2_swap] = temp;
					temp = sp->EffectMiscValue[col1_swap];		sp->EffectMiscValue[col1_swap] = sp->EffectMiscValue[col2_swap] ;	sp->EffectMiscValue[col2_swap] = temp;
					temp = sp->EffectTriggerSpell[col1_swap];	sp->EffectTriggerSpell[col1_swap] = sp->EffectTriggerSpell[col2_swap] ;	sp->EffectTriggerSpell[col2_swap] = temp;
					ftemp = sp->EffectPointsPerComboPoint[col1_swap];	sp->EffectPointsPerComboPoint[col1_swap] = sp->EffectPointsPerComboPoint[col2_swap] ;	sp->EffectPointsPerComboPoint[col2_swap] = ftemp;
				}

		for(uint32 b= 0;b<3;++b)
		{
			if(sp->EffectTriggerSpell[b] != 0 && dbcSpell.LookupEntryForced(sp->EffectTriggerSpell[b]) == NULL)
			{
				/* proc spell referencing non-existent spell. create a dummy spell for use w/ it. */
				CreateDummySpell(sp->EffectTriggerSpell[b]);
			}

			if( sp->Attributes & ATTRIBUTES_ONLY_OUTDOORS && sp->EffectApplyAuraName[b] == SPELL_AURA_MOUNTED )
			{
				sp->Attributes &= ~ATTRIBUTES_ONLY_OUTDOORS;
			}
		}

		if(!strcmp(sp->Name, "Hearthstone") || !strcmp(sp->Name, "Stuck") || !strcmp(sp->Name, "Astral Recall"))
			sp->self_cast_only = true;

		sp->proc_interval = 0;//trigger at each event
		sp->c_is_flags = 0;
		sp->spell_coef_flags = 0;
		sp->Base_coef_override = 0;
		sp->Misc_coef_override = 0;
		sp->AP_coef = 0;
		sp->RAP_coef = 0;

		talentSpellIterator = talentSpells.find(sp->Id);
		if(talentSpellIterator == talentSpells.end())
			sp->talent_tree = 0;
		else
			sp->talent_tree = talentSpellIterator->second;

		// parse rank text
		if( !sscanf( sp->Rank, "Rank %d", (unsigned int*)&rank) )
			rank = 0;

		//Rogue: Poison time fix
		if( strstr( sp->Name, "Crippling Poison") && sp->Effect[0] == 54 )
			sp->EffectBasePoints[0] = 3599;
		if( strstr( sp->Name, "Mind-numbing Poison") && sp->Effect[0] == 54 )
			sp->EffectBasePoints[0] = 3599;
		if( strstr( sp->Name, "Instant Poison") && sp->Effect[0] == 54 )
			sp->EffectBasePoints[0] = 3599;
		if( strstr( sp->Name, "Deadly Poison") && sp->Effect[0] == 54 )
			sp->EffectBasePoints[0] = 3599;
		if( strstr( sp->Name, "Wound Poison") && sp->Effect[0] == 54 )
			sp->EffectBasePoints[0] = 3599;
		if( strstr( sp->Name, "Anesthetic Poison") && sp->Effect[0] == 54 )
			sp->EffectBasePoints[0] = 3599;

        if( strstr( sp->Name, "Sharpen Blade") && sp->Effect[0] == 54 ) //All BS stones
            sp->EffectBasePoints[0] = 3599;

		//these mostly do not mix so we can use else
        // look for seal, etc in name
        if( strstr( sp->Name, "Seal of"))
            sp->BGR_one_buff_on_target |= SPELL_TYPE_SEAL;
		else if( strstr( sp->Name, "Hand of"))
            sp->BGR_one_buff_on_target |= SPELL_TYPE_HAND;
        else if( !strstr( sp->Name, "Improved Blessing") && strstr( sp->Name, "Blessing"))
            sp->BGR_one_buff_on_target |= SPELL_TYPE_BLESSING;
        else if( strstr( sp->Name, "Curse"))
            sp->BGR_one_buff_on_target |= SPELL_TYPE_CURSE;
        else if( !strstr( sp->Name, "Improved Corruption") && !strstr( sp->Name, "Empowered Blessing") && strstr( sp->Name, "Corruption"))
            sp->BGR_one_buff_on_target |= SPELL_TYPE_CORRUPTION;
        else if( strstr( sp->Name, "Aspect"))
            sp->BGR_one_buff_on_target |= SPELL_TYPE_ASPECT;
        else if( strstr( sp->Name, "Sting") || strstr( sp->Name, "sting"))
            sp->BGR_one_buff_on_target |= SPELL_TYPE_STING;
        // don't break armor items!
        else if(strstr( sp->Name, "Fel Armor") || strstr( sp->Name, "Frost Armor") || strstr( sp->Name, "Ice Armor") || strstr( sp->Name, "Mage Armor") || strstr( sp->Name, "Molten Armor") || strstr( sp->Name, "Demon Skin") || strstr( sp->Name, "Demon Armor"))
            sp->BGR_one_buff_on_target |= SPELL_TYPE_ARMOR;
        else if( strstr( sp->Name, "Aura")
			&& !strstr( sp->Name, "Trueshot") && !strstr( sp->Name, "Moonkin")
			&& !strstr( sp->Name, "Crusader") && !strstr( sp->Name, "Sanctity") && !strstr( sp->Name, "Devotion") && !strstr( sp->Name, "Retribution") && !strstr( sp->Name, "Concentration") && !strstr( sp->Name, "Shadow Resistance") && !strstr( sp->Name, "Frost Resistance") && !strstr( sp->Name, "Fire Resistance")
			)
            sp->BGR_one_buff_on_target |= SPELL_TYPE_AURA;
		else if( strstr( sp->Name, "Track")==sp->Name)
            sp->BGR_one_buff_on_target |= SPELL_TYPE_TRACK;
		else if( namehash == SPELL_HASH_GIFT_OF_THE_WILD || namehash == SPELL_HASH_MARK_OF_THE_WILD )
            sp->BGR_one_buff_on_target |= SPELL_TYPE_MARK_GIFT;
		else if( namehash == SPELL_HASH_IMMOLATION_TRAP || namehash == SPELL_HASH_FREEZING_TRAP || namehash == SPELL_HASH_FROST_TRAP || namehash == SPELL_HASH_EXPLOSIVE_TRAP || namehash == SPELL_HASH_SNAKE_TRAP )
            sp->BGR_one_buff_on_target |= SPELL_TYPE_HUNTER_TRAP;
		else if( namehash == SPELL_HASH_ARCANE_INTELLECT || namehash == SPELL_HASH_ARCANE_BRILLIANCE )
            sp->BGR_one_buff_on_target |= SPELL_TYPE_MAGE_INTEL;
		else if( namehash == SPELL_HASH_AMPLIFY_MAGIC || namehash == SPELL_HASH_DAMPEN_MAGIC )
            sp->BGR_one_buff_on_target |= SPELL_TYPE_MAGE_MAGI;
		else if( namehash == SPELL_HASH_FIRE_WARD || namehash == SPELL_HASH_FROST_WARD )
            sp->BGR_one_buff_on_target |= SPELL_TYPE_MAGE_WARDS;
		else if( namehash == SPELL_HASH_SHADOW_PROTECTION || namehash == SPELL_HASH_PRAYER_OF_SHADOW_PROTECTION )
            sp->BGR_one_buff_on_target |= SPELL_TYPE_PRIEST_SH_PPROT;
		else if( namehash == SPELL_HASH_WATER_SHIELD || namehash == SPELL_HASH_EARTH_SHIELD || namehash == SPELL_HASH_LIGHTNING_SHIELD )
            sp->BGR_one_buff_on_target |= SPELL_TYPE_SHIELD;
		else if( namehash == SPELL_HASH_POWER_WORD__FORTITUDE || namehash == SPELL_HASH_PRAYER_OF_FORTITUDE )
            sp->BGR_one_buff_on_target |= SPELL_TYPE_FORTITUDE;
		else if( namehash == SPELL_HASH_DIVINE_SPIRIT || namehash == SPELL_HASH_PRAYER_OF_SPIRIT )
            sp->BGR_one_buff_on_target |= SPELL_TYPE_SPIRIT;
		else if( strstr( sp->Name, "Immolate") )
			sp->BGR_one_buff_on_target |= SPELL_TYPE_WARLOCK_IMMOLATE;
		else if( strstr( sp->Name, "Amplify Magic") || strstr( sp->Name, "Dampen Magic") )
			sp->BGR_one_buff_on_target |= SPELL_TYPE_MAGE_AMPL_DUMP;
        else if( strstr( sp->Description, "Battle Elixir") )
            sp->BGR_one_buff_on_target |= SPELL_TYPE_ELIXIR_BATTLE;
        else if( strstr( sp->Description, "Guardian Elixir") )
            sp->BGR_one_buff_on_target |= SPELL_TYPE_ELIXIR_GUARDIAN;
        else if( strstr( sp->Description, "Battle and Guardian elixir") )
            sp->BGR_one_buff_on_target |= SPELL_TYPE_ELIXIR_FLASK;
		else if( namehash == SPELL_HASH_HUNTER_S_MARK )		// hunter's mark
			sp->BGR_one_buff_on_target |= SPELL_TYPE_HUNTER_MARK;
        else if( namehash == SPELL_HASH_COMMANDING_SHOUT || namehash == SPELL_HASH_BATTLE_SHOUT )
            sp->BGR_one_buff_on_target |= SPELL_TYPE_WARRIOR_SHOUT;
		else if( strstr( sp->Description, "Finishing move") == sp->Description )
			sp->c_is_flags |= SPELL_FLAG_IS_FINISHING_MOVE;
		if( IsDamagingSpell( sp ) )
			sp->c_is_flags |= SPELL_FLAG_IS_DAMAGING;
		if( IsHealingSpell( sp ) )
			sp->c_is_flags |= SPELL_FLAG_IS_HEALING;
		if( IsTargetingStealthed( sp ) )
			sp->c_is_flags |= SPELL_FLAG_IS_TARGETINGSTEALTHED;

		if( sp->NameHash == SPELL_HASH_HEMORRHAGE )
			sp->c_is_flags |= SPELL_FLAG_IS_MAXSTACK_FOR_DEBUFF;

		//stupid spell ranking problem
		if( sp->spellLevel == 0 )
		{
			uint32 new_level = 0;

			if( strstr( sp->Name, "Apprentice ") )
				new_level = 1;
			else if( strstr( sp->Name, "Journeyman ") )
				new_level = 2;
			else if( strstr( sp->Name, "Expert ") )
				new_level = 3;
			else if( strstr( sp->Name, "Artisan ") )
				new_level = 4;
			else if( strstr( sp->Name, "Master ") )
				new_level = 5;
			else if( strstr( sp->Name, "Grand Master ") )
				new_level = 6;

			if( new_level != 0 )
			{
				uint32 teachspell = 0;
				if( sp->Effect[0] == SPELL_EFFECT_LEARN_SPELL )
					teachspell = sp->EffectTriggerSpell[0];
				else if( sp->Effect[1] == SPELL_EFFECT_LEARN_SPELL )
					teachspell = sp->EffectTriggerSpell[1];
				else if( sp->Effect[2] == SPELL_EFFECT_LEARN_SPELL )
					teachspell = sp->EffectTriggerSpell[2];

				if( teachspell )
				{
					SpellEntry *spellInfo;
					spellInfo = dbcSpell.LookupEntryForced(teachspell);
					spellInfo->spellLevel = new_level;
					sp->spellLevel = new_level;
				}
			}
		}

		// find diminishing status
		sp->DiminishStatus = GetDiminishingGroup( namehash );

		//another grouping rule

		//Quivers, Ammo Pouches and Thori'dal the Star's Fury
		if( ( namehash == SPELL_HASH_HASTE && sp->Attributes & 0x10000 ) || sp->Id == 44972 )
		{
			sp->Attributes &= ~ATTRIBUTES_PASSIVE;//Otherwise we couldn't remove them
			sp->BGR_one_buff_on_target |= SPELL_TYPE_QUIVER_HASTE;
		}

		switch( namehash )
		{
		case SPELL_HASH_DEVOTION_AURA:
		case SPELL_HASH_RETRIBUTION_AURA:
		case SPELL_HASH_CONCENTRATION_AURA:
		case SPELL_HASH_SHADOW_RESISTANCE_AURA:
		case SPELL_HASH_FIRE_RESISTANCE_AURA:
		case SPELL_HASH_FROST_RESISTANCE_AURA:
		case SPELL_HASH_CRUSADER_AURA:
			sp->BGR_one_buff_from_caster_on_self = SPELL_TYPE2_PALADIN_AURA;
			break;
		}

		switch(namehash)
		{
		case SPELL_HASH_BLOOD_PRESENCE:
		case SPELL_HASH_FROST_PRESENCE:
		case SPELL_HASH_UNHOLY_PRESENCE:
			sp->BGR_one_buff_from_caster_on_self = SPELL_TYPE3_DEATH_KNIGHT_AURA;
			break;
		}

		//grouping rule
		switch(namehash)
		{
		case SPELL_HASH_HUNTER_S_MARK:		// Hunter's mark
			sp->BGR_one_buff_from_caster_on_1target = SPELL_TYPE_INDEX_MARK;
			break;

		case SPELL_HASH_POLYMORPH:			// Polymorph
		case SPELL_HASH_POLYMORPH__CHICKEN:	// Polymorph: Chicken
		case SPELL_HASH_POLYMORPH___PENGUIN: // Polymorph: Penguin
		case SPELL_HASH_POLYMORPH__SHEEP:	// Polymorph: Sheep
		//case SPELL_HASH_POLYMORPH__TURTLE:	// Polymorph: Turtle
			sp->BGR_one_buff_from_caster_on_1target = SPELL_TYPE_INDEX_POLYMORPH;
			break;

		case SPELL_HASH_FEAR:				// Fear
			sp->BGR_one_buff_from_caster_on_1target = SPELL_TYPE_INDEX_FEAR;
			break;

		case SPELL_HASH_SAP:				// Sap
			sp->BGR_one_buff_from_caster_on_1target = SPELL_TYPE_INDEX_SAP;
			break;

		case SPELL_HASH_SCARE_BEAST:		// Scare Beast
			sp->BGR_one_buff_from_caster_on_1target = SPELL_TYPE_INDEX_SCARE_BEAST;
			break;

		case SPELL_HASH_HIBERNATE:			// Hibernate
			sp->BGR_one_buff_from_caster_on_1target = SPELL_TYPE_INDEX_HIBERNATE;
			break;

		case SPELL_HASH_EARTH_SHIELD:		// Earth Shield
			sp->BGR_one_buff_from_caster_on_1target = SPELL_TYPE_INDEX_EARTH_SHIELD;
			break;

		case SPELL_HASH_CYCLONE:			// Cyclone
			sp->BGR_one_buff_from_caster_on_1target = SPELL_TYPE_INDEX_CYCLONE;
			break;

		case SPELL_HASH_BANISH:				// Banish
			sp->BGR_one_buff_from_caster_on_1target = SPELL_TYPE_INDEX_BANISH;
			break;

		case SPELL_HASH_JUDGEMENT_OF_LIGHT:
		case SPELL_HASH_JUDGEMENT_OF_WISDOM:
		case SPELL_HASH_JUDGEMENT_OF_JUSTICE:
			sp->BGR_one_buff_from_caster_on_1target = SPELL_TYPE_INDEX_JUDGEMENT;
			break;

		case SPELL_HASH_BEACON_OF_LIGHT:	// Beacon of Light
			sp->BGR_one_buff_from_caster_on_1target = SPELL_TYPE_INDEX_BEACON_OF_LIGHT;
			break;

		case SPELL_HASH_FOCUS_MAGIC:		// Focus Magic buff
			sp->BGR_one_buff_from_caster_on_1target = SPELL_TYPE_INDEX_FOCUS_MAGIC;
			break;

		case SPELL_HASH_VIGILANCE:
			sp->BGR_one_buff_from_caster_on_1target = SPELL_TYPE_INDEX_VIGILANCE;
			break;

		case SPELL_HASH_HEX:
			sp->BGR_one_buff_from_caster_on_1target = SPELL_TYPE_INDEX_HEX;
			break;
		}

		// HACK FIX: Break roots/fear on damage.. this needs to be fixed properly!
		if( !(sp->AuraInterruptFlags & AURA_INTERRUPT_ON_ANY_DAMAGE_TAKEN) )
		{
			for( uint32 z = 0; z < 3; ++z ) 
			{
				if( sp->EffectApplyAuraName[z] == SPELL_AURA_MOD_FEAR || sp->EffectApplyAuraName[z] == SPELL_AURA_MOD_ROOT )
				{
					sp->AuraInterruptFlags |= AURA_INTERRUPT_ON_UNUSED2;
					break;
				}

				if( ( sp->Effect[z] == SPELL_EFFECT_SCHOOL_DAMAGE && sp->Spell_Dmg_Type == SPELL_DMG_TYPE_MELEE ) || sp->Effect[z] == SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL || sp->Effect[z] == SPELL_EFFECT_WEAPON_DAMAGE || sp->Effect[z] == SPELL_EFFECT_WEAPON_PERCENT_DAMAGE || sp->Effect[z] == SPELL_EFFECT_DUMMYMELEE )
					sp->is_melee_spell = true;

				if( sp->Effect[z] == SPELL_EFFECT_SCHOOL_DAMAGE && sp->Spell_Dmg_Type == SPELL_DMG_TYPE_RANGED )
					sp->is_ranged_spell = true;
			}
		}

		// set extra properties
		sp->RankNumber = rank;

		if( strstr( sp->Description, "Must remain seated"))
		{
			sp->RecoveryTime = 1000;
			sp->CategoryRecoveryTime = 1000;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// procintervals
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//if you are bored you could make these by hand but i guess we might find other spells with this problem..and this way it's safe
		//mage ignite talent should proc only on some chances //test
		else if( strstr( sp->Name, "Ignite") && sp->Id >= 11119 && sp->Id <= 12848 && sp->EffectApplyAuraName[0] == SPELL_AURA_DUMMY )
		{
			//check if we can find in the description
			char *startofid = strstr(sp->Description, "an additional ");
			if(startofid)
			{
				startofid += strlen("an additional ");
				sp->EffectBasePoints[0] = atoi(startofid);
			}
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA; //aura
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL; //force him to use procspell effect
			sp->EffectTriggerSpell[0] = 12654; //evil , but this is good for us :D
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procFlagExtra = CUSTOMPROC_ON_CRIT;
		}
		// Winter's Chill handled by frost school
		else if( strstr( sp->Name, "Winter's Chill"))
		{
			sp->School = 4;
		}
		// Blackout handled by Shadow school
		else if( strstr( sp->Name, "Blackout"))
		{
			sp->School = 5;
		}
		//more triggered spell ids are wrong. I think blizz is trying to outsmart us :S
		//Chain Heal all ranks %50 heal value (49 + 1)
		else if( strstr( sp->Name, "Chain Heal"))
		{
			sp->EffectDieSides[0] = 49;
		}
		else if( strstr( sp->Name, "Holy Shock"))
		{
			//check if we can find in the description
			char *startofid=strstr(sp->Description, "causing $");
			if(startofid)
			{
				startofid += strlen("causing $");
				sp->EffectTriggerSpell[0] = atoi(startofid);
			}
			//check if we can find in the description
			startofid=strstr(sp->Description, " or $");
			if(startofid)
			{
				startofid += strlen(" or $");
				sp->EffectTriggerSpell[1]=atoi(startofid);
			}
		}
		else if( strstr( sp->Name, "Touch of Weakness"))
		{
			//check if we can find in the description
			char *startofid=strstr(sp->Description, "cause $");
			if(startofid)
			{
				startofid += strlen("cause $");
				sp->EffectTriggerSpell[0] = atoi(startofid);
				sp->EffectTriggerSpell[1]=sp->EffectTriggerSpell[0]; //later versions of this spell changed to eff[1] the aura
				sp->procFlags = PROC_ON_MELEE_HIT_VICTIM | PROC_ON_MELEE_ABILITY_LAND_VICTIM;
			}
		}
		else if( strstr( sp->Name, "Firestone Passive"))
		{
			//Enchants the main hand weapon with fire, granting each attack a chance to deal $17809s1 additional fire damage.
			//check if we can find in the description
			char * startofid=strstr(sp->Description, "to deal $");
			if(startofid)
			{
				startofid += strlen("to deal $");
				sp->EffectTriggerSpell[0] = atoi(startofid);
				sp->EffectApplyAuraName[0] = 42;
				sp->procFlags = PROC_ON_MELEE_HIT | PROC_ON_MELEE_ABILITY_LAND;
				sp->procChance = 50;
			}
		}
		//some procs trigger at intervals
		else if( strstr( sp->Name, "Shadowguard"))
			sp->proc_interval = 3000; //few seconds
		else if( strstr( sp->Name, "Poison Shield"))
			sp->proc_interval = 3000; //few seconds
		else if( strstr( sp->Name, "Infused Mushroom"))
			sp->proc_interval = 10000; //10 seconds
		else if( strstr( sp->Name, "Aviana's Purpose"))
			sp->proc_interval = 10000; //10 seconds
		//don't change to namehash since we are searching only a portion of the name
 		else if( strstr( sp->Name, "Crippling Poison"))
		{
			sp->c_is_flags |= SPELL_FLAG_IS_POISON;
		}
		else if( strstr( sp->Name, "Mind-numbing Poison"))
		{
			sp->c_is_flags |= SPELL_FLAG_IS_POISON;
		}
		else if( strstr( sp->Name, "Instant Poison"))
		{
			sp->c_is_flags |= SPELL_FLAG_IS_POISON;
		}
		else if( strstr( sp->Name, "Deadly Poison"))
		{
			sp->c_is_flags |= SPELL_FLAG_IS_POISON;
		}
		else if( strstr( sp->Name, "Wound Poison"))
		{
			sp->c_is_flags |= SPELL_FLAG_IS_POISON;
		}
		else if( strstr( sp->Name, "Scorpid Poison") )
		{
			// groups?
			sp->c_is_flags |= SPELL_FLAG_IS_POISON;
		}

		// Set default mechanics if we don't already have one
		if( !sp->MechanicsType )
		{
			//Set Silencing spells mechanic.
			if( sp->EffectApplyAuraName[0] == 27 ||
				sp->EffectApplyAuraName[1] == 27 ||
				sp->EffectApplyAuraName[2] == 27 )
				sp->MechanicsType = MECHANIC_SILENCED;

			//Set Stunning spells mechanic.
			if( sp->EffectApplyAuraName[0] == 12 ||
				sp->EffectApplyAuraName[1] == 12 ||
				sp->EffectApplyAuraName[2] == 12 )
				sp->MechanicsType = MECHANIC_STUNNED;

			//Set Fearing spells mechanic
			if( sp->EffectApplyAuraName[0] == 7 ||
				sp->EffectApplyAuraName[1] == 7 ||
				sp->EffectApplyAuraName[2] == 7 )
				sp->MechanicsType = MECHANIC_FLEEING;

			//Set Interrupted spells mech
			if( sp->Effect[0] == SPELL_EFFECT_INTERRUPT_CAST ||
				sp->Effect[1] == SPELL_EFFECT_INTERRUPT_CAST ||
				sp->Effect[2] == SPELL_EFFECT_INTERRUPT_CAST )
				sp->MechanicsType = MECHANIC_INTERRUPTED;
		}

		if(
			((sp->Attributes & ATTRIBUTES_TRIGGER_COOLDOWN) && (sp->AttributesEx & ATTRIBUTESEX_NOT_BREAK_STEALTH)) //rogue cold blood
			|| ((sp->Attributes & ATTRIBUTES_TRIGGER_COOLDOWN) && (!sp->AttributesEx || sp->AttributesEx & ATTRIBUTESEX_REMAIN_OOC))
			)
		{
			sp->c_is_flags |= SPELL_FLAG_IS_REQUIRECOOLDOWNUPDATE;
		}

		if( namehash == SPELL_HASH_SHRED || namehash == SPELL_HASH_BACKSTAB || namehash == SPELL_HASH_AMBUSH || namehash == SPELL_HASH_GARROTE || namehash == SPELL_HASH_RAVAGE )
		{
			// FIX ME: needs different flag check
			sp->FacingCasterFlags = SPELL_INFRONT_STATUS_REQUIRE_INBACK;
		}

		// Lightning Shield - cannot crit, not a mistake, the correct proc spell for lightning shield is also called lightning shield
		if( sp->NameHash == SPELL_HASH_LIGHTNING_SHIELD ) 
			sp->spell_can_crit = false;

		// THESE FIXES ARE GROUPED FOR CODE CLEANLINESS.
		//Mend Pet
		//if( sp->NameHash == SPELL_HASH_MEND_PET )
			//sp->ChannelInterruptFlags = 0;

		// Flametongue Totem passive target fix
		if( sp->NameHash == SPELL_HASH_FLAMETONGUE_TOTEM && sp->Attributes & ATTRIBUTES_PASSIVE )
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SELF;
			sp->EffectImplicitTargetB[0] = 0;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_SELF;
			sp->EffectImplicitTargetB[1] = 0;
		}

		// Hex
		if( sp->NameHash == SPELL_HASH_HEX )
			sp->AuraInterruptFlags = AURA_INTERRUPT_ON_UNUSED2;
	}

	/////////////////////////////////////////////////////////////////
	//SPELL COEFFICIENT SETTINGS START
	//////////////////////////////////////////////////////////////////
	SpellEntry * sp;

	for(uint32 x= 0; x < cnt; x++)
	{
		// get spellentry
		sp = dbcSpell.LookupRow(x);

		//Setting Cast Time Coefficient
		SpellCastTime *sd = dbcSpellCastTime.LookupEntry(sp->CastingTimeIndex);
		float castaff = float(GetCastTime(sd));
		if( castaff < 1500 )
			castaff = 1500;
		else
			if( castaff > 7000 )
				castaff = 7000;

		SpellEntry * spz;
		bool spcheck = false;

		//Flag for DoT and HoT
		for( uint8 i = 0 ; i < 3 ; i++ )
		{
			if (sp->EffectApplyAuraName[i] == SPELL_AURA_PERIODIC_DAMAGE ||
				sp->EffectApplyAuraName[i] == SPELL_AURA_PERIODIC_HEAL ||
				sp->EffectApplyAuraName[i] == SPELL_AURA_PERIODIC_LEECH )
			{
				sp->spell_coef_flags |= SPELL_FLAG_IS_DOT_OR_HOT_SPELL;
				break;
			}
		}

		//Flag for DD or DH
		for( uint8 i = 0 ; i < 3 ; i++ )
		{
			if ( sp->EffectApplyAuraName[i] == SPELL_AURA_PERIODIC_TRIGGER_SPELL && sp->EffectTriggerSpell[i] )
			{
				spz = dbcSpell.LookupEntry( sp->EffectTriggerSpell[i] );
				if( spz &&
					( spz->Effect[i] == SPELL_EFFECT_SCHOOL_DAMAGE ||
						spz->Effect[i] == SPELL_EFFECT_HEAL )
					)
					spcheck = true;
			}
			if (sp->Effect[i] == SPELL_EFFECT_SCHOOL_DAMAGE ||
				sp->Effect[i] == SPELL_EFFECT_HEAL ||
				spcheck
				)
			{
				sp->spell_coef_flags |= SPELL_FLAG_IS_DD_OR_DH_SPELL;
				break;
			}
		}

		for(uint8 i = 0 ; i < 3; i++)
		{
			switch (sp->EffectImplicitTargetA[i])
			{
				//AoE
			case EFF_TARGET_ALL_TARGETABLE_AROUND_LOCATION_IN_RADIUS:
			case EFF_TARGET_ALL_ENEMY_IN_AREA:
			case EFF_TARGET_ALL_ENEMY_IN_AREA_INSTANT:
			case EFF_TARGET_ALL_PARTY_AROUND_CASTER:
			case EFF_TARGET_ALL_ENEMIES_AROUND_CASTER:
			case EFF_TARGET_IN_FRONT_OF_CASTER:
			case EFF_TARGET_ALL_ENEMY_IN_AREA_CHANNELED:
			case EFF_TARGET_ALL_PARTY_IN_AREA_CHANNELED:
			case EFF_TARGET_ALL_FRIENDLY_IN_AREA:
			case EFF_TARGET_ALL_TARGETABLE_AROUND_LOCATION_IN_RADIUS_OVER_TIME:
			case EFF_TARGET_ALL_PARTY:
			case EFF_TARGET_LOCATION_INFRONT_CASTER:
			case EFF_TARGET_BEHIND_TARGET_LOCATION:
			case EFF_TARGET_LOCATION_INFRONT_CASTER_AT_RANGE:
				{
					sp->spell_coef_flags |= SPELL_FLAG_AOE_SPELL;
					break;
				}
			}
		}

		for(uint8 i = 0 ; i < 3 ; i++)
		{
			switch (sp->EffectImplicitTargetB[i])
			{
				//AoE
			case EFF_TARGET_ALL_TARGETABLE_AROUND_LOCATION_IN_RADIUS:
			case EFF_TARGET_ALL_ENEMY_IN_AREA:
			case EFF_TARGET_ALL_ENEMY_IN_AREA_INSTANT:
			case EFF_TARGET_ALL_PARTY_AROUND_CASTER:
			case EFF_TARGET_ALL_ENEMIES_AROUND_CASTER:
			case EFF_TARGET_IN_FRONT_OF_CASTER:
			case EFF_TARGET_ALL_ENEMY_IN_AREA_CHANNELED:
			case EFF_TARGET_ALL_PARTY_IN_AREA_CHANNELED:
			case EFF_TARGET_ALL_FRIENDLY_IN_AREA:
			case EFF_TARGET_ALL_TARGETABLE_AROUND_LOCATION_IN_RADIUS_OVER_TIME:
			case EFF_TARGET_ALL_PARTY:
			case EFF_TARGET_LOCATION_INFRONT_CASTER:
			case EFF_TARGET_BEHIND_TARGET_LOCATION:
			case EFF_TARGET_LOCATION_INFRONT_CASTER_AT_RANGE:
				{
					sp->spell_coef_flags |= SPELL_FLAG_AOE_SPELL;
					break;
				}
			}
		}

		//Special Cases
		//Holy Light & Flash of Light
		if(sp->NameHash == SPELL_HASH_HOLY_LIGHT ||
			sp->NameHash == SPELL_HASH_FLASH_OF_LIGHT)
			sp->spell_coef_flags |= SPELL_FLAG_IS_DD_OR_DH_SPELL;


		//Additional Effect (not healing or damaging)
		for( uint8 i = 0 ; i < 3 ; i++ )
		{
			if(sp->Effect[i] == 0)
				continue;

			switch (sp->Effect[i])
			{
			case SPELL_EFFECT_SCHOOL_DAMAGE:
			case SPELL_EFFECT_ENVIRONMENTAL_DAMAGE:
			case SPELL_EFFECT_HEALTH_LEECH:
			case SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL:
			case SPELL_EFFECT_ADD_EXTRA_ATTACKS:
			case SPELL_EFFECT_WEAPON_PERCENT_DAMAGE:
			case SPELL_EFFECT_POWER_BURN:
			case SPELL_EFFECT_ATTACK:
			case SPELL_EFFECT_HEAL:
			case SPELL_EFFECT_HEAL_MAX_HEALTH:
			case SPELL_EFFECT_DUMMY:
				continue;
			}

			switch (sp->EffectApplyAuraName[i])
			{
			case SPELL_AURA_PERIODIC_DAMAGE:
			case SPELL_AURA_PROC_TRIGGER_DAMAGE:
			case SPELL_AURA_PERIODIC_DAMAGE_PERCENT:
			case SPELL_AURA_POWER_BURN:
			case SPELL_AURA_PERIODIC_HEAL:
			case SPELL_AURA_MOD_INCREASE_HEALTH:
			case SPELL_AURA_PERIODIC_HEALTH_FUNNEL:
			case SPELL_AURA_DUMMY:
				continue;
			}

			sp->spell_coef_flags |= SPELL_FLAG_ADITIONAL_EFFECT;
			break;

		}
	} 
	// END OF LOOP

	//Settings for special cases
	QueryResult * resultx = WorldDatabase.Query("SELECT * FROM spell_coef_override");
	if( resultx != NULL )
	{
		do
		{
			Field * f;
			f = resultx->Fetch();
			SpellEntry * sp = dbcSpell.LookupEntryForced( f[0].GetUInt32() );
			if( sp != NULL )
			{
				sp->Base_coef_override = f[2].GetFloat();
				sp->Misc_coef_override = f[3].GetFloat();
				sp->AP_coef = f[4].GetFloat();
				sp->RAP_coef = f[5].GetFloat();
			}
			else
				Log.Warning("SpellCoefOverride", "Has nonexistent spell %u.", f[0].GetUInt32());
		} while( resultx->NextRow() );
		delete resultx;
	}

	/////////////////////////////////////////////////////////////////
	//SPELL COEFFICIENT SETTINGS END
	/////////////////////////////////////////////////////////////////

	/**********************************************************
	 * Thrown - add a 1.6 second cooldown
	 **********************************************************/
	const static uint32 thrown_spells[] = { SPELL_RANGED_GENERAL, SPELL_RANGED_THROW, SPELL_RANGED_WAND, 26679, 27084, 29436, 37074, 41182, 41346, 0 };
	for( i = 0; thrown_spells[i] != 0; ++i )
	{
		sp = dbcSpell.LookupEntryForced( thrown_spells[i] );
		if( sp != NULL && sp->RecoveryTime==0 && sp->StartRecoveryTime == 0 )
		{
			if( sp->Id == SPELL_RANGED_GENERAL ) 
				sp->RecoveryTime = 500; // cebernic: hunter general with 0.5s
			else sp->RecoveryTime = 1500; // 1.5cd
		}
	}

	/**********************************************************
	 * Wands
	 **********************************************************/
	sp = dbcSpell.LookupEntryForced( SPELL_RANGED_WAND );
	if( sp != NULL )
		sp->Spell_Dmg_Type = SPELL_DMG_TYPE_RANGED;



	//////////////////////////////////////////////////////
	// CLASS-SPECIFIC SPELL FIXES						//
	//////////////////////////////////////////////////////

	/* Note: when applying spell hackfixes, please follow a template */

	//////////////////////////////////////////
	// DRUID								//
	//////////////////////////////////////////

		/**********************************************************
		 *	Balance
		 **********************************************************/

		// Druid - Brambles, Daze while unit has Barkskin
		//sp = dbcSpell.LookupEntryForced( 22812 );
		//if(sp != NULL)
			//sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM | PROC_ON_MELEE_ATTACK;

		// Druid - Force of Nature
		sp = dbcSpell.LookupEntryForced(33831);
		if( sp != NULL )
			sp->c_is_flags |= SPELL_FLAG_IS_INHERITING_LEVEL;

		/**********************************************************
		 *	Feral Combat
		 **********************************************************/

		// Druid - Dash, mask for FORM_CAT(1) = 1 << (1 - 1), which is 1
		sp = dbcSpell.LookupEntryForced( 1850 );
		if( sp != NULL )
			sp->RequiredShapeShift = 1;

		sp = dbcSpell.LookupEntryForced( 9821 );
		if( sp != NULL )
			sp->RequiredShapeShift = 1;

		sp = dbcSpell.LookupEntryForced( 33357 );
		if( sp != NULL )
			sp->RequiredShapeShift = 1;

		// Druid - Survival Instincts
		//sp = dbcSpell.LookupEntryForced( 50322 );
		//if( sp != NULL )
			//sp->NameHash = 0;

		// Druid - Bash - Interrupt effect
		sp = dbcSpell.LookupEntryForced( 5211 );
		if( sp != NULL )
		{
			sp->Effect[1] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1] = 32747;
		}
		sp = dbcSpell.LookupEntryForced( 6798 );
		if( sp != NULL )
		{
			sp->Effect[1] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1] = 32747;
		}
		sp = dbcSpell.LookupEntryForced( 8983 );
		if( sp != NULL )
		{
			sp->Effect[1] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1] = 32747;
		}

		// Druid - Maim
		sp = dbcSpell.LookupEntryForced(22570);
		if( sp != NULL )
			sp->is_melee_spell = true;

		sp = dbcSpell.LookupEntryForced(49802);
		if( sp != NULL )
			sp->is_melee_spell = true;

		sp = dbcSpell.LookupEntryForced(49376); //feral charge cat
		if( sp != NULL )
		{
			sp->Effect[1] = SPELL_EFFECT_CHARGE; //hackfix (meant to use trigger missile which isn't implemented)
			sp->EffectImplicitTargetA[1] = EFF_TARGET_SINGLE_ENEMY;
			sp->Effect[2] = 0;
		}

		sp = dbcSpell.LookupEntryForced(20719); //feline grace
		if( sp != NULL )
			sp->Effect[0] = 0;

		// Druid - Frenzied Regeneration - remake it
		sp = dbcSpell.LookupEntryForced( 22842 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 22845;
		}
		sp = dbcSpell.LookupEntryForced( 22895 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 22845;
		}
		sp = dbcSpell.LookupEntryForced( 22896 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 22845;
		}
		sp = dbcSpell.LookupEntryForced( 26999 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 22845;
		}

		// Druid - Primal Fury (talent)
		/*sp = dbcSpell.LookupEntryForced( 37116 );
		if( sp != NULL )
			sp->RequiredShapeShift = 0;

		sp = dbcSpell.LookupEntryForced( 37117 );
		if( sp != NULL )
			sp->RequiredShapeShift = 0;*/

		// Druid - Predatory Strikes
		uint32 mm = DecimalToMask(FORM_BEAR) | DecimalToMask(FORM_DIREBEAR) | DecimalToMask(FORM_MOONKIN) | DecimalToMask(FORM_CAT);

		sp = dbcSpell.LookupEntryForced( 16972 );
		if( sp != NULL )
			sp->RequiredShapeShift = mm;

		sp = dbcSpell.LookupEntryForced( 16974 );
		if( sp != NULL )
			sp->RequiredShapeShift = mm;

		sp = dbcSpell.LookupEntryForced( 16975 );
		if( sp != NULL )
			sp->RequiredShapeShift = mm;

		/**********************************************************
		 *	Restoration
		 **********************************************************/

		// Druid - Improved Barkskin (Passive)
		sp = dbcSpell.LookupEntryForced( 66530 );
		if( sp != NULL )
			sp->ShapeshiftExclude = DecimalToMask(FORM_BEAR) | DecimalToMask(FORM_DIREBEAR) | DecimalToMask(FORM_MOONKIN) | DecimalToMask(FORM_CAT) | DecimalToMask(FORM_TREE) | DecimalToMask(FORM_AQUA);
		
		// Druid - Tree Form Aura
		//sp = dbcSpell.LookupEntryForced( 34123 );
		//if( sp != NULL )
			//sp->NameHash = 0;

		// Druid - Natural Shapeshifter
		sp = dbcSpell.LookupEntryForced( 16833 );
		if( sp != NULL )
			sp->DurationIndex = 0;

		sp = dbcSpell.LookupEntryForced( 16834 );
		if( sp != NULL )
			sp->DurationIndex = 0;

		sp = dbcSpell.LookupEntryForced( 16835 );
		if( sp != NULL )
			sp->DurationIndex = 0;

	//////////////////////////////////////////
	// DEATH KNIGHT							//
	//////////////////////////////////////////

		// Death Knight - Rune Tap
		sp = dbcSpell.LookupEntryForced( 48982 );
		if( sp != NULL )
           sp->Effect[0] = SPELL_EFFECT_DUMMY;

		// Death Knight - Death and Decay
		sp = dbcSpell.LookupEntryForced( 43265 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PERIODIC_DAMAGE;
			sp->Effect[0] = SPELL_EFFECT_PERSISTENT_AREA_AURA;
		}

		sp = dbcSpell.LookupEntryForced( 49936 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PERIODIC_DAMAGE;
			sp->Effect[0] = SPELL_EFFECT_PERSISTENT_AREA_AURA;
		}

		sp = dbcSpell.LookupEntryForced( 49937 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PERIODIC_DAMAGE;
			sp->Effect[0] = SPELL_EFFECT_PERSISTENT_AREA_AURA;
		}

		sp = dbcSpell.LookupEntryForced( 49938 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PERIODIC_DAMAGE;
			sp->Effect[0] = SPELL_EFFECT_PERSISTENT_AREA_AURA;
		}

		// Death Knight - Unholy Aura
		sp = dbcSpell.LookupEntryForced( 50391 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_INCREASE_SPEED_ALWAYS;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AREA_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1] = 50392;
			sp->Effect[1] = SPELL_EFFECT_APPLY_AREA_AURA;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_SELF;
		}

		sp = dbcSpell.LookupEntryForced( 50392 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_INCREASE_SPEED_ALWAYS;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AREA_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1] = 50392;
			sp->Effect[1] = SPELL_EFFECT_APPLY_AREA_AURA;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_SELF;
		}

		// Death Knight - Frost Presence
		sp = dbcSpell.LookupEntryForced( 48263 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_BASE_RESISTANCE_PCT;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT;
			sp->EffectBasePoints[1] = 9;
			sp->EffectApplyAuraName[2] = SPELL_AURA_MOD_DAMAGE_TAKEN;
		}

		// Death Knight - Unholy Presence
		sp = dbcSpell.LookupEntryForced( 48265 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_HASTE;
			sp->EffectBasePoints[0] = 14;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_INCREASE_SPEED;
			sp->EffectBasePoints[1] = 14;
		}

		// Death Knight - Death Grip
		sp = dbcSpell.LookupEntryForced( 49576 );
		if( sp != NULL )
			sp->Effect[0] = SPELL_EFFECT_DUMMY;

		// Death Knight - Acherus Deatcharger
		sp = dbcSpell.LookupEntryForced( 48778 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOUNTED;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_INCREASE_MOUNTED_SPEED;
			sp->EffectBasePoints[1] = 99;
		}

		// Death Knight - Path of Frost
		sp = dbcSpell.LookupEntryForced( 3714 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_WATER_WALK;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
		}

		// Death Knight - Shadow of Death
		sp = dbcSpell.LookupEntryForced( 49157 );
		if( sp != NULL )
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;

		// Death Knight - Frost Strike - really can't be dpb?
		sp = dbcSpell.LookupEntryForced( 49143 );
		if( sp != NULL )
			sp->Attributes = ATTRIBUTES_CANT_BE_DPB;

		sp = dbcSpell.LookupEntryForced( 51416 );
		if( sp != NULL )
			sp->Attributes = ATTRIBUTES_CANT_BE_DPB;

		sp = dbcSpell.LookupEntryForced( 51417 );
		if( sp != NULL )
			sp->Attributes = ATTRIBUTES_CANT_BE_DPB;

		sp = dbcSpell.LookupEntryForced( 51418 );
		if( sp != NULL )
			sp->Attributes = ATTRIBUTES_CANT_BE_DPB;

		sp = dbcSpell.LookupEntryForced( 51419 );
		if( sp != NULL )
			sp->Attributes = ATTRIBUTES_CANT_BE_DPB;

		sp = dbcSpell.LookupEntryForced( 55268 );
		if( sp != NULL )
			sp->Attributes = ATTRIBUTES_CANT_BE_DPB;

		// Death Knight - Blood Fury Healing Debuff
		sp = dbcSpell.LookupEntryForced( 23230 );
		if( sp != NULL )
			sp->c_is_flags |= SPELL_FLAG_IS_FORCEDDEBUFF;

	//////////////////////////////////////////
	// HUNTER								//
	//////////////////////////////////////////

		/**********************************************************
		 *	Beast Mastery
		 **********************************************************/

		// Hunter - Feed pet
		sp = dbcSpell.LookupEntryForced( 6991 );
		if( sp != NULL )
			sp->EffectImplicitTargetA[0] = 0;

		// Hunter - The Beast Within - check it
		sp = dbcSpell.LookupEntryForced( 34692 );
		if( sp != NULL )
		{
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 100;
			sp->EffectTriggerSpell[1] = 34471;
		}

		// Hunter - Spirit Bond
		sp = dbcSpell.LookupEntryForced( 19578 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER | SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 19579;
		}

		sp = dbcSpell.LookupEntryForced( 20895 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER | SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 24529;
		}

		sp = dbcSpell.LookupEntryForced( 19579 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA; //we should do the same for player too as we did for pet
			sp->EffectApplyAuraName[1] = sp->EffectApplyAuraName[0];
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
			sp->EffectBasePoints[1] = sp->EffectBasePoints[0];
			sp->EffectAmplitude[1] = sp->EffectAmplitude[0];
			sp->EffectDieSides[1] = sp->EffectDieSides[0];
		}

		sp = dbcSpell.LookupEntryForced( 24529 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA; //we should do the same for player too as we did for pet
			sp->EffectApplyAuraName[1] = sp->EffectApplyAuraName[0];
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
			sp->EffectBasePoints[1] = sp->EffectBasePoints[0];
			sp->EffectAmplitude[1] = sp->EffectAmplitude[0];
			sp->EffectDieSides[1] = sp->EffectDieSides[0];
		}

		// Hunter - Ferocious Inspiration
		sp = dbcSpell.LookupEntryForced( 34455 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->EffectTriggerSpell[0] = 34456;
			sp->procFlags = PROC_ON_MELEE_HIT | PROC_ON_RANGED_HIT | PROC_ON_CAST_SPELL;
			sp->procFlagExtra = CUSTOMPROC_ON_CRIT;
			sp->Effect[1] = 0; //remove this
		}

		sp = dbcSpell.LookupEntryForced( 34459 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->EffectTriggerSpell[0] = 34456;
			sp->procFlags = PROC_ON_MELEE_HIT | PROC_ON_RANGED_HIT | PROC_ON_CAST_SPELL;
			sp->procFlagExtra = CUSTOMPROC_ON_CRIT;
			sp->Effect[1] = 0; //remove this
		}

		sp = dbcSpell.LookupEntryForced( 34460 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->EffectTriggerSpell[0] = 34456;
			sp->procFlags = PROC_ON_MELEE_HIT | PROC_ON_RANGED_HIT | PROC_ON_CAST_SPELL;
			sp->procFlagExtra = CUSTOMPROC_ON_CRIT;
			sp->Effect[1] = 0; //remove this
		}

		// Hunter - Focused Fire
		sp = dbcSpell.LookupEntryForced( 35029 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER | SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 35060;
		}

		sp = dbcSpell.LookupEntryForced( 35030 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER | SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 35061;
		}

		sp = dbcSpell.LookupEntryForced( 35060 );
		if( sp != NULL )
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;

		sp = dbcSpell.LookupEntryForced( 35061 );
		if( sp != NULL )
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;

		// Hunter - Frenzy
		sp = dbcSpell.LookupEntryForced( 19621 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 19615;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->procChance = sp->EffectBasePoints[0];
			sp->procFlags = PROC_ON_MELEE_HIT | PROC_ON_RANGED_HIT | PROC_ON_CAST_SPELL;
			sp->procFlagExtra = CUSTOMPROC_ON_CRIT;
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET | SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
		}

		sp = dbcSpell.LookupEntryForced( 19622 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 19615;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->procChance = sp->EffectBasePoints[0];
			sp->procFlags = PROC_ON_MELEE_HIT | PROC_ON_RANGED_HIT | PROC_ON_CAST_SPELL;
			sp->procFlagExtra = CUSTOMPROC_ON_CRIT;
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET | SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
		}

		sp = dbcSpell.LookupEntryForced( 19623 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 19615;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->procChance = sp->EffectBasePoints[0];
			sp->procFlags = PROC_ON_MELEE_HIT | PROC_ON_RANGED_HIT | PROC_ON_CAST_SPELL;
			sp->procFlagExtra = CUSTOMPROC_ON_CRIT;
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET | SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
		}

		sp = dbcSpell.LookupEntryForced( 19624 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 19615;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->procChance = sp->EffectBasePoints[0];
			sp->procFlags = PROC_ON_MELEE_HIT | PROC_ON_RANGED_HIT | PROC_ON_CAST_SPELL;
			sp->procFlagExtra = CUSTOMPROC_ON_CRIT;
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET | SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
		}

		sp = dbcSpell.LookupEntryForced( 19625 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 19615;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->procChance = sp->EffectBasePoints[0];
			sp->procFlags = PROC_ON_MELEE_HIT | PROC_ON_RANGED_HIT | PROC_ON_CAST_SPELL;
			sp->procFlagExtra = CUSTOMPROC_ON_CRIT;
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET | SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
		}

		/**********************************************************
		 *	Marksmanship
		 **********************************************************/

		/**********************************************************
		 *	Survival
		 **********************************************************/

	//////////////////////////////////////////
	// MAGE									//
	//////////////////////////////////////////

		/**********************************************************
		 *	Arcane
		 **********************************************************/

		// Mage - Arcane Blast Proc
		sp = dbcSpell.LookupEntryForced( 36032 );
		{
			//sp->procCharges = 0;
			sp->c_is_flags |= SPELL_FLAG_IS_FORCEDDEBUFF;
		}

		// Mage - Invisibility
		sp = dbcSpell.LookupEntryForced( 66 );
		if( sp != NULL )
		{
			sp->AuraInterruptFlags |= AURA_INTERRUPT_ON_CAST_SPELL;
			sp->Effect[1] = 0;
			sp->EffectApplyAuraName[2] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
			sp->Effect[2] = SPELL_EFFECT_APPLY_AURA;
            sp->EffectAmplitude[2] = 3000;
			sp->EffectImplicitTargetA[2] = EFF_TARGET_SELF;
			sp->EffectBaseDice[2] = 1;
			sp->EffectDieSides[2] = 1;
			sp->EffectTriggerSpell[2] = 32612;
			sp->EffectBasePoints[2] = -1;
			/*sp->EffectTriggerSpell[0] = 32612;*/
		}

		// Mage - Invisibility triggered spell, should be removed on cast
		sp = dbcSpell.LookupEntryForced( 32612 );
		if( sp != NULL )
			sp->AuraInterruptFlags |= AURA_INTERRUPT_ON_CAST_SPELL;

		// Mage - Arcane Potency
		sp = dbcSpell.LookupEntryForced( 57529 );
		if( sp != NULL )
			sp->RankNumber = 100;

		sp = dbcSpell.LookupEntryForced( 57531 );
		if( sp != NULL )
			sp->RankNumber = 101;

		// Mage - Conjure Refreshment Table
		sp = dbcSpell.LookupEntryForced( 43985 );
		if ( sp != NULL )
			sp->EffectImplicitTargetA[0] = EFF_TARGET_DYNAMIC_OBJECT;

		/**********************************************************
		 *	Fire
		 **********************************************************/

		// Mage - Fiery Payback - Buff effect
		sp = dbcSpell.LookupEntryForced( 64353 );
        if( sp != NULL )
        {
            sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 44440;
			sp->procFlags = PROC_ON_ANY_DAMAGE;
			sp->procChance = 100;
        }
		sp = dbcSpell.LookupEntryForced( 64357 );
        if( sp != NULL )
        {
            sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 44441;
			sp->procFlags = PROC_ON_ANY_DAMAGE;
			sp->procChance = 100;
        }

		// Mage - Combustion
		sp = dbcSpell.LookupEntryForced( 11129 );
		if( sp != NULL )
		{
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procCharges = 0;
			sp->c_is_flags |= SPELL_FLAG_IS_REQUIRECOOLDOWNUPDATE;
		}

		/**********************************************************
		 *	Frost
		 **********************************************************/

		// Mage - Hypothermia: undispellable
		sp = dbcSpell.LookupEntryForced( 41425 );
		if( sp != NULL )
			sp->c_is_flags = SPELL_FLAG_IS_FORCEDDEBUFF;

		// Mage - Frostbite
		sp = dbcSpell.LookupEntryForced( 11071 );
		if( sp != NULL )
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;

		sp = dbcSpell.LookupEntryForced( 12496 );
		if( sp != NULL )
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;

		sp = dbcSpell.LookupEntryForced( 12497 );
		if( sp != NULL )
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;

		//Mage - Improved Blizzard
		sp = dbcSpell.LookupEntryForced( 11185 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 12484;
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 12487 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 12485;
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 12488 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 12486;
			sp->procFlags = PROC_ON_CAST_SPELL;
		}

		// Mage - Brain Freeze
		sp = dbcSpell.LookupEntryForced( 57761 );
		if( sp != NULL )
			sp->AuraInterruptFlags = AURA_INTERRUPT_ON_CAST_SPELL;

	//////////////////////////////////////////
	// PALADIN								//
	//////////////////////////////////////////

		/**********************************************************
		 *	Holy
		 **********************************************************/

		// Paladin - Improved Concentration Aura
		sp = dbcSpell.LookupEntryForced( 63510 );
		if( sp != NULL )
		{
			//sp->NameHash = 0;
			//sp->EffectRadiusIndex[0] = 23;
			//sp->EffectRadiusIndex[1] = 23;
		}

		// Seal of Righteousness
		sp = dbcSpell.LookupEntryForced( 25742 );
		if( sp != NULL )
			sp->spell_can_crit = false;

		/**********************************************************
		 *	Protection
		 **********************************************************/

		// Paladin - Improved Devotion Aura
		sp = dbcSpell.LookupEntryForced( 63514 );
		if( sp != NULL )
		{
			//sp->NameHash = 0;
			//sp->EffectRadiusIndex[0] = 23;
		}

		// Paladin - Hammer of the Righteous - try without it
		sp = dbcSpell.LookupEntryForced( 53595 );
		if( sp != NULL )
			sp->speed = 0;	//without, no damage is done

		// Paladin - Hammer of Justice - Interrupt effect
		sp = dbcSpell.LookupEntryForced( 853 );
		if( sp != NULL )
		{
			sp->Effect[1] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1] = 32747;
		}
		sp = dbcSpell.LookupEntryForced( 5588 );
		if( sp != NULL )
		{
			sp->Effect[1] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1] = 32747;
		}
		sp = dbcSpell.LookupEntryForced( 5589 );
		if( sp != NULL )
		{
			sp->Effect[1] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1] = 32747;
		}
		sp = dbcSpell.LookupEntryForced( 10308 );
		if( sp != NULL )
		{
			sp->Effect[1] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1] = 32747;
		}

		/**********************************************************
		 *	Retribution
		 **********************************************************/

		// Paladin - Sanctified Retribution
		sp = dbcSpell.LookupEntryForced( 63531 );
		if( sp != NULL )
		{
			//sp->NameHash = 0;
			//sp->EffectRadiusIndex[0] = 23;
			//sp->EffectRadiusIndex[1] = 23;
		}

		// Paladin - Seal of Command - Holy damage, but melee mechanics (crit damage, chance, etc) - try without it
		sp = dbcSpell.LookupEntryForced( 20424 );
		if( sp != NULL )
		{
			sp->is_melee_spell = true;
			sp->School = SCHOOL_HOLY; //the procspells of the original seal of command have physical school instead of holy
			sp->Spell_Dmg_Type = SPELL_DMG_TYPE_MAGIC; //heh, crazy spell uses melee/ranged/magic dmg type for 1 spell. Now which one is correct ?
		}

		// Paladin - Judgement of Command
		sp = dbcSpell.LookupEntryForced( 20467 );
		if( sp != NULL )
			sp->Spell_Dmg_Type = SPELL_DMG_TYPE_MAGIC;

		// Paladin - Judgements - rebuff
		//sp = dbcSpell.LookupEntryForced( 20184 );
		//if( sp != NULL )
			//sp->RankNumber = 0;

		// Paladin - Judgement of Light
		sp = dbcSpell.LookupEntryForced( 20185 );
        if( sp != NULL )
        {
            sp->Effect[0] = 6;
			sp->EffectApplyAuraName[0] = 42;
			sp->EffectTriggerSpell[0] = 20267;
			sp->RankNumber = 0;
			sp->proc_interval = 4000;
        }
		sp = dbcSpell.LookupEntryForced( 20267 );
		if( sp != NULL )
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SINGLE_ENEMY;

		// Paladin - Judgement of Wisdom
		sp = dbcSpell.LookupEntryForced( 20186 );
        if( sp != NULL )
        {
            sp->Effect[0] = 6;
			sp->EffectApplyAuraName[0] = 42;
			sp->EffectTriggerSpell[0] = 20268;
			sp->RankNumber = 0;
			sp->proc_interval = 4000;
        }
		sp = dbcSpell.LookupEntryForced( 20268 );
		if( sp != NULL )
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SINGLE_ENEMY;

		// Paladin - Vengeance - should work without it
		/*sp = dbcSpell.LookupEntryForced( 20050 );
		if( sp != NULL )
			sp->maxstack = 3;

		sp = dbcSpell.LookupEntryForced( 20052 );
		if( sp != NULL )
			sp->maxstack = 3;

		sp = dbcSpell.LookupEntryForced( 20053 );
		if( sp != NULL )
			sp->maxstack = 3;*/

		// Paladin - Divine Storm
		sp = dbcSpell.LookupEntryForced( 53385 );
		if( sp != NULL )
		{
			sp->EffectRadiusIndex[0] = 43; //16 yards, EffectRadiusIndex[0] is 0 and EfectRadiusIndex[3] is 14 in dbc(14 is 8 yards)
			sp->MaxTargets = 4; //it's 5 in dbc, have it changed or maxtargets is dbc value - 1 ?
		}

		// Paladin - Judgements of the Wise - check it again
		sp = dbcSpell.LookupEntryForced( 31930 );
		if( sp != NULL )
		{
			sp->SpellFamilyName = 0;
			sp->SpellGroupType[0] =0;
			sp->SpellGroupType[1] =0;
			sp->SpellGroupType[2] =0;
			//sp->RankNumber = 0;
		}

		sp = dbcSpell.LookupEntryForced( 54180 );
		if( sp != NULL )
		{
			sp->SpellFamilyName = 0;
			sp->SpellGroupType[0] = 0;
			sp->SpellGroupType[1] = 0;
			sp->SpellGroupType[2] = 0;
			//sp->RankNumber = 0;
			sp->proc_interval = 4000;
		}

		sp = dbcSpell.LookupEntryForced( 31876 ); //to dummy aura
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 54180;
		}
		sp = dbcSpell.LookupEntryForced( 31877 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 54180;
		}
		sp = dbcSpell.LookupEntryForced( 31878 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 54180;
		}

		// Paladin - Crusader Strike
		sp = dbcSpell.LookupEntryForced( 35395 );
		if( sp != NULL )
			sp->noproc = true;

		// Paladin - Art of War
		sp = dbcSpell.LookupEntryForced( 53489 );
		if( sp != NULL )
			sp->AuraInterruptFlags = AURA_INTERRUPT_ON_CAST_SPELL;

		sp = dbcSpell.LookupEntryForced( 59578 );
		if( sp != NULL )
			sp->AuraInterruptFlags = AURA_INTERRUPT_ON_CAST_SPELL;

		// Paladin - Forbearance - Is forced debuff
		sp = dbcSpell.LookupEntryForced( 25771 );
		if( sp != NULL )
			sp->c_is_flags |= SPELL_FLAG_IS_FORCEDDEBUFF;

	//////////////////////////////////////////
	// PRIEST								//
	//////////////////////////////////////////
	
		/**********************************************************
		 *	Discipline
		 **********************************************************/
	
		// Priest - Inner Focus
		sp = dbcSpell.LookupEntryForced( 14751 );
		if(sp != NULL)
			sp->AuraInterruptFlags = static_cast<uint32>( AURA_INTERRUPT_ON_CAST_SPELL );

		// Priest - Grace
		sp = dbcSpell.LookupEntryForced( 47930 );
		if( sp != NULL )
			sp->BGR_one_buff_from_caster_on_1target = SPELL_TYPE_INDEX_GRACE;

		// Priest - Weakened Soul - Is forced debuff
		sp = dbcSpell.LookupEntryForced( 6788 );
		if( sp != NULL )
			sp->c_is_flags = SPELL_FLAG_IS_FORCEDDEBUFF;

		// Priest - Reflective Shield - to dummy aura
		sp = dbcSpell.LookupEntryForced( 33201 );
		if( sp != NULL )
		{
			sp->procFlagExtra = CUSTOMPROC_ON_ABSORB;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 33619;
		}
		sp = dbcSpell.LookupEntryForced( 33202 );
		if( sp != NULL )
		{
			sp->procFlagExtra = CUSTOMPROC_ON_ABSORB;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 33619;
		}

		// Priest - Penance - is it ok?
		sp = dbcSpell.LookupEntryForced( 47540 );
		if ( sp != NULL )
		{
			sp->DurationIndex = 566; // Change to instant cast as script will cast the real channeled spell. //0
			sp->ChannelInterruptFlags = 0; // Remove channeling behavior. //
		}
		sp = dbcSpell.LookupEntryForced( 53005 );
		if ( sp != NULL )
		{
			sp->DurationIndex = 566;
			sp->ChannelInterruptFlags = 0;
		}
		sp = dbcSpell.LookupEntryForced( 53006 );
		if ( sp != NULL )
		{
			sp->DurationIndex = 566;
			sp->ChannelInterruptFlags = 0;
		}
		sp = dbcSpell.LookupEntryForced( 53007 );
		if ( sp != NULL )
		{
			sp->DurationIndex = 566;
			sp->ChannelInterruptFlags = 0;
		}

		// Priest - Penance triggered healing spells have wrong targets.
		sp = dbcSpell.LookupEntryForced( 47750 );
		if ( sp != NULL )
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SINGLE_FRIEND;
		}
		sp = dbcSpell.LookupEntryForced( 52983 );
		if ( sp != NULL )
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SINGLE_FRIEND;
		}
		sp = dbcSpell.LookupEntryForced( 52984 );
		if ( sp != NULL )
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SINGLE_FRIEND;
		}
		sp = dbcSpell.LookupEntryForced( 52985 );
		if ( sp != NULL )
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SINGLE_FRIEND;
		}

		/**********************************************************
		 *	Holy
		 **********************************************************/

		// Priest - Holy Nova
		sp = dbcSpell.LookupEntryForced( 15237 );
		if( sp != NULL )
		{
			sp->Effect[1] = 64;
			sp->EffectTriggerSpell[1] = 23455;
		}
		sp = dbcSpell.LookupEntryForced( 15430 );
		if( sp != NULL )
		{
			sp->Effect[1] = 64;
			sp->EffectTriggerSpell[1] = 23458;
		}
		sp = dbcSpell.LookupEntryForced( 15431 );
		if( sp != NULL )
		{
			sp->Effect[1] = 64;
			sp->EffectTriggerSpell[1] = 23459;
		}
		sp = dbcSpell.LookupEntryForced( 27799 );
		if( sp != NULL )
		{
			sp->Effect[1] = 64;
			sp->EffectTriggerSpell[1] = 27803;
		}
		sp = dbcSpell.LookupEntryForced( 27800 );
		if( sp != NULL )
		{
			sp->Effect[1] = 64;
			sp->EffectTriggerSpell[1] = 27804;
		}
		sp = dbcSpell.LookupEntryForced( 27801 );
		if( sp != NULL )
		{
			sp->Effect[1] = 64;
			sp->EffectTriggerSpell[1] = 27805;
		}
		sp = dbcSpell.LookupEntryForced( 25331 );
		if( sp != NULL )
		{
			sp->Effect[1] = 64;
			sp->EffectTriggerSpell[1] = 25329;
		}
		sp = dbcSpell.LookupEntryForced( 48077 );
		if( sp != NULL )
		{
			sp->Effect[1] = 64;
			sp->EffectTriggerSpell[1] = 48075;
		}
		sp = dbcSpell.LookupEntryForced( 48078 );
		if( sp != NULL )
		{
			sp->Effect[1] = 64;
			sp->EffectTriggerSpell[1] = 48076;
		}

		// Priest - Surge of Light
		sp = dbcSpell.LookupEntryForced( 33151 );
		if( sp != NULL )
			sp->AuraInterruptFlags = AURA_INTERRUPT_ON_CAST_SPELL;

		/**********************************************************
		 *	Shadow
		 **********************************************************/

	//////////////////////////////////////////
	// ROGUE								//
	//////////////////////////////////////////

		/**********************************************************
		 *	Assassination
		 **********************************************************/

		// Rogue - Overkill
		sp = dbcSpell.LookupEntryForced( 58427 );
		if( sp != NULL )
			sp->AttributesEx |= ATTRIBUTESEX_NOT_BREAK_STEALTH;

		// Rogue - Garrote - is this used?
		sp = dbcSpell.LookupEntryForced( 37066 );
		if( sp != NULL )
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SINGLE_ENEMY;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_SINGLE_ENEMY;
		}

		/**********************************************************
		 *	Combat
		 **********************************************************/

		//Rogue - Killing Spree Stealth fix
		sp = dbcSpell.LookupEntryForced( 51690 );
		if( sp != NULL )
			sp->AttributesEx |= ATTRIBUTESEX_NOT_BREAK_STEALTH;

		/**********************************************************
		 *	Subtlety
		 **********************************************************/

		// Rogue - Vanish : Second Trigger Spell
		sp = dbcSpell.LookupEntryForced( 18461 );
		if( sp != NULL )
			sp->AttributesEx |= ATTRIBUTESEX_NOT_BREAK_STEALTH;

		// Rogue - Blind (Make it able to miss!)
		sp = dbcSpell.LookupEntryForced( 2094 );
		if( sp != NULL )
			sp->is_ranged_spell = true;

	//////////////////////////////////////////
	// SHAMAN								//
	//////////////////////////////////////////

		/**********************************************************
		 *	Elemental Combat
		 **********************************************************/

		// Shaman - Elemental Mastery
		sp = dbcSpell.LookupEntryForced( 16166 );
		if( sp != NULL )
			sp->AuraInterruptFlags |= AURA_INTERRUPT_ON_CAST_SPELL;

		/**********************************************************
		 *	Enhancement
		 **********************************************************/

		// Shaman - Earth elemental - should inherit casters level
		sp = dbcSpell.LookupEntryForced( 33663 );
		if( sp != NULL )
			sp->c_is_flags |= SPELL_FLAG_IS_INHERITING_LEVEL;

		// Shaman - Wrath of Air Totem
		sp = dbcSpell.LookupEntryForced( 2895 );
		if( sp != NULL )
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SELF;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_SELF;
		}

		/**********************************************************
		 *	Restoration
		 **********************************************************/
		
		// Shaman - Reincarnation
		sp = dbcSpell.LookupEntryForced( 20608 ); 
		if( sp != NULL )
		{
			for( uint32 i=0; i<8; i++ )
			{
				if( sp->Reagent[i] )
				{
					sp->Reagent[i] = 0;
					sp->ReagentCount[i] = 0;
				}
			}
		}

		// Shaman - Ancestral healing proc spell
		sp = dbcSpell.LookupEntryForced( 16177 );
		if( sp != NULL )
			sp->rangeIndex = 4;

		sp = dbcSpell.LookupEntryForced( 16236 );
		if( sp != NULL )
			sp->rangeIndex = 4;

		sp = dbcSpell.LookupEntryForced( 16237 );
		if( sp != NULL )
			sp->rangeIndex = 4;

	//////////////////////////////////////////
	// WARLOCK								//
	//////////////////////////////////////////
		
		/**********************************************************
		 *	Affliction
		 **********************************************************/

		// Warlock - Empowered Corruption
		sp = dbcSpell.LookupEntryForced( 32381 );
		if( sp != NULL )
			sp->EffectBasePoints[0] = 12;

		sp = dbcSpell.LookupEntryForced( 32382 );
		if( sp != NULL )
			sp->EffectBasePoints[0] = 24;

		sp = dbcSpell.LookupEntryForced( 32383 );
		if( sp != NULL )
			sp->EffectBasePoints[0] = 36;

		/**********************************************************
		 *	Demonology
		 **********************************************************/

		// Warlock - Metamorphosis
		/*sp = dbcSpell.LookupEntryForced( 54817 );
		if( sp != NULL )
			sp->NameHash = 0;

		sp = dbcSpell.LookupEntryForced( 54879 );
		if( sp != NULL )
			sp->NameHash = 0;*/

		// Warlock - Healthstones
		int HealthStoneID[8] = {6201, 6202, 5699, 11729, 11730, 27230, 47871, 47878};
		for( int i=0; i<8; i++ )
		{
			sp = dbcSpell.LookupEntryForced( HealthStoneID[i] );
			if( sp != NULL )
			{
				sp->Reagent[1] = 0;
			}
		}

		// Warlock - Immolation Aura
		sp = dbcSpell.LookupEntryForced( 50589 );
		if( sp != NULL )
			sp->ChannelInterruptFlags = 0; // Remove channeling behaviour.

		// Warlock - Demonic Knowledge
		sp = dbcSpell.LookupEntryForced( 35691 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_DAMAGE_DONE;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SELF;
			sp->Effect[1] = 6;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_DONE;
			sp->EffectBasePoints[1] = sp->EffectBasePoints[0];
			sp->EffectImplicitTargetA[1]= EFF_TARGET_PET;
			sp->Effect[2] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[2] = 35696;
			sp->EffectImplicitTargetA[2]=EFF_TARGET_PET;
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER | SPELL_FLAG_IS_EXPIREING_WITH_PET;
		}

		sp = dbcSpell.LookupEntryForced( 35692 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_DAMAGE_DONE;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SELF;
			sp->Effect[1] = 6;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_DONE;
			sp->EffectBasePoints[1] = sp->EffectBasePoints[0];
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
			sp->Effect[2] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[2] = 35696;
			sp->EffectImplicitTargetA[2] = EFF_TARGET_PET;
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER | SPELL_FLAG_IS_EXPIREING_WITH_PET;
		}

		sp = dbcSpell.LookupEntryForced( 35693 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_DAMAGE_DONE;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SELF;
			sp->Effect[1] = 6;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_DONE;
			sp->EffectBasePoints[1] = sp->EffectBasePoints[0];
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
			sp->Effect[2] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[2] = 35696;
			sp->EffectImplicitTargetA[2] = EFF_TARGET_PET;
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER | SPELL_FLAG_IS_EXPIREING_WITH_PET;
		}

		sp = dbcSpell.LookupEntryForced( 35696 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6; //making this only for the visible effect
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY; //no effect here
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}

		// Warlock - Soul Link
		sp = dbcSpell.LookupEntryForced( 19028 );
		if( sp != NULL )
		{
			//this is for the trigger effect
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_SPLIT_DAMAGE;
			sp->EffectMiscValue[0]=20;
			//sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			//this is for the extra 5% dmg for caster and pet
			sp->Effect[1] = 6;
			sp->EffectApplyAuraName[1] = 79;
			sp->EffectBasePoints[1] = 4; //4+1=5
			sp->EffectImplicitTargetA[1] = EFF_TARGET_SELF;
			sp->EffectImplicitTargetB[1] = EFF_TARGET_PET;
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
		}

		// Warlock - Demonic Tactics
		sp = dbcSpell.LookupEntryForced( 30242 );
		if( sp != NULL )
		{
			sp->EffectImplicitTargetB[1] = EFF_TARGET_PET;
			sp->EffectImplicitTargetB[2] = EFF_TARGET_PET;
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
		}

		sp = dbcSpell.LookupEntryForced( 30245 );
		if( sp != NULL )
		{
			sp->EffectImplicitTargetB[1] = EFF_TARGET_PET;
			sp->EffectImplicitTargetB[2] = EFF_TARGET_PET;
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
		}

		sp = dbcSpell.LookupEntryForced( 30246 );
		if( sp != NULL )
		{
			sp->EffectImplicitTargetB[1] = EFF_TARGET_PET;
			sp->EffectImplicitTargetB[2] = EFF_TARGET_PET;
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
		}

		sp = dbcSpell.LookupEntryForced( 30247 );
		if( sp != NULL )
		{
			sp->EffectImplicitTargetB[1] = EFF_TARGET_PET;
			sp->EffectImplicitTargetB[2] = EFF_TARGET_PET;
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
		}

		sp = dbcSpell.LookupEntryForced( 30248 );
		if( sp != NULL )
		{
			sp->EffectImplicitTargetB[1] = EFF_TARGET_PET;
			sp->EffectImplicitTargetB[2] = EFF_TARGET_PET;
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
		}

		// Warlock - Demonic Resilience
		sp = dbcSpell.LookupEntryForced( 30319 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
		}

		sp = dbcSpell.LookupEntryForced( 30320 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
		}

		sp = dbcSpell.LookupEntryForced( 30321 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
		}

		// Warlock - Improved Imp
		sp = dbcSpell.LookupEntryForced( 18694 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}

		sp = dbcSpell.LookupEntryForced( 18695 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}

		sp = dbcSpell.LookupEntryForced( 18696 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}

		// Warlock - Demonic Brutality
		sp = dbcSpell.LookupEntryForced( 18705 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}

		sp = dbcSpell.LookupEntryForced( 18706 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}

		sp = dbcSpell.LookupEntryForced( 18707 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}

		// Warlock - Improved Succubus
		sp = dbcSpell.LookupEntryForced( 18754 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
		}

		sp = dbcSpell.LookupEntryForced( 18755 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
		}

		sp = dbcSpell.LookupEntryForced( 18756 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
		}

		// Warlock - Fel Vitality, 3 is intellect, 2 is stamina
		sp = dbcSpell.LookupEntryForced( 18731 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_PERCENT_STAT;
			sp->EffectMiscValue[0] = 3;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}

		sp = dbcSpell.LookupEntryForced( 18743 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_PERCENT_STAT;
			sp->EffectMiscValue[0] = 3;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}

		sp = dbcSpell.LookupEntryForced( 18744 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_PERCENT_STAT;
			sp->EffectMiscValue[0] = 3;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}

		// Warlock - Unholy Power
		sp = dbcSpell.LookupEntryForced( 18769 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			//this is required since blizz uses spells for melee attacks while we use fixed functions
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
			sp->EffectMiscValue[1] = SCHOOL_NORMAL;
			sp->EffectBasePoints[1] = sp->EffectBasePoints[0] ;
		}

		sp = dbcSpell.LookupEntryForced( 18770 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			//this is required since blizz uses spells for melee attacks while we use fixed functions
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
			sp->EffectMiscValue[1] = SCHOOL_NORMAL;
			sp->EffectBasePoints[1] = sp->EffectBasePoints[0] ;
		}

		sp = dbcSpell.LookupEntryForced( 18771 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			//this is required since blizz uses spells for melee attacks while we use fixed functions
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
			sp->EffectMiscValue[1] = SCHOOL_NORMAL;
			sp->EffectBasePoints[1] = sp->EffectBasePoints[0] ;
		}

		sp = dbcSpell.LookupEntryForced( 18772 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			//this is required since blizz uses spells for melee attacks while we use fixed functions
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
			sp->EffectMiscValue[1] = SCHOOL_NORMAL;
			sp->EffectBasePoints[1] = sp->EffectBasePoints[0] ;
		}

		sp = dbcSpell.LookupEntryForced( 18773 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			//this is required since blizz uses spells for melee attacks while we use fixed functions
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
			sp->EffectMiscValue[1] = SCHOOL_NORMAL;
			sp->EffectBasePoints[1] = sp->EffectBasePoints[0] ;
		}

		// Warlock - Master Demonologist - 25 spells here
		sp = dbcSpell.LookupEntryForced( 23785 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET | SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 23784;
		}

		sp = dbcSpell.LookupEntryForced( 23822 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET | SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 23830;
		}

		sp = dbcSpell.LookupEntryForced( 23823 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET | SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 23831;
		}

		sp = dbcSpell.LookupEntryForced( 23824 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET | SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 23832;
		}

		sp = dbcSpell.LookupEntryForced( 23825 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET | SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 35708;
		}

		sp = dbcSpell.LookupEntryForced( 23784 );
		if( sp != NULL )
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;

		sp = dbcSpell.LookupEntryForced( 23830 );
		if( sp != NULL )
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;

		sp = dbcSpell.LookupEntryForced( 23831 );
		if( sp != NULL )
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;

		sp = dbcSpell.LookupEntryForced( 23832 );
		if( sp != NULL )
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;

		sp = dbcSpell.LookupEntryForced( 35708 );
		if( sp != NULL )
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;

		sp = dbcSpell.LookupEntryForced( 23759 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
		}

		sp = dbcSpell.LookupEntryForced( 23760 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
		}

		sp = dbcSpell.LookupEntryForced( 23761 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
		}

		sp = dbcSpell.LookupEntryForced( 23762 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
		}

		sp = dbcSpell.LookupEntryForced( 23826 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
		}

		sp = dbcSpell.LookupEntryForced( 23827 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
		}

		sp = dbcSpell.LookupEntryForced( 23828 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
		}

		sp = dbcSpell.LookupEntryForced( 23829 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
		}

		for(uint32 i=23833;i<=23844;i++)
		{
			sp = dbcSpell.LookupEntryForced( i );
			if( sp != NULL )
			{
				sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
				sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			}
		}

		sp = dbcSpell.LookupEntryForced( 35702 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->Effect[1] = 0; //hacks, we are handling this in another way
		}

		sp = dbcSpell.LookupEntryForced( 35703 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->Effect[1] = 0; //hacks, we are handling this in another way
		}

		sp = dbcSpell.LookupEntryForced( 35704 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->Effect[1] = 0; //hacks, we are handling this in another way
		}

		sp = dbcSpell.LookupEntryForced( 35705 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->Effect[1] = 0; //hacks, we are handling this in another way
		}

		sp = dbcSpell.LookupEntryForced( 35706 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->Effect[1] = 0; //hacks, we are handling this in another way
		}

		/**********************************************************
		 *	Destruction
		 **********************************************************/

		// Warlock - Chaos Bolt
		sp = dbcSpell.LookupEntryForced( 50796 );
		if( sp != NULL )
			sp->Attributes |= ATTRIBUTES_IGNORE_INVULNERABILITY;

		sp = dbcSpell.LookupEntryForced( 59170 );
		if( sp != NULL )
			sp->Attributes |= ATTRIBUTES_IGNORE_INVULNERABILITY;

		sp = dbcSpell.LookupEntryForced( 59171 );
		if( sp != NULL )
			sp->Attributes |= ATTRIBUTES_IGNORE_INVULNERABILITY;

		sp = dbcSpell.LookupEntryForced( 59172 );
		if( sp != NULL )
			sp->Attributes |= ATTRIBUTES_IGNORE_INVULNERABILITY;

	//////////////////////////////////////////
	// WARRIOR								//
	//////////////////////////////////////////

		/**********************************************************
		 *	Arms
		 **********************************************************/

		// Warrior - Juggernaut
		sp = dbcSpell.LookupEntryForced( 65156 );
		if(sp != NULL)
			sp->AuraInterruptFlags = AURA_INTERRUPT_ON_CAST_SPELL;

		// Warrior - Tactical Mastery Rank 1
		sp = dbcSpell.LookupEntry( 0x00003007 );
		if(sp != NULL)
			sp->RequiredShapeShift = 0x00070000;

		// Warrior - Tactical Mastery Rank 2
		sp = dbcSpell.LookupEntry( 0x00003184 );
		if(sp != NULL)
			sp->RequiredShapeShift = 0x00070000;

		// Warrior - Tactical Mastery Rank 3
		sp = dbcSpell.LookupEntry( 0x00003185 );
		if(sp != NULL)
			sp->RequiredShapeShift = 0x00070000;

		/**********************************************************
		 *	Fury
		 **********************************************************/

		// Warrior - Bloodthirst new version is ok but old version is wrong from now on :(
		sp = dbcSpell.LookupEntryForced( 23881 );
		if( sp != NULL )
		{
			sp->Effect[1] = 64; //cast on us, it is good
			sp->EffectTriggerSpell[1] = 23885; //evil , but this is good for us :D
		}
		sp = dbcSpell.LookupEntryForced( 23892 );
		if( sp != NULL )
		{
			sp->Effect[1] = 64;
			sp->EffectTriggerSpell[1] = 23886; //evil , but this is good for us :D
		}
		sp = dbcSpell.LookupEntryForced( 23893 );
		if( sp != NULL )
		{
			sp->Effect[1] = 64; //
			sp->EffectTriggerSpell[1] = 23887; //evil , but this is good for us :D
		}
		sp = dbcSpell.LookupEntryForced( 23894 );
		if( sp != NULL )
		{
			sp->Effect[1] = 64; //
			sp->EffectTriggerSpell[1] = 23888; //evil , but this is good for us :D
		}
		sp = dbcSpell.LookupEntryForced( 25251 );
		if( sp != NULL )
		{
			sp->Effect[1] = 64; //aura
			sp->EffectTriggerSpell[1] = 25252; //evil , but this is good for us :D
		}
		sp = dbcSpell.LookupEntryForced( 30335 );
		if( sp != NULL )
		{
			sp->Effect[1] = 64; //aura
			sp->EffectTriggerSpell[1] = 30339; //evil , but this is good for us :D
		}

		/**********************************************************
		 *	Protection
		 **********************************************************/

		// Warrior - Intervene, Ranger: stop attack
		sp = dbcSpell.LookupEntryForced( 3411 );
		if( sp != NULL )
			sp->Attributes |= ATTRIBUTES_STOP_ATTACK;

	//////////////////////////////////////////
	// ITEMS								//
	//////////////////////////////////////////

	// Insert items spell fixes here

		//Compact Harvest Reaper
		sp = dbcSpell.LookupEntryForced( 4078 );
		if( sp != NULL )
			sp->DurationIndex = 6;

		//Graccu's Mince Meat Fruitcake
		sp = dbcSpell.LookupEntryForced(25990);
		if( sp != NULL )
			sp->EffectAmplitude[1] = 1000;

		//Extract Gas
		sp = dbcSpell.LookupEntryForced( 30427 );
		if( sp != NULL )
			sp->Effect[0] = SPELL_EFFECT_DUMMY;

		//Spell Focus Trigger (Mystical Skyfire Diamond)
		sp = dbcSpell.LookupEntryForced( 32837 );
		if( sp != NULL )
			sp->procChance = 15;

		//Frost Arrow
		sp = dbcSpell.LookupEntryForced( 29501 );
		if( sp != NULL )
			sp->procChance = 30;//some say it is triggered every now and then

		//Purify helboar meat
		sp = dbcSpell.LookupEntryForced( 29200 );
		if( sp != NULL )
		{
			sp->Reagent[1] = 0;
			sp->ReagentCount[1] = 0;
			sp->Effect[0] = SPELL_EFFECT_CREATE_ITEM;
		}

		// Drums of war targets surrounding party members instead of us
		sp = dbcSpell.LookupEntryForced( 35475 );
		if( sp != NULL )
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_ALL_PARTY;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_ALL_PARTY;
			sp->EffectImplicitTargetA[2] = 0;
			sp->EffectImplicitTargetB[0] = 0;
			sp->EffectImplicitTargetB[1] = 0;
			sp->EffectImplicitTargetB[2] = 0;
		}

		// Drums of Battle targets surrounding party members instead of us
		sp = dbcSpell.LookupEntryForced( 35476 );
		if( sp != NULL )
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_ALL_PARTY;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_ALL_PARTY;
			sp->EffectImplicitTargetA[2] = 0;
			sp->EffectImplicitTargetB[0] = 0;
			sp->EffectImplicitTargetB[1] = 0;
			sp->EffectImplicitTargetB[2] = 0;
		}

		// Drums of Panic targets surrounding creatures instead of us
		sp = dbcSpell.LookupEntryForced( 35474 );
		if( sp != NULL )
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER;
			sp->EffectImplicitTargetA[2] = 0;
			sp->EffectImplicitTargetB[0] = 0;
			sp->EffectImplicitTargetB[1] = 0;
			sp->EffectImplicitTargetB[2] = 0;
		}

		// Drums of Restoration targets surrounding party members instead of us
		sp = dbcSpell.LookupEntryForced( 35478 );
		if( sp != NULL )
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_ALL_PARTY;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_ALL_PARTY;
			sp->EffectImplicitTargetA[2] = 0;
			sp->EffectImplicitTargetB[0] = 0;
			sp->EffectImplicitTargetB[1] = 0;
			sp->EffectImplicitTargetB[2] = 0;
		}

		// Drums of Speed targets surrounding party members instead of us
		sp = dbcSpell.LookupEntryForced( 35477 );
		if( sp != NULL )
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_ALL_PARTY;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_ALL_PARTY;
			sp->EffectImplicitTargetA[2] = 0;
			sp->EffectImplicitTargetB[0] = 0;
			sp->EffectImplicitTargetB[1] = 0;
			sp->EffectImplicitTargetB[2] = 0;
		}

		//all Drums
		sp = dbcSpell.LookupEntryForced( 35474 );
		if( sp != NULL )
			sp->RequiredShapeShift = 0;

		sp = dbcSpell.LookupEntryForced( 35475 );
		if( sp != NULL )
			sp->RequiredShapeShift = 0;

		sp = dbcSpell.LookupEntryForced( 35476 );
		if( sp != NULL )
			sp->RequiredShapeShift = 0;

		sp = dbcSpell.LookupEntryForced( 35477 );
		if( sp != NULL )
			sp->RequiredShapeShift = 0;

		sp = dbcSpell.LookupEntryForced( 35478 );
		if( sp != NULL )
			sp->RequiredShapeShift = 0;

		//Item Set: Malorne Harness
		sp = dbcSpell.LookupEntryForced( 37306 );
		if( sp != NULL )
			sp->procChance = 4;

		sp = dbcSpell.LookupEntryForced( 37311 );
		if( sp != NULL )
			sp->procChance = 4;

		//Item Set: Deathmantle
		sp = dbcSpell.LookupEntryForced( 37170 );
		if( sp != NULL )
			sp->procChance = 4;

		//Item Set: Netherblade
		sp = dbcSpell.LookupEntryForced( 37168 );
		if( sp != NULL )
			sp->procChance = 15;

		//Item Set: Avatar Regalia
		sp = dbcSpell.LookupEntryForced( 37600 );
		if( sp != NULL )
			sp->procChance = 6;

		//Item Set: Voidheart Raiment
		sp = dbcSpell.LookupEntryForced( 37377 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance = 5;
			sp->proc_interval = 20;
			sp->EffectTriggerSpell[0] = 37379;
		}
		sp = dbcSpell.LookupEntryForced( 39437 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance = 5;
			sp->proc_interval = 20;
			sp->EffectTriggerSpell[0] = 37378;
		}

		//Item Set: Cataclysm Raiment
		sp = dbcSpell.LookupEntryForced( 37227 );
		if( sp != NULL )
		{
			sp->proc_interval = 60000;
			sp->procChance = 100;
		}

		//Item Set: Cataclysm Regalia
		sp = dbcSpell.LookupEntryForced( 37228 );
		if( sp != NULL )
			sp->procChance = 7;

		sp = dbcSpell.LookupEntryForced( 37237 );
		if( sp != NULL )
			sp->procChance = 25;

		//Item Set: Cataclysm Harness
		sp = dbcSpell.LookupEntryForced( 37239 );
		if( sp != NULL )
			sp->procChance = 2;

		//Item Set: Cyclone Regalia
		sp = dbcSpell.LookupEntryForced( 37213 );
		if( sp != NULL )
			sp->procChance = 11;

		//Item Set: Lightbringer Battlegear
		sp = dbcSpell.LookupEntryForced( 38427 );
		if( sp != NULL )
			sp->procChance = 20;

		//Item Set: Crystalforge Battlegear
		sp = dbcSpell.LookupEntryForced( 37195 );
		if( sp != NULL )
			sp->procChance = 6;

		//Item Set: Crystalforge Raiment
		sp = dbcSpell.LookupEntryForced( 37189 );
		if( sp != NULL )
			sp->proc_interval = 60000;

		//Item Set: Destroyer Armor
		sp = dbcSpell.LookupEntryForced( 37525 );
		if( sp != NULL )
			sp->procChance = 7;

		//Item Set: Destroyer Battlegear
		sp = dbcSpell.LookupEntryForced( 37528 );
		if( sp != NULL )
			sp->procChance = 100;

		//Item Set: Warbringer Armor
		sp = dbcSpell.LookupEntryForced( 37516 );
		if( sp != NULL )
			sp->procChance = 100;

		//Item Set: Shadowcraft Armor & Darkmantle Armor
		sp = dbcSpell.LookupEntryForced( 27787 );
		if( sp != NULL)
			sp->procChance = 7;

		// Item Set: Warlock Tier 7 Heroes' Plagueheart Garb
		sp = dbcSpell.LookupEntryForced( 60172 );
		if( sp != NULL )
			sp->procChance = 100;

		//Item Set: Warbringer Armor
		// 2 pieces: You have a chance each time you parry to gain Blade Turning, absorbing 200 damage for 15 sec.
		sp = dbcSpell.LookupEntryForced( 37514 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 37515;
			sp->procChance = 25;
		}

		//Thorium Grenade
		sp = dbcSpell.LookupEntryForced(19769);
		if(sp != NULL)
		{
			sp->InterruptFlags |= ~(CAST_INTERRUPT_ON_MOVEMENT);
		}

		//M73 Frag Grenade
		sp = dbcSpell.LookupEntryForced(13808);
		if(sp != NULL)
		{
			sp->InterruptFlags |= ~(CAST_INTERRUPT_ON_MOVEMENT);
		}

		//Iron Grenade
		sp = dbcSpell.LookupEntryForced(4068);
		if(sp != NULL)
		{
			sp->InterruptFlags |= ~(CAST_INTERRUPT_ON_MOVEMENT);
		}

		//Frost Grenade
		sp = dbcSpell.LookupEntryForced(39965);
		if(sp != NULL)
		{
			sp->InterruptFlags |= ~(CAST_INTERRUPT_ON_MOVEMENT);
		}

		// Adamantine Grenade
		sp = dbcSpell.LookupEntryForced( 30217 );
		if( sp != NULL )
		{
			sp->InterruptFlags |= ~(CAST_INTERRUPT_ON_MOVEMENT);
		}

		/**************************************************************
		* Trinkets and Rings Fixes		Please keep nice and clean :) *
		**************************************************************/

		//Thunderfury
		sp = dbcSpell.LookupEntryForced( 21992 );
		if( sp != NULL )
			sp->EffectImplicitTargetA[2] = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER; // cebernic: for enemies not self

		// Vestige of Haldor
		sp = dbcSpell.LookupEntryForced( 60306 );
		if( sp != NULL )
			sp->proc_interval = 45000;

		// Forge Ember
		sp = dbcSpell.LookupEntryForced( 60473 );
		if( sp != NULL )
			sp->proc_interval = 45000;

		// Mirror of Truth
		sp = dbcSpell.LookupEntryForced( 33648 );
		if( sp != NULL )
			sp->proc_interval = 45000;

		// Flow of Knowledge
		sp = dbcSpell.LookupEntryForced( 62114 );
		if( sp != NULL )
			sp->proc_interval = 45000;

		// Embrace of the Spider
		sp = dbcSpell.LookupEntryForced( 60490 );
		if( sp != NULL )
			sp->proc_interval = 45000;

		// Anvil of Titans
		sp = dbcSpell.LookupEntryForced( 62115 );
		if( sp != NULL )
			sp->proc_interval = 45000;

		// Soul of the Dead
		sp = dbcSpell.LookupEntryForced( 60537 );
		if( sp != NULL )
			sp->proc_interval = 45000;

		// Grim Toll
		sp = dbcSpell.LookupEntryForced( 60436 );
		if( sp != NULL )
			sp->proc_interval = 45000;

		// Bandit's Insignia
		sp = dbcSpell.LookupEntryForced( 60442 );
		if( sp != NULL )
			sp->proc_interval = 45000;

		// Meteorite Whetstone
		sp = dbcSpell.LookupEntryForced( 60301 );
		if( sp != NULL )
			sp->proc_interval = 45000;

		// Sonic Booster
		sp = dbcSpell.LookupEntryForced( 54707 );
		if( sp != NULL )
			sp->proc_interval = 60000;

		//Ashtongue Talisman of Lethality
		sp = dbcSpell.LookupEntryForced( 40460 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance = 20;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectTriggerSpell[0] = 40461;
			sp->maxstack = 1;
		}

		//Ashtongue Talisman of Equilibrium
		sp = dbcSpell.LookupEntryForced( 40442 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 25;
			sp->maxstack = 1;
			sp->Effect[0] = 6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 40452;
			sp->Effect[1] = 6;
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1] = 40445;
			sp->Effect[2] = 6;
			sp->EffectApplyAuraName[2] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[2] = 40446;
		}

		//Ashtongue Talisman of Acumen
		sp = dbcSpell.LookupEntryForced( 40438 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance = 10;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectTriggerSpell[0] = 40441;
			sp->maxstack = 1;
			sp->Effect[1] = 6;
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1] = 40440;
		}

		// Serpent-Coil Braid - should be moved to override
		sp = dbcSpell.LookupEntryForced( 37447 );
		if( sp != NULL )
		{
			sp->Effect[1] = 6;
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance = 100;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectTriggerSpell[1] = 37445;
			sp->maxstack = 1;
		}

		// Band of the Eternal Champion
		sp = dbcSpell.LookupEntryForced( 35080 );
		if( sp != NULL )
			sp->proc_interval = 60000;

		// Band of the Eternal Sage
		sp = dbcSpell.LookupEntryForced( 35083 );
		if( sp != NULL )
			sp->proc_interval = 60000;

		// Band of the Eternal Restorer
		sp = dbcSpell.LookupEntryForced( 35086 );
		if( sp != NULL )
			sp->proc_interval = 60000;

		// Band of the Eternal Defender
		sp = dbcSpell.LookupEntryForced( 35077 );
		if( sp != NULL )
			sp->proc_interval = 60000;

		// Memento of Tyrande
		sp = dbcSpell.LookupEntryForced( 37655 );
		if( sp != NULL )
			sp->procChance = 10;

		// Dragonspine Trophy
 		sp = dbcSpell.LookupEntryForced( 34774 );
		if( sp != NULL )
		{
			sp->procChance = 6;
			sp->proc_interval = 30000;
		}

		// Swordguard Embroidery
		sp = dbcSpell.LookupEntryForced( 55776 );
		if( sp != NULL )
			sp->proc_interval = 60000;

		// Lightweave Embroidery
		sp = dbcSpell.LookupEntryForced( 55640 );
		if( sp != NULL )
			sp->proc_interval = 45000;

		// Darkglow Embroidery
		sp = dbcSpell.LookupEntryForced( 55768 );
		if( sp != NULL )
			sp->proc_interval = 60000;

		// Sundial of the Exiled
		sp = dbcSpell.LookupEntryForced( 60063 );
		if( sp != NULL )
			sp->proc_interval = 45000;

		// Je'Tze's Bell
		sp = dbcSpell.LookupEntryForced( 49622 );
		if( sp != NULL )
			sp->proc_interval = 45000;

		// Tears of Bitter Anguish
		sp = dbcSpell.LookupEntryForced( 58901 );
		if( sp != NULL )
			sp->proc_interval = 60000;

		// Embrace of the Spider
		sp = dbcSpell.LookupEntryForced( 60490 );
		if( sp != NULL )
			sp->proc_interval = 30000;

		// Dying Curse
		sp = dbcSpell.LookupEntryForced( 60493 );
		if( sp != NULL )
			sp->proc_interval = 45000;

		// Fury of the Five Flights
		sp = dbcSpell.LookupEntryForced( 60313 );
		if( sp != NULL )
			sp->maxstack = 20;

		// Warp-Spring Coil
		sp = dbcSpell.LookupEntryForced( 37173 );
		if( sp != NULL )
			sp->proc_interval = 30000;

		// Flow of Knowledge
		sp = dbcSpell.LookupEntryForced ( 62114 );
        if( sp != NULL )
			sp->proc_interval = 45000;

		// Majestic Dragon Figurine
		sp = dbcSpell.LookupEntryForced( 60524 );
		if( sp != NULL )
			sp->proc_interval = 100;

		// Illustration of the Dragon Soul
		sp = dbcSpell.LookupEntryForced( 60485 );
		if( sp != NULL )
			sp->proc_interval = 100;

		// Barricade of Eternity
		sp = dbcSpell.LookupEntryForced( 40475 );
		if( sp != NULL )
			sp->procChance = 50;

		// Figurine - Shadowsong Panther
		sp = dbcSpell.LookupEntryForced( 46784 );
		if( sp != NULL )
			sp->AttributesEx |= ATTRIBUTESEX_NOT_BREAK_STEALTH;

		// Infernal Protection
		sp = dbcSpell.LookupEntryForced( 36488 );
		if( sp != NULL )
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SINGLE_FRIEND;

		// Madness of the Betrayer - Black Temple Melee Trinket
		sp = dbcSpell.LookupEntryForced( 40475 );
		if( sp != NULL )
			sp->procChance = 5;

	//////////////////////////////////////////
	// SIGIL								//
	//////////////////////////////////////////

		// Deadly Gladiator's Death Knight Relic
		sp = dbcSpell.LookupEntryForced( 60686 );
		if( sp != NULL )
			sp->ProcOnNameHash[0] = SPELL_HASH_PLAGUE_STRIKE;

		// Sigil of the Unfaltering Knight
		sp = dbcSpell.LookupEntryForced( 62147 );
		if( sp != NULL )
		{
			sp->ProcOnNameHash[0] = SPELL_HASH_ICY_TOUCH;
			sp->procChance = 100;
			sp->proc_interval = 45000;
		}

		// Sigil of Haunted Dreams
		sp = dbcSpell.LookupEntryForced( 60826 );
		if( sp != NULL )
		{
			sp->ProcOnNameHash[0] = SPELL_HASH_BLOOD_STRIKE;
			sp->ProcOnNameHash[1] = SPELL_HASH_HEART_STRIKE;
			sp->proc_interval = 45000;
		}

	//////////////////////////////////////////
	// IDOL									//
	//////////////////////////////////////////

		//Idol of the Unseen Moon
		sp = dbcSpell.LookupEntryForced( 43739 );
		if( sp != NULL )
			sp->ProcOnNameHash[0] = SPELL_HASH_MOONFIRE;

		//Idol of Terror
		sp = dbcSpell.LookupEntryForced( 43737 );
		if( sp != NULL )
		{
			sp->proc_interval = 10000; 
			sp->ProcOnNameHash[0] = SPELL_HASH_MANGLE___CAT;
			sp->ProcOnNameHash[1] = SPELL_HASH_MANGLE___BEAR;
		}

		// Deadly Gladiator's Idol of Resolve
		sp = dbcSpell.LookupEntryForced( 60696 );
		if( sp != NULL )
		{
			sp->ProcOnNameHash[0] = SPELL_HASH_MANGLE___CAT;
			sp->ProcOnNameHash[1] = SPELL_HASH_MANGLE___BEAR;
		}

	//////////////////////////////////////////
	// TOTEMS								//
	//////////////////////////////////////////

		// Deadly Totem of Indomitability
		sp = dbcSpell.LookupEntryForced( 60548 );
		if( sp != NULL )
			sp->ProcOnNameHash[0] = SPELL_HASH_LAVA_LASH;

		sp = dbcSpell.LookupEntryForced( 60549 );
		if( sp != NULL )
		{
			// it have 2 same effects dunno why
			sp->EffectApplyAuraName[1] = 0;
			sp->EffectBasePoints[1] = 0;
		}

		// Totem of the Third Wind - bad range
		SpellEntry * sp_healing_wave = dbcSpell.LookupEntryForced( 8004 );
		sp = dbcSpell.LookupEntryForced( 34132 );
		if( sp != NULL )
			sp->rangeIndex = sp_healing_wave->rangeIndex;

		sp = dbcSpell.LookupEntryForced( 42371 );
		if( sp != NULL )
			sp->rangeIndex = sp_healing_wave->rangeIndex;

		sp = dbcSpell.LookupEntryForced( 43729 );
		if( sp != NULL )
			sp->rangeIndex = sp_healing_wave->rangeIndex;

		sp = dbcSpell.LookupEntryForced( 46099 );
		if( sp != NULL )
			sp->rangeIndex = sp_healing_wave->rangeIndex;

	//////////////////////////////////////////
	// LIBRAMS								//
	//////////////////////////////////////////

		// Deadly Gladiator's Libram of Fortitude
		sp = dbcSpell.LookupEntryForced( 60633 );
		if( sp != NULL )
			sp->ProcOnNameHash[0] = SPELL_HASH_CRUSADER_STRIKE;


		
		//Libram of Saints Departed and Libram of Zeal
		sp = dbcSpell.LookupEntryForced( 34263 );
		if( sp != NULL )
			sp->ProcOnNameHash[0] = SPELL_HASH_JUDGEMENT;	

		//Libram of Avengement 
		sp = dbcSpell.LookupEntryForced( 34260 );
		if( sp != NULL )
			sp->ProcOnNameHash[0] = SPELL_HASH_JUDGEMENT;	

		//Libram of Mending
		sp = dbcSpell.LookupEntryForced( 43742 );
		if( sp != NULL )
			sp->ProcOnNameHash[0] = SPELL_HASH_HOLY_LIGHT;	

	//////////////////////////////////////////
	// BOSSES								//
	//////////////////////////////////////////

		// Insert boss spell fixes here

		// Major Domo - Magic Reflection
		sp = dbcSpell.LookupEntryForced(20619);
		if(sp != NULL)
		{
			for(int i= 0; i<3; i++)
			{
				if(sp->EffectImplicitTargetA[i] > 0)
					sp->EffectImplicitTargetA[i] = EFF_TARGET_ALL_FRIENDLY_IN_AREA;
				if(sp->EffectImplicitTargetB[i] > 0)
					sp->EffectImplicitTargetB[i] = EFF_TARGET_ALL_FRIENDLY_IN_AREA;
			}
		}

		// Major Domo - Damage Shield
		sp = dbcSpell.LookupEntryForced(21075);
		if(sp != NULL)
		{
			for(int i= 0; i<3; i++)
			{
				if(sp->EffectImplicitTargetA[i] > 0)
					sp->EffectImplicitTargetA[i] = EFF_TARGET_ALL_FRIENDLY_IN_AREA;
				if(sp->EffectImplicitTargetB[i] > 0)
					sp->EffectImplicitTargetB[i] = EFF_TARGET_ALL_FRIENDLY_IN_AREA;
			}
		}

		// Dark Glare
		sp = dbcSpell.LookupEntryForced( 26029 );
		if( sp != NULL )
			sp->cone_width = 15.0f; // 15 degree cone

		// Drain Power (Malacrass) // bugged - the charges fade even when refreshed with new ones. This makes them everlasting.
		sp = dbcSpell.LookupEntryForced( 44131 );
 		if( sp != NULL )
			sp->DurationIndex = 21;

		sp = dbcSpell.LookupEntryForced( 44132 );
		if( sp != NULL )
			sp->DurationIndex = 21;

		// Zul'jin spell, proc from Creeping Paralysis
		sp = dbcSpell.LookupEntryForced( 43437 );
 		if( sp != NULL )
		{
			sp->EffectImplicitTargetA[0] = 0;
			sp->EffectImplicitTargetA[1] = 0;
		}

		//Bloodboil
		sp = dbcSpell.LookupEntryForced( 42005 );
 		if( sp != NULL )
			sp->MaxTargets = 5;

		//Moroes - Gouge 
		sp = dbcSpell.LookupEntryForced( 29425 );
 		if( sp != NULL )
			sp->MaxTargets = 1;

		sp = dbcSpell.LookupEntryForced( 28456 );
 		if( sp != NULL )
			sp->MaxTargets = 1;

		//Anthropy
		sp = dbcSpell.LookupEntryForced( 40327 );
 		if( sp != NULL )
			sp->maxstack = 10;

		//Doom
		sp = dbcSpell.LookupEntryForced( 31347 );
		if( sp != NULL )	
			sp->MaxTargets = 1;

		//Shadow of Death
		sp = dbcSpell.LookupEntryForced( 40251 );
		if( sp != NULL )
			sp->EffectApplyAuraName[0] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;

		// Recently Dropped Flag
		sp = dbcSpell.LookupEntryForced( 42792 );
		if (sp != NULL)
			sp->c_is_flags |= SPELL_FLAG_IS_FORCEDDEBUFF;

		
	//////////////////////////////////////////
	// cebernic 's fixes					//
	//////////////////////////////////////////

		//resurrection sickness
		sp = dbcSpell.LookupEntryForced( 15007 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_FORCEDDEBUFF;
		}
		// ghost ,NIGHTELF ghost & sprit
		sp = dbcSpell.LookupEntryForced( 20584 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_FORCEDDEBUFF;
		}

		sp = dbcSpell.LookupEntryForced( 9036 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_FORCEDDEBUFF;
		}

		sp = dbcSpell.LookupEntryForced( 8326 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_FORCEDDEBUFF;
		}

		sp = dbcSpell.LookupEntryForced( 26013 ); //bg deserter
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_FORCEDDEBUFF;
		}

		sp = dbcSpell.LookupEntryForced( 24379 ); //bg Restoration
		if( sp != NULL )
		{
			sp->EffectTriggerSpell[0] = 23493;
			sp->c_is_flags = SPELL_FLAG_IS_FORCEDBUFF;
		}

		sp = dbcSpell.LookupEntryForced( 23493 ); //bg Restoration
		if( sp != NULL )
		{
			sp->EffectTriggerSpell[0] = 24379;
			sp->c_is_flags = SPELL_FLAG_IS_FORCEDBUFF;
		}

		//Bandage
		sp = dbcSpell.LookupEntryForced( 11196 );
		if( sp != NULL )
			sp->c_is_flags = SPELL_FLAG_IS_FORCEDDEBUFF;

		// Noggenfogger elixir - reduce size effect
		/*sp = dbcSpell.LookupEntryForced( 16595 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_SCALE;
			sp->EffectBasePoints[0] = -50;
			sp->maxstack = 1;
      	}*/

	//////////////////////////////////////////////////////
	// GAME-OBJECT SPELL FIXES                          //
	//////////////////////////////////////////////////////

		// Blessing of Zim'Torga
		sp = dbcSpell.LookupEntryForced( 51729 );
		if( sp )
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SCRIPTED_OR_SINGLE_TARGET;
			sp->c_is_flags |= SPELL_FLAG_IS_FORCEDBUFF;
		}

		// Blessing of Zim'Abwa
		sp = dbcSpell.LookupEntryForced( 51265 );
		if( sp )
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SCRIPTED_OR_SINGLE_TARGET;
			sp->c_is_flags |= SPELL_FLAG_IS_FORCEDBUFF;
		}

		// Blessing of Zim'Rhuk
		sp = dbcSpell.LookupEntryForced( 52051 );
		if( sp )
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SCRIPTED_OR_SINGLE_TARGET;
			sp->c_is_flags |= SPELL_FLAG_IS_FORCEDBUFF;
		}

	//////////////////////////////////////////////////////
	// OTHER FIXES				                        //
	//////////////////////////////////////////////////////

		// PROFFESION - Enchant Cloak - Major Resistance
		sp = dbcSpell.LookupEntryForced( 27962 );
		if( sp != NULL )
			sp->EffectMiscValue[0] = 2998;

		sp = dbcSpell.LookupEntryForced( 36285 );
		if( sp != NULL )
			sp->EffectMiscValue[0] = 2998;

		// Tag Murloc
		sp = dbcSpell.LookupEntryForced( 30877 );
		if( sp != NULL )
			sp->EffectImplicitTargetB[0] = 0;

		// Taint of Shadow
		sp = dbcSpell.LookupEntryForced( 23179 );
		if( sp != NULL )
			sp->EffectMiscValue[0] = 1434;

		sp = dbcSpell.LookupEntryForced( 21050 );		// Melodious Rapture rat quest 
		if( sp != NULL )
			sp->RequiresAreaId = 0;

		sp = dbcSpell.LookupEntryForced( 42521 );		// Cleansing Witch Hill q item
		if( sp != NULL )
			sp->RequiresAreaId = 0;

		sp = dbcSpell.LookupEntryForced( 24815 );		// Draw Ancient Glyphs - spell for quest
		if( sp != NULL )
			sp->RequiresAreaId = 3257;

		sp = dbcSpell.LookupEntryForced( 41423 );		// Hungry Nether Rays quest item
		if( sp != NULL )
			sp->RequiresAreaId = 3679;

		sp = dbcSpell.LookupEntryForced( 39844 );		// Fires Over Skettis quest item
		if( sp != NULL )
			sp->RequiresAreaId = 0;

		sp = dbcSpell.LookupEntryForced( 39527 );		// Ashtongue Ruse
		if( sp != NULL )
			sp->RequiresAreaId = 0;

		sp = dbcSpell.LookupEntryForced( 42090 );		// Ashtongue Ruse
		if( sp != NULL )
			sp->RequiresAreaId = 0;

		sp = dbcSpell.LookupEntryForced( 44856 );		// Bash'ir Phasing Device
		if( sp != NULL )
			sp->AuraInterruptFlags = AURA_INTERRUPT_ON_LEAVE_AREA;

		sp = dbcSpell.LookupEntryForced( 27997 );		//Spellsurge
		if( sp != NULL )
			sp->proc_interval = 30000; // Wowhead Comment

		sp = dbcSpell.LookupEntryForced( 24574 );		// Zandalarian Hero Badge 24590 24575 O_O
		if( sp != NULL )
		{
			sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 24590;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance = 100;
		}

		//Tempfix for Stone Statues
		sp = dbcSpell.LookupEntryForced( 32253 );
		if( sp != NULL )
			sp->DurationIndex = 64;
		sp = dbcSpell.LookupEntryForced( 32787 );
		if( sp != NULL )
			sp->DurationIndex = 64;
		sp = dbcSpell.LookupEntryForced( 32788 );
		if( sp != NULL )
			sp->DurationIndex = 64;
		sp = dbcSpell.LookupEntryForced( 32790 );
		if( sp != NULL )
			sp->DurationIndex = 64;
		sp = dbcSpell.LookupEntryForced( 32791 );
		if( sp != NULL )
			sp->DurationIndex = 64;
}