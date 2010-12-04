/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009 ArcEmu Team <http://www.arcemu.org/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Setup.h"

/************************************************************************/
/* Raid_BlackTemple.cpp Script								           */
/************************************************************************/

/////////////////////////////////////////////////////////////////////////////////
////// Black Temple Instance

#define MAP_BLACK_TEMPLE								564
#define CN_SUPREMUS										22898

class BlackTempleScript : public MoonInstanceScript
{
public:
	MOONSCRIPT_INSTANCE_FACTORY_FUNCTION( BlackTempleScript, MoonInstanceScript );
	BlackTempleScript( MapMgr* pMapMgr ) : MoonInstanceScript( pMapMgr )
	{
		// Way to select bosses
		BuildEncounterMap();
		if ( mEncounters.size() == 0 )
			return;

		for ( EncounterMap::iterator Iter = mEncounters.begin(); Iter != mEncounters.end(); ++Iter )
		{
			if ( ( *Iter ).second.mState != State_Finished )
				continue;

			switch ( ( *Iter ).first )
			{
			case CN_SUPREMUS:
				AddGameObjectStateByEntry( 185882, State_Active );	// Gate to Black Temple behind Supremus
				break;
			default:
				continue;
			};
		};
	};

	void OnCreatureDeath( Creature* pVictim, Unit* pKiller )
	{
		if ( pVictim->GetCreatureInfo()->Rank != ELITE_WORLDBOSS )
			return;

		EncounterMap::iterator Iter = mEncounters.find( pVictim->GetEntry() );
		if ( Iter == mEncounters.end() )
			return;

		( *Iter ).second.mState = State_Finished;

		// You don't have to use additional scripts to open any gates / doors
		switch ( pVictim->GetEntry() )
		{
		case CN_SUPREMUS:
			AddGameObjectStateByEntry( 185882, State_Active );	// Gate to Black Temple behind Supremus
			break;
		default:
			{
			};
		};
	};

};

//////////////////////////////////////////////////////////////////////////
/////// Trash AIs
/*
class TemplateAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(TemplateAI, MoonScriptCreatureAI);
	TemplateAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
	}
};
*/
//////////////////////////////////////////////////////////////////////////
/////// Beast AIs

#define CN_DRAGON_TURTLE					22885
#define DRAGON_TRUTLE_SHELL_SHIELD			40087
#define DRAGON_TURTLE_WATER_SPIT			40086

class DragonTurtleAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DragonTurtleAI, MoonScriptCreatureAI);
	DragonTurtleAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DRAGON_TRUTLE_SHELL_SHIELD, Target_Self, 10, 0, 25);
		AddSpell(DRAGON_TURTLE_WATER_SPIT, Target_Current, 8, 2.5, 20, 0, 45);		// Random?
	}
};

#define CN_LEVIATHAN						22884
#define LEVIATHAN_DEBILITATING_SPRAY		40079
#define LEVIATHAN_POISON_SPIT				40078
#define LEVIATHAN_TAIL_SWEEP				40077

class LeviathanAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(LeviathanAI, MoonScriptCreatureAI);
	LeviathanAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(LEVIATHAN_DEBILITATING_SPRAY, Target_RandomPlayer, 8, 2.5, 20);
		AddSpell(LEVIATHAN_POISON_SPIT, Target_RandomPlayer, 7, 2.5, 25);
		AddSpell(LEVIATHAN_TAIL_SWEEP, Target_Self, 7, 0, 30);
	}
};

#define CN_MUTANT_WAR_HOUND					23232
#define MUTANT_WAR_HOUND_CLOUD_OF_DISEASE	41193

class MutantWarHoundAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(MutantWarHoundAI, MoonScriptCreatureAI);
	MutantWarHoundAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) {}

	void OnDied(Unit* pKiller)
	{
		ParentClass::OnDied(pKiller);

		Aura* pAura = new Aura( dbcSpell.LookupEntry(MUTANT_WAR_HOUND_CLOUD_OF_DISEASE),(uint32)20000, _unit, _unit );
		_unit->AddAura(pAura);
	}
};
// I don't have this creature in DB, so hasn't checked it yet
#define CN_SHADOWMOON_RIDING_HOUND					23083
#define SHADOWMOON_RIDING_HOUND_CARNIVOROUS_BITE	41092
#define SHADOWMOON_RIDING_HOUND_CHARGE				25821
#define SHADOWMOON_RIDING_HOUND_ENRAGE				8599

void SpellFunc_Charge(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);

class ShadowmoonRidingHoundAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ShadowmoonRidingHoundAI, MoonScriptCreatureAI);
	ShadowmoonRidingHoundAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SHADOWMOON_RIDING_HOUND_CARNIVOROUS_BITE, Target_Current, 10, 0, 20, 0, 10);
		mCharge = AddSpell(SHADOWMOON_RIDING_HOUND_CHARGE, Target_Current, 0, 0, 0);
		AddSpellFunc(&SpellFunc_Charge, Target_RandomPlayer, 8, 0, 35);
		AddSpell(SHADOWMOON_RIDING_HOUND_ENRAGE, Target_Self, 6, 0, 40);
	}

	SpellDesc*	mCharge;
};

void SpellFunc_Charge(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType)
{
	ShadowmoonRidingHoundAI *pHoundAI = (pCreatureAI != NULL) ? (ShadowmoonRidingHoundAI*)pCreatureAI : NULL;
	if (pHoundAI != NULL)
	{
		Unit* pCurrentTarget = pHoundAI->GetUnit()->GetAIInterface()->getNextTarget();
		if (pCurrentTarget != NULL && pCurrentTarget != pTarget)
		{
			pHoundAI->GetUnit()->GetAIInterface()->AttackReaction(pTarget, 500);
			pHoundAI->GetUnit()->GetAIInterface()->setNextTarget(pTarget);
			pHoundAI->GetUnit()->GetAIInterface()->RemoveThreatByPtr(pCurrentTarget);
		}

		pHoundAI->CastSpell(pHoundAI->mCharge);
	}
}

//////////////////////////////////////////////////////////////////////////
/////// Demon AIs

#define	CN_SISTER_OF_PLEASURE				22964
#define	SISTER_OF_PLEASURE_GREATER_HEAL		41378
#define SISTER_OF_PLEASURE_HOLY_NOVA		41380
#define SISTER_OF_PLEASURE_SHARED_BONDS		41362
#define SISTER_OF_PLEASURE_SHELL_OF_LIFE	41381

class SisterOfPleasureAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SisterOfPleasureAI, MoonScriptCreatureAI);
	SisterOfPleasureAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SISTER_OF_PLEASURE_GREATER_HEAL, Target_WoundedFriendly, 10, 2, 45);
		AddSpell(SISTER_OF_PLEASURE_HOLY_NOVA, Target_Self, 10, 0, 20);
		AddSpell(SISTER_OF_PLEASURE_SHARED_BONDS, Target_Current, 5, 0, 25);
		AddSpell(SISTER_OF_PLEASURE_SHELL_OF_LIFE, Target_Self, 7, 1, 45);
	}
};

#define	CN_SISTER_OF_PAIN					22956
#define	SISTER_OF_PAIN_LASH_OF_PAIN			41353
#define	SISTER_OF_PAIN_PAINFUL_RAGE			41369
#define	SISTER_OF_PAIN_SHADOW_WORD_PAIN		41355
#define	SISTER_OF_PAIN_SHARED_BONDS			41362
#define	SISTER_OF_PAIN_SHELL_OF_PAIN		41371

class SisterOfPainAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SisterOfPainAI, MoonScriptCreatureAI);
	SisterOfPainAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SISTER_OF_PAIN_LASH_OF_PAIN, Target_Current, 10, 0, 25, 0, 10);
		AddSpell(SISTER_OF_PAIN_PAINFUL_RAGE, Target_Self, 5, 0, 60);
		AddSpell(SISTER_OF_PAIN_SHADOW_WORD_PAIN, Target_RandomPlayer, 10, 0, 25);
		AddSpell(SISTER_OF_PAIN_SHARED_BONDS, Target_Current, 5, 0, 25);
		AddSpell(SISTER_OF_PAIN_SHELL_OF_PAIN, Target_Self, 7, 1, 45);
	}
};

#define	CN_PRIESTESS_OF_DEMENTIA				22957
#define PRIESTESS_OF_DEMENTIA_CONFUSION			41397
#define PRIESTESS_OF_DEMENTIA_DEMENTIA			41404

class PriestessOfDementiaAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(PriestessOfDementiaAI, MoonScriptCreatureAI);
	PriestessOfDementiaAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(PRIESTESS_OF_DEMENTIA_CONFUSION, Target_Self, 8, 0, 30);
		AddSpell(PRIESTESS_OF_DEMENTIA_DEMENTIA, Target_Self, 5, 0, 60);
	}
};

#define	CN_PRIESTESS_OF_DELIGHT					22962
#define PRIESTESS_OF_DELIGHT_CURSE_OF_VITALITY	41351

class PriestessOfDelightAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(PriestessOfDelightAI, MoonScriptCreatureAI);
	PriestessOfDelightAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(PRIESTESS_OF_DELIGHT_CURSE_OF_VITALITY, Target_RandomPlayer, 10, 0, 30);
	}
};

#define	CN_ILLIDARI_NIGHTLORD					22855
#define ILLIDARI_NIGHTLORD_SUMMON_SHADOWFIENDS	39649
#define ILLIDARI_NIGHTLORD_SHADOW_INFERNO		39645
#define ILLIDARI_NIGHTLORD_FEAR					41150
#define ILLIDARI_NIGHTLORD_CURSE_OF_MENDING		39647

class IllidariNightlordAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(IllidariNightlordAI, MoonScriptCreatureAI);
	IllidariNightlordAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ILLIDARI_NIGHTLORD_SUMMON_SHADOWFIENDS, Target_Self, 7, 0, 45);			// does it work?
		AddSpell(ILLIDARI_NIGHTLORD_SHADOW_INFERNO, Target_Self, 10, 0, 25);
		AddSpell(ILLIDARI_NIGHTLORD_FEAR, Target_Self, 7, 1, 30);
		AddSpell(ILLIDARI_NIGHTLORD_CURSE_OF_MENDING, Target_RandomPlayer, 8, 2, 35);
	}
};

#define CN_ILLIDARI_HEARTSEEKER							23339
#define ILLIDARI_HEARTSEEKER_CURSE_OF_THE_BLEAKHEART	41170
#define ILLIDARI_HEARTSEEKER_RAPID_SHOT					41173
#define ILLIDARI_HEARTSEEKER_SHOOT						41169
#define ILLIDARI_HEARTSEEKER_SKELETON_SHOT				41171

void SpellFunc_RapidShot(SpellDesc * pThis, MoonScriptCreatureAI *pCreatureAI, Unit* pTarget, TargetType pType);

class IllidariHeartseekerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(IllidariHeartseekerAI, MoonScriptCreatureAI);
	IllidariHeartseekerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ILLIDARI_HEARTSEEKER_CURSE_OF_THE_BLEAKHEART, Target_RandomPlayer, 8, 0, 40, 0, 30);
		mRapidShot = AddSpell(ILLIDARI_HEARTSEEKER_RAPID_SHOT, Target_Self, 0, 8, 0);						// I need to check mechanics (maybe it should speed up usage of Shoot spell (2x lesser freq))
		AddSpell(ILLIDARI_HEARTSEEKER_SHOOT, Target_Current, 80, 0, 1, 0, 30);
		AddSpell(ILLIDARI_HEARTSEEKER_SKELETON_SHOT, Target_RandomPlayer, 7, 0, 25, 0, 30);
		AddSpellFunc(&SpellFunc_RapidShot, Target_Current, 7, 8, 40, 0, 30);
	}

	void OnCombatStart(Unit* pTarget)
	{
		ParentClass::OnCombatStart(pTarget);
		if (GetRangeToUnit(pTarget) <= 30.0f)
		{
			SetBehavior(Behavior_Spell);
			SetCanMove(false);
		}
	}

	void AIUpdate()
	{
		ParentClass::AIUpdate();

		Unit* pTarget = _unit->GetAIInterface()->getNextTarget();
		if (pTarget != NULL)
		{
			if (GetRangeToUnit(pTarget) <= 30.0f)
			{
				SetBehavior(Behavior_Spell);
				SetCanMove(false);
			}
		}
	}

	SpellDesc*	mRapidShot;
};

void SpellFunc_RapidShot(SpellDesc *pThis, MoonScriptCreatureAI *pCreatureAI, Unit* pTarget, TargetType pType)
{
	IllidariHeartseekerAI *pHeartseeker = (pCreatureAI != NULL) ? (IllidariHeartseekerAI*)pCreatureAI : NULL;
	if (pHeartseeker != NULL)
	{
		pHeartseeker->CastSpell(pHeartseeker->mRapidShot);
	}
}

#define CN_ILLIDARI_FEARBRINGER					22954
#define ILLIDARI_FEARBRINGER_ILLIDARI_FLAMES	40938
#define ILLIDARI_FEARBRINGER_RAIN_OF_CHAOS		40946
#define ILLIDARI_FEARBRINGER_WAR_STOMP			40936

class IllidariFearbringerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(IllidariFearbringerAI, MoonScriptCreatureAI);
	IllidariFearbringerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ILLIDARI_FEARBRINGER_ILLIDARI_FLAMES, Target_Current, 8, 2, 25, 0, 10);
		AddSpell(ILLIDARI_FEARBRINGER_RAIN_OF_CHAOS, Target_RandomPlayerDestination, 7, 0, 35);
		AddSpell(ILLIDARI_FEARBRINGER_WAR_STOMP, Target_Self, 8, 0, 35);
	}
};

#define CN_ILLIDARI_DEFILER					22853
#define ILLIDARI_DEFILER_BANISH				39674
#define ILLIDARI_DEFILER_CURSE_OF_AGONY		39672
#define ILLIDARI_DEFILER_FEL_IMMOLATE		39670
#define ILLIDARI_DEFILER_RAIN_OF_CHAOS		39671

class IllidariDefilerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(IllidariDefilerAI, MoonScriptCreatureAI);
	IllidariDefilerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ILLIDARI_DEFILER_BANISH, Target_RandomPlayer, 8, 1, 30);
		AddSpell(ILLIDARI_DEFILER_CURSE_OF_AGONY, Target_RandomPlayer, 7, 0, 45);
		AddSpell(ILLIDARI_DEFILER_FEL_IMMOLATE, Target_RandomPlayer, 8, 2, 25);
		AddSpell(ILLIDARI_DEFILER_RAIN_OF_CHAOS, Target_RandomPlayerDestination, 7, 6, 35);
	}
};

#define CN_ILLIDARI_CENTURION				23337
#define ILLIDARI_CENTURION_CLEAVE			15284
#define ILLIDARI_CENTURION_SONIC_STRIKE		41168

class IllidariCenturionAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(IllidariCenturionAI, MoonScriptCreatureAI);
	IllidariCenturionAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ILLIDARI_CENTURION_CLEAVE, Target_Current, 10, 0, 20, 0, 10);
		AddSpell(ILLIDARI_CENTURION_SONIC_STRIKE, Target_Self, 8, 0, 35);
	}
};

#define CN_ILLIDARI_BONESLICER					22869
#define ILLIDARI_BONESLICER_CLOAK_OF_SHADOWS	39666
#define ILLIDARI_BONESLICER_GOUGE				24698
#define ILLIDARI_BONESLICER_SHADOWSTEP			41176

void SpellFunc_Gouge(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);

class IllidariBoneslicerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(IllidariBoneslicerAI, MoonScriptCreatureAI);
	IllidariBoneslicerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ILLIDARI_BONESLICER_CLOAK_OF_SHADOWS, Target_Self, 8, 0, 25);
		mGouge = AddSpell(ILLIDARI_BONESLICER_GOUGE, Target_Current, 0, 0, 0);
		AddSpellFunc(&SpellFunc_Gouge, Target_Current, 7, 0, 25, 0, 10);
		AddSpell(ILLIDARI_BONESLICER_SHADOWSTEP, Target_Current, 7, 0, 30);
	}

	SpellDesc*	mGouge;
};

void SpellFunc_Gouge(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType)
{
	IllidariBoneslicerAI *pBoneslicer = (pCreatureAI != NULL) ? (IllidariBoneslicerAI*)pCreatureAI : NULL;
	if (pBoneslicer != NULL)
	{
		pBoneslicer->CastSpell(pBoneslicer->mGouge);
		Unit* pSecondHated = pBoneslicer->GetUnit()->GetAIInterface()->GetSecondHated();
		if (pSecondHated != NULL)
		{
			pBoneslicer->GetUnit()->GetAIInterface()->AttackReaction(pSecondHated, 2000);
			pBoneslicer->GetUnit()->GetAIInterface()->setNextTarget(pSecondHated);
			pBoneslicer->GetUnit()->GetAIInterface()->RemoveThreatByPtr(pTarget);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/////// Humanoid AIs

#define CN_ASHTONGUE_BATTLELORD					22844
#define ASHTONGUE_BATTLELORD_CLEAVE				15284
#define ASHTONGUE_BATTLELORD_CONCUSSION_BLOW	32588
#define ASHTONGUE_BATTLELORD_CONCUSSIVE_THROW	41182
#define ASHTONGUE_BATTLELORD_ENRAGE				34970

class AshtongueBattlelordAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AshtongueBattlelordAI, MoonScriptCreatureAI);
	AshtongueBattlelordAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ASHTONGUE_BATTLELORD_CLEAVE, Target_Current, 10, 0, 15, 0, 10);
		AddSpell(ASHTONGUE_BATTLELORD_CONCUSSION_BLOW, Target_Current, 8, 0, 30, 0, 10);
		AddSpell(ASHTONGUE_BATTLELORD_CONCUSSIVE_THROW, Target_RandomPlayerNotCurrent, 8, 0, 25, 10, 40);
		AddSpell(ASHTONGUE_BATTLELORD_ENRAGE, Target_Self, 4, 0, 50);
	}
};

#define CN_ASHTONGUE_DEFENDER					23216
#define ASHTONGUE_DEFENDER_DEBILITATING_STRIKE	41178
#define ASHTONGUE_DEFENDER_SHIELD_BASH			41180

class AshtongueDefenderAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AshtongueDefenderAI, MoonScriptCreatureAI);
	AshtongueDefenderAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ASHTONGUE_DEFENDER_DEBILITATING_STRIKE, Target_Current, 10, 0, 20, 0, 10);
		AddSpell(ASHTONGUE_DEFENDER_SHIELD_BASH, Target_Current, 7, 0, 25, 0, 10);
	}
};

#define CN_ASHTONGUE_ELEMENTALIST				23523
#define ASHTONGUE_ELEMENTALIST_LIGHTNING_BOLT	42024
#define ASHTONGUE_ELEMENTALIST_RAID_OF_FIRE		42023

class AshtongueElementalistAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AshtongueElementalistAI, MoonScriptCreatureAI);
	AshtongueElementalistAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ASHTONGUE_ELEMENTALIST_LIGHTNING_BOLT, Target_Current, 8, 1, 20, 0, 50);	// Random?
		AddSpell(ASHTONGUE_ELEMENTALIST_RAID_OF_FIRE, Target_RandomPlayerDestination, 6, 0, 25);
	}
};
// TO DO: Add Totem AIs
#define CN_ASHTONGUE_MYSTIC						22845
#define ASHTONGUE_MYSTIC_BLOODLUST				41185
#define ASHTONGUE_MYSTIC_CHAIN_HEAL				41114
#define ASHTONGUE_MYSTIC_CYCLONE_TOTEM			39589
#define ASHTONGUE_MYSTIC_FLAME_SHOCK			41115
#define ASHTONGUE_MYSTIC_FROST_SHOCK			41116
#define ASHTONGUE_MYSTIC_SEARING_TOTEM			39588
#define ASHTONGUE_MYSTIC_SUMMON_WINDFURY_TOTEM	39586

class AshtongueMysticAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AshtongueMysticAI, MoonScriptCreatureAI);
	AshtongueMysticAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ASHTONGUE_MYSTIC_BLOODLUST, Target_Self, 5, 0, 45);
		AddSpell(ASHTONGUE_MYSTIC_CHAIN_HEAL, Target_WoundedFriendly, 6, 2.5, 35);
		AddSpell(ASHTONGUE_MYSTIC_CYCLONE_TOTEM, Target_Self, 7, 0, 35);
		AddSpell(ASHTONGUE_MYSTIC_FLAME_SHOCK, Target_Current, 8, 0, 25, 0, 20);
		AddSpell(ASHTONGUE_MYSTIC_FROST_SHOCK, Target_Current, 8, 0, 25, 0, 20);
		AddSpell(ASHTONGUE_MYSTIC_SEARING_TOTEM, Target_Self, 7, 0, 35);
		AddSpell(ASHTONGUE_MYSTIC_SUMMON_WINDFURY_TOTEM, Target_Self, 7, 0, 35);
	}
};

#define CN_ASHTONGUE_PRIMALIST					22847
#define ASHTONGUE_PRIMALIST_MULTISHOT			41187
#define ASHTONGUE_PRIMALIST_SHOOT				41188
#define ASHTONGUE_PRIMALIST_SWEEPING_WING_CLIP	39584
#define ASHTONGUE_PRIMALIST_WYVERN_STRING		41186

class AshtonguePrimalistAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AshtonguePrimalistAI, MoonScriptCreatureAI);
	AshtonguePrimalistAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ASHTONGUE_PRIMALIST_MULTISHOT, Target_Current, 8, 0, 40, 0, 30);			// Target_Self - self dmg too (kill == crash)
		AddSpell(ASHTONGUE_PRIMALIST_SHOOT, Target_Current, 80, 0, 1, 0, 30);
		AddSpell(ASHTONGUE_PRIMALIST_SWEEPING_WING_CLIP, Target_Current, 8, 0, 15, 0, 10, true);
		AddSpell(ASHTONGUE_PRIMALIST_WYVERN_STRING, Target_RandomPlayer, 7, 0, 25, 0, 30);
	}

	void OnCombatStart(Unit* pTarget)
	{
		ParentClass::OnCombatStart(pTarget);
		if (GetRangeToUnit(pTarget) <= 30.0f)
		{
			SetBehavior(Behavior_Spell);
			SetCanMove(false);
		}
	}

	void AIUpdate()
	{
		ParentClass::AIUpdate();

		Unit* pTarget = _unit->GetAIInterface()->getNextTarget();
		if (pTarget != NULL)
		{
			if (GetRangeToUnit(pTarget) <= 30.0f)
			{
				SetBehavior(Behavior_Spell);
				SetCanMove(false);
			}
		}
	}
};

#define CN_ASHTONGUE_ROGUE					23318
#define ASHTONGUE_ROGUE_DEBILITATING_POISON	41978
#define ASHTONGUE_ROGUE_EVISCERATE			41177

class AshtongueRogueAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AshtongueRogueAI, MoonScriptCreatureAI);
	AshtongueRogueAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ASHTONGUE_ROGUE_DEBILITATING_POISON, Target_Current, 7, 0, 30, 0, 10);
		AddSpell(ASHTONGUE_ROGUE_EVISCERATE, Target_Current, 10, 0, 15, 0, 10);
	}
};

#define CN_ASHTONGUE_SPIRITBINDER			23524
#define ASHTONGUE_SPIRITBINDER_CHAIN_HEAL	42027
#define ASHTONGUE_SPIRITBINDER_SPIRIT_HEAL	42317
#define ASHTONGUE_SPIRITBINDER_SPIRIT_HEAL2	42318
#define ASHTONGUE_SPIRITBINDER_SPIRIT_MEND	42025

class AshtongueSpiritbinderAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AshtongueSpiritbinderAI, MoonScriptCreatureAI);
	AshtongueSpiritbinderAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ASHTONGUE_SPIRITBINDER_CHAIN_HEAL, Target_WoundedFriendly, 7, 1, 25);
		if (_unit->GetMapMgr()->iInstanceMode != MODE_HEROIC)	// Guessed
			AddSpell(ASHTONGUE_SPIRITBINDER_SPIRIT_HEAL, Target_Self, 7, 0, 25);				// Weaker version
		else
			AddSpell(ASHTONGUE_SPIRITBINDER_SPIRIT_HEAL2, Target_Self, 7, 0, 25);				// Stronger version
		AddSpell(ASHTONGUE_SPIRITBINDER_SPIRIT_MEND, Target_WoundedFriendly, 8, 0, 25);
	}
};
// Completely guessed mechanics
#define CN_ASHTONGUE_STALKER					23374
#define ASHTONGUE_STALKER_BLIND					34654
#define ASHTONGUE_STALKER_INSTANT_POISON		41189
#define ASHTONGUE_STALKER_MINDNUMBING_POISON	41190
#define ASHTONGUE_STALKER_STEATH				34189

class AshtongueStalkerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AshtongueStalkerAI, MoonScriptCreatureAI);
	AshtongueStalkerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ASHTONGUE_STALKER_BLIND, Target_RandomPlayer, 7, 1, 25);				// Should be Blind - > Attack Next Target?
		AddSpell(ASHTONGUE_STALKER_INSTANT_POISON, Target_Current, 10, 0, 20, 0, 10);
		AddSpell(ASHTONGUE_STALKER_MINDNUMBING_POISON, Target_RandomPlayer, 8, 0, 20);
		ApplyAura(ASHTONGUE_STALKER_STEATH);
	}

	void OnCombatStart(Unit*  pTarget)
	{
		ParentClass::OnCombatStart(pTarget);
		SpellDesc *pSpell = FindSpellById(ASHTONGUE_STALKER_BLIND);
		if (pSpell != NULL)
		{
			CastSpellNowNoScheduling(pSpell);
		}
	}

	void OnCombatStop(Unit*  pTarget)
	{
		ParentClass::OnCombatStop(pTarget);
		if (IsAlive())
		{
			ApplyAura(ASHTONGUE_STALKER_STEATH);
		}
	}
};

#define CN_ASHTONGUE_STORMCALLER				22846
#define ASHTONGUE_STORMCALLER_CHAIN_LIGHTNING	41183
#define ASHTONGUE_STORMCALLER_LIGHTNING_BOLT	41184
#define ASHTONGUE_STORMCALLER_LIGHTNING_SHIELD	41151

class AshtongueStormcallerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AshtongueStormcallerAI, MoonScriptCreatureAI);
	AshtongueStormcallerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ASHTONGUE_STORMCALLER_CHAIN_LIGHTNING, Target_Current, 7, 3, 35, 0, 30);		// Random?
		AddSpell(ASHTONGUE_STORMCALLER_LIGHTNING_BOLT, Target_Current, 10, 2, 20);
		AddSpell(ASHTONGUE_STORMCALLER_LIGHTNING_SHIELD, Target_Self, 10, 0, 50);
	}
};

#define CN_BONECHEWER_BEHEMOTH					23196
#define BONECHEWER_BEHEMOTH_BEHEMOTH_CHARGE		41272
#define BONECHEWER_BEHEMOTH_ENRAGE				8269
#define BONECHEWER_BEHEMOTH_FEL_STOMP			41274
#define BONECHEWER_BEHEMOTH_FIERY_COMET			41277
#define BONECHEWER_BEHEMOTH_METEOR				41276

class BonechewerBehemothAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BonechewerBehemothAI, MoonScriptCreatureAI);
	BonechewerBehemothAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(BONECHEWER_BEHEMOTH_BEHEMOTH_CHARGE, Target_Current, 0, 0, 20);	// Mechanics are like charge on random target?
		AddSpell(BONECHEWER_BEHEMOTH_ENRAGE, Target_Self, 5, 0, 45);
		AddSpell(BONECHEWER_BEHEMOTH_FEL_STOMP, Target_Self, 7, 0, 30);
		AddSpell(BONECHEWER_BEHEMOTH_FIERY_COMET, Target_RandomPlayerDestination, 6, 1, 30);
		AddSpell(BONECHEWER_BEHEMOTH_METEOR, Target_RandomPlayerDestination, 5, 2, 40);
	}

	void OnCombatStart(Unit* pTarget)
	{
		ParentClass::OnCombatStart(pTarget);
		SpellDesc *pCharge = FindSpellById(BONECHEWER_BEHEMOTH_BEHEMOTH_CHARGE);
		if (pCharge != NULL)
		{
			CastSpellNowNoScheduling(pCharge);
		}
	}
};

#define CN_BONECHEWER_BLADE_FURY					23235
#define BONECHEWER_BLADE_FURY_WHIRLWIND				41194
#define BONECHEWER_BLADE_FURY_WHIRLWIND2			41195

class BonechewerBladeFuryAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BonechewerBladeFuryAI, MoonScriptCreatureAI);
	BonechewerBladeFuryAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(BONECHEWER_BLADE_FURY_WHIRLWIND, Target_Self, 7, 8, 40);	// Self / Current ?
	}
};

#define CN_BONECHEWER_BLOOD_PROPHET					23237
#define BONECHEWER_BLOOD_PROPHET_BLOOD_DRAIN		41238
#define BONECHEWER_BLOOD_PROPHET_BLOODBOLT			41229
#define BONECHEWER_BLOOD_PROPHET_ENRAGE				8269
#define BONECHEWER_BLOOD_PROPHET_PROPHECY_OF_BLOOD	41230
#define BONECHEWER_BLOOD_PROPHET_PROPHECY_OF_BLOOD2	41231

class BonechewerBloodProphetAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BonechewerBloodProphetAI, MoonScriptCreatureAI);
	BonechewerBloodProphetAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(BONECHEWER_BLOOD_PROPHET_BLOOD_DRAIN, Target_Current, 6, 5, 30, 0, 20);	// Random?
		AddSpell(BONECHEWER_BLOOD_PROPHET_BLOODBOLT, Target_RandomPlayer, 7, 1.5, 25);
		AddSpell(BONECHEWER_BLOOD_PROPHET_ENRAGE, Target_Self, 5, 0, 50);
		if (_unit->GetMapMgr()->iInstanceMode == MODE_HEROIC)						// Guessed
			AddSpell(BONECHEWER_BLOOD_PROPHET_PROPHECY_OF_BLOOD, Target_RandomPlayer, 10, 0, 30);
		else
			AddSpell(BONECHEWER_BLOOD_PROPHET_PROPHECY_OF_BLOOD2, Target_RandomPlayer, 10, 0, 30);
	}
};

#define CN_BONECHEWER_BRAWLER				23222
#define BONECHEWER_BRAWLER_FRENZY			41254

class BonechewerBrawlerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BonechewerBrawlerAI, MoonScriptCreatureAI);
	BonechewerBrawlerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(BONECHEWER_BRAWLER_FRENZY, Target_Self, 7, 0, 30);
	}
};

#define CN_BONECHEWER_COMBATANT				23239
#define BONECHEWER_COMBATANT_FRENZY			8269

class BonechewerCombatantAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BonechewerCombatantAI, MoonScriptCreatureAI);
	BonechewerCombatantAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(BONECHEWER_COMBATANT_FRENZY, Target_Self, 7, 0, 45);
	}
};

#define CN_BONECHEWER_SHIELD_DISCIPLE				23236
#define BONECHEWER_SHIELD_DISCIPLE_INTERVENE		41198
#define BONECHEWER_SHIELD_DISCIPLE_SHIELD_BASH		41197
#define BONECHEWER_SHIELD_DISCIPLE_SHIELD_WALL		41196
#define BONECHEWER_SHIELD_DISCIPLE_THROW_SHIELD		41213

class BonechewerShieldDiscipleAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BonechewerShieldDiscipleAI, MoonScriptCreatureAI);
	BonechewerShieldDiscipleAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(BONECHEWER_SHIELD_DISCIPLE_INTERVENE, Target_Current, 0, 0, 20);
		AddSpell(BONECHEWER_SHIELD_DISCIPLE_SHIELD_BASH, Target_Current, 8, 0, 25, 0, 10);
		AddSpell(BONECHEWER_SHIELD_DISCIPLE_SHIELD_WALL, Target_Self, 8, 0, 35);
		AddSpell(BONECHEWER_SHIELD_DISCIPLE_THROW_SHIELD, Target_RandomPlayer, 7, 0, 30);	// Current?
	}

	void OnCombatStart(Unit* pTarget)
	{
		ParentClass::OnCombatStart(pTarget);
		SpellDesc *pIntervene = FindSpellById(BONECHEWER_SHIELD_DISCIPLE_INTERVENE);
		if (pIntervene != NULL)
		{
			CastSpellNowNoScheduling(pIntervene);
		}
	}
};

#define CN_BONECHEWER_SPECTATOR					23223
#define BONECHEWER_SPECTATOR_CHARGE				36140
#define BONECHEWER_SPECTATOR_CLEAVE				40505
#define BONECHEWER_SPECTATOR_MORTAL_WOUND		25646
#define BONECHEWER_SPECTATOR_STRIKE				13446
#define BONECHEWER_SPECTATOR_SUNDER_ARMOR		13444

class BonechewerSpectatorAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BonechewerSpectatorAI, MoonScriptCreatureAI);
	BonechewerSpectatorAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(BONECHEWER_SPECTATOR_CHARGE, Target_Current, 0, 0, 20);
		AddSpell(BONECHEWER_SPECTATOR_CLEAVE, Target_Current, 10, 0, 25, 0, 10);
		AddSpell(BONECHEWER_SPECTATOR_MORTAL_WOUND, Target_Current, 7, 0, 15, 0, 10);
		AddSpell(BONECHEWER_SPECTATOR_STRIKE, Target_Current, 10, 0, 10, 0, 10);
		AddSpell(BONECHEWER_SPECTATOR_SUNDER_ARMOR, Target_Current, 7, 0, 20, 0, 10);
	}

	void OnCombatStart(Unit* pTarget)
	{
		ParentClass::OnCombatStart(pTarget);
		SpellDesc *pCharge = FindSpellById(BONECHEWER_SPECTATOR_CHARGE);
		if (pCharge != NULL)
		{
			CastSpellNowNoScheduling(pCharge);
		}
	}
};

#define CN_BONECHEWER_TASKMASTER				23028
#define BONECHEWER_TASKMASTER_DISGRUNTLED		40851
#define BONECHEWER_TASKMASTER_FURY				40845

class BonechewerTaskmasterAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BonechewerTaskmasterAI, MoonScriptCreatureAI);
	BonechewerTaskmasterAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(BONECHEWER_TASKMASTER_DISGRUNTLED, Target_Self, 0, 0, 45);	//5, 0, 45);	// Disabled for now (buffs players instead of Taskmasters)		
		AddSpell(BONECHEWER_TASKMASTER_FURY, Target_Self, 8, 0, 15);
	}
};

#define CN_BONECHEWER_WORKER					22963
#define BONECHEWER_WORKER_THROW_PICK			40844

class BonechewerWorkerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BonechewerWorkerAI, MoonScriptCreatureAI);
	BonechewerWorkerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(BONECHEWER_WORKER_THROW_PICK, Target_RandomPlayer, 8, 0.5, 15);	// Current? Also shouldn't be cast once only?
	}
};

#define CN_CHARMING_COURTESAN					22955
#define CHARMING_COURTESAN_INFATUATION			41345
#define CHARMING_COURTESAN_POISONOUS_THROW		41346

class CharmingCourtesanAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CharmingCourtesanAI, MoonScriptCreatureAI);
	CharmingCourtesanAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(CHARMING_COURTESAN_INFATUATION, Target_RandomPlayer, 7, 20, 40);
		AddSpell(CHARMING_COURTESAN_POISONOUS_THROW, Target_RandomPlayer, 8, 0, 20);
	}
};

#define CN_COILSKAR_GENERAL						22873
#define COILSKAR_GENERAL_BOOMING_VOICE			40080
#define COILSKAR_GENERAL_FREE_FRIEND			40081

class CoilskarGeneralAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CoilskarGeneralAI, MoonScriptCreatureAI);
	CoilskarGeneralAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(COILSKAR_GENERAL_BOOMING_VOICE, Target_Self, 7, 0, 35);
		AddSpell(COILSKAR_GENERAL_FREE_FRIEND, Target_RandomFriendly, 7, 0, 20);	// SpellFunc to check incapacitating effects?
	}
};
// Is it Ranged unit throwing spears all the time?
#define CN_COILSKAR_HARPOONER					22874
#define COILSKAR_HARPOONER_HARPOONERS_MARK		40084
#define COILSKAR_HARPOONER_HOOKED_NET			40082
#define COILSKAR_HARPOONER_SPEAR_THROW			40083

class CoilskarHarpoonerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CoilskarHarpoonerAI, MoonScriptCreatureAI);
	CoilskarHarpoonerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(COILSKAR_HARPOONER_HARPOONERS_MARK, Target_RandomPlayer, 7, 0, 35);		// I must link Dragon Turtle with Harpooner before scripting it
		AddSpell(COILSKAR_HARPOONER_HOOKED_NET, Target_RandomPlayer, 7, 0.5, 25);
		AddSpell(COILSKAR_HARPOONER_SPEAR_THROW, Target_Current, 8, 0, 15, 0, 40);			// Random? Also isn't it typical ranged unit? (using Spear Throw instead of Shoot/Shot spell?)
	}

	void OnCombatStart(Unit* pTarget)
	{
		ParentClass::OnCombatStart(pTarget);
		SpellDesc *pMark = FindSpellById(COILSKAR_HARPOONER_HARPOONERS_MARK);
		if (pMark != NULL)
		{
			CastSpellNowNoScheduling(pMark);
		}
	}
};

#define CN_COILSKAR_SEACALLER					22875
#define COILSKAR_SEACALLER_FORKED_LIGHTNING		40088
#define COILSKAR_SEACALLER_HURRICANE			40090
#define COILSKAR_SEACALLER_SUMMON_GEYSER		40091

class CoilskarSeacallerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CoilskarSeacallerAI, MoonScriptCreatureAI);
	CoilskarSeacallerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(COILSKAR_SEACALLER_FORKED_LIGHTNING, Target_Self, 8, 2, 20);
		AddSpell(COILSKAR_SEACALLER_HURRICANE, Target_RandomPlayerDestination, 8, 20, 35);
		AddSpell(COILSKAR_SEACALLER_SUMMON_GEYSER, Target_RandomPlayer, 7, 2, 25);			// Npc to add to DB + AI
	}
};

#define CN_COILSKAR_SOOTHSAYER					22876
#define COILSKAR_SOOTHSAYER_HOLY_NOVA			40096
#define COILSKAR_SOOTHSAYER_RESTORATION			40097

class CoilskarSoothsayerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CoilskarSoothsayerAI, MoonScriptCreatureAI);
	CoilskarSoothsayerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(COILSKAR_SOOTHSAYER_HOLY_NOVA, Target_Self, 10, 0, 20);
		AddSpell(COILSKAR_SOOTHSAYER_RESTORATION, Target_WoundedFriendly, 8, 1.5, 35);
	}
};

#define CN_COILSKAR_WRANGLER					22877
#define COILSKAR_WRANGLER_CLEAVE				15284
#define COILSKAR_WRANGLER_ELECTRIC_SPUR			40076
#define COILSKAR_WRANGLER_LIGHTNING_PROD		40066

class CoilskarWranglerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CoilskarWranglerAI, MoonScriptCreatureAI);
	CoilskarWranglerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(COILSKAR_WRANGLER_CLEAVE, Target_Current, 10, 0, 20, 0, 10);
		AddSpell(COILSKAR_WRANGLER_ELECTRIC_SPUR, Target_Self, 8, 3, 45);					// Should fraize Leviathan, but it does not (core? or script? :<)
		AddSpell(COILSKAR_WRANGLER_LIGHTNING_PROD, Target_RandomPlayer, 8, 3, 25);
	}
};

#define CN_DRAGONMAW_SKY_STALKER				23030
#define DRAGONMAW_SKY_STALKER_IMMOLATION_ARROW	40872
#define DRAGONMAW_SKY_STALKER_SHOOT				40873

class DragonmawSkyStalkerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DragonmawSkyStalkerAI, MoonScriptCreatureAI);
	DragonmawSkyStalkerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DRAGONMAW_SKY_STALKER_IMMOLATION_ARROW, Target_Current, 8, 2, 15, 0, 40);
		AddSpell(DRAGONMAW_SKY_STALKER_SHOOT, Target_Current, 75, 0, 1, 0, 40);
	}

	void OnCombatStart(Unit* pTarget)
	{
		ParentClass::OnCombatStart(pTarget);
		if (GetRangeToUnit(pTarget) <= 40.0f)
		{
			SetBehavior(Behavior_Spell);
			SetCanMove(false);
		}
	}

	void AIUpdate()
	{
		ParentClass::AIUpdate();

		Unit* pTarget = _unit->GetAIInterface()->getNextTarget();
		if (pTarget != NULL)
		{
			if (GetRangeToUnit(pTarget) <= 40.0f)
			{
				SetBehavior(Behavior_Spell);
				SetCanMove(false);
			}
		}
	}
};
// Should it run away from tank when he's close?
#define CN_DRAGONMAW_WIND_REAVER					23330
#define DRAGONMAW_WIND_REAVER_DOOM_BOLT				40876
#define DRAGONMAW_WIND_REAVER_FIREBALL				40877
#define DRAGONMAW_WIND_REAVER_FREEZE				40875

class DragonmawWindReaverAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DragonmawWindReaverAI, MoonScriptCreatureAI);
	DragonmawWindReaverAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DRAGONMAW_WIND_REAVER_DOOM_BOLT, Target_Current, 10, 1.5, 15, 0, 40);
		AddSpell(DRAGONMAW_WIND_REAVER_FIREBALL, Target_Current, 75, 2, 0, 0, 40);
		AddSpell(DRAGONMAW_WIND_REAVER_FREEZE, Target_RandomPlayer, 10, 2, 15);
	}

	void OnCombatStart(Unit* pTarget)
	{
		ParentClass::OnCombatStart(pTarget);
		if (GetRangeToUnit(pTarget) <= 40.0f)
		{
			SetBehavior(Behavior_Spell);
			SetCanMove(false);
		}
	}

	void AIUpdate()
	{
		ParentClass::AIUpdate();

		Unit* pTarget = _unit->GetAIInterface()->getNextTarget();
		if (pTarget != NULL)
		{
			if (GetRangeToUnit(pTarget) <= 40.0f)
			{
				SetBehavior(Behavior_Spell);
				SetCanMove(false);
			}
		}
	}
};

#define CN_DRAGONMAW_WYRMCALLER					22960
#define DRAGONMAW_WYRMCALLER_CLEAVE				15284
#define DRAGONMAW_WYRMCALLER_FIXATE				40892
#define DRAGONMAW_WYRMCALLER_JAB				40895

class DragonmawWyrmcallerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DragonmawWyrmcallerAI, MoonScriptCreatureAI);
	DragonmawWyrmcallerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DRAGONMAW_WYRMCALLER_CLEAVE, Target_Current, 10, 0, 15, 0, 10);
		AddSpell(DRAGONMAW_WYRMCALLER_FIXATE, Target_RandomUnit, 7, 0, 20);
		AddSpell(DRAGONMAW_WYRMCALLER_JAB, Target_Current, 8, 0, 25, 0, 10);
	}
};

#define CN_ENSLAVED_SERVANT						22965
#define ENSLAVED_SERVANT_KIDNEY_SHOT			41389
#define ENSLAVED_SERVANT_UPPERCUT				41388

class EnslavedServantAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(EnslavedServantAI, MoonScriptCreatureAI);
	EnslavedServantAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ENSLAVED_SERVANT_KIDNEY_SHOT, Target_Current, 7, 0, 25, 0, 10);
		AddSpell(ENSLAVED_SERVANT_UPPERCUT, Target_RandomUnit, 8, 0, 20);

		mHealthResetTimer = -1;
	}

	void OnCombatStart(Unit* pTarget)
	{
		ParentClass::OnCombatStart(pTarget);
		mHealthResetTimer = AddTimer(45000);	// to check
	}

	void AIUpdate()
	{
		if (IsTimerFinished(mHealthResetTimer))
		{
			_unit->SetHealth(_unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH));	// Found such note about this mob
			ResetTimer(mHealthResetTimer, 45000);
		}

		ParentClass::AIUpdate();
	}

	int32	mHealthResetTimer;
};

#define CN_HAND_OF_GOREFIEND				23172
#define HAND_OF_GOREFIEND_FRENZY			38166

class HandOfGorefiendAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(HandOfGorefiendAI, MoonScriptCreatureAI);
	HandOfGorefiendAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(HAND_OF_GOREFIEND_FRENZY, Target_Self, 6, 0, 45);
	}
};
// Mechanics are guessed. I'm also not sure if it's not typical caster unit
#define CN_ILLIDARI_ARCHON					23400
#define ILLIDARI_ARCHON_HEAL				41372
#define ILLIDARI_ARCHON_HOLY_SMITE			41370
#define ILLIDARI_ARCHON_MIND_BLAST			41374
#define ILLIDARI_ARCHON_POWER_WORD_SHIELD	41373
#define ILLIDARI_ARCHON_SHADOW_WORD_DEATH	41375
#define ILLIDARI_ARCHON_SHADOWFORM			29406

class IllidariArchonAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(IllidariArchonAI, MoonScriptCreatureAI);
	IllidariArchonAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		mIsShadowPriest = false;
		switch (RandomUInt(1))
		{
		case 0:
			AddSpell(ILLIDARI_ARCHON_HEAL, Target_WoundedFriendly, 5, 2, 30);
			AddSpell(ILLIDARI_ARCHON_HOLY_SMITE, Target_Current, 8, 2.5, 25, 0, 40);
			AddSpell(ILLIDARI_ARCHON_POWER_WORD_SHIELD, Target_Self, 7, 0, 35);
			break;
		case 1:
			AddSpell(ILLIDARI_ARCHON_MIND_BLAST, Target_Current, 8, 1.5, 25, 0, 30);
			AddSpell(ILLIDARI_ARCHON_SHADOW_WORD_DEATH, Target_RandomPlayer, 7, 0, 35);
			ApplyAura(ILLIDARI_ARCHON_SHADOWFORM);
			mIsShadowPriest = true;
			break;
		}
	}

	void OnCombatStop(Unit* pTarget)
	{
		ParentClass::OnCombatStop(pTarget);
		if (mIsShadowPriest && IsAlive())
		{
			ApplyAura(ILLIDARI_ARCHON_SHADOWFORM);
		}
	}

	void AIUpdate()
	{
		SpellDesc *pDeath = FindSpellById(ILLIDARI_ARCHON_SHADOW_WORD_DEATH);
		if (mIsShadowPriest && _unit->GetUInt32Value(UNIT_FIELD_HEALTH) <= 2500 && pDeath->mEnabled)
		{
			Spell* pCurrentSpell = _unit->GetCurrentSpell();
			if (pCurrentSpell != NULL && pCurrentSpell->pSpellId == pDeath->mInfo->Id)
			{
				pCurrentSpell->cancel();
			}
			if( !mQueuedSpells.empty() )
				for (SpellDescList::iterator itr = mQueuedSpells.begin(); itr != mQueuedSpells.end(); ++itr)
				{
					if ((*itr)->mInfo->Id == pDeath->mInfo->Id)
						mQueuedSpells.erase(itr);
				}
			if( !mScheduledSpells.empty() )
				for (SpellDescList::iterator itr = mScheduledSpells.begin(); itr != mScheduledSpells.end(); ++itr)
				{
					if ((*itr)->mInfo->Id == pDeath->mInfo->Id)
						mScheduledSpells.erase(itr);
				}

			pDeath->mEnabled = false;
		}

		ParentClass::AIUpdate();
	}

	bool	mIsShadowPriest;
};
// Couldn't find mechanics nowhere around the net, so kept it simple
#define CN_ILLIDARI_ASSASSIN				23403
#define ILLIDARI_ASSASSIN_PARALYZING_POISON	3609
#define ILLIDARI_ASSASSIN_VANISH			39667

class IllidariAssassinAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(IllidariAssassinAI, MoonScriptCreatureAI);
	IllidariAssassinAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ILLIDARI_ASSASSIN_PARALYZING_POISON, Target_Current, 8, 0, 25, 0, 10);
		AddSpell(ILLIDARI_ASSASSIN_VANISH, Target_Self, 7, 1, 30);
	}

	void AIUpdate()
	{
		if (_unit->HasAura(ILLIDARI_ASSASSIN_VANISH))
		{
			DelayNextAttack(1500);
		}

		ParentClass::AIUpdate();
	}
};
// I've parted it on frost and fire mage - correct me if it's wrong (also slap me if it's typical caster)
#define CN_ILLIDARI_BATTLEMAGE					23402
#define ILLIDARI_BATTLEMAGE_BLIZZARD			41382
#define ILLIDARI_BATTLEMAGE_FIREBALL			41383
#define ILLIDARI_BATTLEMAGE_FLAMESTRIKE			41379
#define ILLIDARI_BATTLEMAGE_FROSTBOLT			41384

class IllidariBattlemageAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(IllidariBattlemageAI, MoonScriptCreatureAI);
	IllidariBattlemageAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		switch (RandomUInt(1))
		{
		case 0:
			AddSpell(ILLIDARI_BATTLEMAGE_BLIZZARD, Target_RandomPlayerDestination, 8, 8, 35);
			AddSpell(ILLIDARI_BATTLEMAGE_FROSTBOLT, Target_Current, 15, 0, 10, 0, 40);
			break;
		case 1:
			AddSpell(ILLIDARI_BATTLEMAGE_FIREBALL, Target_Current, 15, 0, 10, 0, 40);
			AddSpell(ILLIDARI_BATTLEMAGE_FLAMESTRIKE, Target_RandomPlayerDestination, 8, 0, 40);
			break;
		}
	}
};

#define CN_ILLIDARI_BLOOD_LORD						23397
#define ILLIDARI_BLOOD_LORD_DIVINE_SHIELD			41367
#define ILLIDARI_BLOOD_LORD_HAMMER_OF_JUSTICE		13005
#define ILLIDARI_BLOOD_LORD_JUDGEMENT_OF_COMMAND	41368

class IllidariBloodLordAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(IllidariBloodLordAI, MoonScriptCreatureAI);
	IllidariBloodLordAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ILLIDARI_BLOOD_LORD_DIVINE_SHIELD, Target_Self, 8, 0, 30);
		AddSpell(ILLIDARI_BLOOD_LORD_HAMMER_OF_JUSTICE, Target_Current, 9, 0, 20, 0, 10);
		AddSpell(ILLIDARI_BLOOD_LORD_JUDGEMENT_OF_COMMAND, Target_Current, 8, 0, 25, 0, 10);
	}
};
// Should be summoned by Priestess of Dementia
#define CN_IMAGE_OF_DEMENTIA						23436
#define IMAGE_OF_DEMENTIA_WHRILWIND					41399
#define IMAGE_OF_DEMENTIA_WHIRLWIND2				41400

void SpellFunc_Whirlwind(SpellDesc *pThis, MoonScriptCreatureAI *pCreatureAI, Unit* pTarget, TargetType pType);

class ImageOfDementiaAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ImageOfDementiaAI, MoonScriptCreatureAI);
	ImageOfDementiaAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		mWhirlwind = AddSpell(IMAGE_OF_DEMENTIA_WHRILWIND, Target_Self, 0, 15, 0);
		AddSpellFunc(&SpellFunc_Whirlwind, Target_Self, 15, 15, 35, 0, 10);
	}

	SpellDesc*	mWhirlwind;
};

void SpellFunc_Whirlwind(SpellDesc *pThis, MoonScriptCreatureAI *pCreatureAI, Unit* pTarget, TargetType pType)
{
	ImageOfDementiaAI *Dementia = (pCreatureAI != NULL) ? (ImageOfDementiaAI*)pCreatureAI : NULL;
	if (Dementia != NULL)
	{
		Dementia->CastSpell(Dementia->mWhirlwind);
		Dementia->Despawn(25000);
	}
}

#define CN_SHADOWMOON_BLOOD_MAGE					22945
#define SHADOWMOON_BLOOD_MAGE_BLOOD_SIPHON			41068
#define SHADOWMOON_BLOOD_MAGE_BLOOD_BOLT			41072

class ShadowmoonBloodMageAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ShadowmoonBloodMageAI, MoonScriptCreatureAI);
	ShadowmoonBloodMageAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SHADOWMOON_BLOOD_MAGE_BLOOD_SIPHON, Target_Self, 8, 0, 35);
		AddSpell(SHADOWMOON_BLOOD_MAGE_BLOOD_BOLT, Target_RandomPlayer, 9, 3, 25);
	}
};

#define CN_SHADOWMOON_CHAMPION						22880
#define SHADOWMOON_CHAMPION_CHAOTIC_LIGHT			41063
#define SHADOWMOON_CHAMPION_WHIRLING_BLADE			41053

class ShadowmoonChampionAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ShadowmoonChampionAI, MoonScriptCreatureAI);
	ShadowmoonChampionAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SHADOWMOON_CHAMPION_CHAOTIC_LIGHT, Target_Current, 8, 0, 25, 0, 10);
		AddSpell(SHADOWMOON_CHAMPION_WHIRLING_BLADE, Target_RandomPlayer, 8, 1, 30);	// I must check its mechanics
	}
};

#define CN_SHADOWMOON_DEATHSHAPER					22882
#define SHADOWMOON_DEATHSHAPER_DEATH_COIL			41070
#define SHADOWMOON_DEATHSHAPER_DEMON_ARMOR			13787
#define SHADOWMOON_DEATHSHAPER_RAISE_DEAD			41071
#define SHADOWMOON_DEATHSHAPER_SHADOW_BOLT			41069

void SpellFunc_RaiseDead(SpellDesc *pThis, MoonScriptCreatureAI *pCreatureAI, Unit* pTarget, TargetType pType);

class ShadowmoonDeathshaperAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ShadowmoonDeathshaperAI, MoonScriptCreatureAI);
	ShadowmoonDeathshaperAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SHADOWMOON_DEATHSHAPER_DEATH_COIL, Target_RandomPlayer, 7, 0, 30);
		AddSpell(SHADOWMOON_DEATHSHAPER_DEMON_ARMOR, Target_Self, 8, 0, 60);
		mRaiseDead = AddSpell(SHADOWMOON_DEATHSHAPER_RAISE_DEAD, Target_Self, 0, 1.5, 0);
		AddSpellFunc(&SpellFunc_RaiseDead, Target_RandomCorpse, 12, 1.5, 30);
		AddSpell(SHADOWMOON_DEATHSHAPER_SHADOW_BOLT, Target_Current, 75, 3, 0, 0, 40);	// Typical caster? Slap me if not...

		_unit->SetUInt32Value(UNIT_FIELD_POWER1, 100000);	// temporary way to set up mana of this unit
	}

	void OnCombatStart(Unit* pTarget)
	{
		ParentClass::OnCombatStart(pTarget);
		if (GetRangeToUnit(pTarget) <= 40.0f)
		{
			SetBehavior(Behavior_Spell);
			SetCanMove(false);
		}
	}

	void AIUpdate()
	{
		ParentClass::AIUpdate();

		Unit* pTarget = _unit->GetAIInterface()->getNextTarget();
		if (pTarget != NULL)
		{
			if (GetRangeToUnit(pTarget) <= 40.0f)
			{
				SetBehavior(Behavior_Spell);
				SetCanMove(false);
			}
		}
	}

	SpellDesc*	mRaiseDead;
};

void SpellFunc_RaiseDead(SpellDesc *pThis, MoonScriptCreatureAI *pCreatureAI, Unit* pTarget, TargetType pType)
{
	ShadowmoonDeathshaperAI *pDeathshaper = (pCreatureAI != NULL) ? (ShadowmoonDeathshaperAI*)pCreatureAI : NULL;
	if (pDeathshaper != NULL)
	{
		pDeathshaper->CastSpellNowNoScheduling(pDeathshaper->mRaiseDead);
		MoonScriptCreatureAI *pAI = pDeathshaper->SpawnCreature(23371, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), pTarget->GetOrientation());
		if (pAI != NULL)
		{
			pAI->GetUnit()->GetAIInterface()->StopMovement(2500);
			pAI->DelayNextAttack(2500);
		}

		TO_CREATURE(pTarget)->Despawn(3000, 0);
	}
}
// No entry in my DB, but AI may stay :)
#define CN_SHADOWMOON_FALLEN					23371

class ShadowmoonFallenAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ShadowmoonFallenAI, MoonScriptCreatureAI);
	ShadowmoonFallenAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AggroNearestPlayer(200);
	}
};

#define CN_SHADOWMOON_HOUNDMASTER						23018
#define SHADOWMOON_HOUNDMASTER_FLARE					41094
#define SHADOWMOON_HOUNDMASTER_FREEZING_TRAP			41085
#define SHADOWMOON_HOUNDMASTER_SHOOT					41093
#define SHADOWMOON_HOUNDMASTER_SILENCING_SHOT			41084
#define SHADOWMOON_HOUNDMASTER_SUMMON_RIDING_WARHOUND	39906
#define SHADOWMOON_HOUNDMASTER_VOLLEY					41091
#define SHADOWMOON_HOUNDMASTER_WING_CLIP				32908

class ShadowmoonHoundmasterAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ShadowmoonHoundmasterAI, MoonScriptCreatureAI);
	ShadowmoonHoundmasterAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SHADOWMOON_HOUNDMASTER_FLARE, Target_RandomPlayerDestination, 5, 0, 30);
		AddSpell(SHADOWMOON_HOUNDMASTER_FREEZING_TRAP, Target_Self, 5, 0, 30);
		AddSpell(SHADOWMOON_HOUNDMASTER_SHOOT, Target_Current, 70, 0, 1, 0, 30);
		AddSpell(SHADOWMOON_HOUNDMASTER_SILENCING_SHOT, Target_RandomPlayer, 6, 0, 35);
		AddSpell(SHADOWMOON_HOUNDMASTER_SUMMON_RIDING_WARHOUND, Target_Self, 5, 0, 45);
		AddSpell(SHADOWMOON_HOUNDMASTER_VOLLEY, Target_RandomPlayerDestination, 5, 1, 25);
		AddSpell(SHADOWMOON_HOUNDMASTER_WING_CLIP, Target_Current, 5, 0, 20, 0, 10, true);
	}

	void OnCombatStart(Unit* pTarget)
	{
		ParentClass::OnCombatStart(pTarget);
		if (GetRangeToUnit(pTarget) <= 30.0f)
		{
			SetBehavior(Behavior_Spell);
			SetCanMove(false);
		}
	}

	void AIUpdate()
	{
		ParentClass::AIUpdate();
		Unit* pTarget = _unit->GetAIInterface()->getNextTarget();
		if (pTarget != NULL)
		{
			if (GetRangeToUnit(pTarget) <= 30.0f)
			{
				SetBehavior(Behavior_Spell);
				SetCanMove(false);
			}
		}
	}
};

#define CN_SHADOWMOON_REAVER					22879
#define SHADOWMOON_REAVER_SPELL_ABSORPTION		41034

class ShadowmoonReaverAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ShadowmoonReaverAI, MoonScriptCreatureAI);
	ShadowmoonReaverAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SHADOWMOON_REAVER_SPELL_ABSORPTION, Target_Self, 8, 0, 35);
	}
};

#define CN_SHADOWMOON_SOLDIER					23047
#define SHADOWMOON_SOLDIER_STRIKE				11976

class ShadowmoonSoldierAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ShadowmoonSoldierAI, MoonScriptCreatureAI);
	ShadowmoonSoldierAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SHADOWMOON_SOLDIER_STRIKE, Target_Current, 10, 0, 15, 0, 10);
	}
};
// Haven't found informations about Shield Wall ability
#define CN_SHADOWMOON_WEAPON_MASTER					23049
#define SHADOWMOON_WEAPON_MASTER_BATTLE_AURA		41106
#define SHADOWMOON_WEAPON_MASTER_BATTLE_STANCE		41099
#define SHADOWMOON_WEAPON_MASTER_BERSEKER_AURA		41107
#define SHADOWMOON_WEAPON_MASTER_BERSERKER_STANCE	41100
#define SHADOWMOON_WEAPON_MASTER_DEFENSIVE_AURA		41105
#define SHADOWMOON_WEAPON_MASTER_DEFENSIVE_STANCE	41101
#define SHADOWMOON_WEAPON_MASTER_KNOCK_AWAY			18813
#define SHADOWMOON_WEAPON_MASTER_MUTILATE			41103
#define SHADOWMOON_WEAPON_MASTER_WHIRLWIND			41097
#define SHADOWMOON_WEAPON_MASTER_WHIRLWIND2			41098
#define SHADOWMOON_WEAPON_MASTER_SHIELD_WALL		41104	// guessed, but close Id :)

void SpellFunc_DefensiveStance(SpellDesc *pThis, MoonScriptCreatureAI *pCreatureAI, Unit* pTarget, TargetType pType);
void SpellFunc_BerserkerStance(SpellDesc *pThis, MoonScriptCreatureAI *pCreatureAI, Unit* pTarget, TargetType pType);

class ShadowmoonWeaponMasterAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ShadowmoonWeaponMasterAI, MoonScriptBossAI);
	ShadowmoonWeaponMasterAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
	{
		AddPhaseSpell(1, AddSpell(SHADOWMOON_WEAPON_MASTER_KNOCK_AWAY, Target_Current, 9, 0, 25, 0, 10));
		AddPhaseSpell(1, AddSpell(SHADOWMOON_WEAPON_MASTER_MUTILATE, Target_Current, 8, 0, 30, 0, 10));
		AddPhaseSpell(2, AddSpell(SHADOWMOON_WEAPON_MASTER_KNOCK_AWAY, Target_Current, 9, 0, 25, 0, 10));
		AddPhaseSpell(2, AddSpell(SHADOWMOON_WEAPON_MASTER_SHIELD_WALL, Target_Self, 10, 0, 35));
		AddPhaseSpell(3, AddSpell(SHADOWMOON_WEAPON_MASTER_KNOCK_AWAY, Target_Current, 9, 0, 25, 0, 10));
		AddPhaseSpell(3, AddSpell(SHADOWMOON_WEAPON_MASTER_WHIRLWIND, Target_Self, 10, 15, 35));
		AddPhaseSpell(3, AddSpell(SHADOWMOON_WEAPON_MASTER_MUTILATE, Target_Current, 8, 0, 30, 0, 10));
		mDefensiveStance = AddSpellFunc(&SpellFunc_DefensiveStance, Target_Self, 0, 0, 0);
		mBerserkerStance = AddSpellFunc(&SpellFunc_BerserkerStance, Target_Self, 0, 0, 0);
		//SetDisplayWeaponIds(0, 0)	// Sword
		ApplyAura(SHADOWMOON_WEAPON_MASTER_BATTLE_STANCE);
		ApplyAura(SHADOWMOON_WEAPON_MASTER_BATTLE_AURA);
	}

	void OnCombatStop(Unit* pTarget)
	{
		ParentClass::OnCombatStop(pTarget);

		if (IsAlive())
		{
			RemoveAura(SHADOWMOON_WEAPON_MASTER_DEFENSIVE_AURA);
			RemoveAura(SHADOWMOON_WEAPON_MASTER_BERSEKER_AURA);
			//SetDisplayWeaponIds(0, 0)	// Sword
			ApplyAura(SHADOWMOON_WEAPON_MASTER_BATTLE_STANCE);
			ApplyAura(SHADOWMOON_WEAPON_MASTER_BATTLE_AURA);
		}
	}

	void AIUpdate()
	{
		if (GetPhase() == 1 && GetHealthPercent() <= 85)
		{
			SetPhase(2, mDefensiveStance);
			return;
		}
		if (GetPhase() == 2 && GetHealthPercent() <= 35)
		{
			SetPhase(3, mBerserkerStance);
			return;
		}

		ParentClass::AIUpdate();
	}

	SpellDesc*	mDefensiveStance;
	SpellDesc*	mBerserkerStance;
};

void SpellFunc_DefensiveStance(SpellDesc *pThis, MoonScriptCreatureAI *pCreatureAI, Unit* pTarget, TargetType pType)
{
	ShadowmoonWeaponMasterAI *pWeaponMaster = (pCreatureAI != NULL) ? (ShadowmoonWeaponMasterAI*)pCreatureAI : NULL;
	if (pWeaponMaster != NULL)
	{
		pWeaponMaster->RemoveAura(SHADOWMOON_WEAPON_MASTER_BATTLE_AURA);
		//SetDisplayWeaponIds(0, 0)	// Axe + Shield
		pWeaponMaster->ApplyAura(SHADOWMOON_WEAPON_MASTER_DEFENSIVE_STANCE);
		pWeaponMaster->ApplyAura(SHADOWMOON_WEAPON_MASTER_DEFENSIVE_AURA);
	}
}

void SpellFunc_BerserkerStance(SpellDesc *pThis, MoonScriptCreatureAI *pCreatureAI, Unit* pTarget, TargetType pType)
{
	ShadowmoonWeaponMasterAI *pWeaponMaster = (pCreatureAI != NULL) ? (ShadowmoonWeaponMasterAI*)pCreatureAI : NULL;
	if (pWeaponMaster != NULL)
	{
		pWeaponMaster->RemoveAura(SHADOWMOON_WEAPON_MASTER_DEFENSIVE_AURA);
		//SetDisplayWeaponIds(0, 0)	// Sword
		pWeaponMaster->ApplyAura(SHADOWMOON_WEAPON_MASTER_BERSERKER_STANCE);
		pWeaponMaster->ApplyAura(SHADOWMOON_WEAPON_MASTER_BERSEKER_AURA);
		pWeaponMaster->Emote("Berserker stance! Attack them recklessly!", Text_Say, 0);
	}
}

#define CN_SPELLBOUND_ATTENDANT						22959
#define SPELLBOUND_ATTENDANT_KICK					41395
#define SPELLBOUND_ATTENDANT_SLEEP					41396

class SpellboundAttendantAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SpellboundAttendantAI, MoonScriptCreatureAI);
	SpellboundAttendantAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SPELLBOUND_ATTENDANT_KICK, Target_Current, 9, 0, 25, 0, 10);
		AddSpell(SPELLBOUND_ATTENDANT_SLEEP, Target_RandomPlayerNotCurrent, 8, 1, 25);
	}
};

#define CN_TEMPLE_CONCUBINE							22939
#define TEMPLE_CONCUBINE_LOVE_TAP					41338
#define TEMPLE_CONCUBINE_POLYMORPH					41334

class TempleConcubineAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(TempleConcubineAI, MoonScriptCreatureAI);
	TempleConcubineAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(TEMPLE_CONCUBINE_LOVE_TAP, Target_Current, 9, 0, 25, 0, 10);
		AddSpell(TEMPLE_CONCUBINE_POLYMORPH, Target_RandomPlayerNotCurrent, 7, 1, 25);
	}
};

//////////////////////////////////////////////////////////////////////////
/////// Elemental AIs

#define CN_STORM_FURY				22848
#define STORM_FURY_STORM_BLINK		39581

void SpellFunc_StormBlink(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);

class StormFuryAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(StormFuryAI, MoonScriptCreatureAI);
	StormFuryAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		mStormBlink = AddSpell(STORM_FURY_STORM_BLINK, Target_Self, 0, 1, 0);	// Mechanics was guessed
		AddSpellFunc(&SpellFunc_StormBlink, Target_RandomPlayerNotCurrent, 8, 0, 35);
	}

	void AIUpdate()
	{
		if (_unit->HasAura(STORM_FURY_STORM_BLINK))
		{
			DelayNextAttack(2000);
		}

		ParentClass::AIUpdate();
	}

	SpellDesc*	mStormBlink;
};

void SpellFunc_StormBlink(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType)
{
	StormFuryAI *pStormFuryAI = (pCreatureAI != NULL) ? (StormFuryAI*)pCreatureAI : NULL;
	if (pStormFuryAI != NULL)
	{
		Unit* pCurrentTarget = pStormFuryAI->GetUnit()->GetAIInterface()->getNextTarget();
		if (pCurrentTarget == NULL)
			return;

		if (pTarget == pCurrentTarget)
			return;

		pStormFuryAI->ClearHateList();
		pStormFuryAI->GetUnit()->GetAIInterface()->AttackReaction(pTarget, 500);
		pStormFuryAI->GetUnit()->GetAIInterface()->setNextTarget(pTarget);
		pStormFuryAI->GetUnit()->GetAIInterface()->RemoveThreatByPtr(pCurrentTarget);
		pStormFuryAI->CastSpell(pStormFuryAI->mStormBlink);
	}
}

#define CN_AQUEOUS_SURGER					22881
#define AQUEOUS_SURGER_POISON_BOLT_VOLLEY	40095

class AqueousSurgerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AqueousSurgerAI, MoonScriptCreatureAI);
	AqueousSurgerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(AQUEOUS_SURGER_POISON_BOLT_VOLLEY, Target_Self, 7, 0, 20);
	}
};

#define CN_AQUEOUS_SPAWN				22883
#define AQUEOUS_SPAWN_MERGE				40106
#define AQUEOUS_SPAWN_SLUDGE_NOVA		40102

class AqueousSpawnAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AqueousSpawnAI, MoonScriptCreatureAI);
	AqueousSpawnAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(AQUEOUS_SPAWN_MERGE, Target_Self, 7, 11, 45);
		AddSpell(AQUEOUS_SPAWN_SLUDGE_NOVA, Target_RandomPlayer, 8, 2.5, 20);
	}
};

#define CN_AQUEOUS_LORD					22878
#define AQUEOUS_LORD_CRASHING_WAVE		40100
#define AQUEOUS_LORD_VILE_SLIME			40099

void SpellFunc_SpawnAqueousSpawn(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);

class AqueousLordAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AqueousLordAI, MoonScriptCreatureAI);
	AqueousLordAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(AQUEOUS_LORD_CRASHING_WAVE, Target_Self, 8, 0, 35);
		AddSpell(AQUEOUS_LORD_VILE_SLIME, Target_RandomPlayer, 10, 0, 25);
		AddSpellFunc(&SpellFunc_SpawnAqueousSpawn, Target_Self, 10, 0, 30);
	}
};

void SpellFunc_SpawnAqueousSpawn(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType)
{
	AqueousLordAI *pAqueousLordAI = (pCreatureAI != NULL) ? (AqueousLordAI*)pCreatureAI : NULL;
	if (pAqueousLordAI != NULL)
	{
		MoonScriptCreatureAI *pSpawnAI = pAqueousLordAI->SpawnCreature(CN_AQUEOUS_SPAWN);
		if (pSpawnAI != NULL)
		{
			pSpawnAI->AggroRandomUnit(500);
			pSpawnAI->SetDespawnWhenInactive(true);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/////// Mechanical AIs

#define CN_PROMENADE_SENTINEL				23394
#define PROMENADE_SENTINEL_L5_ARCANE_CHARGE	41360

class PromenadeSentinelAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(PromenadeSentinelAI, MoonScriptCreatureAI);
	PromenadeSentinelAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(PROMENADE_SENTINEL_L5_ARCANE_CHARGE, Target_RandomPlayer, 8, 2.7f, 35);
	}
};

//////////////////////////////////////////////////////////////////////////
/////// Uncategorized AIs

#define CN_ANGERED_SOUL_FRAGMENT			23398
#define ANGERED_SOUL_FRAGMENT_ANGER			41986

class AngeredSoulFragmentAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AngeredSoulFragmentAI, MoonScriptCreatureAI);
	AngeredSoulFragmentAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ANGERED_SOUL_FRAGMENT_ANGER, Target_Self, 10, 3, 25);
	}
};

#define CN_ASHTONGUE_FERAL_SPIRIT				22849
#define ASHTONGUE_FERAL_SPIRIT_SPIRIT_BOND		39578	// won't be used for now
#define ASHTONGUE_FERAL_SPIRIT_CHARGE_FRENZY	39575	// won't be added for now (can cause crashes - self-kills)

class AshtongueFeralSpiritAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AshtongueFeralSpiritAI, MoonScriptCreatureAI);
	AshtongueFeralSpiritAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
	}
};

//////////////////////////////////////////////////////////////////////////
/////// Undead AIs

#define CN_ENSLAVED_SOUL				23469
#define ENSLAVED_SOUL_SOUL_RELEASE		41542

class EnslavedSoulAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(EnslavedSoulAI, MoonScriptCreatureAI);
	EnslavedSoulAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) {}

	void OnDied(Unit* mKiller)
	{
		ApplyAura(ENSLAVED_SOUL_SOUL_RELEASE);			// beg core to support OnDied casts
		Despawn(120000, 0);
		ParentClass::OnDied(mKiller);
	}
};

#define CN_HUNGERING_SOUL_FRAGMENT					23401
#define HUNGERING_SOUL_FRAGMENT_CONSUMING_STRIKES	41248

class HungeringSoulFragmentAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(HungeringSoulFragmentAI, MoonScriptCreatureAI);
	HungeringSoulFragmentAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		ApplyAura(HUNGERING_SOUL_FRAGMENT_CONSUMING_STRIKES);
	}

	void OnCombatStop(Unit* pTarget)
	{
		ParentClass::OnCombatStop(pTarget);
		if (IsAlive())
		{
			ApplyAura(HUNGERING_SOUL_FRAGMENT_CONSUMING_STRIKES);
		}
	}
};

#define CN_SHADOWY_CONSTRUCT					23111
#define SHADOWY_CONSTRUCT_ATROPHY				40327

class ShadowyConstructAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ShadowyConstructAI, MoonScriptCreatureAI);
	ShadowyConstructAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SHADOWY_CONSTRUCT_ATROPHY, Target_Current, 10, 0, 45, 0, 10);
	}
};

#define CN_SUFFERING_SOUL_FRAGMENT				23999
#define SUFFERING_SOUL_FRAGMENT_SOUL_BLAST		41245

class SufferingSoulFragmentAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SufferingSoulFragmentAI, MoonScriptCreatureAI);
	SufferingSoulFragmentAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SUFFERING_SOUL_FRAGMENT_SOUL_BLAST, Target_Self, 8, 2, 30);
	}
};
// Teron Gorefiend transforms random player into it with those abilities, but AI might be handy too (too overpowered?) - cannot check (no npc in DB)
#define CN_VANGEFUL_SPIRIT					23109
#define VANGEFUL_SPIRIT_SPIRIT_CHAINS		40175
#define VANGEFUL_SPIRIT_SPIRIT_LANCE		40157
#define VANGEFUL_SPIRIT_SPIRIT_SHIELD		40322
#define VANGEFUL_SPIRIT_SPIRIT_STRIKE		40325
#define VANGEFUL_SPIRIT_SPIRIT_VOLLEY		40314

class VangefulSpiritAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(VangefulSpiritAI, MoonScriptCreatureAI);
	VangefulSpiritAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(VANGEFUL_SPIRIT_SPIRIT_CHAINS, Target_Self, 8, 0, 25);
		AddSpell(VANGEFUL_SPIRIT_SPIRIT_LANCE, Target_RandomPlayer, 6, 0, 35);	// Current?
		AddSpell(VANGEFUL_SPIRIT_SPIRIT_SHIELD, Target_Self, 6, 0, 50);
		AddSpell(VANGEFUL_SPIRIT_SPIRIT_STRIKE, Target_Current, 9, 0, 15, 0, 10);
		AddSpell(VANGEFUL_SPIRIT_SPIRIT_VOLLEY, Target_Self, 4, 0, 40);
	}
};

#define CN_WRATHBONE_FLAYER				22953
#define WRATHBONE_FLAYER_CLEAVE			15496
#define WRATHBONE_FLAYER_IGNORED		39544

class WrathboneFlayerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(WrathboneFlayerAI, MoonScriptCreatureAI);
	WrathboneFlayerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(WRATHBONE_FLAYER_CLEAVE, Target_Current, 10, 0, 15, 0, 10);
		AddSpell(WRATHBONE_FLAYER_IGNORED, Target_Current, 7, 0, 25, 0, 10);		// Current or random?
	}
};

// Bosses
//----------------------High Warlord Naj'entus--------------------------//

#define CN_NAJENTUS 22887

#define CRASHING_WAVE					40100	// Crashing Wave (2313-2687 Nature damage)
#define NEEDLE_SPINE					39835	// Needle Spine (3188-4312 Damage, AoE of 2375-2625 Aura): Fires a needle spine at an enemy target.
#define NEEDLE_SPINE_EXPLOSION			39968
#define TIDAL_SHIELD					39872	// Tidal Shield : Impervious to normal attack and spells. Regenerating health. Will deal 8500 frost damage to the raid when broken.
#define IMPALING_SPINE					39837
#define NAJENTUS_SPINE_GO				3264	// Campfire so it wont crash Real go is 185584 //Did not find it in any database (including drake's go's), tested with a chair, and it works
// There are also other sounds, but Idk where they should go (mostly specials and enrage - which erange spell is that O_O)
class NajentusAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(NajentusAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    NajentusAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 4;

		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(CRASHING_WAVE);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 15;

		spells[1].info = dbcSpell.LookupEntry(NEEDLE_SPINE);
		spells[1].targettype = TARGET_RANDOM_SINGLE;
		spells[1].instant = false;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 2000;
		spells[1].cooldown = 10;
		spells[1].mindist2cast = 0.0f;
		spells[1].maxdist2cast = 60.0f;

		spells[2].info = dbcSpell.LookupEntry(TIDAL_SHIELD);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = false;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 2000;
		spells[2].cooldown = 105;

		spells[3].info = dbcSpell.LookupEntry(IMPALING_SPINE);
		spells[3].targettype = TARGET_RANDOM_SINGLE;
		spells[3].instant = false;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 2000;
		spells[3].cooldown = 20;
		spells[3].mindist2cast = 0.0f;
		spells[3].maxdist2cast = 60.0f;
    }

    void OnCombatStart(Unit* mTarget)
    {
		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;

		uint32 t = (uint32)time(NULL);
		spells[2].casttime = t + 60;
		spells[3].casttime = t + 20;

		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You will die, in the name of Lady Vashj!");
		_unit->PlaySoundToSet(11450);

		RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
    }

	void OnCombatStop(Unit* mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

       RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Lord Illidan will... crush you.");
		_unit->PlaySoundToSet(11459);

		RemoveAIUpdateEvent();
    }

	void OnTargetDied(Unit* mTarget)
    {
		switch (RandomUInt(2))
		{
		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Time for you to go!");
			_unit->PlaySoundToSet(11456);
			break;
		default:
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your success was short lived!");	// not sure
				_unit->PlaySoundToSet(11455);
			}
		}
	}

	void AIUpdate()
	{	// Disabled till I find way to make it dispellable like on blizz
		uint32 t = (uint32)time(NULL);
		/*if (t > spells[2].casttime)
		{
			_unit->setAttackTimer(spells[2].attackstoptimer, false);

			_unit->CastSpell(_unit, spells[2].info, spells[2].instant);

			spells[2].casttime = t + spells[2].cooldown;
			spells[1].casttime = t + 55;
			spells[3].casttime = t + 65;
			return;
		}*/

		if (t > spells[3].casttime)
		{
			switch (RandomUInt(2))
			{
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Stick around!");
				_unit->PlaySoundToSet(11451);
				break;
			default:
				{
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I'll deal with you later!"); 
					_unit->PlaySoundToSet(11452);
				}
			}
			_unit->setAttackTimer(spells[3].attackstoptimer, false);

			CastSpellOnRandomTarget(3, spells[3].mindist2cast, spells[3].maxdist2cast, 0, 100);

			spells[3].casttime = t + spells[3].cooldown;
			return;
		}

		float val = RandomFloat(100.0f);
        SpellCast(val);
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
        {
			float comulativeperc = 0;
		    Unit* target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->getNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
							//_unit->GetMapMgr()->GetInterface()->SpawnGameObject(NAJENTUS_SPINE_GO , target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0.207343, true, 0, 0); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
        {
			std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && (*itr)->IsUnit()) // isAttackable(_unit, (*itr)) && 
				{
					Unit* RandomTarget = NULL;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget == _unit->GetAIInterface()->GetMostHated() && i == 3)
						continue;

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit*  RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}

protected:

	int nrspells;
};

//------------------------------------
//          -= Supremus =-
//------------------------------------

#define CN_SUPREMUS 22898

//Abilities - Phase 1
#define MOLTEN_FLAME 40253 //Molten Flame: Blue-flamed fire sent along the ground in a straight line toward random players that deals 3325-3675 fire damage every 1 second while stood on. Ability used every 20 seconds.
#define HURTFUL_STRIKE 40126 //33813 //Hurtful Strike: A massive melee ability applied to the second highest on his aggro list. If there are no other melee targets in range, it will be performed on the main tank instead.

//Abilities - Phase 2
//Gaze: Supremus will target a random player and follow that target around for 10 seconds at 90% of normal movement speed. Switches targets after 10 seconds.
#define MOLTEN_PUNCH 40126 //Molten Punch: Once a targeted player is within a 40 yard range, Supremus will do a knockback ability for 5250 damage.
#define VOLCANIC_GAZER 42055 //Volcanic Geyser: Emoting "the ground begins to crack open", Supremus will summon small volcanoes that spit out blue flames for 4163-4837 fire damage in a 15 yard radius.

class SupremusAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SupremusAI);
    SupremusAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		m_MoltenFlame = m_HurtfulStrike = m_MoltenPunch = m_VolcanicGazer = true;

        infoMoltenFlame = dbcSpell.LookupEntry(MOLTEN_FLAME);
		infoHurtfulStrike = dbcSpell.LookupEntry(HURTFUL_STRIKE);
		infoMoltenPunch =  dbcSpell.LookupEntry(MOLTEN_PUNCH);
		infoVolcanicGazer =  dbcSpell.LookupEntry(VOLCANIC_GAZER);
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Bear witness to the agent of your demise!");	// used when he kills Warden Mellichar
		_unit->PlaySoundToSet(11123);
		timer = 0;
    }

    void OnCombatStop(Unit* mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
       RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I am merely one of... infinite multitudes.");
		_unit->PlaySoundToSet(11126);
		RemoveAIUpdateEvent();
    }

	void OnTargetDied(Unit* mTarget)
    {
		if (_unit->GetHealthPct() > 0)
		{
			int RandomSpeach;
			RandomSpeach=rand()%2;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your fate is written!");	// this one needs verification
				_unit->PlaySoundToSet(11124);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The chaos I have sown here is but a taste....");
				_unit->PlaySoundToSet(11125);
				break;
			}
		}
    }

    void AIUpdate()
    {	
		switch(m_phase)
        {
        case 1:
            {
                PhaseOne();
            }break;
        case 2:
            {
                PhaseTwo();
            }break;
        default:
            {
                m_phase = 1;
            };
        };
    }
	
	void PhaseOne()
	{
		timer++;

        uint32 val = RandomUInt(1000);

        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())//_unit->getAttackTarget())
        {
			if(m_MoltenFlame)
            {
				_unit->CastSpell(_unit, infoMoltenFlame, false);
                m_MoltenFlame = false;
                return;
            }

			else if(m_HurtfulStrike)
            {
                _unit->CastSpell(_unit, infoHurtfulStrike, false);
                m_HurtfulStrike = false;
                return;
            }

            if(val >= 0 && val <= 500)
            {
                _unit->setAttackTimer(6000, false);//6000
                m_MoltenFlame = true;
            }

            else if(val > 500 && val <= 1000)
            {
                _unit->setAttackTimer(4000, false);//2000
                m_HurtfulStrike = true;
            }
        }
		
		if(timer >= 45)
        {
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "We span the universe, as countless as the stars!");
			_unit->PlaySoundToSet(11131);
			timer = 0;
            m_phase = 2;
		}
	}

	void PhaseTwo()
	{
		timer++;

		uint32 val = RandomUInt(1000);

        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())//_unit->getAttackTarget())
        {
			if(m_MoltenPunch)
            {
				_unit->CastSpell(_unit, infoMoltenPunch, false);
                m_MoltenPunch = false;
                return;
            }

			else if(m_VolcanicGazer)
            {
				//_unit->SendChatMessage(CHAT_MSG_MONSTER_EMOTE, LANG_UNIVERSAL, "The ground begins to crack open");
                _unit->CastSpell(_unit, infoVolcanicGazer, false);
                m_VolcanicGazer = false;
                return;
				
            }

            if(val >= 0 && val <= 500)
            {
                _unit->setAttackTimer(6000, false);//6000
                m_MoltenPunch = true;
            }

            else if(val > 500 && val <= 1000)
            {
                _unit->setAttackTimer(4000, false);//2000
                m_VolcanicGazer = true;
            }
        }

		if(timer >= 45)
        {
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "We span the universe, as countless as the stars!");
			_unit->PlaySoundToSet(11131);
			timer = 0;
            m_phase = 1;
			
		}
	}

protected:
	uint32 timer;
	uint32 m_phase;
	bool m_MoltenFlame, m_HurtfulStrike, m_MoltenPunch, m_VolcanicGazer;
    SpellEntry *infoMoltenFlame, *infoHurtfulStrike,*infoMoltenPunch, *infoVolcanicGazer;
};

//------------------------------------
//    -= Shade Of Akama =-
//------------------------------------
#define CN_SHADE_OF_AKAMA 22841
#define CN_ASHTONGUE_CHANNELER 23421
#define CN_ASHTONGUE_SORCERER 23215

#define CN_ASHTONGUE_DEFENDER 23216
#define DEBILITATING_STRIKE 41178

#define CN_ASHTONGUE_ELEMENTALIST 23523
#define RAIN_OF_FIRE 42023

#define CN_ASHTONGUE_ROGUE 23318
#define DEBILITATING_POISON 41978

#define CN_ASHTONGUE_SPIRITBINDER 23524
#define SPIRIT_MEND 42025
#define CHAIN_HEAL 42027

//-------------------------Gurtogg Bloodboil----------------------------//

#define CN_GURTOGG_BLOODBOIL 22948

// Phase 1
#define BLOODBOIL		42005
#define ACIDIC_WOUND	40481
#define DISORIENT		32752
#define KNOCKBACK		38576

// Phase 2
#define FEL_RAGE1		40604
#define FEL_RAGE2		40594	// 40616 - doesn't work at all
#define ACID_GEYSER		40629

// Spells common for both phases
#define ARCING_SMASH	40599
#define FEL_ACID_BREATH	40595

class GurtoggAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(GurtoggAI);
	SP_AI_Spell spells[9];
	bool m_spellcheck[9];

    GurtoggAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 5;

		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(ARCING_SMASH);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 10;

		spells[1].info = dbcSpell.LookupEntry(FEL_ACID_BREATH);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 2000;
		spells[1].cooldown = 15;

		// Phase 1
		spells[2].info = dbcSpell.LookupEntry(BLOODBOIL);
		spells[2].targettype = TARGET_VARIOUS;
		spells[2].instant = true;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 2000;
		spells[2].cooldown = 10;

		// Not sure about this one system of casting
		spells[3].info = dbcSpell.LookupEntry(ACIDIC_WOUND);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = true;
		spells[3].perctrigger = 10.0f;
		spells[3].attackstoptimer = 2000;
		spells[3].cooldown = 15;

		spells[4].info = dbcSpell.LookupEntry(DISORIENT);
		spells[4].targettype = TARGET_ATTACKING;
		spells[4].instant = true;
		spells[4].perctrigger = 7.0f;
		spells[4].attackstoptimer = 2000;
		spells[4].cooldown = 10;

		spells[5].info = dbcSpell.LookupEntry(KNOCKBACK);
		spells[5].targettype = TARGET_ATTACKING;
		spells[5].instant = true;
		spells[5].perctrigger = 0.0f;
		spells[5].attackstoptimer = 2000;
		spells[5].cooldown = 15;

		// Phase 2
		spells[6].info = dbcSpell.LookupEntry(ACID_GEYSER);
		spells[6].targettype = TARGET_DESTINATION;
		spells[6].instant = true;
		spells[6].perctrigger = 0.0f;
		spells[6].attackstoptimer = 2000;
		spells[6].cooldown = 10;

		spells[7].info = dbcSpell.LookupEntry(FEL_RAGE1);
		spells[7].targettype = TARGET_RANDOM_SINGLE;
		spells[7].instant = true;
		spells[7].perctrigger = 0.0f;
		spells[7].attackstoptimer = 2000;
		spells[7].cooldown = 90;
		spells[7].mindist2cast = 0.0f;
		spells[7].maxdist2cast = 60.0f;

		spells[8].info = dbcSpell.LookupEntry(FEL_RAGE2);
		spells[8].targettype = TARGET_RANDOM_SINGLE;
		spells[8].instant = true;
		spells[8].perctrigger = 0.0f;
		spells[8].attackstoptimer = 2000;
		spells[8].cooldown = 90;
		spells[8].mindist2cast = 0.0f;
		spells[8].maxdist2cast = 60.0f;

		LastThreat = 0;
		PhaseTimer = 0;
		Phase = 1;
    }

    void OnCombatStart(Unit* mTarget)
    {
		for (int i = 0; i < 9; i++)
		{
			spells[i].casttime = 0;
		}

		spells[2].casttime = (uint32)time(NULL) + 10;

		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Horde will crush you!");
		_unit->PlaySoundToSet(11432);

		RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));

		PhaseTimer = (uint32)time(NULL) + 60;
		LastThreat = 0;
		Phase = 1;
    }

	void OnCombatStop(Unit* mTarget)
    {
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

		if (_unit->isAlive())
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I'll rip the meat from your bones!");
			_unit->PlaySoundToSet(11437);
		}

		RemoveAIUpdateEvent();

		LastThreat = 0;
		PhaseTimer = 0;
		Phase = 1;
    }

    void OnDied(Unit* mKiller)
    {
		_unit->PlaySoundToSet(11439);

		RemoveAIUpdateEvent();
    }

	void OnTargetDied(Unit* mTarget)
    {
		switch (RandomUInt(2))
		{
		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "More! I want more!");
			_unit->PlaySoundToSet(11434);
			break;
		default:
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Time to feast!");
				_unit->PlaySoundToSet(11433);
			}
		}
	}

	void AIUpdate()
	{
		uint32 t = (uint32)time(NULL);
		if (Phase == 1 && t > PhaseTimer)
		{
			spells[7].casttime = 0;

			PhaseTimer = 0;
			Phase = 2;
		}

		// Phase 1
		if (Phase == 1 && _unit->GetAIInterface()->getNextTarget() && _unit->GetCurrentSpell() == NULL)
		{
			// Bloodboil
			if (t > spells[2].casttime)
			{
				_unit->setAttackTimer(spells[2].attackstoptimer, false);

				_unit->CastSpell(_unit, spells[2].info, spells[2].instant);

				spells[2].casttime = t + spells[2].cooldown;
				return;
			}

			// Arcing Smash
			if (t > spells[0].casttime)
			{
				_unit->setAttackTimer(spells[0].attackstoptimer, false);

				Unit* target = _unit->GetAIInterface()->getNextTarget();
				_unit->CastSpell(target, spells[0].info, spells[0].instant);

				spells[0].casttime = t + spells[0].cooldown;
				return;
			}

			// Knockback
			if (t > spells[5].casttime)
			{
				_unit->setAttackTimer(spells[5].attackstoptimer, false);

				Unit* target = _unit->GetAIInterface()->getNextTarget();
				_unit->CastSpell(target, spells[5].info, spells[5].instant);

				if (_unit->GetAIInterface()->GetSecondHated())
					_unit->GetAIInterface()->setNextTarget(_unit->GetAIInterface()->GetSecondHated());

				spells[5].casttime = t + spells[5].cooldown;
				return;
			}
		}

		if (Phase == 2 && _unit->GetAIInterface()->getNextTarget() && _unit->GetCurrentSpell() == NULL)
		{
			// Fel Rage check
			if (t > spells[7].casttime - 62 && LastThreat != 0)
			{
				Unit* RageTarget = NULL;
				RageTarget = _unit->GetAIInterface()->getNextTarget();
				if (RageTarget->FindAura(FEL_RAGE1) && RageTarget->FindAura(FEL_RAGE2))
				{
					_unit->GetAIInterface()->RemoveThreatByPtr(RageTarget);
					_unit->GetAIInterface()->AttackReaction(RageTarget, LastThreat, 0);

					LastThreat = 0;
				}
			}

			// Acid Geyser
			if (LastThreat != 0 && RandomUInt(5) == 1 && t > spells[6].casttime)
			{
				Unit* RageTarget = NULL;
				RageTarget = _unit->GetAIInterface()->getNextTarget();
				if (RageTarget->FindAura(FEL_RAGE1) || RageTarget->FindAura(FEL_RAGE2))
				{
					_unit->setAttackTimer(spells[6].attackstoptimer, false);

					_unit->CastSpellAoF(RageTarget->GetPositionX(),RageTarget->GetPositionY(),RageTarget->GetPositionZ(), spells[6].info, spells[6].instant);

					spells[6].casttime = t + spells[6].cooldown;
					return;
				}
			}

			// Arcing Smash
			if (t > spells[0].casttime)
			{
				_unit->setAttackTimer(spells[0].attackstoptimer, false);

				Unit* target = _unit->GetAIInterface()->getNextTarget();
				_unit->CastSpell(target, spells[0].info, spells[0].instant);

				spells[0].casttime = t + spells[0].cooldown;
				return;
			}

			// Fel Rage - not sure about system
			if (t > spells[7].casttime)
			{
				_unit->setAttackTimer(spells[7].attackstoptimer, false);

				std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
													/* If anyone wants to use this function, then leave this note!										 */
				for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
				{ 
					if (isHostile(_unit, (*itr)) && (*itr) != _unit && (*itr)->IsUnit())
					{
						Unit* RandomTarget = NULL;
						RandomTarget = TO_UNIT(*itr);

						if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= 0.0f && _unit->GetDistance2dSq(RandomTarget) <= 6400.0f && _unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))
						{
							TargetTable.push_back(RandomTarget);
						} 
					} 
				}

				if (!TargetTable.size())
					return;

				size_t RandTarget = rand()%TargetTable.size();

				Unit*  RTarget = TargetTable[RandTarget];

				if (!RTarget)
					return;

				_unit->CastSpell(RTarget, spells[7].info, spells[7].instant);
				_unit->CastSpell(_unit, spells[8].info, spells[8].instant);

				LastThreat = _unit->GetAIInterface()->getThreatByPtr(RTarget);

				Unit* Target = _unit->GetAIInterface()->GetMostHated();
				uint32 MostHatedThreat = 0;
				if (Target)	// unneeded, but still safe check :P
				{
					MostHatedThreat = _unit->GetAIInterface()->getThreatByPtr(Target);
				}

				_unit->GetAIInterface()->AttackReaction(RTarget, MostHatedThreat + 100000, 0);

				TargetTable.clear();

				switch (RandomUInt(2))
				{
				case 1:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I'll rip the meat from your bones!");
					_unit->PlaySoundToSet(11438);
					break;
				default:
					{
						_unit->PlaySoundToSet(11437);
					}
				}

				spells[7].casttime = t + spells[7].cooldown;
				spells[6].casttime = t;
				return;
			}
		}

		float val = RandomFloat(100.0f);
        SpellCast(val);
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
        {
			float comulativeperc = 0;
		    Unit* target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if ((i == 3 || i == 4) && Phase != 1)
					continue;

				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->getNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant);
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
        {
			std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && (*itr)->IsUnit()) // isAttackable(_unit, (*itr)) && 
				{
					Unit* RandomTarget = NULL;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit*  RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();

			if (i == 7 || i == 8)
			{
				//Aura* aura = new Aura*(spells[7].info, 30000, _unit, RTarget);
				//if (aura)
				//	RTarget->AddAura(aura);
				RTarget->CastSpell(RTarget, spells[7].info, spells[7].instant);

				LastThreat = _unit->GetAIInterface()->getThreatByPtr(RTarget);
				Unit* Target = _unit->GetAIInterface()->getNextTarget();
				RTarget->GetAIInterface()->AttackReaction(RTarget, _unit->GetAIInterface()->getThreatByPtr(Target) + 100000, 0);
			}
		}
	}

