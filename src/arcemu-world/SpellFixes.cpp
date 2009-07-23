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
	memset(sp, 0, sizeof(SpellEntry));
	sp->Id = id;
	sp->Attributes = 384;
	sp->AttributesEx = 268435456;
	sp->AttributesExB = 4;
	sp->CastingTimeIndex=1;
	sp->procChance=75;
	sp->rangeIndex=13;
	sp->EquippedItemClass=uint32(-1);
	sp->Effect[0]=3;
	sp->EffectImplicitTargetA[0]=25;
	sp->NameHash=crc32((const unsigned char*)name, (unsigned int)strlen(name));
	sp->dmg_multiplier[0]=1.0f;
	sp->StanceBarOrder=-1;
	dbcSpell.SetRow(id,sp);
	sWorld.dummyspells.push_back(sp);
}

void ApplyNormalFixes()
{
	//Updating spell.dbc

	Log.Notice("World", "Processing %u spells...", dbcSpell.GetNumRows());

	uint32 cnt = dbcSpell.GetNumRows();
	uint32 effect;
	uint32 result;

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

	for(uint32 x=0; x < cnt; x++)
	{
		// Read every SpellEntry row
		SpellEntry * sp = dbcSpell.LookupRow(x);

		uint32 rank = 0;
		uint32 namehash = 0;

		sp->self_cast_only = false;
		sp->apply_on_shapeshift_change = false;
		sp->always_apply = false;

		// hash the name
		//!!!!!!! representing all strings on 32 bits is dangerous. There is a chance to get same hash for a lot of strings ;)
        namehash = crc32((const unsigned char*)sp->Name, (unsigned int)strlen(sp->Name));
		sp->NameHash   = namehash; //need these set before we start processing spells

		float radius=std::max(::GetRadius(dbcSpellRadius.LookupEntry(sp->EffectRadiusIndex[0])),::GetRadius(dbcSpellRadius.LookupEntry(sp->EffectRadiusIndex[1])));
		radius=std::max(::GetRadius(dbcSpellRadius.LookupEntry(sp->EffectRadiusIndex[2])),radius);
		radius=std::max(GetMaxRange(dbcSpellRange.LookupEntry(sp->rangeIndex)),radius);
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
		for (i=0; i<8; i++)
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

		for(uint32 b=0;b<3;++b)
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
		sp->Dspell_coef_override = -1;
		sp->OTspell_coef_override = -1;
		sp->casttime_coef = 0;
		sp->fixed_dddhcoef = -1;
		sp->fixed_hotdotcoef = -1;

		talentSpellIterator = talentSpells.find(sp->Id);
		if(talentSpellIterator == talentSpells.end())
			sp->talent_tree = 0;
		else
			sp->talent_tree = talentSpellIterator->second;

		// parse rank text
		if( !sscanf( sp->Rank, "Rank %d", (unsigned int*)&rank) )
			rank = 0;

		//seal of command
		else if( namehash == SPELL_HASH_SEAL_OF_COMMAND )
			sp->Spell_Dmg_Type = SPELL_DMG_TYPE_MAGIC;

		//judgement of command
		else if( namehash == SPELL_HASH_JUDGEMENT_OF_COMMAND )
			sp->Spell_Dmg_Type = SPELL_DMG_TYPE_MAGIC;

		else if( namehash == SPELL_HASH_ARCANE_SHOT )
			sp->c_is_flags |= SPELL_FLAG_IS_NOT_USING_DMG_BONUS;

		else if( namehash == SPELL_HASH_SERPENT_STING )
			sp->c_is_flags |= SPELL_FLAG_IS_NOT_USING_DMG_BONUS;

		//Rogue: Poison time fix for 2.3
		if( strstr( sp->Name, "Crippling Poison") && sp->Effect[0] == 54 ) //I, II
			sp->EffectBasePoints[0] = 3599;
		if( strstr( sp->Name, "Mind-numbing Poison") && sp->Effect[0] == 54 ) //I,II,III
			sp->EffectBasePoints[0] = 3599;
		if( strstr( sp->Name, "Instant Poison") && sp->Effect[0] == 54 ) //I,II,III,IV,V,VI,VII
			sp->EffectBasePoints[0] = 3599;
		if( strstr( sp->Name, "Deadly Poison") && sp->Effect[0] == 54 ) //I,II,III,IV,V,VI,VII
			sp->EffectBasePoints[0] = 3599;
		if( strstr( sp->Name, "Wound Poison") && sp->Effect[0] == 54 ) //I,II,III,IV,V
			sp->EffectBasePoints[0] = 3599;
		if( strstr( sp->Name, "Anesthetic Poison") && sp->Effect[0] == 54 ) //I
			sp->EffectBasePoints[0] = 3599;

        if( strstr( sp->Name, "Sharpen Blade") && sp->Effect[0] == 54 ) //All BS stones
            sp->EffectBasePoints[0] = 3599;

		//these mostly do not mix so we can use else
        // look for seal, etc in name
        if( strstr( sp->Name, "Seal"))
            sp->BGR_one_buff_on_target |= SPELL_TYPE_SEAL;
		else if( strstr( sp->Name, "Hand of"))
            sp->BGR_one_buff_on_target |= SPELL_TYPE_HAND;
        else if( strstr( sp->Name, "Blessing"))
            sp->BGR_one_buff_on_target |= SPELL_TYPE_BLESSING;
        else if( strstr( sp->Name, "Curse"))
            sp->BGR_one_buff_on_target |= SPELL_TYPE_CURSE;
        else if( strstr( sp->Name, "Corruption"))
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
//		else if( strstr( sp->Name, "Curse of Weakness") || strstr( sp->Name, "Curse of Agony") || strstr( sp->Name, "Curse of Recklessness") || strstr( sp->Name, "Curse of Tongues") || strstr( sp->Name, "Curse of the Elements") || strstr( sp->Name, "Curse of Idiocy") || strstr( sp->Name, "Curse of Shadow") || strstr( sp->Name, "Curse of Doom"))
//		else if(namehash==4129426293 || namehash==885131426 || namehash==626036062 || namehash==3551228837 || namehash==2784647472 || namehash==776142553 || namehash==3407058720 || namehash==202747424)
//		else if( strstr( sp->Name, "Curse of "))
//            type |= SPELL_TYPE_WARLOCK_CURSES;
		else if( strstr( sp->Name, "Immolate") || strstr( sp->Name, "Conflagrate") )
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

		/*FILE * f = fopen("C:\\spells.txt", "a");
		fprintf(f, "case 0x%08X:		// %s\n", namehash, sp->Name);
		fclose(f);*/

		// find diminishing status
		sp->DiminishStatus = GetDiminishingGroup( namehash );

		//another grouping rule

		//Quivers, Ammo Pouches and Thori'dal the Star's Fury
		if( ( namehash == SPELL_HASH_HASTE && sp->Attributes & 0x10000 ) || sp->Id == 44972 )
		{
			sp->Attributes &= ~ATTRIBUTES_PASSIVE;//Otherwise we couldn't remove them
			sp->BGR_one_buff_on_target |= SPELL_TYPE_QUIVER_HASTE;
		}

		switch(namehash)
		{
		//case SPELL_HASH_SANCTITY_AURA:
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

		case SPELL_HASH_FOCUS_MAGIC:				// Focus Magic buff
			sp->BGR_one_buff_from_caster_on_1target = SPELL_TYPE_INDEX_FOCUS_MAGIC;
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
		}

		// HACK FIX: Break roots/fear on damage.. this needs to be fixed properly!
		if(!(sp->AuraInterruptFlags & AURA_INTERRUPT_ON_ANY_DAMAGE_TAKEN))
		{
			for(uint32 z = 0; z < 3; ++z) {
				if(sp->EffectApplyAuraName[z] == SPELL_AURA_MOD_FEAR ||
					sp->EffectApplyAuraName[z] == SPELL_AURA_MOD_ROOT)
				{
					sp->AuraInterruptFlags |= AURA_INTERRUPT_ON_UNUSED2;
					break;
				}

				if( ( sp->Effect[z] == SPELL_EFFECT_SCHOOL_DAMAGE && sp->Spell_Dmg_Type == SPELL_DMG_TYPE_MELEE ) || sp->Effect[z] == SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL || sp->Effect[z] == SPELL_EFFECT_WEAPON_DAMAGE || sp->Effect[z] == SPELL_EFFECT_WEAPON_PERCENT_DAMAGE || sp->Effect[z] == SPELL_EFFECT_DUMMYMELEE )
					sp->is_melee_spell = true;
				if( ( sp->Effect[z] == SPELL_EFFECT_SCHOOL_DAMAGE && sp->Spell_Dmg_Type == SPELL_DMG_TYPE_RANGED ) )
				{
					//Log.Notice( "SpellFixes" , "Ranged Spell: %u [%s]" , sp->Id , sp->Name );
					sp->is_ranged_spell = true;
				}
			}
		}

		// set extra properties
		sp->RankNumber = rank;

		uint32 pr=sp->procFlags;
		for(uint32 y=0;y < 3; y++)
		{
			// get the effect number from the spell
			effect = sp->Effect[y];

			//spell group

			if(effect==SPELL_EFFECT_APPLY_AURA)
			{
				uint32 aura = sp->EffectApplyAuraName[y];
				if( aura == SPELL_AURA_PROC_TRIGGER_SPELL ||
					aura == SPELL_AURA_PROC_TRIGGER_DAMAGE
					)//search for spellid in description
				{
					const char *p=sp->Description;
					while((p=strstr(p,"$")) != 0)
					{
						p++;
						//got $  -> check if spell
						if(*p>='0' && *p <='9')
						{//woot this is spell id

							result=atoi(p);
						}
					}
					pr=0;

					uint32 len = (uint32)strlen(sp->Description);
					for(i = 0; i < len; ++i)
						sp->Description[i] = tolower(sp->Description[i]);
					//dirty code for procs, if any1 got any better idea-> u are welcome
					//139944 --- some magic number, it will trigger on all hits etc
						//for seems to be smth like custom check
					if( strstr( sp->Description,"your ranged criticals"))
						pr|=PROC_ON_RANGED_CRIT_ATTACK;
					if( strstr( sp->Description,"chance on hit"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( sp->Description,"takes damage"))
						pr|=PROC_ON_ANY_DAMAGE_VICTIM;
					if( strstr( sp->Description,"attackers when hit"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( sp->Description,"character strikes an enemy"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( sp->Description,"strike you with a melee attack"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( sp->Description,"target casts a spell"))
						pr|=PROC_ON_CAST_SPELL;
                    if( strstr( sp->Description,"your harmful spells land"))
                        pr|=PROC_ON_CAST_SPELL;
                    if( strstr( sp->Description,"on spell critical hit"))
                        pr|=PROC_ON_SPELL_CRIT_HIT;
                    if( strstr( sp->Description,"spell critical strikes"))
                        pr|=PROC_ON_SPELL_CRIT_HIT;
                    if( strstr( sp->Description,"being able to resurrect"))
                        pr|=PROC_ON_DIE;
					if( strstr( sp->Description,"any damage caused"))
						pr|=PROC_ON_ANY_DAMAGE_VICTIM;
					if( strstr( sp->Description,"the next melee attack against the caster"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( sp->Description,"when successfully hit"))
						pr|=PROC_ON_MELEE_ATTACK ;
					if( strstr( sp->Description,"an enemy on hit"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( sp->Description,"when it hits"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( sp->Description,"when successfully hit"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( sp->Description,"on a successful hit"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( sp->Description,"damage to attacker on hit"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( sp->Description,"on a hit"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( sp->Description,"strikes you with a melee attack"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( sp->Description,"when caster takes damage"))
						pr|=PROC_ON_ANY_DAMAGE_VICTIM;
					if( strstr( sp->Description,"when the caster is using melee attacks"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( sp->Description,"when struck in combat") || strstr(sp->Description,"When struck in combat"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( sp->Description,"successful melee attack"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( sp->Description,"chance per attack"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( sp->Description,"chance per hit"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( sp->Description,"that strikes a party member"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( sp->Description,"when hit by a melee attack"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( sp->Description,"landing a melee critical strike"))
						pr|=PROC_ON_CRIT_ATTACK;
					if( strstr( sp->Description,"your critical strikes"))
						pr|=PROC_ON_CRIT_ATTACK;
					if( strstr( sp->Description,"whenever you deal ranged damage"))
						pr|=PROC_ON_RANGED_ATTACK;
					if( strstr( sp->Description,"you deal melee damage"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( sp->Description,"your melee attacks"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( sp->Description,"damage with your Sword"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( sp->Description,"when struck in melee combat"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( sp->Description,"any successful spell cast against the priest"))
						pr|=PROC_ON_SPELL_HIT_VICTIM;
					if( strstr( sp->Description,"the next melee attack on the caster"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( sp->Description,"striking melee or ranged attackers"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM|PROC_ON_RANGED_ATTACK_VICTIM;
					if( strstr( sp->Description,"when damaging an enemy in melee"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( sp->Description,"victim of a critical strike"))
						pr|=PROC_ON_CRIT_HIT_VICTIM;
					if( strstr( sp->Description,"on successful melee or ranged attack"))
						pr|=PROC_ON_MELEE_ATTACK|PROC_ON_RANGED_ATTACK;
					if( strstr( sp->Description,"enemy that strikes you in melee"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( sp->Description,"after getting a critical strike"))
						pr|=PROC_ON_CRIT_ATTACK;
					if( strstr( sp->Description,"whenever damage is dealt to you"))
						pr|=PROC_ON_ANY_DAMAGE_VICTIM;
					if( strstr( sp->Description,"when ranged or melee damage is dealt"))
						pr|=PROC_ON_MELEE_ATTACK|PROC_ON_RANGED_ATTACK;
					if( strstr( sp->Description,"damaging melee attacks"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( sp->Description,"on melee or ranged attack"))
						pr|=PROC_ON_MELEE_ATTACK|PROC_ON_RANGED_ATTACK;
					if( strstr( sp->Description,"on a melee swing"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( sp->Description,"Chance on melee"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( sp->Description,"spell criticals against you"))
						pr|=PROC_ON_SPELL_CRIT_HIT_VICTIM;
					if( strstr( sp->Description,"after being struck by a melee or ranged critical hit"))
						pr|=PROC_ON_CRIT_HIT_VICTIM;
					if( strstr( sp->Description,"on a critical hit"))
					if( strstr( sp->Description,"critical hit"))
						pr|=PROC_ON_CRIT_ATTACK;
					if( strstr( sp->Description,"strikes the caster"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( sp->Description,"a spell, melee or ranged attack hits the caster"))
						pr|=PROC_ON_ANY_DAMAGE_VICTIM;
					if( strstr( sp->Description,"after dealing a critical strike"))
						pr|=PROC_ON_CRIT_ATTACK;
					if( strstr( sp->Description,"each melee or ranged damage hit against the priest"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM|PROC_ON_RANGED_ATTACK_VICTIM;
					if( strstr( sp->Description, "a chance to deal additional"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( sp->Description, "chance to get an extra attack"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( sp->Description, "melee attacks has"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( sp->Description, "any damage spell hits a target"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( sp->Description, "giving each melee attack a chance"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( sp->Description, "damage when hit"))
						pr|=PROC_ON_ANY_DAMAGE_VICTIM; //maybe melee damage ?
					if( strstr( sp->Description, "gives your"))
					{
						if( strstr( sp->Description, "melee"))
							pr|=PROC_ON_MELEE_ATTACK;
						else if( strstr( sp->Description,"sinister strike, backstab, gouge and shiv"))
							pr|=PROC_ON_CAST_SPELL;
						else if( strstr( sp->Description,"chance to daze the target"))
							pr|=PROC_ON_CAST_SPELL;
						else if( strstr( sp->Description,"finishing moves"))
							pr|=PROC_ON_CAST_SPELL;
						else pr|=PROC_ON_CAST_SPECIFIC_SPELL;
					}
					if( strstr( sp->Description, "chance to add an additional combo") && strstr(sp->Description, "critical") )
						pr|=PROC_ON_CRIT_ATTACK;
					else if( strstr( sp->Description, "chance to add an additional combo"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( sp->Description, "victim of a melee or ranged critical strike"))
						pr|=PROC_ON_CRIT_HIT_VICTIM;
					if( strstr( sp->Description, "getting a critical effect from"))
						pr|=PROC_ON_SPELL_CRIT_HIT_VICTIM;
					if( strstr( sp->Description, "damaging attack is taken"))
						pr|=PROC_ON_ANY_DAMAGE_VICTIM;
					if( strstr( sp->Description, "struck by a Stun or Immobilize"))
						pr|=PROC_ON_SPELL_HIT_VICTIM;
					if( strstr( sp->Description, "melee critical strike"))
						pr|=PROC_ON_CRIT_ATTACK;
					if( strstr( sp->Name, "Bloodthirst"))
						pr|=PROC_ON_MELEE_ATTACK | static_cast<uint32>(PROC_TARGET_SELF);
					if( strstr( sp->Description, "experience or honor"))
						pr|=PROC_ON_GAIN_EXPIERIENCE;
					if( strstr( sp->Description,"your next offensive ability"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( sp->Description,"hit by a melee or ranged attack"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM;
					if( strstr( sp->Description,"enemy strikes the caster"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( sp->Description,"melee and ranged attacks against you"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM;
					if( strstr( sp->Description,"when a block occurs"))
						pr|=PROC_ON_BLOCK_VICTIM;
					if( strstr( sp->Description,"dealing a critical strike from a weapon swing, spell, or ability"))
						pr|=PROC_ON_CRIT_ATTACK|PROC_ON_SPELL_CRIT_HIT;
					if( strstr( sp->Description,"dealing a critical strike from a weapon swing, spell, or ability"))
						pr|=PROC_ON_CRIT_ATTACK|PROC_ON_SPELL_CRIT_HIT;
					if( strstr( sp->Description,"shadow bolt critical strikes increase shadow damage"))
						pr|=PROC_ON_SPELL_CRIT_HIT;
					if( strstr( sp->Description,"next offensive ability"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( sp->Description,"after being hit with a shadow or fire spell"))
						pr|=PROC_ON_SPELL_LAND_VICTIM;
					if( strstr( sp->Description,"giving each melee attack"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( sp->Description,"each strike has"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( sp->Description,"your Fire damage spell hits"))
						pr|=PROC_ON_CAST_SPELL;		//this happens only on hit ;)
					if( strstr( sp->Description,"corruption, curse of agony, siphon life and seed of corruption spells also cause"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( sp->Description,"pain, mind flay and vampiric touch spells also cause"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( sp->Description,"shadow damage spells have"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( sp->Description,"on successful spellcast"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( sp->Description,"your spell criticals have"))
						pr|=PROC_ON_SPELL_CRIT_HIT | PROC_ON_SPELL_CRIT_HIT_VICTIM;
					if( strstr( sp->Description,"after dodging their attack"))
					{
						pr|=PROC_ON_DODGE_VICTIM;
						if( strstr( sp->Description,"add a combo point"))
							pr|=PROC_TARGET_SELF;
					}
					if( strstr( sp->Description,"fully resisting"))
						pr|=PROC_ON_RESIST_VICTIM;
					if( strstr( sp->Description,"Your Shadow Word: Pain, Mind Flay and Vampiric Touch spells also cause the target"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( sp->Description,"chance on spell hit"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( sp->Description,"your melee and ranged attacks"))
						pr|=PROC_ON_MELEE_ATTACK|PROC_ON_RANGED_ATTACK;
					//////////////////////////////////////////////////
					//proc dmg flags
					//////////////////////////////////////////////////
					if( strstr( sp->Description,"each attack blocked"))
						pr|=PROC_ON_BLOCK_VICTIM;
					if( strstr( sp->Description,"into flame, causing an additional"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( sp->Description,"victim of a critical melee strike"))
						pr|=PROC_ON_CRIT_HIT_VICTIM;
					if( strstr( sp->Description,"damage to melee attackers"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( sp->Description,"target blocks a melee attack"))
						pr|=PROC_ON_BLOCK_VICTIM;
					if( strstr( sp->Description,"ranged and melee attacks to deal"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM;
					if( strstr( sp->Description,"damage on hit"))
						pr|=PROC_ON_ANY_DAMAGE_VICTIM;
					if( strstr( sp->Description,"chance on hit"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( sp->Description,"after being hit by any damaging attack"))
						pr|=PROC_ON_ANY_DAMAGE_VICTIM;
					if( strstr( sp->Description,"striking melee or ranged attackers"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM;
					if( strstr( sp->Description,"damage to attackers when hit"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( sp->Description,"striking melee attackers"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( sp->Description,"whenever the caster takes damage"))
						pr|=PROC_ON_ANY_DAMAGE_VICTIM;
					if( strstr( sp->Description,"damage on every attack"))
						pr|=PROC_ON_MELEE_ATTACK | PROC_ON_RANGED_ATTACK;
					if( strstr( sp->Description,"chance to reflect Fire spells"))
						pr|=PROC_ON_SPELL_HIT_VICTIM;
					if( strstr( sp->Description,"hunter takes on the aspects of a hawk"))
						pr|=PROC_TARGET_SELF | PROC_ON_RANGED_ATTACK;
					if( strstr( sp->Description,"successful auto shot attacks"))
						pr|=PROC_ON_AUTO_SHOT_HIT;
					if( strstr( sp->Description,"after getting a critical effect from your"))
						pr=PROC_ON_SPELL_CRIT_HIT;
				}//end "if procspellaura"
			}//end "if aura"
		}//end "for each effect"
		sp->procFlags = pr;

		if( strstr( sp->Description, "Must remain seated"))
		{
			sp->RecoveryTime = 1000;
			sp->CategoryRecoveryTime = 1000;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// procintervals
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//omg lightning shield trigger spell id's are all wrong ?
		//if you are bored you could make these by hand but i guess we might find other spells with this problem..and this way it's safe
		if( strstr( sp->Name, "Lightning Shield" ) && sp->EffectTriggerSpell[0] )
		{
			//check if we can find in the description
			char *startofid = strstr(sp->Description, "for $");
			if( startofid )
			{
				startofid += strlen("for $");
				sp->EffectTriggerSpell[0] = atoi( startofid ); //get new lightning shield trigger id
			}
			sp->proc_interval = 3000; //few seconds
		}
		//mage ignite talent should proc only on some chances
		else if( strstr( sp->Name, "Ignite") && sp->Id>=11119 && sp->Id<=12848 && sp->EffectApplyAuraName[0] == 4 )
		{
			//check if we can find in the description
			char *startofid=strstr(sp->Description, "an additional ");
			if(startofid)
			{
				startofid += strlen("an additional ");
				sp->EffectBasePoints[0]=atoi(startofid); //get new value. This is actually level*8 ;)
			}
			sp->Effect[0] = 6; //aura
			sp->EffectApplyAuraName[0] = 42; //force him to use procspell effect
			sp->EffectTriggerSpell[0] = 12654; //evil , but this is good for us :D
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT; //add procflag here since this was not processed with the others !
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
#ifndef NEW_PROCFLAGS
		// Shadow Weaving
		else if( strstr( sp->Name, "Shadow Weaving"))
		{
			sp->School = 5;
			sp->EffectApplyAuraName[0] = 42;
			sp->procChance = sp->EffectBasePoints[0] + 1;
			sp->procFlags = PROC_ON_CAST_SPECIFIC_SPELL;
		}
#endif
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
				sp->procFlags = uint32(PROC_ON_MELEE_ATTACK_VICTIM);
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
				sp->procFlags = PROC_ON_MELEE_ATTACK;
				sp->procChance = 50;
			}
		}
		//some procs trigger at intervals
		else if( strstr( sp->Name, "Water Shield"))
		{
			sp->proc_interval = 3000; //few seconds
			sp->procFlags |= PROC_TARGET_SELF;
		}
		else if( strstr( sp->Name, "Earth Shield"))
			sp->proc_interval = 3000; //few seconds
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

		if( sp->NameHash == SPELL_HASH_ILLUMINATION )
			sp->procFlags |= PROC_TARGET_SELF;

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

		if( sp->proc_interval != 0 )
			sp->procFlags |= PROC_REMOVEONUSE;

		// Seal of Command - Proc Chance
		if( sp->NameHash == SPELL_HASH_SEAL_OF_COMMAND )
		{
			sp->procChance = 25;
			sp->School = SCHOOL_HOLY; //the procspells of the original seal of command have physical school instead of holy
			sp->Spell_Dmg_Type = SPELL_DMG_TYPE_MAGIC; //heh, crazy spell uses melee/ranged/magic dmg type for 1 spell. Now which one is correct ?
		}

		//Seal of Jusice - Proc Chance
		if( sp->NameHash == SPELL_HASH_SEAL_OF_JUSTICE )
			sp->procChance = 25;

		/* Decapitate */
		if( sp->NameHash == SPELL_HASH_DECAPITATE )
			sp->procChance = 30;

		//shaman - shock, has no spellgroup.very dangerous move !

		//mage - fireball. Only some of the spell has the flags

		if( sp->NameHash == SPELL_HASH_DIVINE_SHIELD || sp->NameHash == SPELL_HASH_DIVINE_PROTECTION || sp->NameHash == SPELL_HASH_BLESSING_OF_PROTECTION )
			sp->MechanicsType = MECHANIC_INVULNARABLE;

		/* hackfix for this - FIX ME LATER - Burlex */
		if( namehash == SPELL_HASH_SEAL_FATE )
			sp->procFlags = 0;

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
	}

	/////////////////////////////////////////////////////////////////
	//SPELL COEFFICIENT SETTINGS START
	//////////////////////////////////////////////////////////////////
	SpellEntry * sp;

	for(uint32 x=0; x < cnt; x++)
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

		sp->casttime_coef = castaff / 3500;

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

		//Calculating fixed coeficients
		//Channeled spells
		if( sp->ChannelInterruptFlags != 0 )
		{
			float Duration = float( GetDuration( dbcSpellDuration.LookupEntry( sp->DurationIndex ) ));
			if(Duration < 1500) Duration = 1500;
			else if(Duration > 7000) Duration = 7000;
			sp->fixed_hotdotcoef = (Duration / 3500.0f);

			if( sp->spell_coef_flags & SPELL_FLAG_ADITIONAL_EFFECT )
				sp->fixed_hotdotcoef *= 0.95f;
			if( sp->spell_coef_flags & SPELL_FLAG_AOE_SPELL )
				sp->fixed_hotdotcoef *= 0.5f;
		}

		//Standard spells
		else if( (sp->spell_coef_flags & SPELL_FLAG_IS_DD_OR_DH_SPELL) && !(sp->spell_coef_flags & SPELL_FLAG_IS_DOT_OR_HOT_SPELL) )
		{
			sp->fixed_dddhcoef = sp->casttime_coef;
			if( sp->spell_coef_flags & SPELL_FLAG_ADITIONAL_EFFECT )
				sp->fixed_dddhcoef *= 0.95f;
			if( sp->spell_coef_flags & SPELL_FLAG_AOE_SPELL )
				sp->fixed_dddhcoef *= 0.5f;
		}

		//Over-time spells
		else if( !(sp->spell_coef_flags & SPELL_FLAG_IS_DD_OR_DH_SPELL) && (sp->spell_coef_flags & SPELL_FLAG_IS_DOT_OR_HOT_SPELL) )
		{
			float Duration = float( GetDuration( dbcSpellDuration.LookupEntry( sp->DurationIndex ) ));
			sp->fixed_hotdotcoef = (Duration / 15000.0f);

			if( sp->spell_coef_flags & SPELL_FLAG_ADITIONAL_EFFECT )
				sp->fixed_hotdotcoef *= 0.95f;
			if( sp->spell_coef_flags & SPELL_FLAG_AOE_SPELL )
				sp->fixed_hotdotcoef *= 0.5f;

		}

		//Combined standard and over-time spells
		else if( sp->spell_coef_flags & SPELL_FLAG_IS_DD_DH_DOT_SPELL )
		{
			float Duration = float( GetDuration( dbcSpellDuration.LookupEntry( sp->DurationIndex ) ));
			float Portion_to_Over_Time = (Duration / 15000.0f) / ((Duration / 15000.0f) + sp->casttime_coef );
			float Portion_to_Standard = 1.0f - Portion_to_Over_Time;

			sp->fixed_dddhcoef = sp->casttime_coef * Portion_to_Standard;
			sp->fixed_hotdotcoef = (Duration / 15000.0f) * Portion_to_Over_Time;

			if( sp->spell_coef_flags & SPELL_FLAG_ADITIONAL_EFFECT )
			{
				sp->fixed_dddhcoef *= 0.95f;
				sp->fixed_hotdotcoef *= 0.95f;
			}
			if( sp->spell_coef_flags & SPELL_FLAG_AOE_SPELL )
			{
				sp->fixed_dddhcoef *= 0.5f;
				sp->fixed_hotdotcoef *= 0.5f;
			}
		}

		///	SPELLS CAN CRIT ///
		sp->spell_can_crit = true; // - except in special cases noted in this section


		//////////////////////////////////////////////////////
		// CLASS-SPECIFIC SPELL FIXES						//
		//////////////////////////////////////////////////////

		/* Note: when applying spell hackfixes, please follow a template */
		/* Please don't put fixes like "sp = dbcSpell.LookupEntryForced( 15270 );" inside the loop */

		//////////////////////////////////////////
		// WARRIOR								//
		//////////////////////////////////////////
		
        

		//////////////////////////////////////////
		// PALADIN								//
		//////////////////////////////////////////

		// Insert paladin spell fixes here

		// Seal of Righteousness - cannot crit
		if( sp->NameHash == SPELL_HASH_SEAL_OF_RIGHTEOUSNESS )
			sp->spell_can_crit = false;

		// Shield of Righteousness
		if( sp->NameHash == SPELL_HASH_SHIELD_OF_RIGHTEOUSNESS)
		{
			sp->School = SCHOOL_HOLY;
			sp->Effect[0] = SPELL_EFFECT_DUMMY;
			sp->Effect[1] = 0; //hacks, handling it in Spell::SpellEffectSchoolDMG(uint32 i)
			sp->Effect[2] = SPELL_EFFECT_SCHOOL_DAMAGE; //hack
		}

		//////////////////////////////////////////
		// HUNTER								//
		//////////////////////////////////////////

		// THESE FIXES ARE GROUPED FOR CODE CLEANLINESS.
		//Mend Pet
		if( sp->NameHash == SPELL_HASH_MEND_PET )
			sp->ChannelInterruptFlags = 0;

		//////////////////////////////////////////
		// ROGUE								//
		//////////////////////////////////////////

		// Insert rogue spell fixes here

		//////////////////////////////////////////
		// PRIEST								//
		//////////////////////////////////////////

		//megai2: Borrowed Time
		sp = dbcSpell.LookupEntryForced( 52795 ); //Rank 1
        if( sp != NULL )
        {
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 52797 ); //Rank 2
        if( sp != NULL )
        {
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 52798 ); //Rank 3
        if( sp != NULL )
        {
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 52799 ); //Rank 4
        if( sp != NULL )
        {
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 52800 ); //Rank 5
        if( sp != NULL )
        {
			sp->procFlags = PROC_ON_CAST_SPELL;
		}

		//megai2: Grace http://www.wowhead.com/?spell=47516
		sp = dbcSpell.LookupEntryForced( 47516 ); //Rank 1
        if( sp != NULL )
        {
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 47517 ); //Rank 2
        if( sp != NULL )
        {
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 47930 );
		if( sp != NULL )
		{
			sp->rangeIndex = 4;
		}

		//////////////////////////////////////////
		// SHAMAN								//
		//////////////////////////////////////////

		// Insert shaman spell fixes here
	
		// Flametongue Totem passive target fix
		if( sp->NameHash == SPELL_HASH_FLAMETONGUE_TOTEM && sp->Attributes & ATTRIBUTES_PASSIVE )
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SELF;
			sp->EffectImplicitTargetB[0] = 0;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_SELF;
			sp->EffectImplicitTargetB[1] = 0;
		}

		// Lightning Shield - cannot crit
		if( sp->NameHash == SPELL_HASH_LIGHTNING_SHIELD ) // not a mistake, the correct proc spell for lightning shield is also called lightning shield
			sp->spell_can_crit = false;

		// Frostbrand Weapon - 10% spd coefficient
		if( sp->NameHash == SPELL_HASH_FROSTBRAND_ATTACK )
			sp->fixed_dddhcoef = 0.1f;

		// Fire Nova - 0% spd coefficient
		if( sp->NameHash == SPELL_HASH_FIRE_NOVA )
			sp->fixed_dddhcoef = 0.0f;

		// Searing Totem - 8% spd coefficient
		if( sp->NameHash == SPELL_HASH_ATTACK )
			sp->fixed_dddhcoef = 0.08f;

		// Healing Stream Totem - 8% healing coefficient
		if( sp->NameHash == SPELL_HASH_HEALING_STREAM )
			sp->OTspell_coef_override = 0.08f;

		// Nature's Guardian
		if( sp->NameHash == SPELL_HASH_NATURE_S_GUARDIAN )
		{
			sp->procFlags = PROC_ON_SPELL_HIT_VICTIM | PROC_ON_MELEE_ATTACK_VICTIM |
				PROC_ON_RANGED_ATTACK_VICTIM | PROC_ON_ANY_DAMAGE_VICTIM;
			sp->proc_interval = 5000;
			sp->EffectTriggerSpell[0] = 31616;
		}

		if( sp->NameHash == SPELL_HASH_HEX )
		{
			sp->AuraInterruptFlags |= AURA_INTERRUPT_ON_UNUSED2;
		}

		//////////////////////////////////////////
		// MAGE									//
		//////////////////////////////////////////

		//////////////////////////////////////////
		// WARLOCK								//
		//////////////////////////////////////////

		//////////////////////////////////////////
		// DRUID								//
		//////////////////////////////////////////
		
		// Dash
		if( sp->NameHash == SPELL_HASH_DASH )
		{
			// mask for FORM_CAT(1) = 1 << (1 - 1), which is 1
			sp->RequiredShapeShift = 1;
		}

		//Bash - Interrupt effect
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
				sp->Dspell_coef_override = f[2].GetFloat();
				sp->OTspell_coef_override = f[3].GetFloat();
			}
			else
				Log.Warning("SpellCoefOverride", "Has nonexistent spell %u.", f[0].GetUInt32());
		} while( resultx->NextRow() );
		delete resultx;
	}

	//Fully loaded coefficients, we must share channeled coefficient to its triggered spells
	for(uint32 x=0; x < cnt; x++)
	{
		// get spellentry
		SpellEntry * sp = dbcSpell.LookupRow(x);
		SpellEntry * spz;

		//Case SPELL_AURA_PERIODIC_TRIGGER_SPELL
		for( uint8 i = 0 ; i < 3 ; i++ )
		{
			if ( sp->EffectApplyAuraName[i] == SPELL_AURA_PERIODIC_TRIGGER_SPELL )
			{
				spz = dbcSpell.LookupEntry( sp->EffectTriggerSpell[i] );
				if( spz != NULL )
				{
					if( sp->Dspell_coef_override >= 0 )
						spz->Dspell_coef_override = sp->Dspell_coef_override;
					else
					{
						//we must set bonus per tick on triggered spells now (i.e. Arcane Missiles)
						if( sp->ChannelInterruptFlags != 0 )
						{
							float Duration = float( GetDuration( dbcSpellDuration.LookupEntry( sp->DurationIndex ) ));
							float amp = float(sp->EffectAmplitude[i]);
							sp->fixed_dddhcoef = sp->fixed_hotdotcoef * amp / Duration;
						}
						spz->fixed_dddhcoef = sp->fixed_dddhcoef;
					}

					if( sp->OTspell_coef_override >= 0 )
						spz->OTspell_coef_override = sp->OTspell_coef_override;
					else
					{
						//we must set bonus per tick on triggered spells now (i.e. Arcane Missiles)
						if( sp->ChannelInterruptFlags != 0 )
						{
							float Duration = float( GetDuration( dbcSpellDuration.LookupEntry( sp->DurationIndex ) ));
							float amp = float(sp->EffectAmplitude[i]);
							sp->fixed_hotdotcoef *= amp / Duration;
						}
						spz->fixed_hotdotcoef = sp->fixed_hotdotcoef;
					}
					break;
				}
			}
		}
	}

	/////////////////////////////////////////////////////////////////
	//SPELL COEFFICIENT SETTINGS END
	/////////////////////////////////////////////////////////////////

	EnchantEntry* Enchantment;

	// Flametongue weapon
	Enchantment = dbcEnchant.LookupEntryForced( 2634 );
	if( Enchantment != NULL )
	{
		Enchantment->type[0] = 1;
		Enchantment->spell[0] = 25488;
	}
	Enchantment = dbcEnchant.LookupEntryForced( 1666 );
	if( Enchantment != NULL )
	{
		Enchantment->type[0] = 1;
		Enchantment->spell[0] = 16344;
	}
	Enchantment = dbcEnchant.LookupEntryForced( 1665 );
	if( Enchantment != NULL )
	{
		Enchantment->type[0] = 1;
		Enchantment->spell[0] = 16343;
	}
	Enchantment = dbcEnchant.LookupEntryForced( 523 );
	if( Enchantment != NULL )
	{
		Enchantment->type[0] = 1;
		Enchantment->spell[0] = 10445;
	}
	Enchantment = dbcEnchant.LookupEntryForced( 3 );
	if( Enchantment != NULL )
	{
		Enchantment->type[0] = 1;
		Enchantment->spell[0] = 8029;
	}
	Enchantment = dbcEnchant.LookupEntryForced( 4 );
	if( Enchantment != NULL )
	{
		Enchantment->type[0] = 1;
		Enchantment->spell[0] = 8028;
	}
	Enchantment = dbcEnchant.LookupEntryForced( 5 );
	if( Enchantment != NULL )
	{
		Enchantment->type[0] = 1;
		Enchantment->spell[0] = 8026;
	}

	/********************************************************
	 * Windfury Enchantment
	 ********************************************************/

	Enchantment = dbcEnchant.LookupEntryForced( 283 );
	if( Enchantment != NULL )
	{
		Enchantment->spell[0] = 33757; //this is actually good
		sp = dbcSpell.LookupEntryForced( 33757 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags = PROC_ON_MELEE_ATTACK; //we do not need proc on spell ;)
			sp->EffectTriggerSpell[0] = 8232; //for the logs and rest
			sp->procChance = 20;
			sp->proc_interval = 3000;//http://www.wowwiki.com/Windfury_Weapon
			sp->maxstack = 1;
		}
	}

	Enchantment = dbcEnchant.LookupEntryForced( 284 );
	if( Enchantment != NULL )
	{
		Enchantment->spell[0] = 33756;
		sp = dbcSpell.LookupEntryForced( 33756 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags = PROC_ON_MELEE_ATTACK; //we do not need proc on spell ;)
			sp->EffectTriggerSpell[0] = 8235; //for the logs and rest
			sp->procChance = 20;
			sp->proc_interval = 3000; //http://www.wowwiki.com/Windfury_Weapon
			sp->maxstack = 1;
		}
	}

	Enchantment = dbcEnchant.LookupEntryForced( 525 );
	if( Enchantment != NULL )
	{
		Enchantment->spell[0] = 33755;
		sp = dbcSpell.LookupEntryForced( 33755 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags = PROC_ON_MELEE_ATTACK; //we do not need proc on spell ;)
			sp->EffectTriggerSpell[0] = 10486; //for the logs and rest
			sp->procChance = 20;
			sp->proc_interval = 3000;//http://www.wowwiki.com/Windfury_Weapon
			sp->maxstack = 1;
		}
	}

	Enchantment = dbcEnchant.LookupEntryForced( 1669 );
	if( Enchantment != NULL )
	{
		Enchantment->spell[0] = 33754;
		sp = dbcSpell.LookupEntryForced( 33754 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags = PROC_ON_MELEE_ATTACK; //we do not need proc on spell ;)
			sp->EffectTriggerSpell[0] = 16362; //for the logs and rest
			sp->procChance = 20;
			sp->proc_interval = 3000;//http://www.wowwiki.com/Windfury_Weapon
			sp->maxstack = 1;
		}
	}

	Enchantment = dbcEnchant.LookupEntryForced( 2636 );
	if( Enchantment != NULL )
	{
		Enchantment->spell[0] = 33727;
		sp = dbcSpell.LookupEntryForced( 33727 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags = PROC_ON_MELEE_ATTACK; //we do not need proc on spell ;)
			sp->EffectTriggerSpell[0] = 25505; //for the logs and rest
			sp->procChance = 20;
			sp->proc_interval = 3000;//http://www.wowwiki.com/Windfury_Weapon
			sp->maxstack = 1;
		}
	}

	/**********************************************************
	 *	PROFFESION - Enchant Cloak - Major Resistance
	 **********************************************************/
	sp = dbcSpell.LookupEntryForced( 27962 );
	if( sp != NULL )
		sp->EffectMiscValue[0] = 2998;
	sp = dbcSpell.LookupEntryForced( 36285 );
	if( sp != NULL )
		sp->EffectMiscValue[0] = 2998;

	/**********************************************************
	 * Berserking - TROLL'S RACIAL SPELL
	 **********************************************************/
	sp = dbcSpell.LookupEntryForced( 20554 );
	if( sp != NULL )
	{
		sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
		sp->EffectTriggerSpell[0] = 26635;
	}
	sp = dbcSpell.LookupEntryForced( 26296 );
	if( sp != NULL )
	{
		sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
		sp->EffectTriggerSpell[0] = 26635;
	}
	sp = dbcSpell.LookupEntryForced( 26297 );
	if( sp != NULL )
	{
		sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
		sp->EffectTriggerSpell[0] = 26635;
	}

	sp = dbcSpell.LookupEntryForced( 20134 ); // << --- WTF?
	if( sp != NULL )
		sp->procChance = 50;

	/**********************************************************
	* Mana Tap - BLOOD ELF RACIAL
	**********************************************************/
	sp = dbcSpell.LookupEntryForced( 28734 );
	if( sp != NULL )
//		sp->Effect[0] = SPELL_EFFECT_POWER_BURN; // should be Power Burn, not Power Drain. Power Drain leeches mana which is incorrect.
		//Zazk : tested on retail and it is only leech and not burn !
		sp->Effect[0] = SPELL_EFFECT_POWER_DRAIN; // should be Power Burn, not Power Drain. Power Drain leeches mana which is incorrect.

	/**********************************************************
	* Gift of the Naaru - Draenei RACIAL
	**********************************************************/
	sp = dbcSpell.LookupEntryForced( 28880 );
	if( sp != NULL )
		sp->Effect[1] = SPELL_EFFECT_DUMMY;

	/**********************************************************
	 * thrown - add a 1.6 second cooldown
	 **********************************************************/
	const static uint32 thrown_spells[] = {SPELL_RANGED_GENERAL,SPELL_RANGED_THROW,SPELL_RANGED_WAND, 26679, 27084, 29436, 37074, 41182, 41346, 0};
	for(i = 0; thrown_spells[i] != 0; ++i)
	{
		sp = dbcSpell.LookupEntryForced( thrown_spells[i] );
		if( sp != NULL && sp->RecoveryTime==0 && sp->StartRecoveryTime == 0 ){
			if ( sp->Id == SPELL_RANGED_GENERAL ) sp->RecoveryTime = 500; // cebernic: hunter general with 0.5s
			else sp->RecoveryTime = 1500; // 1.5cd
		}
	}

	/**********************************************************
	 * Wands
	 **********************************************************/
	sp = dbcSpell.LookupEntryForced( SPELL_RANGED_WAND );
	if( sp != NULL )
		sp->Spell_Dmg_Type = SPELL_DMG_TYPE_RANGED;

	/**********************************************************
	 * Misc stuff (questfixes etc)
	 **********************************************************/

	sp = dbcSpell.LookupEntryForced( 30877 );
	if( sp != NULL )
	{
		sp->EffectImplicitTargetB[0]=0;
	}

	sp = dbcSpell.LookupEntryForced( 23179 );
	if( sp != NULL )
		sp->EffectMiscValue[0] = 1434;

	// list of guardians that should inherit casters level
	//fire elemental
	sp = dbcSpell.LookupEntryForced( 32982 );
	if( sp != NULL )
		sp->c_is_flags |= SPELL_FLAG_IS_INHERITING_LEVEL;

	//Earth elemental
	sp = dbcSpell.LookupEntryForced( 33663 );
	if( sp != NULL )
		sp->c_is_flags |= SPELL_FLAG_IS_INHERITING_LEVEL;

	//////////////////////////////////////////////////////
	// CLASS-SPECIFIC SPELL FIXES						//
	//////////////////////////////////////////////////////

	/* Note: when applying spell hackfixes, please follow a template */

	//////////////////////////////////////////
	// WARRIOR								//
	//////////////////////////////////////////

	// Insert warrior spell fixes here

		/**********************************************************
		 *	Arms
		 **********************************************************/

		// Trauma Rank 1
		sp = dbcSpell.LookupEntryForced( 46854 );
		if(sp != NULL)
		{
			//sp->procChance = 100;
			sp->procFlags = PROC_ON_CRIT_ATTACK;
		}

		// Trauma Rank 2
		sp = dbcSpell.LookupEntryForced( 46855 );
		if(sp != NULL)
		{
			//sp->procChance = 100;
			sp->procFlags = PROC_ON_CRIT_ATTACK;
		}

		// Taste for Blood Rank 1
		sp = dbcSpell.LookupEntryForced( 56636 );
		if(sp != NULL)
		{
			//sp->procChance = 33;
			sp->procFlags = PROC_ON_CAST_SPELL | PROC_ON_ANY_HOSTILE_ACTION;
			sp->proc_interval = 6000;
		}

		// Taste for Blood Rank 2
		sp = dbcSpell.LookupEntryForced( 56637 );
		if(sp != NULL)
		{
			//sp->procChance = 66;
			sp->procFlags = PROC_ON_CAST_SPELL | PROC_ON_ANY_HOSTILE_ACTION;
			sp->proc_interval = 6000;
		}

		// Taste for Blood Rank 3
		sp = dbcSpell.LookupEntryForced( 56638 );
		if(sp != NULL)
		{
			//sp->procChance = 100;
			sp->procFlags = PROC_ON_CAST_SPELL | PROC_ON_ANY_HOSTILE_ACTION;
			sp->proc_interval = 6000;
		}

		// Wrecking Crew Rank 1
		sp = dbcSpell.LookupEntryForced( 46867 );
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[1] = 57518;
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->procChance = 100;
		}

		// Wrecking Crew Rank 2
		sp = dbcSpell.LookupEntryForced( 56611 );
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[1] = 57519;
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->procChance = 100;
		}

		// Wrecking Crew Rank 3
		sp = dbcSpell.LookupEntryForced( 56612 );
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[1] = 57520;
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->procChance = 100;
		}

		// Wrecking Crew Rank 4
		sp = dbcSpell.LookupEntryForced( 56613 );
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[1] = 57521;
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->procChance = 100;
		}

		// Wrecking Crew Rank 5
		sp = dbcSpell.LookupEntryForced( 56614 );
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[1] = 57522;
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->procChance = 100;
		}

		// Warrior - Deep Wounds
		sp = dbcSpell.LookupEntryForced( 12834 );
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[0] = 12721;
			sp->procFlags = PROC_ON_CRIT_ATTACK | PROC_ON_SPELL_CRIT_HIT;
		}
		sp = dbcSpell.LookupEntryForced( 12849 );
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[0] = 12721;
			sp->procFlags = PROC_ON_CRIT_ATTACK | PROC_ON_SPELL_CRIT_HIT;
		}
		sp = dbcSpell.LookupEntryForced( 12867 );
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[0] = 12721;
			sp->procFlags = PROC_ON_CRIT_ATTACK | PROC_ON_SPELL_CRIT_HIT;
		}

		// Warrior - Charge Rank 1
		sp = dbcSpell.LookupEntryForced( 100 );
		if(sp != NULL)
		{
			sp->Effect[1] = SPELL_EFFECT_DUMMY;
			sp->EffectMiscValue[1] = 90;
		}
		// Warrior - Charge Rank 2
		sp = dbcSpell.LookupEntryForced( 6178 );
		if(sp != NULL)
		{
			sp->Effect[1] = SPELL_EFFECT_DUMMY;
			sp->EffectMiscValue[1] = 120;
		}
		// Warrior - Charge Rank 3
		sp = dbcSpell.LookupEntryForced( 11578 );
		if(sp != NULL)
		{
			sp->Effect[1] = SPELL_EFFECT_DUMMY;
			sp->EffectMiscValue[1] = 150;
		}

		// Warrior - Improved Hamstring Rank 1
		sp = dbcSpell.LookupEntryForced( 12289 );
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[1] = 23694;
			sp->procFlags = PROC_ON_CAST_SPECIFIC_SPELL;
			sp->procChance = 5;
		}
		// Warrior - Improved Hamstring Rank 2
		sp = dbcSpell.LookupEntryForced( 12668 );
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[1] = 23694;
			sp->procFlags = PROC_ON_CAST_SPECIFIC_SPELL;
			sp->procChance = 10;
		}
		// Warrior - Improved Hamstring Rank 3
		sp = dbcSpell.LookupEntryForced( 23695 );
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[1] = 23694;
			sp->procFlags = PROC_ON_CAST_SPECIFIC_SPELL;
			sp->procChance = 15;
		}

		// Warrior - Retaliation
		sp = dbcSpell.LookupEntryForced( 20230 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6; //aura
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 22858; //evil , but this is good for us :D
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM; //add procflag here since this was not processed with the others !
		}

		// Warrior - Second Wind should trigger on self
		sp = dbcSpell.LookupEntryForced( 29841 );
		if( sp != NULL )
			sp->procFlags |= PROC_TARGET_SELF;

		sp = dbcSpell.LookupEntryForced( 29842 );
		if( sp != NULL )
			sp->procFlags |= PROC_TARGET_SELF;

		// Warrior - Sudden Death Rank 1
		sp = dbcSpell.LookupEntryForced( 29723 );
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[0] = 52437;
			sp->procFlags = PROC_ON_MELEE_ATTACK;
			sp->procChance = 3;
		}		

		// Warrior - Sudden Death Rank 2
		sp = dbcSpell.LookupEntryForced( 29725 );
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[0] = 52437;
			sp->procFlags = PROC_ON_MELEE_ATTACK;
			sp->procChance = 6;
		}	

		// Warrior - Sudden Death Rank 3
		sp = dbcSpell.LookupEntryForced( 29724 );
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[0] = 52437;
			sp->procFlags = PROC_ON_MELEE_ATTACK;
			sp->procChance = 9;
		}

		// Warrior - Overpower Rank 1
		sp = dbcSpell.LookupEntryForced( 7384 );
		if(sp != NULL)
			sp->Attributes |= ATTRIBUTES_CANT_BE_DPB;
		// Warrior - Overpower Rank 2
		sp = dbcSpell.LookupEntryForced( 7887 );
		if(sp != NULL)
			sp->Attributes |= ATTRIBUTES_CANT_BE_DPB;
		// Warrior - Overpower Rank 3
		sp = dbcSpell.LookupEntryForced( 11584 );
		if(sp != NULL)
			sp->Attributes |= ATTRIBUTES_CANT_BE_DPB;
		// Warrior - Overpower Rank 4
		sp = dbcSpell.LookupEntryForced( 11585 );
		if(sp != NULL)
			sp->Attributes |= ATTRIBUTES_CANT_BE_DPB;

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

		// Warrior - Heroic Throw
		sp = dbcSpell.LookupEntryForced( 57755 );
		if( sp != NULL ){
			sp->Effect[0] = SPELL_EFFECT_SCHOOL_DAMAGE;
		}

		// Warrior - Blood Frenzy
		sp = dbcSpell.LookupEntryForced( 29836 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 29859 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
		}

		// Warrior - Rend
		sp = dbcSpell.LookupEntryForced( 772 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 6546 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 6547 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 6548 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 11572 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 11573 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 11574 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 25208 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;

		/**********************************************************
		 *	Fury
		 **********************************************************/

		// Warrior - Slam		
		sp = dbcSpell.LookupEntryForced( 1464 );
		if (sp != NULL)
			sp->Effect[0] = SPELL_EFFECT_SCHOOL_DAMAGE;

		sp = dbcSpell.LookupEntryForced( 8820 );
		if (sp != NULL)
			sp->Effect[0] = SPELL_EFFECT_SCHOOL_DAMAGE;

		sp = dbcSpell.LookupEntryForced( 11604 );
		if (sp != NULL)
			sp->Effect[0] = SPELL_EFFECT_SCHOOL_DAMAGE;

		sp = dbcSpell.LookupEntryForced( 11605 );
		if (sp != NULL)
			sp->Effect[0] = SPELL_EFFECT_SCHOOL_DAMAGE;

		sp = dbcSpell.LookupEntryForced( 25241 );
		if (sp != NULL)
			sp->Effect[0] = SPELL_EFFECT_SCHOOL_DAMAGE;

		sp = dbcSpell.LookupEntryForced( 25242 );
		if (sp != NULL)
			sp->Effect[0] = SPELL_EFFECT_SCHOOL_DAMAGE;

		sp = dbcSpell.LookupEntryForced( 47474 );
		if (sp != NULL)
			sp->Effect[0] = SPELL_EFFECT_SCHOOL_DAMAGE;

		sp = dbcSpell.LookupEntryForced( 47475 );
		if (sp != NULL)
			sp->Effect[0] = SPELL_EFFECT_SCHOOL_DAMAGE;

		// Warrior - Bloodsurge
		sp = dbcSpell.LookupEntryForced( 46913 );
		if (sp != NULL)
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_ON_CAST_SPELL;
			sp->procChance = 7;
		}
		sp = dbcSpell.LookupEntryForced( 46914 );
		if (sp != NULL)
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_ON_CAST_SPELL;
			sp->procChance = 13;
		}
		sp = dbcSpell.LookupEntryForced( 46915 );
		if (sp != NULL)
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_ON_CAST_SPELL;
			sp->procChance = 20;
		}

		// Warrior - Furious Attacks
		sp = dbcSpell.LookupEntryForced( 46910 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK;
			sp->proc_interval = 7000;
		}
		sp = dbcSpell.LookupEntryForced( 46911 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK;
			sp->proc_interval = 5000;
		}

		// Warrior - Enrage Procflags
        sp = dbcSpell.LookupEntryForced( 12317 ); 
        if(sp != NULL) 
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM | PROC_ON_SPELL_HIT_VICTIM;
		sp = dbcSpell.LookupEntryForced( 13045 );
		if(sp != NULL)
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM | PROC_ON_SPELL_HIT_VICTIM;
 		sp = dbcSpell.LookupEntryForced( 13046 );
 		if(sp != NULL)
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM | PROC_ON_SPELL_HIT_VICTIM;
 		sp = dbcSpell.LookupEntryForced( 13047 );
		if(sp != NULL)
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM | PROC_ON_SPELL_HIT_VICTIM;
 		sp = dbcSpell.LookupEntryForced( 13048 );
 		if(sp != NULL)
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM | PROC_ON_SPELL_HIT_VICTIM;

 		/* Remove the charges only on melee attacks */
		sp = dbcSpell.LookupEntryForced( 12880 );
		if(sp != NULL)
			sp->procFlags = PROC_ON_MELEE_ATTACK;
		sp = dbcSpell.LookupEntryForced( 14201 );
		if(sp != NULL)
			sp->procFlags = PROC_ON_MELEE_ATTACK;
		sp = dbcSpell.LookupEntryForced( 14202 );
		if(sp != NULL)
			sp->procFlags = PROC_ON_MELEE_ATTACK;
		sp = dbcSpell.LookupEntryForced( 14203 );
		if(sp != NULL)
			sp->procFlags = PROC_ON_MELEE_ATTACK;
		sp = dbcSpell.LookupEntryForced( 14204 );
		if(sp != NULL)
			sp->procFlags = PROC_ON_MELEE_ATTACK;

		// Warrior - Blood Craze Procflags
		sp = dbcSpell.LookupEntryForced( 16487 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM | PROC_ON_RANGED_CRIT_ATTACK_VICTIM | PROC_ON_SPELL_CRIT_HIT_VICTIM;
		sp = dbcSpell.LookupEntryForced( 16489 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM | PROC_ON_RANGED_CRIT_ATTACK_VICTIM | PROC_ON_SPELL_CRIT_HIT_VICTIM;
		sp = dbcSpell.LookupEntryForced( 16492 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM | PROC_ON_RANGED_CRIT_ATTACK_VICTIM | PROC_ON_SPELL_CRIT_HIT_VICTIM;

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

		// Warrior - Rampage
		sp = dbcSpell.LookupEntryForced( 29801 );
		if( sp != NULL )
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags = PROC_ON_CRIT_ATTACK | static_cast<uint32>(PROC_TARGET_SELF);
			sp->EffectTriggerSpell[0] = 30029;
		}
		
		// Warrior - Berserker Rage
		sp = dbcSpell.LookupEntryForced( 18499 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;//Forcing a dummy aura, so we can add the missing 4th effect.
			sp->Effect[1] = 0;
			sp->Effect[2] = 0;
		}

		// Warrior - Improved Berserker Rage
		sp = dbcSpell.LookupEntryForced( 20500 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL | static_cast<uint32>(PROC_TARGET_SELF);
		sp = dbcSpell.LookupEntryForced( 20501 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL | static_cast<uint32>(PROC_TARGET_SELF);

		// Warrior - Unbridled Wrath
		sp = dbcSpell.LookupEntryForced( 12322 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_MELEE_ATTACK | static_cast<uint32>(PROC_TARGET_SELF);
		sp = dbcSpell.LookupEntryForced( 12999 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_MELEE_ATTACK | static_cast<uint32>(PROC_TARGET_SELF);
		sp = dbcSpell.LookupEntryForced( 13000 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_MELEE_ATTACK | static_cast<uint32>(PROC_TARGET_SELF);
		sp = dbcSpell.LookupEntryForced( 13001 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_MELEE_ATTACK | static_cast<uint32>(PROC_TARGET_SELF);
		sp = dbcSpell.LookupEntryForced( 13002 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_MELEE_ATTACK | static_cast<uint32>(PROC_TARGET_SELF);

		// Warrior - Heroic Fury
		sp = dbcSpell.LookupEntryForced( 60970 );
		if( sp != NULL ){
			sp->Effect[0] = SPELL_EFFECT_DUMMY;
		}

		/**********************************************************
		 *	Protection
		 **********************************************************/

		// Sword and Board Rank 1
		sp = dbcSpell.LookupEntryForced( 46951 );
		if(sp != NULL)
			sp->procFlags = PROC_ON_CAST_SPELL;

		// Sword and Board Rank 2
		sp = dbcSpell.LookupEntryForced( 46952 );
		if(sp != NULL)
			sp->procFlags = PROC_ON_CAST_SPELL;

		// Sword and Board Rank 3
		sp = dbcSpell.LookupEntryForced( 46953 );
		if(sp != NULL)
			sp->procFlags = PROC_ON_CAST_SPELL;

		// Safeguard Rank 1
		sp = dbcSpell.LookupEntryForced( 46945 );
		if(sp != NULL)
			sp->procFlags = PROC_ON_CAST_SPELL;

		// Safeguard Rank 2
		sp = dbcSpell.LookupEntryForced( 46949 );
		if(sp != NULL)
			sp->procFlags = PROC_ON_CAST_SPELL;

		// Improved Defensive Stance Rank 1
		sp = dbcSpell.LookupEntryForced( 29593 );
		if(sp != NULL)
			sp->procFlags = PROC_ON_DODGE_VICTIM | PROC_ON_BLOCK_VICTIM;

		// Improved Defensive Stance Rank 2
		sp = dbcSpell.LookupEntryForced( 29594 );
		if(sp != NULL)
			sp->procFlags = PROC_ON_DODGE_VICTIM | PROC_ON_BLOCK_VICTIM;

		// Damage Shield Rank 1
		sp = dbcSpell.LookupEntryForced( 58872 );
		if(sp != NULL)
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 59652;
			sp->procChance = 100;
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM | PROC_ON_BLOCK_VICTIM;
		}

		// Damage Shield Rank 2
		sp = dbcSpell.LookupEntryForced( 58874 );
		if(sp != NULL)
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 59653;
			sp->procChance = 100;
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM | PROC_ON_BLOCK_VICTIM;
		}

		// Improved Revenge Rank 1
		sp = dbcSpell.LookupEntryForced( 12797 );
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[1] = 12798;
			sp->procFlags = PROC_ON_CAST_SPECIFIC_SPELL;
			sp->procChance = 25;
		}

		// Improved Revenge Rank 2
		sp = dbcSpell.LookupEntryForced( 12799 );
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[1] = 12798;
			sp->procFlags = PROC_ON_CAST_SPECIFIC_SPELL;
			sp->procChance = 50;
		}

		// Intervene  Ranger: stop attack
		sp = dbcSpell.LookupEntryForced( 3411 );
		if( sp != NULL )
		{
			sp->Attributes |= ATTRIBUTES_STOP_ATTACK;
		}

		// Gag Order Rank 1
		sp = dbcSpell.LookupEntryForced( 12311 );
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[1] = 18498;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 50;
		}

		// Gag Order Rank 2
		sp = dbcSpell.LookupEntryForced( 12958 );
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[1] = 18498;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 100;
		}

		// Shockwave Damage - useless?
		sp = dbcSpell.LookupEntryForced( 46968 );
		if(sp!=NULL){
			sp->Effect[1] = SPELL_EFFECT_SCHOOL_DAMAGE;
			sp->Effect[2] = SPELL_EFFECT_DUMMY;
		}
	
		//////////////////////////////////////////
		// DRUID								//
		//////////////////////////////////////////

		//Force of Nature
		sp = dbcSpell.LookupEntryForced(33831);
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_INHERITING_LEVEL;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SELF; //some land under target is used that gathers multiple targets ...
			sp->EffectImplicitTargetA[1] = EFF_TARGET_NONE;
		}

		//Nature's Grace Rank 1
		sp = dbcSpell.LookupEntryForced(16880);
		if(sp != NULL)
		{
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
			sp->procChance = 33;
			sp->maxstack = 1;
		}

		//Nature's Grace Rank 2
		sp = dbcSpell.LookupEntryForced(61345);
		if(sp != NULL)
		{
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
			sp->procChance = 66;
			sp->maxstack = 1;
		}

		//Nature's Grace Rank 3
		sp = dbcSpell.LookupEntryForced(61346);
		if(sp != NULL)
		{
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
			sp->procChance = 100;
			sp->maxstack = 1;
		}

		//Earth and Moon Rank 1
		sp = dbcSpell.LookupEntryForced(48506);
		if(sp != NULL)
			sp->procFlags = PROC_ON_CAST_SPELL;

		//Earth and Moon Rank 2
		sp = dbcSpell.LookupEntryForced(48510);
		if(sp != NULL)
			sp->procFlags = PROC_ON_CAST_SPELL;

		//Earth and Moon Rank 3
		sp = dbcSpell.LookupEntryForced(48511);
		if(sp != NULL)
			sp->procFlags = PROC_ON_CAST_SPELL;

		//////////////////////////////////////////
		// PALADIN								//
		//////////////////////////////////////////

		// Insert paladin spell fixes here

		//Paladin - Judgements - rebuff
		sp = dbcSpell.LookupEntryForced( 20184 );
		if( sp != NULL )
			sp->RankNumber = 0;

		//Paladin - Judgement of Light
		sp = dbcSpell.LookupEntryForced( 20185 );
        if( sp != NULL )
        {
            sp->Effect[0] = 6;
			sp->EffectApplyAuraName[0] = 42;
			sp->EffectTriggerSpell[0] = 20267;
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;
			sp->RankNumber = 0;
			sp->procChance = 30;
        }
		sp = dbcSpell.LookupEntryForced( 20267 );
		if( sp != NULL )
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SINGLE_ENEMY;

		//Paladin - Judgement of Wisdom
		sp = dbcSpell.LookupEntryForced( 20186 );
        if( sp != NULL )
        {
            sp->Effect[0] = 6;
			sp->EffectApplyAuraName[0] = 42;
			sp->EffectTriggerSpell[0] = 20268;
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;
			sp->RankNumber = 0;
			sp->proc_interval = 4000;
        }
		sp = dbcSpell.LookupEntryForced( 20268 );
		if( sp != NULL )
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SINGLE_ENEMY;

		//Paladin - Seal of Light
		sp = dbcSpell.LookupEntryForced( 20165 );
		if( sp != NULL )
			sp->proc_interval = 4000;

		//Paladin - Seal of Wisdom
		sp = dbcSpell.LookupEntryForced( 20166 );
		if( sp != NULL )
			sp->proc_interval = 4000;

		//Paladin - Vengeance
		sp = dbcSpell.LookupEntryForced( 20049 ); //Rank 1
		if( sp != NULL )
		{
			sp->EffectTriggerSpell[0] = 20050;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags = PROC_ON_CRIT_ATTACK | PROC_ON_SPELL_CRIT_HIT;
		}

		sp = dbcSpell.LookupEntryForced( 20050 ); // Rank 1 proc
		if( sp != NULL )
			sp->maxstack = 3;

		sp = dbcSpell.LookupEntryForced( 20056 ); //Rank 2
		if( sp != NULL )
		{
			sp->EffectTriggerSpell[0] = 20052;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags = PROC_ON_CRIT_ATTACK | PROC_ON_SPELL_CRIT_HIT;
		}

		sp = dbcSpell.LookupEntryForced( 20052 ); // Rank 2 proc
		if( sp != NULL )
			sp->maxstack = 3;

		sp = dbcSpell.LookupEntryForced( 20057 ); //Rank 3
		if( sp != NULL )
		{
			sp->EffectTriggerSpell[0] = 20053;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags = PROC_ON_CRIT_ATTACK | PROC_ON_SPELL_CRIT_HIT;
		}
		sp = dbcSpell.LookupEntryForced( 20053 ); // Rank 3 proc
		if( sp != NULL )
			sp->maxstack = 3;

		//Paladin - Seal of Command - trigger
		sp = dbcSpell.LookupEntryForced( 20375 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK;
			sp->EffectTriggerSpell[0] = 20424;
		}

		//Paladin - Seal of Command - Holy damage, but melee mechanics (crit damage, chance, etc)
		sp = dbcSpell.LookupEntryForced( 20424 );
		if( sp != NULL )
			sp->is_melee_spell = true;

		//Paladin - Seal of Vengeance
		sp = dbcSpell.LookupEntryForced( 31801 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK;
			sp->EffectTriggerSpell[0] = 31803;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
		}
		//Paladin - Holy Vengeance
		sp = dbcSpell.LookupEntryForced( 31803 );
		if(sp != NULL)
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PERIODIC_DAMAGE;

		//Paladin - Seal of Corruption
		sp = dbcSpell.LookupEntryForced( 53736 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK;
			sp->EffectTriggerSpell[0] = 53742;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
		}

		//Paladin - Hammer of the Righteous
		sp = dbcSpell.LookupEntryForced( 53595);
		if( sp != NULL )
		{
			sp->speed = 0;	//without, no damage is done
		}

		sp = dbcSpell.LookupEntryForced( 38008 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 31893;
		}

		//Paladin - Seal of Martyr
		sp = dbcSpell.LookupEntryForced( 53720 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 53719;
			sp->School = SCHOOL_HOLY;
			sp->procFlags = PROC_ON_MELEE_ATTACK;
		}
		//Paladin - seal of blood
		sp = dbcSpell.LookupEntryForced( 31892 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 31893;	
			sp->School = SCHOOL_HOLY;
			sp->procFlags = PROC_ON_MELEE_ATTACK;
		}
		sp = dbcSpell.LookupEntryForced( 53719 );
		if( sp != NULL )
		{
			sp->School = SCHOOL_HOLY;
			sp->Spell_Dmg_Type = SPELL_DMG_TYPE_MAGIC;
		}
		sp = dbcSpell.LookupEntryForced( 31893 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_PHYSICAL_ATTACK;
			sp->School = SCHOOL_HOLY;
			sp->Spell_Dmg_Type = SPELL_DMG_TYPE_MAGIC;
		}

		//Paladin - Divine Storm
		sp = dbcSpell.LookupEntryForced( 53385 );
		if( sp != NULL )
		{
			sp->Effect[1] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectRadiusIndex[0] = 43; //16 yards
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1] = 54172;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 100;
			sp->MaxTargets = 4;
		}

		//Paladin - Sacred Shield - bonus to flash is not working
		sp = dbcSpell.LookupEntryForced( 53601 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM | static_cast<uint32>(PROC_TARGET_SELF);
			sp->proc_interval = 6000;
			sp->EffectTriggerSpell[0] = 58597;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
		}

		//Paladin - Vindication
		sp = dbcSpell.LookupEntryForced( 26021 );
		if( sp != NULL )
		{
			sp->procChance = 30;
			sp->procFlags = PROC_ON_MELEE_ATTACK;
		}
		sp = dbcSpell.LookupEntryForced( 26016 );
		if( sp != NULL )
		{
			sp->procChance = 30;
			sp->procFlags = PROC_ON_MELEE_ATTACK;
		}
		sp = dbcSpell.LookupEntryForced( 9452 );
		if( sp != NULL )
		{
			sp->procChance = 30;
			sp->procFlags = PROC_ON_MELEE_ATTACK;
		}

		/**********************************************************
		 *	Blessing of Light
		 **********************************************************
		sp = dbcSpell.LookupEntryForced( 19977 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
			sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
		}
		sp = dbcSpell.LookupEntryForced( 19978 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
			sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
		}
		sp = dbcSpell.LookupEntryForced( 19979 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
			sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
		}
		sp = dbcSpell.LookupEntryForced( 27144 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
			sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
		}
		sp = dbcSpell.LookupEntryForced( 32770 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
			sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
		}*/

		//Paladin - Reckoning
		sp = dbcSpell.LookupEntryForced( 20177 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM | static_cast<uint32>(PROC_TARGET_SELF);

		sp = dbcSpell.LookupEntryForced( 20179 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM | static_cast<uint32>(PROC_TARGET_SELF);

		sp = dbcSpell.LookupEntryForced( 20180 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM | static_cast<uint32>(PROC_TARGET_SELF);

		sp = dbcSpell.LookupEntryForced( 20181 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM | static_cast<uint32>(PROC_TARGET_SELF);

		sp = dbcSpell.LookupEntryForced( 20182 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM | static_cast<uint32>(PROC_TARGET_SELF);

		//Paladin - Reckoning Effect
		sp = dbcSpell.LookupEntryForced( 20178 );
		if( sp != NULL )
		{
			sp->procChance = 100;
			sp->procFlags = PROC_ON_MELEE_ATTACK | static_cast<uint32>(PROC_TARGET_SELF);
		}

		//Paladin - Eye for an Eye
		sp = dbcSpell.LookupEntryForced( 9799 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM | PROC_ON_SPELL_CRIT_HIT_VICTIM;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 25997;
			sp->fixed_dddhcoef = 0.0f;
		}
		sp = dbcSpell.LookupEntryForced( 25988 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM | PROC_ON_SPELL_CRIT_HIT_VICTIM;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 25997;
			sp->fixed_dddhcoef = 0.0f;
		}
		sp = dbcSpell.LookupEntryForced( 25997 );
		if( sp != NULL )
			sp->fixed_dddhcoef = 0.0f;

		//Paladin - Judgements of the Wise
		sp = dbcSpell.LookupEntryForced( 31930 );
		if( sp != NULL )
		{
			sp->SpellFamilyName = 0;
			sp->SpellGroupType[0] =0;
			sp->SpellGroupType[1] =0;
			sp->SpellGroupType[2] =0;
			sp->RankNumber = 0;
		}

		sp = dbcSpell.LookupEntryForced( 54180 );
		if( sp != NULL )
		{
			sp->SpellFamilyName = 0;
			sp->SpellGroupType[0] = 0;
			sp->SpellGroupType[1] = 0;
			sp->SpellGroupType[2] = 0;
			sp->RankNumber = 0;
			sp->proc_interval = 4000;
		}

		sp = dbcSpell.LookupEntryForced( 31876 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 54180;
			sp->procChance = 33;
		}
		sp = dbcSpell.LookupEntryForced( 31877 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 54180;
			sp->procChance = 66;
		}
		sp = dbcSpell.LookupEntryForced( 31878 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 54180;
			sp->procChance = 100;
		}

		//Paladin - Blessed Life ranks 1-3
		sp = dbcSpell.LookupEntryForced( 31828 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;

		sp = dbcSpell.LookupEntryForced( 31829 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;
		
		sp = dbcSpell.LookupEntryForced( 31830 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;

		//Palarin - Light's Grace
		sp = dbcSpell.LookupEntryForced( 31833 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL;

		sp = dbcSpell.LookupEntryForced( 31835 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL;

		sp = dbcSpell.LookupEntryForced( 31836 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL;

		//Paladin - Spiritual Attunement
		sp = dbcSpell.LookupEntryForced( 31785 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_SPELL_LAND_VICTIM | static_cast<uint32>(PROC_TARGET_SELF) ;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 31786;
		}
		sp = dbcSpell.LookupEntryForced( 33776 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_SPELL_LAND_VICTIM | static_cast<uint32>(PROC_TARGET_SELF);
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 31786;
		}

		//Paladin - Seal of Justice - Proc Chance
		sp = dbcSpell.LookupEntryForced( 20164 );
		if( sp != NULL )
			sp->procChance = 25;

		//Paladin - Improved Lay on Hands
		sp = dbcSpell.LookupEntryForced( 20234 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL;

		sp = dbcSpell.LookupEntryForced( 20235 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL;

		//Paladin - Crusader Strike
		sp = dbcSpell.LookupEntryForced( 35395 );
		if( sp != NULL )
			sp->noproc = true;

		//Paladin - Forbearance - Is forced debuff
		sp = dbcSpell.LookupEntryForced( 25771 );
		if( sp != NULL )
			sp->c_is_flags |= SPELL_FLAG_IS_FORCEDDEBUFF;

		//Paladin - Infusion of Light
		sp = dbcSpell.LookupEntryForced( 53569 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
			sp->procChance = 100;
		}
		sp = dbcSpell.LookupEntryForced( 53576 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
			sp->procChance = 100;
		}

		//Paladin - Sacred Cleansing
		sp = dbcSpell.LookupEntryForced( 53551 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 10;
		}
		sp = dbcSpell.LookupEntryForced( 53552 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 20;
		}
		sp = dbcSpell.LookupEntryForced( 53553 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 30;
		}

		//Paladin - Judgements of the Pure
		sp = dbcSpell.LookupEntryForced( 53671 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 100;
		}
		sp = dbcSpell.LookupEntryForced( 53673 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 100;
		}
		sp = dbcSpell.LookupEntryForced( 54151 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 100;
		}
		sp = dbcSpell.LookupEntryForced( 54154 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 100;
		}
		sp = dbcSpell.LookupEntryForced( 54155 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 100;
		}

		//Paladin -  Heart of the Crusader
		sp = dbcSpell.LookupEntryForced( 20335 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 21183;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 100;
		}
		sp = dbcSpell.LookupEntryForced( 20336 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 54498;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 100;
		}
		sp = dbcSpell.LookupEntryForced( 20337 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 54499;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 100;
		}

		//Paladin - Art of War
		sp = dbcSpell.LookupEntryForced( 53486 );
		if( sp !=NULL )
		{
			sp->Effect[1] = 6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_DAMAGE_DONE;
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->EffectTriggerSpell[1] = 53489;
		}
		sp = dbcSpell.LookupEntryForced( 53489 );
		if( sp != NULL )
			sp->AuraInterruptFlags = AURA_INTERRUPT_ON_CAST_SPELL;

		sp = dbcSpell.LookupEntryForced( 53488 );
		if( sp !=NULL )
		{
			sp->Effect[1] = 6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_DAMAGE_DONE;
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->EffectTriggerSpell[1] = 59578;
		}
		sp = dbcSpell.LookupEntryForced( 59578 );
		if( sp != NULL )
			sp->AuraInterruptFlags = AURA_INTERRUPT_ON_CAST_SPELL;

		//Paladin - Hammer of Justice - Interrupt effect
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

        // Paladin - Sheath of Light
        sp = dbcSpell.LookupEntryForced( 53501 );
        if( sp != NULL )
        {
            sp->Effect[1] = 6;
			sp->EffectApplyAuraName[1] = 42;
			sp->EffectTriggerSpell[1] = 54203;
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
        }

        sp = dbcSpell.LookupEntryForced( 53502 );
        if( sp != NULL )
        {
            sp->Effect[1] = 6;
			sp->EffectApplyAuraName[1] = 42;
			sp->EffectTriggerSpell[1] = 54203;
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
        }

        sp = dbcSpell.LookupEntryForced( 53503 );
        if( sp != NULL )
        {
            sp->Effect[1] = 6;
			sp->EffectApplyAuraName[1] = 42;
			sp->EffectTriggerSpell[1] = 54203;
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
        }

	//////////////////////////////////////////
	// HUNTER								//
	//////////////////////////////////////////
    //Wild quiver rank 1
    sp = dbcSpell.LookupEntryForced( 53215 );
    if( sp != NULL )
    {
            sp->procFlags = PROC_ON_RANGED_ATTACK;
            sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
            sp->EffectTriggerSpell[0] = 53254;
            sp->procChance = 4;
    }
    sp = dbcSpell.LookupEntryForced( 53216 );
    if( sp != NULL )
        {
            sp->procFlags = PROC_ON_RANGED_ATTACK;
            sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
            sp->EffectTriggerSpell[0] = 53254;
            sp->procChance = 7;
        }
        sp = dbcSpell.LookupEntryForced( 53217 );
        if( sp != NULL )
        {
            sp->procFlags = PROC_ON_RANGED_ATTACK;
            sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
            sp->EffectTriggerSpell[0] = 53254;
            sp->procChance = 10;
        }

	// Insert hunter spell fixes here

		// Pashtet: Lock'n'Load, only for traps. Need something more for Serpent Sting DoT.
		sp = dbcSpell.LookupEntryForced( 56342 );
		if (sp != NULL)
		{
			sp->procFlags |= PROC_ON_TRAP_TRIGGER;
			sp->proc_interval = 30000;
		}			
		sp = dbcSpell.LookupEntryForced( 56343 );
		if (sp != NULL)
		{
			sp->procFlags |= PROC_ON_TRAP_TRIGGER;
			sp->proc_interval = 30000;
		}	
		sp = dbcSpell.LookupEntryForced( 56344 );
		if (sp != NULL)
		{
			sp->procFlags |= PROC_ON_TRAP_TRIGGER;
			sp->proc_interval = 30000;
		}		

		//Hunter - Bestial Wrath
		sp = dbcSpell.LookupEntryForced( 19574 );
		if( sp != NULL )
			sp->EffectApplyAuraName[2] = SPELL_AURA_DUMMY;

		//Hunter - The Beast Within
		sp = dbcSpell.LookupEntryForced( 34692 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 100;
			sp->EffectTriggerSpell[0] = 34471;
		}
		sp = dbcSpell.LookupEntryForced( 34471 );
		if( sp != NULL )
			sp->EffectApplyAuraName[2] = SPELL_AURA_DUMMY;

		//Hunter - Go for the Throat
		sp = dbcSpell.LookupEntryForced( 34950 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_RANGED_CRIT_ATTACK;
		sp = dbcSpell.LookupEntryForced( 34954 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_RANGED_CRIT_ATTACK;
		sp = dbcSpell.LookupEntryForced( 34952 );
		if( sp != NULL )
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		sp = dbcSpell.LookupEntryForced( 34953 );
		if( sp != NULL )
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;

		// Hunter - Improved Wing Clip
		sp = dbcSpell.LookupEntryForced( 19228 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPECIFIC_SPELL;
			sp->ProcOnNameHash[0] = SPELL_HASH_WING_CLIP;
		}
		sp = dbcSpell.LookupEntryForced( 19232 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPECIFIC_SPELL;
			sp->ProcOnNameHash[0] = SPELL_HASH_WING_CLIP;
		}
		sp = dbcSpell.LookupEntryForced( 19233 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPECIFIC_SPELL;
			sp->ProcOnNameHash[0] = SPELL_HASH_WING_CLIP;
		}

		// Hunter - Master Tactician
		sp = dbcSpell.LookupEntryForced( 34506 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_RANGED_ATTACK | static_cast<uint32>(PROC_TARGET_SELF);
		sp = dbcSpell.LookupEntryForced( 34507 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_RANGED_ATTACK | static_cast<uint32>(PROC_TARGET_SELF);
		sp = dbcSpell.LookupEntryForced( 34508 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_RANGED_ATTACK | static_cast<uint32>(PROC_TARGET_SELF);
		sp = dbcSpell.LookupEntryForced( 34838 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_RANGED_ATTACK | static_cast<uint32>(PROC_TARGET_SELF);
		sp = dbcSpell.LookupEntryForced( 34839 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_RANGED_ATTACK | static_cast<uint32>(PROC_TARGET_SELF);

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

		//Hunter Silencing Shot
		//http://www.naxxramas.net/bug_list/showreport.php?bugid=234 NTY
		sp = dbcSpell.LookupEntryForced(34490);
		if(sp != NULL)
		{
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_SILENCE ;
		}

		// Hunter - Ferocious Inspiration
		sp = dbcSpell.LookupEntryForced( 34455 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->EffectTriggerSpell[0] = 34456;
			sp->procFlags = PROC_ON_CRIT_ATTACK | PROC_ON_SPELL_CRIT_HIT | static_cast<uint32>(PROC_TARGET_SELF); //maybe target master ?
			sp->Effect[1] = 0; //remove this
		}
		sp = dbcSpell.LookupEntryForced( 34459 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->EffectTriggerSpell[0] = 34456;
			sp->procFlags = PROC_ON_CRIT_ATTACK | PROC_ON_SPELL_CRIT_HIT | static_cast<uint32>(PROC_TARGET_SELF);
			sp->Effect[1] = 0; //remove this
		}
		sp = dbcSpell.LookupEntryForced( 34460 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->EffectTriggerSpell[0] = 34456;
			sp->procFlags = PROC_ON_CRIT_ATTACK | PROC_ON_SPELL_CRIT_HIT | static_cast<uint32>(PROC_TARGET_SELF);
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

		// Hunter - Thrill of the Hunt
		sp = dbcSpell.LookupEntryForced( 34497 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT | static_cast<uint32>(PROC_TARGET_SELF);
			sp->procChance = sp->EffectBasePoints[0]+1;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 34720;
		}
		sp = dbcSpell.LookupEntryForced( 34498 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT | static_cast<uint32>(PROC_TARGET_SELF);
			sp->procChance = sp->EffectBasePoints[0]+1;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 34720;
		}
		sp = dbcSpell.LookupEntryForced( 34499 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT | static_cast<uint32>(PROC_TARGET_SELF);
			sp->procChance = sp->EffectBasePoints[0]+1;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 34720;
		}

		// Hunter - Expose Weakness
		sp = dbcSpell.LookupEntryForced( 34500 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_RANGED_CRIT_ATTACK;
		sp = dbcSpell.LookupEntryForced( 34502 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_RANGED_CRIT_ATTACK;
		sp = dbcSpell.LookupEntryForced( 34503 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_RANGED_CRIT_ATTACK;

		//Hunter - Frenzy
		sp = dbcSpell.LookupEntryForced( 19621 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 19615;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->procChance = sp->EffectBasePoints[0];
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET | SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET | static_cast<uint32>(PROC_TARGET_SELF);
		}
		sp = dbcSpell.LookupEntryForced( 19622 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 19615;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->procChance = sp->EffectBasePoints[0];
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET | SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET | static_cast<uint32>(PROC_TARGET_SELF);
		}
		sp = dbcSpell.LookupEntryForced( 19623 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 19615;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->procChance = sp->EffectBasePoints[0];
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET | SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET | static_cast<uint32>(PROC_TARGET_SELF);
		}
		sp = dbcSpell.LookupEntryForced( 19624 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 19615;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->procChance = sp->EffectBasePoints[0];
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET | SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET | static_cast<uint32>(PROC_TARGET_SELF);
		}
		sp = dbcSpell.LookupEntryForced( 19625 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 19615;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->procChance = sp->EffectBasePoints[0];
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET | SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET | static_cast<uint32>(PROC_TARGET_SELF);
		}

		//Hunter : Pathfinding
		sp = dbcSpell.LookupEntryForced( 19559 );
		if( sp != NULL )
		{
			sp->EffectMiscValue[0] = SMT_MISC_EFFECT;
		}
		sp = dbcSpell.LookupEntryForced( 19560 );
		if( sp != NULL )
		{
			sp->EffectMiscValue[0] = SMT_MISC_EFFECT;
		}

		//Hunter : Rapid Killing - might need to add honor trigger too here. I'm guessing you receive Xp too so I'm avoiding double proc
		sp = dbcSpell.LookupEntryForced( 34948 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_GAIN_EXPIERIENCE | static_cast<uint32>(PROC_TARGET_SELF);
		}
		sp = dbcSpell.LookupEntryForced( 34949 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_GAIN_EXPIERIENCE | static_cast<uint32>(PROC_TARGET_SELF);
		}

		//Hunter : Entrapment
		sp = dbcSpell.LookupEntryForced( 19184 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_TRAP_TRIGGER;
		sp = dbcSpell.LookupEntryForced( 19387 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_TRAP_TRIGGER;
		sp = dbcSpell.LookupEntryForced( 19388 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_TRAP_TRIGGER;

		/* aspect of the pack - change to AA */
		sp = dbcSpell.LookupEntryForced( 13159 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;

		/* aspect of the cheetah - add proc flags */
		sp = dbcSpell.LookupEntryForced( 5118 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;

		// Feed pet
		sp = dbcSpell.LookupEntryForced( 6991 );
		if( sp != NULL )
		{
			sp->EffectImplicitTargetA[0] = 0;
		}
	/*	// !!! not sure this is good !!! have to test
		// Hunter's mark r1
		sp = dbcSpell.LookupEntryForced( 1130 );
		if( sp != NULL )
			sp->maxstack = (sp->EffectBasePoints[1]*3) / (sp->EffectBasePoints[1]/10);
		sp = dbcSpell.LookupEntryForced( 14323 );
		if( sp != NULL )
			sp->maxstack = (sp->EffectBasePoints[1]*3) / (sp->EffectBasePoints[1]/10);
		sp = dbcSpell.LookupEntryForced( 14324 );
		if( sp != NULL )
			sp->maxstack = (sp->EffectBasePoints[1]*3) / (sp->EffectBasePoints[1]/10);
		sp = dbcSpell.LookupEntryForced( 14325 );
		if( sp != NULL )
			sp->maxstack = (sp->EffectBasePoints[1]*3) / (sp->EffectBasePoints[1]/10);
			*/


	//////////////////////////////////////////
	// ROGUE								//
	//////////////////////////////////////////

	// Insert rogue spell fixes here

		//Rogue - Blade Twisting Rank 1
		sp = dbcSpell.LookupEntryForced(31124);
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[1] = 31125;
			sp->procFlags = PROC_ON_MELEE_ATTACK;
			sp->procChance = 10;
		}

		//Rogue - Blade Twisting Rank 2
		sp = dbcSpell.LookupEntryForced(31126);
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[1] = 51585;
			sp->procFlags = PROC_ON_MELEE_ATTACK;
			sp->procChance = 10;
		}

		// Waylay talent rank 1
		sp = dbcSpell.LookupEntryForced(51692);
		if(sp != NULL)
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]=51693;// Waylay debuff
			sp->procChance = 50;
		}
		//Waylay talent rank 2
		sp = dbcSpell.LookupEntryForced(51696);
		if(sp != NULL)
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]=51693;// Waylay debuff
			sp->procChance = 100;
		}

		/**********************************************************
		 *	Garrote - this is used?
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 37066 );
		if( sp != NULL )
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SINGLE_ENEMY;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_SINGLE_ENEMY;
		}

		//rogue - Find Weakness.
		sp = dbcSpell.LookupEntryForced( 31233 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL;
		sp = dbcSpell.LookupEntryForced( 31239 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL;
		sp = dbcSpell.LookupEntryForced( 31240 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL;
		sp = dbcSpell.LookupEntryForced( 31241 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL;
		sp = dbcSpell.LookupEntryForced( 31242 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL;

		//rogue - Camouflage.
		sp = dbcSpell.LookupEntryForced( 13975 );
		if( sp != NULL )
		{
			sp->EffectMiscValue[0] = SMT_MISC_EFFECT;
		}
		sp = dbcSpell.LookupEntryForced( 14062 );
		if( sp != NULL )
		{
			sp->EffectMiscValue[0] = SMT_MISC_EFFECT;
		}
		sp = dbcSpell.LookupEntryForced( 14063 );
		if( sp != NULL )
		{
			sp->EffectMiscValue[0] = SMT_MISC_EFFECT;
		}
		sp = dbcSpell.LookupEntryForced( 14064 );
		if( sp != NULL )
		{
			sp->EffectMiscValue[0] = SMT_MISC_EFFECT;
		}
		sp = dbcSpell.LookupEntryForced( 14065 );
		if( sp != NULL )
		{
			sp->EffectMiscValue[0] = SMT_MISC_EFFECT;
		}

		//rogue - Vanish : Second Trigger Spell
		sp = dbcSpell.LookupEntryForced( 18461 );
		if( sp != NULL )
			sp->AttributesEx |= ATTRIBUTESEX_NOT_BREAK_STEALTH;

		// rogue - shadowstep
//		sp = dbcSpell.LookupEntryForced( 36554 );
//		if( sp != NULL )
//			sp->AttributesEx |= ATTRIBUTESEX_NOT_BREAK_STEALTH;

		// rogue - Blind (Make it able to miss!)
		sp = dbcSpell.LookupEntryForced( 2094 );
		if( sp != NULL )
		{
			sp->Spell_Dmg_Type = SPELL_DMG_TYPE_RANGED;
			sp->is_ranged_spell = true;
		}
		//rogue - Mace Specialization.
		sp = dbcSpell.LookupEntryForced( 13709 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_MELEE_ATTACK;
		sp = dbcSpell.LookupEntryForced( 13800 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_MELEE_ATTACK;
		sp = dbcSpell.LookupEntryForced( 13801 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_MELEE_ATTACK;
		sp = dbcSpell.LookupEntryForced( 13802 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_MELEE_ATTACK;
		sp = dbcSpell.LookupEntryForced( 13803 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_MELEE_ATTACK;

		//rogue - Shadowstep
		sp = dbcSpell.LookupEntryForced( 36563 );
		if( sp != NULL )
		{
			sp->EffectMiscValue[2] = SMT_MISC_EFFECT;
//			sp->EffectTriggerSpell[1] = 36554;
//			sp->EffectTriggerSpell[2] = 44373;
//			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		// Still related to shadowstep - prevent the trigger spells from breaking stealth.
		sp = dbcSpell.LookupEntryForced( 44373 );
		if( sp )
			sp->AttributesEx |= ATTRIBUTESEX_NOT_BREAK_STEALTH;
		sp = dbcSpell.LookupEntryForced( 36563 );
		if( sp )
			sp->AttributesEx |= ATTRIBUTESEX_NOT_BREAK_STEALTH;
		sp = dbcSpell.LookupEntryForced( 36554 );
		if( sp != NULL )
			sp->AttributesEx |= ATTRIBUTESEX_NOT_BREAK_STEALTH;

		//rogue - Seal Fate
		sp = dbcSpell.LookupEntryForced( 14186 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 14189;
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->procChance = 20;
		}
		sp = dbcSpell.LookupEntryForced( 14190 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 14189;
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->procChance = 40;
		}
		sp = dbcSpell.LookupEntryForced( 14193 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 14189;
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->procChance = 60;
		}
		sp = dbcSpell.LookupEntryForced( 14194 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 14189;
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->procChance = 80;
		}
		sp = dbcSpell.LookupEntryForced( 14195 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 14189;
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->procChance = 100;
		}
	#ifndef NEW_PROCFLAGS
		//Improved Sprint
		sp = dbcSpell.LookupEntryForced( 13743 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 50;
		}
		sp = dbcSpell.LookupEntryForced( 13875 );
		if( sp != NULL )
		{
			sp->procChance = 100;
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
	#else
		//Improved Sprint
		sp = dbcSpell.LookupEntryForced( 13743 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0]=64;
		sp = dbcSpell.LookupEntryForced( 13875 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0]=64;
	#endif
		//garrot
		sp = dbcSpell.LookupEntryForced( 703 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 8631 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 8632 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 8633 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 11289 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 11290 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 26839 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 26884 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;

		//rupture
		sp = dbcSpell.LookupEntryForced( 1943 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 8639 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 8640 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 11273 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 11274 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 11275 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 26867 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
	#ifndef NEW_PROCFLAGS
		//Relentless Strikes
		sp = dbcSpell.LookupEntryForced( 14179 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;//proc spell
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectBasePoints[1] = 20; //client showes 20% chance but we do not have it ? :O
		}
	#else
		//Relentless Strikes
		sp = dbcSpell.LookupEntryForced( 14179 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0]= 262144 | 2097152 | 8388608 | 8519680 | 524288 | 1048576 | 8388608;
	#endif
	#ifndef NEW_PROCFLAGS
		//rogue - initiative
		sp = dbcSpell.LookupEntryForced( 13976 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags = uint32(PROC_ON_CAST_SPELL|PROC_TARGET_SELF);
		}
		sp = dbcSpell.LookupEntryForced( 13979 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags = uint32(PROC_ON_CAST_SPELL|PROC_TARGET_SELF);
		}
		sp = dbcSpell.LookupEntryForced( 13980 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags = uint32(PROC_ON_CAST_SPELL|PROC_TARGET_SELF);
		}
	#else
		//rogue - initiative
		sp = dbcSpell.LookupEntryForced( 13976 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 8389120 | 256 | 1024;
		sp = dbcSpell.LookupEntryForced( 13979 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 8389120 | 256 | 1024;
		sp = dbcSpell.LookupEntryForced( 13980 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 8389120 | 256 | 1024;
	#endif

	//Rogue - Killing Spree Stealth fix
	sp = dbcSpell.LookupEntryForced( 51690 );
	if( sp != NULL )
		sp->AttributesEx |= ATTRIBUTESEX_NOT_BREAK_STEALTH;

	// Rogue - Cold Blood (Interrupt Flag)
	sp = dbcSpell.LookupEntryForced( 14177 );
	if(sp != NULL)
		sp->AuraInterruptFlags |= AURA_INTERRUPT_ON_AFTER_CAST_SPELL;

	/* Rogue - Improved Expose Armor (rank 1)
	sp = dbcSpell.LookupEntryForced( 14168 );
	if(sp != NULL)
	{
		sp->EffectApplyAuraName[0] = */

	//////////////////////////////////////////
	// PRIEST								//
	//////////////////////////////////////////

	// Insert priest spell fixes here
		// Prayer of mending. !very very overwritten
		// how it is after rewriten : we simply proc on damage and prochandler will get new target + do healing
		sp = dbcSpell.LookupEntryForced( 33076 );
		if( sp != NULL )
		{
			//we use this heal spell when we jump to other player
			SpellEntry *healsp = dbcSpell.LookupEntryForced( sp->EffectTriggerSpell[1] );
			if( healsp )
			{
				healsp->Effect[0] = SPELL_EFFECT_HEAL;
				healsp->Effect[1] = healsp->Effect[2] = SPELL_EFFECT_NULL;
				healsp->EffectBasePoints[0] = sp->EffectBasePoints[0];
				healsp->EffectBaseDice[0] = sp->EffectBaseDice[0];
				healsp->EffectDicePerLevel[0] = sp->EffectDicePerLevel[0];
				healsp->EffectDieSides[0] = sp->EffectDieSides[0];
				healsp->EffectImplicitTargetA[0] = EFF_TARGET_PARTY_MEMBER;
			}
			//this spell is just to register the proc
			SpellEntry *procsp = dbcSpell.LookupEntryForced( sp->EffectTriggerSpell[0] );
			if( procsp )
			{
				procsp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
				healsp->Effect[1] = healsp->Effect[2] = SPELL_EFFECT_NULL;
				procsp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
				procsp->EffectBasePoints[0] = sp->procCharges - 1; //we loose 1 charge each time we cast so we need this value mobile
				procsp->EffectImplicitTargetA[0] = EFF_TARGET_PARTY_MEMBER; //we jump on an injured party member
				procsp->EffectTriggerSpell[0] = sp->EffectTriggerSpell[1]; //!we proc self but our system does not allow proc loops !
				procsp->procCharges = 1;
				procsp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM | static_cast<uint32>(PROC_TARGET_SELF);
			}
			//simplify old system with a simple cast spell
			sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->Effect[1] = SPELL_EFFECT_NULL;
			sp->Effect[2] = SPELL_EFFECT_NULL;
		}

		// Spirit Tap
		sp = dbcSpell.LookupEntryForced( 15270 ); //rank 1
		if( sp != NULL )
		{
//			sp->procFlags = PROC_ON_TARGET_DIE;
			sp->procFlags = PROC_ON_GAIN_EXPIERIENCE;
		}
		sp = dbcSpell.LookupEntryForced( 15335 ); //rank 2
		if( sp != NULL )
		{
//			sp->procFlags = PROC_ON_TARGET_DIE;
			sp->procFlags = PROC_ON_GAIN_EXPIERIENCE;
		}
		sp = dbcSpell.LookupEntryForced( 15336 ); //rank 3
		if( sp != NULL )
		{
//			sp->procFlags = PROC_ON_TARGET_DIE;
			sp->procFlags = PROC_ON_GAIN_EXPIERIENCE;
		}
		sp = dbcSpell.LookupEntryForced( 15337 ); //rank 4
		if( sp != NULL )
		{
//			sp->procFlags = PROC_ON_TARGET_DIE;
			sp->procFlags = PROC_ON_GAIN_EXPIERIENCE;
		}
		sp = dbcSpell.LookupEntryForced( 15338 ); //rank 5
		if( sp != NULL )
		{
//			sp->procFlags = PROC_ON_TARGET_DIE;
			sp->procFlags = PROC_ON_GAIN_EXPIERIENCE;
		}

		/**********************************************************
		 *	Holy Nova
		 **********************************************************/
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
		// Holy Nova
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

		// Holy Concentration
        sp = dbcSpell.LookupEntryForced( 34753 ); //Rank 1
            if( sp != NULL )
            {
            sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
            sp->procChance = 10;
            sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
            sp->EffectTriggerSpell[0] = 34754;
			}	
        sp = dbcSpell.LookupEntryForced( 34859 ); //Rank 2
            if( sp != NULL )
            {
            sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
            sp->procChance = 20;
            sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
            sp->EffectTriggerSpell[0] = 34754;
			}
		sp = dbcSpell.LookupEntryForced( 34860 ); //Rank 3
            if( sp != NULL )
            {
            sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
            sp->procChance = 30;
            sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
            sp->EffectTriggerSpell[0] = 34754;
			}

		//Priest: Blessed Recovery
		sp = dbcSpell.LookupEntryForced(27811);
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[0] = 27813;
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM | PROC_ON_RANGED_CRIT_ATTACK_VICTIM;
		}
		sp = dbcSpell.LookupEntryForced(27815);
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[0] = 27817;
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM | PROC_ON_RANGED_CRIT_ATTACK_VICTIM;
		}
		sp = dbcSpell.LookupEntryForced(27816);
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[0] = 27818;
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM | PROC_ON_RANGED_CRIT_ATTACK_VICTIM;
		}
		//priest- Blessed Resilience
		sp = dbcSpell.LookupEntryForced( 33142 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM | PROC_ON_SPELL_CRIT_HIT_VICTIM | PROC_ON_RANGED_CRIT_ATTACK_VICTIM;
			sp->procChance = 20;
		}
		sp = dbcSpell.LookupEntryForced( 33145 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM | PROC_ON_SPELL_CRIT_HIT_VICTIM | PROC_ON_RANGED_CRIT_ATTACK_VICTIM;
			sp->procChance = 40;
		}
		sp = dbcSpell.LookupEntryForced( 33146 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM | PROC_ON_SPELL_CRIT_HIT_VICTIM | PROC_ON_RANGED_CRIT_ATTACK_VICTIM;
			sp->procChance = 60;
		}

		//priest- Focused Will
		sp = dbcSpell.LookupEntryForced( 45234 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM | PROC_ON_SPELL_CRIT_HIT_VICTIM | PROC_ON_RANGED_CRIT_ATTACK_VICTIM;
		sp = dbcSpell.LookupEntryForced( 45243 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM | PROC_ON_SPELL_CRIT_HIT_VICTIM | PROC_ON_RANGED_CRIT_ATTACK_VICTIM;
		sp = dbcSpell.LookupEntryForced( 45244 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM | PROC_ON_SPELL_CRIT_HIT_VICTIM | PROC_ON_RANGED_CRIT_ATTACK_VICTIM;

		//Priest: Shadowguard
		sp = dbcSpell.LookupEntryForced( 18137 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_SPELL_HIT_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM | PROC_ON_MELEE_ATTACK_VICTIM;
			sp->proc_interval = 3000; //every 3 seconds
			sp->EffectTriggerSpell[0] = 28377;
		}
		sp = dbcSpell.LookupEntryForced( 19308 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_SPELL_HIT_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM | PROC_ON_MELEE_ATTACK_VICTIM;
			sp->proc_interval = 3000; //every 3 seconds
			sp->EffectTriggerSpell[0] = 28378;
		}
		sp = dbcSpell.LookupEntryForced( 19309 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_SPELL_HIT_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM | PROC_ON_MELEE_ATTACK_VICTIM;
			sp->proc_interval = 3000; //every 3 seconds
			sp->EffectTriggerSpell[0] = 28379;
		}
		sp = dbcSpell.LookupEntryForced( 19310 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_SPELL_HIT_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM | PROC_ON_MELEE_ATTACK_VICTIM;
			sp->proc_interval = 3000; //every 3 seconds
			sp->EffectTriggerSpell[0] = 28380;
		}
		sp = dbcSpell.LookupEntryForced( 19311 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_SPELL_HIT_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM | PROC_ON_MELEE_ATTACK_VICTIM;
			sp->proc_interval = 3000; //every 3 seconds
			sp->EffectTriggerSpell[0] = 28381;
		}
		sp = dbcSpell.LookupEntryForced( 19312 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_SPELL_HIT_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM | PROC_ON_MELEE_ATTACK_VICTIM;
			sp->proc_interval = 3000; //every 3 seconds
			sp->EffectTriggerSpell[0] = 28382;
		}
		sp = dbcSpell.LookupEntryForced( 25477 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_SPELL_HIT_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM | PROC_ON_MELEE_ATTACK_VICTIM;
			sp->proc_interval = 3000; //every 3 seconds
			sp->EffectTriggerSpell[0] = 28385;
		}

		//Priest - Wand Specialization
		sp = dbcSpell.LookupEntryForced( 14524 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectMiscValue[0] = SMT_MISC_EFFECT;
		}
		sp = dbcSpell.LookupEntryForced( 14525 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectMiscValue[0] = SMT_MISC_EFFECT;
		}
		sp = dbcSpell.LookupEntryForced( 14526 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectMiscValue[0] = SMT_MISC_EFFECT;
		}
		sp = dbcSpell.LookupEntryForced( 14527 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectMiscValue[0] = SMT_MISC_EFFECT;
		}
		sp = dbcSpell.LookupEntryForced( 14528 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectMiscValue[0] = SMT_MISC_EFFECT;
		}

	#ifdef NEW_PROCFLAGS
		//priest -  Shadow Weaving
		if (sp != NULL)
		{
			uint32 group = sp->EffectSpellGroupRelation[0];
			sp = dbcSpell.LookupEntryForced(15334);
			if (sp !=NULL)
				sp->EffectSpellGroupRelation[0] = group;
			sp = dbcSpell.LookupEntryForced(15333);
			if (sp !=NULL)
				sp->EffectSpellGroupRelation[0] = group;
			sp = dbcSpell.LookupEntryForced(15332);
			if (sp !=NULL)
				sp->EffectSpellGroupRelation[0] = group;
			sp = dbcSpell.LookupEntryForced(15331);
			if (sp !=NULL)
				sp->EffectSpellGroupRelation[0] = group;
			sp = dbcSpell.LookupEntryForced(15257);
			if (sp !=NULL)
				sp->EffectSpellGroupRelation[0] = group;
		}
	#endif

		//Priest - Inspiration proc spell
		sp = dbcSpell.LookupEntryForced( 14893 );
		if( sp != NULL )
			sp->rangeIndex = 4;
		sp = dbcSpell.LookupEntryForced( 15357 );
		if( sp != NULL )
			sp->rangeIndex = 4;
		sp = dbcSpell.LookupEntryForced( 15359 );
		if( sp != NULL )
			sp->rangeIndex = 4;

		//priest - surge of light
		sp = dbcSpell.LookupEntryForced( 33150 );
		if( sp != NULL )
			sp->procFlags = uint32(PROC_ON_SPELL_CRIT_HIT | static_cast<uint32>(PROC_TARGET_SELF));
		sp = dbcSpell.LookupEntryForced( 33154 );
		if( sp != NULL )
			sp->procFlags = uint32(PROC_ON_SPELL_CRIT_HIT | static_cast<uint32>(PROC_TARGET_SELF));
		sp = dbcSpell.LookupEntryForced( 33151 );
		if( sp != NULL )
		{
			sp->AuraInterruptFlags = AURA_INTERRUPT_ON_CAST_SPELL;
		}
		// priest - Reflective Shield
		sp = dbcSpell.LookupEntryForced( 33201 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_ABSORB;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 33619; //!! WRONG spell, we will make direct dmg here
		}
		sp = dbcSpell.LookupEntryForced( 33202 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_ABSORB;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 33619; //!! WRONG spell, we will make direct dmg here
		}
		sp = dbcSpell.LookupEntryForced( 33203 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_ABSORB;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 33619; //!! WRONG spell, we will make direct dmg here
		}
		sp = dbcSpell.LookupEntryForced( 33204 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_ABSORB;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 33619; //!! WRONG spell, we will make direct dmg here
		}
		sp = dbcSpell.LookupEntryForced( 33205 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_ABSORB;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 33619; //!! WRONG spell, we will make direct dmg here
		}

		// Mind Flay - Reduce movements by 50% and deal periodic damage to target.
/*
		const uint32 MindFlayRanks[] = { 15407, 17311, 17312, 17313, 17314, 18807, 25387, 48155, 48156, 0 };
		for(uint8 _i=0; MindFlayRanks[_i]!=0; ++_i)
		{
			sp = dbcSpell.LookupEntryForced( MindFlayRanks[_i] );
			if( sp != NULL )
			{
				sp->EffectApplyAuraName[0] = SPELL_AURA_PERIODIC_DAMAGE;
				sp->EffectImplicitTargetA[0] = EFF_TARGET_SINGLE_ENEMY;
			}
		}
*/
		//Mind Sear - Effect rank 1

		sp = dbcSpell.LookupEntryForced( 49821 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PERIODIC_DAMAGE;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SINGLE_ENEMY;
			sp->EffectImplicitTargetB[0] = EFF_TARGET_SINGLE_ENEMY;
		}

		// Mind Sear - Effect rank 2

		sp = dbcSpell.LookupEntryForced( 53023 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PERIODIC_DAMAGE;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SINGLE_ENEMY;
			sp->EffectImplicitTargetB[0] = EFF_TARGET_SINGLE_ENEMY;
		}

		// Weakened Soul - Is forced debuff
		sp = dbcSpell.LookupEntryForced( 6788 );
		if( sp != NULL )
		{
			sp->c_is_flags = SPELL_FLAG_IS_FORCEDDEBUFF;
		}

		// Penance
		sp = dbcSpell.LookupEntryForced( 47540 );
		if ( sp != NULL )
		{
			sp->DurationIndex = 566; // Change to instant cast as script will cast the real channeled spell.
			sp->ChannelInterruptFlags = 0; // Remove channeling behavior.
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

		// Penance triggered healing spells have wrong targets.
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


		//Ranger: Priest - Lightwell (rewrite, based on 2.4.3)
		sp = dbcSpell.LookupEntryForced( 724 );
		if ( sp != NULL )
		{
			sp->Effect[0] = 50;
			sp->EffectImplicitTargetA[0] = 52;
			sp->EffectRadiusIndex[0] = 7;
			sp->EffectMiscValue[0] = 3000000; //Gameobject in DB
		}

		sp = dbcSpell.LookupEntryForced( 27870 );
		if ( sp != NULL )
		{
			sp->Effect[0] = 50;
			sp->EffectImplicitTargetA[0] = 52;
			sp->EffectRadiusIndex[0] = 7;
			sp->EffectMiscValue[0] = 3000001; //Gameobject in DB
		}

		sp = dbcSpell.LookupEntryForced( 27871 );
		if ( sp != NULL )
		{
			sp->Effect[0] = 50;
			sp->EffectImplicitTargetA[0] = 52;
			sp->EffectRadiusIndex[0] = 7;
			sp->EffectMiscValue[0] = 3000002; //Gameobject in DB
		}

		sp = dbcSpell.LookupEntryForced( 28275 );
		if ( sp != NULL )
		{
			sp->Effect[0] = 50;
			sp->EffectImplicitTargetA[0] = 52;
			sp->EffectRadiusIndex[0] = 7;
			sp->EffectMiscValue[0] = 3000003; //Gameobject in DB
		}

		sp = dbcSpell.LookupEntryForced( 48086 );
		if ( sp != NULL )
		{
			sp->Effect[0] = 50;
			sp->EffectImplicitTargetA[0] = 52;
			sp->EffectRadiusIndex[0] = 7;
			sp->EffectMiscValue[0] = 3000004; //Gameobject in DB
		}

		sp = dbcSpell.LookupEntryForced( 48087 );
		if ( sp != NULL )
		{
			sp->Effect[0] = 50;
			sp->EffectImplicitTargetA[0] = 52;
			sp->EffectRadiusIndex[0] = 7;
			sp->EffectMiscValue[0] = 3000005; //Gameobject in DB
		}


		//Grace Rank 1
		sp = dbcSpell.LookupEntryForced(47516);
		if(sp != NULL)
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->proc_interval = 100;
		}

		//Grace Rank 2
		sp = dbcSpell.LookupEntryForced(47517);
		if(sp != NULL)
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->proc_interval = 100;
		}

	//////////////////////////////////////////
	// SHAMAN								//
	//////////////////////////////////////////

	// Insert shaman spell fixes here
#ifdef NEW_PROCFLAGS
	//Shaman - Healing Focus
	sp = dbcSpell.LookupEntryForced( 16181 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] =  64 | 128 | 256;
	sp = dbcSpell.LookupEntryForced( 16230 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] =  64 | 128 | 256;
	sp = dbcSpell.LookupEntryForced( 16232 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] =  64 | 128 | 256;
	sp = dbcSpell.LookupEntryForced( 16233 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] =  64 | 128 | 256;
	sp = dbcSpell.LookupEntryForced( 16234 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] =  64 | 128 | 256;


	//shaman - Tidal focus . Add more heal spells here if i forgot any :P
	sp = dbcSpell.LookupEntryForced( 16179 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = 64 | 128 | 256;
	sp = dbcSpell.LookupEntryForced( 16214 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = 64 | 128 | 256;
	sp = dbcSpell.LookupEntryForced( 16215 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = 64 | 128 | 256;
	sp = dbcSpell.LookupEntryForced( 16216 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = 64 | 128 | 256;
	sp = dbcSpell.LookupEntryForced( 16217 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = 64 | 128 | 256;

	//shaman - Earth's Grasp
	sp = dbcSpell.LookupEntryForced( 16130 );
	if( sp != NULL ){
		sp->EffectSpellGroupRelation[0] = 0x8;
		sp->EffectSpellGroupRelation_high[1] = 0x1;
	}
	sp = dbcSpell.LookupEntryForced( 16043 );
	if( sp != NULL ){
		sp->EffectSpellGroupRelation[0] = 0x8;
		sp->EffectSpellGroupRelation_high[1] = 0x1;
	}

	//shaman - Healing Grace
	sp = dbcSpell.LookupEntryForced( 29191 );
	if( sp != NULL ){
		sp->EffectSpellGroupRelation[0] = 64 | 128 | 256;
		sp->EffectSpellGroupRelation[1] = 0xFFFFFFFF; // all spells
		sp->EffectSpellGroupRelation_high[1] = 0xFFFFFFFF; // all spells
	}
	sp = dbcSpell.LookupEntryForced( 29189 );
	if( sp != NULL ){
		sp->EffectSpellGroupRelation[0] = 64 | 128 | 256;
		sp->EffectSpellGroupRelation[1] = 0xFFFFFFFF; // all spells
		sp->EffectSpellGroupRelation_high[1] = 0xFFFFFFFF; // all spells
	}
	sp = dbcSpell.LookupEntryForced( 29187 );
	if( sp != NULL ){
		sp->EffectSpellGroupRelation[0] = 64 | 128 | 256;
		sp->EffectSpellGroupRelation[1] = 0xFFFFFFFF; // all spells
		sp->EffectSpellGroupRelation_high[1] = 0xFFFFFFFF; // all spells
	}

	//shaman - Tidal Mastery
	sp = dbcSpell.LookupEntryForced( 16221 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = 451;
	sp = dbcSpell.LookupEntryForced( 16220 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = 451;
	sp = dbcSpell.LookupEntryForced( 16219 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = 451;
	sp = dbcSpell.LookupEntryForced( 16218 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = 451;
	sp = dbcSpell.LookupEntryForced( 16217 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = 451;

	//shaman - Improved Chain Heal
	sp = dbcSpell.LookupEntryForced( 30873 );
	if( sp != NULL ){
		sp->EffectSpellGroupRelation[0] = 256;
		sp->EffectDieSides[0] = 0;
	}
	sp = dbcSpell.LookupEntryForced( 30872 );
	if( sp != NULL ){
		sp->EffectSpellGroupRelation[0] = 256;
		sp->EffectDieSides[0] = 0;
	}
	// Shaman - Call of Flame
	sp = dbcSpell.LookupEntryForced( 16038 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = 0x40000000;
	sp = dbcSpell.LookupEntryForced( 16160 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = 0x40000000;
	sp = dbcSpell.LookupEntryForced( 16161 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = 0x40000000;

		//shaman ( grouping ) Mental Quickness (missing 18 spells which have no grouping)
	group_relation_shaman_mental_quickness = 0x00000008 | 0x00000010 | 0x00000200 | 0x00000400 | 0x00080000 | 0x00100000 | 0x00400000 | 0x20000000 | 0x10000000 | 0x80000000;

	//shaman - Mental Quickness
	sp = dbcSpell.LookupEntryForced( 30812 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = group_relation_shaman_mental_quickness;
	sp = dbcSpell.LookupEntryForced( 30813 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = group_relation_shaman_mental_quickness;
	sp = dbcSpell.LookupEntryForced( 30814 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = group_relation_shaman_mental_quickness;

	//shaman ( grouping ) Totems
	group_relation_shaman_totems |= 0x00000008 | 0x00000010 | 0x00001000 | 0x00080000 | 0x20000000; //these are only selected flag since some totems contain more then 1 flag

	//shaman - Totemic focus
	sp = dbcSpell.LookupEntryForced( 16173 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = group_relation_shaman_totems;
	sp = dbcSpell.LookupEntryForced( 16222 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = group_relation_shaman_totems;
	sp = dbcSpell.LookupEntryForced( 16223 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = group_relation_shaman_totems;
	sp = dbcSpell.LookupEntryForced( 16224 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = group_relation_shaman_totems;
	sp = dbcSpell.LookupEntryForced( 16225 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = group_relation_shaman_totems;

	//shaman ( grouping ) Lightning = Lightning Bolt + Chain Lightning
	group_relation_shaman_lightning = 0x00000001 | 0x00000002;

	//shaman - Call of Thunder
	sp = dbcSpell.LookupEntryForced( 16041 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = group_relation_shaman_lightning;
	sp = dbcSpell.LookupEntryForced( 16117 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = group_relation_shaman_lightning;
	sp = dbcSpell.LookupEntryForced( 16118 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = group_relation_shaman_lightning;
	sp = dbcSpell.LookupEntryForced( 16119 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = group_relation_shaman_lightning;
	sp = dbcSpell.LookupEntryForced( 16120 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = group_relation_shaman_lightning;

    //shaman ( grouping ) Shock = Earth + Flame + Frost
	group_relation_shaman_shock = 0x00100000 | 0x10000000 | 0x80000000;

	//shaman - Convection
	sp = dbcSpell.LookupEntryForced( 16039 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;
	sp = dbcSpell.LookupEntryForced( 16109 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;
	sp = dbcSpell.LookupEntryForced( 16110 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;
	sp = dbcSpell.LookupEntryForced( 16111 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;
	sp = dbcSpell.LookupEntryForced( 16112 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;

    //shaman - Concussion
    sp = dbcSpell.LookupEntryForced( 16035 );
    if( sp != NULL )
        sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;
    sp = dbcSpell.LookupEntryForced( 16105 );
    if( sp != NULL )
        sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;
    sp = dbcSpell.LookupEntryForced( 16106 );
    if( sp != NULL )
        sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;
    sp = dbcSpell.LookupEntryForced( 16107 );
    if( sp != NULL )
        sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;
    sp = dbcSpell.LookupEntryForced( 16108 );
    if( sp != NULL )
        sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;

	// Shaman Shock Range Bonus (Gauntlets bonus)
	sp = dbcSpell.LookupEntryForced( 32973 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock;

	// Shaman Shock Crit Bonus (set bonus)
	sp = dbcSpell.LookupEntryForced( 22804 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock;

	// Shaman - Storm Reach
	sp = dbcSpell.LookupEntryForced( 28999 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = 2 | 1;
	sp = dbcSpell.LookupEntryForced( 29000 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = 2 | 1;


	#endif
	#ifndef NEW_PROCFLAGS
	//shaman - Healing Way
	sp = dbcSpell.LookupEntryForced( 29202 );
	if( sp != NULL )
	{
		sp->procFlags = PROC_ON_CAST_SPELL;
		sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
	}
	sp = dbcSpell.LookupEntryForced( 29205 );
	if( sp != NULL )
	{
		sp->procFlags = PROC_ON_CAST_SPELL;
		sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
	}
	sp = dbcSpell.LookupEntryForced( 29206 );
	if( sp != NULL )
	{
		sp->procFlags = PROC_ON_CAST_SPELL;
		sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
	}
#else
	//shaman - Healing Way
	sp = dbcSpell.LookupEntryForced( 29202 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = 64;
	sp = dbcSpell.LookupEntryForced( 29205 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = 64;
	sp = dbcSpell.LookupEntryForced( 29206 );
	if( sp != NULL )
		sp->EffectSpellGroupRelation[0] = 64;
#endif
		
		/**********************************************************
		 *	Elemental Mastery
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced(16166);
		if(sp != NULL) {
			sp->EffectMiscValue[0] = SMT_CRITICAL;
			sp->EffectMiscValue[1] = SMT_COST;
			// sp->AuraInterruptFlags = AURA_INTERRUPT_ON_AFTER_CAST_SPELL;
		}

		/**********************************************************
		 *	Shamanistic Rage
		 **********************************************************/
		SpellEntry*  parentsp = dbcSpell.LookupEntryForced( 30823 );
		SpellEntry* triggersp = dbcSpell.LookupEntryForced( 30824 );
		if( parentsp != NULL && triggersp != NULL )
			triggersp->EffectBasePoints[0] = parentsp->EffectBasePoints[0];

		//summon only 1 elemental totem
		sp = dbcSpell.LookupEntryForced( 2894 );
		if( sp != NULL && sp->Id == 2894 )
			sp->EffectImplicitTargetA[0] = EFF_TARGET_TOTEM_FIRE; //remove this targeting. it is enough to get 1 target

		//summon only 1 elemental totem
		sp = dbcSpell.LookupEntryForced( 2062 );
		if( sp != NULL && sp->Id == 2062 )
			sp->EffectImplicitTargetA[0] = EFF_TARGET_TOTEM_EARTH; //remove this targeting. it is enough to get 1 target

		/**********************************************************
		 *	Elemental Focus
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 16164 );
		if( sp != NULL && sp->Id == 16164 )
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;

		/**********************************************************
		 *	Stormstrike
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 17364 );
		if( sp != NULL && sp->Id == 17364 )
		{
			sp->procFlags=PROC_ON_SPELL_HIT_VICTIM ;
		}

		/**********************************************************
		 *	Bloodlust
		 **********************************************************/
		// Bloodlust is working now without any fix required (2/23/2009, rev 2255)
		// The old fix made it not function as described (target raid)

		/**********************************************************
		 *	Heroism
		 **********************************************************/
		// Heroism is working now without any fix required (2/23/2009, rev 2255)
		// The old fix made it not function as described (target raid)

		/**********************************************************
		 *	Lightning Overload
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 30675 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 39805;//proc something (we will override this)
			sp->procFlags = PROC_ON_SPELL_HIT;
		}
		sp = dbcSpell.LookupEntryForced( 30678 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 39805;//proc something (we will override this)
			sp->procFlags = PROC_ON_SPELL_HIT;
		}
		sp = dbcSpell.LookupEntryForced( 30679 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 39805;//proc something (we will override this)
			sp->procFlags = PROC_ON_SPELL_HIT;
		}
		sp = dbcSpell.LookupEntryForced( 30680 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 39805;//proc something (we will override this)
			sp->procFlags = PROC_ON_SPELL_HIT;
		}
		sp = dbcSpell.LookupEntryForced( 30681 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 39805;//proc something (we will override this)
			sp->procFlags = PROC_ON_SPELL_HIT;
		}
		/**********************************************************
		 *	Purge
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 370 );
		if( sp != NULL )
			sp->DispelType = DISPEL_MAGIC;
		sp = dbcSpell.LookupEntryForced( 8012 );
		if( sp != NULL )
			sp->DispelType = DISPEL_MAGIC;
		sp = dbcSpell.LookupEntryForced( 27626 );
		if( sp != NULL )
			sp->DispelType = DISPEL_MAGIC;
		sp = dbcSpell.LookupEntryForced( 33625 );
		if( sp != NULL )
			sp->DispelType = DISPEL_MAGIC;

		/**********************************************************
		 *	Eye of the Storm
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 29062 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM;
		sp = dbcSpell.LookupEntryForced( 29064 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM;
		sp = dbcSpell.LookupEntryForced( 29065 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM;

		//Shaman - Shamanistic Focus
		// needs to be fixed (doesn't need to proc, it now just reduces mana cost always by %)
		sp = dbcSpell.LookupEntryForced( 43338 );
		if( sp != NULL )
		{
			sp->EffectTriggerSpell[0] = 43339;
		}

		sp = dbcSpell.LookupEntryForced( 43339 );
		if( sp != NULL )
		{
			sp->EffectMiscValue[0] = SMT_COST;
		}

		//shaman - Improved Chain Heal
		sp = dbcSpell.LookupEntryForced( 30873 );
		if( sp != NULL ){
			sp->EffectDieSides[0] = 0;
		}
		sp = dbcSpell.LookupEntryForced( 30872 );
		if( sp != NULL ){
			sp->EffectDieSides[0] = 0;
		}

		// Flametongue Weapon - 10% spd coefficient
		sp = dbcSpell.LookupEntryForced( 29469 );
		if( sp != NULL )
			sp->fixed_dddhcoef = 0.1f;

		// Flametongue Totem - 0% spd coefficient
		sp = dbcSpell.LookupEntryForced( 16368 );
		if( sp != NULL )
			sp->fixed_dddhcoef = 0.0f;

		//shaman - Improved Weapon Totems
		sp = dbcSpell.LookupEntryForced( 29193 );
		if( sp != NULL ){
			sp->EffectApplyAuraName[0]=SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectApplyAuraName[1]=SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectMiscValue[0] = SMT_MISC_EFFECT;
			sp->EffectMiscValue[1] = SMT_MISC_EFFECT;
		}
		sp = dbcSpell.LookupEntryForced( 29192 );
		if( sp != NULL ){
			sp->EffectApplyAuraName[0]=SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectApplyAuraName[1]=SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectMiscValue[0] = SMT_MISC_EFFECT;
			sp->EffectMiscValue[1] = SMT_MISC_EFFECT;
		}

		// Shaman - Improved Fire Totems
		sp = dbcSpell.LookupEntryForced( 16544 );
		if( sp != NULL ){
			sp->EffectMiscValue[0] = SMT_DURATION;
		}
		sp = dbcSpell.LookupEntryForced( 16086 );
		if( sp != NULL ){
			sp->EffectMiscValue[0] = SMT_DURATION;
		}

		// Shaman Arena totems fix
		// Totem of Survival
		sp = dbcSpell.LookupEntryForced( 46097 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 43860 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 43861 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 43862 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}

		// Totem of Indomitability
		sp = dbcSpell.LookupEntryForced( 43859 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 46096 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 43857 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 43858 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}

		// Totem of Third WInd
		sp = dbcSpell.LookupEntryForced( 46098 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 34138 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 42370 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 43728 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}

		//shaman - Elemental Weapons
		sp = dbcSpell.LookupEntryForced( 29080 );
		if( sp != NULL ){
			sp->EffectMiscValue[1] = SMT_DAMAGE_DONE;
			sp->EffectMiscValue[2] = SMT_DAMAGE_DONE;
		}
		sp = dbcSpell.LookupEntryForced( 29079 );
		if( sp != NULL ){
			sp->EffectMiscValue[1] = SMT_DAMAGE_DONE;
			sp->EffectMiscValue[2] = SMT_DAMAGE_DONE;
		}
		sp = dbcSpell.LookupEntryForced( 16266 );
		if( sp != NULL ){
			sp->EffectMiscValue[1] = SMT_DAMAGE_DONE;
			sp->EffectMiscValue[2] = SMT_DAMAGE_DONE;
		}

		// Magma Totem - 0% spd coefficient
		sp = dbcSpell.LookupEntryForced( 25550 );
		if( sp != NULL )
			sp->fixed_dddhcoef = 0.0f;
		sp = dbcSpell.LookupEntryForced( 10581 );
		if( sp != NULL )
			sp->fixed_dddhcoef = 0.0f;
		sp = dbcSpell.LookupEntryForced( 10580 );
		if( sp != NULL )
			sp->fixed_dddhcoef = 0.0f;
		sp = dbcSpell.LookupEntryForced( 10579 );
		if( sp != NULL )
			sp->fixed_dddhcoef = 0.0f;
		sp = dbcSpell.LookupEntryForced( 8187 );
		if( sp != NULL )
			sp->fixed_dddhcoef = 0.0f;

		//Tidal Waves
		sp = dbcSpell.LookupEntryForced( 51562 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 20;
		}
		sp = dbcSpell.LookupEntryForced( 51563 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 20;
		}
		sp = dbcSpell.LookupEntryForced( 51564 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 20;
		}
		sp = dbcSpell.LookupEntryForced( 51565 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 20;
		}
		sp = dbcSpell.LookupEntryForced( 51566 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 20;
		}

		//Earthliving Weapon
		sp = dbcSpell.LookupEntryForced( 51940 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 20;
		}
		sp = dbcSpell.LookupEntryForced( 51989 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 20;
		}
		sp = dbcSpell.LookupEntryForced( 52004 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 20;
		}
		sp = dbcSpell.LookupEntryForced( 52005 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 20;
		}
		sp = dbcSpell.LookupEntryForced( 52007 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 20;
		}
		sp = dbcSpell.LookupEntryForced( 52008 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 20;
		}

		//Maelstrom Weapon
		sp = dbcSpell.LookupEntryForced( 51528 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK;
			sp->proc_interval = 24000;
		}
		sp = dbcSpell.LookupEntryForced( 51529 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK;
			sp->proc_interval = 12000;
		}
		sp = dbcSpell.LookupEntryForced( 51530 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK;
			sp->proc_interval = 8000;
		}
		sp = dbcSpell.LookupEntryForced( 51531 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK;
			sp->proc_interval = 6000;
		}
		sp = dbcSpell.LookupEntryForced( 51532 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK;
			sp->proc_interval = 5000;
		}
		sp = dbcSpell.LookupEntryForced( 53817 );
		if( sp != NULL )
			sp->procCharges = 0;

		/**********************************************************
		 *	Healing Way
		 **********************************************************/
		/*
		Zack : disabled this to not create confusion that it is working. Burlex deleted code so it needs to be reverted in order to work
		sp = dbcSpell.LookupEntryForced( 29202 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
            sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
            sp->EffectImplicitTargetA[0] = EFF_TARGET_SCRIPTED_OR_SINGLE_TARGET;
			sp->procChance = sp->EffectBasePoints[0] + 1;
		}
		sp = dbcSpell.LookupEntryForced( 29205 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
            sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
            sp->EffectImplicitTargetA[0] = EFF_TARGET_SCRIPTED_OR_SINGLE_TARGET;
			sp->procChance = sp->EffectBasePoints[0] + 1;
		}
		sp = dbcSpell.LookupEntryForced( 29206 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
            sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
            sp->EffectImplicitTargetA[0] = EFF_TARGET_SCRIPTED_OR_SINGLE_TARGET;
			sp->procChance = sp->EffectBasePoints[0] + 1;
		}
		*/
		/*********************************************************
		 * Unleashed Rage - LordLeeCH
		 *********************************************************/
		sp = dbcSpell.LookupEntryForced( 30802 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AREA_AURA;
		}
		sp = dbcSpell.LookupEntryForced( 30808 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AREA_AURA;
		}
		sp = dbcSpell.LookupEntryForced( 30809 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AREA_AURA;
		}
		sp = dbcSpell.LookupEntryForced( 30810 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AREA_AURA;
		}

		sp = dbcSpell.LookupEntryForced( 30811 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AREA_AURA;
		}


		/**********************************************************
		 *	Elemental Devastation
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 29179 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
		sp = dbcSpell.LookupEntryForced( 29180 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
		sp = dbcSpell.LookupEntryForced( 30160 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;

		/**********************************************************
		 *	Ancestral healing
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 16176 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
		sp = dbcSpell.LookupEntryForced( 16235 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
		sp = dbcSpell.LookupEntryForced( 16240 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;

		/**********************************************************
		 *	Ancestral healing proc spell
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 16177 );
		if( sp != NULL )
			sp->rangeIndex = 4;
		sp = dbcSpell.LookupEntryForced( 16236 );
		if( sp != NULL )
			sp->rangeIndex = 4;
		sp = dbcSpell.LookupEntryForced( 16237 );
		if( sp != NULL )
			sp->rangeIndex = 4;

		sp = dbcSpell.LookupEntryForced( 20608 ); //Reincarnation
		if( sp != NULL )
		{
			for(uint32 i=0;i<8;i++)
			{
				if(sp->Reagent[i])
				{
					sp->Reagent[i] = 0;
					sp->ReagentCount[i] = 0;
				}
			}
		}

		//////////////////////////////////////////
		// SHAMAN WRATH OF AIR TOTEM			//
		//////////////////////////////////////////
		sp = dbcSpell.LookupEntryForced( 2895 );
		if( sp != NULL )
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SELF;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_SELF;
			sp->EffectImplicitTargetA[2] = 0;
			sp->EffectImplicitTargetB[0] = 0;
			sp->EffectImplicitTargetB[1] = 0;
			sp->EffectImplicitTargetB[2] = 0;
		}

	//////////////////////////////////////////
	// MAGE									//
	//////////////////////////////////////////

	// Insert mage spell fixes here

		//Missile Barrage
		sp = dbcSpell.LookupEntryForced( 44404 );
        if( sp != NULL )
        {
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 4;
        }
		sp = dbcSpell.LookupEntryForced( 54486 );
        if( sp != NULL )
        {
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 8;
        }
		sp = dbcSpell.LookupEntryForced( 54488 );
        if( sp != NULL )
        {
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 12;
        }
		sp = dbcSpell.LookupEntryForced( 54489 );
        if( sp != NULL )
        {
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 16;
        }
		sp = dbcSpell.LookupEntryForced( 54490 );
        if( sp != NULL )
        {
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 20;
        }

		//mage - Hot Streak
		sp = dbcSpell.LookupEntryForced( 44445 );
		if( sp != NULL )
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->Effect[1]=0;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 48108;
			sp->procChance= 33;
			sp->procFlags = PROC_ON_SPELL_HIT | PROC_ON_SPELL_CRIT_HIT;
			sp->procCharges = 0;
		}

		sp = dbcSpell.LookupEntryForced( 44446 );
		if( sp != NULL )
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->Effect[1]=0;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 48108;
			sp->procChance= 66;
			sp->procFlags = PROC_ON_SPELL_HIT | PROC_ON_SPELL_CRIT_HIT;
			sp->procCharges = 0;
		}

		sp = dbcSpell.LookupEntryForced( 44448 );
		if( sp != NULL )
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->Effect[1]=0;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 48108;
			sp->procChance= 100;
			sp->procFlags = PROC_ON_SPELL_HIT | PROC_ON_SPELL_CRIT_HIT;
			sp->procCharges = 0;
		}

		// Brain Freeze rank 1
		sp = dbcSpell.LookupEntryForced( 44546 );
		if(sp != NULL)
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;

		// Brain Freeze rank 2
		sp = dbcSpell.LookupEntryForced( 44548 );
		if(sp != NULL)
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;

		// Brain Freeze rank 3
		sp = dbcSpell.LookupEntryForced( 44549 );
		if(sp != NULL)
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;

		// Fingers of Frost rank 1
		sp = dbcSpell.LookupEntryForced( 44543 );
		if(sp != NULL)
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;


		// Fingers of Frost rank 2
		sp = dbcSpell.LookupEntryForced( 44545 );
		if(sp != NULL)
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;

		/**********************************************************
		*    Improved Blink by Alice
		**********************************************************/

		//Improved Blink - *Rank 1*
		sp = dbcSpell.LookupEntryForced( 31569 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPECIFIC_SPELL;
		}
			//Improved Blink - *Rank 2*
		sp = dbcSpell.LookupEntryForced( 31570 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPECIFIC_SPELL;
		}
		/**********************************************************
		 *	Arcane Concentration
		 **********************************************************/

		sp = dbcSpell.LookupEntryForced( 11213 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_SPELL_HIT | static_cast<uint32>(PROC_TARGET_SELF);
		sp = dbcSpell.LookupEntryForced( 12574 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_SPELL_HIT | static_cast<uint32>(PROC_TARGET_SELF);
		sp = dbcSpell.LookupEntryForced( 12575 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_SPELL_HIT | static_cast<uint32>(PROC_TARGET_SELF);
		sp = dbcSpell.LookupEntryForced( 12576 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_SPELL_HIT | static_cast<uint32>(PROC_TARGET_SELF);
		sp = dbcSpell.LookupEntryForced( 12577 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_SPELL_HIT | static_cast<uint32>(PROC_TARGET_SELF);

		//Mage - Arcane Concentration proc
		sp = dbcSpell.LookupEntryForced( 12536 );
		if( sp != NULL )
		{
			sp->BGR_one_buff_on_target = 0;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procCharges = 2;
		}

		//Mage - Wand Specialization. Not the forst thing we messed up. Blizz uses attack as magic and wandds as weapons :S
		sp = dbcSpell.LookupEntryForced( 6057 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectMiscValue[0] = SMT_MISC_EFFECT;
		}
		sp = dbcSpell.LookupEntryForced( 6085 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectMiscValue[0] = SMT_MISC_EFFECT;
		}
		//Mage - Spell Power
		sp = dbcSpell.LookupEntryForced( 35578 );
		if( sp != NULL )
		{
			sp->EffectMiscValue[0] = SMT_CRITICAL_DAMAGE;
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
		}
		sp = dbcSpell.LookupEntryForced( 35581 );
		if( sp != NULL )
		{
			sp->EffectMiscValue[0] = SMT_CRITICAL_DAMAGE;
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
		}

		//Mage - Elemental Precision
		sp = dbcSpell.LookupEntryForced( 29438 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectMiscValue[0] = SMT_COST;
		}
		sp = dbcSpell.LookupEntryForced( 29439 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectMiscValue[0] = SMT_COST;
		}
		sp = dbcSpell.LookupEntryForced( 29440 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectMiscValue[0] = SMT_COST;
		}

		//Mage - Arcane Blast
		sp = dbcSpell.LookupEntryForced( 30451 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1] = 36032;
			sp->procFlags = PROC_ON_CAST_SPECIFIC_SPELL;
			sp->ProcOnNameHash[1] = SPELL_HASH_ARCANE_BLAST;
		}

		// Updated ranks by Joker
		// Arcane Blast
		sp = dbcSpell.LookupEntryForced( 42894 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1] = 36032;
			sp->procFlags = PROC_ON_CAST_SPECIFIC_SPELL;
			sp->ProcOnNameHash[1] = SPELL_HASH_ARCANE_BLAST;
		}

		sp = dbcSpell.LookupEntryForced( 42896 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1] = 36032;
			sp->procFlags = PROC_ON_CAST_SPECIFIC_SPELL;
		}

		sp = dbcSpell.LookupEntryForced( 42897 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1] = 36032;
			sp->procFlags = PROC_ON_CAST_SPECIFIC_SPELL;
		}
	
		//mage - Arcane Blast Proc
		sp = dbcSpell.LookupEntryForced( 36032 );
		{
			sp->procFlags = 0;
			sp->maxstack = 3;
			sp->procCharges = 0;
			sp->c_is_flags |= SPELL_FLAG_IS_FORCEDDEBUFF;
		}

		//mage : Empowered Arcane Missiles
		//heh B thinks he is smart by adding this to description ? If it doesn't work std then it still needs to made by hand
		sp = dbcSpell.LookupEntryForced( 31579 );
		if( sp != NULL )
		{
			sp->EffectBasePoints[0] = 5 * (sp->EffectBasePoints[0] + 1); 
		}
		sp = dbcSpell.LookupEntryForced( 31582 );
		if( sp != NULL )
		{
			sp->EffectBasePoints[0] = 5 * (sp->EffectBasePoints[0] + 1);
		}
		sp = dbcSpell.LookupEntryForced( 31583 );
		if( sp != NULL )
		{
			sp->EffectBasePoints[0] = 5 * (sp->EffectBasePoints[0] + 1);
		}

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

		// cebernic: not for self?
		// impact
		sp = dbcSpell.LookupEntryForced( 12355 );
		if( sp != NULL )
		{
		  // passive rank: 11103, 12357, 12358 ,12359,12360 :D
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM | PROC_ON_SPELL_CRIT_HIT | PROC_ON_SPELL_HIT;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER;
			sp->EffectImplicitTargetB[0] = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER;
			sp->EffectImplicitTargetB[1] = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER;
			sp->EffectImplicitTargetA[2] = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER;
			sp->EffectImplicitTargetB[2] = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER;
			sp->c_is_flags |= SPELL_FLAG_IS_FORCEDDEBUFF;
		}

		//Mage - Invisibility
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
		}

		 //Invisibility triggered spell, should be removed on cast
		sp = dbcSpell.LookupEntryForced( 32612 );
		if( sp != NULL )
		{
			sp->AuraInterruptFlags |= AURA_INTERRUPT_ON_CAST_SPELL;
		}

		//Fingers of frost proc
		sp = dbcSpell.LookupEntryForced( 44544 );
		if( sp != NULL )
		{
			sp->procCharges = 2;
			sp->procFlags = PROC_ON_SPELL_HIT;
		}
 
		//Brain Freeze proc (Fireball!)
		sp = dbcSpell.LookupEntryForced( 57761 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procCharges = 1;
		}

		//Arcane Potency procs
		sp = dbcSpell.LookupEntryForced( 57529 );
		if( sp != NULL )
		{
			sp->procFlags = 0;
			sp->procCharges = 1;
			sp->RankNumber = 100;
			sp->AuraInterruptFlags = 0;
		}

		sp = dbcSpell.LookupEntryForced( 57531 );
		if( sp != NULL )
		{
			sp->procFlags = 0;
			sp->procCharges = 1;
			sp->RankNumber = 101;
			sp->AuraInterruptFlags = 0;
		}

		//Hot Streak proc
		sp = dbcSpell.LookupEntryForced( 48108 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procCharges = 1;
		}

		//Ice Lances
		sp = dbcSpell.LookupEntryForced( 42914 );
		if( sp != NULL )
			sp->Dspell_coef_override = 0.1429f;

		sp = dbcSpell.LookupEntryForced( 42913 );
		if( sp != NULL )
			sp->Dspell_coef_override = 0.1429f;

		sp = dbcSpell.LookupEntryForced( 30455 );
		if( sp != NULL )
			sp->Dspell_coef_override = 0.1429f;

		// Frostfire Bolts
		sp = dbcSpell.LookupEntryForced( 47610 );
		if( sp != NULL )
			sp->fixed_dddhcoef = 0.8571f;

		sp = dbcSpell.LookupEntryForced( 44614 );
		if( sp != NULL )
			sp->fixed_dddhcoef = 0.8571f;


		//mage - Combustion
		sp = dbcSpell.LookupEntryForced( 11129 );
		if( sp != NULL )
		{
			sp->Effect[0] = 0;
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1] = 28682;
			sp->procFlags = PROC_ON_SPELL_HIT | PROC_ON_SPELL_CRIT_HIT | static_cast<uint32>(PROC_TARGET_SELF);
			sp->procCharges = 0;
			sp->c_is_flags |= SPELL_FLAG_IS_REQUIRECOOLDOWNUPDATE;
		}

		//mage - Master of Elements
		sp = dbcSpell.LookupEntryForced( 29074 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 29077;
			sp->procFlags = uint32(PROC_ON_SPELL_CRIT_HIT|PROC_TARGET_SELF);
			sp->procChance = 100;
		}
		sp = dbcSpell.LookupEntryForced( 29075 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 29077;
			sp->procFlags = uint32(PROC_ON_SPELL_CRIT_HIT|PROC_TARGET_SELF);
			sp->procChance = 100;
		}
		sp = dbcSpell.LookupEntryForced( 29076 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 29077;
			sp->procFlags = uint32(PROC_ON_SPELL_CRIT_HIT|PROC_TARGET_SELF);
			sp->procChance = 100;
		}

		//mage: Blazing Speed
		sp = dbcSpell.LookupEntryForced( 31641 );
		if( sp != NULL )
			sp->EffectTriggerSpell[0] = 31643;
		sp = dbcSpell.LookupEntryForced( 31642 );
		if( sp != NULL )
			sp->EffectTriggerSpell[0] = 31643;

		//mage talent "frostbyte". we make it to be dummy
		sp = dbcSpell.LookupEntryForced( 11071 );
		if( sp != NULL )
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
		sp = dbcSpell.LookupEntryForced( 12496 );
		if( sp != NULL )
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
		sp = dbcSpell.LookupEntryForced( 12497 );
		if( sp != NULL )
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
		//Mage - Improved Scorch
		sp = dbcSpell.LookupEntryForced( 11095 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance =33;
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 12872 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance =66;
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 12873 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance =100;
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		// mage - Frost Warding
		sp = dbcSpell.LookupEntryForced( 11189 );
		if( sp != NULL )
		{
			sp->procChance = 10;
		}
		sp = dbcSpell.LookupEntryForced( 28332 );
		if( sp != NULL )
		{
			sp->procChance = 20;
		}

		// mage - Conjure Refreshment Table
		sp = dbcSpell.LookupEntryForced( 43985 );
		if ( sp != NULL )
			sp->EffectImplicitTargetA[0] = EFF_TARGET_DYNAMIC_OBJECT;

		// Hypothermia: undispellable
		sp = dbcSpell.LookupEntryForced( 41425 );
		if( sp != NULL )
			sp->c_is_flags = SPELL_FLAG_IS_FORCEDDEBUFF;

		// Mage - Permafrost Rank 1
		sp = dbcSpell.LookupEntryForced(11175);
		if(sp != NULL)
		{
			sp->EffectMiscValue[1] = SMT_MISC_EFFECT;
		}

		// Mage - Permafrost Rank 2
		sp = dbcSpell.LookupEntryForced(12569);
		if(sp != NULL)
		{
			sp->EffectMiscValue[1] = SMT_MISC_EFFECT;
		}

		// Mage - Permafrost Rank 3
		sp = dbcSpell.LookupEntryForced(12571);
		if(sp != NULL)
		{
			sp->EffectMiscValue[1] = SMT_MISC_EFFECT;
		}

		//Improved Counterspell rank 1
		sp = dbcSpell.LookupEntryForced( 11255 );
		if( sp!=NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPECIFIC_SPELL;
			sp->ProcOnNameHash[0] = SPELL_HASH_COUNTERSPELL;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 18469;
		}

		//Improved Counterspell rank 2
		sp = dbcSpell.LookupEntryForced( 12598 );
		if( sp!=NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPECIFIC_SPELL;
			sp->ProcOnNameHash[0] = SPELL_HASH_COUNTERSPELL;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 55021;
		}
	//////////////////////////////////////////
	// WARLOCK								//
	//////////////////////////////////////////

	// Insert warlock spell fixes here
		
		//megai2: Immolation Aura
		sp = dbcSpell.LookupEntryForced( 50589 );
		if (sp != NULL)
		{
			sp->ChannelInterruptFlags = 0; // Remove channeling behaviour.
		}

		//megai2: Everlasting Affliction
		sp = dbcSpell.LookupEntryForced( 47205 );
		if (sp != NULL)
		{
			sp->EffectSpellClassMask[1][0] = 0x111;
			sp->EffectSpellClassMask[1][1] = 0;
			sp->procFlags = PROC_ON_ANY_HOSTILE_ACTION;
			sp->procCharges = -1;
		}

		sp = dbcSpell.LookupEntryForced( 47204 );
		if (sp != NULL)
		{
			sp->EffectSpellClassMask[1][0] = 0x111;
			sp->EffectSpellClassMask[1][1] = 0;
			sp->procFlags = PROC_ON_ANY_HOSTILE_ACTION;
			sp->procCharges = -1;
		}

		sp = dbcSpell.LookupEntryForced( 47203 );
		if (sp != NULL)
		{
			sp->EffectSpellClassMask[1][0] = 0x111;
			sp->EffectSpellClassMask[1][1] = 0;
			sp->procFlags = PROC_ON_ANY_HOSTILE_ACTION;
			sp->procCharges = -1;
		}

		sp = dbcSpell.LookupEntryForced( 47202 );
		if (sp != NULL)
		{
			sp->EffectSpellClassMask[1][0] = 0x111;
			sp->EffectSpellClassMask[1][1] = 0;
			sp->procFlags = PROC_ON_ANY_HOSTILE_ACTION;
			sp->procCharges = -1;
		}

		sp = dbcSpell.LookupEntryForced( 47201 );
		if (sp != NULL)
		{
			sp->EffectSpellClassMask[1][0] = 0x111;
			sp->EffectSpellClassMask[1][1] = 0;
		}

		//Unstable Affliction
		sp = dbcSpell.LookupEntryForced( 31117 );
		if (sp != NULL)
			sp->c_is_flags |= SPELL_FLAG_IS_FORCEDDEBUFF;

		//warlock: Eradication
		sp = dbcSpell.LookupEntryForced( 47195 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_ANY_HOSTILE_ACTION;
			sp->procCharges = -1;
		}

		sp = dbcSpell.LookupEntryForced( 47196 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_ANY_HOSTILE_ACTION;
			sp->procCharges = -1;
		}

		sp = dbcSpell.LookupEntryForced( 47197 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_ANY_HOSTILE_ACTION;
			sp->procCharges = -1;
		}

		//Warlock Molten Core
		sp = dbcSpell.LookupEntryForced( 47245 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_ANY_HOSTILE_ACTION;
			sp->procCharges = -1;
		}

		sp = dbcSpell.LookupEntryForced( 47246 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_ANY_HOSTILE_ACTION;
			sp->procCharges = -1;
		}

		sp = dbcSpell.LookupEntryForced( 47247 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_ANY_HOSTILE_ACTION;
			sp->procCharges = -1;
		}

		/**********************************************************
		 *	Nether Protection
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 30299 );
		if (sp != NULL)
		{
			sp->procChance = 10;
			sp->proc_interval = 13000;
		}
		sp = dbcSpell.LookupEntryForced( 30301 );
		if (sp != NULL)
		{
			sp->procChance = 20;
			sp->proc_interval = 13000;
		}
		sp = dbcSpell.LookupEntryForced( 30302 );
		if (sp != NULL)
		{
			sp->procChance = 30;
			sp->proc_interval = 13000;
		}
		/**********************************************************
		 *	Backlash
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 34935 );
		if (sp != NULL)
		{
			sp->proc_interval = 8000;
			sp->procFlags |= PROC_ON_MELEE_ATTACK_VICTIM | static_cast<uint32>(PROC_TARGET_SELF);
		}
		sp = dbcSpell.LookupEntryForced( 34938 );
		if (sp != NULL)
		{
			sp->proc_interval = 8000;
			sp->procFlags |= PROC_ON_MELEE_ATTACK_VICTIM | static_cast<uint32>(PROC_TARGET_SELF);
		}
		sp = dbcSpell.LookupEntryForced( 34939 );
		if (sp != NULL)
		{
			sp->proc_interval = 8000;
			sp->procFlags |= PROC_ON_MELEE_ATTACK_VICTIM | static_cast<uint32>(PROC_TARGET_SELF);
		}
		sp = dbcSpell.LookupEntryForced( 34936 );
		if (sp != NULL)
		{
			sp->AuraInterruptFlags = AURA_INTERRUPT_ON_CAST_SPELL;
		}

		/**********************************************************
		 *	Demonic Knowledge
		 **********************************************************/
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

		//warlock -  Seed of Corruption
		sp = dbcSpell.LookupEntryForced( 27243 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1] = 27285;
			sp->procFlags = PROC_ON_SPELL_HIT_VICTIM | PROC_ON_DIE;
			sp->procChance = 100;
		}

		//warlock -  soul link
		sp = dbcSpell.LookupEntryForced( 19028 );
		if( sp != NULL )
		{
			//this is for the trigger effect
			sp->Effect[0]=SPELL_EFFECT_APPLY_AURA;
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

		//warlock: Nightfall
		sp = dbcSpell.LookupEntryForced( 18094 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 17941;
			sp->procFlags = PROC_ON_ANY_HOSTILE_ACTION | static_cast<uint32>(PROC_TARGET_SELF);
			sp->procChance = 2;
		}
		sp = dbcSpell.LookupEntryForced( 18095 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 17941;
			sp->procFlags = PROC_ON_ANY_HOSTILE_ACTION | static_cast<uint32>(PROC_TARGET_SELF);
			sp->procChance = 4;
		}
		//Shadow Trance should be removed on the first SB
		sp = dbcSpell.LookupEntryForced( 17941 );
		if( sp != NULL )
		{
			sp->AuraInterruptFlags = AURA_INTERRUPT_ON_CAST_SPELL;
		}

		//warlock: Empowered Corruption
		sp = dbcSpell.LookupEntryForced( 32381 );
		if( sp != NULL )
		{
			sp->EffectBasePoints[0] *= 6;
		}
		sp = dbcSpell.LookupEntryForced( 32382 );
		if( sp != NULL )
		{
			sp->EffectBasePoints[0] *= 6;
		}
		sp = dbcSpell.LookupEntryForced( 32383 );
		if( sp != NULL )
		{
			sp->EffectBasePoints[0] *= 6;
		}

		//warlock: Improved Enslave Demon
		sp = dbcSpell.LookupEntryForced( 18821 );
		if( sp != NULL )
		{
			sp->EffectMiscValue[0]=SMT_SPELL_VALUE_PCT;
			sp->EffectBasePoints[0] = -(sp->EffectBasePoints[0]+2);
		}

		//warlock - Demonic Sacrifice
		sp = dbcSpell.LookupEntryForced( 18789 );
		if( sp != NULL )
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_ON_PET;
		sp = dbcSpell.LookupEntryForced( 18790 );
		if( sp != NULL )
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_ON_PET;
		sp = dbcSpell.LookupEntryForced( 18791 );
		if( sp != NULL )
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_ON_PET;
		sp = dbcSpell.LookupEntryForced( 18792 );
		if( sp != NULL )
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_ON_PET;
		sp = dbcSpell.LookupEntryForced( 35701 );
		if( sp != NULL )
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_ON_PET;

		//warlock - Demonic Tactics
		sp = dbcSpell.LookupEntryForced( 30242 );
		if( sp != NULL )
		{
			sp->Effect[0] = 0; //disable this. This is just blizz crap. Pure proof that they suck :P
			sp->EffectImplicitTargetB[1] = EFF_TARGET_PET;
			sp->EffectApplyAuraName[2] = SPELL_AURA_MOD_SPELL_CRIT_CHANCE; //lvl 1 has it fucked up :O
			sp->EffectImplicitTargetB[2] = EFF_TARGET_PET;
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
		}
		sp = dbcSpell.LookupEntryForced( 30245 );
		if( sp != NULL )
		{
			sp->Effect[0] = 0; //disable this. This is just blizz crap. Pure proof that they suck :P
			sp->EffectImplicitTargetB[1] = EFF_TARGET_PET;
			sp->EffectImplicitTargetB[2] = EFF_TARGET_PET;
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
		}
		sp = dbcSpell.LookupEntryForced( 30246 );
		if( sp != NULL )
		{
			sp->Effect[0] = 0; //disable this. This is just blizz crap. Pure proof that they suck :P
			sp->EffectImplicitTargetB[1] = EFF_TARGET_PET;
			sp->EffectImplicitTargetB[2] = EFF_TARGET_PET;
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
		}
		sp = dbcSpell.LookupEntryForced( 30247 );
		if( sp != NULL )
		{
			sp->Effect[0] = 0; //disable this. This is just blizz crap. Pure proof that they suck :P
			sp->EffectImplicitTargetB[1] = EFF_TARGET_PET;
			sp->EffectImplicitTargetB[2] = EFF_TARGET_PET;
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
		}
		sp = dbcSpell.LookupEntryForced( 30248 );
		if( sp != NULL )
		{
			sp->Effect[0] = 0; //disable this. This is just blizz crap. Pure proof that they suck :P
			sp->EffectImplicitTargetB[1] = EFF_TARGET_PET;
			sp->EffectImplicitTargetB[2] = EFF_TARGET_PET;
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
		}

		//warlock - Demonic Resilience
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

		//warlock - Improved Imp
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

		//warlock - Improved Voidwalker
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

		//warlock - Improved Succubus
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

		//warlock - Fel Intellect
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

		//warlock - Fel Stamina
		sp = dbcSpell.LookupEntryForced( 18748 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_PERCENT_STAT;
			sp->EffectMiscValue[0] = 2;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}
		sp = dbcSpell.LookupEntryForced( 18749 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_PERCENT_STAT;
			sp->EffectMiscValue[0] = 2;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}
		sp = dbcSpell.LookupEntryForced( 18750 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_PERCENT_STAT;
			sp->EffectMiscValue[0] = 2;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}

		//warlock - Demonic Tactics
		sp = dbcSpell.LookupEntryForced( 30242 );
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

		//warlock - Unholy Power
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

		//warlock - Master Demonologist - 25 spells here
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
		//and the rest
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

		//warlock - Improved Drain Soul
		sp = dbcSpell.LookupEntryForced( 18213 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_TARGET_DIE | static_cast<uint32>(PROC_TARGET_SELF);
			sp->procChance = 100;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 18371;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SELF;
			sp->Effect[2] = 0 ; //remove this effect
		}
		sp = dbcSpell.LookupEntryForced( 18372 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_TARGET_DIE | static_cast<uint32>(PROC_TARGET_SELF);
			sp->procChance = 100;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 18371;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SELF;
			sp->Effect[2] = 0 ; //remove this effect
		}

		//warlock - Shadow Embrace
		sp = dbcSpell.LookupEntryForced( 32385 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->Effect[1] = 0 ; //remove this effect ? Maybe remove the other one :P xD
		}
		sp = dbcSpell.LookupEntryForced( 32387 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->Effect[1] = 0 ; //remove this effect ? Maybe remove the other one :P xD
		}
		sp = dbcSpell.LookupEntryForced( 32392 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->Effect[1] = 0 ; //remove this effect ? Maybe remove the other one :P xD
		}
		sp = dbcSpell.LookupEntryForced( 32393 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->Effect[1] = 0 ; //remove this effect ? Maybe remove the other one :P xD
		}
		sp = dbcSpell.LookupEntryForced( 32394 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->Effect[1] = 0 ; //remove this effect ? Maybe remove the other one :P xD
		}

		//warlock - soul leech
		sp = dbcSpell.LookupEntryForced( 30293 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6; //aura
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 30294;
			sp->procFlags = uint32(PROC_ON_CAST_SPELL|PROC_TARGET_SELF);
		}
		sp = dbcSpell.LookupEntryForced( 30295 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6; //aura
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 30294;
			sp->procFlags = uint32(PROC_ON_CAST_SPELL|PROC_TARGET_SELF);
		}
		sp = dbcSpell.LookupEntryForced( 30296 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6; //aura
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 30294;
			sp->procFlags = uint32(PROC_ON_CAST_SPELL|PROC_TARGET_SELF);
		}

		//warlock - Pyroclasm
		sp = dbcSpell.LookupEntryForced( 18073 );
		if( sp != NULL )
		{
			sp->Effect[0] = 0; //delete this override effect :P
			sp->EffectTriggerSpell[1] = 18093; //trigger spell was wrong :P
			sp->procFlags = PROC_ON_ANY_HOSTILE_ACTION;
			sp->procChance = 13; //god, save us from fixed values !
		}
		sp = dbcSpell.LookupEntryForced( 18096 );
		if( sp != NULL )
		{
			sp->Effect[0] = 0; //delete this override effect :P
			sp->EffectTriggerSpell[1] = 18093; //trigger spell was wrong :P
			sp->procFlags = PROC_ON_ANY_HOSTILE_ACTION;
			sp->procChance = 26; //god, save us from fixed values !
		}
		//Warlock Chaos bolt
		sp = dbcSpell.LookupEntryForced( 50796 );
		if( sp != NULL )
		{
			sp->Attributes |= ATTRIBUTES_IGNORE_INVULNERABILITY;
			sp->School = 2;
		}

		sp = dbcSpell.LookupEntryForced( 59170 );
		if( sp != NULL )
		{
			sp->Attributes |= ATTRIBUTES_IGNORE_INVULNERABILITY;
			sp->School = 2;
		}

		sp = dbcSpell.LookupEntryForced( 59171 );
		if( sp != NULL )
		{
			sp->Attributes |= ATTRIBUTES_IGNORE_INVULNERABILITY;
			sp->School = 2;
		}

		sp = dbcSpell.LookupEntryForced( 59172 );
		if( sp != NULL )
		{
			sp->Attributes |= ATTRIBUTES_IGNORE_INVULNERABILITY;
			sp->School = 2;
		}
		// End Warlock chaos bolt

		//Warlock Healthstones
		int HealthStoneID[8]={6201,6202,5699,11729,11730,27230,47871,47878};
		for(int i=0;i<8;i++)
		{
			sp = dbcSpell.LookupEntryForced( HealthStoneID[i] );
			if( sp != NULL )
			{
				sp->Reagent[1] = NULL;
			}
		}

		//Backdraft Rank 1 
		sp = dbcSpell.LookupEntryForced(47258);
		if(sp != NULL)
		{
			sp->procFlags = PROC_TARGET_SELF | PROC_ON_ANY_HOSTILE_ACTION;
			sp->procChance = 100;
		}	

		//Backdraft Rank 2
		sp = dbcSpell.LookupEntryForced(47259);
		if(sp != NULL)
		{
			sp->procFlags = PROC_TARGET_SELF | PROC_ON_ANY_HOSTILE_ACTION;
			sp->procChance = 100;
		}

		//Backdraft Rank 3
		sp = dbcSpell.LookupEntryForced(47260);
		if(sp != NULL)
		{
			sp->procFlags = PROC_TARGET_SELF | PROC_ON_ANY_HOSTILE_ACTION;
			sp->procChance = 100;
		}	

	//////////////////////////////////////////
	// DRUID								//
	//////////////////////////////////////////
			
	// Insert druid spell fixes here
		
		// Spell 22570 (Maim Rank 1)
		sp = dbcSpell.LookupEntryForced(22570);
		if( sp != NULL )
		{
			sp->AuraInterruptFlags = AURA_INTERRUPT_ON_UNUSED2;
			sp->is_melee_spell = true;
		}
		// Spell 22570 (Maim Rank 2)
		sp = dbcSpell.LookupEntryForced(49802);
		if( sp != NULL )
		{
			sp->AuraInterruptFlags = AURA_INTERRUPT_ON_UNUSED2;
			sp->is_melee_spell = true;
		}
		sp = dbcSpell.LookupEntryForced(49376); //feral charge cat
		if( sp != NULL )
		{
			sp->Effect[1] = SPELL_EFFECT_CHARGE; //hackfix (meant to use trigger missile which isn't implemented)
			sp->EffectImplicitTargetA[1] = EFF_TARGET_SINGLE_ENEMY;
			sp->Effect[2] = NULL;
		}
		sp = dbcSpell.LookupEntryForced(20719); //feline grace
		if( sp != NULL )
			sp->Effect[0] = NULL;

		//Druid: Feral Swiftness
		sp = dbcSpell.LookupEntryForced( 17002 );
		if ( sp != NULL )
		{
			sp->Effect[1] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1] = 24867;
		}
		sp = dbcSpell.LookupEntryForced( 24866 );
		if ( sp != NULL )
		{
			sp->Effect[1] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1] = 24864;
		}

		//Druid: Natural Perfection
		sp = dbcSpell.LookupEntryForced( 33881 );
		if ( sp != NULL )
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM | PROC_ON_SPELL_CRIT_HIT_VICTIM | PROC_ON_RANGED_CRIT_ATTACK_VICTIM;
		sp = dbcSpell.LookupEntryForced( 33882 );
		if ( sp != NULL )
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM | PROC_ON_SPELL_CRIT_HIT_VICTIM | PROC_ON_RANGED_CRIT_ATTACK_VICTIM;
		sp = dbcSpell.LookupEntryForced( 33883 );
		if ( sp != NULL )
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM | PROC_ON_SPELL_CRIT_HIT_VICTIM | PROC_ON_RANGED_CRIT_ATTACK_VICTIM;

		//Druid: Frenzied Regeneration
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

		//Druid - Celestial Focus
		sp = dbcSpell.LookupEntryForced( 16850 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 16923 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 16924 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}

		//Druid: Leader of the Pack
		sp = dbcSpell.LookupEntryForced( 24932 );
		if( sp != NULL )
		{
			sp->Effect[1] = 0;
			sp->Effect[2] = 0; //removing strange effects.
		}

		//Druid: Improved Leader of the Pack
		sp = dbcSpell.LookupEntryForced( 34297 );
		if( sp != NULL )
			sp->proc_interval = 6000;//6 secs
		//Druid: Improved Leader of the Pack Rank 2
		sp = dbcSpell.LookupEntryForced( 34300 );
		if( sp != NULL )
			sp->proc_interval = 6000;//6 secs

		//druid - Primal Fury (talent)
		sp = dbcSpell.LookupEntryForced( 37116 );
		if( sp != NULL )
			sp->RequiredShapeShift = 0;

		sp = dbcSpell.LookupEntryForced( 37117 );
		if( sp != NULL )
			sp->RequiredShapeShift = 0;

		//druid - Blood Frenzy (proc)
		sp = dbcSpell.LookupEntryForced( 16954 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CRIT_ATTACK;

		sp = dbcSpell.LookupEntryForced( 16952 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CRIT_ATTACK;

		//druid - Primal Fury (proc)
		sp = dbcSpell.LookupEntryForced( 16961 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CRIT_ATTACK;

		sp = dbcSpell.LookupEntryForced( 16958 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CRIT_ATTACK;

		//druid - Intensity
		sp = dbcSpell.LookupEntryForced( 17106 );
		if( sp != NULL )
		{
		   sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
		   sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 17107 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			 sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 17108 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		//Nature's Grasp
		sp = dbcSpell.LookupEntryForced( 16689 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 339;
			sp->procChance = 100;
			sp->Effect[1] = 0;
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM | PROC_REMOVEONUSE;
			sp->AuraInterruptFlags = 0; //we remove it on proc or timeout
		}
		sp = dbcSpell.LookupEntryForced( 16810 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 1062;
			sp->Effect[1] = 0;
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM | PROC_REMOVEONUSE;
			sp->AuraInterruptFlags = 0; //we remove it on proc or timeout
			sp->procChance = 100;
		}
		sp = dbcSpell.LookupEntryForced( 16811 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 5195;
			sp->Effect[1] = 0;
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM | PROC_REMOVEONUSE;
			sp->AuraInterruptFlags = 0; //we remove it on proc or timeout
			sp->procChance = 100;
		}
		sp = dbcSpell.LookupEntryForced( 16812 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 5196;
			sp->Effect[1] = 0;
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM | PROC_REMOVEONUSE;
			sp->AuraInterruptFlags = 0; //we remove it on proc or timeout
			sp->procChance = 100;
		}
		sp = dbcSpell.LookupEntryForced( 16813 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 9852;
			sp->Effect[1] = 0;
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM | PROC_REMOVEONUSE;
			sp->AuraInterruptFlags = 0; //we remove it on proc or timeout
			sp->procChance = 100;
		}
		sp = dbcSpell.LookupEntryForced( 17329 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 9853;
			sp->Effect[1] = 0;
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM | PROC_REMOVEONUSE;
			sp->AuraInterruptFlags = 0; //we remove it on proc or timeout
			sp->procChance = 100;
		}
		sp = dbcSpell.LookupEntryForced( 27009 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 26989;
			sp->Effect[1] = 0;
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM | PROC_REMOVEONUSE;
			sp->AuraInterruptFlags = 0; //we remove it on proc or timeout
			sp->procChance = 100;
		}

		//rake
	 /* sp = dbcSpell.LookupEntryForced( 1822 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 1823 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 1824 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 9904 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 27003 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 48573 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 48574 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;

		//lacerate
		sp = dbcSpell.LookupEntryForced( 33745 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 48567 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 48568 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING; */

		//Druid: Natural Shapeshifter
		sp = dbcSpell.LookupEntryForced( 16833 );
		if( sp != NULL )
			sp->DurationIndex = 0;
		sp = dbcSpell.LookupEntryForced( 16834 );
		if( sp != NULL )
			sp->DurationIndex = 0;
		sp = dbcSpell.LookupEntryForced( 16835 );
		if( sp != NULL )
			sp->DurationIndex = 0;

		// druid - Naturalist
		sp = dbcSpell.LookupEntryForced( 17069 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
			sp->EffectMiscValue[1] = 1;
		}
		sp = dbcSpell.LookupEntryForced( 17070 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
			sp->EffectMiscValue[1] = 1;
		}
		sp = dbcSpell.LookupEntryForced( 17071 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
			sp->EffectMiscValue[1] = 1;
		}
		sp = dbcSpell.LookupEntryForced( 17072 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
			sp->EffectMiscValue[1] = 1;
		}
		sp = dbcSpell.LookupEntryForced( 17073 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
			sp->EffectMiscValue[1] = 1;
		}

		// druid - Nature's Grace
		sp = dbcSpell.LookupEntryForced( 16880 );
		if( sp != NULL )
		{
            sp->procFlags = PROC_ON_SPELL_CRIT_HIT | static_cast<uint32>(PROC_TARGET_SELF);
            sp->procChance = 100;
            sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
            sp->EffectTriggerSpell[0] = 16886;
            sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
		}

		sp = dbcSpell.LookupEntryForced( 16886 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
            sp->procChance = 100;
			sp->procCharges = 2; // I know.. hacky.. but first charge gets lost when it gets proceed
		}

		// Druid: Omen of Clarity
		sp = dbcSpell.LookupEntryForced( 16864 );
		if( sp != NULL )
		{
			sp->procChance=6; //procchance dynamic. 3ppm
			sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_ON_CRIT_ATTACK;
		}
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

	//////////////////////////////////////////
	// ITEMS								//
	//////////////////////////////////////////

	// Insert items spell fixes here

	#ifdef NEW_PROCFLAGS

		//Bonescythe Armor
		sp = dbcSpell.LookupEntryForced( 28814 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation[0]=8519680;

		//Tome of the Lightbringer
		sp = dbcSpell.LookupEntryForced( 41042 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation[0]=8388608;
		//Gladiator's Libram of Fortitude
		sp = dbcSpell.LookupEntryForced( 43850 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation[0]=8388608;
		//Vengeful Gladiator's Libram of Fortitude
		sp = dbcSpell.LookupEntryForced( 43852 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation[0]=8388608;
		//Merciless Gladiator's Libram of Fortitude
		sp = dbcSpell.LookupEntryForced( 43851 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation[0]=8388608;
		//Gladiator's Libram of Vengeance
		sp = dbcSpell.LookupEntryForced( 43854 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation_high[0]=64;
		//Merciless Gladiator's Libram of Vengeance
		sp = dbcSpell.LookupEntryForced( 43855 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation_high[0]=64;
		//Vengeful Gladiator's Libram of Vengeance
		sp = dbcSpell.LookupEntryForced( 43856 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation_high[0]=64;
		//The Earthshatterer
		sp = dbcSpell.LookupEntryForced( 28821 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation[0]=1024;
		//Idol of the White Stag
		sp = dbcSpell.LookupEntryForced( 41037 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation_high[0]=64 | 1024;
		//Merciless Gladiator's Idol of Resolve
		sp = dbcSpell.LookupEntryForced( 43842 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation_high[0]=64 | 1024;
		//Vengeful Gladiator's Idol of Resolve
		sp = dbcSpell.LookupEntryForced( 43843 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation_high[0]=64 | 1024;
		//Merciless Gladiator's Idol of Steadfastness
		sp = dbcSpell.LookupEntryForced( 43844 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation[0]=2;
		//Vengeful Gladiator's Idol of Steadfastness
		sp = dbcSpell.LookupEntryForced( 43845 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation[0]=2;
		//Merciless Gladiator's Totem of Indomitability
		sp = dbcSpell.LookupEntryForced( 43858 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation_high[0]=16;
		//Vengeful Gladiator's Totem of Indomitability
		sp = dbcSpell.LookupEntryForced( 43859 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation_high[0]=16;
		//Gladiator's Totem of Indomitability
		sp = dbcSpell.LookupEntryForced( 43857 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation_high[0]=16;
		//Merciless Gladiator's Totem of Survival
		sp = dbcSpell.LookupEntryForced( 43861 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation[0]= 1048576 |268435456 | 2147483648;
		//Vengeful Gladiator's Totem of Survival
		sp = dbcSpell.LookupEntryForced( 43862 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation[0]= 1048576 |268435456 | 2147483648;
		//Gladiator's Totem of Survival
		sp = dbcSpell.LookupEntryForced( 43861 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation[0]= 1048576 |268435456 | 2147483648;
		//Wolfshead Helm
		sp = dbcSpell.LookupEntryForced( 17768 );
		if (sp != NULL)
		{
			sp->EffectSpellGroupRelation[0]= 1073741824;
			sp->EffectSpellGroupRelation[1]= 2147483648;
		}
		//Set: Plagueheart Raiment
		sp = dbcSpell.LookupEntryForced( 28831 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation[0]= 1;
		//Set: Gladiator's Idol of Resolve
		sp = dbcSpell.LookupEntryForced( 37191 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation_high[0]=64 | 1024;
		//Set: Gladiator's Idol of Steadfastness
		sp = dbcSpell.LookupEntryForced( 43841 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation[0]=2;
		//Set: Incarnate Raiment
		sp = dbcSpell.LookupEntryForced( 37564 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation[0]=512;
		//Talon of Al'ar
		sp = dbcSpell.LookupEntryForced( 37507 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation[0]=2048;
		//Set: Crystalforge Armor
		sp = dbcSpell.LookupEntryForced( 37191 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation_high[0]=64;
		//Set: Redemption Armor
		sp = dbcSpell.LookupEntryForced( 28787 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation[0]=4096;
		//Idol of the Claw
		sp = dbcSpell.LookupEntryForced( 34323 );
		if( sp != NULL )
		{
			sp->Flags5 = FLAGS5_PROCCHANCE_COMBOBASED;
			sp->EffectSpellGroupRelation[0]=8388608;
			sp->EffectSpellGroupRelation_high[0]=128;
		}

	#endif
		//Compact Harvest Reaper
		sp = dbcSpell.LookupEntryForced( 4078 );
		if( sp != NULL )
		{
			sp->DurationIndex = 6;
		}

		//Graccu's Mince Meat Fruitcake
		sp = dbcSpell.LookupEntryForced(25990);
		if( sp != NULL )
		{
			sp->EffectAmplitude[1] = 1000;
		}

		//Extract Gas
		sp = dbcSpell.LookupEntryForced( 30427 );
		if( sp != NULL )
		{
			sp->Effect[0] = SPELL_EFFECT_DUMMY;
		}

		//Relic - Idol of the Unseen Moon
		sp = dbcSpell.LookupEntryForced( 43739 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 43740;
		}

		//Lunar Grace - Idol of the Unseen Moon proc
		sp = dbcSpell.LookupEntryForced( 43740 );
		if( sp != NULL )
		{
			sp->ProcOnNameHash[0] = SPELL_HASH_MOONFIRE;
		}

		//Relic - Idol of Terror
		sp = dbcSpell.LookupEntryForced( 43737 );
		if( sp != NULL )
		{
			sp->proc_interval = 10001; //block proc when is already active.. (Primal Instinct duration = 10 sec)
			sp->procFlags = PROC_ON_CAST_SPELL | static_cast<uint32>(PROC_TARGET_SELF);
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 43738;
			sp->procChance=85;
		}

		//Primal Instinct - Idol of Terror proc
		sp = dbcSpell.LookupEntryForced( 43738 );
		if( sp != NULL )
		{
			sp->self_cast_only = true;
			sp->ProcOnNameHash[0] = SPELL_HASH_MANGLE___CAT;
			sp->ProcOnNameHash[1] = SPELL_HASH_MANGLE___BEAR;
		}

		//Tome of Fiery Redemption
		sp = dbcSpell.LookupEntryForced( 37197 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 37198;
			sp->procChance = 15;
		}

		//Thunderfury
		sp = dbcSpell.LookupEntryForced( 21992 );
		if( sp != NULL )
		{
			sp->Effect[2] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[2] = 27648;
			sp->EffectImplicitTargetA[2] = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER; // cebernic: for enemies not self
		}

		//Energized
		sp = dbcSpell.LookupEntryForced( 43750 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL;

		//Spell Haste Trinket
		sp = dbcSpell.LookupEntryForced( 33297 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL | static_cast<uint32>(PROC_TARGET_SELF);

		

// #ifdef NEW_PROCFLAGS

// #endif
		//Spell Focus Trigger (Mystical Skyfire Diamond)
		sp = dbcSpell.LookupEntryForced( 32837 );
		if( sp != NULL )
			sp->procChance = 15;

		// Band of the Eternal Sage
		sp = dbcSpell.LookupEntryForced( 35083 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL;

		// Band of the Eternal Restorer
		sp = dbcSpell.LookupEntryForced( 35086 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL;

		// Ashtongue Talisman of Shadows
		sp = dbcSpell.LookupEntryForced( 40478 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL;

		// Ashtongue Talisman of Swiftness
		sp = dbcSpell.LookupEntryForced( 40485 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL;

		// Ashtongue Talisman of Valor
		sp = dbcSpell.LookupEntryForced( 40458 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL;

		// Memento of Tyrande
		sp = dbcSpell.LookupEntryForced( 37655 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 10;
		}

		// Ashtongue Talisman of Insight
		sp = dbcSpell.LookupEntryForced( 40482 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;

		//Ashtongue Talisman of Equilibrium
		sp = dbcSpell.LookupEntryForced( 40442 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance = 40;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectTriggerSpell[0] = 40452;
			sp->maxstack = 1;
			sp->Effect[1] = 6;
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance = 25;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectTriggerSpell[1] = 40445;
			sp->maxstack = 1;
			sp->Effect[2] = 6;
			sp->EffectApplyAuraName[2] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance = 25;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectTriggerSpell[2] = 40446;
			sp->maxstack = 1;
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
			sp->procChance = 10;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectTriggerSpell[1] = 40440;
			sp->maxstack = 1;
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
		// Symbol of Hope targets surrounding party members instead of us
		sp = dbcSpell.LookupEntryForced( 32548 );
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

		// Dragonspine Trophy
 		sp = dbcSpell.LookupEntryForced( 34774 );
		if( sp != NULL ){
			sp->procChance = 6;
			sp->procFlags  = PROC_ON_MELEE_ATTACK | PROC_ON_RANGED_ATTACK;
			sp->proc_interval = 30000;
		}
	#ifndef NEW_PROCFLAGS
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
	#else
		sp = dbcSpell.LookupEntryForced( 40460 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 262144 | 2097152 | 8388608 | 8519680 | 524288 | 1048576 | 8388608;
	#endif

		//Serpent-Coil Braid
		sp = dbcSpell.LookupEntryForced( 37447 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance = 100;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectTriggerSpell[0] = 37445;
			sp->maxstack = 1;
		}

		// Band of the Eternal Champion
		sp = dbcSpell.LookupEntryForced( 35080 );
		if( sp != NULL ){
			sp->procFlags  = PROC_ON_MELEE_ATTACK | PROC_ON_RANGED_ATTACK;
			sp->proc_interval = 60000;
		}
		// Band of the Eternal Sage
		sp = dbcSpell.LookupEntryForced( 35083 );
		if( sp != NULL ){
			sp->procFlags  = PROC_ON_CAST_SPELL;
			sp->proc_interval = 60000;
		}
		// Band of the Eternal Restorer
		sp = dbcSpell.LookupEntryForced( 35086 );
		if( sp != NULL ){
			sp->procFlags  = PROC_ON_CAST_SPELL;
			sp->proc_interval = 60000;
		}
		// Band of the Eternal Defender
		sp = dbcSpell.LookupEntryForced( 35077 );
		if( sp != NULL ){
			sp->procFlags  = PROC_ON_MELEE_ATTACK_VICTIM | PROC_ON_SPELL_HIT_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM;
			sp->proc_interval = 60000;
		}

		//Item Set: Malorne Harness
		sp = dbcSpell.LookupEntryForced( 37306 );
		if( sp != NULL )
		{
			sp->procChance = 4;
			sp->procFlags = PROC_ON_MELEE_ATTACK;
		}
		sp = dbcSpell.LookupEntryForced( 37311 );
		if( sp != NULL )
		{
			sp->procChance = 4;
			sp->procFlags = PROC_ON_MELEE_ATTACK;
		}

		//Item Set: Deathmantle
		sp = dbcSpell.LookupEntryForced( 37170 );
		if( sp != NULL )
		{
			sp->procChance = 4;
			sp->procFlags = PROC_ON_MELEE_ATTACK;
		}

		//Item Set: Netherblade
		sp = dbcSpell.LookupEntryForced( 37168 );
		if( sp != NULL )
		{
			sp->procChance = 15;
			//sp->procFlags = PROC_ON_CAST_SPELL; Need new flag - PROC_ON_FINISH_MOVE;
		}

		//Item Set: Tirisfal Regalia
		sp = dbcSpell.LookupEntryForced( 37443 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
		}

		//Item Set: Avatar Regalia
		sp = dbcSpell.LookupEntryForced( 37600 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 6;
		}

		//Item Set: Incarnate Raiment
		sp = dbcSpell.LookupEntryForced( 37568 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}

		//Item Set: Voidheart Raiment
		sp = dbcSpell.LookupEntryForced( 37377 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance = 5;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->proc_interval = 20;
			sp->EffectTriggerSpell[0] = 37379;
		}
		sp = dbcSpell.LookupEntryForced( 39437 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance = 5;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->proc_interval = 20;
			sp->EffectTriggerSpell[0] = 37378;
		}

		//Item Set: Cataclysm Raiment
		sp = dbcSpell.LookupEntryForced( 37227 );
		if( sp != NULL )
		{
			sp->proc_interval = 60000;
			sp->procChance = 100;
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
		}

		//Item Set: Cataclysm Regalia
		sp = dbcSpell.LookupEntryForced( 37228 );
		if( sp != NULL )
		{
			sp->procChance = 7;
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 37237 );
		if( sp != NULL )
		{
			sp->procChance = 25;
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
		}

		//Item Set: Cataclysm Harness
		sp = dbcSpell.LookupEntryForced( 37239 );
		if( sp != NULL )
		{
			sp->procChance = 2;
			sp->procFlags = PROC_ON_MELEE_ATTACK;
		}

		//Item Set: Cyclone Regalia
		sp = dbcSpell.LookupEntryForced( 37213 );
		if( sp != NULL )
		{
			sp->procChance = 11;
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
		}

		//Item Set: Lightbringer Battlegear
		sp = dbcSpell.LookupEntryForced( 38427 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK;
			sp->procChance = 20;
		}

		//Item Set: Crystalforge Armor
		sp = dbcSpell.LookupEntryForced( 37191 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}

		//Item Set: Crystalforge Battlegear
		sp = dbcSpell.LookupEntryForced( 37195 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 6;
		}

		//Item Set: Crystalforge Raiment
		sp = dbcSpell.LookupEntryForced( 37189 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
			sp->proc_interval = 60000;
		}
		sp = dbcSpell.LookupEntryForced( 37188 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}

		//Item Set: Destroyer Armor
		sp = dbcSpell.LookupEntryForced( 37525 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM;
			sp->procChance = 7;
		}

		//Item Set: Destroyer Battlegear
		sp = dbcSpell.LookupEntryForced( 37528 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 100;
		}

		//Item Set: Warbringer Armor
		sp = dbcSpell.LookupEntryForced( 37516 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 100;
		}

		//Item Set: Shadowcraft Armor & Darkmantle Armor
		sp = dbcSpell.LookupEntryForced( 27787 );
		if( sp != NULL)
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM;
			sp->procChance = 7;
		}

		// Item Set: Warlock Tier 7 Heroes' Plagueheart Garb
		sp = dbcSpell.LookupEntryForced(60172);
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[0] = 61082;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 100;
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

		//this an on equip item spell(2824) :  ice arrow(29501)
		sp = dbcSpell.LookupEntryForced( 29501 );
		if( sp != NULL )
		{
			sp->procChance = 30;//some say it is triggered every now and then
			sp->procFlags = PROC_ON_RANGED_ATTACK;
		}

		//Purify helboar meat
		sp = dbcSpell.LookupEntryForced( 29200 );
		if( sp != NULL )
		{
			sp->Reagent[1] = 0;
			sp->ReagentCount[1] = 0;
			sp->Effect[0] = 24;
		}

		// - Warrior - Warbringer Armor
		// 2 pieces: You have a chance each time you parry to gain Blade Turning, absorbing 200 damage for 15 sec.
		// SPELL ID = 37514 (http://www.wowhead.com/?spell=37514)

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

		//Adamantine Grenade
		sp = dbcSpell.LookupEntryForced(30217);
		if(sp != NULL)
		{
			sp->InterruptFlags |= ~(CAST_INTERRUPT_ON_MOVEMENT);
		}

		//Swordguard Embroidery
		sp = dbcSpell.LookupEntryForced(55776);
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[0] = 55775;
			sp->procFlags = PROC_ON_MELEE_ATTACK;
			sp->procChance = 25;
			sp->proc_interval = 60000;
		}

		//Lightweave Embroidery - this will work in 3.1
		/*sp = dbcSpell.LookupEntryForced(55640);
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[0] = 55637;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 50;
			sp->proc_interval = 45000;
		}*/

		//Darkglow Embroidery
		sp = dbcSpell.LookupEntryForced(55768);
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[0] = 55767;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 35;
			sp->proc_interval = 60000;
		}
		/**************************************************************
		* Trinket Fixes		Please keep nice and clean :)										  *
		**************************************************************/

		// Citrine Pendant of Golden Healing
		sp = dbcSpell.LookupEntryForced( 25608 );		//	http://www.wowhead.com/?item=20976
		if( sp != NULL )
		{	//Overrides any spell coefficient calculation - DBCStores.h
			sp->Dspell_coef_override = 0;	//DD&DH
			sp->OTspell_coef_override = 0;	//HOT&DOT
		}
		//Barricade of Eternity
		sp = dbcSpell.LookupEntryForced( 40475 );		//	http://www.wowhead.com/?item=40475
		if( sp != NULL )
			sp->procChance = 50;	// Sets change to proc

		//Figurine - Shadowsong Panther
		sp = dbcSpell.LookupEntryForced( 46784 );		//	http://www.wowhead.com/?item=35702
		if( sp != NULL )
			sp->AttributesEx |= ATTRIBUTESEX_NOT_BREAK_STEALTH;

		// Infernal Protection
		sp = dbcSpell.LookupEntryForced(36488);			//	http://www.wowhead.com/?spell=36488
		if( sp != NULL )
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SINGLE_FRIEND;

		// Band of the Eternal Champion: reduced proc rate
		sp = dbcSpell.LookupEntryForced( 35080 );
		if( sp != NULL )
			sp->procChance = 5;

		// Band of the Eternal Sage: reduced proc rate
		sp = dbcSpell.LookupEntryForced( 35083 );
		if( sp != NULL )
			sp->procChance = 5;

		// Band of the Eternal Defender: reduced proc rate
		sp = dbcSpell.LookupEntryForced( 35077 );
		if( sp != NULL )
			sp->procChance = 5;

		// Band of the Eternal Restorer: reduced proc rate
		sp = dbcSpell.LookupEntryForced( 35086 );
		if( sp != NULL )
			sp->procChance = 5;

		// Deadly Throw Interrupt
		sp = dbcSpell.LookupEntryForced( 32748 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_RANGED_ATTACK | PROC_ON_CAST_SPELL;
		}

		//Sundial of the Exiled
		sp = dbcSpell.LookupEntryForced(60063);
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[1] = 60064;
			sp->procFlags = PROC_ON_SPELL_HIT;
			sp->procChance = 10;
			sp->proc_interval = 45000;
		}

		//Je'Tze's Bell
		sp = dbcSpell.LookupEntryForced(49622);
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[1] = 49623;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 10;
			sp->proc_interval = 45000;
		}

		//Tears of Bitter Anguish
		sp = dbcSpell.LookupEntryForced(58901);
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[1]=58904;
			sp->procFlags = PROC_ON_RANGED_CRIT_ATTACK | PROC_ON_CRIT_ATTACK;
			sp->procChance = 10;
			sp->proc_interval = 60000;
		}

		//Embrace of the Spider
		sp = dbcSpell.LookupEntryForced(60490);
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[1] = 60492;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 10;
			sp->proc_interval = 30000;
		}

		//Dying Curse
		sp = dbcSpell.LookupEntryForced(60493);
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[1] = 60494;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 15;
			sp->proc_interval = 45000;
		}

		//Fury of the Five Flights
		sp = dbcSpell.LookupEntryForced(60313);
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[1] = 60314;
			sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_ON_RANGED_ATTACK;
			sp->procChance = 100;
			sp->maxstack = 20;
		}

		//Vial of the Sunwell
		sp = dbcSpell.LookupEntryForced( 45059 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL;

		//Pendant of the Violet Eye
		sp = dbcSpell.LookupEntryForced( 29601 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL | static_cast<uint32>(PROC_TARGET_SELF);
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 35095;
			sp->procChance = 100;
		}
		sp = dbcSpell.LookupEntryForced( 35095 );
		if( sp != NULL )
		{
			sp->self_cast_only = true;
			sp->procChance = 100;
		}
				
		sp = dbcSpell.LookupEntryForced( 38332 );		// Ribbon of Sacrifice
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL;

		sp = dbcSpell.LookupEntryForced( 40475 );		// Black temple melee trinket
		if( sp != NULL )
			sp->procChance = 5;

		sp = dbcSpell.LookupEntryForced( 32642 );		// Sporegarr - Petrified Lichen Guard
		if( sp != NULL )
		{	
			sp->procFlags = PROC_ON_BLOCK_VICTIM;
			sp->procChance = 100;
			sp->EffectTriggerSpell[0] = 32643;
		}

        //Flow of Knowledge
		sp = dbcSpell.LookupEntryForced ( 62114 );
        if(sp != NULL )
		{
			sp->EffectTriggerSpell[1] = 60064;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 10;
			sp->proc_interval = 45000;
		}

		//Majestic Dragon Figurine
		sp = dbcSpell.LookupEntryForced(60524);
		if(sp != NULL)
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->proc_interval = 100;
		}

		//Illustration of the Dragon Soul
		sp = dbcSpell.LookupEntryForced(60485);
		if(sp != NULL)
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->proc_interval = 100;
		}

		//////////////////////////////////////////
		// BOSSES								//
		//////////////////////////////////////////

		// Insert boss spell fixes here

		// Major Domo - Magic Reflection
		sp = dbcSpell.LookupEntryForced(20619);
		if(sp != NULL)
		{
			for(int i=0; i<3; i++)
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
			for(int i=0; i<3; i++)
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
		{
			sp->MaxTargets = 5;
		}

		//Acidic Wound
		sp = dbcSpell.LookupEntryForced( 40484 );
 		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK;
			sp->procChance = 100;
		}	

		//Inject Poison
		sp = dbcSpell.LookupEntryForced( 44599 );
 		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK;
			sp->procChance = 100;
		}
		sp = dbcSpell.LookupEntryForced( 46046 );
 		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK;
			sp->procChance = 100;
		}

		//Anthropy
		sp = dbcSpell.LookupEntryForced( 40327 );
 		if( sp != NULL )
		{
			sp->maxstack = 10;
		}

		//Doom
		sp = dbcSpell.LookupEntryForced( 31347 );
		if( sp != NULL )
		{	
			sp->MaxTargets = 1;
		}
		//Shadow of Death
		sp = dbcSpell.LookupEntryForced( 40251 );
		if( sp != NULL )
		{	
			sp->EffectApplyAuraName[0] = 23;
			sp->EffectTriggerSpell[0] = 0;
		}

		// Recently Dropped Flag
		sp = dbcSpell.LookupEntryForced( 42792 );
		if (sp != NULL)
			sp->c_is_flags |= SPELL_FLAG_IS_FORCEDDEBUFF;

		/*
		cebernic 's fixes
		*/
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

		sp = dbcSpell.LookupEntryForced ( 5246 ); // why self?
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 20511; // cebernic: this just real spell
			sp->EffectImplicitTargetA[0] = EFF_TARGET_NONE;
		}


		//Bandage
		sp = dbcSpell.LookupEntryForced( 11196 );
		if( sp != NULL )
			sp->c_is_flags = SPELL_FLAG_IS_FORCEDDEBUFF;

		//////////////////////////////////////////
		// DEATH KNIGHT							//
		//////////////////////////////////////////

		// Insert Death Knight spells here ---- Made by Alice

		/**********************************************************
		 *	Death and Decay - Ranks 1, 2, 3, 4
		 **********************************************************/
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
		/**********************************************************
		 *	Unholy Aura - Ranks 1 & 2
		 **********************************************************/
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

		/**********************************************************
		 *	Blood Presence
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 48266 );
		if( sp != NULL )
		{
			sp->BGR_one_buff_from_caster_on_self = SPELL_TYPE3_DEATH_KNIGHT_AURA;
		}

		/**********************************************************
		 *	Frost Presence
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 48263 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_BASE_RESISTANCE_PCT;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT;
			sp->EffectBasePoints[1] = 9;
			sp->EffectApplyAuraName[2] = SPELL_AURA_MOD_DAMAGE_TAKEN;
			sp->BGR_one_buff_from_caster_on_self = SPELL_TYPE3_DEATH_KNIGHT_AURA;
		}

		/**********************************************************
		 *	Unholy Presence
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 48265 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_HASTE;
			sp->EffectBasePoints[0] = 14;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_INCREASE_SPEED;
			sp->EffectBasePoints[1] = 14;
			sp->BGR_one_buff_from_caster_on_self = SPELL_TYPE3_DEATH_KNIGHT_AURA;
		}

		/**********************************************************
		 *	Death Grip
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 49576 );
		if( sp != NULL )
		{
			sp->Effect[0] = SPELL_EFFECT_DUMMY;
		}

		/**********************************************************
		 *	Acherus Deatcharger
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 48778 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOUNTED;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_INCREASE_MOUNTED_SPEED;
			sp->EffectBasePoints[1] = 99;
		}

		/**********************************************************
		 *	Path of Frost
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 3714 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_WATER_WALK;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
		}

		//Shadow of Death
		sp = dbcSpell.LookupEntryForced( 49157 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
		}

		//Rune Strike
		sp = dbcSpell.LookupEntryForced( 56815 );
		if( sp != NULL )
		{
			sp->Attributes = ATTRIBUTES_CANT_BE_DPB;
		}

		//Frost Strike
		sp = dbcSpell.LookupEntryForced( 49143 );
		if( sp != NULL )
		{
			sp->Attributes = ATTRIBUTES_CANT_BE_DPB;
		}
		sp = dbcSpell.LookupEntryForced( 51416 );
		if( sp != NULL )
		{
			sp->Attributes = ATTRIBUTES_CANT_BE_DPB;
		}
		sp = dbcSpell.LookupEntryForced( 51417 );
		if( sp != NULL )
		{
			sp->Attributes = ATTRIBUTES_CANT_BE_DPB;
		}
		sp = dbcSpell.LookupEntryForced( 51418 );
		if( sp != NULL )
		{
			sp->Attributes = ATTRIBUTES_CANT_BE_DPB;
		}
		sp = dbcSpell.LookupEntryForced( 51419 );
		if( sp != NULL )
		{
			sp->Attributes = ATTRIBUTES_CANT_BE_DPB;
		}
		sp = dbcSpell.LookupEntryForced( 55268 );
		if( sp != NULL )
		{
			sp->Attributes = ATTRIBUTES_CANT_BE_DPB;
		}

		// Blood Fury Healing Debuff
		sp = dbcSpell.LookupEntryForced( 23230 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_FORCEDDEBUFF;
		}

		// Noggenfogger elixir - reduce size effect
		sp = dbcSpell.LookupEntryForced( 16595 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_SCALE;
			sp->EffectBasePoints[0] = -50;
			sp->maxstack = 1;
      	}

		sp = dbcSpell.LookupEntryForced( 46585 );
		if( sp != NULL )
		{
		sp->Effect[0] = SPELL_EFFECT_SUMMON;
		sp->EffectMiscValue[0] = 26125;
		}

		sp = dbcSpell.LookupEntryForced( 51052 );
		if( sp != NULL )
		{
		sp->Effect[0] = SPELL_EFFECT_SUMMON;
		sp->EffectMiscValue[0] = 26125;
		}

		sp = dbcSpell.LookupEntryForced( 46584 );
		if( sp != NULL )
		{
		sp->Effect[0] = SPELL_EFFECT_DUMMY;
		sp->Effect[1] = SPELL_EFFECT_DUMMY;
		}

		//PvP Librams of Justice 
		//Gladiator's Libram of Justice
		sp = dbcSpell.LookupEntryForced( 34139 );
		if( sp != NULL )	
			sp->procFlags = PROC_ON_CAST_SPELL;

		//Merciless Gladiator's Libram of Justice
		sp = dbcSpell.LookupEntryForced( 42368 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL;

		//Vengeful Gladiator's Libram of Justice
		sp = dbcSpell.LookupEntryForced( 43726 );
		if( sp != NULL )	
			sp->procFlags = PROC_ON_CAST_SPELL;

		//Brutal Gladiator's Libram of Justice
		sp = dbcSpell.LookupEntryForced( 46092 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL;
		
		//Other Librams 
		//Libram of Saints Departed and Libram of Zeal
		sp = dbcSpell.LookupEntryForced( 34262 );
		if( sp != NULL )
		{	
			sp->procFlags = PROC_ON_CAST_SPELL | static_cast<uint32>(PROC_TARGET_SELF);
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 34263;	
			sp->procChance = 100;		
		}	
		sp = dbcSpell.LookupEntryForced( 34263 );
		if( sp != NULL )
		{	
			sp->self_cast_only = true;
			sp->ProcOnNameHash[0] = SPELL_HASH_JUDGEMENT;	
			sp->procChance = 100;		
		}

		//Libram of Avengement 
		sp = dbcSpell.LookupEntryForced( 34258 );
		if( sp != NULL )
		{	
			sp->procFlags = PROC_ON_CAST_SPELL | static_cast<uint32>(PROC_TARGET_SELF);
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 34260;	
			sp->procChance = 100;		
		}	
		sp = dbcSpell.LookupEntryForced( 34260 );
		if( sp != NULL )
		{	
			sp->self_cast_only = true;
			sp->ProcOnNameHash[0] = SPELL_HASH_JUDGEMENT;	
			sp->procChance = 100;		
		}

		//Libram of Mending
		sp = dbcSpell.LookupEntryForced( 43741 );
		if( sp != NULL )
		{	
			sp->procFlags = PROC_ON_CAST_SPELL | static_cast<uint32>(PROC_TARGET_SELF);
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 43742;	
			sp->procChance = 100;		
		}	
		sp = dbcSpell.LookupEntryForced( 43742 );
		if( sp != NULL )
		{	
			sp->self_cast_only = true;
			sp->ProcOnNameHash[0] = SPELL_HASH_HOLY_LIGHT;	
			sp->procChance = 100;		
		}

		//Libram of Divine Judgement
		sp = dbcSpell.LookupEntryForced( 43745 );
		if( sp != NULL )
		{	
			sp->procFlags = PROC_ON_CAST_SPELL | static_cast<uint32>(PROC_TARGET_SELF);
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 43747;	
			sp->procChance = 40;		
		}	
		sp = dbcSpell.LookupEntryForced( 43747 );
		if( sp != NULL )
		{	
			sp->self_cast_only = true;	
			sp->procChance = 100;		
		}

		//Stonebreaker's Totem
		sp = dbcSpell.LookupEntryForced( 43748 );
		if( sp != NULL )
		{	
			sp->procFlags = PROC_ON_CAST_SPELL | static_cast<uint32>(PROC_TARGET_SELF);
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 43749;	
			sp->procChance = 50;		
		}	
		sp = dbcSpell.LookupEntryForced( 43749 ); 
		if( sp != NULL )
		{	
			sp->self_cast_only = true;	
			sp->procChance = 100;	
		}

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
		{
			sp->proc_interval = 30000; // Wowhead Comment
			sp->procChance = 3; //Enchantment Text
		}

		sp = dbcSpell.LookupEntryForced( 24574 );		// Zandalarian Hero Badge 24590 24575
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

		//STEW's VEHICLE MOUNT TEMP FIXES
		//MECHANO HOG (Horde bike)
		sp = dbcSpell.LookupEntryForced( 55531 );
		if( sp != NULL )
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOUNTED;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_INCREASE_MOUNTED_SPEED;
			sp->EffectMiscValue[0] = 29929;
			sp->EffectMiscValueB[1] = 0;
			sp->EffectImplicitTargetA[0] = 1;
			sp->EffectImplicitTargetA[1] = 1;
			sp->EffectBasePoints[0] = 0;
			sp->EffectBasePoints[1] = 99;
		}
		//MEKGINEER'S CHOPPER (Alliance Bike)
		sp = dbcSpell.LookupEntryForced( 60424 );
		if( sp != NULL )
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOUNTED;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_INCREASE_MOUNTED_SPEED;
			sp->EffectMiscValue[0] = 32286;
			sp->EffectMiscValueB[1] = 0;
			sp->EffectImplicitTargetA[0] = 1;
			sp->EffectImplicitTargetA[1] = 1;
			sp->EffectBasePoints[0] = 0;
			sp->EffectBasePoints[1] = 99;
		}
		//Tundra Mammoth (Alliance flavour)
		sp = dbcSpell.LookupEntryForced( 61425 );
		if( sp != NULL )
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOUNTED;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_INCREASE_MOUNTED_SPEED;
			sp->EffectMiscValue[0] = 32633;
			sp->EffectMiscValueB[1] = 0;
			sp->EffectImplicitTargetA[0] = 1;
			sp->EffectImplicitTargetA[1] = 1;
			sp->EffectBasePoints[0] = 0;
			sp->EffectBasePoints[1] = 99;
		}
		 
		//Tundra Mammoth (Horde flavour)
		sp = dbcSpell.LookupEntryForced( 61447 );
		if( sp != NULL )
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOUNTED;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_INCREASE_MOUNTED_SPEED;
			sp->EffectMiscValue[0] = 32640;
			sp->EffectMiscValueB[1] = 0;
			sp->EffectImplicitTargetA[0] = 1;
			sp->EffectImplicitTargetA[1] = 1;
			sp->EffectBasePoints[0] = 0;
			sp->EffectBasePoints[1] = 99;
		}
		 
		//Swift Mooncloth Carpet
		sp = dbcSpell.LookupEntryForced( 61442 );
		if( sp != NULL )
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOUNTED;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_INCREASE_MOUNTED_SPEED;
			sp->EffectMiscValue[0] = 32634;
			sp->EffectMiscValueB[1] = 0;
			sp->EffectImplicitTargetA[0] = 1;
			sp->EffectImplicitTargetA[1] = 1;
			sp->EffectBasePoints[0] = 0;
			sp->EffectBasePoints[1] = 99;
		}
		 
		//Swift Spellfire Carpet
		sp = dbcSpell.LookupEntryForced( 61446 );
		if( sp != NULL )
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOUNTED;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_INCREASE_MOUNTED_SPEED;
			sp->EffectMiscValue[0] = 32636;
			sp->EffectMiscValueB[1] = 0;
			sp->EffectImplicitTargetA[0] = 1;
			sp->EffectImplicitTargetA[1] = 1;
			sp->EffectBasePoints[0] = 0;
			sp->EffectBasePoints[1] = 99;
		}
		 
		//Swift Shadoweave Carpet
		sp = dbcSpell.LookupEntryForced( 61444 );
		if( sp != NULL )
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOUNTED;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_INCREASE_MOUNTED_SPEED;
			sp->EffectMiscValue[0] = 32635;
			sp->EffectMiscValueB[1] = 0;
			sp->EffectImplicitTargetA[0] = 1;
			sp->EffectImplicitTargetA[1] = 1;
			sp->EffectBasePoints[0] = 0;
			sp->EffectBasePoints[1] = 99;
		}
		 
		//Grand Ice Mammoth (Horde flavour)
		sp = dbcSpell.LookupEntryForced( 61469 );
		if( sp != NULL )
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOUNTED;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_INCREASE_MOUNTED_SPEED;
			sp->EffectMiscValue[0] = 31857;
			sp->EffectMiscValueB[1] = 0;
			sp->EffectImplicitTargetA[0] = 1;
			sp->EffectImplicitTargetA[1] = 1;
			sp->EffectBasePoints[0] = 0;
			sp->EffectBasePoints[1] = 99;
		}
		 
		//Grand Ice Mammoth (Alliance flavour)
		sp = dbcSpell.LookupEntryForced( 61470 );
		if( sp != NULL )
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOUNTED;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_INCREASE_MOUNTED_SPEED;
			sp->EffectMiscValue[0] = 31858;
			sp->EffectMiscValueB[1] = 0;
			sp->EffectImplicitTargetA[0] = 1;
			sp->EffectImplicitTargetA[1] = 1;
			sp->EffectBasePoints[0] = 0;
			sp->EffectBasePoints[1] = 99;
		}
		 
		//Grand Black War Mammoth (Horde flavour)
		sp = dbcSpell.LookupEntryForced( 61467 );
		if( sp != NULL )
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOUNTED;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_INCREASE_MOUNTED_SPEED;
			sp->EffectMiscValue[0] = 31861;
			sp->EffectMiscValueB[1] = 0;
			sp->EffectImplicitTargetA[0] = 1;
			sp->EffectImplicitTargetA[1] = 1;
			sp->EffectBasePoints[0] = 0;
			sp->EffectBasePoints[1] = 99;
		}
		 
		//Grand Black War Mammoth (Alliance flavour)
		sp = dbcSpell.LookupEntryForced( 61465 );
		if( sp != NULL )
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOUNTED;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_INCREASE_MOUNTED_SPEED;
			sp->EffectMiscValue[0] = 31862;
			sp->EffectMiscValueB[1] = 0;
			sp->EffectImplicitTargetA[0] = 1;
			sp->EffectImplicitTargetA[1] = 1;
			sp->EffectBasePoints[0] = 0;
			sp->EffectBasePoints[1] = 99;
		}

       // Rune Tap
	   sp = dbcSpell.LookupEntryForced( 48982 );
       if( sp != NULL )
           sp->Effect[0] = SPELL_EFFECT_DUMMY;

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
}