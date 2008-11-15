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

/************************************************************************/
/* Instance_WailinCaverns.cpp Script		                               			     */
/************************************************************************/

struct Coords
{
    float x;
    float y;
    float z;
    float o;
};

// Devouring Ectoplasm AI by Soulshifter

#define CN_DEVOURING_ECTOPLASM		3638
#define SPELL_SUMMON_EVOLVING      	7968 


class DevouringEctoplasmAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(DevouringEctoplasmAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];
		
    DevouringEctoplasmAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 1;
		
		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
		// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 
        spells[0].info = dbcSpell.LookupEntry(SPELL_SUMMON_EVOLVING);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = false;
		spells[0].perctrigger = 10.0f;
		spells[0].cooldown = 20000;
		spells[0].attackstoptimer = 1000; // 1sec
		
	}
    
    void OnCombatStart(Unit* mTarget)
    {
        RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
	}

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
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


// Druid of the Fang AI by Soulshifter

#define CN_DRUID_FANG				3840
#define SPELL_LIGHTNING_BOLT      	915 
#define SPELL_SERPENT_FORM     		32817 
#define SPELL_HEALINGT     			5187 

class DruidFangAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(DruidFangAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];
	bool healed;
		
    DruidFangAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 3;
		healed = false;
		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
		// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 
        spells[0].info = dbcSpell.LookupEntry(SPELL_LIGHTNING_BOLT);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 30.0f;
		spells[0].attackstoptimer = 1000; // 1sec
		
		spells[1].info = dbcSpell.LookupEntry(SPELL_SERPENT_FORM);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].perctrigger = 30.0f;
		spells[1].attackstoptimer = 1000; // 1sec
		
		spells[2].info = dbcSpell.LookupEntry(SPELL_HEALINGT);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000; // 1sec
	}
    
    void OnCombatStart(Unit* mTarget)
    {
        RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
	}

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		if(!healed && _unit->GetHealthPct() <= 5)
		{
		_unit->CastSpell(_unit, spells[2].info, spells[2].instant);
		healed = true;
		}
		
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

// BOSSES

// Lady Anacondra AI by Soulshifter

#define CN_LADY_ANACONDRA			3671
#define SPELL_LIGHTNING_BOLT      	915 
#define SPELL_SLEEP			     	1090
#define SAY_AGGROLADY               "No one can stand against the Serpent Lords!"
#define SOUND_AGGROLADY          	5786

class LadyAnacondraAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(LadyAnacondraAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];
	bool Used_sleep;
	
    LadyAnacondraAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 2;
		
		//This defines if Lady has used her sleep spell (just castable once)
		Used_sleep = false;
		
		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
		// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 
        spells[0].info = dbcSpell.LookupEntry(SPELL_LIGHTNING_BOLT);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 30.0f;
		spells[0].attackstoptimer = 1000; // 1sec
		
		spells[1].info = dbcSpell.LookupEntry(SPELL_SLEEP);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 5000; // 1sec
	}
    
    void OnCombatStart(Unit* mTarget)
    {
		_unit->PlaySoundToSet(SOUND_AGGROLADY);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, SAY_AGGROLADY);
        RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
	}

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		
		if(!Used_sleep)
		{
		Unit *target = NULL;
		target = _unit->GetAIInterface()->GetNextTarget();
		_unit->CastSpell(target,spells[1].info, spells[1].instant);
		Used_sleep = true;
		}
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

// Lord Cobrahn AI by Soulshifter

#define CN_LORD_COBRAHN				3669
#define SPELL_LIGHTNING_BOLT      	915 
#define SPELL_POISON		     	34969
#define SPELL_SERPENTFORM			7965
#define SAY_AGGROCOBRAHN            "You will never wake the dreamer!"
#define SOUND_AGGROCOBRAHN        	5785

class LordCobrahnAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(LordCobrahnAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];
	bool Serpent_form;
	
    LordCobrahnAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 3;
		
		// Defines if Serpent form is active or not.
		Serpent_form = false;
		
		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
		// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 
        spells[0].info = dbcSpell.LookupEntry(SPELL_LIGHTNING_BOLT);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 30.0f;
		spells[0].attackstoptimer = 1000; // 1sec
		
		spells[1].info = dbcSpell.LookupEntry(SPELL_POISON);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 30.0f;
		spells[1].attackstoptimer = 1000; // 1sec
		
		spells[2].info = dbcSpell.LookupEntry(SPELL_SERPENTFORM);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000; // 1sec
	}
    
    void OnCombatStart(Unit* mTarget)
    {
        _unit->PlaySoundToSet(SOUND_AGGROCOBRAHN);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, SAY_AGGROCOBRAHN);
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
	}

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		if(!Serpent_form && _unit->GetHealthPct() <= 20)
		{
		_unit->CastSpell(_unit, spells[2].info, spells[2].instant);
		Serpent_form = true;
		}
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

// Lord Pythas AI by Soulshifter

#define CN_LORD_PYTHAS				3670
#define SPELL_LIGHTNING_BOLT      	915 
#define SPELL_SLEEP2		     	1090
#define SPELL_THUNDERCLAP			8732
#define SAY_AGGROPYTHAS            "The coils of death... Will crush you!"
#define SOUND_AGGROPYTHAS        	5787

class LordPythasAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(LordPythasAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];
	bool Used_sleep;
	
    LordPythasAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 3;
		
		//This defines if Pythas has used his sleep spell. Just usable once. (Starts on false).
		Used_sleep = false;
		
		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
		// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 
        spells[0].info = dbcSpell.LookupEntry(SPELL_LIGHTNING_BOLT);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 30.0f;
		spells[0].attackstoptimer = 1000; // 1sec
		
		spells[1].info = dbcSpell.LookupEntry(SPELL_SLEEP2);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 5000; // 1sec
		
		spells[2].info = dbcSpell.LookupEntry(SPELL_THUNDERCLAP);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].perctrigger = 20.0f;
		spells[2].attackstoptimer = 1000; // 1sec
	}
    
    void OnCombatStart(Unit* mTarget)
    {
        _unit->PlaySoundToSet(SOUND_AGGROPYTHAS);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, SAY_AGGROPYTHAS);
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
	}

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		if(!Used_sleep)
		{
		Unit *target = NULL;
		target = _unit->GetAIInterface()->GetNextTarget();
		_unit->CastSpell(target,spells[1].info, spells[1].instant);
		Used_sleep = true;
		}
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

// Lord Serpentis AI by Soulshifter

#define CN_LORD_SERPENTIS			3673
#define SPELL_LIGHTNING_BOLT      	915 
#define SPELL_SLEEP3		     	1090
#define SAY_AGGROSERPENTIS     		"I am the serpent king, i can do anything!"
#define SOUND_AGGROSERPENTIS      	5788

class LordSerpentisAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(LordSerpentisAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];
	bool Used_sleep;
    LordSerpentisAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 2;
		
		//This defines if Serpentis has used his sleep spell. Just usable once. (Starts on false).
		Used_sleep = false;
		
		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
		// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 
        spells[0].info = dbcSpell.LookupEntry(SPELL_LIGHTNING_BOLT);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 30.0f;
		spells[0].attackstoptimer = 1000; // 1sec
		
		spells[1].info = dbcSpell.LookupEntry(SPELL_SLEEP3);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 5000; // 1sec
		
	}
    
    void OnCombatStart(Unit* mTarget)
    {
        _unit->PlaySoundToSet(SOUND_AGGROSERPENTIS);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, SAY_AGGROSERPENTIS);
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
	}

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		if(!Used_sleep)
		{
		Unit *target = NULL;
		target = _unit->GetAIInterface()->GetNextTarget();
		_unit->CastSpell(target, spells[1].info, spells[1].instant);
		Used_sleep = true;
		}
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

