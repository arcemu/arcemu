/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008 Arcemu Team
 * Copyright (C) 2007 Moon++ <http://www.moonplusplus.com/>
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

#include "StdAfx.h"
#include "Setup.h"
#include "Base.h"

/************************************************************************/
/* Instance_Arcatraz.cpp Script											*/
/************************************************************************/

// Arcatraz DefenderAI

#define CN_ARCATRAZ_DEFENDER				20857
#define ARCATRAZ_DEFENDER_PROTEAN_SUBDUAL	36288
#define ARCATRAZ_DEFENDER_FLAMING_WEAPON	36601
#define ARCATRAZ_DEFENDER_IMMOLATE			36638

class ArcatrazDefenderAI : public ArcScriptCreatureAI
{
    ARCSCRIPT_FACTORY_FUNCTION(ArcatrazDefenderAI, ArcScriptCreatureAI);
	ArcatrazDefenderAI(Creature* pCreature) : ArcScriptCreatureAI(pCreature)
	{
		AddSpell(ARCATRAZ_DEFENDER_PROTEAN_SUBDUAL, Target_Current, 5, 0, 0, 0, 5);
		AddSpell(ARCATRAZ_DEFENDER_FLAMING_WEAPON, Target_Current, 15, 0, 0, 0, 5);
		AddSpell(ARCATRAZ_DEFENDER_IMMOLATE, Target_Current, 8, 0, 0, 0, 30);
	}
};

// Arcatraz SentinelAI

#define CN_ARCATRAZ_SENTINEL				20869
#define ARCATRAZ_SENTINEL_ENERGY_DISCHARGE	36716	//36717
#define ARCATRAZ_SENTINEL_EXPLODE			36719	// DBC: 36722, 36719
//Permanent Feign Death (Root) 31261 ?

class ArcatrazSentinelAI : public ArcScriptCreatureAI
{
    ARCSCRIPT_FACTORY_FUNCTION(ArcatrazSentinelAI, ArcScriptCreatureAI);
	ArcatrazSentinelAI(Creature* pCreature) : ArcScriptCreatureAI(pCreature)
	{
		AddSpell(ARCATRAZ_SENTINEL_ENERGY_DISCHARGE, Target_Self, 8, 0, 0);
		mExplode = AddSpell(ARCATRAZ_SENTINEL_EXPLODE, Target_Self, 0, 0, 0);
	}

	void OnCombatStart(Unit* pTarget)
	{
		mExplode->mEnabled = true;
		ParentClass::OnCombatStart(pTarget);
	}

	void AIUpdate()
	{
		if( mExplode->mEnabled && GetHealthPercent() <= 5 && !IsCasting() )
		{
			CastSpellNowNoScheduling(mExplode);
			mExplode->mEnabled = false;
		}
		ParentClass::AIUpdate();
	}

	SpellDesc* mExplode;
};


// Arcatraz WarderAI
// SPELL CASTING TO REWRITE (spell ids are correct)
#define CN_ARCATRAZ_WARDER 20859

#define ARCANE_SHOT 36609
#define SHOT 15620
#define IMPROVED_WING_CLIP 35963
#define CHARGED_ARCANE_SHOT 36608

//Shoot Arcane Explosion Arrow 36327 ?

class ArcatrazWarderAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ArcatrazWarderAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    ArcatrazWarderAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
        spells[0].info = dbcSpell.LookupEntry(ARCANE_SHOT);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = dbcSpell.LookupEntry(SHOT);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].cooldown = -1;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(IMPROVED_WING_CLIP);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].cooldown = -1;
		spells[2].perctrigger = 8.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(CHARGED_ARCANE_SHOT);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = true;
		spells[3].cooldown = -1;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnTargetDied(Unit* mTarget)
    {
    }

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		if (_unit->GetAIInterface()->GetNextTarget())
		{
			Unit *target = NULL;
			target = _unit->GetAIInterface()->GetNextTarget();
			if (_unit->GetDistance2dSq(target) >= 100.0f && _unit->GetDistanceSq(target) <= 900.0f)
			{
				_unit->GetAIInterface()->m_canMove = false;
				int RangedSpell;
				RandomUInt(1000);
				RangedSpell=rand()%100;
				if (RangedSpell >= 0 && RangedSpell <= 20)
				{
					_unit->CastSpell(target, spells[3].info, spells[3].instant);
					_unit->setAttackTimer(spells[3].attackstoptimer, false);
				}
				if (RangedSpell > 20 && RangedSpell <= 40)
				{
					_unit->CastSpell(target, spells[0].info, spells[0].instant);
					_unit->setAttackTimer(spells[0].attackstoptimer, false);
				}
				else
				{
					_unit->CastSpell(target, spells[1].info, spells[1].instant);
					_unit->setAttackTimer(spells[1].attackstoptimer, false);
				}
			}

			else
			{
				_unit->GetAIInterface()->m_canMove = true;
				if (_unit->GetDistance2dSq(target) < 100.0f)
				{
					float val = (float)RandomFloat(100.0f);
					SpellCast(val);
				}
			}
		}
    }

	void SpellCast(float val)
	{
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())	// I was too lazy to rewrite it =S
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
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

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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
};

// Blackwing DrakonaarAI
// SIMPLE_TELEPORT should be used when creature is summoned during Skyriss encounter
#define CN_BLACKWING_DRAKONAAR					20911
#define BLACKWING_DRAKONAAR_SIMPLE_TELEPORT		12980
#define BLACKWING_DRAKONAAR_BROOD_POWER_BLACK	39033
#define BLACKWING_DRAKONAAR_MORTAL_STRIKE		13737
#define BLACKWING_DRAKONAAR_BLAST_WAVE			39038

class BlackwingDrakonaarAI : public ArcScriptCreatureAI
{
    ARCSCRIPT_FACTORY_FUNCTION(BlackwingDrakonaarAI, ArcScriptCreatureAI);
	BlackwingDrakonaarAI(Creature* pCreature) : ArcScriptCreatureAI(pCreature)
	{
		AddSpell(BLACKWING_DRAKONAAR_BROOD_POWER_BLACK, Target_Self, 7, 0, 0);
		AddSpell(BLACKWING_DRAKONAAR_MORTAL_STRIKE, Target_Current, 10, 0, 0, 0, 8, true);
		AddSpell(BLACKWING_DRAKONAAR_BLAST_WAVE, Target_Self, 6, 0, 0);
	}
};

// Blazing TricksterAI

#define CN_BLACKWING_TRICKSTER			20905
#define BLACKWING_TRICKSTER_FIREBOLT	36906
#define BLACKWING_TRICKSTER_FIRE_SHIELD	36907

class BlackwingTricksterAI : public ArcScriptCreatureAI
{
    ARCSCRIPT_FACTORY_FUNCTION(BlackwingTricksterAI, ArcScriptCreatureAI);
	BlackwingTricksterAI(Creature* pCreature) : ArcScriptCreatureAI(pCreature)
	{
		AddSpell(BLACKWING_TRICKSTER_FIREBOLT, Target_Current, 10, 1000, 0, 0, 40, true);
		AddSpell(BLACKWING_TRICKSTER_FIRE_SHIELD, Target_Self, 7, 0, 0);
	}
};

// Death WatcherAI

#define CN_DEATH_WATCHER				20867
#define DEATH_WATCHER_TENTACLE_CLEAVE	36664
#define DEATH_WATCHER_DRAIN_LIFE		36655
#define DEATH_WATCHER_DEATH_COUNT		36657	// DBC: 36657, 36660

class DeathWatcherAI : public ArcScriptCreatureAI
{
    ARCSCRIPT_FACTORY_FUNCTION(DeathWatcherAI, ArcScriptCreatureAI);
	DeathWatcherAI(Creature* pCreature) : ArcScriptCreatureAI(pCreature)
	{
		AddSpell(DEATH_WATCHER_TENTACLE_CLEAVE, Target_Current, 10, 0, 0, 0, 8, true);
		AddSpell(DEATH_WATCHER_DRAIN_LIFE, Target_Current, 8, 0, 0, 0, 20, true);
		mDeathCount = AddSpell(DEATH_WATCHER_DEATH_COUNT, Target_Self, 0, 0, 0);
	}

	void OnCombatStart(Unit* pTarget)
	{
		mDeathCount->mEnabled = true;
		ParentClass::OnCombatStart(pTarget);
	}

	void AIUpdate()
	{
		if( mDeathCount->mEnabled && GetHealthPercent() <= 50 && !IsCasting() )
		{
			CastSpellNowNoScheduling(mDeathCount);
			mDeathCount->mEnabled = false;
		}
		ParentClass::AIUpdate();
	}

	SpellDesc* mDeathCount;
};

// Defender CorpseAI
// Seems to be ok, but not sure if I won't mess with this script a bit
#define CN_DEFENDER_CORPSE					21303
#define DEFENDER_CORPSE_CORPSE_BURST		36593
#define DEFENDER_CORPSE_BLOODY_EXPLOSION	36599
//#define PERMANENT_FEIGN_DEATH_ROOT 31261	// I don't have much infos and I am not sure about how it should work
											// I presume this is just corpse which should cast spell when player is close or attk it
