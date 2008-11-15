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
/* Instance_TheShatteredHalls.cpp Script		                        */
/************************************************************************/

// Fel Orc ConvertAI

#define CN_FEL_ORC_CONVERT 17083

#define HEMORRHAGE 30478

class FELORCCONVERTAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(FELORCCONVERTAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    FELORCCONVERTAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(HEMORRHAGE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 25;
		spells[0].perctrigger = 0.0f;
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

// Shattered Hand HeathenAI

#define CN_SHATTERED_HAND_HEATHEN 17420

#define BLOODTHIRST 30474 // 30475
#define ENRAGE_HEATHEN 30485	// those should be correct, but still not sure

class SHATTEREDHANDHEATHENAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SHATTEREDHANDHEATHENAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    SHATTEREDHANDHEATHENAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(BLOODTHIRST);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 25;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = dbcSpell.LookupEntry(ENRAGE_HEATHEN);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].cooldown = 70;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->CastSpell(_unit, spells[1].info, spells[1].instant);
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

// Shattered Hand LegionnaireAI

#define CN_SHATTERED_HAND_LEGIONNAIRE 16700

#define AURA_OF_DISCIPLINE 30472
#define PUMMEL 15615	// should be all good (Idk if those are all spells [summon/spawn spell?])
#define ENRAGE 30485

class SHATTEREDHANDLEGIONNAIREAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SHATTEREDHANDLEGIONNAIREAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    SHATTEREDHANDLEGIONNAIREAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
        spells[0].info = dbcSpell.LookupEntry(AURA_OF_DISCIPLINE);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].cooldown = -1;	// no idea if this should be like that
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = dbcSpell.LookupEntry(PUMMEL);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].cooldown = 25;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

        spells[2].info = dbcSpell.LookupEntry(ENRAGE);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;
		spells[2].cooldown = 70;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->CastSpell(_unit, spells[0].info, spells[0].instant);
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

// Shattered Hand SavageAI

#define CN_SHATTERED_HAND_SAVAGE 16523

#define SLICE_AND_DICE 30470
#define ENRAGE_SAVAGE 30485
#define DEATHBLOW 36023

class SHATTEREDHANDSAVAGEAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SHATTEREDHANDSAVAGEAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    SHATTEREDHANDSAVAGEAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(SLICE_AND_DICE);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].cooldown = 35;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = dbcSpell.LookupEntry(ENRAGE_SAVAGE);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].cooldown = 70;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

        spells[2].info = dbcSpell.LookupEntry(DEATHBLOW);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].cooldown = 25;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->CastSpell(_unit, spells[1].info, spells[1].instant);
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


// ShadowArcScript AcolyteAI

#define CN_SHADOWArcScript_ACOLYTE 16594

#define HEAL 31730 // 32130, 31730, 39378, 31739 // is this really used?
#define POWER_WORD_SHIELD 35944 // 41373, 29408, 36052, 35944, 32595
#define MIND_BLAST 31516 //26048 //38259 // ofc not sure (and this one can be really overpowered)
#define RESIST_SHADOW 30479	// not sure to those both
//#define PRAYER_OF_HEALING 15585	// crashes server
//#define 105 resist shadow buff?
// Self Visual - Sleep Until Cancelled (DND) 16093 ?
class SHADOWArcScriptACOLYTEAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SHADOWArcScriptACOLYTEAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    SHADOWArcScriptACOLYTEAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(HEAL);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = false;
		spells[0].cooldown = 35;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = dbcSpell.LookupEntry(POWER_WORD_SHIELD);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].cooldown = 45;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

        spells[2].info = dbcSpell.LookupEntry(MIND_BLAST);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = false;
		spells[2].cooldown = 10;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;

        spells[3].info = dbcSpell.LookupEntry(RESIST_SHADOW);
		spells[3].targettype = TARGET_SELF; // should be ally
		spells[3].instant = true;
		spells[3].cooldown = 65;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;
/*
        spells[4].info = dbcSpell.LookupEntry(PRAYER_OF_HEALING);
		spells[4].targettype = TARGET_VARIOUS;
		spells[4].instant = false;
		spells[4].cooldown = 15;
		spells[4].perctrigger = 0.0f;
		spells[4].attackstoptimer = 1000;
*/
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		if (RandomUInt(4) == 1)
			_unit->CastSpell(_unit, spells[3].info, spells[3].instant);
		else
			spells[3].casttime = RandomUInt(20)+10;
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

