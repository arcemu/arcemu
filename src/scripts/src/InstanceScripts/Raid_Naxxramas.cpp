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
/* Raid_Naxxramas.cpp Script											*/
/************************************************************************/

// ---- Spider Wing ----

// Carrion Spinner AI

#define CN_CARRION_SPINNER 15975

#define POISON_BOLT 26052
#define WEB_WRAP 28618 // TODO: PULL EFFECT *FUN*

class CarrionSpinnerAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(CarrionSpinnerAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    CarrionSpinnerAI(Creature* pCreature) : CreatureAIScript(pCreature)
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
        spells[0].info = dbcSpell.LookupEntry(POISON_BOLT);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 15.0f;
		spells[0].attackstoptimer = 1000; // 1sec

		spells[1].info = dbcSpell.LookupEntry(WEB_WRAP);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].perctrigger = 0.0f;	// 4.0f;
		spells[1].attackstoptimer = 2000; // 2sec
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


// Dread Creeper AI

#define CN_DREAD_CREEPER 15974

#define VEIL_OF_SHADOW 28440

class DreadCreeperAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(DreadCreeperAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    DreadCreeperAI(Creature* pCreature) : CreatureAIScript(pCreature)
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
        spells[0].info = dbcSpell.LookupEntry(VEIL_OF_SHADOW);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 10.0f;
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



// Naxxramas Cultist AI

#define CN_NAXX_CULTIST 15980

#define KNOCKBACK 10689

class NaxxCultistAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(NaxxCultistAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    NaxxCultistAI(Creature* pCreature) : CreatureAIScript(pCreature)
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
        spells[0].info = dbcSpell.LookupEntry(KNOCKBACK);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 10.0f;
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



// Necro Stalker AI - Venom Stalker as same ai

#define CN_NECRO_STALKER 16453
#define VENOM_STALKER 15976

#define POISON_CHARGE 28431

class NecroStalkerAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(NecroStalkerAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    NecroStalkerAI(Creature* pCreature) : CreatureAIScript(pCreature)
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
        spells[0].info = dbcSpell.LookupEntry(POISON_CHARGE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 10.0f;
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

// Tomb Horror AI

#define CN_TOMB_HORROR 15979

#define SPIKE_VOLLEY 28615

class TombHorrorAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(TombHorrorAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    TombHorrorAI(Creature* pCreature) : CreatureAIScript(pCreature)
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
        spells[0].info = dbcSpell.LookupEntry(SPIKE_VOLLEY);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 10.0f;
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



// Necropolis Acolyte AI

#define CN_NECRO_ACOLYTE 16368

#define SHADOWBOLT_VOLLEY 27831
#define ARCANE_EXPLOSION 26643

class NecroAcolyteAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(NecroAcolyteAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    NecroAcolyteAI(Creature* pCreature) : CreatureAIScript(pCreature)
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
        spells[0].info = dbcSpell.LookupEntry(SHADOWBOLT_VOLLEY);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 4000; // 1sec

		spells[1].info = dbcSpell.LookupEntry(ARCANE_EXPLOSION);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].perctrigger = 10.0f;
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



// Maexxna AI *BOSS*
#define CN_MAEXXNA 15952

#define WEB_WRAP_MAEXX 28619 // 1 target
#define WEB_SPRAY 29484 // various
#define POISON_SHOCK 28741 // various
#define NECROTIC_POISON 28776 // 1 target


class MaexxnaAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(MaexxnaAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    MaexxnaAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 4;

		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
		// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 
        spells[0].info = dbcSpell.LookupEntry(WEB_WRAP_MAEXX);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 0.0f;	//5.0f;
		spells[0].attackstoptimer = 4000; // 1sec

		spells[1].info = dbcSpell.LookupEntry(WEB_SPRAY);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = false;
		spells[1].perctrigger = 7.0f;
		spells[1].attackstoptimer = 1000; // 1sec

		spells[2].info = dbcSpell.LookupEntry(POISON_SHOCK);
		spells[2].targettype = TARGET_VARIOUS;
		spells[2].instant = false;
		spells[2].perctrigger = 5.0f;
		spells[2].attackstoptimer = 1000; // 1sec

		spells[3].info = dbcSpell.LookupEntry(NECROTIC_POISON);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = false;
		spells[3].perctrigger = 10.0f;
		spells[3].attackstoptimer = 1000; // 1sec

		Enraged = false;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
        RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		Enraged = false;
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
		if (_unit->GetHealthPct() <= 30 && !Enraged && _unit->GetCurrentSpell() == NULL)
		{
			_unit->CastSpell(_unit, 28747, true);

			Enraged = true;
			return;
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

	bool Enraged;
	int nrspells;
};

// Grand Widow Faerlina AI

// 8794,'A_FAER_NAXX_AGGRO01'
// 8795,'A_FAER_NAXX_AGGRO02'
// 8796,'A_FAER_NAXX_AGGRO03'
// 8797,'A_FAER_NAXX_AGGRO04'
// 8798,'A_FAER_NAXX_DEATH'
// 8799,'A_FAER_NAXX_GREET'
// 8800,'A_FAER_NAXX_SLAY01'
// 8801,'A_FAER_NAXX_SLAY02'

#define CN_WIDOW_FAERLINA 15953

#define POISON_BOLT_VOLLEY 25991 // various
#define RAIN_OF_FIRE 38635 // dest

class WidowFaerlinaAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(WidowFaerlinaAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    WidowFaerlinaAI(Creature* pCreature) : CreatureAIScript(pCreature)
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
        spells[0].info = dbcSpell.LookupEntry(POISON_BOLT_VOLLEY);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000; // 1sec

		spells[1].info = dbcSpell.LookupEntry(RAIN_OF_FIRE);
		spells[1].targettype = TARGET_DESTINATION;
		spells[1].instant = false;
		spells[1].perctrigger = 5.0f;
		spells[1].attackstoptimer = 7000; // 1sec

    }
    
    void OnCombatStart(Unit* mTarget)
    {
        RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Slay them in the master's name!");
		_unit->PlaySoundToSet(8794);
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

	void OnTargetDied(Unit* mTarget)
    { 
		if(Rand(25.0f))
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You have failed!");
			_unit->PlaySoundToSet(8800);
		}
		else
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Pathetic wretch!");
			_unit->PlaySoundToSet(8801);
		}
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The master... will avenge me!");
        _unit->PlaySoundToSet(8798);

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

// Anub'Rekhan
#define CN_ANUB 15956

// 8785,'A_ANU_NAXX_AGGRO01'
// 8786,'A_ANU_NAXX_AGGRO02'
// 8787,'A_ANU_NAXX_AGGRO03'
// 8788,'A_ANU_NAXX_GREET'
// 8789,'A_ANU_NAXX_SLAY'
// 8790,'A_ANU_NAXX_TAUNT01'
// 8791,'A_ANU_NAXX_TAUNT02'
// 8792,'A_ANU_NAXX_TAUNT03'
// 8793,'A_ANU_NAXX_TAUNT04'

#define IMPALE 28783 // various, wicked spell :P
#define LOCUST_SWARM 28785 // target

class AnubAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(AnubAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    AnubAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 2;

		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------

		tountcooldown = 10;
		tountcont = 0;
		m_attackstart = false;
	
		// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 
        spells[0].info = dbcSpell.LookupEntry(IMPALE);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 3000; // 1sec

		spells[1].info = dbcSpell.LookupEntry(LOCUST_SWARM);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].perctrigger = 5.0f;
		spells[1].attackstoptimer = 1000; // 1sec

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		m_attackstart = true;
        RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		if(Rand(25.0f))
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Just a little taste...");
			_unit->PlaySoundToSet(8785);
		}
		else if(Rand(25.0f))
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "There is no way out.");
			_unit->PlaySoundToSet(8786);
		}
		else
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Yes, run! It makes the blood pump faster!");
			_unit->PlaySoundToSet(8787);
		}

			
    }

    void OnCombatStop(Unit *mTarget)
    {
		m_attackstart = false;
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

	void OnTargetDied(Unit* mTarget)
    { 
        _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Shhh... it will all be over soon.");
        _unit->PlaySoundToSet(8789);
    }

    void OnDied(Unit * mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);

		if(!m_attackstart)
		{
			if(!tountcooldown)
			{
				tountcooldown = 10;
				
				switch(tountcont)
				{
					case 0:
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I hear little hearts beating. Yesss... beating faster now. Soon the beating will stop.");
						_unit->PlaySoundToSet(8790);
					case 1:
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Where to go? What to do? So many choices that all end in pain, end in death.");
						_unit->PlaySoundToSet(8791);
					case 2:
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Which one shall I eat first? So difficult to choose... they all smell so delicious.");
						_unit->PlaySoundToSet(8792);
					case 3:
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Closer now... tasty morsels. I've been too long without food. Without blood to drink.");
						_unit->PlaySoundToSet(8793);

				}
				tountcont++;
				if(tountcont >= 4)
					tountcont = 0;
			}
			tountcooldown--;
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

    bool m_attackstart;
	int nrspells;
	uint32 tountcooldown,tountcont;
};





// ---- Abomination Wing ----

// Bile Retcher AI
#define CN_BILE_RETCHER 16018

#define BILE_RETCHER_SLAM 27862 //various
#define BILE_VOMIT 27807 //various

class BileRetcherAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(BileRetcherAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    BileRetcherAI(Creature* pCreature) : CreatureAIScript(pCreature)
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
        spells[0].info = dbcSpell.LookupEntry(BILE_RETCHER_SLAM);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 2000; // 1sec
		
		spells[1].info = dbcSpell.LookupEntry(BILE_VOMIT);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = false;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 3000; // 1sec

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


// Patchwork Golem AI
#define CN_PATCHWORK_GOLEM 16017

#define DISEASE_CLOUD 28362
#define WAR_STOMP 33707

class PatchworkGolemAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(PatchworkGolemAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    PatchworkGolemAI(Creature* pCreature) : CreatureAIScript(pCreature)
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
        spells[0].info = dbcSpell.LookupEntry(DISEASE_CLOUD);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 2000; // 1sec
		
		spells[1].info = dbcSpell.LookupEntry(WAR_STOMP);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = false;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 2000; // 1sec

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


// Stitched Spewer AI
#define STICKED_SPEWER 16025

#define CN_SLIME_BOLT 32309
#define CN_UPPERCUT 26007

class StickedSpewerAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(StickedSpewerAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    StickedSpewerAI(Creature* pCreature) : CreatureAIScript(pCreature)
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
        spells[0].info = dbcSpell.LookupEntry(CN_SLIME_BOLT);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 2000; // 1sec
		
		spells[1].info = dbcSpell.LookupEntry(CN_UPPERCUT);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = false;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 2000; // 1sec

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



//Surgical Assistant AI
#define CN_SURGICAL_ASSISTANT 16022

#define MIND_FLAY 28310

class SurgicalAssistantAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(SurgicalAssistantAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    SurgicalAssistantAI(Creature* pCreature) : CreatureAIScript(pCreature)
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
        spells[0].info = dbcSpell.LookupEntry(MIND_FLAY);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 20.0f;
		spells[0].attackstoptimer = 6000; // 1sec

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


// Sludge Belcher AI
#define CN_SLUDGE_BELCHER 16029

#define DISEISE_BUFFET 27891
#define SUMMON_BILE_SLIMES 27889 // TODO: GAWD :P

class SludgeBelcherAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(SludgeBelcherAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    SludgeBelcherAI(Creature* pCreature) : CreatureAIScript(pCreature)
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
        spells[0].info = dbcSpell.LookupEntry(DISEISE_BUFFET);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 20.0f;
		spells[0].attackstoptimer = 1000; // 1sec
		
		spells[1].info = dbcSpell.LookupEntry(SUMMON_BILE_SLIMES);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = false;
		spells[1].perctrigger = 5.0f;
		spells[1].attackstoptimer = 3000; // 1sec

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

// BOSS'S

// Patchwerk AI
#define CN_PATCHWREK 16028

#define HATEFULL_STRIKE 28308

class PatchwerkAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(PatchwerkAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    PatchwerkAI(Creature* pCreature) : CreatureAIScript(pCreature)
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
        spells[0].info = dbcSpell.LookupEntry(HATEFULL_STRIKE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 35.0f;
		spells[0].attackstoptimer = 3000; // 1sec

    }
    
    void OnCombatStart(Unit* mTarget)
    {
        RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));

		if(Rand(50.0f))
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Patchwerk want to play.");
			_unit->PlaySoundToSet(8909);
		}
		else
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Kel'Thuzad make Patchwerk his avatar of war!");
			_unit->PlaySoundToSet(8910);
		}
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

	void OnTargetDied(Unit* mTarget)
    { 
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "No more play?");
		_unit->PlaySoundToSet(8912);
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "What happened to... Patch...");
		_unit->PlaySoundToSet(8911);
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

// Grobbulus AI
#define CN_GROBBULUS 15931

#define POISON_CLOUD_GROB 31259 // self
#define SLIME_SPRAY 28157 // various
#define SUMMON_FALLOUT_SLIME 28218 // TODO: cast on slime sprayd targets
#define MUTATING_INJECTION 28169 // TODO: DUMMY AURA

class GrobbulusAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(GrobbulusAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    GrobbulusAI(Creature* pCreature) : CreatureAIScript(pCreature)
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
        spells[0].info = dbcSpell.LookupEntry(POISON_CLOUD_GROB);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = false;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000; // 1sec

		spells[1].info = dbcSpell.LookupEntry(SLIME_SPRAY);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = false;
		spells[1].perctrigger = 15.0f;
		spells[1].attackstoptimer = 3000; // 1sec

    }
    
    void OnCombatStart(Unit* mTarget)
    {
        RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->CastSpell(_unit, spells[0].info, spells[0].instant);
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


// Gluth AI
#define CN_GLUTH 15932

#define MORTAL_WOUND 28308 // target
#define DECIMATE 28374 // TODO: needs to be scripted
#define TERRIFYING_ROAR 37939
#define FRENZY 38664 // self

class GluthAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(GluthAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    GluthAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 3;

		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
		// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 
        spells[0].info = dbcSpell.LookupEntry(MORTAL_WOUND);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 15.0f;
		spells[0].attackstoptimer = 2000; // 1sec

		spells[1].info = dbcSpell.LookupEntry(TERRIFYING_ROAR);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = false;
		spells[1].perctrigger = 5.0f;
		spells[1].attackstoptimer = 3000; // 1sec

		spells[2].info = dbcSpell.LookupEntry(FRENZY);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = false;
		spells[2].perctrigger = 5.0f;
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

	void OnTargetDied(Unit* mTarget)
    { 
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

// ---- Plague Wing ----

// Infectious Ghoul AI
#define CN_INFECTIOUS_GHOUL 16244

#define ENRAGE 8269 // self
//TODO: Melee Chain Cleave - Strikes enemies in front of the Construct, chaining to anyone in melee range of the targets.

class InfGhoulAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(InfGhoulAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    InfGhoulAI(Creature* pCreature) : CreatureAIScript(pCreature)
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
        spells[0].info = dbcSpell.LookupEntry(ENRAGE);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = false;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000; // 1sec
		m_spellcheck[0] = true;

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
		if(_unit->GetHealthPct() <= 50 && m_spellcheck[0])
        {
			// cast divine
			_unit->CastSpell(_unit, spells[0].info, spells[0].instant);
            m_spellcheck[0] = false;
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

// Stoneskin Gargoyle AI
#define CN_STONEKIN_GARGOYLE 16168

#define STONEKIND 28995
#define ACID_VOLLEY 29325

class StonekinGargoyleAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(StonekinGargoyleAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    StonekinGargoyleAI(Creature* pCreature) : CreatureAIScript(pCreature)
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
        spells[0].info = dbcSpell.LookupEntry(STONEKIND);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = false;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000; // 1sec
		m_spellcheck[0] = true;

		spells[1].info = dbcSpell.LookupEntry(ACID_VOLLEY);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = false;
		spells[1].perctrigger = 17.0f;
		spells[1].attackstoptimer = 2000; // 1sec

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
		if(_unit->GetHealthPct() <= 30 && m_spellcheck[0])
        {
			// cast divine
			_unit->CastSpell(_unit, spells[0].info, spells[0].instant);
            m_spellcheck[0] = false;
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


// Eye Stalk AI
#define CN_EYE_STALK 16244

#define MIND_FLAY_EYE 29570 // target
//TODO: Melee Chain Cleave - Strikes enemies in front of the Construct, chaining to anyone in melee range of the targets.

class EyeStalkAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(EyeStalkAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    EyeStalkAI(Creature* pCreature) : CreatureAIScript(pCreature)
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
        spells[0].info = dbcSpell.LookupEntry(MIND_FLAY_EYE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 100.0f;
		spells[0].attackstoptimer = 7000; // 1sec
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

// ---- Deathknight Wing ----

// Bony Construct AI
#define CN_BONY_CONSTRUCT 16167

#define SWEEPING_SLAM 25322
//TODO: Melee Chain Cleave - Strikes enemies in front of the Construct, chaining to anyone in melee range of the targets.

class BonyConstructAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(BonyConstructAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    BonyConstructAI(Creature* pCreature) : CreatureAIScript(pCreature)
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
        spells[0].info = dbcSpell.LookupEntry(SWEEPING_SLAM);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 15.0f;
		spells[0].attackstoptimer = 2000; // 1sec

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



// Death Lord AI
#define CN_DEATH_LORD 16861

#define AURA_OF_AGONY 28413

class DeathLordAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(DeathLordAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    DeathLordAI(Creature* pCreature) : CreatureAIScript(pCreature)
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
        spells[0].info = dbcSpell.LookupEntry(AURA_OF_AGONY);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 15.0f;
		spells[0].attackstoptimer = 2000; // 1sec

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



// Deathknight Captain AI
#define DEATHKNIGHT_CAPTAIN 16145

#define WIRLMWIND 36175

class DeathknightCaptainAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(DeathknightCaptainAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    DeathknightCaptainAI(Creature* pCreature) : CreatureAIScript(pCreature)
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
        spells[0].info = dbcSpell.LookupEntry(WIRLMWIND);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 6000; // 1sec

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

// -- BOSS'S --

// Instructor Razuvious AI
#define CN_INSTRUCTOR_RAZUVIOUS 16061

#define UNBALANCING_STRIKE 26613
#define DISRUPTING_SHOUT 29107

class RazuviousAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(RazuviousAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    RazuviousAI(Creature* pCreature) : CreatureAIScript(pCreature)
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
        spells[0].info = dbcSpell.LookupEntry(DISRUPTING_SHOUT);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 15.0f;
		spells[0].attackstoptimer = 3000; // 1sec

		spells[1].info = dbcSpell.LookupEntry(UNBALANCING_STRIKE);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].perctrigger = 35.0f;
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

	void OnTargetDied(Unit* mTarget)
    { 
		if(Rand(50.0f))
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You should've stayed home!");
			_unit->PlaySoundToSet(8862);
		}
		else
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You disappoint me, students!");
			_unit->PlaySoundToSet(8863);
		}
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "An honorable... death...");
		_unit->PlaySoundToSet(8860);
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
// - The Four Horsemen: -

#define SHIELD_WALL 31731

// Highlord Mograine AI
#define CN_HIGHLORD_MOGRAINE 16062

#define MARK_OF_MOGRAINE 28834 // various
#define SUMMON_SPIRIT_OF_MOGRAINE 28928 // TODO: SUMMON WILD EFFECT
#define RIGHTHEOUS_FIRE 28881 // self

class MograineAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(MograineAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    MograineAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 4;

		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------

		tountcooldown = 10;
		tountcont = 0;
		m_attackstart = false;
		
		// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 
        spells[0].info = dbcSpell.LookupEntry(MARK_OF_MOGRAINE);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 1000; // 1sec

		spells[1].info = dbcSpell.LookupEntry(SUMMON_SPIRIT_OF_MOGRAINE);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = false;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000; // 1sec

		spells[2].info = dbcSpell.LookupEntry(SHIELD_WALL);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = false;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000; // 1sec
		m_spellcheck[1] = true;
		m_spellcheck[2] = true;


		spells[3].info = dbcSpell.LookupEntry(RIGHTHEOUS_FIRE);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = false;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000; // 1sec

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		m_attackstart = true;
		if(Rand(50.0f))
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You seek death!");
			_unit->PlaySoundToSet(8835);
		}
		else if(Rand(50.0f))
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Non shall pass!");
			_unit->PlaySoundToSet(8836);
		}
		else
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Be still!");
			_unit->PlaySoundToSet(8837);
		}

		_unit->CastSpell(_unit, spells[3].info, spells[3].instant);
        RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

    void OnCombatStop(Unit *mTarget)
    {
		m_attackstart = false;
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

	void OnTargetDied(Unit* mTarget)
    { 
		if(Rand(50.0f))
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You will find no peace in death!");
			_unit->PlaySoundToSet(8839);
		}
		else
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The master's will is done!");
			_unit->PlaySoundToSet(8840);
		}
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "i am released.");
		_unit->PlaySoundToSet(8838);
	   _unit->CastSpell(_unit, spells[1].info, spells[1].instant);
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		if(_unit->GetHealthPct() <= 50 && m_spellcheck[1])
        {
			// cast divine
			_unit->CastSpell(_unit, spells[2].info, spells[2].instant);
            m_spellcheck[1] = false;
		}
		else if(_unit->GetHealthPct() <= 20 && m_spellcheck[2])
        {
			// cast divine
			_unit->CastSpell(_unit, spells[2].info, spells[2].instant);
            m_spellcheck[2] = false;
		}

		if(!m_attackstart)
		{
			if(!tountcooldown)
			{
				tountcooldown = 10;
				
				switch(tountcont)
				{
					case 0:
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "enough pratteling. let them come! we shall grind their bones to dust!");
						_unit->PlaySoundToSet(8842);
					case 1:
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Conserve your anger! Harness your rage! You will all have outlets for your frustration soon enough!");
						_unit->PlaySoundToSet(8843);
					case 2:
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Life is meaningless. It is in death that we are truly tested.");
						_unit->PlaySoundToSet(8844);

				}
				tountcont++;
				if(tountcont >= 3)
					tountcont = 0;
			}
			tountcooldown--;
		}
		if(m_spellcheck[0])
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "bow to the might of the highlord!");
			_unit->PlaySoundToSet(8841);
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

    bool m_attackstart;
	uint32 tountcooldown,tountcont;
	int nrspells;
};

/*  
8836 - Non shall pass
8835 - You seek death
8837 - Be still!
8838 - i am released
8839 - You will find no peace in death
8840 - The master's will is done
8841 - bow to the might of the highlord!
8842 - enough pratteling. let them come! we shall grind their bones to dust
8843 - Conserve your anger! Harness your rage! You will all have outlets for your frustration soon enough
8844 - Life is meaningless. It is in death that we are truly tested.

8835, 'A_MOG_NAXX_AGGRO01'
8836, 'A_MOG_NAXX_AGGRO02'
8837, 'A_MOG_NAXX_AGGRO03'
8838, 'A_MOG_NAXX_DEATH'
8839, 'A_MOG_NAXX_SLAY01'
8840, 'A_MOG_NAXX_SLAY02'
8841, 'A_MOG_NAXX_SPECIAL'
8842, 'A_MOG_NAXX_TAUNT01'
8843, 'A_MOG_NAXX_TAUNT02'
8844, 'A_MOG_NAXX_TAUNT03'
*/

// Thane Korth'azz AI
#define CN_THANE_KORTHAZZ 16064

#define MARK_OF_KORTHAZZ 28832
#define SUMMON_SPIRIT_OF_KORTHAZZ 28932
#define METEOR 35181 // 1 target

class KorthazzAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(KorthazzAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    KorthazzAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 4;

		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------

		tountcooldown = 6;
		tountcont = 0;
		m_attackstart = false;
		
		// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 
        spells[0].info = dbcSpell.LookupEntry(MARK_OF_KORTHAZZ);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 1000; // 1sec

		spells[1].info = dbcSpell.LookupEntry(SUMMON_SPIRIT_OF_KORTHAZZ);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = false;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000; // 1sec

		spells[2].info = dbcSpell.LookupEntry(SHIELD_WALL);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = false;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000; // 1sec
		m_spellcheck[1] = true;
		m_spellcheck[2] = true;


		spells[3].info = dbcSpell.LookupEntry(METEOR);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = false;
		spells[3].perctrigger = 15.0f;
		spells[3].attackstoptimer = 3000; // 1sec

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		m_attackstart = true;
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Come out and fight ye wee ninny!");
		_unit->PlaySoundToSet(8899);

        RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

    void OnCombatStop(Unit *mTarget)
    {
		m_attackstart = false;
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

	void OnTargetDied(Unit* mTarget)
    { 
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "next time. bring more friends!");
		_unit->PlaySoundToSet(8901);
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "what a bloody waste this is!");
		_unit->PlaySoundToSet(8900);
	   _unit->CastSpell(_unit, spells[1].info, spells[1].instant);
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		if(_unit->GetHealthPct() <= 50 && m_spellcheck[1])
        {
			// cast divine
			_unit->CastSpell(_unit, spells[2].info, spells[2].instant);
            m_spellcheck[1] = false;
		}
		else if(_unit->GetHealthPct() <= 20 && m_spellcheck[2])
        {
			// cast divine
			_unit->CastSpell(_unit, spells[2].info, spells[2].instant);
            m_spellcheck[2] = false;
		}

		if(!m_attackstart)
		{
			if(!tountcooldown)
			{
				tountcooldown = 6;
				
				switch(tountcont)
				{
					case 0:
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "To arms, ye roustabouts! We've got company!");
						_unit->PlaySoundToSet(8903);
					case 1:
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I heard about enough of yer sniveling. Shut yer fly trap 'afore I shut it for ye!");
						_unit->PlaySoundToSet(8904);
					case 2:
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I'm gonna enjoy killin' these slack-jawed daffodils!");
						_unit->PlaySoundToSet(8905);

				}
				tountcont++;
				if(tountcont >= 3)
					tountcont = 0;
			}
			tountcooldown--;
		}

		if(m_spellcheck[0])
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "i like my meat extra crispy.");
			_unit->PlaySoundToSet(8902);
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

    bool m_attackstart;
	uint32 tountcooldown,tountcont;
	int nrspells;
};

/*
8899 - Come out and fight ye wee ninny!
8900 - what a bloody waste this is!
8901 - next time. bring more friend
8902 - i like my meat extra crispy
8903 - To arms, ye roustabouts! We've got company!
8904 - I heard about enough of yer sniveling. Shut yer fly trap 'afore I shut it for ye!
8905 - I'm gonna enjoy killin' these slack-jawed daffodils!

8899, 'A_KOR_NAXX_AGGRO'
8900, 'A_KOR_NAXX_DEATH'
8901, 'A_KOR_NAXX_SLAY'
8902, 'A_KOR_NAXX_SPECIAL'
8903, 'A_KOR_NAXX_TAUNT01'
8904, 'A_KOR_NAXX_TAUNT02'
8905, 'A_KOR_NAXX_TAUNT03'
*/

// Lady Blaumeux AI
#define CN_LADY_BLAUMEUX 16065

#define MARK_OF_BLAUMEUX 28833
#define SUMMON_SPIRIT_OF_BLAUMEUX 28931
#define VOID_ZONE 28863 // TODO: DUMMY PART

class BlaumeuxAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(BlaumeuxAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    BlaumeuxAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 4;

		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------

		tountcooldown = 16;
		tountcont = 0;
		m_attackstart = false;
		
		// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 
        spells[0].info = dbcSpell.LookupEntry(MARK_OF_BLAUMEUX);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 1000; // 1sec

		spells[1].info = dbcSpell.LookupEntry(SUMMON_SPIRIT_OF_BLAUMEUX);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = false;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000; // 1sec

		spells[2].info = dbcSpell.LookupEntry(SHIELD_WALL);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = false;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000; // 1sec
		m_spellcheck[1] = true;
		m_spellcheck[2] = true;


		spells[3].info = dbcSpell.LookupEntry(VOID_ZONE);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = false;
		spells[3].perctrigger = 15.0f;
		spells[3].attackstoptimer = 3000; // 1sec

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		m_attackstart = true;
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Defend yourself!");
		_unit->PlaySoundToSet(8892);

        RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

    void OnCombatStop(Unit *mTarget)
    {
		m_attackstart = false;
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

	void OnTargetDied(Unit* mTarget)
    { 
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Who's next?");
		_unit->PlaySoundToSet(8894);
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Tou...che.");
		_unit->PlaySoundToSet(8893);
	   _unit->CastSpell(_unit, spells[1].info, spells[1].instant);
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		if(_unit->GetHealthPct() <= 50 && m_spellcheck[1])
        {
			// cast divine
			_unit->CastSpell(_unit, spells[2].info, spells[2].instant);
            m_spellcheck[1] = false;
		}
		else if(_unit->GetHealthPct() <= 20 && m_spellcheck[2])
        {
			// cast divine
			_unit->CastSpell(_unit, spells[2].info, spells[2].instant);
            m_spellcheck[2] = false;
		}

		if(!m_attackstart)
		{
			if(!tountcooldown)
			{
				tountcooldown = 16;
				
				switch(tountcont)
				{
					case 0:
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Come, Zeliek, do not drive them out. Not before we've had our fun.");
						_unit->PlaySoundToSet(8896);
					case 1:
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I do hope they stay alive long enough for me to... introduce myself.");
						_unit->PlaySoundToSet(8897);
					case 2:
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The first kill goes to me! Anyone care to wager?");
						_unit->PlaySoundToSet(8898);

				}
				tountcont++;
				if(tountcont >= 3)
					tountcont = 0;
			}
			tountcooldown--;
		}

		if(m_spellcheck[0])
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your life is mine!");
			_unit->PlaySoundToSet(8895);
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

    bool m_attackstart;
	uint32 tountcooldown,tountcont;
	int nrspells;
};

/*
8892 - Defend yourself
8893 - Tou...che
8894 - Who's next?
8895 - Your life is mine!
8896 - Come, Zeliek, do not drive them out. Not before we've had our fun
8897 - I do hope they stay alive long enough for me to... introduce myself,
8898 - The first kill goes to me! Anyone care to wager?

8892, 'A_BLA_NAXX_AGGRO'
8893, 'A_BLA_NAXX_DEATH'
8894, 'A_BLA_NAXX_SLAY'
8895, 'A_BLA_NAXX_SPECIAL'
8896, 'A_BLA_NAXX_TAUNT01'
8897, 'A_BLA_NAXX_TAUNT02'
8898, 'A_BLA_NAXX_TAUNT03'
*/

// Sir Zeliek AI
#define CN_SIR_ZELIEK 16063

#define MARK_OF_ZELIEK 28835
#define SUMMON_SPIRIT_OF_ZELIEK 28934
#define HOLY_WRATH 32445 // 1target

class ZeliekAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(ZeliekAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    ZeliekAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 4;

		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------

		tountcooldown = 13;
		tountcont = 0;
		m_attackstart = false;
		
		// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 
        spells[0].info = dbcSpell.LookupEntry(MARK_OF_ZELIEK);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 1000; // 1sec

		spells[1].info = dbcSpell.LookupEntry(SUMMON_SPIRIT_OF_ZELIEK);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = false;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000; // 1sec

		spells[2].info = dbcSpell.LookupEntry(SHIELD_WALL);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = false;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000; // 1sec
		m_spellcheck[1] = true;
		m_spellcheck[2] = true;


		spells[3].info = dbcSpell.LookupEntry(HOLY_WRATH);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = false;
		spells[3].perctrigger = 15.0f;
		spells[3].attackstoptimer = 1000; // 1sec

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		m_attackstart = true;
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Flee before its too late!");
		_unit->PlaySoundToSet(8913);

        RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

    void OnCombatStop(Unit *mTarget)
    {
		m_attackstart = false;
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

	void OnTargetDied(Unit* mTarget)
    { 
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Forgive me!");
		_unit->PlaySoundToSet(8915);
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "it is... as it should be.");
		_unit->PlaySoundToSet(8914);
	   _unit->CastSpell(_unit, spells[1].info, spells[1].instant);
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		if(_unit->GetHealthPct() <= 50 && m_spellcheck[1])
        {
			// cast divine
			_unit->CastSpell(_unit, spells[2].info, spells[2].instant);
            m_spellcheck[1] = false;
		}
		else if(_unit->GetHealthPct() <= 20 && m_spellcheck[2])
        {
			// cast divine
			_unit->CastSpell(_unit, spells[2].info, spells[2].instant);
            m_spellcheck[2] = false;
		}

		if(!m_attackstart)
		{
			if(!tountcooldown)
			{
				tountcooldown = 13;
				
				switch(tountcont)
				{
					case 0:
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Invaders, cease this foolish venture at once! Turn away while you still can!");
						_unit->PlaySoundToSet(8917);
					case 1:
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Perhaps they will come to their senses, and run away as fast as they can!");
						_unit->PlaySoundToSet(8918);
					case 2:
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Do not continue! Turn back while there's still time!");
						_unit->PlaySoundToSet(8919);

				}
				tountcont++;
				if(tountcont >= 3)
					tountcont = 0;
			}
			tountcooldown--;
		}

		if(m_spellcheck[0])
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "i have no choice but to obey!");
			_unit->PlaySoundToSet(8916);
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

    bool m_attackstart;
	uint32 tountcooldown,tountcont;
	int nrspells;
};

// -- Sapphiron Encounter by M4ksiu -- //

// Settings

#define DISABLE_FROST_BREATH

// Frost Breath TrgiggerAI

#define CN_FROST_BREATH_TRIGGER		50010	// Flying "ball" of frost energy
#define CN_FROST_BREATH_TRIGGER2	50011	// Explosion
#define CN_FROST_BREATH_TRIGGER3	50012	// Wing Buffet
#define CN_CHILL_TRIGGER			50013	// Mass chill trigger (used, because we can't cast many aoe triggers on one character >_>)

// Air phase spells
#define FROST_BREATH		28524
#define FROST_BREATH_EFFECT	30101
#define FROST_BREATH_DAMAGE	29318

// Additional spells
#define SAPPHIRONS_WING_BUFFET	29328

struct Spawns
{
	float x;
	float y;
	float z;
	float o;
};

struct Spawns PhaseTwoWP[]=
{
	{  },
	{ 3520.820068f, -5233.799805f, 137.626007f, 4.553010f }
};

/*struct Spawns IceBlocks[]=	// Those are not blizzlike pos, because those blocks are spawned randomly
{
	{  },
	{ 3580.986084f, -5241.330078f, 137.627304f, 3.006957f },
	{ 3562.967285f, -5257.952148f, 137.860916f, 2.468959f },
	{ 3569.620850f, -5276.108398f, 137.582733f, 2.480744f },
	{ 3551.420410f, -5283.535156f, 137.731903f, 2.009505f },
	{ 3535.933594f, -5294.710938f, 138.080002f, 1.823366f },
	{ 3522.235107f, -5286.610352f, 138.115601f, 1.532768f },
	{ 3503.184814f, -5296.418945f, 138.111252f, 1.222535f },
	{ 3489.055664f, -5278.863770f, 138.119934f, 0.884814f },
	{ 3473.002686f, -5277.641602f, 137.733414f, 0.680609f },
	{ 3472.302734f, -5255.734863f, 137.755569f, 0.331107f },
	{ 3458.193848f, -5241.013672f, 137.566147f, 0.111195f },
	{ 3463.324463f, -5221.530273f, 137.634888f, 6.084152f },
	{ 3467.574219f, -5200.617676f, 137.559662f, 5.860314f },
	{ 3479.394775f, -5178.301758f, 140.904312f, 5.405583f },
	{ 3507.219727f, -5180.725098f, 140.625473f, 4.431685f },
	{ 3518.371338f, -5172.666504f, 142.269135f, 4.694800f },
	{ 3542.516846f, -5184.699707f, 140.655182f, 4.470973f },
	{ 3559.013916f, -5183.916016f, 140.899689f, 4.644558f },
	{ 3559.006592f, -5183.923340f, 140.895554f, 3.952624f },
	{ 3571.978760f, -5209.633301f, 137.671906f, 3.514374f }
};*/

struct Spawns IceBlocks[]=	// Those are not blizzlike pos, because those blocks are spawned randomly
{
	{  },
	{ 3580.986084f, -5241.330078f, 137.627304f, 3.006957f },
	{ 3562.967285f, -5257.952148f, 137.860916f, 2.468959f },
	{ 3569.620850f, -5276.108398f, 137.582733f, 2.480744f },

	{ 3535.933594f, -5294.710938f, 138.080002f, 1.823366f },
	{ 3522.235107f, -5286.610352f, 138.115601f, 1.532768f },
	{ 3503.184814f, -5296.418945f, 138.111252f, 1.222535f },
	
	{ 3473.002686f, -5277.641602f, 137.733414f, 0.680609f },
	{ 3472.302734f, -5255.734863f, 137.755569f, 0.331107f },
	{ 3458.193848f, -5241.013672f, 137.566147f, 0.111195f },

	{ 3467.574219f, -5200.617676f, 137.559662f, 5.860314f },
	{ 3479.394775f, -5178.301758f, 140.904312f, 5.405583f },
	{ 3507.219727f, -5180.725098f, 140.625473f, 4.431685f },

	{ 3542.516846f, -5184.699707f, 140.655182f, 4.470973f },
	{ 3559.013916f, -5183.916016f, 140.899689f, 4.644558f },
	{ 3559.006592f, -5183.923340f, 140.895554f, 3.952624f },

	{ 3551.420410f, -5283.535156f, 137.731903f, 2.009505f },
	{ 3489.055664f, -5278.863770f, 138.119934f, 0.884814f },
	{ 3463.324463f, -5221.530273f, 137.634888f, 6.084152f },
	{ 3518.371338f, -5172.666504f, 142.269135f, 4.694800f },
	{ 3571.978760f, -5209.633301f, 137.671906f, 3.514374f }
};

class FrostBreathTriggerAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(FrostBreathTriggerAI);

    FrostBreathTriggerAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		_unit->GetAIInterface()->MoveTo(PhaseTwoWP[1].x, PhaseTwoWP[1].y, PhaseTwoWP[1].z + 10.5f, PhaseTwoWP[1].o);
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		_unit->GetAIInterface()->m_moveFly = true;
		_unit->m_noRespawn = true;
		_unit->Despawn(7000, 0);

		RegisterAIUpdateEvent(1000);

		AICounter = 7;
    }
    
    void OnCombatStart(Unit* mTarget) {}

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
    }

    void OnDied(Unit * mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		_unit->CastSpell(_unit, FROST_BREATH_EFFECT, true);

		AICounter--;
		if (AICounter == 6)
			_unit->GetAIInterface()->MoveTo(PhaseTwoWP[1].x, PhaseTwoWP[1].y, PhaseTwoWP[1].z + AICounter * 1.5f, PhaseTwoWP[1].o);
		else
			_unit->GetAIInterface()->MoveTo(PhaseTwoWP[1].x, PhaseTwoWP[1].y, PhaseTwoWP[1].z, PhaseTwoWP[1].o);
	}
	
protected:

	int AICounter;
};

class FrostBreathTrigger2AI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(FrostBreathTrigger2AI);

    FrostBreathTrigger2AI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
#ifdef DISABLE_FROST_BREATH
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
#else
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
#endif
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->m_canMove = false;
		_unit->m_noRespawn = true;
		_unit->Despawn(8000, 0);

		_unit->CastSpell(_unit, FROST_BREATH, false);
	}

	void OnCombatStop(Unit *mTarget)
	{
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
    }

    void OnDied(Unit * mKiller)
    {
       RemoveAIUpdateEvent();
    }
};

class FrostBreathTrigger3AI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(FrostBreathTrigger3AI);

    FrostBreathTrigger3AI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
		_unit->CastSpell(_unit, SAPPHIRONS_WING_BUFFET, true);
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->m_canMove = false;
		_unit->m_noRespawn = true;

		RegisterAIUpdateEvent(1000);
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
    }

    void OnDied(Unit * mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		_unit->CastSpell(_unit, SAPPHIRONS_WING_BUFFET, true);
	}
};

class ChillTriggerAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(ChillTriggerAI);

    ChillTriggerAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		_unit->CastSpellAoF(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), dbcSpell.LookupEntry(28547), true);
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->m_canMove = false;
		_unit->m_noRespawn = true;
		_unit->Despawn(15000, 0);
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
    }
};