class DefenderCorpseAI : public ArcScriptCreatureAI
{
    ARCSCRIPT_FACTORY_FUNCTION(DefenderCorpseAI, ArcScriptCreatureAI);
	DefenderCorpseAI(Creature* pCreature) : ArcScriptCreatureAI(pCreature)
	{
		mCorpseBurst = AddSpell(DEFENDER_CORPSE_CORPSE_BURST, Target_Self, 0, 0, 0);
		mBloodyExplosion = AddSpell(DEFENDER_CORPSE_BLOODY_EXPLOSION, Target_Self, 0, 0, 0);
	}

	void OnCombatStart(Unit* pTarget)
	{
		CastSpellNowNoScheduling(mCorpseBurst);
		CastSpellNowNoScheduling(mBloodyExplosion);

		// Slap me, if this does not work
		_unit->SetUInt32Value(UNIT_FIELD_HEALTH, 0);
		_unit->setDeathState(DEAD);

		// Let's keep it safe tho
		Despawn(10000, 0);
	}

	SpellDesc* mCorpseBurst;
	SpellDesc* mBloodyExplosion;
};

// Protean SpawnAI

#define CN_PROTEAN_SPAWN			21395	// it's sth that spawns from Defender Corpse
#define PROTEAN_SPAWN_ACIDIC_BITE	36796
//#define BLOODY_EXPLOSION 36588

class ProteanSpawnAI : public ArcScriptCreatureAI
{
    ARCSCRIPT_FACTORY_FUNCTION(ProteanSpawnAI, ArcScriptCreatureAI);
	ProteanSpawnAI(Creature* pCreature) : ArcScriptCreatureAI(pCreature)
	{
		AddSpell(PROTEAN_SPAWN_ACIDIC_BITE, Target_Current, 8, 0, 0, 0, 8, true);
	}
};

// Entropic EyeAI			-- TO RECHECK BEFORE REWRITE --

#define CN_ENTROPIC_EYE 20868

#define WAVERING_WILL 36699			// not sure if this is right one (I mean spell, not ID)
#define PIERCING_SHADOW 36698		// this "smaller" list must be rechecked =S
#define HEX 36700
#define TANTACLE_CLEAVE_EYE 36664

/*
#define TANTACLE_CLEAVE_EYE 36664
#define FIREBALL_EYE 36711
#define FROSTBOLT_EYE 36710
#define WITHERED_TOUCH_EYE 36696
#define HEX_EYE 36700
#define PIERCING_SHADOW_EYE 36698
#define CHAOS_BREATH_EYE 36677	// SSS
#define POISON_BOLT_EYE 36713				// DBC List :O
#define NECROTIC_POISON_EYE 36693
#define SHRINK_EYE 36697
#define SHADOW_BOLT_EYE 36714
#define FEVERED_FATIGUE_EYE 36695
#define WAVERING_WILL_EYE 36699
#define CORROSIVE_POISON_EYE 36694
#define ARCANE_BOLT_EYE 36712
*/

class EntropicEyeAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(EntropicEyeAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    EntropicEyeAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
        spells[0].info = dbcSpell.LookupEntry(WAVERING_WILL);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(PIERCING_SHADOW);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].cooldown = -1;
		spells[1].perctrigger = 5.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(HEX);
		spells[2].targettype = TARGET_VARIOUS;
		spells[2].instant = true;
		spells[2].cooldown = -1;
		spells[2].perctrigger = 5.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(TANTACLE_CLEAVE_EYE);
		spells[3].targettype = TARGET_ATTACKING;	// changed from VAR. to prevent crashes
		spells[3].instant = true;
		spells[3].cooldown = -1;
		spells[3].perctrigger = 12.0f;
		spells[3].attackstoptimer = 1000;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		CastTime();
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
		CastTime();
    }

    void OnDied(Unit * mKiller)
    {
		RemoveAIUpdateEvent();
		CastTime();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
    }

	void SpellCast(float val)
	{
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
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

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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
};

// Eredar DeathbringerAI

#define CN_EREDAR_DEATHBRINGER				20880
#define EREDAR_DEATHBRINGER_UNHOLY_AURA		27987	// DBC: 27988, 27987
#define EREDAR_DEATHBRINGER_DIMINISH_SOUL	36789	// DBC: 36789, 36791 (SSS)
#define EREDAR_DEATHBRINGER_FORCEFUL_CLEAVE	36787

class EredarDeathbringerAI : public ArcScriptCreatureAI
{
    ARCSCRIPT_FACTORY_FUNCTION(EredarDeathbringerAI, ArcScriptCreatureAI);
	EredarDeathbringerAI(Creature* pCreature) : ArcScriptCreatureAI(pCreature)
	{
		AddSpell(EREDAR_DEATHBRINGER_DIMINISH_SOUL, Target_Current, 7, 0, 0, 0, 8, true);		// Correct target type?
		AddSpell(EREDAR_DEATHBRINGER_FORCEFUL_CLEAVE, Target_Current, 13, 0, 0, 0, 8, true);
		ApplyAura(EREDAR_DEATHBRINGER_UNHOLY_AURA);
	}
};

// Eredar Soul-EaterAI

#define CN_EREDAR_SOUL_EATER 20879

#define SOUL_CHILL 36786
#define SOUL_STEAL1 36778	// this on target, another on self
#define SOUL_STEAL2 36782	// DBC: 36782, 36778

class EredarSoulEaterAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(EredarSoulEaterAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    EredarSoulEaterAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
        spells[0].info = dbcSpell.LookupEntry(SOUL_CHILL);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(SOUL_STEAL1);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].cooldown = -1;
		spells[1].perctrigger = 7.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(SOUL_STEAL2);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;
		spells[2].cooldown = -1;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		CastTime();
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
		CastTime();
    }

    void OnDied(Unit * mKiller)
    {
		RemoveAIUpdateEvent();
		CastTime();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
    }

	void SpellCast(float val)
	{
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
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

					if (m_spellcheck[1] == true)
						_unit->CastSpell(_unit, spells[2].info, spells[2].instant);

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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
};

// Ethereum Life-BinderAI

#define CN_ETHEREUM_LIFE_BINDER					21702
#define ETHEREUM_LIFE_BINDER_BIND				37480
#define ETHEREUM_LIFE_BINDER_SHADOW_WORD_PAIN	15654

class EthereumLifeBinderAI : public ArcScriptCreatureAI
{
    ARCSCRIPT_FACTORY_FUNCTION(EthereumLifeBinderAI, ArcScriptCreatureAI);
	EthereumLifeBinderAI(Creature* pCreature) : ArcScriptCreatureAI(pCreature)
	{
		AddSpell(ETHEREUM_LIFE_BINDER_BIND, Target_Self, 8, 1500, 0);
		AddSpell(ETHEREUM_LIFE_BINDER_SHADOW_WORD_PAIN, Target_Current, 7, 0, 0, 0, 30, true);
	}
};

// Ethereum SlayerAI

#define CN_ETHEREUM_SLAYER				20896
#define ETHEREUM_SLAYER_IMPARING_POISON	36839
#define ETHEREUM_SLAYER_SLAYER_STRIKE	36838
#define ETHEREUM_SLAYER_EVASION			15087

class EthereumSlayerAI : public ArcScriptCreatureAI
{
    ARCSCRIPT_FACTORY_FUNCTION(EthereumSlayerAI, ArcScriptCreatureAI);
	EthereumSlayerAI(Creature* pCreature) : ArcScriptCreatureAI(pCreature)
	{
		AddSpell(ETHEREUM_SLAYER_IMPARING_POISON, Target_Current, 7, 0, 0, 0, 40, true);
		AddSpell(ETHEREUM_SLAYER_SLAYER_STRIKE, Target_Current, 12, 0, 0, 0, 8, true);
		AddSpell(ETHEREUM_SLAYER_EVASION, Target_Self, 6, 0, 0);
	}
};

// Ethereum Wave-CasterAI

#define CN_ETHEREUM_WAVE_CASTER				20897
#define ETHEREUM_WAVE_CASTER_POLYMORPH		36840
#define ETHEREUM_WAVE_CASTER_ARCANE_HASTE	32693
#define ETHEREUM_WAVE_CASTER_SONIC_BOMB		36841	// should counter spell so I must code it a bit more
// KNOCKBACK ?

class EthereumWaveCasterAI : public ArcScriptCreatureAI
{
    ARCSCRIPT_FACTORY_FUNCTION(EthereumWaveCasterAI, ArcScriptCreatureAI);
	EthereumWaveCasterAI(Creature* pCreature) : ArcScriptCreatureAI(pCreature)
	{
		AddSpell(ETHEREUM_WAVE_CASTER_POLYMORPH, Target_RandomPlayer, 7, 1500, 0, 0, 30, true);
		AddSpell(ETHEREUM_WAVE_CASTER_ARCANE_HASTE, Target_Self, 5, 0, 0);
		AddSpell(ETHEREUM_WAVE_CASTER_SONIC_BOMB, Target_Current, 11, 0, 0, 0, 45, true);
	}
};

