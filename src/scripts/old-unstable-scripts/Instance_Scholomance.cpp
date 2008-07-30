#include "StdAfx.h"
#include "Setup.h"

/************************************************************************/
/* Instance_Scholomance.cpp Script										*/
/************************************************************************/

/*****************************/
/*                           */
/*         Boss AIs          */
/*                           */
/*****************************/

// Doctor Theolen KrastinovAI

#define CN_DOCTOR_THEOLEN_KRASTINOV 11261	//+

#define REND	18106            
#define KRASTINOVCLEAVE	15584
#define FRENZY	28371

class DoctorTheolenKrastinovAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(DoctorTheolenKrastinovAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    DoctorTheolenKrastinovAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		FRENZY_LIMITER = 0;
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		}

		spells[0].info = sSpellStore.LookupEntry(REND);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 20.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = sSpellStore.LookupEntry(KRASTINOVCLEAVE);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 9.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = sSpellStore.LookupEntry(FRENZY);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000; 
	}		
	void OnCombatStart(Unit* mTarget)
    {
		FRENZY_LIMITER = 0;
        RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

    void OnCombatStop(Unit *mTarget)
    {
		FRENZY_LIMITER = 0;
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

	void OnDied(Unit * mKiller)
    {
		FRENZY_LIMITER = 0;
       RemoveAIUpdateEvent();
    }

	void AIUpdate()
    {
		if(_unit->GetHealthPct() <= 50 && FRENZY_LIMITER == 0)
        {
			// FRENZY
			_unit->CastSpell(_unit, spells[2].info, spells[2].instant);
			FRENZY_LIMITER = 1;
		}
		else
		{
			float val = sRand.rand(100.0f);
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
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						target = _unit->GetAIInterface()->GetNextTarget();
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); 
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

	uint32 FRENZY_LIMITER;
	int nrspells;
};

// Instructor MaliciaAI

#define CN_INSTRUCTOR_MALICIA 10505	//+

#define CALL_OF_GRAVE	17831	
#define CORRUPTION		11672  
#define FLASH_HEAL		17138	//10917
#define RENEW			10929
#define HEAL			15586	// not sure

class InstructorMaliciaAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(InstructorMaliciaAI);
	SP_AI_Spell spells[5];
	bool m_spellcheck[5];

    InstructorMaliciaAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 5;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		}

		spells[0].info = sSpellStore.LookupEntry(CALL_OF_GRAVE);
		spells[0].targettype = TARGET_DESTINATION; // VARIOUS, DESINATION or ATTACKING?
		spells[0].instant = true;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = sSpellStore.LookupEntry(CORRUPTION);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;	// should be false, but doesn't work then
		spells[1].perctrigger = 9.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = sSpellStore.LookupEntry(FLASH_HEAL);
		spells[2].targettype = TARGET_SELF;	// works for caster and his enemy :o
		spells[2].instant = true;	// should be false, but doesn't work then
		spells[2].perctrigger = 5.0f;
		spells[2].attackstoptimer = 1000; 

		spells[3].info = sSpellStore.LookupEntry(RENEW);
		spells[3].targettype = TARGET_SELF;	// w00t, heals player too ? :|
		spells[3].instant = true;
		spells[3].perctrigger = 4.0f;	// why 0?:|
		spells[3].attackstoptimer = 1000;

		spells[4].info = sSpellStore.LookupEntry(HEAL);
		spells[4].targettype = TARGET_SELF;
		spells[4].instant = true;	// should be false, but doesn't work then
		spells[4].perctrigger = 5.0f;
		spells[4].attackstoptimer = 1000; 
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
		float val = sRand.rand(100.0f);
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
						target = _unit->GetAIInterface()->GetNextTarget();
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); 
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

// The RavenianAI

#define CN_THE_RAVENIAN 10507	//+

#define TRAMPLE			15550 	
#define RAVENIANCLEAVE	20691 
#define SUNDERINCLEAVE	25174
#define KNOCKAWAY		10101   

class TheRavenianAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(TheRavenianAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    TheRavenianAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		}

		spells[0].info = sSpellStore.LookupEntry(TRAMPLE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = sSpellStore.LookupEntry(RAVENIANCLEAVE);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 9.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = sSpellStore.LookupEntry(SUNDERINCLEAVE);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].perctrigger = 20.0f;
		spells[2].attackstoptimer = 1000; 

		spells[3].info = sSpellStore.LookupEntry(KNOCKAWAY);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = true;
		spells[3].perctrigger = 11.0f;
		spells[3].attackstoptimer = 1000;
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
		float val = sRand.rand(100.0f);
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
						target = _unit->GetAIInterface()->GetNextTarget();
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); 
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

// Lady Illucia BarovAI

#define CN_LADY_ILLUCIA_BAROV 10502	//+

#define CURSE_OF_AGONY	18671	//+
#define SHADOW_SHOCK		20603	//+
#define SILENCE			15487	//+
#define ILLUCIA_FEAR	26580	//26661	//+
#define DOMINATE_MIND	20740	//+

class LadyIlluciaBarovAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(LadyIlluciaBarovAI);
	SP_AI_Spell spells[5];
	bool m_spellcheck[5];

    LadyIlluciaBarovAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 5;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		}

		spells[0].info = sSpellStore.LookupEntry(CURSE_OF_AGONY);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = sSpellStore.LookupEntry(SHADOW_SHOCK);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].perctrigger = 12.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = sSpellStore.LookupEntry(SILENCE);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].perctrigger = 8.0f;
		spells[2].attackstoptimer = 1000; 

		spells[3].info = sSpellStore.LookupEntry(ILLUCIA_FEAR);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = true;
		spells[3].perctrigger = 4.0f;
		spells[3].attackstoptimer = 1000;

		spells[4].info = sSpellStore.LookupEntry(DOMINATE_MIND);
		spells[4].targettype = TARGET_ATTACKING;
		spells[4].instant = true;
		spells[4].perctrigger = 4.0f;
		spells[4].attackstoptimer = 1000;
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
		float val = sRand.rand(100.0f);
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
						target = _unit->GetAIInterface()->GetNextTarget();
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); 
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

// Ras ForstwhisperAI

#define CN_RAS_FORSTWHISPER 10508	//+

#define FROSTBOLT		21369	//+
#define ICE_ARMOR		18100	//+
#define FREEZE			18763	//+
#define RAS_FEAR		26070	//+
#define CHILL_NOVA		18099	//+
#define FROSTBOLT_VOLLEY 22643	//8398	

class RasForstwhisperAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(RasForstwhisperAI);
	SP_AI_Spell spells[6];
	bool m_spellcheck[6];

    RasForstwhisperAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 6;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		}

		spells[0].info = sSpellStore.LookupEntry(FROSTBOLT);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 14.0f;
		spells[0].attackstoptimer = 2000;

		spells[1].info = sSpellStore.LookupEntry(ICE_ARMOR);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = sSpellStore.LookupEntry(FREEZE);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = false;
		spells[2].perctrigger = 11.0f;
		spells[2].attackstoptimer = 4000; 

		spells[3].info = sSpellStore.LookupEntry(RAS_FEAR);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = true;	// should be false, but doesn't work then
		spells[3].perctrigger = 9.0f;
		spells[3].attackstoptimer = 2000;

		spells[4].info = sSpellStore.LookupEntry(CHILL_NOVA);
		spells[4].targettype = TARGET_VARIOUS;
		spells[4].instant = true;
		spells[4].perctrigger = 8.0f;
		spells[4].attackstoptimer = 1000;

		spells[5].info = sSpellStore.LookupEntry(FROSTBOLT_VOLLEY);
		spells[5].targettype = TARGET_VARIOUS;
		spells[5].instant = true;	// should be false, but doesn't work then
		spells[5].perctrigger = 13.0f;
		spells[5].attackstoptimer = 2000; 
	}		
	void OnCombatStart(Unit* mTarget)
    {
        RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->CastSpell(_unit, spells[1].info, spells[1].instant);
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
		float val = sRand.rand(100.0f);
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
						target = _unit->GetAIInterface()->GetNextTarget();
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); 
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

