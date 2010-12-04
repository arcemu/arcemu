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
/* Instance_ShadowLabyrinth.cpp Script									*/
/************************************************************************/

// Cabal Acolyte AI

#define CN_CABAL_ACOLYTE 18633

#define SHADOW_PROTECTION 17548 // couldn't find more accurate id
#define HEAL 31739 // no idea if this is good id

class CABALACOLYTEAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(CABALACOLYTEAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    CABALACOLYTEAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(SHADOW_PROTECTION);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].perctrigger = 6.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(HEAL);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = false;
		spells[1].perctrigger = 6.0f;
		spells[1].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
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
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
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
};

// Cabal Deathsworn AI

#define CN_CABAL_DEATHSWORN 18635

#define SHADOW_CLEAVE 30495 // should be 29832, 30495 or maybe other ?
#define KNOCKBACK 37317 // 37317 or 38576 or other?
#define BLACK_CLEAVE 38226
// Does it use black cleave too? :|

class CABALDEATHSWORNAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(CABALDEATHSWORNAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    CABALDEATHSWORNAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(SHADOW_CLEAVE);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 9.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(KNOCKBACK);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].perctrigger = 6.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(BLACK_CLEAVE);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].perctrigger = 9.0f;
		spells[2].attackstoptimer = 1000;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
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
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
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
};

// Cabal Fanatic AI

#define CN_CABAL_FANATIC 18830

#define FIXATE 40414

class CABALFANATICAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(CABALFANATICAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    CABALFANATICAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(FIXATE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
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
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
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
};

// Cabal Shadow Priest AI

#define CN_CABAL_SHADOW_PRIEST 18637

#define MIND_FLAY 29570 // 29570 or 37276
#define SHADOW_WORD_PAIN 24212 // 24212 or 34441

class CABALSHADOWPRIESTAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(CABALSHADOWPRIESTAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    CABALSHADOWPRIESTAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(MIND_FLAY);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 7.0f; 
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(SHADOW_WORD_PAIN);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
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
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
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
};

// Cabal Spellbinder AI

#define CN_CABAL_SPELLBINDER 18639

#define MIND_CONTROL 36797 // 36797 or 36798
#define EARTH_SHOCK 26194 // 26194 or 24685 or 22885

class CABALSPELLBINDERAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(CABALSPELLBINDERAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    CABALSPELLBINDERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(MIND_CONTROL);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 7.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(EARTH_SHOCK);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
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
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
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
};

// Cabal Warlock AI

#define CN_CABAL_WARLOCK 18640

#define SHADOW_BOLT 31618 // can be also: 31618, 32860, 36714, 30686, 36986 and many others
#define SEED_OF_CORRUPTION 32863 // can be: 32863, 36123, 38252, 39367
// Should come with Succubus or Felhunter as pet

class CABALWARLOCKAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(CABALWARLOCKAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    CABALWARLOCKAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(SHADOW_BOLT);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 13.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(SEED_OF_CORRUPTION);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].perctrigger = 8.0f;
		spells[1].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
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
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
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
};

// Cabal Zealot AI
#define CN_CABAL_ZEALOT 18638

#define SHADOW_BOLT 31618 // no idea to this spell (added same as for warlock)
#define TRANSFROMATION 0 // can't find correct id for now
/*turns into a mini-Magmadar at low health, cannot 
cast but does extremely heavy melee damage.*/

class CABALZEALOTAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(CABALZEALOTAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    CABALZEALOTAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(SHADOW_BOLT);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 13.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(TRANSFROMATION);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
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
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
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
};

/*
   #  Cabal Ritualist

    * with daggers - Gouges, hits about 1500 on cloth, low health.
    * with staff or single blade - 3 different types of casters 

   1. Arcane - Arcane Missles, Addle Humanoid
   2. Fire - Fire Blast, Flame Buffet (small amount of damage and DoT)
   3. Frost - Frostbolt 

    * Heroic: can dispel CC, such as Trap and Polymorph. Tank and kill away from other CC'd Ritualists.
    * Heroic: immune to MC 

*/
// Cabal Ritualist AI

#define CN_CABAL_RITUALIST 18794

#define	GOUGE	36862 // 36862 or 29425 or 34940 or 28456 (?)
#define	ARCANE_MISSILES	35034 // not sure
#define	ADDLE_HUMANOID	33487
#define	FIRE_BLAST	36339
#define	FLAME_BUFFET	34121
#define	FROSTBOLT	32370 // also can be: 36710, 32370, 32364, 36279