// Gargantuan AbyssalAI

#define CN_GARGANTUAN_ABYSSAL			20898
#define GARGANTUAN_ABYSSAL_FIRE_SHIELD	38855	// DBC: 38855, 38893
#define GARGANTUAN_ABYSSAL_METEOR		36837

class GargantuanAbyssalAI : public ArcScriptCreatureAI
{
    ARCSCRIPT_FACTORY_FUNCTION(GargantuanAbyssalAI, ArcScriptCreatureAI);
	GargantuanAbyssalAI(Creature* pCreature) : ArcScriptCreatureAI(pCreature)
	{
		AddSpell(GARGANTUAN_ABYSSAL_FIRE_SHIELD, Target_Self, 7, 0, 0);
		AddSpell(GARGANTUAN_ABYSSAL_METEOR, Target_RandomDestination, 8, 3000, 0, 0, 80, true);
	}
};

// Greater Fire ElementalAI
// FIX ME: Mark of death
#define CN_GREATER_FIRE_ELEMENTAL				15438
#define GREATER_FIRE_ELEMENTAL_FIRE_SHIELD		13376
#define GREATER_FIRE_ELEMENTAL_FIRE_NOVA		12470
#define GREATER_FIRE_ELEMENTAL_FIRE_BLAST		13339
#define GREATER_FIRE_ELEMENTAL_MARK_OF_DEATH	37128	// SSS	// should be applied on target

class GreaterFireElementalAI : public ArcScriptCreatureAI
{
    ARCSCRIPT_FACTORY_FUNCTION(GreaterFireElementalAI, ArcScriptCreatureAI);
	GreaterFireElementalAI(Creature* pCreature) : ArcScriptCreatureAI(pCreature)
	{
		AddSpell(GREATER_FIRE_ELEMENTAL_FIRE_NOVA, Target_Self, 9, 2000, 0);
		AddSpell(GREATER_FIRE_ELEMENTAL_FIRE_BLAST, Target_Current, 8, 0, 0, 0, 20, true);
		AddSpell(GREATER_FIRE_ELEMENTAL_MARK_OF_DEATH, Target_RandomPlayerApplyAura, 5, 0, 0, 0, 80, true);
		ApplyAura(GREATER_FIRE_ELEMENTAL_FIRE_SHIELD);
	}
};

// IronjawAI					-- NEEDS TO BE RECHECKED BEFORE REWRITE --
// Comment says all ---^
#define CN_IRONJAW 18670
// I couldn't force Ironjaw to cast spells with "?"
#define FURIOUS_HOWL 24597	// ?
#define DASH 23110
#define BITE 27050
#define GROWL 14921			// ?
#define FEROCIOUS_INSPIRATION 34456	// ?
#define FRENZY_EFFECT 19615
#define KILL_COMMAND 34027
#define BESTIAL_WRATH 24395	// DBC: 24395, 24396, 24397, 26592 (all are diffrent)	// ?
#define SNARL 32919
#define CHILLING_HOWL 32918

class IronjawAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(IronjawAI);
	SP_AI_Spell spells[10];
	bool m_spellcheck[10];

    IronjawAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 10;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
        spells[0].info = dbcSpell.LookupEntry(FURIOUS_HOWL);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 3.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(DASH);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].cooldown = -1;
		spells[1].perctrigger = 3.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(BITE);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].cooldown = -1;
		spells[2].perctrigger = 8.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(GROWL);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = true;
		spells[3].cooldown = -1;
		spells[3].perctrigger = 5.0f;
		spells[3].attackstoptimer = 1000;

		spells[4].info = dbcSpell.LookupEntry(FEROCIOUS_INSPIRATION);
		spells[4].targettype = TARGET_SELF;
		spells[4].instant = true;
		spells[4].cooldown = -1;
		spells[4].perctrigger = 3.0f;
		spells[4].attackstoptimer = 1000;

		spells[5].info = dbcSpell.LookupEntry(FRENZY_EFFECT);
		spells[5].targettype = TARGET_SELF;
		spells[5].instant = true;
		spells[5].cooldown = -1;
		spells[5].perctrigger = 4.0f;
		spells[5].attackstoptimer = 1000;

		spells[6].info = dbcSpell.LookupEntry(KILL_COMMAND);
		spells[6].targettype = TARGET_ATTACKING;
		spells[6].instant = true;
		spells[6].cooldown = -1;
		spells[6].perctrigger = 6.0f;
		spells[6].attackstoptimer = 1000;

		spells[7].info = dbcSpell.LookupEntry(BESTIAL_WRATH);
		spells[7].targettype = TARGET_ATTACKING;
		spells[7].instant = true;
		spells[7].cooldown = -1;
		spells[7].perctrigger = 4.0f;
		spells[7].attackstoptimer = 1000;

		spells[8].info = dbcSpell.LookupEntry(SNARL);
		spells[8].targettype = TARGET_ATTACKING;
		spells[8].instant = true;
		spells[8].cooldown = -1;
		spells[8].perctrigger = 8.0f;
		spells[8].attackstoptimer = 1000;

		spells[9].info = dbcSpell.LookupEntry(CHILLING_HOWL);
		spells[9].targettype = TARGET_ATTACKING;
		spells[9].instant = true;
		spells[9].cooldown = -1;
		spells[9].perctrigger = 5.0f;
		spells[9].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
    }

	void SpellCast(float val)
	{
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
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

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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
};

// Millhouse ManastormAI			-- SHOULD BE RECHECKED BEFORE REWRITE --
// Probably should be rewritten and is part of Skyriss encounter 
#define CN_MILLHOUSE_MANASTORM 20977

#define SIMPLE_TELEPORT_MM 12980
#define CONJURE_WATER 36879	// SSS =/
#define ARCANE_INTELLECT 36880
#define ICE_ARMOR 36881
#define DRINK 30024
#define FIREBALL_MM 14034
#define FROSTBOLT_MM 15497
#define FIRE_BLAST_MM 13341
#define CONE_OF_COLD 12611
#define ARCANE_MISSILES 33832	// DBC: 33832, 33833
#define PYROBLAST 33975
//#define CHILLED 7321 effect of ICE_ARMOR 36881

class MillhouseManastormAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(MillhouseManastormAI);
	SP_AI_Spell spells[11];
	bool m_spellcheck[11];

    MillhouseManastormAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 11;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
        spells[0].info = dbcSpell.LookupEntry(SIMPLE_TELEPORT_MM);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(CONJURE_WATER);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = false;
		spells[1].cooldown = -1;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(ARCANE_INTELLECT);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;
		spells[2].cooldown = -1;
		spells[2].perctrigger = 3.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(ICE_ARMOR);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = true;
		spells[3].cooldown = -1;
		spells[3].perctrigger = 3.0f;
		spells[3].attackstoptimer = 1000;

		spells[4].info = dbcSpell.LookupEntry(DRINK);
		spells[4].targettype = TARGET_SELF;
		spells[4].instant = true;
		spells[4].cooldown = -1;
		spells[4].perctrigger = 0.0f;
		spells[4].attackstoptimer = 1000;

		spells[5].info = dbcSpell.LookupEntry(FIREBALL_MM);
		spells[5].targettype = TARGET_ATTACKING;
		spells[5].instant = false;
		spells[5].cooldown = -1;
		spells[5].perctrigger = 5.0f;
		spells[5].attackstoptimer = 1000;

		spells[6].info = dbcSpell.LookupEntry(FROSTBOLT_MM);
		spells[6].targettype = TARGET_ATTACKING;
		spells[6].instant = false;
		spells[6].cooldown = -1;
		spells[6].perctrigger = 5.0f;
		spells[6].attackstoptimer = 1000;

		spells[7].info = dbcSpell.LookupEntry(FIRE_BLAST_MM);
		spells[7].targettype = TARGET_ATTACKING;
		spells[7].instant = true;
		spells[7].cooldown = -1;
		spells[7].perctrigger = 5.0f;
		spells[7].attackstoptimer = 1000;

		spells[8].info = dbcSpell.LookupEntry(CONE_OF_COLD);
		spells[8].targettype = TARGET_VARIOUS;
		spells[8].instant = true;
		spells[8].cooldown = -1;
		spells[8].perctrigger = 4.0f;
		spells[8].attackstoptimer = 1000;

		spells[9].info = dbcSpell.LookupEntry(ARCANE_MISSILES);
		spells[9].targettype = TARGET_ATTACKING;
		spells[9].instant = false;
		spells[9].cooldown = -1;
		spells[9].perctrigger = 5.0f;
		spells[9].attackstoptimer = 1000;

		spells[10].info = dbcSpell.LookupEntry(PYROBLAST);
		spells[10].targettype = TARGET_ATTACKING;
		spells[10].instant = false;
		spells[10].cooldown = -1;
		spells[10].perctrigger = 2.0f;
		spells[10].attackstoptimer = 1000;

		_unit->CastSpell(_unit, spells[0].info, spells[0].instant);

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit *mTarget)
    {/*
		CastTime();
		if (_unit->GetUInt32Value(UNIT_FIELD_POWER1) < _unit->GetUInt32Value(UNIT_FIELD_BASE_MANA))
		{
			_unit->CastSpell(_unit, spells[1].info, spells[1].instant);
			_unit->CastSpell(_unit, spells[4].info, spells[4].instant);
		}*/
		_unit->CastSpell(_unit, spells[1].info, spells[1].instant);
		_unit->CastSpell(_unit, spells[4].info, spells[4].instant);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
    }

	void SpellCast(float val)
	{
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
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

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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
};