// Shattered Hand AssassinAI

#define CN_SHATTERED_HAND_ASSASSIN 17695	// [*]

#define SAP 30980
#define STEALTH 30991 // 32615, 30831, 30991, 31526, 31621, 34189, 32199	// I think should be harder to detect
#define CHEAP_SHOT 30986

class SHATTEREDHANDASSASSINAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SHATTEREDHANDASSASSINAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    SHATTEREDHANDASSASSINAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(SAP);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = dbcSpell.LookupEntry(STEALTH);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].cooldown = -1;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

        spells[2].info = dbcSpell.LookupEntry(CHEAP_SHOT);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].cooldown = 25;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;

		_unit->CastSpell(_unit, spells[1].info, spells[1].instant);

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		if (_unit->GetCurrentSpell() && mTarget)
			_unit->CastSpell(mTarget, spells[0].info, spells[0].instant);
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
		_unit->CastSpell(_unit, spells[1].info, spells[1].instant);
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

// ShadowArcScript Darkcaster AI

#define CN_SHADOWArcScript_DARKCASTER 17694

#define RAIN_OF_FIRE 37279 // DBC: 11990; 37279, 39376, 36808, 34360, 33617
#define FEAR 12542 //38154 // 38595, 38660, 39119, 39210, 39415, 38154, 34259, 33924, 31358, 30615
#define SHADOW_BOLT 12471 // not sure

class SHADOWArcScriptDARKCASTERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SHADOWArcScriptDARKCASTERAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    SHADOWArcScriptDARKCASTERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(RAIN_OF_FIRE);
		spells[0].targettype = TARGET_DESTINATION;
		spells[0].instant = true;
		spells[0].cooldown = 20;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = dbcSpell.LookupEntry(FEAR);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].cooldown = 35;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

        spells[2].info = dbcSpell.LookupEntry(SHADOW_BOLT);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = false;
		spells[2].cooldown = 10;
		spells[2].perctrigger = 0.0f;
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

// Shattered Hand Gladiator AI

#define CN_SHATTERED_HAND_GLADIATOR 17464	// [*]

#define MORTAL_STRIKE 31911 // 31911, 29572, 32736, 35054, 39171, 37335
// sth more?

class SHATTEREDHANDGLADIATORAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SHATTEREDHANDGLADIATORAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    SHATTEREDHANDGLADIATORAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(MORTAL_STRIKE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 15;
		spells[0].perctrigger = 0.0f;
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

// Shattered Hand Houndmaster AI

#define CN_SHATTERED_HAND_HOUNDMASTER 17670	// [*]

#define VOLLEY 34100 // 34100, 35950, 30933, 22908
// he patrols with Rabid Warhounds

class SHATTEREDHANDHOUNDMASTERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SHATTEREDHANDHOUNDMASTERAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    SHATTEREDHANDHOUNDMASTERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(VOLLEY);
		spells[0].targettype = TARGET_DESTINATION;
		spells[0].instant = false;
		spells[0].cooldown = 30;
		spells[0].perctrigger = 0.0f;
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

// Shattered Hand Reaver AI

#define CN_SHATTERED_HAND_REAVER 16699

#define CLEAVE_REAVER 15754 //34995 // no idea if this is good id
#define UPPERCUT 30471 // 32055, 34014, 34996, 39069, 41388, 30471
#define ENRAGE 30485 // 34624, 37023, 37648, 38046, 41305, 34670, 34970, 34971, 36992, 38947, 41447 and many others =/

class SHATTEREDHANDREAVERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SHATTEREDHANDREAVERAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    SHATTEREDHANDREAVERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
        spells[0].info = dbcSpell.LookupEntry(CLEAVE_REAVER);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].cooldown = 10;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = dbcSpell.LookupEntry(UPPERCUT);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].cooldown = 35;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

        spells[2].info = dbcSpell.LookupEntry(ENRAGE);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;
		spells[2].cooldown = 70;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		if (RandomUInt(4) == 1)
			_unit->CastSpell(_unit, spells[2].info, spells[2].instant);
		else
			spells[2].casttime = RandomUInt(30)+20;
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