// SapphironAI
// Missing spawning effect with building skeleton of Sappiron
#define CN_SAPPHIRON	15989

// Land phase spells
#define LIFE_DRAIN					28542
#define CHILL						28547
#define FROST_AURA					28531

// Air phase spells
#define ICEBOLT						28522

// Additional spells
#define SAPPHIRON_DIES				29357
#define BERSERK						26662	// 28498 - casts frostbolt (would be cool for Sapphiron), but every 2 sec for 5 min (~16k dmg per hit);
											// 27680 - 10 mins instead 5 mins
// Researches
#define SAPPHIRON_BIRTH				181356
#define FROSTWYRM_WATERFALL_DOOR	181225
#define ICE_BLOCK_GO				181247

// Move types
#define WALK	0
#define RUN		256
#define FLY		768

// Immunities
#define IMMUNITY_DISEASE			6681
#define IMMUNITY_SHADOW				7743
#define IMMUNITY_FROST				7940
#define IMMUNITY_NATURE				7941
#define IMMUNITY_FIRE				7942
#define IMMUNITY_HOLY				34182
#define IMMUNITY_ARCANE				34184
#define IMMUNITY_PHYSICAL			34310

class SapphironAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(SapphironAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    SapphironAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(1, 3000, RUN));

		nrspells = 2;
		for(int i = 0; i < nrspells; i++)
		{
			m_spellcheck[i] = false;
			spells[i].casttime = 0;
		}
		
        spells[0].info = dbcSpell.LookupEntry(LIFE_DRAIN);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].cooldown = 20;
		spells[0].attackstoptimer = 2000;

		spells[1].info = dbcSpell.LookupEntry(CHILL);
		spells[1].targettype = TARGET_RANDOM_DESTINATION;
		spells[1].instant = true;
		spells[1].perctrigger = 10.0f;
		spells[1].cooldown = 15;
		spells[1].attackstoptimer = 1000;
		spells[1].mindist2cast = 0.0f;
		spells[1].maxdist2cast = 40.0f;
		spells[1].minhp2cast = 0;
		spells[1].maxhp2cast = 100;

		spells[2].info = dbcSpell.LookupEntry(ICEBOLT);
		spells[2].targettype = TARGET_RANDOM_SINGLE;
		spells[2].instant = true;
		spells[2].perctrigger = 0.0f;
		spells[2].cooldown = 0;
		spells[2].attackstoptimer = 1000;
		spells[2].mindist2cast = 0.0f;
		spells[2].maxdist2cast = 70.0f;
		spells[2].minhp2cast = 0;
		spells[2].maxhp2cast = 100;
		
		spells[3].info = dbcSpell.LookupEntry(BERSERK);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = true;
		spells[3].perctrigger = 0.0f;
		spells[3].cooldown = 900;
		spells[3].attackstoptimer = 1000;

		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
		_unit->GetAIInterface()->m_moveFly = false;
		_unit->GetAIInterface()->m_canMove = true;
		_unit->CastSpell(_unit, IMMUNITY_FROST, true);

		WorldPacket data(SMSG_MOVE_UNSET_HOVER, 13);
		data << _unit->GetNewGUID();
		data << uint32(0);
		_unit->SendMessageToSet(&data, false);

		ChillCounter = NULL;
		FlightActions = 0;
		ChillCounter = 0;
		PhaseTimer = 0;
		m_phase = 1;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		for(int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;

		spells[3].casttime = (uint32)time(NULL) + spells[3].cooldown;

		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
		_unit->GetAIInterface()->m_moveFly = false;
		_unit->GetAIInterface()->m_canMove = true;

		WorldPacket data(SMSG_MOVE_UNSET_HOVER, 13);
		data << _unit->GetNewGUID();
		data << uint32(0);
		_unit->SendMessageToSet(&data, false);

		GameObject *Waterfall = NULL;
		Waterfall = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(3536.852783f, -5159.951172f, 143.636139f, FROSTWYRM_WATERFALL_DOOR);
		if (Waterfall != NULL)
		{
			Waterfall->SetUInt32Value(GAMEOBJECT_STATE, 1);
		}

		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));

		PhaseTimer = (uint32)time(NULL) + 35;
		ChillCounter = NULL;
		FlightActions = 0;
		ChillCounter = 0;
		m_phase = 1;
    }

    void OnCombatStop(Unit *mTarget)
    {
		Creature *BreathTrigger = NULL;
		BreathTrigger = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(PhaseTwoWP[1].x, PhaseTwoWP[1].y, PhaseTwoWP[1].z, CN_FROST_BREATH_TRIGGER3);
		if (BreathTrigger != NULL)
			BreathTrigger->Despawn(0,0);

		for (int i = 1; i < 21; i++)
		{
			GameObject *IceBlock = NULL;
			IceBlock = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(IceBlocks[i].x, IceBlocks[i].y, IceBlocks[i].z, ICE_BLOCK_GO);
			if (IceBlock != NULL)
			{
				delete IceBlock;
			}
		}

		GameObject *Waterfall = NULL;
		Waterfall = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(3536.852783f, -5159.951172f, 143.636139f, FROSTWYRM_WATERFALL_DOOR);
		if (Waterfall != NULL)
		{
			Waterfall->SetUInt32Value(GAMEOBJECT_STATE, 0);
		}

		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
		_unit->GetAIInterface()->m_moveFly = false;
		_unit->GetAIInterface()->m_canMove = true;

		WorldPacket data(SMSG_MOVE_UNSET_HOVER, 13);
		data << _unit->GetNewGUID();
		data << uint32(0);
		_unit->SendMessageToSet(&data, false);

		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);

		RemoveAIUpdateEvent();
    }

	void OnTargetDied(Unit* mTarget)
	{
	}

    void OnDied(Unit * mKiller)
    {
		_unit->CastSpell(_unit, SAPPHIRON_DIES, true);

		RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		uint32 t = (uint32)time(NULL);
		if (t > spells[3].casttime && _unit->GetCurrentSpell() == NULL)
		{
			_unit->CastSpell(_unit, spells[3].info, spells[3].instant);

			spells[3].casttime = t + spells[3].cooldown;
		}

		switch (m_phase)
		{
		case 1:
			PhaseOne();
			break;
		case 2:
			PhaseTwo();
			break;
		default:
			{
			}
		}
    }

	void PhaseOne()
	{
		_unit->CastSpell(_unit, FROST_AURA, true);

		if (_unit->GetAIInterface()->getMoveType() == MOVEMENTTYPE_WANTEDWP)
			return;

		if (_unit->GetHealthPct() > 10)
		{
			uint32 t = (uint32)time(NULL);
			if (t > PhaseTimer)
			{
				if (_unit->GetCurrentSpell() != NULL)
					_unit->GetCurrentSpell()->cancel();

				_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
				_unit->GetAIInterface()->StopMovement(0);
				_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
				_unit->GetAIInterface()->setWaypointToMove(1);

				return;
			}
		}

		if (ChillCounter > 0)
		{
			ChillCounter--;
			if (ChillTarget != NULL)
			{
				_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CHILL_TRIGGER, ChillTarget->GetPositionX(), ChillTarget->GetPositionY(), ChillTarget->GetPositionZ(), ChillTarget->GetOrientation(), true, false, 0, 0);
			}

			if (ChillCounter == 0)
			{
				ChillTarget = NULL;
			}
		}

		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
	}

	void PhaseTwo()
	{
		if (FlightActions == 0)
		{
			_unit->GetAIInterface()->m_canMove = false;
			_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
			_unit->GetAIInterface()->setCurrentAgent(AGENT_SPELL);
			_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
			_unit->GetAIInterface()->setWaypointToMove(0);
		}

		if (FlightActions < 5)
		{
			if (_unit->GetCurrentSpell() == NULL)
			{
				if (_unit->GetAIInterface()->GetNextTarget() != NULL)
				{
					CastSpellOnRandomTarget(2, 0.0f, 40.0f, 0, 100);

					FlightActions++;
					if (FlightActions >= 5)
					{
						uint32 LastOne = 0;
						for (int i = 0; i < 2; i++)
						{
							uint32 Block = 0;
							while (LastOne == Block)
							{
								Block = RandomUInt(5) + 15;
							}

							LastOne = Block;

							GameObject *IceBlock = NULL;
							IceBlock = _unit->GetMapMgr()->GetInterface()->SpawnGameObject(ICE_BLOCK_GO, IceBlocks[Block].x, IceBlocks[Block].y, IceBlocks[Block].z, IceBlocks[Block].o, true, 0, 0);
							if (IceBlock != NULL)
							{
								IceBlock->SetUInt32Value(GAMEOBJECT_FLAGS, 1);
							}
						}

						RemoveAIUpdateEvent();
						RegisterAIUpdateEvent(3000);

						FlightActions = 5;
					}

					if (FlightActions == 2)
					{
						for (int i = 0; i < 2; i++)
						{
							uint32 Block = 0;
							if (i == 0)
								Block = RandomUInt(3) + 1;
							else
								Block = RandomUInt(3) + 10;

							GameObject *IceBlock = NULL;
							IceBlock = _unit->GetMapMgr()->GetInterface()->SpawnGameObject(ICE_BLOCK_GO, IceBlocks[Block].x, IceBlocks[Block].y, IceBlocks[Block].z, IceBlocks[Block].o, true, 0, 0);
							if (IceBlock != NULL)
							{
								IceBlock->SetUInt32Value(GAMEOBJECT_FLAGS, 1);
							}
						}
					}

					if (FlightActions == 4)
					{
						for (int i = 0; i < 2; i++)
						{
							uint32 Block = 0;
							if (i == 0)
								Block = RandomUInt(3) + 7;
							else
								Block = RandomUInt(9) + 13;

							GameObject *IceBlock = NULL;
							IceBlock = _unit->GetMapMgr()->GetInterface()->SpawnGameObject(ICE_BLOCK_GO, IceBlocks[Block].x, IceBlocks[Block].y, IceBlocks[Block].z, IceBlocks[Block].o, true, 0, 0);
							if (IceBlock != NULL)
							{
								IceBlock->SetUInt32Value(GAMEOBJECT_FLAGS, 1);
							}
						}
					}
				}
			}
		}

		else
		{
			if (FlightActions == 5)
			{
				Unit *FlyingFrostBreath = NULL;
				FlyingFrostBreath =_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FROST_BREATH_TRIGGER, PhaseTwoWP[1].x, PhaseTwoWP[1].y, PhaseTwoWP[1].z + 18.0f, _unit->GetOrientation(), true, false, 0, 0);
				if (FlyingFrostBreath != NULL)
				{
					FlyingFrostBreath->GetAIInterface()->MoveTo(PhaseTwoWP[1].x, PhaseTwoWP[1].y, PhaseTwoWP[1].z, PhaseTwoWP[1].o);
				}

				_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FROST_BREATH_TRIGGER2, PhaseTwoWP[1].x, PhaseTwoWP[1].y, PhaseTwoWP[1].z, _unit->GetOrientation(), true, false, 0, 0);

				RemoveAIUpdateEvent();
				RegisterAIUpdateEvent(10000);

				Creature *BreathTrigger = NULL;
				BreathTrigger = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(PhaseTwoWP[1].x, PhaseTwoWP[1].y, PhaseTwoWP[1].z, CN_FROST_BREATH_TRIGGER3);
				if (BreathTrigger != NULL)
					BreathTrigger->Despawn(0,0);
			}

			if (FlightActions == 6)
			{
				for (int i = 1; i < 21; i++)
				{
					GameObject *IceBlock = NULL;
					IceBlock = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(IceBlocks[i].x, IceBlocks[i].y, IceBlocks[i].z, ICE_BLOCK_GO);
					if (IceBlock != NULL)
					{
						delete IceBlock;
					}
				}

				_unit->GetAIInterface()->m_moveFly = false;
				_unit->Emote(EMOTE_ONESHOT_LAND);

				WorldPacket data(SMSG_MOVE_UNSET_HOVER, 13);
				data << _unit->GetNewGUID();
				data << uint32(0);
				_unit->SendMessageToSet(&data, false);

				RemoveAIUpdateEvent();
				RegisterAIUpdateEvent(3000);
			}

			if (FlightActions == 7)
			{
				_unit->GetAIInterface()->m_canMove = true;
				_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
                _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
                _unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
                _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
                _unit->GetAIInterface()->setWaypointToMove(0);

				RemoveAIUpdateEvent();
				RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));

				PhaseTimer = (uint32)time(NULL) + 67;
				ChillTarget = NULL;
				FlightActions = 0;
				ChillCounter = 0;
				m_phase = 1;
			}

			FlightActions++;
		}
	}

	void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget() != NULL)
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for (int i = 0; i < nrspells; i++)
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
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) && t > spells[i].casttime)
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

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget() != NULL)
        {
			std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (isHostile(_unit, (*itr)) && (*itr) != _unit && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID())
				{
					Unit* RandomTarget = NULL;
					RandomTarget = (Unit*)(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && _unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit * RTarget = TargetTable[RandTarget];

			if (RTarget == NULL)
				return;

			if (i == 1)
			{
				ChillCounter = RandomUInt(3) + 3;
				ChillTarget = RTarget;
			}

			else
			{
				switch (spells[i].targettype)
				{
				case TARGET_RANDOM_FRIEND:
				case TARGET_RANDOM_SINGLE:
					_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
				case TARGET_RANDOM_DESTINATION:
					_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
				}
			}

			TargetTable.clear();
		}
	}

	void OnReachWP(uint32 iWaypointId, bool bForwards)
    {
		if (iWaypointId == 1)
		{
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FROST_BREATH_TRIGGER3, PhaseTwoWP[1].x, PhaseTwoWP[1].y, PhaseTwoWP[1].z, _unit->GetOrientation(), true, false, 0, 0);
			_unit->GetAIInterface()->m_moveFly = true;
			_unit->Emote(EMOTE_ONESHOT_LIFTOFF);

			WorldPacket data(SMSG_MOVE_SET_HOVER, 13);
			data << _unit->GetNewGUID();
			data << uint32(0);
			_unit->SendMessageToSet(&data, false);

			RemoveAIUpdateEvent();
			RegisterAIUpdateEvent(3500);

			ChillTarget = NULL;
			FlightActions = 0;
			ChillCounter = 0;
			m_phase = 2;
		}
	}

	inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
    {
        WayPoint * wp = _unit->CreateWaypointStruct();
        wp->id = id;
        wp->x = PhaseTwoWP[id].x;
        wp->y = PhaseTwoWP[id].y;
        wp->z = PhaseTwoWP[id].z;
        wp->o = PhaseTwoWP[id].o;
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

	Unit *ChillTarget;	// I don't like it >_>

	uint32 FlightActions;
	uint32 ChillCounter;
	uint32 PhaseTimer;
	uint32 m_phase;
	int nrspells;
};

