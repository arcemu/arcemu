/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009 ArcEmu Team <http://www.arcemu.org/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
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
		RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit* mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		CastTime();
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
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
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
		RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		_unit->CastSpell(_unit, spells[1].info, spells[1].instant);
	}

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit* mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		CastTime();
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
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
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
		RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		_unit->CastSpell(_unit, spells[0].info, spells[0].instant);
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit* mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		CastTime();
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
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
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
		RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		_unit->CastSpell(_unit, spells[1].info, spells[1].instant);
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit* mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		CastTime();
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
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
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


// Shadowmoon AcolyteAI

#define CN_SHADOWMOON_ACOLYTE 16594

#define HEAL 31730 // 32130, 31730, 39378, 31739 // is this really used?
#define POWER_WORD_SHIELD 35944 // 41373, 29408, 36052, 35944, 32595
#define MIND_BLAST 31516 //26048 //38259 // ofc not sure (and this one can be really overpowered)
#define RESIST_SHADOW 30479	// not sure to those both
//#define PRAYER_OF_HEALING 15585	// crashes server
//#define 105 resist shadow buff?
// Self Visual - Sleep Until Cancelled (DND) 16093 ?
class SHADOWMOONACOLYTEAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SHADOWMOONACOLYTEAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    SHADOWMOONACOLYTEAI(Creature* pCreature) : CreatureAIScript(pCreature)
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
		RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
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

    void OnCombatStop(Unit* mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		CastTime();
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
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
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
		RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		if (_unit->GetCurrentSpell() && mTarget)
			_unit->CastSpell(mTarget, spells[0].info, spells[0].instant);
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit* mTarget)
    {
		CastTime();
		_unit->CastSpell(_unit, spells[1].info, spells[1].instant);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		CastTime();
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
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
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
		RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit* mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		CastTime();
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
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
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
		RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit* mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		CastTime();
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
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
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
		RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
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

    void OnCombatStop(Unit* mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		CastTime();
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
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
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
		RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit* mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		CastTime();
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		if (_unit->GetAIInterface()->getNextTarget() && _unit->GetCurrentSpell())
		{
			Unit* target = NULL;
			target = _unit->GetAIInterface()->getNextTarget();
			if (_unit->GetDistance2dSq(target) > 225.0f && RandomUInt(4) == 1)
			{
				_unit->CastSpell(target, spells[1].info, spells[1].instant);
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
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
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
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		if (_unit->GetAIInterface()->getNextTarget() && _unit->GetDistance2dSq(_unit->GetAIInterface()->getNextTarget()) <= 900.0f)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_SPELL);
			if (_unit->GetCurrentSpell() == NULL)
			{
				uint32 Chance = RandomUInt(100);
				if (Chance <= 70)
				{
					_unit->CastSpell(_unit->GetAIInterface()->getNextTarget(), spells[2].info, spells[2].instant);
				}

				else if (Chance > 70 && Chance <= 78)
				{
					_unit->CastSpell(_unit->GetAIInterface()->getNextTarget(), spells[1].info, spells[1].instant);
				}

				else if (Chance > 78 && Chance <= 82)
				{
					_unit->CastSpell(_unit->GetAIInterface()->getNextTarget(), spells[3].info, spells[3].instant);
				}

				else if (Chance > 82 && Chance <= 86)
				{
					_unit->CastSpell(_unit->GetAIInterface()->getNextTarget(), spells[0].info, spells[0].instant);
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
		RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit* mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		CastTime();
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
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
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

static Location Darkcasters[]=
{
	{ 160.563004f, 272.989014f, -13.189000f },
	{ 176.201004f, 264.669006f, -13.141600f },
	{ 194.951004f, 265.657990f, -13.181700f }
};

// Shadowmoon Darkcaster AI

#define CN_SHADOWMOON_DARKCASTER 17694

#define RAIN_OF_FIRE 37279 // DBC: 11990; 37279, 39376, 36808, 34360, 33617
#define FEAR 12542 //38154 // 38595, 38660, 39119, 39210, 39415, 38154, 34259, 33924, 31358, 30615
#define SHADOW_BOLT 12471 // not sure

class ShadowmoonDarkcasterAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ShadowmoonDarkcasterAI);

    ShadowmoonDarkcasterAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		Unit* GrandWarlock = NULL;
		GrandWarlock = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(178.811996f, 292.377991f, -8.190210f, 16807);
		if (GrandWarlock)
		{
			GrandWarlock->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
			GrandWarlock->GetAIInterface()->SetAllowedToEnterCombat(false);
		}
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		Unit* GrandWarlock = NULL;
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

    void OnCombatStop(Unit* mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
    }

    void OnDied(Unit* mKiller)
    {
		Creature* GrandWarlock = NULL;
		GrandWarlock = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(178.811996f, 292.377991f, -8.190210f, 16807);
		if (GrandWarlock)	// any emotes needed?
		{
			uint32 Counter = 0;
			for (int i = 0; i < 3; i++)
			{
				Unit* Servant = NULL;
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

			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
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

    void OnCombatStop(Unit* mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

		if (Started)
			RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
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
			_unit->SetEmoteState(0);
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);

			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();

			Started = true;

			Unit* target = NULL;
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
		if (t > spells[2].casttime && _unit->GetHealthPct() <= 20 && _unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
		{
			_unit->setAttackTimer(2500, false);

			_unit->CastSpell(_unit, spells[2].info, spells[2].instant);

			spells[2].casttime = t + 120;
			return;
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

	// A bit rewritten FindTarget function
	Unit* FindTarget()
	{
		Unit* target = NULL;
		float distance = 50.0f;

		Unit* pUnit;
		float dist;

		for (set<Object*>::iterator itr = _unit->GetInRangeOppFactsSetBegin(); itr != _unit->GetInRangeOppFactsSetEnd(); itr++)
		{
			if(!(*itr)->IsUnit())
				continue;

			pUnit = TO_UNIT((*itr));

			if(pUnit->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_FEIGN_DEATH))
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
		RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = 0;
	}

    void OnCombatStop(Unit* mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		CastTime();
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
// Maybe timer for 'afterspeech' should be added too?
#define CN_WARBRINGER_OMROGG			16809
#define WARBRINGER_OMROGG_THUNDERCLAP	30633
#define WARBRINGER_OMROGG_FEAR			30584
#define WARBRINGER_OMROGG_BURNING_MAUL	30598 // 30598 or 30599
#define WARBRINGER_OMROGG_BLAST_WAVE	30600

#define CN_LEFT_HEAD					19523
#define CN_RIGHT_HEAD					19524

void SpellFunc_Warbringer_BurningMaul(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);

class WarbringerOmroggAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION( WarbringerOmroggAI, MoonScriptCreatureAI );
	WarbringerOmroggAI( Creature* pCreature ) : MoonScriptCreatureAI( pCreature )
	{
		AddSpell( WARBRINGER_OMROGG_THUNDERCLAP, Target_Self, 25, 1, 12 );
		AddSpell( WARBRINGER_OMROGG_FEAR, Target_Self, 7, 0, 20 );
		AddSpellFunc( &SpellFunc_Warbringer_BurningMaul, Target_Self, 100, 0, 30 );
		mBlastWave = AddSpell( WARBRINGER_OMROGG_BLAST_WAVE, Target_Self, 0, 1, 0 );
		mBlastWaveTimer = mSpeechTimer = mSpeechId = mAggroShiftTimer = INVALIDATE_TIMER;
		mRightHead = mLeftHead = NULL;
	}

	void OnCombatStart(Unit* pTarget)
	{
		ParentClass::OnCombatStart( pTarget );
		mAggroShiftTimer = AddTimer( 20000 + RandomUInt( 10 ) * 1000 );
		mBlastWaveTimer = mSpeechTimer = mSpeechId = INVALIDATE_TIMER;
		mLeftHead = SpawnCreature( 19523 );
		mRightHead = SpawnCreature( 19524 );
		if ( mLeftHead != NULL )
		{
			mLeftHead->GetUnit()->GetAIInterface()->SetUnitToFollow( _unit );
		}
		if ( mRightHead != NULL )
		{
			mRightHead->GetUnit()->GetAIInterface()->SetUnitToFollow( _unit );
		}

		if ( mLeftHead == NULL || mRightHead == NULL )
			return;

		switch( RandomUInt( 2 ) )
		{
		case 0:
			mLeftHead->Emote( "If you nice me let you live.", Text_Yell, 10308 );
			mSpeechTimer = AddTimer( 4000 );
			mSpeechId = 1;
			break;
		case 1:
			mLeftHead->Emote( "Me hungry!", Text_Yell, 10309 );
			mSpeechTimer = AddTimer( 2500 );
			mSpeechId = 2;
			break;
		case 2:
			mLeftHead->Emote( "Smash!", Text_Yell, 10306 );
			mSpeechTimer = AddTimer( 2000 );
			mSpeechId = 3;
			break;
		}
	}

	void OnCombatStop(Unit* pTarget)
	{
		ParentClass::OnCombatStop( pTarget );
		if ( IsAlive() )
		{
			if ( mLeftHead != NULL )
			{
				mLeftHead->Despawn( 1000 );
				mLeftHead = NULL;
			}
			if ( mRightHead != NULL )
			{
				mRightHead->Despawn( 1000 );
				mRightHead = NULL;
			}
		}
	}

	void OnTargetDied(Unit* pTarget)
    {
		if ( mLeftHead == NULL || mRightHead == NULL || mSpeechTimer != INVALIDATE_TIMER )
			return;

		switch ( RandomUInt( 1 ) )
		{
		case 0:
			mLeftHead->Emote( "I'm tired. You kill the next one!", Text_Yell, 10320 );
			break;
		case 1:
			mLeftHead->Emote( "This one die easy!", Text_Yell, 10310 );
			mSpeechTimer = AddTimer( 3000 );
			mSpeechId = 0;
			break;
		}
    }

	void OnDied(Unit* pKiller)
    {
		ParentClass::OnDied( pKiller );
		if ( mLeftHead == NULL || mRightHead == NULL )
			return;

		mLeftHead->Emote( "This all... your fault!", Text_Yell, 10311 );
		mLeftHead->Despawn( 1000 );
		mLeftHead = NULL;
		mRightHead->RegisterAIUpdateEvent( 3000 );
		mRightHead->Despawn( 4000 );
		mRightHead = NULL;
    }

	void AIUpdate()
	{
		ParentClass::AIUpdate();

		if ( mSpeechTimer != INVALIDATE_TIMER && IsTimerFinished( mSpeechTimer ) )
		{
			bool ResetSpeech = true;
			RemoveTimer( mSpeechTimer );
			if ( mLeftHead != NULL && mRightHead != NULL )
			{
				switch ( mSpeechId )
				{
				case 0:
					mRightHead->Emote( "That's because I do all the hard work!", Text_Yell, 10321 );
					break;
				case 1:
					mRightHead->Emote( "No, we will NOT let you live!", Text_Yell, 10318 );
					break;
				case 2:
					mRightHead->Emote( "You always hungry. That why we so fat!", Text_Yell, 10319 );
					break;
				case 3:
					mRightHead->Emote( "Why don't you let me do the talking!", Text_Yell, 10317 );
					break;
				case 4:
					mRightHead->Emote( "I'm not done yet, idiot!", Text_Yell, 10313 );
					break;
				case 5:
					mRightHead->Emote( "Bored, he's almost dead!", Text_Yell, 10316 );
					break;
				case 6:
					mRightHead->Emote( "That's not funny!", Text_Yell, 10314 );
					mSpeechTimer = AddTimer( 6000 );
					mSpeechId = 8;
					ResetSpeech = false;
					break;
				case 7:
					mRightHead->Emote( "What are you doing!?", Text_Yell, 10315 );
					break;
				case 8:
					mLeftHead->Emote( "Ha ha ha!", Text_Yell, 10304 );
					break;
				}
			}

			if ( ResetSpeech )
				mSpeechId = -1;
		}
		else if ( IsTimerFinished( mAggroShiftTimer ) )
		{
			ResetTimer( mAggroShiftTimer, 20000 + RandomUInt( 10 ) * 1000 );
			ShiftAggro();
		}

		if ( mBlastWaveTimer != INVALIDATE_TIMER && IsTimerFinished( mBlastWaveTimer ) )
		{
			RemoveTimer( mBlastWaveTimer );
			CastSpell( mBlastWave );
		}
	}

	void ShiftAggro()
	{
		Unit* pTarget = GetBestPlayerTarget( TargetFilter_NotCurrent );
		if ( pTarget != NULL )
		{
			ClearHateList();
			_unit->GetAIInterface()->setNextTarget( pTarget );
			_unit->GetAIInterface()->modThreatByPtr( pTarget, 1000 );

			if ( mLeftHead == NULL || mRightHead == NULL || mSpeechTimer != INVALIDATE_TIMER )
				return;

			switch ( RandomUInt( 6 ) )
			{
			case 0:
				mLeftHead->Emote( "We kill his friend!", Text_Yell, 10301 );
				break;
			case 1:
				mLeftHead->Emote( "Me not like this one...", Text_Yell, 10300 );
				mSpeechTimer = AddTimer( 3000 );
				mSpeechId = 4;
				break;
			case 2:
				mRightHead->Emote( "Hey, you numbskull!", Text_Yell, 10312 );
				break;
			case 3:
				mLeftHead->Emote( "Me get bored.", Text_Yell, 10305 );
				mSpeechTimer = AddTimer( 3000 );
				mSpeechId = 5;
				break;
			case 4:
				mRightHead->Emote( "What are you doing!?", Text_Yell, 10315 );
				break;
			case 5:
				mLeftHead->Emote( "You stay here. Me go kill someone else!", Text_Yell, 10303 );
				mSpeechTimer = AddTimer( 4000 );
				mSpeechId = 6;
				break;
			case 6:
				mLeftHead->Emote( "Me kill someone else!", Text_Yell, 10302 );
				mSpeechTimer = AddTimer( 3000 );
				mSpeechId = 7;
				break;
			}
		}
	}

	MoonScriptCreatureAI*	mLeftHead;
	MoonScriptCreatureAI*	mRightHead;
	int32					mAggroShiftTimer;
	int32					mBlastWaveTimer;
	int32					mSpeechTimer;
	int32					mSpeechId;
	SpellDesc*				mBlastWave;
};

void SpellFunc_Warbringer_BurningMaul(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType)
{
	WarbringerOmroggAI *Warbringer = (pCreatureAI) ? (WarbringerOmroggAI*)pCreatureAI : NULL;
	if ( Warbringer != NULL )
	{
		Warbringer->CastSpell( Warbringer->mBlastWave );
		Warbringer->mBlastWaveTimer = Warbringer->AddTimer( RandomUInt( 5 ) + 5 );
	}
}

class HeadAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION( HeadAI, MoonScriptCreatureAI );
	HeadAI( Creature* pCreature ) : MoonScriptCreatureAI( pCreature )
	{
		SetScale( 4.0f );
		_unit->SetUInt32Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE );
		_unit->m_noRespawn = true;
	}

	void AIUpdate()
	{
		if ( _unit->GetEntry() != CN_RIGHT_HEAD )
			return;

		Emote( "I... hate... you!", Text_Yell, 10322 );
		RemoveAIUpdateEvent();								// Dangerous!
	}

	void Destroy()
	{
		Unit* pUnit = ForceCreatureFind( CN_WARBRINGER_OMROGG );
		if ( pUnit != NULL && TO_CREATURE(pUnit)->GetScript() != NULL )
		{
			WarbringerOmroggAI *pAI = static_cast< WarbringerOmroggAI* >( TO_CREATURE(pUnit)->GetScript() );
			if ( pAI->mLeftHead == ( MoonScriptCreatureAI* )( this ) )
				pAI->mLeftHead = NULL;
			if ( pAI->mRightHead == ( MoonScriptCreatureAI* )( this ) )
				pAI->mRightHead = NULL;
		}
	}
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

		RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));

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

    void OnCombatStop(Unit* mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The true horde... will... prevail!");
		_unit->PlaySoundToSet(10328);
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		uint32 t = (uint32)time(NULL);
		if (t > spells[0].casttime && _unit->GetAIInterface()->getNextTarget() && _unit->GetCurrentSpell() == NULL)
		{
			_unit->setAttackTimer(1500, false);

			_unit->CastSpell(_unit->GetAIInterface()->getNextTarget(), spells[0].info, spells[0].instant);

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
	mgr->register_creature_script(CN_SHADOWMOON_ACOLYTE, &SHADOWMOONACOLYTEAI::Create);
	mgr->register_creature_script(CN_SHATTERED_HAND_ASSASSIN, &SHATTEREDHANDASSASSINAI::Create);
	mgr->register_creature_script(CN_SHATTERED_HAND_GLADIATOR, &SHATTEREDHANDGLADIATORAI::Create);	
	mgr->register_creature_script(CN_SHATTERED_HAND_HOUNDMASTER, &SHATTEREDHANDHOUNDMASTERAI::Create);	
	mgr->register_creature_script(CN_SHATTERED_HAND_REAVER, &SHATTEREDHANDREAVERAI::Create);	
	mgr->register_creature_script(CN_SHATTERED_HAND_SENTRY, &SHATTEREDHANDSENTRYAI::Create);
	mgr->register_creature_script(CN_SHATTERED_HAND_SHARPSHOOTER, &SHATTEREDHANDSHARPSHOOTERAI::Create);
	mgr->register_creature_script(CN_SHATTERED_HAND_BRAWLER, &SHATTEREDHANDBRAWLERAI::Create);
	mgr->register_creature_script(CN_SHADOWMOON_DARKCASTER, &ShadowmoonDarkcasterAI::Create);
	mgr->register_creature_script(CN_GRAND_WARLOCK_NETHEKURSE, &GrandWarlockNethekurseAI::Create);
	mgr->register_creature_script(CN_BLOOD_GUARD_PORUNG, &BloodGuardPorungAI::Create);
	mgr->register_creature_script(CN_WARBRINGER_OMROGG, &WarbringerOmroggAI::Create);
	mgr->register_creature_script(CN_LEFT_HEAD, &HeadAI::Create);
	mgr->register_creature_script(CN_RIGHT_HEAD, &HeadAI::Create);
	mgr->register_creature_script(CN_WARCHIEF_KARGATH_BLADEFIST, &WarchiefKargathBladefistAI::Create);
}

// TO DO: Shattered Hand Executioner 17301, Shattered Hand Champion 17671, 
// Shattered Hand Centurion 17465, Shattered Hand Blood Guard 17461, 
// Shattered Hand Archer 17427, Sharpshooter Guard 17622, Shattered Hand Zealot 17462
// (lack of infos or don't have any spells!) more?