// Shattered Hand Sentry AI

#define CN_SHATTERED_HAND_SENTRY 16507

#define CHARGE 22911 // 35570 many others
#define HAMSTERING 31553 // not sure if it uses it

class SHATTEREDHANDSENTRYAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SHATTEREDHANDSENTRYAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    SHATTEREDHANDSENTRYAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(HAMSTERING);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 20;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(CHARGE);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].cooldown = -1;
		spells[1].perctrigger = 0.0f;
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
		if (_unit->GetAIInterface()->GetNextTarget() && _unit->GetCurrentSpell())
		{
			Unit *target = NULL;
			target = _unit->GetAIInterface()->GetNextTarget();
			if (_unit->GetDistance2dSq(target) > 225.0f && RandomUInt(4) == 1)
			{
				_unit->CastSpell(target, spells[1].info, spells[1].instant);
				return;
			}
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

// Shattered Hand Sharpshooter AI

#define CN_SHATTERED_HAND_SHARPSHOOTER 16704

#define SCATTER_SHOT 23601 // 36732 // not sure
#define IMMOLATION_ARROW 35932 // same here (Idk if it uses it for sure)
#define SHOT 15620 // must find way to force mob to cast this only when dist > xx
#define INCENDIARY_SHOT 30481 // not sure to these

class SHATTEREDHANDSHARPSHOOTERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SHATTEREDHANDSHARPSHOOTERAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    SHATTEREDHANDSHARPSHOOTERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(SCATTER_SHOT);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 20;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = dbcSpell.LookupEntry(IMMOLATION_ARROW);
		spells[1].targettype = TARGET_ATTACKING;	// no idea why fire stays under caster instead of target
		spells[1].instant = false;
		spells[1].cooldown = 5;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

        spells[2].info = dbcSpell.LookupEntry(SHOT);	// disabled for now
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].cooldown = -1;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;

        spells[3].info = dbcSpell.LookupEntry(INCENDIARY_SHOT);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = false;
		spells[3].cooldown = 35;
		spells[3].perctrigger = 0.0f;
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
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		if (_unit->GetAIInterface()->GetNextTarget() && _unit->GetDistance2dSq(_unit->GetAIInterface()->GetNextTarget()) <= 900.0f)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_SPELL);
			if (_unit->GetCurrentSpell() == NULL)
			{
				uint32 Chance = RandomUInt(100);
				if (Chance <= 70)
				{
					_unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), spells[2].info, spells[2].instant);
				}

				else if (Chance > 70 && Chance <= 78)
				{
					_unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), spells[1].info, spells[1].instant);
				}

				else if (Chance > 78 && Chance <= 82)
				{
					_unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), spells[3].info, spells[3].instant);
				}

				else if (Chance > 82 && Chance <= 86)
				{
					_unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), spells[0].info, spells[0].instant);
				}
			}
		}
    }

protected:

	int nrspells;
};

// Shattered Hand Brawler AI

#define CN_SHATTERED_HAND_BRAWLER 16593

#define CURSE_OF_THE_SHATTERED_HAND 36020 //36020
#define KICK 36033 // no idea about these spells
#define TRASH 3391 // W00T? doesn't work (maybe lack of core support?)
// Self Visual - Sleep Until Cancelled (DND) 16093 ?

class SHATTEREDHANDBRAWLERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SHATTEREDHANDBRAWLERAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    SHATTEREDHANDBRAWLERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
        spells[0].info = dbcSpell.LookupEntry(CURSE_OF_THE_SHATTERED_HAND);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 35;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = dbcSpell.LookupEntry(KICK);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].cooldown = 25;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

        spells[2].info = dbcSpell.LookupEntry(TRASH);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;
		spells[2].cooldown = 20;
		spells[2].perctrigger = 0.0f;
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

/*****************************/
/*                           */
/*         Boss AIs          */
/*                           */
/*****************************/

//---------------- Grand Warlock Nethekurse Encounter ----------------//

struct Coords
{
	float x;
	float y;
	float z;
};

struct Coords Darkcasters[]=
{
	{ 160.563004f, 272.989014f, -13.189000f },
	{ 176.201004f, 264.669006f, -13.141600f },
	{ 194.951004f, 265.657990f, -13.181700f }
};

// ShadowArcScript Darkcaster AI