// Jandice BarovAI

#define CN_JANDICE_BAROV 10503	//+

#define CURSE_OF_BLOOD	24673
#define SUMMON_ILLUSION	17773
#define BANISH			39674 // not sure //8994
class JandiceBarovAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(JandiceBarovAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    JandiceBarovAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		}

		spells[0].info = sSpellStore.LookupEntry(CURSE_OF_BLOOD);
		spells[0].targettype = TARGET_DESTINATION;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = sSpellStore.LookupEntry(BANISH);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].perctrigger = 8.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = sSpellStore.LookupEntry(SUMMON_ILLUSION);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;
		spells[2].perctrigger = 5.0f;
		spells[2].attackstoptimer = 1000;
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
		float val = sRand.rand(100.0f);
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
						target = _unit->GetAIInterface()->GetNextTarget();
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); 
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

// KormokAI

#define CN_KORMOK 14491	//+

#define SHADOW_BOLT_VOLLEY	20741            
#define BONE_SHIELD			27688
#define SUMMON_RISEY_LACKEY 17618	// not sure

class KormokAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(KormokAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    KormokAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		}

		spells[0].info = sSpellStore.LookupEntry(SHADOW_BOLT_VOLLEY);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 11.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = sSpellStore.LookupEntry(BONE_SHIELD);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = sSpellStore.LookupEntry(SUMMON_RISEY_LACKEY);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = false;
		spells[2].perctrigger = 4.0f;
		spells[2].attackstoptimer = 1000;
	}		
	void OnCombatStart(Unit* mTarget)
    {
        RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->CastSpell(_unit, spells[1].info, spells[1].instant);
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
		float val = sRand.rand(100.0f);
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
						target = _unit->GetAIInterface()->GetNextTarget();
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); 
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

// VectusAI

#define CN_VECTUS	10432	//+

#define FIRE_SHIELD		19627        
#define BLAST_WAVE		13021
#define FRENZY			28371

class VectusAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(VectusAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    VectusAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		FRENZY_LIMITER = 0;
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		}

		spells[0].info = sSpellStore.LookupEntry(BLAST_WAVE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 18.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = sSpellStore.LookupEntry(FIRE_SHIELD);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].perctrigger = 5.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = sSpellStore.LookupEntry(FRENZY);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;
	}		
	void OnCombatStart(Unit* mTarget)
    {
		FRENZY_LIMITER = 0;
        RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->CastSpell(_unit, spells[1].info, spells[1].instant);
    }

    void OnCombatStop(Unit *mTarget)
    {
		FRENZY_LIMITER = 0;
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

	void OnDied(Unit * mKiller)
    {
		FRENZY_LIMITER = 0;
       RemoveAIUpdateEvent();
    }

	void AIUpdate()
    {
		if(_unit->GetHealthPct() <= 25 && !FRENZY_LIMITER)
		{
			_unit->CastSpell(_unit, spells[2].info, spells[2].instant);
			FRENZY_LIMITER = 1;
		}
		float val = sRand.rand(100.0f);
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
						target = _unit->GetAIInterface()->GetNextTarget();
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); 
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

	uint32 FRENZY_LIMITER;
	int nrspells;
};

// Lord Alexei BarovAI

#define CN_LORD_ALEXEI_BAROV 10504	//+

#define UNHOLY_AURA		17467
#define IMMOLATE		20294
#define VEIL_OF_SHADOW	17820

class LordAlexeiBarovAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(LordAlexeiBarovAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    LordAlexeiBarovAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		}

		spells[0].info = sSpellStore.LookupEntry(UNHOLY_AURA);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = sSpellStore.LookupEntry(IMMOLATE);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = sSpellStore.LookupEntry(VEIL_OF_SHADOW);
		spells[2].targettype = TARGET_VARIOUS;
		spells[2].instant = false;
		spells[2].perctrigger = 8.0f;
		spells[2].attackstoptimer = 2000;
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
		float val = sRand.rand(100.0f);
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
						target = _unit->GetAIInterface()->GetNextTarget();
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); 
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