// Verdan the Everliving AI by Soulshifter

#define CN_VERDAN_EVERLIVING		5775
#define SPELL_GRASPING_VINES      	8142 

class VerdanEverlivingAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(VerdanEverlivingAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];
	
    VerdanEverlivingAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 1;
			
		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
		// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 
        spells[0].info = dbcSpell.LookupEntry(SPELL_GRASPING_VINES);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 30.0f;
		spells[0].attackstoptimer = 1000; // 1sec
			
	}
    
    void OnCombatStart(Unit* mTarget)
    {
       	RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
	}

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
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

// Skum AI by Soulshifter

#define CN_SKUM						3674
#define SPELL_CHAINED_BOLT      	15549 

class SkumAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(SkumAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];
	bool Run_in_fear;
    SkumAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 1;
			
		// This defines if Skum is low on HP and try to run away (starts on false).
		Run_in_fear = false;
					
		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
		// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 
        spells[0].info = dbcSpell.LookupEntry(SPELL_CHAINED_BOLT);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 50.0f;
		spells[0].attackstoptimer = 1000; // 1sec
			
	}
    
    void OnCombatStart(Unit* mTarget)
    {
       	RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
	}

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		if(!Run_in_fear && _unit->GetHealthPct() <= 10)
		{
		_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Trying to run cower in fear");
		Run_in_fear = true;
		}
		if(Run_in_fear)
		{
		_unit->GetAIInterface()->MoveTo(-262.829742f, -299.363159f, -68.293579f, 0.267827f);
		}
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

// Kresh AI by Soulshifter

#define CN_KRESH			3653
#define SPELL_BITE			17258
#define	SPELL_SHELL_SHIELD	40087

class KreshAI : public CreatureAIScript

{

public:
	ADD_CREATURE_FACTORY_FUNCTION(KreshAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];
		
    KreshAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 2;
		
		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
		// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 
        spells[0].info = dbcSpell.LookupEntry(SPELL_BITE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 30.0f;
		spells[0].cooldown = 20000;
		spells[0].attackstoptimer = 1000; // 1sec
		
		spells[1].info = dbcSpell.LookupEntry(SPELL_SHELL_SHIELD);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = false;
		spells[1].perctrigger = 15.0f;
		spells[1].cooldown = 20000;
		spells[1].attackstoptimer = 1000; // 1sec
		
	}
    
    void OnCombatStart(Unit* mTarget)
    {
        RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
	}

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
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

// Mutanus the Devourer AI by Soulshifter

#define CN_MUTANUS				3654
#define SPELL_THUNDERSHOCK		7803
#define	SPELL_TERRIFY			7399

class MutanusAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(MutanusAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];
		
    MutanusAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 2;
		
		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
		// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 
        spells[0].info = dbcSpell.LookupEntry(SPELL_THUNDERSHOCK);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 15.0f;
		spells[0].cooldown = 20000;
		spells[0].attackstoptimer = 1000; // 1sec
		
		spells[1].info = dbcSpell.LookupEntry(SPELL_TERRIFY);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 15.0f;
		spells[1].cooldown = 20000;
		spells[1].attackstoptimer = 1000; // 1sec
		
	}
    
    void OnCombatStart(Unit* mTarget)
    {
        RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
	}

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
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



// Wailing Caverns Event
// Discipline of Naralex Gossip by Soulshifter

#define CN_NARALEX				3679
#define CN_DIS_NARALEX			3678
#define SPELL_AWAKENING			6271