#define CN_SHADOWArcScript_DARKCASTER 17694

class ShadowArcScriptDarkcasterAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ShadowArcScriptDarkcasterAI);

    ShadowArcScriptDarkcasterAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		Unit *GrandWarlock = NULL;
		GrandWarlock = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(178.811996f, 292.377991f, -8.190210f, 16807);
		if (GrandWarlock)
		{
			GrandWarlock->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
			GrandWarlock->GetAIInterface()->SetAllowedToEnterCombat(false);
		}
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		Unit *GrandWarlock = NULL;
		GrandWarlock = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(178.811996f, 292.377991f, -8.190210f, 16807);
		if (GrandWarlock)
		{
			int RandomSpeach = rand()%4;
			switch (RandomSpeach)		// must be verified + emotes?
			{
			case 0:
				GrandWarlock->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You can have that one. I no longer need him.");
				GrandWarlock->PlaySoundToSet(10263);
				break;
			case 1:
				GrandWarlock->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Yes! Beat him mercilessly!");	// to add
				GrandWarlock->PlaySoundToSet(10264);
				break;
			case 2:
				GrandWarlock->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Don't waste your time on that one. He's weak.");
				GrandWarlock->PlaySoundToSet(10265);
				break;
			case 3:
				GrandWarlock->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You want him? Farewell, take him.");
				GrandWarlock->PlaySoundToSet(10266);
				break;
			}
		}
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
    }

    void OnDied(Unit * mKiller)
    {
		Creature *GrandWarlock = NULL;
		GrandWarlock = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(178.811996f, 292.377991f, -8.190210f, 16807);
		if (GrandWarlock)	// any emotes needed?
		{
			uint32 Counter = 0;
			for (int i = 0; i < 3; i++)
			{
				Unit *Servant = NULL;
				Servant = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(Darkcasters[i].x, Darkcasters[i].y, Darkcasters[i].z, 17694);
				if (!Servant)
					continue;
				if (!Servant->isAlive())
					continue;
				Counter++;
			}
			
			if (Counter == 0)
			{
				GrandWarlock->GetAIInterface()->HandleEvent(EVENT_ENTERCOMBAT, GrandWarlock, 0);
			}

			int RandomSpeach = rand()%3;
			switch (RandomSpeach)	// those need to be verified too
			{
			case 0:
				GrandWarlock->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "One pitiful wretch down. Go on, take another one.");
				GrandWarlock->PlaySoundToSet(10267);
				break;
			case 1:
				GrandWarlock->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Ah, what a waste. Next!");
				GrandWarlock->PlaySoundToSet(10268);
				break;
			case 2:
				GrandWarlock->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I was going to kill him anyway.");
				GrandWarlock->PlaySoundToSet(10269);
				break;
			}
		}
    }
};

// Grand Warlock NethekurseAI

#define CN_GRAND_WARLOCK_NETHEKURSE 16807

#define DEATH_COIL 30500 // 30741 or 30500; not sure if this is right id and if it's working like it should
#define DARK_SPIN 30502 // this should be correct	// doesn't work because of lack of core support? (so can't check)
#define LESSER_SHADOW_FISSURE 30496 // can be: 36147, 30496, 30744 // doesn't work, coz lack of core support for summons
// It has much more sounds (like for servant dies etc.). 
// For future makes researches on them.

class GrandWarlockNethekurseAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(GrandWarlockNethekurseAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    GrandWarlockNethekurseAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(DEATH_COIL);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].cooldown = 15;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].mindist2cast = 0.0f;
		spells[0].maxdist2cast = 40.0f;
		// disabled for now
		spells[1].info = dbcSpell.LookupEntry(LESSER_SHADOW_FISSURE);
		spells[1].targettype = TARGET_RANDOM_SINGLE;
		spells[1].instant = true;	// doesn't work, because of lack of core support (so to prevent channeling I changed false to true)
		spells[1].cooldown = 25;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;
		spells[1].mindist2cast = 0.0f;
		spells[1].maxdist2cast = 40.0f;

        spells[2].info = dbcSpell.LookupEntry(DARK_SPIN);
		spells[2].targettype = TARGET_VARIOUS;
		spells[2].instant = false;
		spells[2].cooldown = 60;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;

		Started = false;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		for (int i=0; i<3; i++)
			spells[i].casttime = 0;

		if (Started)
		{
			int RandomSpeach = rand()%3;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I'm already bored!");
				_unit->PlaySoundToSet(10271);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Come on! Show me a real fight!");
				_unit->PlaySoundToSet(10272);
				break;
			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I had more fun torturing the peons!");
				_unit->PlaySoundToSet(10273);
				break;
			}

			RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		}

		else
			RegisterAIUpdateEvent(4000);
    }

	void OnTargetDied(Unit* mTarget)
    {
		if (_unit->GetHealthPct() > 0)	// Hack to prevent double yelling (OnDied and OnTargetDied when creature is dying)
		{
			int RandomSpeach = rand()%2;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You loose!");
				_unit->PlaySoundToSet(10274);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Oh, just die!");
				_unit->PlaySoundToSet(10275);
				break;
			}
		}
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

		if (Started)
			RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "What a... shame.");
		_unit->PlaySoundToSet(10276);

		RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		if (!Started)
		{
			_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
			_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);

			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();

			Started = true;

			Unit *target = NULL;
			target = FindTarget();
			if (target)
			{
				_unit->GetAIInterface()->AttackReaction(target, 1, 0);
			}
		}

		if (_unit->FindAura(DARK_SPIN))
		{
			_unit->setAttackTimer(2500, false);
			return;
		}

		uint32 t = (uint32)time(NULL);
		// not sure if this should work like that
		if (t > spells[2].casttime && _unit->GetHealthPct() <= 20 && _unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			_unit->setAttackTimer(2500, false);

			_unit->CastSpell(_unit, spells[2].info, spells[2].instant);

			spells[2].casttime = t + 120;
			return;
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

	// A bit rewritten FindTarget function
	Unit* FindTarget()
	{
		Unit* target = NULL;
		float distance = 50.0f;

		Unit *pUnit;
		float dist;

		for (std::set<Object*>::iterator itr = _unit->GetInRangeOppFactsSetBegin(); itr != _unit->GetInRangeOppFactsSetEnd(); itr++)
		{
			if((*itr)->GetTypeId() != TYPEID_UNIT && (*itr)->GetTypeId() != TYPEID_PLAYER)
				continue;

			pUnit = static_cast<Unit*>((*itr));

			if(pUnit->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_FEIGN_DEATH))
				continue;

			if(pUnit->m_invisible)
				continue;
			
			if(!pUnit->isAlive() || _unit == pUnit)
				continue;

			dist = _unit->GetDistance2dSq(pUnit);

			if(dist > distance*distance)
				continue;

			target = pUnit;
			break;
		}

		return target;
	}

protected:

	bool Started;
	int nrspells;
};

// Blood Guard PorungAI

#define CN_BLOOD_GUARD_PORUNG 20923

#define CLEAVE 37476 // right description, but no idea if this is right spell
//#define FEAR <-- disabled in 2.1
// Note: This boss appears only in Heroic mode and I don't have much infos about it =/

class BloodGuardPorungAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(BloodGuardPorungAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    BloodGuardPorungAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(CLEAVE);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].cooldown = 15;
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
			spells[i].casttime = 0;
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

// Warbringer O'mroggAI

#define CN_WARBRINGER_OMROGG 16809

#define THUNDERCLAP 30633
#define AOE_FEAR 30584 // can be : 30584, 33547, 29321
#define BURNING_MAUL 30598 // 30598 or 30599; after it blast wave should be casted
#define BLAST_WAVE 30600 // should be caster after burning maul and then after few sec should be casted again

class WARBRINGEROMROGGAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(WARBRINGEROMROGGAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    WARBRINGEROMROGGAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		BURNING_MAULCooldown = 30;
		BLAST_WAVE_TIMER = 11;
		ThreatTimer = RandomUInt(5)+20;
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
        spells[0].info = dbcSpell.LookupEntry(THUNDERCLAP);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].cooldown = 12;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 2000;

        spells[1].info = dbcSpell.LookupEntry(AOE_FEAR);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].cooldown = 35;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

        spells[2].info = dbcSpell.LookupEntry(BURNING_MAUL);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;
		spells[2].cooldown = -1;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;

        spells[3].info = dbcSpell.LookupEntry(BLAST_WAVE);
		spells[3].targettype = TARGET_VARIOUS;
		spells[3].instant = false;
		spells[3].cooldown = -1;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 2000;

		LHead = _unit->GetMapMgr()->GetInterface()->SpawnCreature(19523, _unit->GetPositionX(), _unit->GetPositionY(), 
			_unit->GetPositionZ(), 0, true, false, 0, 0);
		RHead = _unit->GetMapMgr()->GetInterface()->SpawnCreature(19524, _unit->GetPositionX(), _unit->GetPositionY(), 
			_unit->GetPositionZ(), 0, true, false, 0, 0);
		LHead->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		RHead->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		BURNING_MAULCooldown = 30;
		BLAST_WAVE_TIMER = 11;
		ThreatTimer = RandomUInt(5)+20;
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		
		if(!LHead || !RHead)
			return;
		LHead->GetAIInterface()->SetUnitToFollow(_unit);
		RHead->GetAIInterface()->SetUnitToFollow(_unit);

		uint32 val = (uint32)RandomUInt(100)%3;
		switch(val)
		{
		case 0:
			LHead->PlaySoundToSet(10308);
			LHead->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "If you nice me let you live.");
			sEventMgr.AddEvent(((Object*)RHead), &Object::PlaySoundToSet, (uint32)10318, EVENT_UNK, 4000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			sEventMgr.AddEvent(((Unit*)RHead), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_YELL, (uint32)LANG_UNIVERSAL, 
				"No, we will NOT let you live!", EVENT_UNIT_CHAT_MSG, 4000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			break;
		case 1:
			LHead->PlaySoundToSet(10309);
			LHead->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Me hungry!");
			sEventMgr.AddEvent(((Object*)RHead), &Object::PlaySoundToSet, (uint32)10319, EVENT_UNK, 2500, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			sEventMgr.AddEvent(((Unit*)RHead), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_YELL, (uint32)LANG_UNIVERSAL, 
				"You always hungry. That why we so fat!", EVENT_UNIT_CHAT_MSG, 2500, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			break;
		case 2:
			LHead->PlaySoundToSet(10306);
			LHead->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Smash!");
			sEventMgr.AddEvent(((Object*)RHead), &Object::PlaySoundToSet, (uint32)10317, EVENT_UNK, 2000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			sEventMgr.AddEvent(((Unit*)RHead), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_YELL, (uint32)LANG_UNIVERSAL, 
				"Why don't you let me do the talking!", EVENT_UNIT_CHAT_MSG, 2000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			break;
		}
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnTargetDied(Unit* mTarget)
    {
		val = (uint32)RandomUInt(100)%2;
		switch(val)
		{
		case 0:
			LHead->PlaySoundToSet(10320);
			LHead->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I'm tired. You kill the next one!");
			break;
		case 1:
			LHead->PlaySoundToSet(10310);
			LHead->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "This one die easy!");
			sEventMgr.AddEvent(((Object*)RHead), &Object::PlaySoundToSet, (uint32)10321, EVENT_UNK, 3000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			sEventMgr.AddEvent(((Unit*)RHead), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_YELL, (uint32)LANG_UNIVERSAL, 
				"That's because I do all the hard work!", EVENT_UNIT_CHAT_MSG, 3000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			break;
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
		if(!LHead || !RHead)
			return;
		LHead->PlaySoundToSet(10311);
		LHead->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "This all... your fault!");
		sEventMgr.AddEvent(((Object*)RHead), &Object::PlaySoundToSet, (uint32)10322, EVENT_UNK, 3000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		sEventMgr.AddEvent(((Unit*)RHead), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_YELL, (uint32)LANG_UNIVERSAL, 
			"I... hate... you!", EVENT_UNIT_CHAT_MSG, 3000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		LHead->Despawn(10000, 0);
		RHead->Despawn(10000, 0);
    }

	void ThreatShift()
	{
		// Modified somewhat
		std::vector<Player *> TargetTable;	/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
											/* If anyone wants to use this function, then leave this note!										 */
		for(set<Player*>::iterator itr = _unit->GetInRangePlayerSetBegin(); itr != _unit->GetInRangePlayerSetEnd(); ++itr) 
		{ 
			Player* RandomTarget = NULL;
			RandomTarget = (Player*)(*itr);

			if (RandomTarget->isAlive() && _unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 )
				TargetTable.push_back(RandomTarget);
		}

		if (!TargetTable.size())
			return;

		size_t RandTarget = rand()%TargetTable.size();

		RTarget = TargetTable[RandTarget];
		
		_unit->ClearHateList();
		_unit->GetAIInterface()->taunt(RTarget, true);
		val = (uint32)RandomUInt(100)%7;
		switch(val)
		{
		case 0:
			LHead->PlaySoundToSet(10301);
			LHead->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "We kill his friend!");
			break;
		case 1:
			LHead->PlaySoundToSet(10300);
			LHead->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Me not like this one...");
			sEventMgr.AddEvent(((Object*)RHead), &Object::PlaySoundToSet, (uint32)10313, EVENT_UNK, 3000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			sEventMgr.AddEvent(((Unit*)RHead), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_YELL, (uint32)LANG_UNIVERSAL, 
				"I'm not done yet, idiot!", EVENT_UNIT_CHAT_MSG, 3000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			break;
		case 2:
			RHead->PlaySoundToSet(10312);
			RHead->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Hey, you numbskull!");
			break;
		case 3:
			LHead->PlaySoundToSet(10305);
			LHead->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Me get bored.");
			sEventMgr.AddEvent(((Object*)RHead), &Object::PlaySoundToSet, (uint32)10316, EVENT_UNK, 3000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			sEventMgr.AddEvent(((Unit*)RHead), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_YELL, (uint32)LANG_UNIVERSAL, 
				"Bored, he's almost dead!", EVENT_UNIT_CHAT_MSG, 3000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			break;
		case 4:
			RHead->PlaySoundToSet(10315);
			RHead->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "What are you doing!?");
			break;
		case 5:
			LHead->PlaySoundToSet(10303);
			LHead->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You stay here. Me go kill someone else!");
			sEventMgr.AddEvent(((Object*)RHead), &Object::PlaySoundToSet, (uint32)10314, EVENT_UNK, 4000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			sEventMgr.AddEvent(((Unit*)RHead), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_YELL, (uint32)LANG_UNIVERSAL, 
				"That's not funny!", EVENT_UNIT_CHAT_MSG, 4000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			sEventMgr.AddEvent(((Object*)LHead), &Object::PlaySoundToSet, (uint32)10304, EVENT_UNK, 6000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			sEventMgr.AddEvent(((Unit*)LHead), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_YELL, (uint32)LANG_UNIVERSAL, 
				"Ha ha ha!", EVENT_UNIT_CHAT_MSG, 6000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			break;
		case 6:
			LHead->PlaySoundToSet(10302);
			LHead->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Me kill someone else!");
			sEventMgr.AddEvent(((Object*)RHead), &Object::PlaySoundToSet, (uint32)10315, EVENT_UNK, 3000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			sEventMgr.AddEvent(((Unit*)RHead), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_YELL, (uint32)LANG_UNIVERSAL, 
				"What are you doing!?", EVENT_UNIT_CHAT_MSG, 3000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			break;
		}
	}

    void AIUpdate()
    {
		BURNING_MAULCooldown--;
		BLAST_WAVE_TIMER++;
		ThreatTimer--;

		if (!BURNING_MAULCooldown)
		{
			_unit->CastSpell(_unit, spells[2].info, spells[2].instant);
			BLAST_WAVE_TIMER = 0;
			BURNING_MAULCooldown = 30;
		}

		if (BLAST_WAVE_TIMER == 1 || BLAST_WAVE_TIMER == 5)	// 3/10
		{
			_unit->CastSpell(_unit, spells[3].info, spells[3].instant);
		}

		if (!ThreatTimer)
		{
			_unit->GetAIInterface()->taunt(RTarget, false);
			ThreatShift();
			ThreatTimer = RandomUInt(5)+20;
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

	uint32 val;
	int ThreatTimer;
	int BLAST_WAVE_TIMER;
	int BURNING_MAULCooldown;
	int nrspells;
	Unit *RTarget;
	Creature *LHead;
	Creature *RHead;
};

// Warchief Kargath BladefistAI

#define CN_WARCHIEF_KARGATH_BLADEFIST 16808

#define BLADE_DANCE 30739	// should be each 30 sec, but Idk correct attktime
// Should call for support?
// does he use only one ability?

class WarchiefKargathBladefistAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(WarchiefKargathBladefistAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    WarchiefKargathBladefistAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(BLADE_DANCE);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].cooldown = 30;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		int RandomSpeach = rand()%3;
		switch (RandomSpeach)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Ours is the true horde! The only horde!");
			_unit->PlaySoundToSet(10323);
			break;
		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I'll carve the meat from your bones!");
			_unit->PlaySoundToSet(10324);
			break;
		case 2:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I am called Bladefists for a reason... as you will see!");
			_unit->PlaySoundToSet(10325);
			break;
		}

		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));

		spells[0].casttime = (uint32)time(NULL) + 30;
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
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "For the real horde!");
				_unit->PlaySoundToSet(10326);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I am the only warchief!");
				_unit->PlaySoundToSet(10327);
				break;
			}
		}
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The true horde... will... prevail!");
		_unit->PlaySoundToSet(10328);
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		uint32 t = (uint32)time(NULL);
		if (t > spells[0].casttime && _unit->GetAIInterface()->GetNextTarget() && _unit->GetCurrentSpell() == NULL)
		{
			_unit->setAttackTimer(1500, false);

			_unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), spells[0].info, spells[0].instant);

			spells[0].casttime = t + spells[0].cooldown;
		}
    }

protected:

	int nrspells;
};

void SetupTheShatteredHalls(ScriptMgr * mgr)
{
	mgr->register_creature_script(CN_FEL_ORC_CONVERT, &FELORCCONVERTAI::Create);
	mgr->register_creature_script(CN_SHATTERED_HAND_HEATHEN, &SHATTEREDHANDHEATHENAI::Create);
	mgr->register_creature_script(CN_SHATTERED_HAND_LEGIONNAIRE, &SHATTEREDHANDLEGIONNAIREAI::Create);
	mgr->register_creature_script(CN_SHATTERED_HAND_SAVAGE, &SHATTEREDHANDSAVAGEAI::Create);
	mgr->register_creature_script(CN_SHADOWArcScript_ACOLYTE, &SHADOWArcScriptACOLYTEAI::Create);
	mgr->register_creature_script(CN_SHATTERED_HAND_ASSASSIN, &SHATTEREDHANDASSASSINAI::Create);
	mgr->register_creature_script(CN_SHADOWArcScript_DARKCASTER, &SHADOWArcScriptDARKCASTERAI::Create);
	mgr->register_creature_script(CN_SHATTERED_HAND_GLADIATOR, &SHATTEREDHANDGLADIATORAI::Create);	
	mgr->register_creature_script(CN_SHATTERED_HAND_HOUNDMASTER, &SHATTEREDHANDHOUNDMASTERAI::Create);	
	mgr->register_creature_script(CN_SHATTERED_HAND_REAVER, &SHATTEREDHANDREAVERAI::Create);	
	mgr->register_creature_script(CN_SHATTERED_HAND_SENTRY, &SHATTEREDHANDSENTRYAI::Create);
	mgr->register_creature_script(CN_SHATTERED_HAND_SHARPSHOOTER, &SHATTEREDHANDSHARPSHOOTERAI::Create);
	mgr->register_creature_script(CN_SHATTERED_HAND_BRAWLER, &SHATTEREDHANDBRAWLERAI::Create);
	mgr->register_creature_script(CN_SHADOWArcScript_DARKCASTER, &ShadowArcScriptDarkcasterAI::Create);
	mgr->register_creature_script(CN_GRAND_WARLOCK_NETHEKURSE, &GrandWarlockNethekurseAI::Create);
	mgr->register_creature_script(CN_BLOOD_GUARD_PORUNG, &BloodGuardPorungAI::Create);
	mgr->register_creature_script(CN_WARBRINGER_OMROGG, &WARBRINGEROMROGGAI::Create);
	mgr->register_creature_script(CN_WARCHIEF_KARGATH_BLADEFIST, &WarchiefKargathBladefistAI::Create);
}

// TO DO: Shattered Hand Executioner 17301, Shattered Hand Champion 17671, 
// Shattered Hand Centurion 17465, Shattered Hand Blood Guard 17461, 
// Shattered Hand Archer 17427, Sharpshooter Guard 17622, Shattered Hand Zealot 17462
// (lack of infos or don't have any spells!) more?