// -- Kel'thuzad Encounter by M4ksiu -- //

// Encounter mobs

#define CN_THE_LICH_KING 30002
#define CN_SOLDIER_OF_THE_FROZEN_WASTES 16427
#define CN_UNSTOPPABLE_ABOMINATION 16428
#define CN_SOUL_WEAVER 16429
#define CN_GUARDIAN_OF_ICECROWN 16441

/*
  _____
 /  K  \
/1     4\
|       |
|2     5|
|       |
\3     6/
 \_ 7 _/
   | |

*/

static Spawns SFrozenWastes[] =	// Soldier of the Frozen Wastes (no idea about those :| )
{
	{ 3759.149902f, -5074.879883f, 143.175003f, 1.203640f },	// 1
	{ 3762.959961f, -5067.399902f, 143.453003f, 0.893413f },
	{ 3772.419922f, -5076.379883f, 143.466995f, 3.606970f },
	{ 3779.699951f, -5078.180176f, 143.764008f, 4.038940f },
	{ 3770.219971f, -5065.740234f, 143.477005f, 0.630304f },
	{ 3765.709961f, -5060.799805f, 143.748001f, 1.608120f },
	{ 3776.909912f, -5066.100098f, 143.550003f, 5.130640f },
	{ 3782.659912f, -5069.529785f, 143.757004f, 5.150280f },
	{ 3773.909912f, -5059.589844f, 143.774002f, 6.257680f },
	{ 3780.260010f, -5061.580078f, 143.742996f, 5.169910f },
	{ 3721.429932f, -5052.759766f, 143.442993f, 1.457330f },	// 2
	{ 3732.149902f, -5051.589844f, 143.444000f, 1.017500f },
	{ 3741.889893f, -5047.439941f, 143.886002f, 3.075250f },
	{ 3726.229980f, -5043.410156f, 143.455994f, 6.150070f },
	{ 3718.679932f, -5042.520020f, 143.768005f, 1.614410f },
	{ 3733.060059f, -5040.979980f, 143.557007f, 1.669380f },
	{ 3741.860107f, -5038.410156f, 143.917999f, 4.410420f },
	{ 3736.189941f, -5032.810059f, 143.847000f, 5.026970f },
	{ 3723.219971f, -5035.770020f, 143.764999f, 4.701020f },
	{ 3728.760010f, -5031.759766f, 143.785995f, 3.723200f },
	{ 3683.189941f, -5062.419922f, 143.175995f, 0.559623f },	// 3
	{ 3687.739990f, -5057.779785f, 143.175995f, 1.345020f },
	{ 3674.040039f, -5067.899902f, 143.524994f, 0.005909f },
	{ 3688.340088f, -5052.009766f, 143.473007f, 2.299280f },
	{ 3681.659912f, -5052.549805f, 143.263000f, 3.343860f },
	{ 3678.840080f, -5055.529785f, 143.332993f, 3.512720f },
	{ 3685.800049f, -5045.779785f, 143.615997f, 6.253750f },
	{ 3673.479980f, -5053.350098f, 143.554993f, 1.945840f },
	{ 3670.649902f, -5061.149902f, 143.820007f, 4.266700f },
	{ 3688.018311f, -5061.541992f, 143.178223f, 0.512497f },
	{ 3742.659912f, -5157.009766f, 143.171005f, 0.773261f },	// 4
	{ 3750.310059f, -5153.779785f, 143.179993f, 5.603460f },
	{ 3739.770020f, -5163.000000f, 143.662994f, 1.095270f },
	{ 3746.649902f, -5164.560059f, 143.520004f, 1.970990f },
	{ 3748.479980f, -5169.109863f, 143.649002f, 2.591460f },
	{ 3744.560059f, -5173.520020f, 143.895996f, 1.370160f },
	{ 3758.260010f, -5156.299805f, 143.570999f, 6.192510f },
	{ 3762.620117f, -5159.149902f, 143.712997f, 4.433220f },
	{ 3760.840088f, -5162.959961f, 143.649002f, 4.280060f },
	{ 3756.580078f, -5170.060059f, 143.623001f, 3.031280f },
	{ 3705.229980f, -5163.189941f, 143.266998f, 5.692200f },	// 5
	{ 3696.949951f, -5163.370117f, 143.608002f, 0.901274f },
	{ 3710.080078f, -5168.709961f, 143.585999f, 4.675110f },
	{ 3697.030029f, -5170.220215f, 143.606003f, 0.343642f },
	{ 3692.030029f, -5170.020020f, 143.815994f, 0.932691f },
	{ 3710.320068f, -5175.319824f, 143.705002f, 4.639770f },
	{ 3694.550049f, -5177.540039f, 143.839005f, 1.549230f },
	{ 3705.830078f, -5179.120117f, 143.729996f, 3.956480f },
	{ 3712.800049f, -5179.109863f, 143.891006f, 3.855950f },
	{ 3701.669922f, -5181.859863f, 143.804001f, 1.380370f },
	{ 3676.590088f, -5144.899902f, 143.186005f, 3.575550f },	// 6
	{ 3670.649902f, -5142.629883f, 143.173996f, 4.313820f },
	{ 3665.689941f, -5136.279785f, 143.177994f, 1.176150f },
	{ 3664.870117f, -5132.330078f, 143.205002f, 3.249600f },
	{ 3662.120117f, -5144.299805f, 143.320007f, 2.256080f },
	{ 3658.979980f, -5139.490234f, 143.421997f, 6.077040f },
	{ 3651.850098f, -5137.140137f, 143.764999f, 0.799161f },
	{ 3654.689941f, -5144.009766f, 143.546997f, 2.519180f },
	{ 3666.020020f, -5149.500000f, 143.587006f, 3.359560f },
	{ 3667.959961f, -5153.120117f, 143.791000f, 4.015380f },
	{ 3659.979980f, -5100.450195f, 143.453003f, 5.168254f },	// 7
	{ 3662.800049f, -5088.189941f, 143.557999f, 4.409650f },
	{ 3658.439941f, -5090.649902f, 143.470001f, 5.528840f },
	{ 3652.810059f, -5090.450195f, 143.475998f, 1.362310f },
	{ 3651.780029f, -5095.339844f, 143.388000f, 0.981387f },
	{ 3652.629883f, -5098.970215f, 143.544998f, 2.799580f },
	{ 3652.679932f, -5083.479980f, 143.774994f, 0.482659f },
	{ 3647.189941f, -5085.490234f, 143.570999f, 1.919940f },
	{ 3645.120117f, -5097.240234f, 143.487000f, 0.765403f },
	{ 3646.360107f, -5101.200195f, 143.681000f, 2.909540f }
};