static Coords ToNaralex[] = 
{
	{  },
	{ -132.498077f, 125.888153f, -78.418182f, 0.244260f },
	{ -123.892235f, 130.538422f, -78.808937f, 0.519935f },
	{ -116.654480f, 142.935806f, -80.233383f, 1.149039f },
	{ -111.656868f, 156.927307f, -79.880676f, 1.344603f },
	{ -108.829506f, 169.213165f, -79.752487f, 1.344603f },
	{ -107.592789f, 183.854782f, -79.735558f, 1.500112f },
	{ -106.628258f, 197.477676f, -80.526184f, 1.500112f },
	{ -109.725700f, 215.487885f, -85.336075f, 1.650287f },
	{ -106.663147f, 225.879135f, -88.962914f, 0.215201f },
	{ -90.607216f, 228.829071f, -91.022133f, 6.067203f },
	{ -79.377800f, 219.999466f, -93.990906f, 5.482866f },
	{ -69.134697f, 209.446045f, -93.404358f, 5.482866f },
	{ -53.198994f, 204.919601f, -95.677971f, 6.071915f },
	{ -38.501598f, 211.024460f, -96.222626f, 0.559205f },
	{ -39.211544f, 197.527161f, -96.574646f, 4.658991f },
	{ -40.258022f, 177.948105f, -96.374756f, 4.658991f },
	{ -41.385948f, 156.845230f, -94.969429f, 4.658991f },
	{ -49.557240f, 145.598206f, -93.284225f, 4.084079f },
	{ -52.191185f, 133.269424f, -90.334198f, 4.501911f },
	{ -53.070702f, 122.185814f, -89.757874f, 5.128569f },
	{ -47.618214f, 115.986847f, -87.939827f, 5.562199f },
	{ -36.105568f, 109.539597f, -87.755760f, 5.772686f },
	{ -23.849794f, 109.712982f, -89.580704f, 0.014146f },
	{ -15.070121f, 119.703346f, -89.904770f, 0.849840f },
	{ -6.799855f, 134.471298f, -89.574089f, 1.060297f },
	{ 1.530990f, 143.322433f, -89.091454f, 0.589058f },
	{ 11.134405f, 149.739365f, -88.872955f, 0.589058f },
	{ 21.220901f, 156.479080f, -89.180771f, 0.802686f },
	{ 31.682161f, 167.308456f, -88.896530f, 0.802686f },
	{ 43.933167f, 179.990555f, -88.922348f, 0.802686f },
	{ 51.662514f, 195.831421f, -89.649101f, 1.116846f },
	{ 70.554794f, 204.757950f, -92.880386f, 0.441403f },
	{ 85.016724f, 211.591156f, -92.615730f, 0.441403f },
	{ 99.523796f, 213.738951f, -96.214615f, 0.047919f },
	{ 112.235191f, 214.378525f, -98.332832f, 0.679379f },
	{ 118.665100f, 220.504974f, -98.305420f, 1.254290f },
	{ 121.259758f, 228.493378f, -97.359711f, 1.632852f },
	{ 116.031120f, 236.451187f, -96.007195f, 3.089230f }
};

class SCRIPT_DECL DofNaralexGossip : public GossipScript
{
public:
	void GossipHello(Object * pObject, Player* Plr, bool AutoSend);
	void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code);
	void GossipEnd(Object * pObject, Player* Plr);
	void Destroy()
	{
	delete this;
	}
	
};
	
	
void DofNaralexGossip::GossipHello(Object * pObject, Player* Plr, bool AutoSend)
{
	
	Unit* Fanglord1 = pObject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-151.139008f, 414.367004f, -72.629402f, CN_LORD_COBRAHN);
	Unit* Fanglord2 = pObject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(36.807400f, -241.063995f, -79.498901f, CN_LORD_PYTHAS);
	Unit* Fanglord3 = pObject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-118.710999f, -24.990999f, -28.498501f, CN_LORD_SERPENTIS);
	Unit* Fanglord4 = pObject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-70.788902f, 120.072998f, -89.673599f, CN_LADY_ANACONDRA);
		
	if((!Fanglord1 || !Fanglord1->isAlive()) && (!Fanglord2 || !Fanglord2->isAlive()) && (!Fanglord3 || !Fanglord3->isAlive()) && (!Fanglord4 || !Fanglord4->isAlive()))
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 699, Plr);
		Menu->AddItem(0, "Let's go!", 2);
		Menu->SendTo(Plr);
	}
	else
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 698, Plr);
		Menu->AddItem(0, "I will slay those Fanglords", 1);
		Menu->SendTo(Plr);
	}
	
}