protected:

	uint32 LastThreat;
	uint32 PhaseTimer;
	uint32 Phase;
	int nrspells;
};

//----------------------------------------------------------------------//
//-------------------------Reliquary of Souls---------------------------//
//--------------------------Scripted by Higi----------------------------//
//----------------------------------------------------------------------//

//////////////////////////////////////////////////////////////////////////
// Essence of Suffering

#define CN_ESSENCEOFSUFFERING	23418

#define EOS_FIXATE				40893 //40414
#define EOS_FRENZY				41305
#define EOS_SOUL_DRAIN			41303
#define EOS_AURA_OF_SUFFERING	41292

class EssenceOfSufferingAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(EssenceOfSufferingAI, MoonScriptCreatureAI);
    EssenceOfSufferingAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {

		AddSpell(EOS_FIXATE, Target_ClosestPlayer, 10, 0, 5);
		AddSpell(EOS_FRENZY, Target_Self, 100, 3, 60);
		AddSpell(EOS_SOUL_DRAIN, Target_RandomPlayerNotCurrent, 7, 1, 15);
		mAuraOfSuffering = AddSpell(EOS_AURA_OF_SUFFERING, Target_RandomPlayer, 0, 0, 0);

		AddEmote(Event_OnTargetDied, "Look at what you made me do!", Text_Yell, 11417);
		AddEmote(Event_OnTargetDied, "I didn't ask for this!", Text_Yell, 11418);
		AddEmote(Event_OnTargetDied, "The pain is only beginning!", Text_Yell, 11419);

		// Freed
		Emote("Pain and suffering are all that await you.", Text_Yell, 11415);
    }

    void OnCombatStart(Unit* mTarget)
    {
		ParentClass::OnCombatStart(mTarget);
		CastSpellNowNoScheduling(mAuraOfSuffering);
    }

	void AIUpdate()
	{
		ParentClass::AIUpdate();
		if ( GetHealthPercent() <= 1)
		{
			_unit->SetHealthPct(1);
			SetCanEnterCombat(false);
			SetAllowMelee(false);
			SetAllowSpell(false);
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);	
			RemoveAllAuras();
			RemoveAuraOnPlayers(EOS_AURA_OF_SUFFERING);
			MoonScriptCreatureAI* mRoS = GetNearestCreature(22856);
			if ( mRoS != NULL && mRoS->IsAlive() )
				MoveTo( mRoS );
		}
	}
    
	SpellDesc*	mAuraOfSuffering;
};

//////////////////////////////////////////////////////////////////////////
// Essence Of Desire

#define CN_ESSENCEOFDESIRE 23419

#define EOD_RUNE_SHIELD 41431
#define EOD_DEADEN 41410
#define EOD_SPIRIT_SHOCK 41426
#define EOD_AURA_OF_DESIRE 41350

class EssenceOfDesireAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(EssenceOfDesireAI, MoonScriptCreatureAI);
    EssenceOfDesireAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(EOD_RUNE_SHIELD, Target_Self, 6, 0, 15);
		AddSpell(EOD_DEADEN, Target_Current, 6, 1, 15);
		AddSpell(EOD_SPIRIT_SHOCK, Target_Current, 100, 1, 15);
		mAuraOfDesire = AddSpell(EOD_AURA_OF_DESIRE, Target_RandomPlayer, 0, 0, 0);
		
		AddEmote(Event_OnTargetDied, "Fulfilment is at hand.", Text_Yell, 11409);
		AddEmote(Event_OnTargetDied, "Yes, you'll stay with us now.", Text_Yell, 11410);
		AddEmote(Event_OnTargetDied, "Your reach exceeds your grasp.", Text_Yell, 11413);

		// Freed
		Emote("You can have anything you desire... for a price.", Text_Yell, 11408);
    }

	void OnCombatStart(Unit* mTarget)
    {
		ParentClass::OnCombatStart(mTarget);
		CastSpellNowNoScheduling(mAuraOfDesire);
    }

	void AIUpdate()
	{
		ParentClass::AIUpdate();
		if ( GetHealthPercent() <= 1)
		{
			_unit->SetHealthPct(1);
			
			SetCanEnterCombat(false);
			SetAllowMelee(false);
			SetAllowSpell(false);
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);			
			RemoveAllAuras();
			RemoveAuraOnPlayers(EOD_AURA_OF_DESIRE);
			MoonScriptCreatureAI* mRoS = GetNearestCreature(22856);
			if ( mRoS != NULL && mRoS->IsAlive() )
				MoveTo( mRoS );
		}
	}
    
	SpellDesc*	mAuraOfDesire;
};

//////////////////////////////////////////////////////////////////////////
// EssenceOfAnger

#define CN_ESSENCEOFANGER		23420

#define EOA_SEETHE				41520
#define EOA_SOUL_SCREAM			41545
#define EOA_SPITE				41377
#define EOA_AURA_OF_ANGER		41337

class EssenceOfAngerAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(EssenceOfAngerAI, MoonScriptCreatureAI);
    EssenceOfAngerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		mTaunt = AddSpell(EOA_SEETHE, Target_Current, 0, 0, 0); // on Taunt
		mSoulScream = AddSpell(EOA_SOUL_SCREAM, Target_Current, 6, 1, 15);
		mSoulScream->AddEmote("So foolish!", Text_Yell, 11400);
		mSpite = AddSpell(EOA_SPITE, Target_Current, 6, 0, 15);
		mSpite->AddEmote("On your knees!", Text_Yell, 11403);
		mAuraOfAnger = AddSpell(EOA_AURA_OF_ANGER, Target_RandomPlayer, 0, 0, 0);
		AddEmote(Event_OnTargetDied, "", Text_Yell, 11401);
		AddEmote(Event_OnTargetDied, "Enough, no more!", Text_Yell, 11402);
		AddEmote(Event_OnDied, "Beware, cowards!", Text_Yell, 11405);
		AddEmote(Event_OnDied, "I won't be ingored.", Text_Yell, 11404);

		Emote("Beware - I live!", Text_Yell, 11399);
	}

	void OnCombatStart(Unit* mTarget)
    {
		ParentClass::OnCombatStart(mTarget);
		CastSpellNowNoScheduling(mAuraOfAnger);
    }
	void OnDied(Unit* mKiller)
	{
		ParentClass::OnDied(mKiller);
		RemoveAuraOnPlayers(EOD_AURA_OF_DESIRE);	
	}

	void AIUpdate()
	{
		ParentClass::AIUpdate();

		if ( _unit->GetAIInterface()->GetIsTaunted() )
		{
			CastSpellNowNoScheduling(mTaunt);
		}
	}

	SpellDesc*	mSoulScream;
	SpellDesc*	mAuraOfAnger;
	SpellDesc*	mTaunt;
	SpellDesc*	mSpite;
};

#define CN_RELIQUARY_OF_SOULS	22856
#define ROS_SUMMON_SUFFERING	41488
#define ROS_SUMMON_DESIRE		41493
#define ROS_SUMMON_ANGER		41496

class ReliquaryOfSoulsAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(ReliquaryOfSoulsAI, MoonScriptCreatureAI);
    ReliquaryOfSoulsAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		mSummonSuffering = AddSpell(ROS_SUMMON_SUFFERING, Target_Self, 0, 0, 0);
		mSummonDesire = AddSpell(ROS_SUMMON_DESIRE, Target_Self, 0, 0, 0);
		mSummonAnger = AddSpell(ROS_SUMMON_ANGER, Target_Self, 0, 0, 0);
		SetCanEnterCombat(true);
		SetAllowMelee(true);
		SetAllowRanged(true);
		SetCanMove(false);
		Phase = 0;
		mEnslavedSoulTimer = -1;
		SpawnedEnsalvedSoul = false;
	}

	void OnCombatStart(Unit* mTarget)
	{
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);		
		SetAllowMelee(false);
		SetAllowRanged(false);
			
		if (Phase == 0)
		{
			_unit->Emote(EMOTE_ONESHOT_SUBMERGE);
			CastSpellNowNoScheduling(mSummonSuffering);
			Phase = 1;
		}
		RegisterAIUpdateEvent(1000);
		ParentClass:: OnCombatStart(mTarget);
	}

	void AIUpdate()
	{
		ParentClass::AIUpdate();
		switch (Phase)
		{
			case 1:
				{
					
					_unit->Emote(EMOTE_STATE_SUBMERGED);
					mEoS = GetNearestCreature(CN_ESSENCEOFSUFFERING);
					if (mEoS && mEoS->GetUnit() && mEoS->IsAlive())
					{
						Creature* pEoS = TO_CREATURE(mEoS->GetUnit());
						if ( pEoS->GetHealthPct()<=1 && pEoS->CalcDistance(_unit)<=3 )
						{
							_unit->Emote(EMOTE_STATE_STAND);
							mEoS->Emote("Now what do I do?!", Text_Yell, 11414);
							pEoS->Emote(EMOTE_ONESHOT_SUBMERGE);
							pEoS->Despawn(100,0);
							Phase = 2;
							mEnslavedSoulTimer = AddTimer(5000);
						}
					}
				}break;
			case 3:
				{
					mEoD = GetNearestCreature(CN_ESSENCEOFDESIRE);
					if (!mEoD || !mEoD->GetUnit())
					{						
						_unit->Emote(EMOTE_ONESHOT_SUBMERGE);
						CastSpellNowNoScheduling(mSummonDesire);
						Phase = 4;
					}
				}break;
			case 4:
				{
					mEoD = GetNearestCreature(CN_ESSENCEOFDESIRE);
					if (mEoD && mEoD->GetUnit() && mEoD->IsAlive())
					{
						Creature* pEoD = TO_CREATURE(mEoD->GetUnit());
						if ( pEoD->GetHealthPct()<=1 && pEoD->CalcDistance(_unit)<=3 )
						{							
							_unit->Emote(EMOTE_STATE_STAND);
							mEoD->Emote("I'll be waiting.", Text_Yell, 11413);
							pEoD->Emote(EMOTE_ONESHOT_SUBMERGE);
							pEoD->Despawn(100,0);
							Phase = 5;
							mEnslavedSoulTimer = AddTimer(5000);
						}
						else{
							_unit->Emote(EMOTE_STATE_SUBMERGED);
						}
					}
				}break;
			case 6:
				{
					mEoA = GetNearestCreature(CN_ESSENCEOFANGER);
					if (!mEoA || !mEoA->GetUnit())
					{
						CastSpellNowNoScheduling(mSummonAnger);
						_unit->Emote(EMOTE_ONESHOT_SUBMERGE);
						Phase = 7;
					}
				}break;
			case 7:
				{					
					_unit->Emote(EMOTE_STATE_SUBMERGED);
					mEoA = GetNearestCreature(CN_ESSENCEOFANGER);
					if (mEoA && mEoA->GetUnit() && !mEoA->GetUnit()->isAlive())
					{						
						Despawn(100, 0);
						Phase = 8;
					}
				}break;
			case 2:
			case 5:
				{
					_unit->Emote(EMOTE_STATE_STAND);
					if ( IsTimerFinished(mEnslavedSoulTimer) && !SpawnedEnsalvedSoul )
					{
						RemoveTimer( mEnslavedSoulTimer );
						SpawnedEnsalvedSoul = true;
						MoonScriptCreatureAI* pSpawnedEnsalvedSoul;
						for( int i=0; i<10; i++ )
						{
							pSpawnedEnsalvedSoul = SpawnCreature(CN_ENSLAVED_SOUL);
							if( pSpawnedEnsalvedSoul )
							{
								pSpawnedEnsalvedSoul->AggroNearestPlayer();
								pSpawnedEnsalvedSoul = NULL;
							};
						};
						RemoveTimer( mEnslavedSoulTimer );
					}
					if (SpawnedEnsalvedSoul)
					{
						
						Creature* creature = NULL;
						DeadSoulCount = 0;
						for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
						{
							if((*itr)->IsCreature())
							{
								creature = TO_CREATURE((*itr));
								if (creature->GetCreatureInfo()->Id == CN_ENSLAVED_SOUL && !creature->isAlive())
									DeadSoulCount++;
							}
						}
						if (DeadSoulCount == 10){
							Phase++;
							SpawnedEnsalvedSoul = false;
						}
					}
				}break;
		}
	}

	bool					SpawnedEnsalvedSoul;
	int						Phase;
	int						DeadSoulCount;
	int32					mEnslavedSoulTimer;
	MoonScriptCreatureAI*	mEoS;
	MoonScriptCreatureAI*	mEoD;
	MoonScriptCreatureAI*	mEoA;
	SpellDesc*				mSummonAnger;
	SpellDesc*				mSummonDesire;
	SpellDesc*				mSummonSuffering;
};

//--------------------------Mother Shahraz------------------------------//

#define CN_MOTHER_SHAHRAZ 22947

#define SINFUL_BEAM				40827
#define SINISTER_BEAM			40859
#define VILE_BEAM				40860
#define WICKED_BEAM				40861

#define SABER_LASH				40810	// Should be 40816 but 40816 is not supported by core
#define FATAL_ATTRACTION		41001	// 40869 - needs more scripting
#define MS_ENRAGE				40743	// 40683
// Dunno where "spell" sounds/texts should go
class ShahrazAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ShahrazAI);
	SP_AI_Spell spells[6];
	bool m_spellcheck[6];

    ShahrazAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;

		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(SABER_LASH);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 15;

		spells[1].info = dbcSpell.LookupEntry(FATAL_ATTRACTION);
		spells[1].targettype = TARGET_RANDOM_SINGLE;
		spells[1].instant = true;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 2000;
		spells[1].cooldown = 15;
		spells[1].mindist2cast = 10.0f;
		spells[1].maxdist2cast = 60.0f;

		spells[2].info = dbcSpell.LookupEntry(SINFUL_BEAM);
		spells[2].targettype = TARGET_RANDOM_SINGLE;
		spells[2].instant = true;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 2000;
		spells[2].cooldown = 9;
		spells[2].mindist2cast = 0.0f;
		spells[2].maxdist2cast = 80.0f;
		
		spells[3].info = dbcSpell.LookupEntry(WICKED_BEAM);
		spells[3].targettype = TARGET_RANDOM_SINGLE;
		spells[3].instant = true;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 2000;
		spells[3].cooldown = 9;
		spells[3].mindist2cast = 0.0f;
		spells[3].maxdist2cast = 80.0f;

		spells[4].info = dbcSpell.LookupEntry(VILE_BEAM);
		spells[4].targettype = TARGET_RANDOM_SINGLE;
		spells[4].instant = true;
		spells[4].perctrigger = 0.0f;
		spells[4].attackstoptimer = 2000;
		spells[4].cooldown = 9;
		spells[4].mindist2cast = 0.0f;
		spells[4].maxdist2cast = 80.0f;

		spells[5].info = dbcSpell.LookupEntry(SINISTER_BEAM);
		spells[5].targettype = TARGET_RANDOM_SINGLE;
		spells[5].instant = true;
		spells[5].perctrigger = 0.0f;
		spells[5].attackstoptimer = 2000;
		spells[5].cooldown = 9;
		spells[5].mindist2cast = 0.0f;
		spells[5].maxdist2cast = 80.0f;

		Enraged = false;
		AuraChange = 0;
		SoundTimer = 0;
    }

    void OnCombatStart(Unit* mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "So, business... or pleasure?");
		_unit->PlaySoundToSet(11504);

		for (int i = 0; i < 6; i++)
			spells[i].casttime = 0;

		AuraChange = (uint32)time(NULL) + 15;
		Enraged = false;
		SoundTimer = 5;

		RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
    }

	void OnCombatStop(Unit* mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

		RemoveAIUpdateEvent();
    }
	
    void OnDied(Unit* mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I wasn't finished.");
		_unit->PlaySoundToSet(11511);

		RemoveAIUpdateEvent();
    }

	void OnTargetDied(Unit* mTarget)
    {
		switch (RandomUInt(2))
		{
		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "So much for a happy ending.");
			_unit->PlaySoundToSet(11509);
			break;
		default:
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Easy come, easy go.");
				_unit->PlaySoundToSet(11508);
			}
		}
	}

	void AIUpdate()
	{
		SoundTimer++;
		if (_unit->GetAIInterface()->GetIsTaunted() && SoundTimer > 10)
		{
			switch (RandomUInt(3))
			{
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I'm not impressed.");
				_unit->PlaySoundToSet(11502);
				break;
			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Enjoying yourselves?");
				_unit->PlaySoundToSet(11503);
				break;
			default:
				{
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You play, you pay.");
					_unit->PlaySoundToSet(11501);
				}
			}

			SoundTimer = 0;
		}

		if (!Enraged && _unit->GetHealthPct() <= 20)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Stop toying with my emotions!");
			_unit->PlaySoundToSet(11510);

			_unit->CastSpell(_unit, MS_ENRAGE, true);

			Enraged = true;
		}

		uint32 t = (uint32)time(NULL);

		// In normal way it is applied to players all around enemy caster =/
		if (t > AuraChange)
		{
			uint32 SpellId = 0;
			switch (RandomUInt(6))
			{
			case 1:
				SpellId = 40891;	// Arcane
				break;
			case 2:
				SpellId = 40882;	// Fire
				break;
			case 3:
				SpellId = 40896;	// Frost
				break;
			case 4:
				SpellId = 40897;	// Holy
				break;
			case 5:
				SpellId = 40883;	// Nature
				break;
			case 6:
				SpellId = 40880;	// Shadow
				break;
			default:
				{
					SpellId = 40880;	// Shadow
				}
			}

			//_unit->CastSpell(_unit, SpellId, true);
			Aura* aura = new Aura( dbcSpell.LookupEntry(SpellId),(uint32)15000, _unit, _unit );
			_unit->AddAura(aura);

			AuraChange = t + 15;
		}

		if (t > spells[2].casttime)
		{
			uint32 SpellId = RandomUInt(4) + 1;
			if (SpellId < 2 || SpellId > 5)
				SpellId = 5;

			_unit->setAttackTimer(spells[2].attackstoptimer, false);

			CastSpellOnRandomTarget(SpellId, spells[2].mindist2cast, spells[2].maxdist2cast, 0, 100);

			spells[2].casttime = t + spells[2].cooldown;
			return;
		}

		float val = RandomFloat(100.0f);
        SpellCast(val);
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
        {
			float comulativeperc = 0;
		    Unit* target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->getNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
        {
			std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && (*itr)->IsUnit()) // isAttackable(_unit, (*itr)) && 
				{
					Unit* RandomTarget = NULL;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit*  RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}

protected:

	uint32 SoundTimer;
	uint32 AuraChange;
	bool Enraged;
	int nrspells;
};

//-----------------------The Illidari Council---------------------------//

//------------------------------------
//    -= Gathios the Shatterer =-
//------------------------------------
#define CN_GATHIOS_THE_SHATTERER 22949

typedef std::vector<Creature*> EncounterVector;

#define HAMMER_OF_JUSTICE			41468
#define SEAL_OF_COMMAND				41469
#define SEAL_OF_BLOOD				41459
#define CONSECRATION				41541
#define BLESSING_OF_SPELL_WARDING	41451
#define BLESSING_OF_PROTECTION		41450
#define CHROMATIC_RESISTANCE_AURA	41453
#define DEVOTION_AURA				41452

class GathiosAI : public MoonScriptCreatureAI
{
public:
    MOONSCRIPT_FACTORY_FUNCTION(GathiosAI, MoonScriptCreatureAI );
    GathiosAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell( HAMMER_OF_JUSTICE, Target_RandomPlayer, 15, 0, 14 );
		AddSpell( SEAL_OF_COMMAND, Target_Self, 15, 0, 30 );
		AddSpell( SEAL_OF_BLOOD, Target_Self, 15, 0, 30 );
		AddSpell( CONSECRATION, Target_Self, 15, 0, 30 ); 
		AddSpell( BLESSING_OF_SPELL_WARDING, Target_RandomFriendly, 15, 0, 60 );  
		AddSpell( BLESSING_OF_PROTECTION, Target_RandomFriendly, 15, 0, 60 ); 
		
		AddEmote( Event_OnTargetDied, "Selama amor'anore!", Text_Yell, 11423);
		AddEmote( Event_OnCombatStart, "I have better things to do!", Text_Yell, 11422);
		AddEmote( Event_OnDied, "Lord Illidan... I...", Text_Yell, 11425);
    };

	void OnCombatStart( Unit* pTarget )
	{
		ApplyAura( DEVOTION_AURA );
		ParentClass::OnCombatStart(pTarget);
	};

	void OnDamageTaken(Unit* mAttacker, uint32 fAmount) 
	{
		DealDamageToFriends( fAmount, _unit->GetEntry() );
	};

	void AddEncounterCreature( Creature* pCreature )
	{
		mEncounterVector.push_back( pCreature );
	};

	void DealDamageToFriends( uint32 val, uint32 pCreatureEntry )
	{
		for( vector<Creature*>::iterator itr = mEncounterVector.begin(); itr != mEncounterVector.end(); ++itr )
		{
			if( (*itr) && (*itr)->isAlive() && (*itr)->GetEntry() != pCreatureEntry )
			{
				(*itr)->DealDamage( (*itr), val, 0, 0, 0 );
			};
		};

		if( IsAlive() && _unit->GetEntry() != pCreatureEntry )
			_unit->DealDamage( _unit, val, 0, 0, 0 );
	};

	void Destroy()
	{
		mEncounterVector.clear();
		delete this;
	};

private:
	EncounterVector mEncounterVector;
};

//------------------------------------
//    -=Veras Darkshadow  =-
//------------------------------------
#define CN_VERAS_DARKSHADOW 22952

#define DEADLY_POISON	41485
#define ENVENOM			41487
#define VANISH			41476

class VerasAI : public MoonScriptCreatureAI
{
public:
    MOONSCRIPT_FACTORY_FUNCTION( VerasAI, MoonScriptCreatureAI );
    VerasAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell( DEADLY_POISON, Target_RandomPlayer, 15.0f, 0, 20 );
		AddSpell( ENVENOM, Target_RandomPlayer, 15.0f, 0, 20 );

		AddEmote( Event_OnTargetDied, "Valiant Effort.", Text_Yell, 11525 ); 
		AddEmote( Event_OnDied, "You got lucky!", Text_Yell, 11527 );
		AddEmote( Event_OnCombatStart, "You wish to test me?", Text_Yell, 11524 );
    
		pGethois = static_cast<GathiosAI*>( GetNearestCreature( CN_GATHIOS_THE_SHATTERER ) );
		if( pGethois != NULL )
			pGethois->AddEncounterCreature( _unit );
    };

	void OnDamageTaken(Unit* mAttacker, uint32 fAmount) 
	{
		pGethois->DealDamageToFriends( fAmount, _unit->GetEntry() );
	};

	GathiosAI* pGethois;
};

//------------------------------------
//    -= High Nethermancer Zerevor  =-
//------------------------------------
#define CN_HIGH_NETHERMANCER_ZEREVOR 22950

#define ARCANE_BOLT			41483
#define BLIZZARD			41482
#define FLAMESTRIKE			41481
#define ARCANE_EXPLOSION	29973
#define DAMPEN_MAGIC		41478

class ZerevorAI : public MoonScriptCreatureAI
{
public:
    MOONSCRIPT_FACTORY_FUNCTION( ZerevorAI, MoonScriptCreatureAI );
    ZerevorAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell( ARCANE_BOLT, Target_Current, 65.0f, 2, 6 );
		AddSpell( BLIZZARD, Target_RandomDestination, 25.0f, 0, 16 );
		AddSpell( FLAMESTRIKE, Target_RandomDestination, 10.0f, 1.5f, 25 );
		AddSpell( DAMPEN_MAGIC, Target_Self, 10, 0, 60 );
		AddSpell( ARCANE_EXPLOSION, Target_RandomPlayer, 15, 0, 6 );

		AddEmote( Event_OnCombatStart, "Common... such a crude language. Bandal!", Text_Yell, 11440);
		AddEmote( Event_OnTargetDied, "Shorel'aran.", Text_Yell, 11441);

		pGethois = static_cast<GathiosAI*>( GetNearestCreature( CN_GATHIOS_THE_SHATTERER ) );
		if( pGethois != NULL )
			pGethois->AddEncounterCreature( _unit );
    };

	void OnDamageTaken(Unit* mAttacker, uint32 fAmount) 
	{
		pGethois->DealDamageToFriends( fAmount, _unit->GetEntry() );
	};

	GathiosAI* pGethois;
};

//------------------------------------
//    -= Lady Malande =-
//------------------------------------
#define CN_LADY_MALANDE 22951

#define DIVINE_WRATH		41472
#define REFLECTIVE_SHIELD	41475
#define EMPOWERED_SMITE		41471
#define CIRCLE_OF_HEALING	41455

class MalandeAI : public MoonScriptCreatureAI
{
public:
    MOONSCRIPT_FACTORY_FUNCTION( MalandeAI, MoonScriptCreatureAI );
    MalandeAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell( DIVINE_WRATH, Target_RandomPlayer, 15, 0, 14 );
		AddSpell( REFLECTIVE_SHIELD, Target_Self, 10, 0, 30 );
		AddSpell( EMPOWERED_SMITE, Target_RandomPlayer, 30, 2.0f, 6 );
		AddSpell( CIRCLE_OF_HEALING, Target_RandomFriendly, 30, 2.5f, 18  );

		AddEmote( Event_OnCombatStart, "Flee or die!", Text_Yell, 11482);
		AddEmote( Event_OnDied, "Destiny... awaits!", Text_Yell, 11485);
		AddEmote( Event_OnTargetDied, "My work is done.", Text_Yell, 11483);

		pGethois = static_cast<GathiosAI*>( GetNearestCreature( CN_GATHIOS_THE_SHATTERER ) );
		if( pGethois != NULL )
			pGethois->AddEncounterCreature( _unit );
    };

	void OnDamageTaken(Unit* mAttacker, uint32 fAmount) 
	{
		if( pGethois != NULL )
			pGethois->DealDamageToFriends( fAmount, _unit->GetEntry() );
	};

	GathiosAI* pGethois;
};

//--------------------------Teron Gorefiend-----------------------------//

#define CN_TERON_GOREFIEND 22871