// no idea how to separate spells for same creature (as in DB/sites is only one
// type of that creature!) so I am giving "all in one" and I don't have time to
// think about it.

class CABALRITUALISTAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(CABALRITUALISTAI);
	SP_AI_Spell spells[6];
	bool m_spellcheck[6];

    CABALRITUALISTAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 6;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(GOUGE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(ARCANE_MISSILES);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 8.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(ADDLE_HUMANOID);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = false;
		spells[2].perctrigger = 7.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(FIRE_BLAST);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = true;
		spells[3].perctrigger = 9.0f;
		spells[3].attackstoptimer = 1000;

		spells[4].info = dbcSpell.LookupEntry(FLAME_BUFFET);
		spells[4].targettype = TARGET_VARIOUS; // ?
		spells[4].instant = false;
		spells[4].perctrigger = 9.0f;
		spells[4].attackstoptimer = 1000;

		spells[5].info = dbcSpell.LookupEntry(FROSTBOLT);
		spells[5].targettype = TARGET_ATTACKING;
		spells[5].instant = false;
		spells[5].perctrigger = 9.0f;
		spells[5].attackstoptimer = 1000;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
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
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
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
};

// Fel Overseer AI

#define CN_FEL_OVERSEER 18796

#define INTIMIDATING_SHOUT 33789
#define CHARGE_OVERSEER 33709 // no idea
#define HEAL_OVERSEER 33144 // 32130/33144 || here too :P	// doesn't work?:|
#define MORTAL_STRIKE 24573 // same
#define UPPERCUT 32055

class FELOVERSEERAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(FELOVERSEERAI);
	SP_AI_Spell spells[5];
	bool m_spellcheck[5];

    FELOVERSEERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		HealCooldown = 1;
		nrspells = 5;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(INTIMIDATING_SHOUT);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 4.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(CHARGE_OVERSEER);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 5.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(HEAL_OVERSEER);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true; // should be false, but doesn't work then
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(MORTAL_STRIKE);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = true;
		spells[3].perctrigger = 12.0f;
		spells[3].attackstoptimer = 1000;

		spells[4].info = dbcSpell.LookupEntry(UPPERCUT);
		spells[4].targettype = TARGET_ATTACKING;
		spells[4].instant = true;
		spells[4].perctrigger = 5.0f;
		spells[4].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		HealCooldown = 1;
        RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
    }

    void OnCombatStop(Unit* mTarget)
    {
		HealCooldown = 1;
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		HealCooldown = 1;
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		HealCooldown--;
		if (_unit->GetHealthPct() <= 25 && HealCooldown <= 0)
		{
			_unit->CastSpell(_unit, spells[2].info, spells[2].instant);
			HealCooldown = 60;
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

	int HealCooldown;
	int nrspells;
};

// Malicious Instructor AI

#define CN_MALICIOUS_INSTRUCTOR 18848

#define SHADOW_NOVA 33846
#define MARK_OF_MALICE 33493

class MALICIOUSINSTRUCTORAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(MALICIOUSINSTRUCTORAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    MALICIOUSINSTRUCTORAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(SHADOW_NOVA);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 12.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(MARK_OF_MALICE);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].perctrigger = 9.0f;
		spells[1].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
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
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
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
};

/*****************************/
/*                           */
/*         Boss AIs          */
/*                           */
/*****************************/

// Ambassador HellmawAI 

#define CN_AMBASSADOR_HELLMAW 18731

#define CORROSIVE_ACID 33551
#define AOE_FEAR 33547
// Help sound

class AMBASSADORHELLMAWAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(AMBASSADORHELLMAWAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    AMBASSADORHELLMAWAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		} 

		spells[0].info = dbcSpell.LookupEntry(CORROSIVE_ACID);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 15;

		spells[1].info = dbcSpell.LookupEntry(AOE_FEAR);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;
		spells[1].cooldown = 25;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		for (int i = 0; i < 2; i++)
			spells[i].casttime = 0;
		spells[1].casttime = (uint32)time(NULL) + 25;

		int RandomSpeach = rand()%3;
		switch (RandomSpeach)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Pathetic mortals, you will pay dearly!"); // not sure if doomed
			_unit->PlaySoundToSet(10475);
			break;
		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I will break you"); // drink you, bring you, w00t?
			_unit->PlaySoundToSet(10476);
			break;
		case 2:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Finally, something to relieve the tedium!"); // something to believe the team?
			_unit->PlaySoundToSet(10477);
			break;
		}
		RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
    }

	void OnTargetDied(Unit* mTarget)
    {
		if (_unit->GetHealthPct() > 0)	// Hack to prevent double yelling (OnDied and OnTargetDied when creature is dying)
		{
			int RandomSpeach = rand()%2;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Do you fear death? Hahaha!"); // needs corrections
				_unit->PlaySoundToSet(10478);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "This is the part I enjoy most!");
				_unit->PlaySoundToSet(10479);
				break;
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
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Do not... grow... overconfident... mortal!");
		_unit->PlaySoundToSet(10480);

		RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		uint32 t = (uint32)time(NULL);
		if (t > spells[1].casttime && _unit->GetCurrentSpell() == NULL)
		{
			_unit->CastSpell(_unit, spells[1].info, spells[1].instant);

			spells[1].casttime = t + spells[1].cooldown;
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

protected:

	int nrspells;
};


// Blackheart the InciterAI

#define CN_BLACKHEART_THE_INCITER 18667

#define INCITE_CHAOS 33684 // 33684 or 33676?
#define CHARGE 39574 //39574 (HM) or 38461 // couldn't find more accurate
#define WAR_STOMP 33707
//#define AOE_KNOCKBACK 30056 // 30056, 37317 or 38576
// Help sound? And other (as there were sounds like _BlckHrt02_ (and I used only with 01)
// Is sound id 10488 for aggro or mind control?

class BLACKHEARTTHEINCITERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(BLACKHEARTTHEINCITERAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    BLACKHEARTTHEINCITERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		}

		spells[0].info = dbcSpell.LookupEntry(CHARGE);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 15;
		spells[0].mindist2cast = 0.0f;
		spells[0].maxdist2cast = 40.0f;

		spells[1].info = dbcSpell.LookupEntry(WAR_STOMP);
		spells[1].targettype = TARGET_VARIOUS; 
		spells[1].instant = true;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 1000;
		spells[1].cooldown = 20;

		spells[2].info = dbcSpell.LookupEntry(INCITE_CHAOS);
		spells[2].targettype = TARGET_RANDOM_SINGLE;
		spells[2].instant = true;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;
		spells[2].cooldown = 40;
		spells[2].mindist2cast = 0.0f;
		spells[2].maxdist2cast = 40.0f;
    }
    // sound corrections needed!
    void OnCombatStart(Unit* mTarget)
    {
		for (int i = 0; i < 3; i++)
			spells[i].casttime = 0;
		spells[2].casttime = (uint32)time(NULL) + 20;

		int RandomSpeach = rand()%3;
		switch (RandomSpeach)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You'll be sorry!");
			_unit->PlaySoundToSet(10486);
			break;
		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I see dead people.");
			_unit->PlaySoundToSet(10488);
			break;
		case 2:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Time for fun!");
			_unit->PlaySoundToSet(10487);
			break;
		}
		RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
    }

	void OnTargetDied(Unit* mTarget)
    {
		if (_unit->GetHealthPct() > 0)	// Hack to prevent double yelling (OnDied and OnTargetDied when creature is dying)
		{
			int RandomSpeach = rand()%2;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "No comming back for you!");
				_unit->PlaySoundToSet(10489);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Nice try!");
				_unit->PlaySoundToSet(10490);
				break;
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
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "This...no..good");
		_unit->PlaySoundToSet(10491);
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		uint32 t = (uint32)time(NULL);
		if (t > spells[2].casttime && _unit->GetCurrentSpell() == NULL)
		{
			CastSpellOnRandomTarget(2, spells[2].mindist2cast, spells[2].maxdist2cast, 0, 100);

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

	int nrspells;
};

// Grandmaster VorpilAI

#define CN_GRANDMASTER_VORPIL 18732

#define SHADOW_BOLT_VOLLEY	33841
#define DRAW_SHADOWS		33563 // can't find it =/
#define RAIN_OF_FIRE		33617 // 33617 or 34360	// breaks model behavior
#define VOID_PORTAL_VISUAL	33569 // must work on that 33566
// Help sound and it's case?
// Should OnCombatStart should spawn 3 portals for Voidwalkers? (33569 ?)
/*The fight starts as soon as one of the players moves close enough
to Vorpil to aggro him. Vorpil will immediately open the Void Rifts
around him, and Voidwalkers will start spawning, at an increasingly 
faster rate as the battle progresses.*/ 

class GRANDMASTERVORPILAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(GRANDMASTERVORPILAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    GRANDMASTERVORPILAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		}

		spells[0].info = dbcSpell.LookupEntry(SHADOW_BOLT_VOLLEY);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 15.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 10;

		spells[1].info = dbcSpell.LookupEntry(DRAW_SHADOWS);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].perctrigger = 8.0f;
		spells[1].attackstoptimer = 1000;
		spells[1].cooldown = 25;

		spells[2].info = dbcSpell.LookupEntry(RAIN_OF_FIRE);
		spells[2].targettype = TARGET_DESTINATION; 
		spells[2].instant = false;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 7000;
		spells[2].cooldown = 25;

		spells[3].info = dbcSpell.LookupEntry(VOID_PORTAL_VISUAL);
		spells[3].targettype = TARGET_SELF; 
		spells[3].instant = true;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;
		spells[3].cooldown = -1;

		Teleported = false;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		for (int i = 0; i < 4; i++)
			spells[i].casttime = 0;

		Teleported = false;

		int RandomSpeach = rand()%3;
		switch (RandomSpeach)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I'll make an offering of your blood!");
			_unit->PlaySoundToSet(10524);
			break;
		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You'll be a fine example, for the others.");
			_unit->PlaySoundToSet(10525);
			break;
		case 2:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Good, a worthy sacrifice.");
			_unit->PlaySoundToSet(10526);
			break;
		}
		//_unit->CastSpell(_unit, spells[3].info, spells[3].instant);
		RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
    }

	void OnTargetDied(Unit* mTarget)
    {
		if (_unit->GetHealthPct() > 0)	// Hack to prevent double yelling (OnDied and OnTargetDied when creature is dying)
		{
			int RandomSpeach = rand()%2;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I serve with pride.");
				_unit->PlaySoundToSet(10527);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your death is for the greater cause!");
				_unit->PlaySoundToSet(10528);
				break;
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
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I give my life... Gladly.");
		_unit->PlaySoundToSet(10529);

		RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		if (Teleported && _unit->GetCurrentSpell() == NULL)
		{
			_unit->CastSpellAoF(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), spells[2].info, spells[2].instant);

			Teleported = false;
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
					}

					if (i == 1)
						Teleported = true;

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

protected:

	bool Teleported;
	int nrspells;
};

// MurmurAI

#define CN_MURMUR 18708

#define SHOCKWAVE			33686	// 25425
#define MURMURS_TOUCH		33711
#define SONIC_BOOM1			33923	// anything missed? additional dmging spell?
#define SONIC_BOOM2			33666
#define RESONANCE			33657	// should be applied only when no target in melee combat range (each 5 sec)

/*#define THUNDERING_STORM 39365

#define SUPPRESSION_BLAST 33332*/
// #define SONIC_BOOM 33666
// 33666 or 38795 (I think it's dummy (33923); it should be connected with Murmur's Touch; Murmur should say 
// Murmur draws energy from the air then use Murmur's Touch (33711), Sonic Boom (33923) and
// release Sonic Boom (38052); it also shouldn't attack during all those casts;

class MURMURAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(MURMURAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    MURMURAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		} 

		spells[0].info = dbcSpell.LookupEntry(SONIC_BOOM1);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = false;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 25;
		//spells[0].speech = "Murmur draws energy from the air...";

		spells[1].info = dbcSpell.LookupEntry(SHOCKWAVE);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].perctrigger = 6.0f;
		spells[1].attackstoptimer = 1000;
		spells[1].cooldown = 15;

		spells[2].info = dbcSpell.LookupEntry(MURMURS_TOUCH);
		spells[2].targettype = TARGET_VARIOUS; 
		spells[2].instant = false;
		spells[2].perctrigger = 10.0f;
		spells[2].attackstoptimer = 2000;
		spells[2].cooldown = 20;

		spells[3].info = dbcSpell.LookupEntry(RESONANCE);
		spells[3].targettype = TARGET_VARIOUS;
		spells[3].instant = true;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;
		spells[3].cooldown = 5;

		if(_unit->GetMapMgr() != NULL && _unit->GetMapMgr()->iInstanceMode != MODE_HEROIC && _unit->GetHealthPct() >= 41)
		{
			_unit->SetHealthPct(40);
		}

		SonicBoom = false;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		for (int i = 0; i < 3; i++)
			spells[i].casttime = 0;
		spells[3].casttime = (uint32)time(NULL) + 5;

		SonicBoom = false;

		if(_unit->GetMapMgr() != NULL && _unit->GetMapMgr()->iInstanceMode != MODE_HEROIC && _unit->GetHealthPct() >= 41)
		{
			_unit->SetHealthPct(40);
		}

		RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));

		_unit->Root();
    }

    void OnCombatStop(Unit* mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

		if(_unit->GetMapMgr() != NULL && _unit->GetMapMgr()->iInstanceMode != MODE_HEROIC && _unit->GetHealthPct() >= 41)
		{
			_unit->SetHealthPct(40);
		}

        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		if (SonicBoom)
		{
			RemoveAIUpdateEvent();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));

			if (_unit->GetCurrentSpell())
				_unit->GetCurrentSpell()->cancel();

			_unit->CastSpell(_unit, SONIC_BOOM2, true);

			SonicBoom = false;
			return;
		}

		uint32 t = (uint32)time(NULL);
		if (t > spells[3].casttime && _unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget() && _unit->GetDistance2dSq(_unit->GetAIInterface()->getNextTarget()) >= 325.0f)
		{
			Unit* target = NULL;
			target = FindTarget();
			if (target)
			{
				_unit->CastSpell(target, spells[3].info, spells[3].instant);
				_unit->GetAIInterface()->ClearHateList();
				_unit->GetAIInterface()->AttackReaction(target, 100, 0);

				spells[3].casttime = t + spells[3].cooldown;
				return;
			}

			else
			{
				_unit->GetAIInterface()->WipeTargetList();
				_unit->GetAIInterface()->WipeHateList();
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

					if (i == 0)
					{
						RemoveAIUpdateEvent();
						_unit->setAttackTimer(7000, false);
						RegisterAIUpdateEvent(5000);

						SonicBoom = true;
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

	// A bit rewritten FindTarget function
	Unit* FindTarget()
	{
		Unit* target = NULL;
		float distance = 40.0f;
		float z_diff;

		Unit* pUnit;
		float dist;

		for (set<Object*>::iterator itr = _unit->GetInRangeOppFactsSetBegin(); itr != _unit->GetInRangeOppFactsSetEnd(); itr++)
		{
			if(!(*itr)->IsUnit())
				continue;

			pUnit = TO_UNIT((*itr));

			if(pUnit->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_FEIGN_DEATH))
				continue;

			z_diff = fabs(_unit->GetPositionZ() - pUnit->GetPositionZ());
			if(z_diff > 2.5f)
				continue;
			
			if(!pUnit->isAlive() || _unit == pUnit)
				continue;

			dist = _unit->GetDistance2dSq(pUnit);

			if(dist > distance*distance)
				continue;

			if (dist < 3.0f)
				continue;

			distance = dist;
			target = pUnit;
		}

		return target;
	}

protected:

	bool SonicBoom;
	int nrspells;
};

void SetupShadowLabyrinth(ScriptMgr * mgr)
{
	mgr->register_creature_script(CN_CABAL_ACOLYTE, &CABALACOLYTEAI::Create);
	mgr->register_creature_script(CN_CABAL_DEATHSWORN, &CABALDEATHSWORNAI::Create);
	mgr->register_creature_script(CN_CABAL_FANATIC, &CABALFANATICAI::Create);
	mgr->register_creature_script(CN_CABAL_SHADOW_PRIEST, &CABALSHADOWPRIESTAI::Create);
	mgr->register_creature_script(CN_CABAL_SPELLBINDER, &CABALSPELLBINDERAI::Create);
	mgr->register_creature_script(CN_CABAL_WARLOCK, &CABALWARLOCKAI::Create);
	mgr->register_creature_script(CN_CABAL_ZEALOT, &CABALZEALOTAI::Create);
	mgr->register_creature_script(CN_CABAL_RITUALIST, &CABALRITUALISTAI::Create);
	mgr->register_creature_script(CN_FEL_OVERSEER, &FELOVERSEERAI::Create);
	mgr->register_creature_script(CN_MALICIOUS_INSTRUCTOR, &MALICIOUSINSTRUCTORAI::Create);
	mgr->register_creature_script(CN_AMBASSADOR_HELLMAW, &AMBASSADORHELLMAWAI::Create);
	mgr->register_creature_script(CN_BLACKHEART_THE_INCITER, &BLACKHEARTTHEINCITERAI::Create);
	mgr->register_creature_script(CN_GRANDMASTER_VORPIL, &GRANDMASTERVORPILAI::Create);
    mgr->register_creature_script(CN_MURMUR, &MURMURAI::Create);
}

// No accurate info about spells of Cabal Assassins, Cultist, Executioner, Summoner
// so they aren't scripted
