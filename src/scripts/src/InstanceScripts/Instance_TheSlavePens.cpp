/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009 ArcEmu Team <http://www.arcemu.org/>
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
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
/* Instance_TheSlavePens.cpp Script		                                */
/************************************************************************/

// Coilfang Champion AI

#define CN_COILFANG_CHAMPION 17957

#define INTIMIDATING_SHOUT 33789 // or 38945 || after it on off can go to next unfeared target (mostly healer or ranged unit)
// It should also have effect on other allies of target, but somehow it affects caster too (we can use only this: 38946 as workaround
// But I think it's better to leave it as it is to not change it in a future as this effect will be repaired)
// In Heroid Mode is immune to Mind Control and Seduction (same for Bogstrok)

class COILFANGCHAMPIONAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(COILFANGCHAMPIONAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    COILFANGCHAMPIONAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(INTIMIDATING_SHOUT);
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

	void OnTargetDied(Unit* mTarget)
    {
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

// Coilfang Observer AI

#define CN_COILFANG_OBSERVER 17938

#define IMMOLATE 29928
// In Heroic mode, it becomes immune to Mind Control and Seduction, but can still be feared and frozen by traps. 

class COILFANGOBSERVERAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(COILFANGOBSERVERAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    COILFANGOBSERVERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(IMMOLATE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].cooldown = 35;
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

	void OnTargetDied(Unit* mTarget)
    {
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


// Coilfang Defender AI

#define CN_COILFANG_DEFENDER 17958

#define REFLECTIVE_SHIELD 41475 // No idea which id it should be: Reflective Damage Shield (35159), Reflective Magic Shield (35158), Reflective Shield (41475)
// Cannot be trapped or feared, but can be stunned, polymorphed, cycloned and disarmed.
// In Heroic, immune to mind control, seduce and sheep, but still vulnerable to stun and kiting.
// Stealth Detectors, thus cannot be sapped.

class COILFANGDEFENDERAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(COILFANGDEFENDERAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    COILFANGDEFENDERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(REFLECTIVE_SHIELD);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true; // for other spells than 41475 false!
		spells[0].cooldown = 40; // 20
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

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

	void OnTargetDied(Unit* mTarget)
    {
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


// Coilfang Scale-Healer AI

#define CN_COILFANG_SCALE_HEALER 21126

#define HOLY_NOVA 37669 // can be: 37669, 34944, 41380, 40096, 36985
#define POWER_WORD_SHIELD 36052 // can be also: 36052, 29408, 41373, 32595, 35944
#define GREATER_HEAL 35096 // all spellids are just my thoughtfulness

// Priests can Mind Control these; the surrounding mobs will kill them quickly.
// Note: Idk if it casts those spells, but it has them when it's mind controlled.
class COILFANGSCALEHEALERAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(COILFANGSCALEHEALERAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    COILFANGSCALEHEALERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(HOLY_NOVA);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].cooldown = 20;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(POWER_WORD_SHIELD);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].cooldown = 45;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(GREATER_HEAL);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = false;
		spells[2].cooldown = 50;
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

	void OnTargetDied(Unit* mTarget)
    {
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


// Coilfang Soothsayer AI

#define CN_COILFANG_SOOTHSAYER 17960

#define MIND_CONTROL 36797 // maybe: 36797 or 36798 or ... no idea to id, but 
						   // still spell doesn't work, because of lack of core support

// All forms of crowd control work on it.
class COILFANGSOOTHSAYERAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(COILFANGSOOTHSAYERAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    COILFANGSOOTHSAYERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(MIND_CONTROL);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 40;
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

	void OnTargetDied(Unit* mTarget)
    {
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


// Coilfang Technician AI

#define CN_COILFANG_TECHNICIAN 17940

#define RAIN_OF_FIRE 34435 // can be: 34360, 34435, 37465, 38635, 39024, 31340, 33617, 39363
#define BLIZZARD 30093 // can be: 30093, 29951, 37263, 38646, 31266, 34356

// Can be seduced and mind controlled in heroic mode.
// Note: Idk if it casts those spells, but it has them when it's mind controlled.
class COILFANGTECHNICIANAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(COILFANGTECHNICIANAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    COILFANGTECHNICIANAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(RAIN_OF_FIRE);
		spells[0].targettype = TARGET_DESTINATION;
		spells[0].instant = true;
		spells[0].cooldown = 35;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(BLIZZARD);
		spells[1].targettype = TARGET_DESTINATION; 
		spells[1].instant = false;
		spells[1].cooldown = 20;
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

	void OnTargetDied(Unit* mTarget)
    {
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

// Coilfang Ray AI

#define CN_COILFANG_RAY 21128

#define HOWL_OF_TERROR 39048 

// All forms of Beast crowd control work. 
class COILFANGRAYAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(COILFANGRAYAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    COILFANGRAYAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(HOWL_OF_TERROR);
		spells[0].targettype = TARGET_VARIOUS; 
		spells[0].instant = false;
		spells[0].cooldown = 20;
		spells[0].perctrigger = 2.0f;
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

	void OnTargetDied(Unit* mTarget)
    {
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

// TotemsAI

#define CN_MENNUS_HEALING_WARD		20208
#define CN_TAINED_EARTHGRAB_TOTEM	18176
#define CN_TAINED_STONESKIN_TOTEM	18177
#define CN_CORRUPTED_NOVA_TOTEM		14662	// wrong id?

#define HW_MENNUS_HEALING_WARD		34977
#define ET_ENTANGLING_ROOTS			20654
#define ST_		31985
#define NT_		31991

class TotemsAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(TotemsAI);

    TotemsAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		uint32 Despawn = 30000;
		uint32 AIUpdate = 1000;

		SpellID = 1;
		switch (_unit->GetEntry())
		{
		case CN_MENNUS_HEALING_WARD:
			SpellID = 34977;
			break;
		case CN_TAINED_EARTHGRAB_TOTEM:
			SpellID = 20654;
			AIUpdate = 5000;
			break;
		case CN_TAINED_STONESKIN_TOTEM:
			Despawn = 60000;
			SpellID = 25509;	// temporary spell
			AIUpdate = 0;
			break;
		default:	// for Corrupted Nova Totem and it's also safe case
			{
				_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);	// hax
				Despawn = 6000;
				SpellID = 33132;
				AIUpdate = 5000;
			}
		}

		if (AIUpdate != 0)
			RegisterAIUpdateEvent(AIUpdate);

		_unit->GetAIInterface()->setCurrentAgent(AGENT_SPELL);
		_unit->Despawn(Despawn, 0);
		_unit->m_noRespawn = true;

		_unit->CastSpell(_unit, SpellID, true);
    }

	void OnDied(Unit* mKiller)
    {
		if (_unit->GetEntry() != CN_CORRUPTED_NOVA_TOTEM)
			RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		if (_unit->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2))
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);

		_unit->CastSpell(_unit, SpellID, true);
    }

protected:

	uint32 SpellID;
};

// Mennu the BetrayerAI

#define CN_MENNU_THE_BETRAYER 17941

#define MENNUS_HEALING_WARD			34980
#define TAINTED_EARTHGRAB_TOTEM		31981 //31982 //31981
#define TAINTED_STONESKIN_TOTEM		31985
#define CORRUPTED_NOVA_TOTEM		31991
#define LIGHTNING_BOLT				36152
// First 4 spells don't work as more core support is needed for them

uint32 Totems[4] = { 20208, 18176, 18177, 14662 };

class MennuTheBetrayerAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(MennuTheBetrayerAI);
	SP_AI_Spell spells[5];
	bool m_spellcheck[5];
	bool SummonedTotems[4];

    MennuTheBetrayerAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i = 0; i < nrspells; i++)
		{
			m_spellcheck[i] = false;
		}

		for (int i = 0; i < 4; i++)
			SummonedTotems[i] = false;

		spells[0].info = dbcSpell.LookupEntry(LIGHTNING_BOLT);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].cooldown = 15;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 5000;

        spells[1].info = dbcSpell.LookupEntry(MENNUS_HEALING_WARD);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].cooldown = 20;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 1000;

        spells[2].info = dbcSpell.LookupEntry(TAINTED_EARTHGRAB_TOTEM);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;
		spells[2].cooldown = 0;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;

        spells[3].info = dbcSpell.LookupEntry(TAINTED_STONESKIN_TOTEM);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = true;
		spells[3].cooldown = 0;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;

        spells[4].info = dbcSpell.LookupEntry(CORRUPTED_NOVA_TOTEM);
		spells[4].targettype = TARGET_SELF;
		spells[4].instant = true;
		spells[4].cooldown = 0;
		spells[4].perctrigger = 0.0f;
		spells[4].attackstoptimer = 1000;

		TotemCounter = 0;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;

		for (int i = 0; i < 4; i++)
			SummonedTotems[i] = false;

		TotemCounter = 0;

		int RandomSpeach = rand()%3;
		switch (RandomSpeach)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The work must continue!");
			_unit->PlaySoundToSet(10376);
			break;
		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You brought this on yourselves!");
			_unit->PlaySoundToSet(10378);
			break;
		case 2:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Don't make me kill you!");
			_unit->PlaySoundToSet(10379);
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
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "It had to be done!");
				_unit->PlaySoundToSet(10380);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You should not have come!");
				_unit->PlaySoundToSet(10381);
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
	   _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I... Deserve this...");
       _unit->PlaySoundToSet(10382);

       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		if (TotemCounter != 0 && _unit->GetCurrentSpell() == NULL)
		{
			TotemSpawning();
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
					if (!spells[i].instant)
						_unit->GetAIInterface()->StopMovement(1);

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
					{
						spells[1].casttime = (uint32)time(NULL) + spells[1].cooldown;
						TotemSpawning();
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
	// Random totem spawning
	void TotemSpawning()
	{
		_unit->setAttackTimer(1500, false);

		bool Spawned = false;
		uint32 Counter = 0;
		while (!Spawned)
		{
			if (Counter >= 2)
			{
				for (int i = 0; i < 4; i++)
				{
					if (!SummonedTotems[i])
					{
						_unit->GetMapMgr()->GetInterface()->SpawnCreature(Totems[i], _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation(), true, false, 0, 0);
						_unit->CastSpell(_unit, spells[i+1].info, spells[i+1].instant);

						SummonedTotems[i] = true;
						TotemCounter++;
						break;
					}
				}

				Spawned = true;
			}

			uint32 i = rand()%4;
			if (SummonedTotems[i])
				Counter++;
			else
			{
				_unit->GetMapMgr()->GetInterface()->SpawnCreature(Totems[i], _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation(), true, false, 0, 0);
				_unit->CastSpell(_unit, spells[i+1].info, spells[i+1].instant);

				SummonedTotems[i] = true;
				TotemCounter++;
				Spawned = true;
			}
		}

		if (TotemCounter == 4)
		{
			for (int i = 0; i < 4; i++)
				SummonedTotems[i] = false;

			TotemCounter = 0;
		}
	}

protected:

	uint32 TotemCounter;
	int nrspells;
};

// Rokmar the CracklerAI

#define CN_ROKMAR_THE_CRACKLER 17991

#define GRIEVOUS_WOUND	31956
#define WATER_SPIT		35008
#define ENSNARING_MOSS	31948
#define ENRAGE			37023 // ofc not sure ;) maybe: 41305
// boss without sounds

class RokmarTheCracklerAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(RokmarTheCracklerAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    RokmarTheCracklerAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		}

        spells[0].info = dbcSpell.LookupEntry(GRIEVOUS_WOUND);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 20;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = dbcSpell.LookupEntry(WATER_SPIT);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = false;
		spells[1].cooldown = 10;
		spells[1].perctrigger = 16.0f;
		spells[1].attackstoptimer = 1000;

        spells[2].info = dbcSpell.LookupEntry(ENSNARING_MOSS);
		spells[2].targettype = TARGET_RANDOM_SINGLE;
		spells[2].instant = false;
		spells[2].cooldown = 35;
		spells[2].perctrigger = 8.0f;
		spells[2].attackstoptimer = 1000;
		spells[2].mindist2cast = 0.0f;
		spells[2].maxdist2cast = 30.0f;

        spells[3].info = dbcSpell.LookupEntry(ENRAGE);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = true;
		spells[3].cooldown = 0;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;

		Enraged = false;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		for(int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;

		RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));

		Enraged = false;
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
		if(_unit->GetHealthPct() <= 20 && !Enraged)
		{
			_unit->CastSpell(_unit, spells[3].info, spells[3].instant);

			Enraged = true;
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
					if (!spells[i].instant)
						_unit->GetAIInterface()->StopMovement(1);

					target = _unit->GetAIInterface()->getNextTarget();
					if ((i == 0 && _unit->GetDistance2dSq(target) <= 100.0f) || i != 0)
					{
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

	bool Enraged;
	int nrspells;
};


// QuagmirranAI

#define CN_QUAGMIRRAN 17942

#define ACID_GEYSER			38971 // it isn't right spell (TOO POWERFUL), but I couldn't find correct one for now (as others Idk why don't want to work)
#define POISON_BOLT_VOLLEY	39340 // maybe be also: 40095, but it isn't dispelable
#define CLEAVE				38474 // 31345, no idea if this is correct

class QuagmirranAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(QuagmirranAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    QuagmirranAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(ACID_GEYSER);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = false;
		spells[0].cooldown = 20;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].mindist2cast = 0.0f;
		spells[0].maxdist2cast = 40.0f;

        spells[1].info = dbcSpell.LookupEntry(POISON_BOLT_VOLLEY);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].cooldown = 10;
		spells[1].perctrigger = 15.0f;
		spells[1].attackstoptimer = 2000;

        spells[2].info = dbcSpell.LookupEntry(CLEAVE);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].cooldown = 15;
		spells[2].perctrigger = 6.0f;
		spells[2].attackstoptimer = 2000;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		for(int i=0; i < nrspells; i++)
			spells[i].casttime = 0;

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
					if (!spells[i].instant)
						_unit->GetAIInterface()->StopMovement(1);

					target = _unit->GetAIInterface()->getNextTarget();
					if ((i == 2 && _unit->GetDistance2dSq(target) <= 100.0f) || i != 2)
					{
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

			if (i == 0)
			{
				_unit->GetAIInterface()->StopMovement(9000);
				_unit->setAttackTimer(9000, false);
			}

			TargetTable.clear();
		}
	}

protected:

	int nrspells;
};