#define DOOM_BLOSSOM		40188	// needs additional creature in DB
#define CRUSHING_SHADOWS	40243
#define INCINERATE			40239
#define SHADOW_OF_DEATH		40251	// need further scripting?
// Dunno where other sounds should go
class TeronGorefiendAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(TeronGorefiendAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    TeronGorefiendAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		} 

		spells[0].info = dbcSpell.LookupEntry(DOOM_BLOSSOM); //summon visual effect 
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 25;

		spells[1].info = dbcSpell.LookupEntry(CRUSHING_SHADOWS);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = false;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 1000;
		spells[1].cooldown = 20;

		spells[2].info = dbcSpell.LookupEntry(INCINERATE);
		spells[2].targettype = TARGET_RANDOM_SINGLE; 
		spells[2].instant = true;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;
		spells[2].cooldown = 20;
		spells[2].mindist2cast = 0.0f;
		spells[2].maxdist2cast = 45.0f;

		spells[3].info = dbcSpell.LookupEntry(SHADOW_OF_DEATH);
		spells[3].targettype = TARGET_RANDOM_SINGLE;
		spells[3].instant = false;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 2000;
		spells[3].cooldown = 30;
		spells[3].mindist2cast = 0.0f;
		spells[3].maxdist2cast = 60.0f;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Vengeance is mine!");
		_unit->PlaySoundToSet(11513);

		for (int i = 0; i < 4; i++)
			spells[i].casttime = 0;

		uint32 t = (uint32)time(NULL);
		spells[2].casttime = t + spells[2].cooldown;
		spells[3].casttime = t + 10;

		RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
    }

	void OnTargetDied(Unit* mTarget)
    {
		if (_unit->GetHealthPct() > 0)	
		{
			switch (RandomUInt(2))
			{
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "It gets worse...");
				_unit->PlaySoundToSet(11515);
				break;
			default:
				{
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I have use for you!");
					_unit->PlaySoundToSet(11514);
				}
			}
		}
    }

    void OnCombatStop(Unit* mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The wheel...spins...again....");
		_unit->PlaySoundToSet(11521);

		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		uint32 t = (uint32)time(NULL);
		if (t > spells[2].casttime)
		{
			_unit->setAttackTimer(spells[2].attackstoptimer, false);

			CastSpellOnRandomTarget(2, spells[2].mindist2cast, spells[2].maxdist2cast, 0, 100);

			spells[2].casttime = t + spells[2].cooldown;
		}

		if (t > spells[3].casttime)
		{
			_unit->setAttackTimer(spells[3].attackstoptimer, false);

			CastSpellOnRandomTarget(3, spells[3].mindist2cast, spells[3].maxdist2cast, 0, 100);

			spells[3].casttime = t + spells[3].cooldown;
		}

		float val = RandomFloat(100.0f);
		SpellCast(val);
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
        {
			float comulativeperc = 0;
		    Unit* target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->getNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
        {
			std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && (*itr)->IsUnit()) // isAttackable(_unit, (*itr)) && 
				{
					Unit* RandomTarget = NULL;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit*  RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}

protected:

	int nrspells;
};

//Shade of AkamaAI

#define CN_SHADE_OF_AKAMA 22841

#define SINFUL_BEAM0 00000

class ShadeofakamaAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ShadeofakamaAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    ShadeofakamaAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		} 
		spells[0].info = dbcSpell.LookupEntry(SINFUL_BEAM0);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 2000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		hm = 100;
		RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
    }

	void OnTargetDied(Unit* mTarget)
    {
		if (_unit->GetHealthPct() > 0)	
		{
			int RandomSpeach;
			RandomUInt(1000);
			RandomSpeach=rand()%3;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I will not last much longer...");
				_unit->PlaySoundToSet(11385);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "It is time to free your soul from Illidan's grasp!");
				//_unit->PlaySoundToSet(11510);
				break;

			}
		}
    }

    void OnCombatStop(Unit* mTarget)
    {
		hm = 100;
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		Creature* cre = NULL;
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I don't want to go back!");
		_unit->PlaySoundToSet(11420);
        cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(22990, 
        643.741f, 305.852f,
        271.689f, 0.00628f, 
        true, false, 0, 0); 
		//cre->GetAIInterface()->setOutOfCombatRange(30000);
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		if(_unit->GetHealthPct() <= 85 && hm == 100) 
        {
            Creature* cre = NULL;
            for(int i = 0; i < 2; i++)
            {
                cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(23421, 
                    _unit->GetPositionX(), _unit->GetPositionY(),
                    _unit->GetPositionZ(), _unit->GetOrientation(),
                    true, false, 0, 0);
				if (cre)
					cre->GetAIInterface()->setOutOfCombatRange(30000);
			}
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Abandon all hope! The legion has returned to finish what was begun so many years ago. This time there will be no escape!");
		_unit->PlaySoundToSet(10999);
		hm=85;
		}
		else if(_unit->GetHealthPct() <= 70 && hm == 85)
        {
            Creature* cre = NULL;
            for(int i = 0; i < 2; i++)
            {
                cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(23215, 
                    _unit->GetPositionX(), _unit->GetPositionY(),
                    _unit->GetPositionZ(), _unit->GetOrientation(),
                    true, false, 0, 0); 
				if (cre)
					cre->GetAIInterface()->setOutOfCombatRange(30000);
			}
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Abandon all hope! The legion has returned to finish what was begun so many years ago. This time there will be no escape!");
		_unit->PlaySoundToSet(10999);
		hm=70;
		}
		else if(_unit->GetHealthPct() <= 55 && hm == 70)
        {
            Creature* cre = NULL;
            for(int i = 0; i < 2; i++)
            {
                cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(23216, 
                    _unit->GetPositionX(), _unit->GetPositionY(),
                    _unit->GetPositionZ(), _unit->GetOrientation(),
                    true, false, 0, 0); 
				if (cre)
					cre->GetAIInterface()->setOutOfCombatRange(30000);
			}
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Abandon all hope! The legion has returned to finish what was begun so many years ago. This time there will be no escape!");
		_unit->PlaySoundToSet(10999);
		hm=55;
		}
		else if(_unit->GetHealthPct() <= 40 && hm == 55)
        {
            Creature* cre = NULL;
            for(int i = 0; i < 2; i++)
            {
                cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(23523, 
                    _unit->GetPositionX(), _unit->GetPositionY(),
                    _unit->GetPositionZ(), _unit->GetOrientation(),
                    true, false, 0, 0); 
				if (cre)
					cre->GetAIInterface()->setOutOfCombatRange(30000);
			}
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Abandon all hope! The legion has returned to finish what was begun so many years ago. This time there will be no escape!");
		_unit->PlaySoundToSet(10999);
		hm=40;
		}
		else if(_unit->GetHealthPct() <= 25 && hm == 40)
        {
            Creature* cre = NULL;
            for(int i = 0; i < 5; i++)
            {
                cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(23318, 
                    _unit->GetPositionX(), _unit->GetPositionY(),
                    _unit->GetPositionZ(), _unit->GetOrientation(),
                    true, false, 0, 0); 
				if (cre)
					cre->GetAIInterface()->setOutOfCombatRange(30000);
			}
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Abandon all hope! The legion has returned to finish what was begun so many years ago. This time there will be no escape!");
		_unit->PlaySoundToSet(10999);
		hm=25;
		}
		else if(_unit->GetHealthPct() <= 10 && hm == 25)
        {
            Creature* cre = NULL;
            for(int i = 0; i < 5; i++)
            {
                cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(23524, 
                    _unit->GetPositionX(), _unit->GetPositionY(),
                    _unit->GetPositionZ(), _unit->GetOrientation(),
                    true, false, 0, 0); 
				if (cre)
					cre->GetAIInterface()->setOutOfCombatRange(30000);
			}
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Abandon all hope! The legion has returned to finish what was begun so many years ago. This time there will be no escape!");
		_unit->PlaySoundToSet(10999);
		hm=10;
		}
		else
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
        {
			float comulativeperc = 0;
		    Unit* target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->getNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

                  	m_spellcheck[i] = false;
					return;
				}

				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

protected:

	int nrspells;
	int hm;
};

//------------------------------------------------------------------------------------------------------------------//
//----------------------------------------Illidan Stormrage Encounter-----------------------------------------------//
//------------------------------------------Percent done: 85% - 90+%-------------------------------------------------//
//--------------------------------------------Scripted by: M4ksiu---------------------------------------------------//
//------------------------------------------------------------------------------------------------------------------//
//-----------------Script is totally private and none of outside forces were involved in its creation---------------//
//--It will remain like that until author decides what to do to not give people opportunity to take credits for it--//
//-----------------------------------For more informations ask script author----------------------------------------//
//------------------------------------------------------------------------------------------------------------------//

/* 
 * TO DO List:
 * 1) Fix Maiev positioning during Demon Form encounter											*DONE*				+
 * 2) Fix Maiev weapon wielding during dialogs and fight										*PARTLY DONE*		*
 * 3) Fix Illidan weapon appearing during Shadow Demons summoning								*DONE*				+
 * 4) Fix not casting spells by Illidan in phases 1, 3											*DONE*				+
 * 5) Add demon cursing after Akama runs away													*DONE*				+
 * 6) Fix trigger channeling during trap execution																	*
 * 7) Maiev yelling when Illidan is caged														*DONE*				+
 * 8) Move Illidan yelling from Maiev class to Illidan's										*DONE*				+
 * 9) Add aura for Shadow Demons																*DONE*				+
 * 10) Recheck texts and their purposes/palces													*PARTLY DONE*		*
 * 11) Adjust Eye Beam movement timers															*DONE*				*
 * 12) Adjust Database data - about speed, damage, health etc.									*PARTLY DONE*		+
 * 13) Prevent Maiev from taking aggro of Illidan												*PARTLY*			+
 * 14) Add Immunities																			*HANDLED BY CORE*	+
 * 15) Adjust Enrage timer with trap timer to prevent dealing massive damage					*DONE*				+
 * 16) Adjust Trap Cage behavior																*DONE W/O ANIM PROB**
 * 17) Adjust Flame Burst spell and its mechanism												*DONE*				+
 * 18) Adjust Flame Burst timer																	*SHOULD BE DONE*	+
 * 19) Fix Maiev facing during Demon Form encounter												*DONE*				*
 * 20) Find Maiev proper knife																						*
 * 21) Add Flame of Azzinoth check to charge target only by one FoA???							*UNKNOWN SITUATION*	*
 * 22) Add 'snow' that appears in Demon phase																		*
 * 23) Fix eventual animation problems																				*
 * 24) Fix problem when sometimes trap catches Illidan too far and rest of it can't be exe.		*SHOULD BE FIXED*	+
 * 25) Check if Parasitic Shadowfiends should attack random target after appearing				*DONE*				+
 * 26) Inspect why Cage Trap crashes server	so often (mostly on spawning or taking Gameobject)	*FIXED?*			+					
 * 27) Fix trigger aggroing - they stop in place												*INVALID?*			+
 * 28) Fix Akama not appearing in death scene													*DONE*				+
 * 29) Change mechanics of Blaze effect (should be spawned after using that Flame Blast spell	*DONE*				+
 * 30) Check core mess with multi-npc spawning by spell																
 * 31) Fix triggers falling through floor or not being spawned correctly						*DONE FOR EYE BLAST**
 * 32) Check what happens with Akama when he runs away and when he should exactly come back							
 * 33) Check timer of Shadow Demon spawning (according to WoWWiki.com)							*COULDN'T FIND INFO**					
 */

// Structures

struct Transformation
{
	uint32			mTimer;
	uint32			mEmote;
	uint32			mEmoteType;			
	const char*		mText;
	uint32			mSoundId;
	uint32			mAura;
	uint32			mUnAura;
	bool			mEquipWeapons;
};

// Transformations
// Emote Types - 0 (Oneshot), 1 (Emotestate)

static Transformation Ascend[]=
{
	{ 250 * 5, 403, 1, "", 0, 0, 0, false },
	{ 250 * 8, 404, 1, "Behold the power... of the demon within!", 11475, 0, 0, false },
	{ 250 * 20, 404, 1, "", 0, 40506, 0, false },
	{ 250 * 10, 405, 1, "", 0, 0, 0, false },
	{ 250 * 10, 405, 1, "", 0, 0, 0, false },				// additional code inside function
	{ 250 * 8, 0, 1, "", 0, 0, 0, false },					// additional code inside function
};

static Transformation Descend[]=
{
	{ 250 * 7, 403, 1, "", 0, 0, 0, false },
	{ 250 * 3, 404, 1, "", 0, 0, 0, false },
	{ 250 * 24, 404, 1, "", 0, 0, 40506, false },			// additional code inside function
	{ 250 * 6, 405, 1, "", 0, 0, 0, false },
	{ 250 * 4, 405, 1, "", 0, 0, 0, true },
	{ 250 * 8, 0, 1, "", 0, 0, 0, true },					// additional code inside function
};

// Paths
// Eye Beam stuff
static Location EyeBeamPaths[]=
{
	{ 642.240601f, 297.297180f, 354.423401f },
	{ 641.197449f, 314.330963f, 354.300262f },
	{ 657.239807f, 256.925568f, 353.996094f },
	{ 657.913330f, 353.562775f, 353.996185f },
	{ 706.751343f, 298.312683f, 354.653809f },
	{ 706.593262f, 310.011475f, 354.693848f },
	{ 706.592407f, 343.425568f, 353.996124f },
	{ 707.019043f, 270.441772f, 353.996063f }
};

// Akama stuff
static Location ToIllidan[]= 
{
	{  },
	{ 660.248596f, 330.695679f, 271.688110f, 1.243284f },
	{ 671.172241f, 353.273193f, 271.689453f, 1.022600f },
	{ 694.227783f, 379.461365f, 271.792145f, 6.232135f },
	{ 724.159973f, 373.263275f, 282.614349f, 5.324218f },
	{ 747.034973f, 335.668274f, 307.047150f, 4.972365f },
	{ 755.477234f, 304.194580f, 312.167328f, 6.276120f },
	{ 771.809753f, 303.744873f, 313.563507f, 6.265894f },
	// After door event waypoints
	{ 778.550232f, 304.515198f, 318.818542f, 0.002354f },
	{ 789.564697f, 304.493652f, 319.759583f, 6.248631f },
	{ 799.598389f, 295.776642f, 319.760040f, 4.687257f },
	{ 799.054016f, 288.826660f, 320.030334f, 4.563174f },
	{ 794.595459f, 262.302856f, 341.463715f, 4.500343f },
	{ 794.396973f, 256.420471f, 341.463715f, 4.557680f },
	{ 783.355957f, 250.460892f, 341.463776f, 3.746361f },
	{ 764.988098f, 238.561462f, 353.646484f, 3.324606f },
	{ 749.337463f, 236.288681f, 352.997223f, 1.633631f },
	{ 751.941528f, 304.626221f, 352.996124f, 3.128243f },
	{ 747.236511f, 304.922913f, 352.996155f, 6.278462f },
	{ 747.834106f, 362.513977f, 352.996155f, 1.604554f }
};

// Illidan stuff
static Location ForIllidan[]= 
{
	{  },
	{ 709.783203f, 305.073883f, 362.988770f, 1.546307f },	// Middle pos
	{ 678.703735f, 337.249664f, 362.988770f, 4.741172f },	// Eastern pos
	{ 679.968384f, 284.618011f, 362.988770f, 1.603503f },	// Western pos
	{ 680.798157f, 305.648743f, 353.192200f, 3.196716f }	// Land
};

static Location UnitPos[]=
{
	{ 676.717346f, 322.445251f, 354.153320f, 5.732623f },	// Blade 1
	{ 677.368286f, 285.374725f, 354.242157f, 5.645614f }	// Blade 2
};

/*			For further purpose

uint32 EventTimers[x][6]
{
	// Door Event	|	Illidan Dialog	|	Akama Escape	|	Blade Event		|	Maiev Arrive	|	Death Scene
};

*/

uint32 DoorEventTimers[]=
{
	1000 * 2,
	1000 * 4,
	1000 * 4,
	1000 * 9,
	1000 * 1,
	1000 * 6,
	1000 * 1,
	1000 * 6,
	1000 * 5,
	1000 * 1,
	1000 * 9,
	1000 * 1,
	1000 * 3,
	1000 * 3,
	1000 * 3,
	// After breaking gate
	1000 * 2,
	1000 * 15
};

uint32 IllidanDialog[]=
{
	1000 * 1,
	1000 * 1,
	1000 * 1,
	1000 * 4,
	1000 * 5,
	1000 * 3,
	1000 * 3,
	1000 * 4,
	1000 * 3,
	1000 * 3,
	1000 * 4,
	1000 * 2,
	1000 * 2,
	1000 * 2,
	1000 * 2
};

uint32 AkamaEscapeTimers[]=
{
	1000 * 1,
	1000 * 7,
	1000 * 1,
	1000 * 1,
	1000 * 1,
	1000 * 1,
	1000 * 2,
	1000 * 2
};

uint32 BladeEvent[]=
{
	500 * 8,
	500 * 2,
	500 * 2,
	500 * 4,
	500 * 4,
	// After killing both Azzinoth elementals
	500 * 4,
	500 * 1,
	500 * 4,
	500 * 6
};

uint32 MaievTimers[]=
{
	1000 * 3,
	1000 * 3,
	1000 * 3,
	1000 * 1,
	1000 * 1,
	1000 * 1,
	1000 * 1,
	1000 * 5,
	1000 * 3,
	1000 * 3,
	1000 * 3,
	1000 * 1,
	1000 * 2//3
};

uint32 DeathSceneTimers[]=
{
	// Maiev
	1000 * 1,
	1000 * 1,
	1000 * 3,
	1000 * 20,
	1000 * 5,
	1000 * 9,
	1000 * 1,
	1000 * 1,
	// Akama
	1000 * 2,
	1000 * 4,
	1000 * 4
};

// ---	 Illidan Encounter Event Options	--- //

#define USE_SHADOW_PRISON	// aggroes a lot of mobs/NPCs if they are not friendly to Illidan

// Unselectable Trigger AI
#define CN_DOOR_EVENT_TRIGGER	60001
#define CN_FACE_TRIGGER			60002

class UnselectableTriggerAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(UnselectableTriggerAI, MoonScriptCreatureAI);
    UnselectableTriggerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }
};

// Generic Trigger AI

// Demon Fire settings
#define CN_DEMON_FIRE				23069
#define DEMON_FIRE					40029
#define DEMON_FIRE_DESPAWN			60000	// time in ms

// Blaze Effect settings
#define CN_BLAZE_EFFECT				23259
#define BLAZE_EFFECT				40610
#define BLAZE_EFFECT_DESPAWN		60000	// time in ms

// Flame Crash Effect settings
#define CN_FLAME_CRASH				23336
#define FLAME_CRASH_EFFECT			40836
#define FLAME_CRASH_EFFECT_DESPAWN	120000	// time in ms

// Flame Burst settings
#define CN_FLAME_BURST				30003
#define FLAME_BURST					41131
#define FLAME_BURST_DESPAWN			1000	// time in ms

class GenericTriggerAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(GenericTriggerAI, MoonScriptCreatureAI);
    GenericTriggerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		SetCanMove(false);
		_unit->m_noRespawn = true;

		mSpellId = mDespawnTimer = 0;
		uint32 AIUpdate = 2000;
		bool OnSpawn = false;
		switch (_unit->GetEntry())
		{
		case CN_DEMON_FIRE:
			OnSpawn = true;
			mSpellId = DEMON_FIRE;
			mDespawnTimer = DEMON_FIRE_DESPAWN;
			break;
		case CN_BLAZE_EFFECT:
			AIUpdate = 4000;						// to recheck
			mSpellId = BLAZE_EFFECT;
			mDespawnTimer = BLAZE_EFFECT_DESPAWN;
			break;
		case CN_FLAME_CRASH:
			mSpellId = FLAME_CRASH_EFFECT;
			mDespawnTimer = FLAME_CRASH_EFFECT_DESPAWN;
			break;
		case CN_FLAME_BURST:
			mSpellId = FLAME_BURST;
			mDespawnTimer = FLAME_BURST_DESPAWN;
			break;
		}

		if (mSpellId == 0)
			return;

		if (OnSpawn)
		{
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
			ApplyAura(mSpellId);
			SetAllowMelee(false);
			Despawn(mDespawnTimer, 0);
		}
		else
		{
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
			RegisterAIUpdateEvent(AIUpdate);
		}
	}

	void AIUpdate()
	{
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
		ApplyAura(mSpellId);
		SetAllowMelee(false);
		Despawn(mDespawnTimer, 0);
		RemoveAIUpdateEvent();

		if (_unit->GetEntry() == CN_FLAME_BURST)
		{
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		}
	}

	uint32	mDespawnTimer;
	uint32	mSpellId;
};

// Eye Beam Trigger AI
#define CN_EYE_BEAM_TRIGGER	30000

#define EYE_BLAST			40017

class EyeBeamTriggerAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(EyeBeamTriggerAI, MoonScriptCreatureAI);
    EyeBeamTriggerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
		_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
		_unit->m_noRespawn = true;

		SetAllowMelee(false);
		RegisterAIUpdateEvent(1000);

		mPosition = -1;
    }

    void OnCombatStop(Unit* mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
    }

    void AIUpdate()
    {
		if (mPosition == -1)
		{
			Despawn(10000, 0);

			mPosition = -2;
		}
		else if (mPosition != -2)
		{
			MoveTo(EyeBeamPaths[7 - mPosition].x, EyeBeamPaths[7 - mPosition].y, EyeBeamPaths[7 - mPosition].z, false);
			
			mPosition = -2;
		}

		ApplyAura(EYE_BLAST);
    }

	int32	mPosition;
};

// ---	Other creature AIs	--- //

// Shadow Demon AI
#define CN_SHADOW_DEMON				23375

#define SHADOW_DEMON_PARALYZE		41083
#define SHADOW_DEMON_CONSUME_SOUL	41080
#define SHADOW_DEMON_PURPLE_BEAM	39123	// temporary spell
#define SHADOW_DEMON_PASSIVE		41079

class ShadowDemonAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(ShadowDemonAI, MoonScriptCreatureAI);
    ShadowDemonAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		mParalyze = AddSpell(SHADOW_DEMON_PARALYZE, Target_Current, 0, 0, 0);
		mConsumeSoul = AddSpell(SHADOW_DEMON_CONSUME_SOUL, Target_Current, 0, 0, 0);
		SetDespawnWhenInactive(true);
		_unit->m_noRespawn = true;

		Unit* pTarget = GetBestPlayerTarget();
		if (pTarget != NULL)
		{
			_unit->GetAIInterface()->AttackReaction(pTarget, 200000);
			SetTargetToChannel(pTarget, SHADOW_DEMON_PURPLE_BEAM);
			CastSpellNowNoScheduling(mParalyze);
			ApplyAura(SHADOW_DEMON_PASSIVE);
			RegisterAIUpdateEvent(1000);
		}
		else
		{
			Despawn(0);
		}
    }

	void OnDied(Unit*  pKiller)
	{
		if (GetTargetToChannel() != NULL)
		{
			Unit* pUnit = GetTargetToChannel();
			pUnit->RemoveAura(SHADOW_DEMON_PARALYZE);
		}

		ParentClass::OnDied(pKiller);
	}

	void OnTargetDied(Unit*  pTarget)
	{
		Despawn(2000, 0);
		ParentClass::OnDied(pTarget);
	}

	void AIUpdate()
	{
		// Ugly code :P
		Unit* pTarget = _unit->GetAIInterface()->getNextTarget();
		if (pTarget != NULL && GetTargetToChannel() != NULL && pTarget == GetTargetToChannel())
		{
			if (GetRangeToUnit(pTarget) <= 8.0f)
			{	
				pTarget->RemoveAura(SHADOW_DEMON_PARALYZE);
				CastSpellNowNoScheduling(mConsumeSoul);

				RemoveAIUpdateEvent();
				Despawn(500, 0);
				return;
			}
		}
		else
		{
			Despawn(500, 0);
		}

		ParentClass::AIUpdate();
	}

	SpellDesc*	mParalyze;
	SpellDesc*	mConsumeSoul;
};

// Parasitic Shadowfiend AI - This AI is one huge hack :P
#define CN_PARASITIC_SHADOWFIEND					23498

#define PARASITIC_SHADOWFIEND_PASSIVE				41913
#define PARASITIC_SHADOWFIEND_WITH_DAMAGE			41917

void SpellFunc_Parasitic(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);

class ParasiticShadowfiendAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(ParasiticShadowfiendAI, MoonScriptCreatureAI);
    ParasiticShadowfiendAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		mParasiticDmg = AddSpell(PARASITIC_SHADOWFIEND_WITH_DAMAGE, Target_Current, 0, 0, 0);
		mParasitic = AddSpellFunc(&SpellFunc_Parasitic, Target_Current, 0, 0, 0);

		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		ApplyAura(PARASITIC_SHADOWFIEND_PASSIVE);
		_unit->m_noRespawn = true;
		SetDespawnWhenInactive(true);
		SetScale(0.0f);

		Unit* pTarget = GetBestPlayerTarget(TargetFilter_Closest);
		if (pTarget != NULL)
		{
			_unit->GetAIInterface()->SetUnitToFollow(pTarget);
			_unit->GetAIInterface()->SetUnitToFollowAngle(2.0f);
			RegisterAIUpdateEvent(11000);
		}
		else
		{
			Despawn(0);
		}
    }

	// Won't work until it's hooked in core
	void OnHit(Unit* pTarget, float pAmount)
	{
		if (!pTarget->HasAura(PARASITIC_SHADOWFIEND_PASSIVE))
		{
			CastSpellNowNoScheduling(mParasitic);
		}
	}

	void AIUpdate()
	{
		if (_unit->GetUInt64Value(UNIT_FIELD_FLAGS) == UNIT_FLAG_NOT_SELECTABLE)
		{
			DelayNextAttack(3000);
			SetScale(1.0f);
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
			_unit->GetAIInterface()->ResetUnitToFollow();
			_unit->GetAIInterface()->SetUnitToFollowAngle(0.0f);
			RemoveAIUpdateEvent();

			Unit* pTarget = GetBestPlayerTarget();
			if (pTarget != NULL)
			{
				_unit->GetAIInterface()->setNextTarget(pTarget);
				_unit->GetAIInterface()->AttackReaction(pTarget, 200);
			}
			else
			{
				Despawn(0);
			}
		}
	}

	SpellDesc*	mParasiticDmg;
	SpellDesc*	mParasitic;
};

void SpellFunc_Parasitic(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType)
{
	ParasiticShadowfiendAI *Parasitic = (pCreatureAI != NULL) ? (ParasiticShadowfiendAI*)pCreatureAI : NULL;
	if (Parasitic != NULL)
	{
		Parasitic->CastSpell(Parasitic->mParasiticDmg);
		if (pTarget != NULL)						// not sure if target is really added here
		{
			// Workaround - we will spawn 2 Parasitic Shadowfiends on that player place
			Parasitic->SpawnCreature(CN_PARASITIC_SHADOWFIEND, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), pTarget->GetOrientation(), false);
			Parasitic->SpawnCreature(CN_PARASITIC_SHADOWFIEND, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), pTarget->GetOrientation(), false);
		}
	}
}

// Akama AI
#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), textid, plr); \
    Menu->SendTo(plr);

class SCRIPT_DECL AkamaGossip : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player* pPlayer, bool AutoSend)
    {
		Creature* pAIOwner = TO_CREATURE(pObject);
		if (pAIOwner->GetScript() == NULL)
			return;

		MoonScriptCreatureAI *pAI = static_cast<MoonScriptCreatureAI*>(pAIOwner->GetScript());
        GossipMenu *Menu;
		if (pAI->GetCurrentWaypoint() >= 10)
		{
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 229902, pPlayer);
			Menu->AddItem(0, "We're ready to face Illidan.", 2);
		}
		else
		{
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 229901, pPlayer);
			Menu->AddItem(0, "I'm ready, Akama.", 1);
		}
	 
		if (AutoSend)
			Menu->SendTo(pPlayer);
    }
 
	void GossipSelectOption(Object* pObject, Player* pPlayer, uint32 Id, uint32 IntId, const char * EnteredCode)
    {
		Creature* pAIOwner = TO_CREATURE(pObject);
		if (pAIOwner->GetScript() == NULL)
			return;

		MoonScriptCreatureAI *pAI = static_cast<MoonScriptCreatureAI*>(pAIOwner->GetScript());
		switch (IntId)
		{
		case 0:
			GossipHello(pObject, pPlayer, true);
			break;
		case 1:
			pAIOwner->SetUInt32Value(UNIT_NPC_FLAGS, 0);
			pAI->ForceWaypointMove(1);
			break;
		case 2:
			pAIOwner->SetUInt32Value(UNIT_NPC_FLAGS, 0);
			pAI->ForceWaypointMove(17);
			pAI->SetWieldWeapon(false);
			break;
		}
	}

};

/* Akama AI - Creature AI Class */

#define CN_AKAMA				22990

#define AKAMA_HEALING_POTION	40535
#define AKAMA_BLESSING_OF_KINGS	20217
#define AKAMA_DESPAWN			41242

// Door Event Spells
#define AKAMA_DOOR_FAIL			41271
#define AKAMA_DOOR_OPEN			41268
#define DEATHSWORN_DOOR_OPEN	41269
#define GATE_FAILURE			10390

#define AKAMA_WAYPOINT_SIZE		20