void DofNaralexGossip::GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
{
	Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;
	if(pObject==NULL)
		return;

	switch(IntId)
	{
		case 0: // Return to start

			GossipHello(pCreature, Plr, true);
			break;

		case 1: // Disciple of Naralex Casts Mark of the Wild on players.
			{
				pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Take this! It will be useful for you. I'll be waiting here when you have slain the 4 Fanglords to awake Naralex!");
				pCreature->CastSpell(Plr, 6756, true);
				pCreature->Emote(EMOTE_ONESHOT_CHEER);
				Plr->Gossip_Complete();
			}
			break;

		case 2: // Start Event
			{
				pCreature->SetUInt32Value(UNIT_NPC_FLAGS, 0);
				pCreature->GetAIInterface()->StopMovement(0);
				pCreature->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
				pCreature->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
				pCreature->GetAIInterface()->setWaypointToMove(1);
					
			}
			break;
	}
};

void DofNaralexGossip::GossipEnd(Object * pObject, Player* Plr)
{
GossipScript::GossipEnd(pObject, Plr);
}

// Disciple of Naralex AI by Soulshifter


class DofNaralexAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(DofNaralexAI);
    DofNaralexAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
	
		// --- Initialization ---
		for (int i = 1; i < 39; i++)
		{
			_unit->GetAIInterface()->addWayPoint(CreateWaypoint(i, 0, 256));
		}

		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
		

	}
    
		
	void OnReachWP(uint32 iWaypointId, bool bForwards)
    {
		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
		_unit->GetAIInterface()->setWaypointToMove(iWaypointId+1);
		if(!Awakening && _unit->GetAIInterface()->getCurrentWaypoint() == 39)
			
		{
			RegisterAIUpdateEvent(2500);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Step Back and be ready!, I'll Try to Awake Naralex");
			_unit->Emote(EMOTE_ONESHOT_TALK);
			_unit->CastSpell(_unit, SPELL_AWAKENING, true);	
			SpawnTimer = 40;
		}
	}

	void AIUpdate()
	{
		SpawnTimer--;

		if(!Wave1 && SpawnTimer == 35)
			{
				Moccasin();
				Wave1 = true;
			}
		if(!Wave2 && SpawnTimer == 20)
			{
				Ectoplasm();
				Wave2 = true;
			}
		if(!Boss && SpawnTimer == 1)
			{
				BMutanus();
				Boss = true;
			}
		
	}
		
	void Moccasin()
	{
		Unit *Moccasin1 = NULL;
		Unit *Moccasin2 = NULL;
		Unit *Moccasin3 = NULL;
		Moccasin1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(5762, 134.249207f, 242.194839f, -98.375496f, 3.325373f, false, false, 0, 0);
		Moccasin2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(5762, 124.917931f, 255.066635f, -97.796837f, 4.176745f, false, false, 0, 0);
		Moccasin3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(5762, 113.077148f, 258.880157f, -97.190590f, 4.688039f, false, false, 0, 0);
	}
	void Ectoplasm()
	{
		Unit *Ectoplasm1 = NULL;
		Unit *Ectoplasm2 = NULL;
		Unit *Ectoplasm3 = NULL;
		Unit *Ectoplasm4 = NULL;
		Unit *Ectoplasm5 = NULL;
		Unit *Ectoplasm6 = NULL;
		Unit *Ectoplasm7 = NULL;
		Ectoplasm1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(5763, 134.249207f, 242.194839f, -98.375496f, 3.325373f, false, false, 0, 0);
		Ectoplasm2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(5763, 124.917931f, 255.066635f, -97.796837f, 4.176745f, false, false, 0, 0);
		Ectoplasm3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(5763, 113.077148f, 258.880157f, -97.190590f, 4.688039f, false, false, 0, 0);
		Ectoplasm4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(5763, 138.794693f, 228.224976f, -100.174332f, 2.471645f, false, false, 0, 0);
		Ectoplasm5 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(5763, 128.170364f, 225.190247f, -99.392830f, 2.411169f, false, false, 0, 0);
		Ectoplasm6 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(5763, 136.762009f, 242.685669f, -98.564545f, 3.344223f, false, false, 0, 0);
		Ectoplasm7 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(5763, 122.403961f, 259.438354f, -98.153984f, 4.366811f, false, false, 0, 0);
	}
	void BMutanus()
	{		
		Mutanus = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_MUTANUS, 136.337006f, 263.769989f, -102.666000f, 4.002330f, false, false, 0, 0);
		Naralex = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(115.407997f, 240.016998f, -94.021004f, 3679);
		
		if(!Mutanus || Mutanus->isAlive())
		{
			_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID, 17089);
			Naralex->SetUInt32Value(UNIT_FIELD_DISPLAYID, 17089);
			Naralex->SetStandState(STANDSTATE_STAND);
			Naralex->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "I am awake... at last");
			Naralex->PlaySoundToSet(5789);
			_unit->GetAIInterface()->MoveTo(-6.704030f, 200.308838f, -26.938824f, 1.667772f);
			Naralex->GetAIInterface()->MoveTo(-6.704030f, 200.308838f, -26.938824f, 1.667772f);
			_unit->GetAIInterface()->setMoveType(768);
			Naralex->GetAIInterface()->setMoveType(768);
		}
	}

	inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
    {
        WayPoint * wp = _unit->CreateWaypointStruct();
        wp->id = id;
        wp->x = ToNaralex[id].x;
        wp->y = ToNaralex[id].y;
        wp->z = ToNaralex[id].z;
        wp->o = ToNaralex[id].o;
        wp->waittime = waittime;
        wp->flags = flags;
        wp->forwardemoteoneshot = 0;
        wp->forwardemoteid = 0;
        wp->backwardemoteoneshot = 0;
        wp->backwardemoteid = 0;
        wp->forwardskinid = 0;
        wp->backwardskinid = 0;
        return wp;
    }
	