static Spawns Abomination[] =	// Unstoppable Abomination
{
	{ 3776.229980f, -5081.439941f, 143.779999f, 4.043730f },	// 1
	{ 3774.419922f, -5071.490234f, 143.423996f, 4.214940f },
	{ 3759.850098f, -5064.479980f, 143.636002f, 6.255410f },
	{ 3740.899902f, -5052.740234f, 143.785995f, 1.836760f },	// 2
	{ 3726.919922f, -5040.020020f, 143.535995f, 2.481570f },
	{ 3715.010010f, -5049.259766f, 143.632004f, 5.071810f },
	{ 3695.060059f, -5052.160156f, 143.548004f, 3.792400f },	// 3
	{ 3678.129883f, -5048.060059f, 143.436996f, 5.363980f },
	{ 3676.120117f, -5061.359863f, 143.492004f, 5.726840f },
	{ 3756.780029f, -5149.419922f, 143.460007f, 2.774530f },	// 4
	{ 3752.262695f, -5164.782227f, 143.409119f, 0.383768f },
	{ 3740.669922f, -5168.600098f, 143.873993f, 1.051360f },
	{ 3714.020020f, -5171.129883f, 143.776993f, 1.742510f },	// 5
	{ 3702.185303f, -5174.303711f, 143.532303f, 5.443298f },
	{ 3693.739990f, -5162.149902f, 143.748001f, 5.696990f },
	{ 3673.189941f, -5150.500000f, 143.751999f, 3.030570f },	// 6
	{ 3658.570068f, -5147.799805f, 143.494003f, 1.230440f },
	{ 3659.560059f, -5132.129883f, 143.677002f, 5.988380f },
	{ 3651.130859f, -5104.800293f, 143.798248f, 5.374194f },	// 7
	{ 3646.947021f, -5092.266113f, 143.305878f, 5.005841f },
	{ 3658.618408f, -5083.832031f, 143.778641f, 5.951464f }
};