class AkamaAI : public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(AkamaAI, MoonScriptBossAI);
	AkamaAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
    {
		AddPhaseSpell(2, AddSpell(AKAMA_BLESSING_OF_KINGS, Target_Self, 15, 0, 60));
		mHealingPotion = AddSpell(AKAMA_HEALING_POTION, Target_Self, 0, 0, 0);
		mHealingPotion->AddEmote("I will not last much longer!", Text_Yell, 11385);
		mHealingPotion->AddEmote("No! Not yet!", Text_Yell, 11386);
		mDespawn = AddSpell(AKAMA_DESPAWN, Target_Self, 0, 0.5f, 0);

		AddEmote(Event_OnTargetDied, "Illidan will pay!", Text_Yell, 11381);
		AddEmote(Event_OnTargetDied, "One step closer!", Text_Yell, 11382);
		AddEmote(Event_OnDied, "Fight on friends! Kill him and end the curse on my people!", Text_Yell, 11391);

		SetMoveType(Move_DontMoveWP);
		SetCanEnterCombat(false);
		SetPhase(1);

		for (int i = 1; i < AKAMA_WAYPOINT_SIZE; ++i)
		{
			AddWaypoint(CreateWaypoint(i, 0, Flag_Run, ToIllidan[i]));
		}

		_unit->SetUInt32Value(UNIT_NPC_FLAGS, 1);
		_unit->SetDualWield(true);

		mUdaloAI = mOlumAI = NULL;
		mIllidanAI = NULL;
		mTimeLeft = mScenePart = 0;
	}
    
    void OnCombatStart(Unit*  pTarget)
    {
		ParentClass::OnCombatStart(pTarget);
		SetWieldWeapon(true);
		CancelAllCooldowns();
    }

    void OnCombatStop(Unit*  pTarget)
    {
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		SetWieldWeapon(false);
    }

	void OnDied(Unit*  pKiller)
	{
		ParentClass::OnDied(pKiller);
	}

	void AIUpdate()
	{
		switch (GetPhase())
		{
		case 1:
			WayToIllidan();
			break;
		case 2:
			FightWithIllidan();
			break;
		case 3:
			EncounterEnd();
			break;
		}
	}

	void ToIllidanEvent()
	{
		if (mScenePart > 6 && mScenePart < 15 && (mUdaloAI == NULL || mOlumAI == NULL))
		{
			mScenePart = -1;
		}

		GameObject* pGate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(774.7f, 304.6f, 314.85f, 185905);
		Unit* pDoorTrigger = ForceCreatureFind(CN_DOOR_EVENT_TRIGGER, 771.5f, 304.7f, 319.0f);
		if ((mScenePart <= 15 && pGate == NULL) || mScenePart == -1)
		{
			Emote("It's strange that Illidan doesn't protect himself against intruders.", Text_Say, 0);
			SetTargetToChannel(NULL, 0);
			ForceWaypointMove(7);

			if (mUdaloAI != NULL)
			{
				mUdaloAI->SetTargetToChannel(NULL, 0);
			}
			if (mOlumAI != NULL)
			{
				mOlumAI->SetTargetToChannel(NULL, 0);
			}

			RemoveAIUpdateEvent();
			mUdaloAI = mOlumAI = NULL;
			mTimeLeft = DoorEventTimers[15];
			mScenePart = 16;
			return;
		}

		mTimeLeft -= mAIUpdateFrequency;
		if (mTimeLeft > 0)
			return;

		switch (mScenePart)
		{
		case 1:
			_unit->SetFacing(6.248631f);
			break;
		case 2:
			Emote("The door is all that stands between us and the Betrayer. Stand aside, friends.", Text_Say, 0);
			_unit->Emote(EMOTE_ONESHOT_TALK);
			break;
		case 3:
			if (pDoorTrigger != NULL)
			{
				ApplyAura(AKAMA_DOOR_FAIL);
				SetTargetToChannel(pDoorTrigger, AKAMA_DOOR_FAIL);
			}
			break;
		case 4:
			SetTargetToChannel(NULL, 0);
			break;
		case 5:
			Emote("I cannot do this alone...", Text_Say, 0);
			_unit->Emote(EMOTE_ONESHOT_NO);
			break;
		case 6:		// summoning two spirits to help Akama with breaking doors
			mUdaloAI = SpawnCreature(23410, 751.884705f, 311.270050f, 312.121185f, 0.047113f);
			mOlumAI  = SpawnCreature(23411, 751.687744f, 297.408600f, 312.124817f, 0.054958f);
			if (mUdaloAI == NULL || mOlumAI == NULL)
			{
				pGate->SetState(0);
				break;
			}
			break;
		case 7:
			mUdaloAI->Emote("You are not alone, Akama.", Text_Say, 0);
			break;
		case 8:
			mOlumAI->Emote("Your people will always be with you.", Text_Say, 0);
			break;
		case 9:
			if (pDoorTrigger != NULL)
			{
				ApplyAura(AKAMA_DOOR_OPEN);
				SetTargetToChannel(pDoorTrigger, AKAMA_DOOR_OPEN);
			}
			break;
		case 10:
			if (pDoorTrigger != NULL)
			{
				mUdaloAI->ApplyAura(DEATHSWORN_DOOR_OPEN);
				mUdaloAI->SetTargetToChannel(pDoorTrigger, DEATHSWORN_DOOR_OPEN);
				mOlumAI->ApplyAura(DEATHSWORN_DOOR_OPEN);
				mOlumAI->SetTargetToChannel(pDoorTrigger, DEATHSWORN_DOOR_OPEN);
			}
			break;
		case 11:
			pGate->SetState(0);
			if (pDoorTrigger != NULL)
			{
				pDoorTrigger->CastSpell(pDoorTrigger, dbcSpell.LookupEntry(GATE_FAILURE), true);
			}
			break;
		case 12:
			SetTargetToChannel(NULL, 0);
			mUdaloAI->SetTargetToChannel(NULL, 0);
			mOlumAI->SetTargetToChannel(NULL, 0);
			break;
		case 13:
			Emote("I thank you for your aid, my brothers. Our people will be redeemed!", Text_Say, 0);
			_unit->Emote(EMOTE_ONESHOT_SALUTE);
			break;
		case 14:
			mUdaloAI->GetUnit()->Emote(EMOTE_ONESHOT_SALUTE);
			mOlumAI->GetUnit()->Emote(EMOTE_ONESHOT_SALUTE);

			mUdaloAI = NULL;
			mOlumAI = NULL;
			break;
		case 15:
			ForceWaypointMove(7);
			RemoveAIUpdateEvent();
			break;
		case 16:
			Emote("", Text_Say, 11388);
			_unit->SetFacing(2.113512f);
			break;
		case 17:
			_unit->SetUInt32Value(UNIT_NPC_FLAGS, 1);
			RemoveAIUpdateEvent();

			mScenePart = 0;
			return;
			break;
		}

		mTimeLeft = DoorEventTimers[mScenePart];
		++mScenePart;
	}

	void IllidanDialogEvent()
	{
		if (mIllidanAI == NULL)
		{
			Creature* pIllidan = TO_CREATURE(ForceCreatureFind(22917, 704.539001f, 305.282013f, 353.919006f));
			if (pIllidan != NULL && pIllidan->GetScript() != NULL)
			{
				mIllidanAI = (MoonScriptCreatureAI*)(pIllidan->GetScript());
			}
		}

		if (mIllidanAI == NULL || !mIllidanAI->IsAlive())
		{
			Emote("Not this time my friends.", Text_Say, 0);
			CastSpellNowNoScheduling(mDespawn);
			Despawn(0);
			return;
		}

		mTimeLeft -= mAIUpdateFrequency;
		if (mTimeLeft > 0)
			return;

		switch (mScenePart)
		{
		case 1:
			_unit->SetFacing(3.126680f);
			break;
		case 2:
			mIllidanAI->RemoveAura(39656);
			break;
		case 3:
			mIllidanAI->Emote("Akama... your duplicity is hardly surprising. I should have slaughtered you and your malformed brethren long ago.", Text_Yell, 11463);
			break;
		case 4:
			mIllidanAI->GetUnit()->Emote(EMOTE_ONESHOT_QUESTION);
			break;
		case 5:
			mIllidanAI->GetUnit()->Emote(EMOTE_ONESHOT_QUESTION);
			break;
		case 6:
			Emote("We've come to end your reign, Illidan. My people and all of Outland shall be free!", Text_Yell, 11389);
			_unit->Emote(EMOTE_ONESHOT_POINT);
		    break;
		case 7:
			_unit->Emote(EMOTE_ONESHOT_TALK);
			break;
		case 8:
			_unit->Emote(EMOTE_ONESHOT_SALUTE);
			break;
		case 9:
			mIllidanAI->Emote("Boldly said. But I remain unconvinced.", Text_Yell, 11464);
			mIllidanAI->GetUnit()->Emote(EMOTE_ONESHOT_QUESTION);
			break;
		case 10:
			mIllidanAI->GetUnit()->Emote(EMOTE_ONESHOT_QUESTION);
			break;
		case 11:
			Emote("The time has come! The moment is at hand!", Text_Yell, 11380);
			_unit->Emote(EMOTE_ONESHOT_SHOUT);
			break;
		case 12:
			SetWieldWeapon(true);
			_unit->Emote(EMOTE_ONESHOT_ROAR);
			break;
		case 13:
			mIllidanAI->Emote("You are not prepared!", Text_Yell, 11466);
			mIllidanAI->GetUnit()->Emote(EMOTE_ONESHOT_CUSTOMSPELL05);

			_unit->SetEmoteState(EMOTE_ONESHOT_READY1H);
			break;
		case 14:
			mIllidanAI->SetWieldWeapon(true);
			break;
		case 15:
			GameObject* pRightGate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(745.07f, 241.802f, 354.292f, 200000);
			GameObject* pLeftGate  = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(744.829f, 369.276f, 354.324f, 200001);
			if (pRightGate != NULL)
			{
				pRightGate->SetState(1);
			}
			if (pLeftGate != NULL)
			{
				pLeftGate->SetState(1);
			}

			SetCanMove(true);
			SetCanEnterCombat(true);
			SetBehavior(Behavior_Default);
			SetMoveType(Move_DontMoveWP);
			SetWaypointToMove(0);
			RemoveAIUpdateEvent();
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);

			_unit->GetAIInterface()->AttackReaction(mIllidanAI->GetUnit(), 1, 0);

			mIllidanAI->SetCanEnterCombat(true);
			mIllidanAI->SetCanMove(true);

			Unit* pTarget = FindClosestTargetToUnit(mIllidanAI->GetUnit());
			if (pTarget == NULL)
			{
				pTarget = _unit;
			}

			mIllidanAI->GetUnit()->GetAIInterface()->AttackReaction(pTarget, 500, 0);

			mScenePart = 0;
			SetPhase(2);
			return;
		}

		mTimeLeft = IllidanDialog[mScenePart];
		++mScenePart;
	}

	void WayToIllidan()
	{
		if (mScenePart != 0)
		{
			if (GetCurrentWaypoint() == 6 || GetCurrentWaypoint() == 16)
			{
				ToIllidanEvent();
			}
			else if (GetCurrentWaypoint() == 17)
			{
				IllidanDialogEvent();
			}
		}
	}

	void FightWithIllidan()
	{
		if (GetHealthPercent() <= 15 && !IsCasting())
		{
			CastSpellNowNoScheduling(mHealingPotion);
		}
		if (mIllidanAI != NULL && !mIllidanAI->IsAlive())
		{
			mIllidanAI = NULL;
		}
		if (mScenePart <= 2)
		{
			if (mIllidanAI == NULL)
			{
				Emote("Not this time my friends.", Text_Say, 0);
				if (!IsCasting())
				{
					CastSpellNowNoScheduling(mDespawn);
				}

				Despawn(0);
				return;
			}
			else if (mIllidanAI->GetUnit()->GetAIInterface()->m_moveFly)
			{
				SetCanEnterCombat(false);
				_unit->GetAIInterface()->SetAIState(STATE_IDLE);
				// azolex to prevent compile error
				uint32 nullfix = 0;
				_unit->GetAIInterface()->setNextTarget( nullfix );
				_unit->GetAIInterface()->WipeTargetList();
				_unit->GetAIInterface()->WipeHateList();
				SetAllowMelee(false);
				SetCanMove(false);
				mIllidanAI->GetUnit()->GetAIInterface()->RemoveThreatByPtr(_unit);
			}
			else
			{
				_unit->SetEmoteState(EMOTE_ONESHOT_READY1H);
				ParentClass::AIUpdate();
			}
		}
		if (mScenePart == 0 && mIllidanAI != NULL && mIllidanAI->GetHealthPercent() <= 85)
		{
			mTimeLeft = AkamaEscapeTimers[0];
			mScenePart = 1;
		}
		if (GetCurrentWaypoint() < 18 && mScenePart != 0)
		{
			mTimeLeft -= mAIUpdateFrequency;
			if (mTimeLeft > 0)
				return;

			switch (mScenePart)
			{
			case 1:
				mIllidanAI->Emote("Come, my minions. Deal with this traitor as he deserves!", Text_Yell, 11465);
				break;
			case 2:
				if (GetCanEnterCombat())
				{
					SetCanEnterCombat(false);
					_unit->GetAIInterface()->SetAIState(STATE_IDLE);
					// ugly code, trows compile error if left just null, this should do it ~ azolex
					uint32 nullfix = 0;
					_unit->GetAIInterface()->setNextTarget( nullfix );
					_unit->GetAIInterface()->WipeTargetList();
					_unit->GetAIInterface()->WipeHateList();
					SetAllowMelee(false);
					SetCanMove(false);

					mIllidanAI->GetUnit()->GetAIInterface()->RemoveThreatByPtr(_unit);
				}

				mIllidanAI = NULL;
				break;
			case 3:
				Emote("I will deal with these mongrels! Strike now, friends! Strike at the Betrayer!", Text_Yell, 11390);
				_unit->SetEmoteState(0);
				break;
			case 4:
				_unit->SetEmoteState(EMOTE_ONESHOT_READY1H);
				break;
			case 5:
				_unit->SetEmoteState(0);
				break;
			case 6:
				SetWieldWeapon(false);
				_unit->Emote(EMOTE_ONESHOT_EXCLAMATION);
				break;
			case 7:
				_unit->SetEmoteState(EMOTE_ONESHOT_READY1H);
				SetWieldWeapon(true);
				break;
			case 8:
				SetCanMove(true);
				ForceWaypointMove(18);
				RemoveAIUpdateEvent();
				//_unit->SetEmoteState(0);

				mScenePart = 0;
				return;
				break;
			}

			mTimeLeft = AkamaEscapeTimers[mScenePart];
			++mScenePart;
		}
	}

	void EncounterEnd()
	{
		mTimeLeft -= mAIUpdateFrequency;
		if (mTimeLeft > 0)
			return;

		switch (mScenePart)
		{
		case 1:{
			Emote("The Light will fill these dismal halls once again. I swear it.", Text_Yell, 11387);

			Unit* pIllidan = ForceCreatureFind(22917);
			if (pIllidan != NULL)
			{
				_unit->GetAIInterface()->setNextTarget(pIllidan);
			}

			_unit->Emote(EMOTE_ONESHOT_TALK);
			}break;
		case 2:
			_unit->Emote(EMOTE_ONESHOT_SALUTE);
			break;
		case 3:
			RemoveAIUpdateEvent();
			CastSpellNowNoScheduling(mDespawn);
			Despawn(0);
			return;
			break;
		}

		mTimeLeft = DeathSceneTimers[mScenePart + 8];
		++mScenePart;
	}

    void OnReachWP(uint32 iWaypointId, bool bForwards)
    {
		switch (iWaypointId)
		{
		case 6:
			RegisterAIUpdateEvent(1000);
			mTimeLeft = DoorEventTimers[0];
			mScenePart = 1;
			break;
		case 16:
			RegisterAIUpdateEvent(1000);
			break;
		case 17:
			RegisterAIUpdateEvent(1000);
			mTimeLeft = IllidanDialog[0];
			mScenePart = 1;
			break;
		case 19:
			//_unit->m_auracount[SPELL_AURA_MOD_INVISIBILITY] = true;						// Arc's
			_unit->UpdateVisibility();
			break;
		case 20:
			RegisterAIUpdateEvent(1000);
			mTimeLeft = DeathSceneTimers[8];
			mScenePart = 1;
			SetPhase(3);
			break;
		default:
			{
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
				_unit->GetAIInterface()->setWaypointToMove(iWaypointId + 1);
			}
		}
    }

	// A bit rewritten FindTarget function
	Unit* FindClosestTargetToUnit(Unit* pSeeker)
	{
		if (pSeeker == NULL)
			return NULL;

		Unit* pTarget = NULL;
		float distance = 70.0f;
		float z_diff;

		Unit* pUnit;
		float dist;

		for (set< Object* >::iterator itr = _unit->GetInRangePlayerSetBegin(); itr != _unit->GetInRangePlayerSetEnd(); ++itr)
		{
			pUnit = static_cast< Unit* >(*itr);

			if(pUnit->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_FEIGN_DEATH))
				continue;

			z_diff = fabs(_unit->GetPositionZ() - pUnit->GetPositionZ());
			if(z_diff > 5.0f)
				continue;

			if(!pUnit->isAlive())
				continue;

			dist = pSeeker->GetDistance2dSq(pUnit);

			if(dist > distance * distance)
				continue;

			distance = dist;
			pTarget = pUnit;
		}

		return pTarget;
	}

	// SPells
	SpellDesc*	mHealingPotion;
	SpellDesc*	mDespawn;

	// AIs
	MoonScriptCreatureAI*	mIllidanAI;
	MoonScriptCreatureAI*	mUdaloAI;
	MoonScriptCreatureAI*	mOlumAI;

	// Other variables
	int32		mScenePart;
	uint32		mTimeLeft;
};

/* Maiev AI - Creature AI Class */

#define CN_MAIEV 23197

//#define TELEPORT_MAIEV 41221
#define MAIEV_TELEPORT			34673
#define MAIEV_SHADOW_STRIKE		40685
#define MAIEV_THROW_DAGGER		41152
#define MAIEV_CAGE_TRAP_SUMMON	40694

class MaievAI : public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(MaievAI, MoonScriptBossAI);
	MaievAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
    {
		AddPhaseSpell(1, AddSpell(MAIEV_SHADOW_STRIKE, Target_Current, 10, 0, 30));
		AddPhaseSpell(2, AddSpell(MAIEV_THROW_DAGGER, Target_Current, 50, 1, 0, 15, 40));
		mTeleport = AddSpell(MAIEV_TELEPORT, Target_Self, 0, 0, 0);
		mTrapSummon = AddSpell(MAIEV_CAGE_TRAP_SUMMON, Target_Self, 0, 1.3f, 0);

		// HACK!
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
		_unit->SetUInt32Value(UNIT_FIELD_MAXHEALTH, 1000000);
		_unit->SetHealth(1000000);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);

		SetMoveType(Move_DontMoveWP);
		SetPhase(1);

		mYellTimer = mScenePart = mTrapTimer = 0;
		mSummonTrap = false;
		mIllidanAI = NULL;
    }
    
    void OnCombatStart(Unit*  pTarget)
    {
		SetWieldWeapon(true);
		CancelAllCooldowns();
    }

    void OnCombatStop(Unit*  pTarget)
    {
		//OnCombatStop(pTarget);		// causes crashes
		CancelAllSpells();
		CancelAllTimers();
		RemoveAllAuras();
		SetBehavior(Behavior_Default);
		//_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		SetWieldWeapon(false);
		if (mIllidanAI != NULL)
		{
			SetPhase(3);
			mScenePart = 1;
			mTimeLeft = DeathSceneTimers[0];
			RegisterAIUpdateEvent(1000);
			DeathScene();
		}
		else
		{
			CastSpellNowNoScheduling(mTeleport);
			RemoveAIUpdateEvent();
			Despawn(500, 0);
		}
    }

	void OnDamageTaken(Unit* mAttacker, uint32 fAmount)
	{
		_unit->SetHealth(_unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH));
	}

    void AIUpdate()
    {
		switch (GetPhase())
		{
		case 1:
			FightWithIllidan();
			break;
		case 2:
			FightWithDemon();
			break;
		case 3:
			DeathScene();
			break;
		}
    }

	void FightWithIllidan()
	{
		if (mIllidanAI == NULL)
		{
			CastSpellNowNoScheduling(mTeleport);
			RemoveAIUpdateEvent();
			Despawn(500, 0);
			return;
		}
		if (mIllidanAI->IsAlive())
		{
			if (mIllidanAI->GetUnit()->HasAura(40506))
			{
				SetPhase(2);
				FightWithDemon();
				return;
			}
			else if (mSummonTrap)
			{
				Emote("There shall be no prison for you this time!", Text_Yell, 11495);
				CastSpellNowNoScheduling(mTrapSummon);
				ResetTimer(mYellTimer, GetTimer(mYellTimer) + (5 + RandomUInt(10))* 1000);
				mSummonTrap = false;
				return;
			}
			else if (IsTimerFinished(mTrapTimer))
			{
				Unit* pTarget = GetBestPlayerTarget();
				if (pTarget != NULL)
				{
					CastSpellNowNoScheduling(mTeleport);
					_unit->SetPosition(pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), pTarget->GetOrientation());		// does it really work?
					_unit->GetAIInterface()->StopMovement(2500);
				}

				ResetTimer(mTrapTimer, ((RandomUInt(5) + 25) * 1000));
				mSummonTrap = true;
				return;
			}
			else if (IsTimerFinished(mYellTimer))
			{
				switch (RandomUInt(2))
				{
				case 0:
					Emote("Bleed as I have bled!", Text_Yell, 11494);
					break;
				case 1:
					Emote("That is for Naisha!", Text_Yell, 11493);
					break;
				case 2:															// Not sure if this one should be here
					Emote("Meet your end, demon!", Text_Yell, 11500);
					break;
				}

				ResetTimer(mYellTimer, (RandomUInt(20) + 20) * 1000);
			}

			ParentClass::AIUpdate();
		}
		else
		{
			mTimeLeft = DeathSceneTimers[0];
			mScenePart = 1;
			SetPhase(3);
			DeathScene();
		}
	}

	void FightWithDemon()
	{
		if (mIllidanAI == NULL)
		{
			CastSpellNowNoScheduling(mTeleport);
			RemoveAIUpdateEvent();
			Despawn(500, 0);
			return;
		}
		if (!mIllidanAI->GetUnit()->HasAura(40506))
		{
			if (mIllidanAI->IsAlive())
			{
				SetBehavior(Behavior_Default);
				SetDisplayWeapon(true, false);
				SetWieldWeapon(true);
				ResetTimer(mTrapTimer, (RandomUInt(5) + 20) * 1000);
				SetPhase(1);
				FightWithIllidan();
			}
			else
			{
				mTimeLeft = DeathSceneTimers[0];
				mScenePart = 1;
				SetPhase(3);
				DeathScene();
			}

			return;
		}
		else if (GetRangeToUnit(mIllidanAI->GetUnit()) < 15.0f)
		{
			_unit->GetAIInterface()->_CalcDestinationAndMove(mIllidanAI->GetUnit(), 20.0f);
			SetBehavior(Behavior_Spell);
			return;
		}
		else if (GetRangeToUnit(mIllidanAI->GetUnit()) > 35.0f)
		{
			_unit->GetAIInterface()->_CalcDestinationAndMove(mIllidanAI->GetUnit(), 30.0f);
			SetBehavior(Behavior_Spell);
			return;
		}

		SetDisplayWeapon(false, false);
		if (mIllidanAI->GetUnit()->GetEmoteState() == 0)		// dunno if it's really needed
		{
			ParentClass::AIUpdate();
		}

		// Ugly -.-'
		float Facing = _unit->calcRadAngle(_unit->GetPositionX(), _unit->GetPositionY(), mIllidanAI->GetUnit()->GetPositionX(), mIllidanAI->GetUnit()->GetPositionY());
		if (_unit->GetOrientation() != Facing)
		{
			_unit->SetFacing(Facing);
		}

		SetBehavior(Behavior_Spell);
	}

	void DeathScene()
	{
		if (mIllidanAI == NULL && mScenePart <= 6)
		{
			CastSpellNowNoScheduling(mTeleport);
			RemoveAIUpdateEvent();
			Despawn(500, 0);
			return;
		}
		if ((mIllidanAI != NULL && mScenePart <= 6) || mScenePart > 6)
		{
			mTimeLeft -= mAIUpdateFrequency;
			if (mTimeLeft > 0)
				return;

			switch (mScenePart)
			{
			case 1:
				Emote("Ah, it is finished. You are beaten.", Text_Yell, 11496);

				mIllidanAI->GetUnit()->Emote(EMOTE_ONESHOT_CUSTOMSPELL06);
				if (mIllidanAI->GetUnit()->GetCurrentSpell() != NULL)
					mIllidanAI->GetUnit()->GetCurrentSpell()->cancel();
				break;
			case 2:
				mIllidanAI->GetUnit()->SetEmoteState(EMOTE_ONESHOT_CUSTOMSPELL07);

				_unit->GetAIInterface()->setNextTarget(mIllidanAI->GetUnit());
				break;
			case 3:
				mIllidanAI->Emote("You have won... Maiev. But the huntress... is nothing without the hunt. You... are nothing... without me.", Text_Yell, 11478);
				_unit->SetEmoteState(0);
				SetWieldWeapon(false);
				break;
			case 4:{
				GameObject* pRightGate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(745.07f, 241.802f, 354.292f, 200000);
				GameObject* pLeftGate  = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(744.829f, 369.276f, 354.324f, 200001);
				if (pRightGate != NULL)
				{
					pRightGate->SetState(0);
				}
				if (pLeftGate != NULL)
				{
					pLeftGate->SetState(0);
				}

				mIllidanAI->GetUnit()->SetEmoteState(0);
				mIllidanAI->GetUnit()->SetHealth(0);
				mIllidanAI->GetUnit()->setDeathState(JUST_DIED);
				}break;
			case 5:
				Emote("He's right. I feel nothing. I am nothing.", Text_Yell, 11497);
				break;
			case 6:{
				Emote("Farewell, champions.", Text_Yell, 11498);
				Creature* pAkama = TO_CREATURE(ForceCreatureFind(CN_AKAMA, ToIllidan[19].x, ToIllidan[19].y, ToIllidan[19].z));
				if (pAkama != NULL)
				{
					AkamaAI *pAkamaAI = (AkamaAI*)(static_cast<MoonScriptCreatureAI*>(pAkama->GetScript()));
					//pAkama->m_auracount[SPELL_AURA_MOD_INVISIBILITY] = true;						// Arc's
					pAkama->UpdateVisibility();
					if (!pAkamaAI->GetCanMove())
					{
						pAkamaAI->SetCanMove(true);
					}

					float IllidanX = mIllidanAI->GetUnit()->GetPositionX();
					float IllidanY = mIllidanAI->GetUnit()->GetPositionY();
					float IllidanZ = mIllidanAI->GetUnit()->GetPositionZ();
					float IllidanO = mIllidanAI->GetUnit()->GetOrientation();

					float Angle = _unit->calcAngle(pAkama->GetPositionX(), pAkama->GetPositionY(), IllidanX, IllidanY) * float(M_PI) / 180.0f;
					float X = 12.0f * cosf(Angle);
					float Y = 12.0f * sinf(Angle);

					X -= cosf(IllidanO);
					Y -= sinf(IllidanO);

					Location pCoords;
					pCoords.x = IllidanX - X;
					pCoords.y = IllidanY - Y;
					pCoords.z = IllidanZ;
					pCoords.o = 0.0f;

					pAkamaAI->AddWaypoint(CreateWaypoint(20, 0, Flag_Run, pCoords));
					pAkamaAI->ForceWaypointMove(20);
				}
				}break;
			case 7:
				CastSpellNowNoScheduling(mTeleport);
				break;
			case 8:
				RemoveAIUpdateEvent();
				Despawn(0);
				return;
				break;
			}

			mTimeLeft = DeathSceneTimers[mScenePart];
			++mScenePart;
		}

		else
		{
			Emote("Farewell, champions.", Text_Yell, 11498);
			Despawn(0);
		}
	}

	// AI class pointers
	MoonScriptCreatureAI*		mIllidanAI;

	// Spell Description pointers
	SpellDesc*					mTrapSummon;
	SpellDesc*					mTeleport;

	// Other variables
	bool						mSummonTrap;
	int32						mTrapTimer;
	int32						mYellTimer;
	uint32						mScenePart;
	uint32						mTimeLeft;
	
};

/*START
			.:Rewritten Illidan Script by M4ksiu:.
START*/

//------- Macros
#define CN_ILLIDAN_STORMRAGE 22917

// Normal Form Spells
#define ILLIDAN_SHEAR								41032  // +
#define ILLIDAN_DRAW_SOUL							40904  // +
#define ILLIDAN_FLAME_CRASH							40832  // +

// Phase 2 Spells
#define ILLIDAN_FIREBALL							40598  // +
#define ILLIDAN_DARK_BARRAGE						40585  // ? Haven't Tried
#define ILLIDAN_SUMMON_TEAR_OF_AZZINOTH				39855  // +
#define ILLIDAN_EYE_BLAST1							39908
#define ILLIDAN_EYE_BLAST2							40017

// Illidan's Glaive spells
#define ILLIDAN_SUMMON_GLAIVE						41466
#define ILLIDAN_GLAIVE_RETURNS						39873
#define ILLIDAN_THROW_GLAIVE1						39635
#define ILLIDAN_THROW_GLAIVE2						39849

#define TEAR_OF_AZZINOTH_CHANNEL					39857

// Phase 3 Spells
#define ILLIDAN_AGONIZING_FLAMES					40932  // +

// Phase 4 Spells
#define ILLIDAN_DEMON_FORM							40506
#define ILLIDAN_AURA_OF_DREAD						41142
#define ILLIDAN_SHADOW_BLAST						41078  // +
#define ILLIDAN_SUMMON_DEMON						41117  // - Core Support
#define ILLIDAN_FLAME_BURST1						41131  // ? Frost-nova-like effect dealing damage
#define ILLIDAN_FLAME_BURST2						41126  // ? Cast Effect

// Demon Forms
#define ILLIDAN_DEMON_FORM1							40511  // +
#define ILLIDAN_DEMON_FORM2							40398  // +
#define ILLIDAN_DEMON_FORM3							40510  // +

// Phase 5 Spells
#define ILLIDAN_ENRAGE								40683  // +

// Other spells
#define ILLIDAN_SHADOW_PRISON						40647  // +
#define ILLIDAN_SKULL_INTRO							39656  // + Works with RemoveAura
#define ILLIDAN_SUMMON_PARASITIC_SHADOWFIENDS		41915  // ? Haven't Tried
#define ILLIDAN_PARASITIC_SHADOWFIEND_WITH_SE		41914
#define ILLIDAN_SUMMON_PARASITIC_SHADOWFIENDS		41915
#define ILLIDAN_BERSERK								45078
#define ILLIDAN_DEATH1								41218
#define ILLIDAN_DEATH2								41220
#define ILLIDAN_RAKE								1822   // + Used to force animation use

// Other macros
#define CN_BLADE_OF_AZZINOTH						22996
#define ILLIDAN_WAYPOINT_SIZE						5

void SpellFunc_Illidan_Parasitic(SpellDesc *pThis, MoonScriptCreatureAI *pCreatureAI, Unit* pTarget, TargetType pType);

