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
		spells[1].perctrigger = 4.0f;
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
		spells[0].perctrigger = 5.0f;
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

/*
8913 - Flee before its too late!
8914 - it is... as it should be
8915 - Forgive me!
8916 - i have no choice but to obey
8917 - Invaders, cease this foolish venture at once! Turn away while you still can!
8918 - Perhaps they will come to their senses, and run away as fast as they can!
8919 - Do not continue! Turn back while there's still time!

8913, 'A_ZEL_NAXX_AGGRO'
8914, 'A_ZEL_NAXX_DEATH'
8915, 'A_ZEL_NAXX_SLAY'
8916, 'A_ZEL_NAXX_SPECIAL'
8917, 'A_ZEL_NAXX_TAUNT01'
8918, 'A_ZEL_NAXX_TAUNT02'
8919, 'A_ZEL_NAXX_TAUNT03'
*/

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
}