static Spawns SoulWeaver[] =	// Soul Weaver
{
	{ 3768.540039f, -5075.140137f, 143.203995f, 5.096160f },
	{ 3728.030029f, -5047.359863f, 143.306000f, 5.230460f },
	{ 3682.929932f, -5055.819824f, 143.184006f, 5.368690f },
	{ 3749.429932f, -5160.419922f, 143.283997f, 4.723090f },
	{ 3706.120117f, -5169.250000f, 143.436996f, 2.682630f },
	{ 3665.310059f, -5142.339844f, 143.220001f, 1.147180f },
	{ 3656.365234f, -5094.724121f, 143.306641f, 6.203571f }
};

static Spawns Guardians[] =		// Guardians of Icecrown
{
	{ 3778.371582f, -5065.141113f, 143.614639f, 3.700061f },
	{ 3731.733398f, -5032.681152f, 143.775040f, 4.485459f },
	{ 3758.592285f, -5170.157715f, 143.667297f, 2.144972f },
	{ 3700.936279f, -5183.230469f, 143.858582f, 1.314648f }
};

static Spawns Waves[] =			// Spawn positions of units that attack circle
{
	{ 3756.380615f, -5080.560059f, 142.906921f, 3.762599f },
	{ 3726.448242f, -5058.546387f, 142.467331f, 4.262112f },
	{ 3690.084229f, -5066.993164f, 142.705917f, 5.245427f },
	{ 3742.711670f, -5146.786133f, 142.964890f, 2.178441f },
	{ 3706.024902f, -5155.362793f, 142.655304f, 1.294868f },
	{ 3676.363281f, -5133.007324f, 142.806168f, 0.615499f },
	{ 3668.310303f, -5096.927246f, 142.307312f, 6.128994f }
};