// Negaton ScreamerAI					-- NEEDS TO BE RECHECKED BEFORE REWRITE --	// this one needs some more work for dmg taken dmg type, but I don't have time for it...
// Add missing function ------------------------------ look below
#define CN_NEGATON_SCREAMER 20875		// I need to add missing about 15 mobs (w00t? More? =() and to this script
										// OnDamageTaken function with check of spell dmg type and correct spell casts in diff cases
#define PSYCHIC_SCREAM 13704
#define DAMAGE_REDUCTION_SHADOW 34338
#define DAMAGE_REDUCTION_FROST 34334
#define DAMAGE_REDUCTION_ARCANE 34331
#define DAMAGE_REDUCTION_FIRE 34333
#define DAMAGE_REDUCTION_HOLY 34336
#define DAMAGE_REDUCTION_NATURE 34335
//#define DAMAGE_REDUCTION_PHYSICAL 34337
#define SHADOW_ENERGY 34399 // Energy SSS-s (I am sure only to spells shadow-frost-arcane-fire)
#define FROST_ENERGY 34404
#define ARCANE_ENERGY 34401
#define FIRE_ENERGY 34398
#define HOLY_ENERGY 34403
#define NATURE_ENERGY 34402
//#define PHYSICAL_ENERGY 
#define SHADOW_BOLT_VOLLEY 36736
#define FROSTBOLT_VOLLEY 36741
#define ARCANE_VOLLEY 36738
#define FIREBALL_VOLLEY 36742
#define HOLY_VOLLEY 36743
#define LIGHTNING_BOLT_VOLLEY	36740
//#define _VOLLEY


class NegatonScreamerAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(NegatonScreamerAI);
	SP_AI_Spell spells[19];
	bool m_spellcheck[19];

    NegatonScreamerAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 19;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
        spells[0].info = dbcSpell.LookupEntry(PSYCHIC_SCREAM);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(DAMAGE_REDUCTION_SHADOW);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].cooldown = -1;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(DAMAGE_REDUCTION_FROST);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;
		spells[2].cooldown = -1;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(DAMAGE_REDUCTION_ARCANE);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = true;
		spells[3].cooldown = -1;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;

		spells[4].info = dbcSpell.LookupEntry(DAMAGE_REDUCTION_FIRE);
		spells[4].targettype = TARGET_SELF;
		spells[4].instant = true;
		spells[4].cooldown = -1;
		spells[4].perctrigger = 0.0f;
		spells[4].attackstoptimer = 1000;

		spells[5].info = dbcSpell.LookupEntry(DAMAGE_REDUCTION_HOLY);
		spells[5].targettype = TARGET_SELF;
		spells[5].instant = true;
		spells[5].cooldown = -1;
		spells[5].perctrigger = 0.0f;
		spells[5].attackstoptimer = 1000;

		spells[6].info = dbcSpell.LookupEntry(DAMAGE_REDUCTION_NATURE);
		spells[6].targettype = TARGET_SELF;
		spells[6].instant = true;
		spells[6].cooldown = -1;
		spells[6].perctrigger = 0.0f;
		spells[6].attackstoptimer = 1000;

		spells[7].info = dbcSpell.LookupEntry(SHADOW_ENERGY);
		spells[7].targettype = TARGET_SELF;
		spells[7].instant = true;
		spells[7].cooldown = -1;
		spells[7].perctrigger = 0.0f;
		spells[7].attackstoptimer = 1000;

		spells[8].info = dbcSpell.LookupEntry(FROST_ENERGY);
		spells[8].targettype = TARGET_SELF;
		spells[8].instant = true;
		spells[8].cooldown = -1;
		spells[8].perctrigger = 0.0f;
		spells[8].attackstoptimer = 1000;

		spells[9].info = dbcSpell.LookupEntry(ARCANE_ENERGY);
		spells[9].targettype = TARGET_SELF;
		spells[9].instant = true;
		spells[9].cooldown = -1;
		spells[9].perctrigger = 0.0f;
		spells[9].attackstoptimer = 1000;

		spells[10].info = dbcSpell.LookupEntry(FIRE_ENERGY);
		spells[10].targettype = TARGET_SELF;
		spells[10].instant = true;
		spells[10].cooldown = -1;
		spells[10].perctrigger = 0.0f;
		spells[10].attackstoptimer = 1000;

		spells[11].info = dbcSpell.LookupEntry(HOLY_ENERGY);
		spells[11].targettype = TARGET_SELF;
		spells[11].instant = true;
		spells[11].cooldown = -1;
		spells[11].perctrigger = 0.0f;
		spells[11].attackstoptimer = 1000;

		spells[12].info = dbcSpell.LookupEntry(NATURE_ENERGY);
		spells[12].targettype = TARGET_SELF;
		spells[12].instant = true;
		spells[12].cooldown = -1;
		spells[12].perctrigger = 0.0f;
		spells[12].attackstoptimer = 1000;

		spells[13].info = dbcSpell.LookupEntry(SHADOW_BOLT_VOLLEY);
		spells[13].targettype = TARGET_VARIOUS;
		spells[13].instant = false;
		spells[13].cooldown = -1;
		spells[13].perctrigger = 0.0f;
		spells[13].attackstoptimer = 1000;

		spells[14].info = dbcSpell.LookupEntry(FROSTBOLT_VOLLEY);
		spells[14].targettype = TARGET_VARIOUS;
		spells[14].instant = false;
		spells[14].cooldown = -1;
		spells[14].perctrigger = 0.0f;
		spells[14].attackstoptimer = 1000;

		spells[15].info = dbcSpell.LookupEntry(ARCANE_VOLLEY);
		spells[15].targettype = TARGET_VARIOUS;
		spells[15].instant = false;
		spells[15].cooldown = -1;
		spells[15].perctrigger = 0.0f;
		spells[15].attackstoptimer = 1000;

		spells[16].info = dbcSpell.LookupEntry(FIREBALL_VOLLEY);
		spells[16].targettype = TARGET_VARIOUS;
		spells[16].instant = false;
		spells[16].cooldown = -1;
		spells[16].perctrigger = 0.0f;
		spells[16].attackstoptimer = 1000;

		spells[17].info = dbcSpell.LookupEntry(HOLY_VOLLEY);
		spells[17].targettype = TARGET_VARIOUS;
		spells[17].instant = false;
		spells[17].cooldown = -1;
		spells[17].perctrigger = 0.0f;
		spells[17].attackstoptimer = 1000;

		spells[18].info = dbcSpell.LookupEntry(LIGHTNING_BOLT_VOLLEY);
		spells[18].targettype = TARGET_VARIOUS;
		spells[18].instant = false;
		spells[18].cooldown = -1;
		spells[18].perctrigger = 0.0f;
		spells[18].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
    }

	void SpellCast(float val)
	{
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
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

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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
};

// Negaton Warp-MasterAI

#define CN_NEGATON_WARP_MASTER						20873
#define NEGATON_WARP_MASTER_SUMMON_NEGATION_FIELD	36813

class NegatonWarpMasterAI : public ArcScriptCreatureAI
{
    ARCSCRIPT_FACTORY_FUNCTION(NegatonWarpMasterAI, ArcScriptCreatureAI);
	NegatonWarpMasterAI(Creature* pCreature) : ArcScriptCreatureAI(pCreature)
	{
		AddSpell(NEGATON_WARP_MASTER_SUMMON_NEGATION_FIELD, Target_Self, 10, 20000, 0);
	}
};

// Negaton FieldAI						-- HUH? TO RECHECK? --
// Not sure if shouldn't be rewritten.
#define CN_NEGATON_FIELD	21414
#define NEGATION_FIELD		36728 // DBC: 36728, 36729;

class NegatonFieldAI : public ArcScriptCreatureAI
{
    ARCSCRIPT_FACTORY_FUNCTION(NegatonFieldAI, ArcScriptCreatureAI);
	NegatonFieldAI(Creature* pCreature) : ArcScriptCreatureAI(pCreature)
	{
		ApplyAura(NEGATION_FIELD);
	}
};

// Phase-HunterAI
// This unit is part of Skyriss encounter and it's teleport skill should be used during that encounter
#define CN_PHASE_HUNTER					20906
#define PHASE_HUNTER_SIMPLE_TELEPORT	12980
#define PHASE_HUNTER_BACK_ATTACK		36909
#define PHASE_HUNTER_WARP				36908	// lack of core support?
#define PHASE_HUNTER_PHASE_BURST		36910

