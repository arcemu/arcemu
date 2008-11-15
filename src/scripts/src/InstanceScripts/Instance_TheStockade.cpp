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
/* Instance_TheStockade.cpp Script										*/
/************************************************************************/

#define CN_KAMDEEPFURY 1666

#define KAM_SHIELDSLAM 15655

class DeepfuryAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(DeepfuryAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    DeepfuryAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
        nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(KAM_SHIELDSLAM);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 0.0f;
		spells[0].cooldown = 10;
		spells[0].attackstoptimer = 1000;
    }

    void OnCombatStart(Unit* mTarget)
    {
        CastTime();
        RegisterAIUpdateEvent(1000);
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
		float val = RandomFloat(100.0f);
		SpellCast(val);
    }

    void CastTime()
    {
		for(int i=0;i<nrspells;i++)
            spells[i].casttime = spells[i].cooldown;
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
					if (!spells[i].instant)
						_unit->GetAIInterface()->StopMovement(1);

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

#define CN_HAMHOCK 1717

#define CHAINLIGHT 12058
#define DEMORALIZO 23511

class HamhockAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(HamhockAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    HamhockAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
        nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(CHAINLIGHT);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 0.0f;
		spells[0].cooldown = 10;
		spells[0].attackstoptimer = 1000;
		
		spells[1].info = dbcSpell.LookupEntry(DEMORALIZO);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 0.0f;
		spells[1].cooldown = 20;
		spells[1].attackstoptimer = 1000;
    }

    void OnCombatStart(Unit* mTarget)
    {
        CastTime();
        RegisterAIUpdateEvent(1000);
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
		float val = RandomFloat(100.0f);
		SpellCast(val);
    }

    void CastTime()
    {
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
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
					if (!spells[i].instant)
						_unit->GetAIInterface()->StopMovement(1);

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

#define CN_BAZILTHREDD 1716

#define BAZILBOMB 7964

class BazilAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(BazilAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    BazilAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
        nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(BAZILBOMB);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].perctrigger = 0.0f;
		spells[0].cooldown = 12;
		spells[0].attackstoptimer = 1000;
    }

    void OnCombatStart(Unit* mTarget)
    {
        CastTime();
        RegisterAIUpdateEvent(1000);
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
		float val = RandomFloat(100.0f);
		SpellCast(val);
    }

    void CastTime()
    {
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
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
					if (!spells[i].instant)
						_unit->GetAIInterface()->StopMovement(1);

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
		   			case TARGET_RANDOM_SINGLE:{
						std::vector<Unit *> target_list;
						for (set<Player*>::iterator itr = _unit->GetInRangePlayerSetBegin(); itr != _unit->GetInRangePlayerSetEnd(); ++itr)
						{
							target = (Unit*)(*itr);
							if(target)
								target_list.push_back(target);

							target = NULL;
						}

						if(target_list.size())
							target = *(target_list.begin()+rand()%target_list.size());
						if(target)
							_unit->CastSpell(target, spells[i].info, spells[i].instant);

						target = NULL;
						}break;
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

#define CN_DEXTRENWARD 1663

#define INTIMIDATING 38945

class DextrenAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(DextrenAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    DextrenAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
        nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(INTIMIDATING);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 0.0f;
		spells[0].cooldown = 15;
		spells[0].attackstoptimer = 1000;
    }

    void OnCombatStart(Unit* mTarget)
    {
        CastTime();
        RegisterAIUpdateEvent(1000);
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
		float val = RandomFloat(100.0f);
		SpellCast(val);
    }

    void CastTime()
    {
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
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
					if (!spells[i].instant)
						_unit->GetAIInterface()->StopMovement(1);

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

#define CN_DEFINMATE 1708

#define CONVICTREND 13445

class InmateAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(InmateAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    InmateAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
        nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(CONVICTREND);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 0.0f;
		spells[0].cooldown = 12;
		spells[0].attackstoptimer = 1000;
    }

    void OnCombatStart(Unit* mTarget)
    {
        CastTime();
        RegisterAIUpdateEvent(1000);
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
		float val = RandomFloat(100.0f);
		SpellCast(val);
    }

    void CastTime()
    {
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
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
					if (!spells[i].instant)
						_unit->GetAIInterface()->StopMovement(1);

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

#define CN_DEFINSURGENT 1715

#define INSURGENTDEMORALIZO 16244

class InsurgentAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(InsurgentAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    InsurgentAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
        nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(INSURGENTDEMORALIZO);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].perctrigger = 0.0f;
		spells[0].cooldown = 15;
		spells[0].attackstoptimer = 1000;
    }

    void OnCombatStart(Unit* mTarget)
    {
        CastTime();
        RegisterAIUpdateEvent(1000);
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
		float val = RandomFloat(100.0f);
		SpellCast(val);
    }

    void CastTime()
    {
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
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
					if (!spells[i].instant)
						_unit->GetAIInterface()->StopMovement(1);

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

#define CN_DEFPRISONER 1706

#define PRISONKICK 27814
#define PRISONDISARM 22691

class PrisonerAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(PrisonerAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    PrisonerAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
        nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(PRISONKICK);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 0.0f;
		spells[0].cooldown = 11;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(PRISONDISARM);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 0.0f;
		spells[1].cooldown = 15;
		spells[1].attackstoptimer = 1000;
    }

    void OnCombatStart(Unit* mTarget)
    {
        CastTime();
        RegisterAIUpdateEvent(1000);
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
		float val = RandomFloat(100.0f);
		SpellCast(val);
    }

    void CastTime()
    {
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
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
					if (!spells[i].instant)
						_unit->GetAIInterface()->StopMovement(1);

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

#define CN_DEFCONVICT 1711

#define CONVICTBACKHAND 12461

class ConvictAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(ConvictAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    ConvictAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(CONVICTBACKHAND);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 0.0f;
		spells[0].cooldown = 10;
		spells[0].attackstoptimer = 1000;
    }

    void OnCombatStart(Unit* mTarget)
    {
        CastTime();
        RegisterAIUpdateEvent(1000);
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
		float val = RandomFloat(100.0f);
		SpellCast(val);
    }

    void CastTime()
    {
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
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
					if (!spells[i].instant)
						_unit->GetAIInterface()->StopMovement(1);

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

void SetupTheStockade(ScriptMgr * mgr)
{
	mgr->register_creature_script(CN_KAMDEEPFURY, &DeepfuryAI::Create);
	mgr->register_creature_script(CN_HAMHOCK, &HamhockAI::Create);
	mgr->register_creature_script(CN_BAZILTHREDD, &BazilAI::Create);
	mgr->register_creature_script(CN_DEXTRENWARD, &DextrenAI::Create);
	mgr->register_creature_script(CN_DEFINMATE, &InmateAI::Create);
	mgr->register_creature_script(CN_DEFINSURGENT, &InsurgentAI::Create);
	mgr->register_creature_script(CN_DEFPRISONER, &PrisonerAI::Create);
	mgr->register_creature_script(CN_DEFCONVICT, &ConvictAI::Create);
}