class IllidanStormrageAI : public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(IllidanStormrageAI, MoonScriptBossAI);
	IllidanStormrageAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
	{
		//Phase 1 spells
		AddPhaseSpell(1, AddSpell(ILLIDAN_SHEAR, Target_Current, 12, 1.5f, 15));
		AddPhaseSpell(1, AddSpell(ILLIDAN_DRAW_SOUL, Target_Self, 6, 1.5f, 40));	// Self-kills are bad =/
		AddPhaseSpell(1, AddSpell(ILLIDAN_FLAME_CRASH, Target_Destination, 12, 1.3f, 25));
		mParasiticDmg = AddSpell(PARASITIC_SHADOWFIEND_WITH_DAMAGE, Target_RandomPlayerNotCurrent, 0, 0, 0);
		mParasitic = AddSpellFunc(&SpellFunc_Illidan_Parasitic, Target_RandomPlayerNotCurrent, 0, 0, 0);
		//Phase 2 spells
		AddPhaseSpell(2, AddSpell(ILLIDAN_THROW_GLAIVE1, Target_Destination, 0, 0.5f, 0));
		AddPhaseSpell(2, AddSpell(ILLIDAN_THROW_GLAIVE2, Target_Self, 0, 0.5f, 0));
		AddPhaseSpell(2, AddSpell(ILLIDAN_FIREBALL, Target_RandomPlayer, 60, 2, 0));
		AddPhaseSpell(2, AddSpell(ILLIDAN_DARK_BARRAGE, Target_RandomPlayer, 40, 10, 40));
		AddPhaseSpell(2, AddSpell(ILLIDAN_GLAIVE_RETURNS, Target_Self, 0, 0.5f, 0));
		//Phase 3 spells
		AddPhaseSpell(3, AddSpell(ILLIDAN_SHEAR, Target_Current, 12, 1.5f, 15));
		AddPhaseSpell(3, AddSpell(ILLIDAN_DRAW_SOUL, Target_Self, 6, 1.5f, 40));	// Self-kills are bad =/
		AddPhaseSpell(3, AddSpell(ILLIDAN_FLAME_CRASH, Target_Destination, 12, 1.3f, 25));
		AddPhaseSpell(3, AddSpell(ILLIDAN_AGONIZING_FLAMES, Target_RandomDestination, 7, 0, 25, 0, 35, true));
		mShadowPrison = AddPhaseSpell(3, AddSpell(ILLIDAN_SHADOW_PRISON, Target_Self, 0, 0, 0));
		//Phase 4 spells
		mShadowBlast = AddPhaseSpell(4, AddSpell(ILLIDAN_SHADOW_BLAST, Target_Destination, 0, 2, 0));
		mFlameBurst = AddPhaseSpell(4, AddSpell(ILLIDAN_FLAME_BURST1, Target_Self, 0, 0, 20));
		mShadowDemons = AddPhaseSpell(4, AddSpell(ILLIDAN_SUMMON_DEMON, Target_Self, 0, 1.3f, 60));
		//Phase 5 spells
		AddPhaseSpell(5, AddSpell(ILLIDAN_SHEAR, Target_Current, 12, 1.5f, 15));
		AddPhaseSpell(5, AddSpell(ILLIDAN_DRAW_SOUL, Target_Self, 6, 1.5f, 40));	// Self-kills are bad =/
		AddPhaseSpell(5, AddSpell(ILLIDAN_FLAME_CRASH, Target_Destination, 12, 1.3f, 25));
		AddPhaseSpell(5, AddSpell(ILLIDAN_AGONIZING_FLAMES, Target_RandomDestination, 7, 0, 25, 0, 35, true));
		mEnrage = AddPhaseSpell(5, AddSpell(ILLIDAN_ENRAGE, Target_Self, 0, 1.3f, 0, 0, 0, true, "You've wasted too much time mortals, now you shall fall!", Text_Yell, 11474));

		AddEmote(Event_OnTargetDied, "Who shall be next to taste my blades?!", Text_Yell, 11473);
		AddEmote(Event_OnTargetDied, "This is too easy!", Text_Yell, 11472);

		SetEnrageInfo(AddSpell(ILLIDAN_BERSERK, Target_Self, 0, 0, 0), 1500000);

		ApplyAura(ILLIDAN_SKULL_INTRO);
		SetCanEnterCombat(false);
		SetWieldWeapon(false);
		SetCanMove(false);
		SetFlyMode(false);
		SetMoveType(Move_DontMoveWP);
		SetPhase(1);

		for (int i = 1; i < ILLIDAN_WAYPOINT_SIZE; ++i)
		{
			AddWaypoint(CreateWaypoint(i, 0, Flag_Fly, ForIllidan[i]));
		}

		_unit->SetBaseAttackTime(RANGED,1800);
		_unit->SetEmoteState(0);
		_unit->SetDualWield(true);

		mFoA1 = mFoA2 = NULL;
		mAllow = true;
	}

	void OnCombatStart(Unit*  pTarget)
	{
		GameObject* pRightGate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(745.07f, 241.802f, 354.292f, 200000);
		GameObject* pLeftGate  = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(744.829f, 369.276f, 354.324f, 200001);
		if (pRightGate != NULL)
		{
			pRightGate->SetState(1);
		}
		if (pLeftGate != NULL)
		{
			pLeftGate->SetState(1);
		}

		_unit->SetEmoteState(EMOTE_ONESHOT_READY1H);

		ParentClass::OnCombatStart(pTarget);
		CancelAllCooldowns();

		mParasitic->mEnabled = false;
	}

	void OnCombatStop(Unit*  pTarget)
	{
		// General
		_unit->SetEmoteState(0);
		SetMoveType(Move_DontMoveWP);
		SetTargetToChannel(NULL, 0);
		SetCanEnterCombat(true);
		SetWieldWeapon(true);
		SetAllowMelee(true);
		SetFlyMode(false);
		SetCanMove(true);

		mAIUpdateFrequency = 1000;
		mAllow = true;

		// Phase 2
		if (mFoA1 != NULL)
		{
			mFoA1->Despawn(0);
			mFoA1 = NULL;
		}
		if (mFoA2 != NULL)
		{
			mFoA2->Despawn(0);
			mFoA2 = NULL;
		}

		Creature* pTrigger = TO_CREATURE(ForceCreatureFind(CN_FACE_TRIGGER, 677.399963f, 305.545044f, 353.192169f));
		if (pTrigger != NULL)
		{
			pTrigger->Despawn(0, 0);
		}
		for (int i = 0; i < 2; ++i)
		{
			Creature* pBlade = TO_CREATURE(ForceCreatureFind(CN_BLADE_OF_AZZINOTH, UnitPos[i].x, UnitPos[i].y, UnitPos[i].z));
			if (pBlade != NULL)
			{
				pBlade->SetChannelSpellTargetGUID(0);
				pBlade->SetChannelSpellId(0);
				pBlade->Despawn(0, 0);
			}
		}

		// Phase 4
		SetDisplayWeapon(true, true);
		RemoveAura(ILLIDAN_DEMON_FORM);

		if (IsAlive())
		{
			GameObject* pRightGate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(745.07f, 241.802f, 354.292f, 200000);
			GameObject* pLeftGate  = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(744.829f, 369.276f, 354.324f, 200001);
			if (pRightGate != NULL)
			{
				pRightGate->SetState(0);
			}
			if (pLeftGate != NULL)
			{
				pLeftGate->SetState(0);
			}

			Creature* pMaiev = TO_CREATURE(ForceCreatureFind(CN_MAIEV));
			if (pMaiev != NULL)
			{
				pMaiev->Despawn(0, 0);
			}
		}

		ParentClass::OnCombatStop(pTarget);
	}

	void OnDied(Unit*  pKiller)
	{
		Creature* pMaiev = TO_CREATURE(ForceCreatureFind(CN_MAIEV));
		if (pMaiev != NULL && pMaiev->isAlive())
		{
			_unit->SetHealth(1);
			SetCanEnterCombat(false);
			ApplyAura(ILLIDAN_DEATH1);
			ApplyAura(ILLIDAN_DEATH2);

			pMaiev->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
			pMaiev->GetAIInterface()->SetAIState(STATE_IDLE);
			pMaiev->GetAIInterface()->WipeTargetList();
			pMaiev->GetAIInterface()->WipeHateList();
		}
		else
		{
			GameObject* pRightGate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(745.07f, 241.802f, 354.292f, 200000);
			GameObject* pLeftGate  = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(744.829f, 369.276f, 354.324f, 200001);
			if (pRightGate != NULL)
			{
				pRightGate->SetState(0);
			}
			if (pLeftGate != NULL)
			{
				pLeftGate->SetState(0);
			}
		}

		ParentClass::OnDied(pKiller);
	}

	// Does not work until it's hooked
	void OnHit(Unit* mVictim, float fAmount)
	{
		Emote("ON HIT1!", Text_Yell, 0);
		if (mVictim->IsCreature() && (mVictim->GetEntry() == CN_MAIEV || mVictim->GetEntry() == CN_AKAMA))
		{
			Emote("ON HIT2!", Text_Yell, 0);
			Unit* pTarget = _unit->GetAIInterface()->getNextTarget();
			if (pTarget == NULL || !pTarget->IsPlayer())
			{
				pTarget = _unit->GetAIInterface()->GetSecondHated();
				if (pTarget == NULL || !pTarget->IsPlayer())
				{
					pTarget = GetBestPlayerTarget(TargetFilter_Closest);
					if (pTarget == NULL)
						return;
				}
			}

			_unit->GetAIInterface()->setNextTarget(pTarget);
			_unit->GetAIInterface()->modThreatByPtr(pTarget, (int32)(fAmount * 2));
			_unit->GetAIInterface()->RemoveThreatByPtr(mVictim);
		}
	}

	void OnDamageTaken(Unit* mAttacker, uint32 fAmount)
	{
		if (mAttacker->IsCreature() && (mAttacker->GetEntry() == CN_MAIEV || mAttacker->GetEntry() == CN_AKAMA))
		{
			Unit* pTarget = _unit->GetAIInterface()->getNextTarget();
			if (pTarget == NULL || !pTarget->IsPlayer())
			{
				pTarget = _unit->GetAIInterface()->GetSecondHated();
			}
			if (pTarget != NULL && pTarget->IsPlayer())
			{
				_unit->GetAIInterface()->setNextTarget(pTarget);
				_unit->GetAIInterface()->modThreatByPtr(pTarget, fAmount * 3);
			}
			else
				return;

			_unit->GetAIInterface()->RemoveThreatByPtr(mAttacker);
		}
	}

	void AIUpdate()
	{
		if (!mAllow)
			return;

		switch (GetPhase())
		{
		case 1:
			PhaseOne();
			break;
		case 2:
			PhaseTwo();
			break;
		case 3:
			PhaseThree();
			break;
		case 4:
			PhaseFour();
			break;
		case 5:
			PhaseFive();
			break;
		default:
			return;
		}
	}

	void PhaseOne()
	{
		if (GetHealthPercent() <= 85)
		{
			if (mParasitic->mEnabled == false)
			{
				mParasitic->mEnabled = true;
				mParasiticTimer = 30000;
			}

			mParasiticTimer -= mAIUpdateFrequency;
			if (mParasiticTimer <= 0)
			{
				CastSpellNowNoScheduling(mParasitic);
				mParasiticTimer = 30000;
				return;
			}
		}
		if (_unit->GetAIInterface()->m_moveFly)
		{
			MoonScriptCreatureAI *pAI = SpawnCreature(CN_FACE_TRIGGER, 677.399963f, 305.545044f, 353.192169f, false);
			if (pAI != NULL)
			{
				pAI->GetUnit()->m_noRespawn = true;

				_unit->GetAIInterface()->setNextTarget(pAI->GetUnit());
			}

			SetPhase(2);
			ForceWaypointMove(1);
			SetAIUpdateFreq(500);

			mTimeLeft = BladeEvent[0];
			mScenePart = 1;
			mAllow = false;
			return;
		}
		else if (GetHealthPercent() <= 65 && !IsCasting())
		{
			Creature* pAkama = TO_CREATURE(ForceCreatureFind(CN_AKAMA));
			if (pAkama != NULL && pAkama->GetScript() != NULL)
			{
				AkamaAI *pAkamaAI = static_cast<AkamaAI*>(pAkama->GetScript());
				if (pAkamaAI->mScenePart <= 2 && pAkamaAI->GetCanEnterCombat())
				{
					pAkamaAI->SetCanEnterCombat(false);
					pAkamaAI->GetUnit()->GetAIInterface()->SetAIState(STATE_IDLE);
					// ugly code, trows compile error if left just null, this should do it ~ azolex
					uint32 nullfix = 0;
					pAkamaAI->GetUnit()->GetAIInterface()->setNextTarget( nullfix );
					pAkamaAI->GetUnit()->GetAIInterface()->WipeTargetList();
					pAkamaAI->GetUnit()->GetAIInterface()->WipeHateList();
					pAkamaAI->SetAllowMelee(false);
					pAkamaAI->SetCanMove(false);

					_unit->GetAIInterface()->RemoveThreatByPtr(pAkamaAI->GetUnit());
				}
			}

			Emote("I will not be touched by rabble such as you!", Text_Yell, 11479);
			MoveTo(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() + 10.0f, false);
			SetCanEnterCombat(false);
			SetAllowMelee(false);
			SetFlyMode(true);

			_unit->SetEmoteState(0);
			_unit->Emote(EMOTE_ONESHOT_LIFTOFF);

			mFireWallTimer = 30000;
			mMovementTimer = 40000;
			mMiscEventPart = 0;
			mLastFireWall = -1;
			return;
		}

		_unit->SetEmoteState(EMOTE_ONESHOT_READY1H);
		ParentClass::AIUpdate();
	}

	void PhaseTwo()
	{
		if (mScenePart != 0)
		{
			mTimeLeft -= mAIUpdateFrequency;
			if (mTimeLeft != 0)
				return;

			switch (mScenePart - 1)
			{
			case 0:{
				Emote("Behold the flames of Azzinoth!", Text_Yell, 11480);
				SpellDesc *mGlaiveThrow = FindSpellById(ILLIDAN_THROW_GLAIVE2);
				if (mGlaiveThrow != NULL)
				{
					CastSpellNowNoScheduling(mGlaiveThrow);
				}
				}break;
			case 1:
				for (int i = 0; i < 2; ++i)
				{
					_unit->CastSpellAoF(UnitPos[i].x, UnitPos[i].y, UnitPos[i].z, dbcSpell.LookupEntry(ILLIDAN_THROW_GLAIVE1), false);
				}
				SetWieldWeapon(false);
				break;
			case 2:
				for (int i = 0; i < 2; ++i)
				{
					Creature* pBlade = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BLADE_OF_AZZINOTH, UnitPos[i].x, UnitPos[i].y, UnitPos[i].z, UnitPos[i].o, true, false, 0, 0);
					if (pBlade != NULL)
					{
						pBlade->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
						pBlade->m_noRespawn = true;
					}
				}
				break;
			case 3:
				mFoA1 = SpawnCreature(22997, 672.039246f, 326.748322f, 354.206390f, 0.207343f);
				mFoA2 = SpawnCreature(22997, 673.008667f, 283.813660f, 354.267548f, 6.203853f);
				if (mFoA1 != NULL)
				{
					Unit* pBlade = ForceCreatureFind(CN_BLADE_OF_AZZINOTH, UnitPos[0].x, UnitPos[0].y, UnitPos[0].z);
					if (pBlade != NULL)
					{
						pBlade->SetChannelSpellTargetGUID(mFoA1->GetUnit()->GetGUID());
						pBlade->SetChannelSpellId(TEAR_OF_AZZINOTH_CHANNEL);
					}
				}
				if (mFoA2 != NULL)
				{
					Unit* pBlade = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(UnitPos[1].x, UnitPos[1].y, UnitPos[1].z, CN_BLADE_OF_AZZINOTH);
					if (pBlade != NULL)
					{
						pBlade->SetChannelSpellTargetGUID(mFoA2->GetUnit()->GetGUID());
						pBlade->SetChannelSpellId(TEAR_OF_AZZINOTH_CHANNEL);
					}
				}
				_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
				break;
			case 4:{
				_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
				SetAIUpdateFreq(1000);
				mScenePart = 0;

				_unit->GetAIInterface()->setNextTarget(GetBestPlayerTarget(TargetFilter_Closest));
				return;
				}break;
			case 5:
				for (int i = 0 ; i < 2; ++i)
				{
					Creature* Blade = TO_CREATURE(ForceCreatureFind(CN_BLADE_OF_AZZINOTH, UnitPos[i].x, UnitPos[i].y, UnitPos[i].z));
					if (Blade != NULL)
					{
						Blade->Despawn(0,0);
					}
				}
				break;
			case 6:{
				SpellDesc *mGlaiveReturns = FindSpellById(ILLIDAN_GLAIVE_RETURNS);
				if (mGlaiveReturns != NULL)
					CastSpellNowNoScheduling(mGlaiveReturns);

				ApplyAura(ILLIDAN_RAKE);
				RemoveAura(ILLIDAN_RAKE);
				SetWieldWeapon(true);
				}break;
			case 7:
				SetFlyMode(false);
				_unit->Emote(EMOTE_ONESHOT_LAND);
				break;
			case 8:{
				Creature* pTrigger = TO_CREATURE(ForceCreatureFind(CN_FACE_TRIGGER, 677.399963f, 305.545044f, 353.192169f));
				if (pTrigger != NULL)
				{
					pTrigger->Despawn(0, 0);
				}

				_unit->GetAIInterface()->setNextTarget(GetBestPlayerTarget(TargetFilter_Closest));
				_unit->SetEmoteState(EMOTE_ONESHOT_READY1H);	
				SetCanEnterCombat(true);
				SetAllowMelee(true);
				SetCanMove(true);
				ClearHateList();
				SetPhase(3);
				_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);

				SetAIUpdateFreq(1000);

				mParasiticTimer = 30000;
				mShadowDemonsTimer = (RandomUInt(25) + 15) * 1000;
				mDemonTimer = 40000;
				mPhaseBackup = 3;
				mScenePart = 0;
				mPlaySound = false;
				return;
				}break;
			}

			mTimeLeft = BladeEvent[mScenePart];
			++mScenePart;
			return;
		}

		mMovementTimer -= mAIUpdateFrequency;
		mFireWallTimer -= mAIUpdateFrequency;
		if (mFoA1 != NULL)
		{
			if (!mFoA1->IsAlive())
			{
				Unit* pBlade = ForceCreatureFind(CN_BLADE_OF_AZZINOTH, UnitPos[0].x, UnitPos[0].y, UnitPos[0].z);
				if (pBlade != NULL)
				{
					pBlade->SetChannelSpellTargetGUID(0);
					pBlade->SetChannelSpellId(0);
				}

				mFoA1 = NULL;
			}
		}
		if (mFoA2 != NULL)
		{
			if (!mFoA2->IsAlive())
			{
				Unit* pBlade = ForceCreatureFind(CN_BLADE_OF_AZZINOTH, UnitPos[1].x, UnitPos[1].y, UnitPos[1].z);
				if (pBlade != NULL)
				{
					pBlade->SetChannelSpellTargetGUID(0);
					pBlade->SetChannelSpellId(0);
				}

				mFoA2 = NULL;
			}
		}
		if (GetMoveType() != Move_WantedWP && !IsCasting())
		{
			if (_unit->GetChannelSpellId() == 0)
			{
				if (mFoA1 == NULL && mFoA2 == NULL)
				{
					Unit* pTrigger = ForceCreatureFind(CN_FACE_TRIGGER, 677.399963f, 305.545044f, 353.192169f);
					if (pTrigger != NULL)
					{
						_unit->GetAIInterface()->setNextTarget(pTrigger);
					}

					ForceWaypointMove(4);
					SetAIUpdateFreq(500);

					mTimeLeft = BladeEvent[5];
					mScenePart = 6;
					mAllow = false;
					return;
				}
				else if (mMovementTimer <= 0)
				{
					if (RandomUInt(1) == 1)	// Move right
					{
						++mCurrentWaypoint;
						if (mCurrentWaypoint > 3)
							mCurrentWaypoint = 1;
					}
					else					// Move left
					{
						--mCurrentWaypoint;
						if (mCurrentWaypoint < 1)
							mCurrentWaypoint = 3;
					}

					ForceWaypointMove(mCurrentWaypoint);

					mMovementTimer = 50000;
					mAllow = false;
					return;
				}
			}
			if (mMiscEventPart != 0)
			{
				if (mMiscEventPart == 1)
				{
					uint32 FireWall = RandomUInt(7);
					while ((int)FireWall == mLastFireWall || (int)FireWall == 7 - mLastFireWall)
					{
						FireWall = RandomUInt(7);
					}

					Creature* pTrigger = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_EYE_BEAM_TRIGGER, EyeBeamPaths[FireWall].x, EyeBeamPaths[FireWall].y, EyeBeamPaths[FireWall].z, EyeBeamPaths[FireWall].o, true, false, 0, 0);
					if (pTrigger != NULL && pTrigger->GetScript() != NULL)
					{
						Emote("Stare into the eyes of the Betrayer!", Text_Yell, 11481);
						SetTargetToChannel(pTrigger, ILLIDAN_EYE_BLAST1);
						_unit->CastSpell(pTrigger, ILLIDAN_EYE_BLAST1, true);
						_unit->GetAIInterface()->setNextTarget(pTrigger);

						float Distance = pTrigger->CalcDistance(EyeBeamPaths[7 - FireWall].x, EyeBeamPaths[7 - FireWall].y, EyeBeamPaths[7 - FireWall].z);
						uint32 TimeToReach = (uint32)(Distance * 1000 / pTrigger->m_walkSpeed);
						EyeBeamTriggerAI *pEyeBeamTriggerAI = (EyeBeamTriggerAI*)(static_cast<MoonScriptCreatureAI*>(TO_CREATURE((pTrigger))->GetScript()));
						pEyeBeamTriggerAI->mPosition = FireWall;
						pEyeBeamTriggerAI->Despawn(TimeToReach + 1500, 0);
						mFireWallTimer = TimeToReach + 1000;
						mLastFireWall = FireWall;
						mMiscEventPart = 2;
					}
					else
					{
						mFireWallTimer = 30000;
						mMiscEventPart = 0;
						mLastFireWall = -1;
					}

					return;
				}
				else if (mMiscEventPart == 2 && mFireWallTimer <= 0)
				{
					SetTargetToChannel(NULL, 0);
					RemoveAura(ILLIDAN_EYE_BLAST1);
					_unit->GetAIInterface()->setNextTarget(GetBestPlayerTarget(TargetFilter_Closest));

					mFireWallTimer = 30000;
					mMiscEventPart = 0;
					return;
				}
			}
			else if (mFireWallTimer <= 0)
			{
				mMiscEventPart = 1;
				return;
			}

			ParentClass::AIUpdate();
		}
	}

	void Transform(Transformation *pTransformation, uint32 pMaxPart)
	{
		mTimeLeft -= mAIUpdateFrequency;
		if (mTimeLeft > 0)
			return;

		// Ugly ass code
		if (pTransformation[mMiscEventPart - 1].mEmoteType == 0)
			_unit->Emote((EmoteType)pTransformation[mMiscEventPart - 1].mEmote);
		else
			_unit->SetEmoteState(pTransformation[mMiscEventPart - 1].mEmote);
		Emote(pTransformation[mMiscEventPart - 1].mText, Text_Yell, pTransformation[mMiscEventPart - 1].mSoundId);
		ApplyAura(pTransformation[mMiscEventPart - 1].mAura);
		RemoveAura(pTransformation[mMiscEventPart - 1].mUnAura);
		SetWieldWeapon(pTransformation[mMiscEventPart - 1].mEquipWeapons);

		if (pTransformation == Ascend)
		{
			if (mMiscEventPart == 5)
			{
				ClearHateList();
				Unit* pTarget = GetBestPlayerTarget();
				if (pTarget != NULL)
				{
					_unit->GetAIInterface()->AttackReaction(pTarget, 5000);
				}
				SetBehavior(Behavior_Spell);
				StopMovement();					// readding settings after target switch
				SetPhase(mPhaseBackup);			// without it he gets back to phase 1 and then immediatly to 2

				if (mPlaySound)
				{
					switch (RandomUInt(2))
					{
					case 0:
						Emote("You know nothing of power!", Text_Yell, 11469);
						mPlaySound = false;
						break;
					case 1:
						Emote("Such arrogance!", Text_Yell, 11471);
						mPlaySound = false;
						break;
					}
				}
			}
			else if (mMiscEventPart == 6)
			{
				if (mPlaySound)
				{
					switch (RandomUInt(2))
					{
					case 0:
						Emote("You know nothing of power!", Text_Yell, 11469);
						mPlaySound = false;
						break;
					case 1:
						Emote("Such arrogance!", Text_Yell, 11471);
						mPlaySound = false;
						break;
					}
				}

				SetDisplayWeapon(false, false);
				SetAIUpdateFreq(1000);

				mShadowDemonsTimer = (RandomUInt(25) + 15) * 1000;
				mFlameBurstTimer = (RandomUInt(7) + 8) * 1000;
				mDemonTimer = 60000;
				mMiscEventPart = 0;

				SetPhase(4);
				PhaseFour();
				return;
			}
		}
		else if (pTransformation == Descend)
		{
			if (mMiscEventPart == 3)
			{
				ClearHateList();
				SetPhase(4);			// without it he gets back to phase 1 and then immediatly to 2
			}
			else if (mMiscEventPart == 6)
			{
				SetBehavior(Behavior_Default);				
				SetPhase(mPhaseBackup);
				if (mPhaseBackup == 5)
				{
					mEnrageTimer = RandomUInt(5) + 25;
				}

				SetAIUpdateFreq(1000);

				mParasiticTimer = 30000;
				mEnrageTimer = 25000;
				mDemonTimer = 60000;
				mYellTimer = 25000;
				mMiscEventPart = 0;
				return;
			}
		}

		++mMiscEventPart;
		if (mMiscEventPart > pMaxPart)
			return;

		mTimeLeft = pTransformation[mMiscEventPart - 1].mTimer;
	}

	bool SpawnMaiev()	// this doesn't have collision checks! so keep in mind that Maiev can be spawned behind walls!
	{
		float xchange  = RandomFloat(15.0f);
		float distance = 15.0f;

		float ychange = sqrt(distance * distance - xchange * xchange);

		if (rand()%2 == 1)
			xchange *= -1;
		if (rand()%2 == 1)
			ychange *= -1;

		float newposx = _unit->GetPositionX() + xchange;
		float newposy = _unit->GetPositionY() + ychange;

		MoonScriptCreatureAI *pMaievAI = SpawnCreature(CN_MAIEV, newposx, newposy, _unit->GetPositionZ() + 0.5f, 2.177125f);
		if (pMaievAI == NULL)
		{
			UnstuckFromShadowPrison();
			return false;
		}

		pMaievAI->GetUnit()->GetAIInterface()->setNextTarget(_unit);
		return true;
	}

	void UnstuckFromShadowPrison()
	{
		RemoveAuraOnPlayers(ILLIDAN_SHADOW_PRISON);
		SetCanEnterCombat(true);
		SetAllowMelee(true);
		SetCanMove(true);

		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);

		mScenePart = 0;
	}

	void MaievScene()	// thinking about converting Maiev's functions to MoonScriptCreatureAI functions
	{
		mTimeLeft -= mAIUpdateFrequency;
		if (mTimeLeft > 0)
			return;

		MaievAI *pMaievAI = NULL;
		if (mScenePart > 3)
		{
			Creature* pMaiev = TO_CREATURE(ForceCreatureFind(CN_MAIEV));
			if (pMaiev != NULL && pMaiev->GetScript() != NULL)
			{
				pMaievAI = (MaievAI*)(static_cast<MoonScriptCreatureAI*>(pMaiev->GetScript()));
			}
			else
			{
				UnstuckFromShadowPrison();
				return;
			}
		}

		switch (mScenePart)
		{
		case 1:
			Emote("Is this it, mortals? Is this all the fury you can muster?", Text_Yell, 11476);
			break;
		case 2:
			_unit->Emote(EMOTE_ONESHOT_QUESTION);
			break;
		case 3:
			if (!SpawnMaiev())
				return;
			break;
		case 4:
			pMaievAI->ApplyAura(MAIEV_TELEPORT);
			_unit->GetAIInterface()->setNextTarget(pMaievAI->GetUnit());
			break;
		case 5:
			pMaievAI->Emote("Their fury pales before mine, Illidan. We have some unsettled business between us.", Text_Yell, 11491);
			break;
		case 6:
			pMaievAI->SetWieldWeapon(true);
			break;
		case 7:
			pMaievAI->GetUnit()->Emote(EMOTE_ONESHOT_EXCLAMATION);
			pMaievAI->SetDisplayWeapon(false, false);
			break;
		case 8:{
			Emote("Maiev... How is it even possible?", Text_Yell, 11477);
			pMaievAI->SetDisplayWeapon(true, true);
			}break;
		case 9:{
			float Facing = pMaievAI->GetUnit()->calcRadAngle(_unit->GetPositionX(), _unit->GetPositionY(), pMaievAI->GetUnit()->GetPositionX(), pMaievAI->GetUnit()->GetPositionY());
			_unit->SetFacing(Facing);
			_unit->SetEmoteState(EMOTE_ONESHOT_READY1H);
			}break;
		case 10:
			pMaievAI->Emote("Ah, my long hunt is finally over. Today, Justice will be done!", Text_Yell, 11492);
			pMaievAI->GetUnit()->Emote(EMOTE_ONESHOT_EXCLAMATION);
			pMaievAI->SetDisplayWeapon(false, false);
			break;
		case 11:
			pMaievAI->GetUnit()->Emote(EMOTE_ONESHOT_YES);
			pMaievAI->SetDisplayWeapon(true, true);
			break;
		case 12:
			pMaievAI->GetUnit()->Emote(EMOTE_ONESHOT_ROAR);
			break;
		case 13:
			SetCanEnterCombat(true);
			SetAllowMelee(true);
			SetCanMove(true);
			SetPhase(5);

			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);

			pMaievAI->RegisterAIUpdateEvent(1000);
			pMaievAI->mYellTimer = pMaievAI->AddTimer((RandomUInt(20) + 20) * 1000);
			pMaievAI->mTrapTimer = pMaievAI->AddTimer((RandomUInt(5) + 18) * 1000);
			pMaievAI->GetUnit()->SetEmoteState(EMOTE_ONESHOT_READY1H);
			pMaievAI->SetCanEnterCombat(true);
			pMaievAI->GetUnit()->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			pMaievAI->GetUnit()->GetAIInterface()->SetAIState(STATE_IDLE);
			pMaievAI->SetCanMove(true);
			pMaievAI->GetUnit()->GetAIInterface()->AttackReaction(_unit, 1, 0);
			pMaievAI->GetUnit()->SetEmoteState(EMOTE_ONESHOT_READY1H);
			pMaievAI->SetWieldWeapon(true);
			pMaievAI->mIllidanAI = this;

			ClearHateList();
			_unit->GetAIInterface()->AttackReaction(pMaievAI->GetUnit(), 200);

			mParasiticTimer = 30000;
			mDemonTimer = 60000;
			mPhaseBackup = 5;
			mEnrageTimer = 25000;
			mYellTimer = 25000;
			break;
		}

		++mScenePart;
		if (mScenePart > 13)
		{
			mScenePart = 0;
			return;
		}

		mTimeLeft = MaievTimers[mScenePart - 1];
	}

	void PhaseThree()
	{
		if (mMiscEventPart != 0)
		{
			Transform(Ascend, 6);
			return;
		}
		else if (mScenePart != 0)
		{
			MaievScene();
			return;
		}
		else if (_unit->GetHealthPct() <= 30 && !IsCasting())
		{
			#ifdef USE_SHADOW_PRISON
				StopMovement();
				CastSpellNowNoScheduling(mShadowPrison);
			#endif

			SetCanEnterCombat(false);
			SetBehavior(Behavior_Default);
			SetAllowMelee(false);
			SetCanMove(false);

			_unit->SetEmoteState(0);

			mTimeLeft = MaievTimers[0];
			mScenePart = 1;
			return;
		}

		mDemonTimer -= mAIUpdateFrequency;
		mParasiticTimer -= mAIUpdateFrequency;
		if (!IsCasting())
		{
			if (mDemonTimer <= 0)
			{
				StopMovement();
				SetBehavior(Behavior_Spell);
				SetAIUpdateFreq(250);

				_unit->SetEmoteState(0);

				mMiscEventPart = 1;
				mTimeLeft = Ascend[0].mTimer;
				return;
			}
			if (mParasiticTimer <= 0)
			{
				CastSpellNowNoScheduling(mParasitic);
				mParasiticTimer = 30000;
				return;
			}

			_unit->SetEmoteState(EMOTE_ONESHOT_READY1H);
			ParentClass::AIUpdate();
		}
	}

	void PhaseFour()
	{
		if (mPlaySound)
		{
			switch (RandomUInt(2))
			{
			case 0:
				Emote("You know nothing of power!", Text_Yell, 11469);
				mPlaySound = false;
				break;
			case 1:
				Emote("Such arrogance!", Text_Yell, 11471);
				mPlaySound = false;
				break;
			}
		}

		mDemonTimer -= mAIUpdateFrequency;
		mFlameBurstTimer -= mAIUpdateFrequency;
		mShadowDemonsTimer -= mAIUpdateFrequency;
		if (IsCasting())
		{
			mPlaySound = false;
			return;
		}

		if (mMiscEventPart != 0)
		{
			Transform(Descend, 6);
			return;
		}

		if (mDemonTimer <= 0 || (GetHealthPercent() <= 30 && mPhaseBackup == 3))
		{
			SetDisplayWeapon(true, true);
			StopMovement();
			SetBehavior(Behavior_Spell);
			SetAIUpdateFreq(250);

			_unit->SetEmoteState(0);

			mMiscEventPart = 1;
			mTimeLeft = Descend[0].mTimer;
		}
		else if (_unit->GetAIInterface()->getNextTarget() != NULL)
		{
			Unit* pTarget = _unit->GetAIInterface()->getNextTarget();
			if (GetRangeToUnit(pTarget) <= 80.0f)
			{
				SetBehavior(Behavior_Spell);
				StopMovement();

				if (mShadowDemonsTimer <= 0)
				{
					CastSpellNowNoScheduling(mShadowDemons);
					SpawnCreature(CN_SHADOW_DEMON, _unit->GetPositionX() + RandomFloat(5), _unit->GetPositionY() + RandomFloat(5), _unit->GetPositionZ() + 2.0f, 0, true);
					SpawnCreature(CN_SHADOW_DEMON, _unit->GetPositionX() - RandomFloat(5), _unit->GetPositionY() + RandomFloat(5), _unit->GetPositionZ() + 2.0f, 0, true);
					SpawnCreature(CN_SHADOW_DEMON, _unit->GetPositionX() + RandomFloat(5), _unit->GetPositionY() - RandomFloat(5), _unit->GetPositionZ() + 2.0f, 0, true);

					mShadowDemonsTimer = 120000;
					return;
				}
				if (mFlameBurstTimer <= 0)
				{
					CastSpellNowNoScheduling(mFlameBurst);
					for (set< Object* >::iterator itr = _unit->GetInRangePlayerSetBegin(); itr != _unit->GetInRangePlayerSetEnd(); ++itr) 
					{
						Unit *pUnit = static_cast< Unit* >(*itr);
						MoonScriptCreatureAI *pAI = SpawnCreature(CN_FLAME_BURST, (*itr)->GetPositionX(), (*itr)->GetPositionY(), (*itr)->GetPositionZ(), 0, true);
						_unit->CastSpell(pUnit, ILLIDAN_FLAME_BURST2, true);
						if (pAI != NULL)
						{
							float Distance = GetRangeToUnit(pUnit);
							if (Distance == 0.0f)
							{
								pAI->RegisterAIUpdateEvent(300);		// o'rly?
							}
							else
							{
								pAI->RegisterAIUpdateEvent((uint32)(Distance * 1000 / 32.796));
							}
						}
					}

					mFlameBurstTimer += 20000;
					return;
				}

				uint32 Spell = RandomUInt()%100;
				if (Spell >= 0 && Spell <= 80)
					CastSpellNowNoScheduling(mShadowBlast);
			}
			else
			{
				SetBehavior(Behavior_Default);
			}
		}
	}

	void PhaseFive()
	{
		if (mMiscEventPart != 0)
		{
			Transform(Ascend, 6);
			return;
		}

		mYellTimer -= mAIUpdateFrequency;
		mDemonTimer -= mAIUpdateFrequency;
		mEnrageTimer -= mAIUpdateFrequency;
		mParasiticTimer -= mAIUpdateFrequency;
		if (_unit->HasAura(40760) || _unit->HasAura(40695))
		{
			mEnrageTimer = 120000;
		}
		else if (!IsCasting())
		{
			if (mDemonTimer <= 0)
			{
				StopMovement();
				SetBehavior(Behavior_Spell);
				SetAIUpdateFreq(250);

				_unit->SetEmoteState(0);

				mMiscEventPart = 1;
				mTimeLeft = Ascend[0].mTimer;
				mPlaySound = true;
				return;
			}
			if (mEnrageTimer <= 0)
			{
				CastSpellNowNoScheduling(mEnrage);
				mEnrageTimer = 120000;
				mYellTimer += 7000;
				return;
			}
			if (mParasiticTimer <= 0)
			{
				CastSpellNowNoScheduling(mParasitic);
				mParasiticTimer = 30000;
				return;
			}
			if (mYellTimer <= 0 && _unit->GetAIInterface()->getNextTarget() != NULL)
			{
				Unit* pTarget = _unit->GetAIInterface()->getNextTarget();
				if (pTarget->IsCreature() && pTarget->GetEntry() == CN_MAIEV)
				{
					Emote("Feel the hatred of ten thousand years!", Text_Yell, 11470);
					mYellTimer = 25000;
				}
			}

			_unit->SetEmoteState(EMOTE_ONESHOT_READY1H);
			ParentClass::AIUpdate();
		}
	}

	void OnReachWP(uint32 pWaypointId, bool pForwards)
	{
		if (pWaypointId == 1)
		{
			ClearHateList();
			Unit* pTarget = _unit->GetAIInterface()->getNextTarget();
			if (pTarget != NULL && (!pTarget->IsCreature() || pTarget->GetEntry() != CN_FACE_TRIGGER))
			{
				Creature* pTrigger = TO_CREATURE(ForceCreatureFind(CN_FACE_TRIGGER, 677.399963f, 305.545044f, 353.192169f));
				if (pTrigger != NULL)
				{
					_unit->GetAIInterface()->setNextTarget(pTrigger);
				}
			}
		}

		StopWaypointMovement();

		mCurrentWaypoint = pWaypointId;
		mAllow = true;
	}

	void Destroy()
	{
		Creature* pAkama = TO_CREATURE(ForceCreatureFind(CN_AKAMA));
		if (pAkama != NULL && pAkama->GetScript() != NULL)
		{
			AkamaAI *pAI = (AkamaAI*)(pAkama->GetScript());
			if (pAI != NULL)
			{
				if (pAI->mIllidanAI == (MoonScriptCreatureAI*)(this))
					pAI->mIllidanAI = NULL;
			}
		}

		Creature* pMaiev = TO_CREATURE(ForceCreatureFind(CN_MAIEV));
		if (pMaiev != NULL && pMaiev->GetScript() != NULL)
		{
			MaievAI *pAI = (MaievAI*)(pMaiev->GetScript());
			if (pAI != NULL)
			{
				if (pAI->mIllidanAI == (MoonScriptCreatureAI*)(this))
					pAI->mIllidanAI = NULL;
			}
		}

		// commented - due to the creature not being in world when this is called, mapmgr == null -> access violation
/*		GameObject* pRightGate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(745.07f, 241.802f, 354.292f, 200000);
		GameObject* pLeftGate  = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(744.829f, 369.276f, 354.324f, 200001);
		if (pRightGate != NULL)
		{
			pRightGate->SetState(0);
		}
		if (pLeftGate != NULL)
		{
			pLeftGate->SetState(0);
		}*/

		delete this;
	}

	// Global variables
	uint32					mPhaseBackup;
	uint32					mScenePart;
	int32					mTimeLeft;
	bool					mAllow;

	// Phase 1 variables
	int32					mParasiticTimer;
	SpellDesc*				mParasiticDmg;
	SpellDesc*				mParasitic;

	// Phase 2 variables
	MoonScriptCreatureAI*	mFoA1;
	MoonScriptCreatureAI*	mFoA2;
	int32					mMovementTimer;
	int32					mFireWallTimer;
	int32					mLastFireWall;
	uint32					mMiscEventPart;

	// Phase 3 variables
	SpellDesc*				mShadowPrison;

	// Phase 4 variables
	int32					mShadowDemonsTimer;
	int32					mFlameBurstTimer;
	SpellDesc*				mFlameBurst;
	SpellDesc*				mShadowDemons;
	SpellDesc*				mShadowBlast;
	bool					mPlaySound;

	// Phase 3 & 4 variables
	int32					mDemonTimer;

	// Phase 5 variables
	int32					mYellTimer;
	int32					mEnrageTimer;
	SpellDesc*				mEnrage;

	// Temporary variables
	uint32					mCurrentWaypoint;
};