protected:
	uint32 SpawnTimer;
	bool Wave1;
	bool Wave2;
	bool Boss;
	bool Awakening;
	Unit *Naralex;
	Unit *Mutanus;

};

// Naralex State
class Naralex : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(Naralex);
	Naralex(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		_unit->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		_unit->SetStandState(STANDSTATE_SLEEP);
	}
};
void SetupWailingCaverns(ScriptMgr * mgr)
{
    mgr->register_creature_script(CN_DRUID_FANG, &DruidFangAI::Create);
	mgr->register_creature_script(CN_DEVOURING_ECTOPLASM, &DevouringEctoplasmAI::Create);
	mgr->register_creature_script(CN_LADY_ANACONDRA, &LadyAnacondraAI::Create);
	mgr->register_creature_script(CN_LORD_COBRAHN, &LordCobrahnAI::Create);
	mgr->register_creature_script(CN_LORD_PYTHAS, &LordPythasAI::Create);
	mgr->register_creature_script(CN_LORD_SERPENTIS, &LordSerpentisAI::Create);
	mgr->register_creature_script(CN_VERDAN_EVERLIVING, &VerdanEverlivingAI::Create);
	mgr->register_creature_script(CN_SKUM, &SkumAI::Create);
	mgr->register_creature_script(CN_KRESH, &KreshAI::Create);
	mgr->register_creature_script(CN_MUTANUS, &MutanusAI::Create);
	GossipScript * DNaralex = (GossipScript*) new DofNaralexGossip();
	mgr->register_gossip_script(CN_DIS_NARALEX, DNaralex);
	mgr->register_creature_script(CN_DIS_NARALEX, &DofNaralexAI::Create);
	mgr->register_creature_script(CN_NARALEX, &Naralex::Create);
}