class PhaseHunterAI : public ArcScriptCreatureAI
{
    ARCSCRIPT_FACTORY_FUNCTION(PhaseHunterAI, ArcScriptCreatureAI);
	PhaseHunterAI(Creature* pCreature) : ArcScriptCreatureAI(pCreature)
	{
		AddSpell(PHASE_HUNTER_WARP, Target_Self, 5, 0, 0);
		AddSpell(PHASE_HUNTER_PHASE_BURST, Target_Self, 5, 0, 0);
		ApplyAura(NEGATION_FIELD);
	}

	void AIUpdate()
	{
		if (_unit->GetAIInterface()->GetNextTarget() != NULL && _unit->GetCurrentSpell() == NULL)
		{
			Unit *pUnit = _unit->GetAIInterface()->GetNextTarget();
			if (_unit->isInBack(pUnit) && _unit->GetDistance2dSq(pUnit) <= 64.0f && RandomUInt(2) == 0)
			{
				this->CastSpellOnTarget(pUnit, Target_Current, dbcSpell.LookupEntry(PHASE_HUNTER_BACK_ATTACK), true);
			}
		}
		ParentClass::AIUpdate();
	}
};

// Protean HorrorAI

#define CN_PROTEAN_HORROR			20865
#define PROTEAN_HORROR_TOOTHY_BITE	36612
//#define BLOODY_EXPLOSION 36588

class ProteanHorrorAI : public ArcScriptCreatureAI
{
    ARCSCRIPT_FACTORY_FUNCTION(ProteanHorrorAI, ArcScriptCreatureAI);
	ProteanHorrorAI(Creature* pCreature) : ArcScriptCreatureAI(pCreature)
	{
		AddSpell(PROTEAN_HORROR_TOOTHY_BITE, Target_Current, 10, 0, 0, 0, 8, true);
	}
};

// Protean NightmareAI

#define CN_PROTEAN_NIGHTMARE				20864
#define PROTEAN_NIGHTMARE_INCUBATION		36622
#define PROTEAN_NIGHTMARE_INFECTIOUS_POISON	36619
#define PROTEAN_NIGHTMARE_GAPIND_MAW		36617

class ProteanNightmareAI : public ArcScriptCreatureAI
{
    ARCSCRIPT_FACTORY_FUNCTION(ProteanNightmareAI, ArcScriptCreatureAI);
	ProteanNightmareAI(Creature* pCreature) : ArcScriptCreatureAI(pCreature)
	{
		AddSpell(PROTEAN_NIGHTMARE_INCUBATION, Target_Current, 6, 0, 0, 0, 8, true);
		AddSpell(PROTEAN_NIGHTMARE_INFECTIOUS_POISON, Target_Self, 10, 0, 0);
		AddSpell(PROTEAN_NIGHTMARE_GAPIND_MAW, Target_Self, 6, 600, 0);
	}
};

// Sulfuron Magma-ThrowerAI
// Part of Skyriss encounter (so same thing to teleport stuff)
#define CN_SULFURON_MAGMA_THROWER 20909

#define SIMPLE_TELEPORT_SMT 12980
#define SHADOW_BOLT_SMT 36986	// DBC: 36987, 36986;
#define MAGMA_THROWERS_CURSE 36917
#define RAIN_OF_FIRE 19717

class SulfuronMagmaThrowerAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SulfuronMagmaThrowerAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    SulfuronMagmaThrowerAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(SIMPLE_TELEPORT_SMT);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(SHADOW_BOLT_SMT);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].cooldown = -1;
		spells[1].perctrigger = 12.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(MAGMA_THROWERS_CURSE);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].cooldown = -1;
		spells[2].perctrigger = 6.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(RAIN_OF_FIRE);
		spells[3].targettype = TARGET_DESTINATION;
		spells[3].instant = true;
		spells[3].cooldown = -1;
		spells[3].perctrigger = 9.0f;
		spells[3].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
    }

	void SpellCast(float val)
	{
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
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

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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
};

// Spiteful TemptressAI

#define CN_SPITEFUL_TEMPTRESS 20883

#define SHADOW_BOLT_ST 36868
#define DOMINATION_ST 36866
#define SPITEFUL_FURY 36886
#define PRAYER_OF_MENDING 33280	// DBC: 33280, 33110; SSS

class SpitefulTemptressAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SpitefulTemptressAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    SpitefulTemptressAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(SHADOW_BOLT_ST);
		spells[0].targettype = TARGET_DESTINATION;	// not sure DESTINATION or ATTACKING
		spells[0].instant = false;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 12.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(DOMINATION_ST);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].cooldown = -1;
		spells[1].perctrigger = 5.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(SPITEFUL_FURY);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].cooldown = -1;
		spells[2].perctrigger = 7.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(PRAYER_OF_MENDING);
		spells[3].targettype = TARGET_VARIOUS;
		spells[3].instant = true;
		spells[3].cooldown = -1;
		spells[3].perctrigger = 8.0f;
		spells[3].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
    }

	void SpellCast(float val)
	{
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
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

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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
};

// Soul DevourerAI

#define CN_SOUL_DEVOURER 20866

#define SIGHTLESS_EYE 36644
#define LAVA_BREATH 21333	// doesn't have animation?
#define FEL_BREATH 36654
#define FRENZY 33958

class SoulDevourerAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SoulDevourerAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    SoulDevourerAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(SIGHTLESS_EYE);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = false;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 7.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(LAVA_BREATH);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].cooldown = -1;
		spells[1].perctrigger = 12.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(FEL_BREATH);
		spells[2].targettype = TARGET_VARIOUS;
		spells[2].instant = true;
		spells[2].cooldown = -1;
		spells[2].perctrigger = 8.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(FRENZY);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = true;
		spells[3].cooldown = -1;
		spells[3].perctrigger = 8.0f;
		spells[3].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		_unit->CastSpell(_unit, spells[0].info, spells[0].instant);
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
    }

	void SpellCast(float val)
	{
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
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

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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
};

// SolomonAI

#define CN_SOLOMON 798	// not sure if this boy should be scripted, but he was in DBC and had spells, so I couldn't
						// stop myself to not give little boy deadly skills muhahah =)

#define SLIME_SPRAY 36486
#define SAW_BLADE 32735
#define CRUSADER_STRIKE 35509	// SE (script effect)

class SolomonAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SolomonAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    SolomonAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(SLIME_SPRAY);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(SAW_BLADE);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].cooldown = -1;
		spells[1].perctrigger = 8.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(CRUSADER_STRIKE);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].cooldown = -1;
		spells[2].perctrigger = 12.0f;
		spells[2].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
    }

	void SpellCast(float val)
	{
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
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

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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
};

// Skulking WitchAI

#define CN_SKULKING_WITCH 20882	// works really cool as assassin :)

#define CHASTISE 36863
#define GOUGE 36862
#define LASH_OF_PAIN 36864
#define GREAT_INVISIBILITY 16380

class SkulkingWitchAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SkulkingWitchAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    SkulkingWitchAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(CHASTISE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 12.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(GOUGE);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].cooldown = -1;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(LASH_OF_PAIN);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].cooldown = -1;
		spells[2].perctrigger = 5.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(GREAT_INVISIBILITY);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = true;
		spells[3].cooldown = -1;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;

		_unit->CastSpell(_unit, spells[3].info, spells[3].instant);
		FIRST_ATTACK = 1;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		FIRST_ATTACK = 1;
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnTargetDied(Unit* mTarget)
    {
    }

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
		FIRST_ATTACK = 1;
		_unit->CastSpell(_unit, spells[3].info, spells[3].instant);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		FIRST_ATTACK = 1;
		CastTime();
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		if (FIRST_ATTACK && _unit->GetAIInterface()->GetNextTarget())
		{
			FIRST_ATTACK = 0;
			Unit * target = NULL;
			target = _unit->GetAIInterface()->GetNextTarget();
			_unit->CastSpell(target, spells[1].info, spells[1].instant);
		}

		else
		{
			float val = (float)RandomFloat(100.0f);
			SpellCast(val);
		}
    }

	void SpellCast(float val)
	{
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
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

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

protected:

	uint32 FIRST_ATTACK;
	int nrspells;
};

// Sightless EyeAI

#define CN_SIGHTLESS_EYE 21346	

#define SIGHTLESS_TOUCH 36646

class SightlessEyeAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SightlessEyeAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    SightlessEyeAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
        spells[0].info = dbcSpell.LookupEntry(SIGHTLESS_TOUCH);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
    }

	void SpellCast(float val)
	{
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
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

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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
};

// Sargeron HellcallerAI

#define CN_SARGERON_HELLCALLER 20902	

#define HELL_RAIN 36829
#define CURSE_OF_THE_ELEMENTS 36831
#define INCINERATE 36832

class SargeronHellcallerAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SargeronHellcallerAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    SargeronHellcallerAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
        spells[0].info = dbcSpell.LookupEntry(HELL_RAIN);
		spells[0].targettype = TARGET_DESTINATION;
		spells[0].instant = true;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(CURSE_OF_THE_ELEMENTS);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].cooldown = -1;
		spells[1].perctrigger = 6.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(INCINERATE);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = false;
		spells[2].cooldown = -1;
		spells[2].perctrigger = 11.0f;
		spells[2].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
    }

	void SpellCast(float val)
	{
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
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

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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
};

// Unbound DevastatorAI

#define CN_UNBOUND_DEVASTATOR 20881

#define DEAFENING_ROAR 36887
#define DEVASTATE 36894	// DBC: 36891, 36894;

class UnboundDevastatorAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(UnboundDevastatorAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    UnboundDevastatorAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
        spells[0].info = dbcSpell.LookupEntry(DEAFENING_ROAR);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 7.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(DEVASTATE);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].cooldown = -1;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
    }

	void SpellCast(float val)
	{
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
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

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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
};

// Unchained DoombringerAI
// TO DO: Look at charge note
#define CN_UNCHAINED_DOOMBRINGER 20900	

#define AGONIZING_ARMOR 36836
#define WAR_STOMP 36835
#define BERSEKER_CHARGE 36833	// should charge the furthest enemy

class UnchainedDoombringerAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(UnchainedDoombringerAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    UnchainedDoombringerAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
        spells[0].info = dbcSpell.LookupEntry(AGONIZING_ARMOR);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 9.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(WAR_STOMP);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].cooldown = -1;
		spells[1].perctrigger = 12.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(BERSEKER_CHARGE);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].cooldown = -1;
		spells[2].perctrigger = 6.0f;
		spells[2].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
    }

	void SpellCast(float val)
	{
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
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

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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
};

// Warder CorpseAI

#define CN_WARDER_CORPSE 21303

#define CORPSE_BURST_WC 36593
#define BLOODY_EXPLOSION_WC 36599
//#define PERMANENT_FEIGN_DEATH_ROOT_WC 31261	// I don't have much infos and I am not sure about how it should work
												// I presume this is just corpse which should cast spell when player is close or attk it
												// so same situation like in Defender Corpse
class WarderCorpseAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(WarderCorpseAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    WarderCorpseAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
        spells[0].info = dbcSpell.LookupEntry(CORPSE_BURST_WC);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(BLOODY_EXPLOSION_WC);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].cooldown = -1;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		_unit->CastSpell(_unit, spells[1].info, spells[1].instant);
		_unit->CastSpell(_unit, spells[0].info, spells[0].instant);
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
    }

	void SpellCast(float val)
	{
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
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

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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
};

// Sargeron ArcherAI
// Probably to rewrite
#define CN_SARGERON_ARCHER 20901

#define FROST_ARROW	35964 //DBC: 35965, 35964;
#define SHOT_SA 22907
#define RAPID_FIRE 36828
#define SCATTER_SHOT 23601
#define HOOKED_NET 36827
#define IMMOLATION_ARROW 35932	// DBC: 35932, 35935

class SargeronArcherAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SargeronArcherAI);
	SP_AI_Spell spells[6];
	bool m_spellcheck[6];

    SargeronArcherAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 6;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
        spells[0].info = dbcSpell.LookupEntry(FROST_ARROW);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = dbcSpell.LookupEntry(SHOT_SA);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].cooldown = -1;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(RAPID_FIRE);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;
		spells[2].cooldown = -1;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(SCATTER_SHOT);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = true;
		spells[3].cooldown = -1;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;

		spells[4].info = dbcSpell.LookupEntry(HOOKED_NET);
		spells[4].targettype = TARGET_VARIOUS;
		spells[4].instant = false;
		spells[4].cooldown = -1;
		spells[4].perctrigger = 6.0f;
		spells[4].attackstoptimer = 1000;

		spells[5].info = dbcSpell.LookupEntry(IMMOLATION_ARROW);
		spells[5].targettype = TARGET_ATTACKING;
		spells[5].instant = false;
		spells[5].cooldown = -1;
		spells[5].perctrigger = 0.0f;
		spells[5].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		if (_unit->GetAIInterface()->GetNextTarget())
		{
			Unit *target = NULL;
			target = _unit->GetAIInterface()->GetNextTarget();
			if (_unit->GetDistance2dSq(target) >= 100.0f && _unit->GetDistanceSq(target) <= 900.0f)
			{
				_unit->GetAIInterface()->m_canMove = false;
				int RangedSpell;
				RandomUInt(1000);
				RangedSpell=rand()%100;
				if (RangedSpell >= 0 && RangedSpell <= 13)
				{
					_unit->CastSpell(target, spells[0].info, spells[0].instant);
					_unit->setAttackTimer(spells[0].attackstoptimer, false);
				}

				if (RangedSpell > 12 && RangedSpell <= 25)
				{
					_unit->CastSpell(target, spells[3].info, spells[3].instant);
					_unit->setAttackTimer(spells[3].attackstoptimer, false);
				}

				if (RangedSpell > 25 && RangedSpell <= 38)
				{
					_unit->CastSpell(target, spells[5].info, spells[5].instant);
					_unit->setAttackTimer(spells[5].attackstoptimer, false);
				}

				if (RangedSpell > 38 && RangedSpell <= 44)
				{
					_unit->CastSpell(target, spells[2].info, spells[2].instant);
					_unit->setAttackTimer(spells[2].attackstoptimer, false);
				}

				if (RangedSpell > 44 && RangedSpell <= 50)
				{
					_unit->CastSpell(target, spells[4].info, spells[4].instant);
					_unit->setAttackTimer(spells[4].attackstoptimer, false);
				}

				else
				{
					_unit->CastSpell(target, spells[1].info, spells[1].instant);
					_unit->setAttackTimer(spells[1].attackstoptimer, false);
				}
			}

			else
			{
				_unit->GetAIInterface()->m_canMove = true;
				if (_unit->GetDistance2dSq(target) < 100.0f)
				{
					float val = (float)RandomFloat(100.0f);
					SpellCast(val);
				}
			}
		}
    }

	void SpellCast(float val)
	{
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())	// I was too lazy to rewrite it =S
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
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

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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
};

/*****************************/
/*                           */
/*         Boss AIs          */
/*                           */
/*****************************/

// Zereketh the UnboundAI
#define CN_ZEREKETH			20870
#define CN_VOIDZONEARC		21101

#define SEED_OF_C			36123	//32865, 36123
#define SHADOW_NOVA			36127 // 30533, 39005, 36127 (normal mode), 39005 (heroic mode?)
#define SHADOW_NOVA_H		39005
#define CONSUMPTION			30498
#define CONSUMPTION_H		39004
// #define VOID_ZONE 36119	// DBC: 36119; it's not fully functionl without additional core support (for dmg and random place targeting).

class ZerekethAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(ZerekethAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    ZerekethAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {

		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
			
		spells[0].info = dbcSpell.LookupEntry(SEED_OF_C);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = false;
		spells[0].cooldown = 20;
		spells[0].perctrigger = 6.0f;
		spells[0].attackstoptimer = 2000;
		spells[0].mindist2cast = 0.0f;
		spells[0].maxdist2cast = 100.0f;

		if(_unit->GetMapMgr()->iInstanceMode != MODE_HEROIC)
		{

			spells[1].info = dbcSpell.LookupEntry(SHADOW_NOVA);
			spells[1].targettype = TARGET_VARIOUS;
			spells[1].instant = false;
			spells[1].cooldown = 15;
			spells[1].perctrigger = 15.0f;
			spells[1].attackstoptimer = 1500;
		}
		else
		{
			spells[1].info = dbcSpell.LookupEntry(SHADOW_NOVA_H);
			spells[1].targettype = TARGET_VARIOUS;
			spells[1].instant = false;
			spells[1].cooldown = 15;
			spells[1].perctrigger = 15.0f;
			spells[1].attackstoptimer = 1500;
		}
	}
    void OnCombatStart(Unit* mTarget)
    {
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;

		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Life energy to... consume.");
		_unit->PlaySoundToSet(11250);

		uint32 t = (uint32)time(NULL);
		VoidTimer = t + RandomUInt(10)+30;
		SpeechTimer = t + RandomUInt(10)+40;
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
	}

    void OnDied(Unit * mKiller)
    {
		//despawn voids
		Creature *creature = NULL;
		for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr)
		{
			if((*itr)->GetTypeId() == TYPEID_UNIT)
			{
				creature = static_cast<Creature*>((*itr));

				if(creature && creature->GetCreatureInfo() && creature->GetCreatureInfo()->Id == 21101 && creature->isAlive())
				{
					creature->Despawn(0, 0);
					//creature->SafeDelete();
				}
			}
		}

		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The Void... beckons.");
		_unit->PlaySoundToSet(11255);

		RemoveAIUpdateEvent();
    }

	void OnTargetDied(Unit* mTarget)
    {
		if (_unit->GetHealthPct() > 0)	// Hack to prevent double yelling (OnDied and OnTargetDied when creature is dying)
		{
			int RandomSpeach;
			RandomUInt(1000);
			RandomSpeach=rand()%2;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "This vessel...is empty.");
				_unit->PlaySoundToSet(11251);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "No... more... life.");	// not sure
				_unit->PlaySoundToSet(11252);
				break;
			}
		}
    }
	
	void Speech()
	{
		switch (RandomUInt(1))
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The shadow... will engulf you.");
			_unit->PlaySoundToSet(11253);
			break;
		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Darkness... consumes all.");
			_unit->PlaySoundToSet(11254);
			break;
		}
		SpeechTimer = (uint32)time(NULL) + RandomUInt(10)+40;
	}

	void VoidZoneArc()
	{
		VoidTimer = (uint32)time(NULL) + RandomUInt(10)+30;
		
		CreatureInfo * ci = CreatureNameStorage.LookupEntry(CN_VOIDZONEARC);
		CreatureProto * cp = CreatureProtoStorage.LookupEntry(CN_VOIDZONEARC);
		if(!cp || !ci)
			return;

		std::vector<Player*> TargetTable;
		std::set<Player*>::iterator Itr = _unit->GetInRangePlayerSetBegin();
		for(; Itr != _unit->GetInRangePlayerSetEnd(); Itr++) 
		{ 
			Player* RandomTarget = NULL;
			RandomTarget = static_cast< Player* >(*Itr);
			if(RandomTarget && RandomTarget->isAlive() && isHostile(*Itr, _unit))
				TargetTable.push_back(RandomTarget);
			RandomTarget = NULL;
		}

		if (!TargetTable.size())
			return;

		size_t RandTarget = rand()%TargetTable.size();

		Player * RTarget = TargetTable[RandTarget];

		if (!RTarget)
			return;

		float vzX = RandomUInt(5) * cos(RandomFloat(6.28f))+RTarget->GetPositionX();
		float vzY = RandomUInt(5) * cos(RandomFloat(6.28f))+RTarget->GetPositionY();
		float vzZ = RTarget->GetPositionZ();
		Creature *VoidZone = _unit->GetMapMgr()->CreateCreature(cp->Id);
		VoidZone->Load(cp, vzX, vzY, vzZ);
		VoidZone->SetInstanceID(_unit->GetInstanceID());
		VoidZone->SetZoneId(_unit->GetZoneId());
		VoidZone->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
		VoidZone->m_noRespawn = true;
		if(VoidZone->CanAddToWorld())
		{
			VoidZone->PushToWorld(_unit->GetMapMgr());
		}
		else
		{
			VoidZone->SafeDelete();
			return;
		}
		RTarget = NULL;
		sEventMgr.AddEvent(VoidZone, &Creature::SafeDelete, EVENT_CREATURE_SAFE_DELETE, 60000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}

    void AIUpdate()
	{
		uint32 t = (uint32)time(NULL);
		if(t > SpeechTimer)
			Speech();

		if(t > VoidTimer)
			VoidZoneArc();
		
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
    }

	void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
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
			target = NULL;
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<Player *> TargetTable;
			for(set<Player*>::iterator itr = _unit->GetInRangePlayerSetBegin(); itr != _unit->GetInRangePlayerSetEnd(); ++itr) 
			{ 
				Player* RandomTarget = NULL;
				RandomTarget = (Player*)(*itr);

				if (RandomTarget && RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast)
					TargetTable.push_back(RandomTarget);
				RandomTarget = NULL;
			}

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit * RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}
			RTarget = NULL;
			TargetTable.clear();
		}
	}

	void Destroy()
	{
		delete this;
	}

protected:

	uint32 SpeechTimer;
	uint32 VoidTimer;
	int nrspells;
};

class VoidZoneARC : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(VoidZoneARC);
	VoidZoneARC(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		_unit->Root();
		_unit->DisableAI();
		SpellEntry *Sp = NULL;
		if(_unit->GetMapMgr()->iInstanceMode != MODE_HEROIC)
			Sp = dbcSpell.LookupEntry(CONSUMPTION);
		else
			Sp = dbcSpell.LookupEntry(CONSUMPTION_H);

		sEventMgr.AddEvent(((Unit*)_unit), &Unit::EventCastSpell, ((Unit*)_unit), Sp, EVENT_UNK, (uint32)1000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
};


// Dalliah the DoomsayerAI

#define CN_DALLIAH_THE_DOOMSAYER 20885	

#define GIFT_OF_THE_DOOMSAYER 36173 // DBC: 36173
#define WHIRLWIND 36175	// DBC: 36142, 36175
#define HEAL 36144
#define SHADOW_WAVE 39016	// Heroic mode spell
// sounds missing related to Wrath... (look on script below this one)

class DalliahTheDoomsayerAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(DalliahTheDoomsayerAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    DalliahTheDoomsayerAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(GIFT_OF_THE_DOOMSAYER);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(WHIRLWIND);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = false;
		spells[1].cooldown = -1;
		spells[1].perctrigger = 15.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(HEAL);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = false;
		spells[2].cooldown = -1;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(SHADOW_WAVE);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = false;
		spells[3].cooldown = -1;
		spells[3].perctrigger = 8.0f;
		spells[3].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "It is unwise to anger me.");	// verification needed
		_unit->PlaySoundToSet(11086);
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
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
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Completely ineffective! Just like someone else I know!");	// need verif.
				_unit->PlaySoundToSet(11087);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You chose the wrong opponent!");	
				_unit->PlaySoundToSet(11088);
				break;
			}
		}
    }

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
		RemoveAIUpdateEvent();
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Now I'm really angry...");	// verification needed
		_unit->PlaySoundToSet(11093);
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
    }

	void HealSound()
	{
		int RandomSpeach;
		RandomSpeach=rand()%20;
		switch (RandomSpeach)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "That is much better.");
			_unit->PlaySoundToSet(11091);
			break;
		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Ah, just what I needed.");	// both need verif.
			_unit->PlaySoundToSet(11092);
			break;
		}
	}

	void WhirlwindSound()
	{
		int RandomWhirlwind;
		RandomWhirlwind=rand()%20;
		switch (RandomWhirlwind)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Reap the Whirlwind!");
			_unit->PlaySoundToSet(11089);
			break;
		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I'll cut you to pieces!");	
			_unit->PlaySoundToSet(11090);
			break;
		}
	}

	void SpellCast(float val)
	{
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{		
					if (m_spellcheck[2] == true)
						HealSound();

					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
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

					if (m_spellcheck[1] == true)
					{
						WhirlwindSound();
						int NextAttack;
						NextAttack=rand()%100+1;
						if (NextAttack <= 25 && NextAttack > 0)
						{
							m_spellcheck[2] = true;
						}
					}

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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
};

// Wrath-Scryer SoccothratesAI
// TO DO: Add moar sounds
#define CN_WRATH_SCRYER_SOCCOTHRATES 20886	

#define IMMOLATION 35959 // DBC: 36051, 35959
#define FELFIRE_SHOCK 35759
#define FELFIRE_LINE_UP 35770	// ?
#define KNOCK_AWAY 20686 // DBC: 36512; but it uses it on himself too so changed to other
#define CHARGE 35754 // DBC: 36058, 35754 =( =(
// CHARGE_TARGETING 36038 ?
// There are more sounds connected with Dalliah and some spells, but I don't know situation in which they are used
// so haven't added them.

class WrathScryerSoccothratesAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(WrathScryerSoccothratesAI);
	SP_AI_Spell spells[5];
	bool m_spellcheck[5];

    WrathScryerSoccothratesAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 5;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(IMMOLATION);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(FELFIRE_SHOCK);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].cooldown = -1;
		spells[1].perctrigger = 8.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(FELFIRE_LINE_UP);	// ?
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;
		spells[2].cooldown = -1;
		spells[2].perctrigger = 8.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(KNOCK_AWAY);
		spells[3].targettype = TARGET_DESTINATION;	// changed from VARIOUS to prevent crashes and gives it at least half working spell
		spells[3].instant = true;
		spells[3].cooldown = -1;
		spells[3].perctrigger = 6.0f;
		spells[3].attackstoptimer = 1000;

		spells[4].info = dbcSpell.LookupEntry(CHARGE);
		spells[4].targettype = TARGET_ATTACKING;
		spells[4].instant = true;
		spells[4].cooldown = -1;
		spells[4].perctrigger = 4.0f;
		spells[4].attackstoptimer = 1000;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "At last, a target for my frustrations!");	
		_unit->PlaySoundToSet(11238);
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
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
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Yes, that was quiet... satisfying.");	// need verif.
				_unit->PlaySoundToSet(11239);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Ha! Much better!");	// verification needed
				_unit->PlaySoundToSet(11240);
				break;
			}		
		}
    }

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
		RemoveAIUpdateEvent();
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Knew this was... the only way out.");	
		_unit->PlaySoundToSet(11243);
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
    }

	void SpellCast(float val)
	{
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
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

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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
};

// Harbinger SkyrissAI
// Full event must be scripted for this gay.
#define CN_HARBRINGER_SKYRISS 20912	