void SetupTheSlavePens(ScriptMgr * mgr)
{
    /*mgr->register_creature_script(CN_COILFANG_CHAMPION, &COILFANGCHAMPIONAI::Create);
	mgr->register_creature_script(CN_COILFANG_OBSERVER, &COILFANGOBSERVERAI::Create);
	mgr->register_creature_script(CN_COILFANG_DEFENDER, &COILFANGDEFENDERAI::Create);
	mgr->register_creature_script(CN_COILFANG_SCALE_HEALER, &COILFANGSCALEHEALERAI::Create);
	mgr->register_creature_script(CN_COILFANG_SOOTHSAYER, &COILFANGSOOTHSAYERAI::Create);
	mgr->register_creature_script(CN_COILFANG_TECHNICIAN, &COILFANGTECHNICIANAI::Create);
	mgr->register_creature_script(CN_COILFANG_RAY, &COILFANGRAYAI::Create);*/
	mgr->register_creature_script(CN_MENNUS_HEALING_WARD, &TotemsAI::Create);
	mgr->register_creature_script(CN_TAINED_EARTHGRAB_TOTEM, &TotemsAI::Create);
	mgr->register_creature_script(CN_TAINED_STONESKIN_TOTEM, &TotemsAI::Create);
	mgr->register_creature_script(CN_CORRUPTED_NOVA_TOTEM, &TotemsAI::Create);
	mgr->register_creature_script(CN_MENNU_THE_BETRAYER, &MennuTheBetrayerAI::Create);
	mgr->register_creature_script(CN_ROKMAR_THE_CRACKLER, &RokmarTheCracklerAI::Create);
	mgr->register_creature_script(CN_QUAGMIRRAN, &QuagmirranAI::Create);
}

// Notes: Coilfang Slavemaster was already scripted in SteamVaults, so I haven't
// copied/pasted it here.
// Still many NPCs left and I don't have infos if any of those use any spell