void SpellFunc_Illidan_Parasitic(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType)
{
	IllidanStormrageAI *Illidan = (pCreatureAI != NULL) ? (IllidanStormrageAI*)pCreatureAI : NULL;
	if (Illidan != NULL)
	{
		Illidan->CastSpell(Illidan->mParasiticDmg);
		if (pTarget != NULL)						// not sure if target is really added here
		{
			// Workaround - we will spawn 2 Parasitic Shadowfiends on that player place
			Illidan->SpawnCreature(CN_PARASITIC_SHADOWFIEND, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), pTarget->GetOrientation(), false);
			Illidan->SpawnCreature(CN_PARASITIC_SHADOWFIEND, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), pTarget->GetOrientation(), false);
		}
	}
}

/*END
			.:Rewritten Illidan Script by M4ksiu:.
END*/

// Cage Trap Trigger AI
#define CN_CAGE_TRAP_DISTURB_TRIGGER	23304		// GO data taken from UDB
#define CN_CAGE_TRAP_TRIGGER			23292		// 23292 - 9
#define GO_CAGE_TRAP					185916

#define CAGE_TRAP						40760
#define CAGED1							40695
#define CAGED2							40713		// must find proper spell for it

float PositionAdds[8][2]=
{
	{ 0, 1 },
	{ 0.5, 0.5 },
	{ 1, 0 },
	{ 0.5, -0.5 },
	{ 0, 1 },
	{ -0.5, 0.5 },
	{ -1, 0 },
	{ -0.5, 0.5 }
};

class CageTrapTriggerAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(CageTrapTriggerAI, MoonScriptCreatureAI);
    CageTrapTriggerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		_unit->GetAIInterface()->m_canMove = false;
		_unit->m_noRespawn = true;

		RegisterAIUpdateEvent(1000);
		SetAllowMelee(false);
		mIsActivated = false;
		mHasTrapped = false;
    }

	void OnCombatStart(Unit*  pTarget) {}

    void OnCombatStop(Unit*  pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
    }

    void AIUpdate()
    {
		Unit* pIllidan = ForceCreatureFind(22917);
		if (pIllidan != NULL)
		{
			IllidanStormrageAI *pAI = (IllidanStormrageAI*)(static_cast<MoonScriptCreatureAI*>(TO_CREATURE(pIllidan)->GetScript()));
			if (pAI->mMiscEventPart != 0 && mTriggerAIList.size() == 0)
			{
				GameObject* pGameObject = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), GO_CAGE_TRAP);
				if (pGameObject != NULL)
				{
					pGameObject->Despawn(0,0);
				}

				Despawn(0);
				return;
			}

			if (!mIsActivated)
				return;

			if (mTriggerAIList.size() == 0 && pIllidan->isAlive() && GetRangeToUnit(pIllidan) <= 5.0f && !pAI->IsCasting())
			{
				for (int i = 0; i < 8; ++i)
				{
					MoonScriptCreatureAI *pTriggerAI = SpawnCreature(CN_CAGE_TRAP_TRIGGER, _unit->GetPositionX() + PositionAdds[i][0], _unit->GetPositionY() + PositionAdds[i][1], _unit->GetPositionZ(), _unit->GetOrientation());
					if (pTriggerAI != NULL)
					{
						pTriggerAI->GetUnit()->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
						pTriggerAI->GetUnit()->GetAIInterface()->m_canMove = false;
						pTriggerAI->GetUnit()->m_noRespawn = true;
						mTriggerAIList.push_back(pTriggerAI);
					}
				}

				pAI->ApplyAura(CAGE_TRAP);
				pAI->SetCanMove(false);
				pAI->SetAllowMelee(false);
				pAI->StopMovement();

				SetAIUpdateFreq(2500);
				return;
			}
			else if (pIllidan->isAlive() && !mHasTrapped && mTriggerAIList.size() != 0)
			{
				pAI->RemoveAura(ILLIDAN_ENRAGE);
				pAI->ApplyAura(CAGED1);
				SetAIUpdateFreq(14000);

				for (size_t i = 0; i < mTriggerAIList.size() / 2; ++i)
				{
					MoonScriptCreatureAI *pTriggerAI1 = mTriggerAIList[i];
					MoonScriptCreatureAI *pTriggerAI2 = mTriggerAIList[mTriggerAIList.size() - i - 1];
					pTriggerAI1->SetTargetToChannel(pTriggerAI2->GetUnit(), 40708);//CAGED2);
					pTriggerAI2->SetTargetToChannel(pTriggerAI1->GetUnit(), 40709);//CAGED2);
				}

				mHasTrapped = true;
				return;
			}
			else if (mHasTrapped || !pIllidan->isAlive())
			{
				GameObject* pGameObject = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), GO_CAGE_TRAP);
				if (pGameObject != NULL)
				{
					pGameObject->Despawn(0,0);
				}

				// Immediatly Illidan, after trap expires, becomes Demon
				if (pIllidan->isAlive())
				{
					pAI->RemoveAura(CAGED1);
					pAI->SetCanMove(true);
					pAI->SetAllowMelee(true);
					pAI->StopMovement();
					pAI->SetBehavior(Behavior_Spell);

					pIllidan->SetEmoteState(0);

					pAI->SetAIUpdateFreq(250);
					pAI->mMiscEventPart = 1;
					pAI->mTimeLeft = 250;
				}
				for (size_t i = 0; i < mTriggerAIList.size(); ++i)
				{
					MoonScriptCreatureAI *pTriggerAI = mTriggerAIList[i];
					pTriggerAI->SetTargetToChannel(NULL, 0);
					pTriggerAI->Despawn(0);
				}

				mTriggerAIList.clear();
				RemoveAIUpdateEvent();
				Despawn(0);
				return;
			}
		}
		else
		{
			GameObject* pGameObject = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), GO_CAGE_TRAP);
			if (pGameObject != NULL)
			{
				pGameObject->Despawn(0,0);
			}

			for (size_t i = 0; i < mTriggerAIList.size(); ++i)
			{
				MoonScriptCreatureAI *pTriggerAI = mTriggerAIList[i];
				pTriggerAI->Despawn(0);
			}

			mTriggerAIList.clear();
			RemoveAIUpdateEvent();
			Despawn(0);
		}
    }

	std::vector<MoonScriptCreatureAI*>	mTriggerAIList;
	bool								mIsActivated;
	bool								mHasTrapped;
};

// Animation progress is wrong - it should be closed and OnActive - open

class CageTrapGO : public GameObjectAIScript
{
public:
	CageTrapGO(GameObject* pGameObject) : GameObjectAIScript(pGameObject)
	{
		_gameobject->SetFloatValue(OBJECT_FIELD_SCALE_X, 3);
	}

	void OnActivate(Player* pPlayer)
	{
		_gameobject->SetUInt32Value(GAMEOBJECT_FLAGS, 1);
		Creature* pTrigger = _gameobject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_gameobject->GetPositionX(), _gameobject->GetPositionY(), _gameobject->GetPositionZ(), CN_CAGE_TRAP_DISTURB_TRIGGER);
		if (pTrigger != NULL && pTrigger->GetScript() != NULL)
		{
			CageTrapTriggerAI *pTriggerAI = static_cast<CageTrapTriggerAI*>(pTrigger->GetScript());
			pTriggerAI->mIsActivated = true;
		}
	}

	static GameObjectAIScript *Create(GameObject* pGameObject) { return new CageTrapGO(pGameObject); }
};

// Udalo and Olum AI
#define CN_UDALO	23410
#define CN_OLUM		23411

class DranaeiSpiritAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(DranaeiSpiritAI, MoonScriptCreatureAI);
    DranaeiSpiritAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		Despawn(45000);
		SetCanEnterCombat(false);
		_unit->m_noRespawn = true;
	}

	void Destroy()
	{
		Creature* pAkama = TO_CREATURE(ForceCreatureFind(22990));
		if (pAkama != NULL && pAkama->GetScript() != NULL)
		{
			AkamaAI *pAI = (AkamaAI*)(pAkama->GetScript());
			if (pAI != NULL)
			{
				if (pAI->mUdaloAI == (MoonScriptCreatureAI*)(this))
					pAI->mUdaloAI = NULL;
				else if (pAI->mOlumAI == (MoonScriptCreatureAI*)(this))
					pAI->mOlumAI = NULL;
			}
		}

		delete this;
	}
};

// Flame of Azzinoth AI
#define CN_FLAME_OF_AZZINOTH				22997	

#define FLAME_OF_AZZINOTH_BLAZE				40637		// this one just summons mob that will trigger that fire cloud - currently not used
#define FLAME_OF_AZZINOTH_FLAME_BLAST		40631
#define FLAME_OF_AZZINOTH_ENRAGE			40683		// 40683 or 40743
#define FLAME_OF_AZZINOTH_CHARGE			40602		// CHAOS_CHARGE 40497 CHARGE 40602

void SpellFunc_FlameOfAzzinothFlameBlast(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);
void SpellFunc_FlameOfAzzinothCharge(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);

class FlameOfAzzinothAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(FlameOfAzzinothAI, MoonScriptCreatureAI);
    FlameOfAzzinothAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		mFlameBlast = AddSpell(FLAME_OF_AZZINOTH_FLAME_BLAST, Target_Self, 0, 0, 0);
		AddSpellFunc(&SpellFunc_FlameOfAzzinothFlameBlast, Target_Current, 15, 0, 25, 0, 10);

		mCharge = AddSpell(FLAME_OF_AZZINOTH_CHARGE, Target_Current, 0, 0, 0);
		mEnrage = AddSpell(FLAME_OF_AZZINOTH_ENRAGE, Target_Self, 0, 0, 0);
		mChargeSpellFunc = AddSpellFunc(&SpellFunc_FlameOfAzzinothCharge, Target_Current, 0, 0, 60);

		_unit->m_noRespawn = true;

		Unit* pTarget = GetBestPlayerTarget(TargetFilter_Closest);
		if (pTarget != NULL)
		{
			_unit->GetAIInterface()->AttackReaction(pTarget, 200);
			OnCombatStart(pTarget);
		}
    }

	void OnCombatStart(Unit*  pTarget)
	{
		ParentClass::OnCombatStart(pTarget);
		CancelAllCooldowns();
	}

	void AIUpdate()
	{
		// Uh... so ugly, but that's what Wiki says
		Unit* pBlade1 = ForceCreatureFind(22996, UnitPos[0].x, UnitPos[0].y, UnitPos[0].z);
		Unit* pBlade2 = ForceCreatureFind(22996, UnitPos[1].x, UnitPos[1].y, UnitPos[1].z);
		if (pBlade1 != NULL && pBlade2 != NULL && mChargeSpellFunc->mLastCastTime + mChargeSpellFunc->mCooldown <= (uint32)time(NULL))
		{
			for (set< Object* >::iterator itr = _unit->GetInRangePlayerSetBegin(); itr != _unit->GetInRangePlayerSetEnd(); ++itr) 
			{	// && or || ? - not sure about details too
				if ((*itr)->CalcDistance(pBlade1) > 40.0f || (*itr)->CalcDistance(pBlade2) > 40.0f)
				{
					Unit *pUnit = static_cast< Unit* >(*itr);
					ClearHateList();
					_unit->GetAIInterface()->setNextTarget(pUnit);
					_unit->GetAIInterface()->AttackReaction(pUnit, 10000);
					CastSpellNowNoScheduling(mChargeSpellFunc);
					break;
				}
			}
		}

		ParentClass::AIUpdate();
	}

	void Destroy()
	{
		Creature* pIllidan = TO_CREATURE(ForceCreatureFind(22917));
		if (pIllidan != NULL && pIllidan->GetScript() != NULL)
		{
			IllidanStormrageAI *pAI = (IllidanStormrageAI*)(pIllidan->GetScript());
			if (pAI != NULL)
			{
				if (pAI->mFoA1 == (MoonScriptCreatureAI*)(this))
					pAI->mFoA1 = NULL;
				else if (pAI->mFoA2 == (MoonScriptCreatureAI*)(this))
					pAI->mFoA2 = NULL;
			}
		}

		delete this;
	}

	// Flame Blast Spell Function
	SpellDesc*	mFlameBlast;
	SpellDesc*	mBlaze;

	// Charge Spell Function
	SpellDesc*	mChargeSpellFunc;
	SpellDesc*	mCharge;
	SpellDesc*	mEnrage;
};

void SpellFunc_FlameOfAzzinothFlameBlast(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType)
{
	FlameOfAzzinothAI* FlameOfAzzinoth = ( pCreatureAI ) ? (FlameOfAzzinothAI*)pCreatureAI : NULL;
	if( FlameOfAzzinoth )
	{
		FlameOfAzzinoth->CastSpell(FlameOfAzzinoth->mFlameBlast);
		FlameOfAzzinoth->SpawnCreature(CN_BLAZE_EFFECT, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), pTarget->GetOrientation());
	}
}

void SpellFunc_FlameOfAzzinothCharge(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType)
{
	FlameOfAzzinothAI* FlameOfAzzinoth = ( pCreatureAI ) ? (FlameOfAzzinothAI*)pCreatureAI : NULL;
	if( FlameOfAzzinoth )
	{
		FlameOfAzzinoth->CastSpell(FlameOfAzzinoth->mCharge);
		FlameOfAzzinoth->CastSpell(FlameOfAzzinoth->mEnrage);
	}
}

void SetupBlackTemple(ScriptMgr * mgr)
{
	mgr->register_instance_script( MAP_BLACK_TEMPLE, &BlackTempleScript::Create );
	//////////////////////////////////////////////////////////////////////////////////////////
	///////// Mobs
	mgr->register_creature_script(CN_DRAGON_TURTLE, &DragonTurtleAI::Create);
	mgr->register_creature_script(CN_LEVIATHAN, &LeviathanAI::Create);
	mgr->register_creature_script(CN_MUTANT_WAR_HOUND, &MutantWarHoundAI::Create);
	mgr->register_creature_script(CN_SHADOWMOON_RIDING_HOUND, &ShadowmoonRidingHoundAI::Create);
	mgr->register_creature_script(CN_SISTER_OF_PLEASURE, &SisterOfPleasureAI::Create);
	mgr->register_creature_script(CN_SISTER_OF_PAIN, &SisterOfPainAI::Create);
	mgr->register_creature_script(CN_PRIESTESS_OF_DEMENTIA, &PriestessOfDementiaAI::Create);
	mgr->register_creature_script(CN_PRIESTESS_OF_DELIGHT, &PriestessOfDelightAI::Create);
	mgr->register_creature_script(CN_ILLIDARI_NIGHTLORD, &IllidariNightlordAI::Create);
	mgr->register_creature_script(CN_ILLIDARI_HEARTSEEKER, &IllidariHeartseekerAI::Create);
	mgr->register_creature_script(CN_ILLIDARI_FEARBRINGER, &IllidariFearbringerAI::Create);
	mgr->register_creature_script(CN_ILLIDARI_DEFILER, &IllidariDefilerAI::Create);
	mgr->register_creature_script(CN_ILLIDARI_CENTURION, &IllidariCenturionAI::Create);
	mgr->register_creature_script(CN_ILLIDARI_BONESLICER, &IllidariBoneslicerAI::Create);
	mgr->register_creature_script(CN_ASHTONGUE_BATTLELORD, &AshtongueBattlelordAI::Create);
	mgr->register_creature_script(CN_ASHTONGUE_DEFENDER, &AshtongueDefenderAI::Create);
	mgr->register_creature_script(CN_ASHTONGUE_ELEMENTALIST, &AshtongueElementalistAI::Create);
	mgr->register_creature_script(CN_ASHTONGUE_MYSTIC, &AshtongueMysticAI::Create);
	mgr->register_creature_script(CN_ASHTONGUE_PRIMALIST, &AshtonguePrimalistAI::Create);
	mgr->register_creature_script(CN_ASHTONGUE_ROGUE, &AshtongueRogueAI::Create);
	mgr->register_creature_script(CN_ASHTONGUE_SPIRITBINDER, &AshtongueSpiritbinderAI::Create);
	mgr->register_creature_script(CN_ASHTONGUE_STALKER, &AshtongueStalkerAI::Create);
	mgr->register_creature_script(CN_ASHTONGUE_STORMCALLER, &AshtongueStormcallerAI::Create);
	mgr->register_creature_script(CN_BONECHEWER_BEHEMOTH, &BonechewerBehemothAI::Create);
	mgr->register_creature_script(CN_BONECHEWER_BLADE_FURY, &BonechewerBladeFuryAI::Create);
	mgr->register_creature_script(CN_BONECHEWER_BLOOD_PROPHET, &BonechewerBloodProphetAI::Create);
	mgr->register_creature_script(CN_BONECHEWER_BRAWLER, &BonechewerBrawlerAI::Create);
	mgr->register_creature_script(CN_BONECHEWER_COMBATANT, &BonechewerCombatantAI::Create);
	mgr->register_creature_script(CN_BONECHEWER_SHIELD_DISCIPLE, &BonechewerShieldDiscipleAI::Create);
	mgr->register_creature_script(CN_BONECHEWER_SPECTATOR, &BonechewerSpectatorAI::Create);
	mgr->register_creature_script(CN_BONECHEWER_TASKMASTER, &BonechewerTaskmasterAI::Create);
	mgr->register_creature_script(CN_BONECHEWER_WORKER, &BonechewerWorkerAI::Create);
	mgr->register_creature_script(CN_CHARMING_COURTESAN, &CharmingCourtesanAI::Create);
	mgr->register_creature_script(CN_COILSKAR_GENERAL, &CoilskarGeneralAI::Create);
	mgr->register_creature_script(CN_COILSKAR_HARPOONER, &CoilskarHarpoonerAI::Create);
	mgr->register_creature_script(CN_COILSKAR_SEACALLER, &CoilskarSeacallerAI::Create);
	mgr->register_creature_script(CN_COILSKAR_SOOTHSAYER, &CoilskarSoothsayerAI::Create);
	mgr->register_creature_script(CN_COILSKAR_WRANGLER, &CoilskarWranglerAI::Create);
	mgr->register_creature_script(CN_DRAGONMAW_SKY_STALKER, &DragonmawSkyStalkerAI::Create);
	mgr->register_creature_script(CN_DRAGONMAW_WIND_REAVER, &DragonmawWindReaverAI::Create);
	mgr->register_creature_script(CN_DRAGONMAW_WYRMCALLER, &DragonmawWyrmcallerAI::Create);
	mgr->register_creature_script(CN_ENSLAVED_SERVANT, &EnslavedServantAI::Create);
	mgr->register_creature_script(CN_HAND_OF_GOREFIEND, &HandOfGorefiendAI::Create);
	mgr->register_creature_script(CN_ILLIDARI_ARCHON, &IllidariArchonAI::Create);
	mgr->register_creature_script(CN_ILLIDARI_ASSASSIN, &IllidariAssassinAI::Create);
	mgr->register_creature_script(CN_ILLIDARI_BATTLEMAGE, &IllidariBattlemageAI::Create);
	mgr->register_creature_script(CN_ILLIDARI_BLOOD_LORD, &IllidariBloodLordAI::Create);
	mgr->register_creature_script(CN_IMAGE_OF_DEMENTIA, &ImageOfDementiaAI::Create);
	mgr->register_creature_script(CN_SHADOWMOON_BLOOD_MAGE, &ShadowmoonBloodMageAI::Create);
	mgr->register_creature_script(CN_SHADOWMOON_CHAMPION, &ShadowmoonChampionAI::Create);
	mgr->register_creature_script(CN_SHADOWMOON_DEATHSHAPER, &ShadowmoonDeathshaperAI::Create);
	mgr->register_creature_script(CN_SHADOWMOON_FALLEN, &ShadowmoonFallenAI::Create);
	mgr->register_creature_script(CN_SHADOWMOON_HOUNDMASTER, &ShadowmoonHoundmasterAI::Create);
	mgr->register_creature_script(CN_SHADOWMOON_REAVER, &ShadowmoonReaverAI::Create);
	mgr->register_creature_script(CN_SHADOWMOON_SOLDIER, &ShadowmoonSoldierAI::Create);
	mgr->register_creature_script(CN_SHADOWMOON_WEAPON_MASTER, &ShadowmoonWeaponMasterAI::Create);
	mgr->register_creature_script(CN_SPELLBOUND_ATTENDANT, &SpellboundAttendantAI::Create);
	mgr->register_creature_script(CN_TEMPLE_CONCUBINE, &TempleConcubineAI::Create);
	mgr->register_creature_script(CN_STORM_FURY, &StormFuryAI::Create);
	mgr->register_creature_script(CN_AQUEOUS_SURGER, &AqueousSurgerAI::Create);
	mgr->register_creature_script(CN_AQUEOUS_SPAWN, &AqueousSpawnAI::Create);
	mgr->register_creature_script(CN_AQUEOUS_LORD, &AqueousLordAI::Create);
	mgr->register_creature_script(CN_PROMENADE_SENTINEL, &PromenadeSentinelAI::Create);
	mgr->register_creature_script(CN_ANGERED_SOUL_FRAGMENT, &AngeredSoulFragmentAI::Create);
	//mgr->register_creature_script(CN_ASHTONGUE_FERAL_SPIRIT, &AshtongueFeralSpiritAI::Create);
	mgr->register_creature_script(CN_ENSLAVED_SOUL, &EnslavedSoulAI::Create);
	mgr->register_creature_script(CN_HUNGERING_SOUL_FRAGMENT, &HungeringSoulFragmentAI::Create);
	mgr->register_creature_script(CN_SHADOWY_CONSTRUCT, &ShadowyConstructAI::Create);
	mgr->register_creature_script(CN_SUFFERING_SOUL_FRAGMENT, &SufferingSoulFragmentAI::Create);
	mgr->register_creature_script(CN_VANGEFUL_SPIRIT, &VangefulSpiritAI::Create);
	mgr->register_creature_script(CN_WRATHBONE_FLAYER, &WrathboneFlayerAI::Create);

	//Bosses
	//mgr->register_creature_script(CN_SUPREMUS, &SupremusAI::Create);
	mgr->register_creature_script(CN_NAJENTUS, &NajentusAI::Create);
	mgr->register_creature_script(CN_GURTOGG_BLOODBOIL, &GurtoggAI::Create);

	mgr->register_creature_script(CN_RELIQUARY_OF_SOULS, &ReliquaryOfSoulsAI::Create);
	mgr->register_creature_script(CN_ESSENCEOFSUFFERING, &EssenceOfSufferingAI::Create);
	mgr->register_creature_script(CN_ESSENCEOFDESIRE, &EssenceOfDesireAI::Create);
	mgr->register_creature_script(CN_ESSENCEOFANGER, &EssenceOfAngerAI::Create);

	mgr->register_creature_script(CN_MOTHER_SHAHRAZ, &ShahrazAI::Create);
	mgr->register_creature_script(CN_LADY_MALANDE, &MalandeAI::Create);
	mgr->register_creature_script(CN_GATHIOS_THE_SHATTERER, &GathiosAI::Create);
	mgr->register_creature_script(CN_HIGH_NETHERMANCER_ZEREVOR, &ZerevorAI::Create);
	mgr->register_creature_script(CN_VERAS_DARKSHADOW, &VerasAI::Create);
	mgr->register_creature_script(CN_TERON_GOREFIEND, &TeronGorefiendAI::Create);
	//mgr->register_creature_script(CN_SHADE_OF_AKAMA, &ShadeofakamaAI::Create); //test

	//Illidan Stormrage related
	GossipScript * AG = new AkamaGossip();
	mgr->register_gossip_script(CN_AKAMA, AG);

	mgr->register_creature_script(CN_DOOR_EVENT_TRIGGER, &UnselectableTriggerAI::Create);
	mgr->register_creature_script(CN_FACE_TRIGGER, &UnselectableTriggerAI::Create);
	mgr->register_creature_script(CN_CAGE_TRAP_TRIGGER, &UnselectableTriggerAI::Create);
	mgr->register_creature_script(CN_CAGE_TRAP_DISTURB_TRIGGER, &CageTrapTriggerAI::Create);
	mgr->register_creature_script(CN_DEMON_FIRE, &GenericTriggerAI::Create);
	mgr->register_creature_script(CN_EYE_BEAM_TRIGGER, &EyeBeamTriggerAI::Create);
	mgr->register_creature_script(CN_FLAME_CRASH, &GenericTriggerAI::Create);
	mgr->register_creature_script(CN_BLAZE_EFFECT, &GenericTriggerAI::Create);
	mgr->register_creature_script(CN_FLAME_BURST, &GenericTriggerAI::Create);
	mgr->register_creature_script(CN_FLAME_OF_AZZINOTH, &FlameOfAzzinothAI::Create);
	mgr->register_creature_script(CN_SHADOW_DEMON, &ShadowDemonAI::Create);
	mgr->register_creature_script(CN_PARASITIC_SHADOWFIEND, &ParasiticShadowfiendAI::Create);
	mgr->register_creature_script(CN_AKAMA, &AkamaAI::Create);
	mgr->register_creature_script(CN_MAIEV, &MaievAI::Create);
	mgr->register_creature_script(CN_ILLIDAN_STORMRAGE, &IllidanStormrageAI::Create);
	mgr->register_creature_script(CN_UDALO, &DranaeiSpiritAI::Create);
	mgr->register_creature_script(CN_OLUM, &DranaeiSpiritAI::Create);
	mgr->register_gameobject_script(GO_CAGE_TRAP, &CageTrapGO::Create);
}