// Kel'thuzad AI
// each ~10-20 sec new mob

#define CN_KELTHUZAD 15990

#define SFROSTBOLT 28478
#define MFROSTBOLT 28479
#define CHAINS_OF_KELTHUZAD 28410
#define DETONATE_MANA 27819
#define SHADOW_FISSURE 27810
#define FROST_BLAST 27808

#define KELTHUZAD_CHANNEL 29423

class KelthuzadAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(KelthuzadAI);
	SP_AI_Spell spells[7];
	bool m_spellcheck[7];
	bool FrozenWastes[7];
	bool Abominations[7];
	bool SoulWeavers[7];

    KelthuzadAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 6;

		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
			spells[i].casttime = 0;
		}

		for (int i = 0; i < 7; i++)
		{
			FrozenWastes[i] = false;
			Abominations[i] = false;
			SoulWeavers[i] = false;
		}
		
        spells[0].info = dbcSpell.LookupEntry(SFROSTBOLT);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 7.0f;
		spells[0].cooldown = 0;
		spells[0].attackstoptimer = 2000;

		spells[1].info = dbcSpell.LookupEntry(MFROSTBOLT);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].perctrigger = 0.1f;
		spells[1].cooldown = 15;			// it's casted after 15 sec anyway, so it does need additional perctrigger
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(CHAINS_OF_KELTHUZAD);
		spells[2].targettype = TARGET_RANDOM_SINGLE;
		spells[2].instant = true;
		spells[2].perctrigger = 5.0f;
		spells[2].cooldown = 20;
		spells[2].attackstoptimer = 1000;
		spells[2].mindist2cast = 0.0f;
		spells[2].maxdist2cast = 40.0f;
		spells[2].minhp2cast = 0;
		spells[2].maxhp2cast = 100;

		spells[3].info = dbcSpell.LookupEntry(DETONATE_MANA);
		spells[3].targettype = TARGET_RANDOM_SINGLE;
		spells[3].instant = true;
		spells[3].perctrigger = 6.0f;
		spells[3].cooldown = 7;
		spells[3].attackstoptimer = 2000;
		spells[3].mindist2cast = 0.0f;
		spells[3].maxdist2cast = 40.0f;
		spells[3].minhp2cast = 0;
		spells[3].maxhp2cast = 100;

		spells[4].info = dbcSpell.LookupEntry(SHADOW_FISSURE);
		spells[4].targettype = TARGET_ATTACKING;
		spells[4].instant = true;
		spells[4].perctrigger = 5.0f;
		spells[4].cooldown = 10;
		spells[4].attackstoptimer = 2000;

		spells[5].info = dbcSpell.LookupEntry(FROST_BLAST);
		spells[5].targettype = TARGET_RANDOM_SINGLE;
		spells[5].instant = true;
		spells[5].perctrigger = 6.0f;
		spells[5].cooldown = 10;
		spells[5].attackstoptimer = 2000;
		spells[5].soundid = 8815;
		spells[5].speech = "I shall freeze the blood in your veins!";	// not sure if it's to this one or to one of bolt spells
		spells[5].mindist2cast = 0.0f;
		spells[5].maxdist2cast = 40.0f;
		spells[5].minhp2cast = 0;
		spells[5].maxhp2cast = 100;

		spells[6].info = dbcSpell.LookupEntry(KELTHUZAD_CHANNEL);
		spells[6].targettype = TARGET_SELF;
		spells[6].instant = false;
		spells[6].perctrigger = 0.0f;
		spells[6].cooldown = 0;
		spells[6].attackstoptimer = 1000;

		_unit->GetAIInterface()->disable_melee = false;
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
		_unit->GetAIInterface()->m_canMove = true;

		DespawnTrash = false;
		EventStart = false;
		SpawnCounter = 0;
		PhaseTimer = 310;
		SpawnTimer = 0;
		GCounter = 0;
		m_phase = 0;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Minions, servants, soldiers of the cold dark, obey the call of Kel'Thuzad!");
		_unit->PlaySoundToSet(8819);

		Unit* TheLichKing = NULL;
		TheLichKing = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(3767.58f, -5117.15f, 174.49f, CN_THE_LICH_KING);
		if (TheLichKing != NULL)
		{
			_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, TheLichKing->GetGUID());
			_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, 29423);
		}

		GameObject* KelGate  = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(3635.44f, -5090.33f, 143.205f, 181228);
					
		if (KelGate)
			KelGate->SetUInt32Value(GAMEOBJECT_STATE, 1);

		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->m_canMove = false;
		
        RegisterAIUpdateEvent(1000);
		CastTime();

		DespawnTrash = false;
		EventStart = true;
		SpawnCounter = 0;
		PhaseTimer = 310;
		SpawnTimer = 0;
		GCounter = 0;
		m_phase = 1;
    }

    void OnCombatStop(Unit *mTarget)
    {
		GameObject* KelGate  = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(3635.44f, -5090.33f, 143.205f, 181228);
		if (KelGate != NULL)
			KelGate->SetUInt32Value(GAMEOBJECT_STATE, 0);

		for (int i = 0; i < 4; i++)
		{
			GameObject* WindowGate  = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(Guardians[i].x, Guardians[i].y, Guardians[i].z, 200002);		
			if (WindowGate != NULL)
				WindowGate->SetUInt32Value(GAMEOBJECT_STATE, 1);
		}

		_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
		_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		_unit->GetAIInterface()->disable_melee = false;
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
		_unit->GetAIInterface()->m_canMove = true;
        RemoveAIUpdateEvent();

		DespawnTrash = true;
		EventStart = false;
		SpawnCounter = 0;
		PhaseTimer = 310;
		SpawnTimer = 0;
		GCounter = 0;
		m_phase = 0;
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = 0;
	}

	void OnTargetDied(Unit* mTarget)
	{
		if (_unit->GetHealthPct() == 0) return;

		uint32 RandomSpeach = rand()%2;
		switch (RandomSpeach)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The dark void awaits you!");
			_unit->PlaySoundToSet(8817);
			break;
		case 1:
			//_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "");	// no text?
			_unit->PlaySoundToSet(8818);
			break;
		}
	}

    void OnDied(Unit * mKiller)
    {
		GameObject* KelGate  = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(3635.44f, -5090.33f, 143.205f, 181228);	
		if (KelGate != NULL)
			KelGate->SetUInt32Value(GAMEOBJECT_STATE, 0);

		for (int i = 0; i < 4; i++)
		{
			GameObject* WindowGate  = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(Guardians[i].x, Guardians[i].y, Guardians[i].z, 200002);
			if (WindowGate != NULL)
				WindowGate->SetUInt32Value(GAMEOBJECT_STATE, 1);
		}

		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Do not rejoice... your victory is a hollow one... for I shall return with powers beyond your imagining!");
		_unit->PlaySoundToSet(8814);

		_unit->GetAIInterface()->disable_melee = false;
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
		_unit->GetAIInterface()->m_canMove = true;
		RemoveAIUpdateEvent();

		EventStart = false;
		SpawnCounter = 0;
		PhaseTimer = 310;
		SpawnTimer = 0;
		GCounter = 0;
		m_phase = 0;
    }

    void AIUpdate()
    {
		switch (m_phase)
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
		default:
			{
			}
		}
    }

	void PhaseOne()
	{
		if (EventStart == true)
		{
			SpawnTimer++;
			if (SpawnTimer == 5 || SpawnTimer == 10)
			{
				for (int i = 0; i < 7; i++)
				{
					int Counter = 0;

					while (Counter == 0)
					{
						if (FrozenWastes[i] == false && (rand()%3 == 0 || SpawnCounter > 0))
						{
							for (int x = 0; x < 10; x++)
							{
								uint32 SpawnID = 10*i + x;
								_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_SOLDIER_OF_THE_FROZEN_WASTES, SFrozenWastes[SpawnID].x, SFrozenWastes[SpawnID].y, SFrozenWastes[SpawnID].z, SFrozenWastes[SpawnID].o, false, false, 0, 0);
							}

							FrozenWastes[i] = true;
							Counter++;
						}

						if (Abominations[i] == false && (rand()%3 == 0 || SpawnCounter > 0))
						{
							for (int x = 0; x < 3; x++)
							{
								uint32 SpawnID = 3*i + x;
								_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_UNSTOPPABLE_ABOMINATION, Abomination[SpawnID].x, Abomination[SpawnID].y, Abomination[SpawnID].z, Abomination[SpawnID].o, false, false, 0, 0);
							}

							Abominations[i] = true;
							Counter++;
						}

						if (SoulWeavers[i] == false && ((rand()%3 == 0 && Counter < 2) || Counter == 0 || SpawnCounter > 0))
						{
							uint32 SpawnID = i;
							_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_SOUL_WEAVER, SoulWeaver[SpawnID].x, SoulWeaver[SpawnID].y, SoulWeaver[SpawnID].z, SoulWeaver[SpawnID].o, false, false, 0, 0);
						
							SoulWeavers[i] = true;
							Counter++;
						}

						if (SoulWeavers[i] == true && Abominations[i] == true && FrozenWastes[i] == true)
							Counter = 1;
					}
				}

				SpawnCounter++;
			}

			if (SpawnCounter == 2)
			{
				for (int i = 0; i < 7; i++)
				{
					FrozenWastes[i] = false;
					Abominations[i] = false;
					SoulWeavers[i] = false;
				}

				WaveTimer = rand()%6+10;
				EventStart = false;
				SpawnCounter = 0;
				PhaseTimer = 310;
				SpawnTimer = 0;
			}
		}

		else
		{
			PhaseTimer--;
			WaveTimer--;

			if (PhaseTimer == 5)
			{
				uint32 RandomSpeach = rand()%3;
				switch (RandomSpeach)
				{
				case 0:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Pray for mercy!");
					_unit->PlaySoundToSet(8809);
					break;
				case 1:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Scream your dying breath!");
					_unit->PlaySoundToSet(8810);
					break;
				case 2:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The end is upon you!");
					_unit->PlaySoundToSet(8811);
					break;
				}
			}

			if (PhaseTimer == 3)
				DespawnTrash = true;

			if (!PhaseTimer)
			{
				_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
				_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);
				_unit->GetAIInterface()->disable_melee = false;
				_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
				_unit->GetAIInterface()->m_canMove = true;
				
				DespawnTrash = false;
				HelpDialog = 0;
				GCounter = 0;
				m_phase = 2;
				return;
			}

			else if (!WaveTimer && PhaseTimer > 5)
			{
				uint32 SpawnPoint = RandomUInt(7);
				uint32 RandomSU;
				if (PhaseTimer > 250)
					RandomSU = RandomUInt(4);
				if (PhaseTimer <= 250 && PhaseTimer >= 150)
					RandomSU = RandomUInt(5);
				if (PhaseTimer <= 150 && PhaseTimer > 100)
					RandomSU = RandomUInt(6);
				if (PhaseTimer <= 100)
					RandomSU = RandomUInt(7);

				uint32 UnitType;

				switch (RandomSU)
				{
					case 0:
					case 1: UnitType = CN_SOLDIER_OF_THE_FROZEN_WASTES; break;
					case 2:
					case 4: UnitType = CN_SOUL_WEAVER; break;
					case 3:
					case 5:
					case 6: UnitType = CN_UNSTOPPABLE_ABOMINATION; break;
					default:
						{
							UnitType = CN_UNSTOPPABLE_ABOMINATION;
						}
				}

				_unit->GetMapMgr()->GetInterface()->SpawnCreature(UnitType, Waves[SpawnPoint].x, Waves[SpawnPoint].y, Waves[SpawnPoint].z, Waves[SpawnPoint].o, false, false, 0, 0);
				WaveTimer = rand()%11+10;
			}
		}
	}

	void PhaseTwo()
	{
		if (_unit->GetHealthPct() <= 40)
		{
			HelpDialog++;
			if (HelpDialog == 1)
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Master! I require aid!");
				_unit->PlaySoundToSet(8816);
			}

			if (HelpDialog == 4)
			{
				Unit* TheLichKing = NULL;
				TheLichKing = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(3767.58f, -5117.15f, 174.49f, CN_THE_LICH_KING);
				if (TheLichKing != NULL)
				{
					TheLichKing->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Very well... warriors of the frozen wastes, rise up, I command you to fight, kill, and die for your master. Let none survive...");
					TheLichKing->PlaySoundToSet(8824);
				}

				else
				{
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Very well... warriors of the frozen wastes, rise up, I command you to fight, kill, and die for your master. Let none survive...");
					_unit->PlaySoundToSet(8824);
				}

				for (int i = 0; i < 4; i++)
				{
					GameObject* WindowGate  = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(Guardians[i].x, Guardians[i].y, Guardians[i].z, 200002);
					if (WindowGate)
						WindowGate->SetUInt32Value(GAMEOBJECT_STATE, 0);
				}
			}

			if (HelpDialog == 10 || HelpDialog == 12 || HelpDialog == 14 || HelpDialog == 16 || HelpDialog == 18)
			{
				Unit *Guardian = NULL;
				uint32 i = RandomUInt(4);
				Guardian =_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GUARDIAN_OF_ICECROWN, Guardians[i].x, Guardians[i].y, Guardians[i].z, Guardians[i].o, false, false, 0, 0);
				if (Guardian  != NULL)
				{
					if (Guardian->GetAIInterface()->GetNextTarget() != NULL)
						Guardian->GetAIInterface()->AttackReaction(Guardian->GetAIInterface()->GetNextTarget(), 1, 0);
				}

				GCounter++;
				if (GCounter == 5)
				{
					GCounter = 0;
					m_phase = 3;
				}
			}
		}

		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
	}

	void PhaseThree()
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
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}

					if (i == 2)
						ChainSound();

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || i == 1) && t > spells[i].casttime)
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

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					Unit* RandomTarget = NULL;
					RandomTarget = (Unit*)(*itr);

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

			Unit * RTarget = TargetTable[RandTarget];

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

	void ChainSound()
	{
		uint32 RandomSpeach = rand()%2;
		switch (RandomSpeach)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your soul is bound to me now!");
			_unit->PlaySoundToSet(8812);
			break;
		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "There will be no escape!");
			_unit->PlaySoundToSet(8813);
			break;
		}
	}

	bool GetDespawnTrash() { return DespawnTrash; }