#define MIND_REND 36924 // DBC: 36859, 36924;
#define FEAR 39415
#define DOMINATION 37162
#define SUMMON_ILLUSION_66 36931	// those 2 don't work
#define SUMMON_ILLUSION_33 36932
// BLINK_VISUAL 36937 ?
// SIMPLE_TELEPORT 12980 ?
// Add sounds related to his dialog with mind controlled guy

class HarbringerSkyrissAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(HarbringerSkyrissAI);
	SP_AI_Spell spells[5];
	bool m_spellcheck[5];

    HarbringerSkyrissAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {

		IllusionCount = 0;
		nrspells = 5;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(MIND_REND);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 15.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(FEAR);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].cooldown = -1;
		spells[1].perctrigger = 8.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(DOMINATION);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = false;
		spells[2].cooldown = -1;
		spells[2].perctrigger = 6.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(SUMMON_ILLUSION_66);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = true;
		spells[3].cooldown = -1;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;

		spells[4].info = dbcSpell.LookupEntry(SUMMON_ILLUSION_33);
		spells[4].targettype = TARGET_SELF;
		spells[4].instant = true;
		spells[4].cooldown = -1;
		spells[4].perctrigger = 0.0f;
		spells[4].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		IllusionCount = 0;
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Bear witness to the agent of your demise!");	// used when he kills Warden Mellichar
		_unit->PlaySoundToSet(11123);
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
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

    void OnCombatStop(Unit *mTarget)
    {
		IllusionCount = 0;
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		IllusionCount = 0;
		CastTime();
       RemoveAIUpdateEvent();
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I am merely one of... infinite multitudes.");
		_unit->PlaySoundToSet(11126);
    }

    void AIUpdate()
	{
		if (_unit->GetHealthPct() <= 66 && !IllusionCount)
		{
			IllusionCount = 1;
			_unit->CastSpell(_unit, spells[3].info, spells[3].instant);
			//_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "We span the universe, as countless as the stars!");
			_unit->PlaySoundToSet(11131);	// Idk if those texts shouldn't be told by clones and by org. so disabled MSG to make it harder to detect =P
		}

		if (_unit->GetHealthPct() <= 33 && IllusionCount == 1)
		{
			IllusionCount = 2;
			_unit->CastSpell(_unit, spells[4].info, spells[4].instant);
			//_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "We span the universe, as countless as the stars!");
			_unit->PlaySoundToSet(11131);
		}
		
		else
		{
			float val = (float)RandomFloat(100.0f);
			SpellCast(val);
		}
    }

	void FearSound()
	{
		int RandomFear;
		RandomFear=rand()%4;
		switch (RandomFear)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Flee in terror.");
			_unit->PlaySoundToSet(11129);
			break;
		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I will show you horrors undreamed of.");
			_unit->PlaySoundToSet(11130);
			break;
		}
	}

	void DominationSound()
	{
		int RandomDomination;
		RandomDomination=rand()%4;
		switch (RandomDomination)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You will do my bidding, weakling.");
			_unit->PlaySoundToSet(11127);
			break;
		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your will is no longer your own.");
			_unit->PlaySoundToSet(11128);
			break;
		}
	}

	void SpellCast(float val)
	{
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
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

					if (m_spellcheck[1] == true)
					{
						FearSound();
					}

					if (m_spellcheck[2] == true)
					{
						DominationSound();
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

protected:

	uint32 IllusionCount;
	int nrspells;
};
/*
// Warden MellicharAI	// he is EVENT STARTER and needs some more time I don't have atm =(

#define CN_WARDEN_MELLICHAR 20904	

#define 

class WardenMellicharAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(WardenMellicharAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    WardenMellicharAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {

		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry();
		spells[0].targettype = TARGET_;
		spells[0].instant = false;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "");
		_unit->PlaySoundToSet();
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnTargetDied(Unit* mTarget)
    {
		int RandomSpeach;
		RandomSpeach=rand()%2;
		switch (RandomSpeach)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "");
			_unit->PlaySoundToSet();
			break;
		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "");
			_unit->PlaySoundToSet();
			break;
			}
		}
    }

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
       RemoveAIUpdateEvent();
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "");
		_unit->PlaySoundToSet();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
    }

	void SpellCast(float val)
	{
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
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

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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
};
*/
void SetupArcatraz(ScriptMgr * mgr)
{
	mgr->register_creature_script(CN_ARCATRAZ_DEFENDER, &ArcatrazDefenderAI::Create);
	mgr->register_creature_script(CN_ARCATRAZ_SENTINEL, &ArcatrazSentinelAI::Create);
	mgr->register_creature_script(CN_ARCATRAZ_WARDER, &ArcatrazWarderAI::Create);
	mgr->register_creature_script(CN_BLACKWING_DRAKONAAR, &BlackwingDrakonaarAI::Create);
	mgr->register_creature_script(CN_BLACKWING_TRICKSTER, &BlackwingTricksterAI::Create);
	mgr->register_creature_script(CN_DEATH_WATCHER, &DeathWatcherAI::Create);
	mgr->register_creature_script(CN_DEFENDER_CORPSE, &DefenderCorpseAI::Create);
	mgr->register_creature_script(CN_PROTEAN_SPAWN, &ProteanSpawnAI::Create);
	mgr->register_creature_script(CN_ENTROPIC_EYE, &EntropicEyeAI::Create);
	mgr->register_creature_script(CN_EREDAR_DEATHBRINGER, &EredarDeathbringerAI::Create);
	mgr->register_creature_script(CN_EREDAR_SOUL_EATER, &EredarSoulEaterAI::Create);
	mgr->register_creature_script(CN_ETHEREUM_LIFE_BINDER, &EthereumLifeBinderAI::Create);
	mgr->register_creature_script(CN_ETHEREUM_SLAYER, &EthereumSlayerAI::Create);
	mgr->register_creature_script(CN_ETHEREUM_WAVE_CASTER, &EthereumWaveCasterAI::Create);
	mgr->register_creature_script(CN_GARGANTUAN_ABYSSAL, &GargantuanAbyssalAI::Create);
	mgr->register_creature_script(CN_GREATER_FIRE_ELEMENTAL, &GreaterFireElementalAI::Create);
	mgr->register_creature_script(CN_IRONJAW, &IronjawAI::Create);
	mgr->register_creature_script(CN_MILLHOUSE_MANASTORM, &MillhouseManastormAI::Create);
	mgr->register_creature_script(CN_NEGATON_SCREAMER, &NegatonScreamerAI::Create);
	mgr->register_creature_script(CN_NEGATON_WARP_MASTER, &NegatonWarpMasterAI::Create);
	mgr->register_creature_script(CN_NEGATON_FIELD, &NegatonFieldAI::Create);
	mgr->register_creature_script(CN_PHASE_HUNTER, &PhaseHunterAI::Create);
	mgr->register_creature_script(CN_PROTEAN_HORROR, &ProteanHorrorAI::Create);
	mgr->register_creature_script(CN_PROTEAN_NIGHTMARE, &ProteanNightmareAI::Create);

	mgr->register_creature_script(CN_SULFURON_MAGMA_THROWER, &SulfuronMagmaThrowerAI::Create);
	mgr->register_creature_script(CN_SPITEFUL_TEMPTRESS, &SpitefulTemptressAI::Create);
	mgr->register_creature_script(CN_SOUL_DEVOURER, &SoulDevourerAI::Create);
	mgr->register_creature_script(CN_SOLOMON, &SolomonAI::Create);
	mgr->register_creature_script(CN_SKULKING_WITCH, &SkulkingWitchAI::Create);
	mgr->register_creature_script(CN_SIGHTLESS_EYE, &SightlessEyeAI::Create);
	mgr->register_creature_script(CN_SARGERON_HELLCALLER, &SargeronHellcallerAI::Create);
	mgr->register_creature_script(CN_UNBOUND_DEVASTATOR, &UnboundDevastatorAI::Create);
	mgr->register_creature_script(CN_UNCHAINED_DOOMBRINGER, &UnchainedDoombringerAI::Create);
	mgr->register_creature_script(CN_WARDER_CORPSE, &WarderCorpseAI::Create);
	mgr->register_creature_script(CN_SARGERON_ARCHER, &SargeronArcherAI::Create);

	mgr->register_creature_script(CN_ZEREKETH, &ZerekethAI::Create);
	mgr->register_creature_script(CN_VOIDZONEARC, &VoidZoneARC::Create);

	mgr->register_creature_script(CN_DALLIAH_THE_DOOMSAYER, &DalliahTheDoomsayerAI::Create);
	mgr->register_creature_script(CN_WRATH_SCRYER_SOCCOTHRATES, &WrathScryerSoccothratesAI::Create);
	mgr->register_creature_script(CN_HARBRINGER_SKYRISS, &HarbringerSkyrissAI::Create);
	//mgr->register_creature_script(CN_WARDEN_MELLICHAR, &WardenMellicharAI::Create);
}

// Note: Don't have infos about: Akkiris Lightning-Waker, Nathan, Third Fragment Guardian, Udalo, Whisper and 
// Twilight Drakonaar; some of those mobs aren't aggresive.