// Lorekeeper PolkeltAI

#define CN_LOREKEEPER_POLKELT 10901	//+

#define VOLATILE_INFECTION	24928 	// wrong id // can cause crashes as it is casted on caster too
#define DARK_PLAGUE			18270	// sure it should be here?
#define CORROSIVE_ACID		19463	// Added Corrosive Acid Spit; 16359 or 20667 or 19463 or 19463	// 23313
#define NOXIOUS_CATALYST	18151

class LorekeeperPolkeltAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(LorekeeperPolkeltAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    LorekeeperPolkeltAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		}

		spells[0].info = sSpellStore.LookupEntry(VOLATILE_INFECTION);
		spells[0].targettype = TARGET_ATTACKING;	// various affects caster too
		spells[0].instant = true;
		spells[0].perctrigger = 6.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = sSpellStore.LookupEntry(DARK_PLAGUE);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 8.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = sSpellStore.LookupEntry(CORROSIVE_ACID);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;	// should be false, but doesn't work then =/
		spells[2].perctrigger = 12.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = sSpellStore.LookupEntry(NOXIOUS_CATALYST);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = true;
		spells[3].perctrigger = 10.0f;
		spells[3].attackstoptimer = 1000;
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
		float val = sRand.rand(100.0f);
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
						target = _unit->GetAIInterface()->GetNextTarget();
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); 
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

// Darkmaster GandlingAI

#define CN_DARKMASTER_GANDLING 1853	//+	

#define ARCANE_MISSILES			25346	
#define CURSE_OF_THE_DARKMASTER	18702
#define SHADOW_SHIELD			22417

class DarkmasterGandlingAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(DarkmasterGandlingAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    DarkmasterGandlingAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3; // 4 ?
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		}

		spells[0].info = sSpellStore.LookupEntry(ARCANE_MISSILES); //VOLATILEINFECTION ???????? :|
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 12.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = sSpellStore.LookupEntry(CURSE_OF_THE_DARKMASTER);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].perctrigger = 7.0f;
		spells[1].attackstoptimer = 2000;

		spells[2].info = sSpellStore.LookupEntry(SHADOW_SHIELD);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;
	}		
	void OnCombatStart(Unit* mTarget)
    {
        RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->CastSpell(_unit, spells[2].info, spells[2].instant);
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
		float val = sRand.rand(100.0f);
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
						target = _unit->GetAIInterface()->GetNextTarget();
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); 
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

void SetupScholomance(ScriptMgr * mgr)
{
	mgr->register_creature_script(CN_DOCTOR_THEOLEN_KRASTINOV, &DoctorTheolenKrastinovAI::Create);
	mgr->register_creature_script(CN_INSTRUCTOR_MALICIA, &InstructorMaliciaAI::Create);
	mgr->register_creature_script(CN_THE_RAVENIAN, &TheRavenianAI::Create);
	mgr->register_creature_script(CN_LADY_ILLUCIA_BAROV, &LadyIlluciaBarovAI::Create);
	mgr->register_creature_script(CN_RAS_FORSTWHISPER, &RasForstwhisperAI::Create);
	mgr->register_creature_script(CN_JANDICE_BAROV, &JandiceBarovAI ::Create);
	mgr->register_creature_script(CN_KORMOK, &KormokAI::Create);
	mgr->register_creature_script(CN_VECTUS, &VectusAI::Create);
	mgr->register_creature_script(CN_LORD_ALEXEI_BAROV, &LordAlexeiBarovAI::Create);
	mgr->register_creature_script(CN_LOREKEEPER_POLKELT, &LorekeeperPolkeltAI::Create);
	mgr->register_creature_script(CN_DARKMASTER_GANDLING, &DarkmasterGandlingAI::Create);
}

// Who should be added ?
//	*Kirtonos the Herald
//	*Death Knight Darkreaver
//	*Lord Blackwood
//	*Marduk Blackpool
//	*Rattlegore