protected:

	bool DespawnTrash, EventStart;
	uint32 SpawnCounter;
	uint32 HelpDialog;
	uint32 SpawnTimer;
	uint32 PhaseTimer;
	uint32 WaveTimer;
	uint32 GCounter;
	uint32 m_phase;
	int nrspells;
};

// The Lich KingAI

class TheLichKingAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(TheLichKingAI);

    TheLichKingAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }
};

// Soldier of the Frozen WastesAI

#define DARK_BLAST 28457 // 28458

class SoldierOfTheFrozenWastesAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SoldierOfTheFrozenWastesAI);
    SoldierOfTheFrozenWastesAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		_unit->m_noRespawn = true;

		OnStart = false;

		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		LastPosX = _unit->GetPositionX();
		LastPosY = _unit->GetPositionY();
		LastPosZ = _unit->GetPositionZ();

		//RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

		//RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		Creature* Kelthuzad = NULL;
		Kelthuzad = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(3749.950195f, -5113.451660f, 141.288635f, 15990);
		if (Kelthuzad == NULL || !Kelthuzad->isAlive())
		{
			_unit->Despawn(0,0);
			return;
		}
		else if (Kelthuzad->GetScript())
		{
			CreatureAIScript *pScript = Kelthuzad->GetScript();
			if (static_cast<KelthuzadAI*>(pScript)->GetDespawnTrash())
			{
				_unit->Despawn(0,0);
				return;
			}
		}
		if (_unit->GetPositionX() == LastPosX && _unit->GetPositionY() == LastPosY && _unit->GetPositionZ() == LastPosZ)
		{
			_unit->GetAIInterface()->MoveTo(newposx, newposy, 141.290451f, _unit->GetOrientation());
		}
		if (OnStart == false)
		{
			for (int i = 0; i < 7; i++)
			{
				if (_unit->GetPositionX() == Waves[i].x && _unit->GetPositionY() == Waves[i].y && _unit->GetPositionZ() == Waves[i].z)
				{
					float xchange  = (float)RandomFloat(10.0f);
					float distance = 10.0f;

					float ychange = sqrt(distance*distance - xchange*xchange);

					if (rand()%2 == 1)
						xchange *= -1;
					if (rand()%2 == 1)
						ychange *= -1;

					newposx = 3715.845703f + xchange;
					newposy = -5106.928223f + ychange;

					_unit->GetAIInterface()->MoveTo(newposx, newposy, 141.290451f, _unit->GetOrientation());
				}
			}

			OnStart = true;
		}

		if (_unit->GetAIInterface()->GetNextTarget() != NULL)
		{
			Unit *target = _unit->GetAIInterface()->GetNextTarget();
			if (_unit->GetDistance2dSq(target) <= 49.0f)
				_unit->CastSpell(_unit, DARK_BLAST, true);
		}
    }

protected:

	float LastPosX, LastPosY, LastPosZ;
	float newposx;
	float newposy;
	bool OnStart;
};

// Unstoppable Abomination AI

#define UA_MORTAL_WOUND 25646	// 36814

class UnstoppableAbominationAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(UnstoppableAbominationAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    UnstoppableAbominationAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
			spells[i].casttime = 0;
		}

		spells[0].info = dbcSpell.LookupEntry(UA_MORTAL_WOUND);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 10;
		spells[0].perctrigger = 15.0f;
		spells[0].attackstoptimer = 1000;

		_unit->m_noRespawn = true;

		OnStart = false;

		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		LastPosX = _unit->GetPositionX();
		LastPosY = _unit->GetPositionY();
		LastPosZ = _unit->GetPositionZ();

		//RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));

		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

		//RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		Creature* Kelthuzad = NULL;
		Kelthuzad = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(3749.950195f, -5113.451660f, 141.288635f, 15990);
		if (Kelthuzad == NULL || !Kelthuzad->isAlive())
		{
			_unit->Despawn(0,0);
			return;
		}
		else if (Kelthuzad->GetScript())
		{
			CreatureAIScript *pScript = Kelthuzad->GetScript();
			if (static_cast<KelthuzadAI*>(pScript)->GetDespawnTrash())
			{
				_unit->Despawn(0,0);
				return;
			}
		}
		if (_unit->GetPositionX() == LastPosX && _unit->GetPositionY() == LastPosY && _unit->GetPositionZ() == LastPosZ)
		{
			_unit->GetAIInterface()->MoveTo(newposx, newposy, 141.290451f, _unit->GetOrientation());
		}
		if (OnStart == false)
		{
			for (int i = 0; i < 7; i++)
			{
				if (_unit->GetPositionX() == Waves[i].x && _unit->GetPositionY() == Waves[i].y && _unit->GetPositionZ() == Waves[i].z)
				{
					float xchange  = (float)RandomFloat(10.0f);
					float distance = 10.0f;

					float ychange = sqrt(distance*distance - xchange*xchange);

					if (rand()%2 == 1)
						xchange *= -1;
					if (rand()%2 == 1)
						ychange *= -1;

					newposx = 3715.845703f + xchange;
					newposy = -5106.928223f + ychange;

					_unit->GetAIInterface()->MoveTo(newposx, newposy, 141.290451f, _unit->GetOrientation());
				}
			}

			OnStart = true;
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

					if (i == 0 && _unit->GetDistance2dSq(target) > 25.0f) return;

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

	float LastPosX, LastPosY, LastPosZ;
	float newposx;
	float newposy;
	bool OnStart;
	int nrspells;
};

// Soul Weaver AI

#define WAIL_OF_SOULS 28459

class SoulWeaverAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SoulWeaverAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    SoulWeaverAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
			spells[i].casttime = 0;
		}

		spells[0].info = dbcSpell.LookupEntry(WAIL_OF_SOULS);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].cooldown = 10;
		spells[0].perctrigger = 15.0f;
		spells[0].attackstoptimer = 1000;

		_unit->m_noRespawn = true;

		OnStart = false;

		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		LastPosX = _unit->GetPositionX();
		LastPosY = _unit->GetPositionY();
		LastPosZ = _unit->GetPositionZ();

		//RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));

		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

		//RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		Creature* Kelthuzad = NULL;
		Kelthuzad = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(3749.950195f, -5113.451660f, 141.288635f, 15990);
		if (Kelthuzad == NULL || !Kelthuzad->isAlive())
		{
			_unit->Despawn(0,0);
			return;
		}
		else if (Kelthuzad->GetScript())
		{
			CreatureAIScript *pScript = Kelthuzad->GetScript();
			if (static_cast<KelthuzadAI*>(pScript)->GetDespawnTrash())
			{
				_unit->Despawn(0,0);
				return;
			}
		}
		if (_unit->GetPositionX() == LastPosX && _unit->GetPositionY() == LastPosY && _unit->GetPositionZ() == LastPosZ)
		{
			_unit->GetAIInterface()->MoveTo(newposx, newposy, 141.290451f, _unit->GetOrientation());
		}
		if (OnStart == false)
		{
			for (int i = 0; i < 7; i++)
			{
				if (_unit->GetPositionX() == Waves[i].x && _unit->GetPositionY() == Waves[i].y && _unit->GetPositionZ() == Waves[i].z)
				{
					float xchange  = (float)RandomFloat(10.0f);
					float distance = 10.0f;

					float ychange = sqrt(distance*distance - xchange*xchange);

					if (rand()%2 == 1)
						xchange *= -1;
					if (rand()%2 == 1)
						ychange *= -1;

					newposx = 3715.845703f + xchange;
					newposy = -5106.928223f + ychange;

					_unit->GetAIInterface()->MoveTo(newposx, newposy, 141.290451f, _unit->GetOrientation());
				}
			}

			OnStart = true;
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

					if (i == 0 && _unit->GetDistance2dSq(target) > 64.0f) return;	// 8yards

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

	float LastPosX, LastPosY, LastPosZ;
	float newposx;
	float newposy;
	bool OnStart;
	int nrspells;
};

// Guardian of Icecrown AI

#define BLOOD_TAP 28459

class GuardianOfIcecrownAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(GuardianOfIcecrownAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    GuardianOfIcecrownAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(BLOOD_TAP);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].cooldown = 0;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

		_unit->GetAIInterface()->m_moveRun = true;
		_unit->m_noRespawn = true;

		OnStart = false;

		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		if (_unit->GetAIInterface()->GetNextTarget())
			LastTarget = _unit->GetAIInterface()->GetNextTarget();

		LastPosX = _unit->GetPositionX();
		LastPosY = _unit->GetPositionY();
		LastPosZ = _unit->GetPositionZ();
		
		//RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

		//RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		Unit* Kelthuzad = NULL;
		Kelthuzad = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(3715.950195f, -5106.451660f, 141.288635f, 15990);
		if (Kelthuzad && !Kelthuzad->isAlive())
		{
			_unit->Despawn(0,0);
		}
		if (_unit->GetPositionX() == LastPosX && _unit->GetPositionY() == LastPosY && _unit->GetPositionZ() == LastPosZ)
		{
			_unit->GetAIInterface()->MoveTo(newposx, newposy, 141.290451f, _unit->GetOrientation());
		}
		if (OnStart == false)
		{
			for (int i = 0; i < 7; i++)
			{
				if (_unit->GetPositionX() == Guardians[i].x && _unit->GetPositionY() == Guardians[i].y && _unit->GetPositionZ() == Guardians[i].z)
				{
					float xchange  = (float)RandomFloat(10.0f);
					float distance = 10.0f;

					float ychange = sqrt(distance*distance - xchange*xchange);

					if (rand()%2 == 1)
						xchange *= -1;
					if (rand()%2 == 1)
						ychange *= -1;

					newposx = 3715.845703f + xchange;
					newposy = -5106.928223f + ychange;

					_unit->GetAIInterface()->MoveTo(newposx, newposy, 141.290451f, _unit->GetOrientation());
				}
			}

			OnStart = true;
		}

		if (_unit->GetAIInterface()->GetNextTarget())
		{
			Unit* target = NULL;
			target = _unit->GetAIInterface()->GetNextTarget();

			if (!LastTarget) { LastTarget = target; return; }

			if (LastTarget != target)
				_unit->CastSpell(_unit, spells[0].info, spells[0].instant);

			LastTarget = target;
		}
    }

protected:

	float LastPosX, LastPosY, LastPosZ;
	Unit* LastTarget;
	float newposx;
	float newposy;
	bool OnStart;
	int nrspells;
};

void SetupNaxxramas(ScriptMgr * mgr)
{
	// ---- Spider Wing ----
    mgr->register_creature_script(CN_CARRION_SPINNER, &CarrionSpinnerAI::Create);
	mgr->register_creature_script(CN_DREAD_CREEPER, &DreadCreeperAI::Create);
    mgr->register_creature_script(CN_NAXX_CULTIST, &NaxxCultistAI::Create);
	mgr->register_creature_script(CN_NECRO_STALKER, &NecroStalkerAI::Create);
	mgr->register_creature_script(VENOM_STALKER, &NecroStalkerAI::Create);
	mgr->register_creature_script(CN_TOMB_HORROR, &TombHorrorAI::Create);
    mgr->register_creature_script(CN_NECRO_ACOLYTE, &NecroAcolyteAI::Create);
	// BOSS'S
	mgr->register_creature_script(CN_MAEXXNA, &MaexxnaAI::Create);
	mgr->register_creature_script(CN_WIDOW_FAERLINA, &WidowFaerlinaAI::Create);
	mgr->register_creature_script(CN_ANUB, &AnubAI::Create);

	// ---- Abomination Wing ----
	mgr->register_creature_script(CN_BILE_RETCHER, &BileRetcherAI::Create);
	mgr->register_creature_script(CN_PATCHWORK_GOLEM, &PatchworkGolemAI::Create);
	mgr->register_creature_script(STICKED_SPEWER, &StickedSpewerAI::Create);
	mgr->register_creature_script(CN_SURGICAL_ASSISTANT, &SurgicalAssistantAI::Create);
	mgr->register_creature_script(CN_SLUDGE_BELCHER, &StickedSpewerAI::Create);
	// BOSS'S
	mgr->register_creature_script(CN_PATCHWREK, &PatchwerkAI::Create);
	mgr->register_creature_script(CN_GROBBULUS, &GrobbulusAI::Create);
	mgr->register_creature_script(CN_GLUTH, &GluthAI::Create);

	// ---- Plague Wing ----
	mgr->register_creature_script(CN_INFECTIOUS_GHOUL, &InfGhoulAI::Create);
	mgr->register_creature_script(CN_STONEKIN_GARGOYLE, &StonekinGargoyleAI::Create);
	mgr->register_creature_script(16446, &StonekinGargoyleAI::Create);
	mgr->register_creature_script(CN_EYE_STALK, &EyeStalkAI::Create);
	// BOSS'S

	// ---- Deathknight Wing ----
	mgr->register_creature_script(CN_BONY_CONSTRUCT, &BonyConstructAI::Create);
	mgr->register_creature_script(CN_DEATH_LORD, &DeathLordAI::Create);
	mgr->register_creature_script(16163, &DeathLordAI::Create);
	mgr->register_creature_script(DEATHKNIGHT_CAPTAIN, &DeathknightCaptainAI::Create);
	mgr->register_creature_script(CN_BONY_CONSTRUCT, &BonyConstructAI::Create);
	// BOSS'S
	mgr->register_creature_script(CN_INSTRUCTOR_RAZUVIOUS, &RazuviousAI::Create);
	// The Four Horsemen:
	mgr->register_creature_script(CN_HIGHLORD_MOGRAINE, &MograineAI::Create);
	mgr->register_creature_script(CN_THANE_KORTHAZZ, &KorthazzAI::Create);
	mgr->register_creature_script(CN_LADY_BLAUMEUX, &BlaumeuxAI::Create);
	mgr->register_creature_script(CN_SIR_ZELIEK, &ZeliekAI::Create);

	// ---- Frostwyrm Lair ---- > Sapphiron Encounter:
	mgr->register_creature_script(CN_FROST_BREATH_TRIGGER, &FrostBreathTriggerAI::Create);
	mgr->register_creature_script(CN_FROST_BREATH_TRIGGER2, &FrostBreathTrigger2AI::Create);
	mgr->register_creature_script(CN_FROST_BREATH_TRIGGER3, &FrostBreathTrigger3AI::Create);
	mgr->register_creature_script(CN_CHILL_TRIGGER, &ChillTriggerAI::Create);
	mgr->register_creature_script(CN_SAPPHIRON, &SapphironAI::Create);

	// ---- Frostwyrm Lair ---- > Kel'thuzad Encounter:
	mgr->register_creature_script(CN_THE_LICH_KING ,&TheLichKingAI::Create);
	mgr->register_creature_script(CN_SOLDIER_OF_THE_FROZEN_WASTES ,&SoldierOfTheFrozenWastesAI::Create);
	mgr->register_creature_script(CN_UNSTOPPABLE_ABOMINATION ,&UnstoppableAbominationAI::Create);
	mgr->register_creature_script(CN_SOUL_WEAVER ,&SoulWeaverAI::Create);
	mgr->register_creature_script(CN_GUARDIAN_OF_ICECROWN ,&GuardianOfIcecrownAI::Create);
	mgr->register_creature_script(CN_KELTHUZAD, &KelthuzadAI::Create);
}