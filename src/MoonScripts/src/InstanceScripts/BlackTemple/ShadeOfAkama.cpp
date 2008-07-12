/*
 * Moon++ Scripts for Ascent MMORPG Server
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

#include "StdAfx.h"
#include "../Setup.h"
#include "../Base.h"

//Shade of AkamaAI

#define CN_SHADE_OF_AKAMA 22841

//Adds
#define CN_ASHTONGUE_CHANNELER 23421
#define CN_ASHTONGUE_DEFENDER 23216
#define CN_ASHTONGUE_ELEMENTALIST 23523
#define CN_ASHTONGUE_ROGUE 23318
#define CN_ASHTONGUE_SPIRITBINDER 23524

#define SINFUL_BEAM0 00000

class ShadeofakamaAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ShadeofakamaAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    ShadeofakamaAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		} 
		spells[0].info = dbcSpell.LookupEntry(SINFUL_BEAM0);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 2000;
		
		_unit->CastSpell(_unit, 41408, false); //ShadowForm Visual
		_unit->m_canMove = false;
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		hm = 100;
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnTargetDied(Unit* mTarget)
    {
		if (_unit->GetHealthPct() > 0)	
		{
			int RandomSpeach;
			RandomUInt(1000);
			RandomSpeach=rand()%3;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I will not last much longer...");
				_unit->PlaySoundToSet(11385);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "It is time to free your soul from Illidan's grasp!");
				//_unit->PlaySoundToSet(11510);
				break;

			}
		}
    }

    void OnCombatStop(Unit *mTarget)
    {
		hm = 100;
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		Creature *cre = NULL;
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I don't want to go back!");
		_unit->PlaySoundToSet(11420);
        cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(22990, 
        643.741f, 305.852f,
        271.689f, 0.00628f, 
        true, false, 0, 0); 
		//cre->GetAIInterface()->setOutOfCombatRange(30000);
       RemoveAIUpdateEvent();
    }

	void spawnAdds(float x, float y, float z, float o)
	{
        _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ASHTONGUE_CHANNELER, x, y, z, o, true, false, 0, 0);

        _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ASHTONGUE_DEFENDER, x, y, z, o, true, false, 0, 0);

        _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ASHTONGUE_ELEMENTALIST, x, y, z, o, true, false, 0, 0);

        _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ASHTONGUE_ROGUE, x, y, z, o, true, false, 0, 0);

        _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ASHTONGUE_SPIRITBINDER, x, y, z, o, true, false, 0, 0);
	}

    void AIUpdate()
	{
		if(_unit->GetHealthPct() <= 85 && hm == 100) 
        {
            Creature *cre = NULL;
            for(int i = 0; i < 2; i++)
            {
                cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(23421, 
                    _unit->GetPositionX(), _unit->GetPositionY(),
                    _unit->GetPositionZ(), _unit->GetOrientation(),
                    true, false, 0, 0);
				if (cre)
					cre->GetAIInterface()->setOutOfCombatRange(30000);
			}
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Abandon all hope! The legion has returned to finish what was begun so many years ago. This time there will be no escape!");
		_unit->PlaySoundToSet(10999);
		hm=85;
		}
		else if(_unit->GetHealthPct() <= 70 && hm == 85)
        {
            Creature *cre = NULL;
            for(int i = 0; i < 2; i++)
            {
                cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(23215, 
                    _unit->GetPositionX(), _unit->GetPositionY(),
                    _unit->GetPositionZ(), _unit->GetOrientation(),
                    true, false, 0, 0); 
				if (cre)
					cre->GetAIInterface()->setOutOfCombatRange(30000);
			}
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Abandon all hope! The legion has returned to finish what was begun so many years ago. This time there will be no escape!");
		_unit->PlaySoundToSet(10999);
		hm=70;
		}
		else if(_unit->GetHealthPct() <= 55 && hm == 70)
        {
            Creature *cre = NULL;
            for(int i = 0; i < 2; i++)
            {
                cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(23216, 
                    _unit->GetPositionX(), _unit->GetPositionY(),
                    _unit->GetPositionZ(), _unit->GetOrientation(),
                    true, false, 0, 0); 
				if (cre)
					cre->GetAIInterface()->setOutOfCombatRange(30000);
			}
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Abandon all hope! The legion has returned to finish what was begun so many years ago. This time there will be no escape!");
		_unit->PlaySoundToSet(10999);
		hm=55;
		}
		else if(_unit->GetHealthPct() <= 40 && hm == 55)
        {
            Creature *cre = NULL;
            for(int i = 0; i < 2; i++)
            {
                cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(23523, 
                    _unit->GetPositionX(), _unit->GetPositionY(),
                    _unit->GetPositionZ(), _unit->GetOrientation(),
                    true, false, 0, 0); 
				if (cre)
					cre->GetAIInterface()->setOutOfCombatRange(30000);
			}
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Abandon all hope! The legion has returned to finish what was begun so many years ago. This time there will be no escape!");
		_unit->PlaySoundToSet(10999);
		hm=40;
		}
		else if(_unit->GetHealthPct() <= 25 && hm == 40)
        {
            Creature *cre = NULL;
            for(int i = 0; i < 5; i++)
            {
                cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(23318, 
                    _unit->GetPositionX(), _unit->GetPositionY(),
                    _unit->GetPositionZ(), _unit->GetOrientation(),
                    true, false, 0, 0); 
				if (cre)
					cre->GetAIInterface()->setOutOfCombatRange(30000);
			}
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Abandon all hope! The legion has returned to finish what was begun so many years ago. This time there will be no escape!");
		_unit->PlaySoundToSet(10999);
		hm=25;
		}
		else if(_unit->GetHealthPct() <= 10 && hm == 25)
        {
            Creature *cre = NULL;
            for(int i = 0; i < 5; i++)
            {
                cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(23524, 
                    _unit->GetPositionX(), _unit->GetPositionY(),
                    _unit->GetPositionZ(), _unit->GetOrientation(),
                    true, false, 0, 0); 
				if (cre)
					cre->GetAIInterface()->setOutOfCombatRange(30000);
			}
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Abandon all hope! The legion has returned to finish what was begun so many years ago. This time there will be no escape!");
		_unit->PlaySoundToSet(10999);
		hm=10;
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
					}

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
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
	int hm;
};


//mobs

// Ashtongue Defender AI
#define DEBILITATING_STRIKE 41178 //


class AshtonguedefenderAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(AshtonguedefenderAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    AshtonguedefenderAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(DEBILITATING_STRIKE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 3.0f;
		spells[0].attackstoptimer = 1000;


    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }


	void OnTargetDied(Unit* mTarget)
    {
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

// AshtongueElementalist AI
#define RAIN_OF_FIRE 42023


class AshtongueelementalistAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(AshtongueelementalistAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    AshtongueelementalistAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(RAIN_OF_FIRE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 1000;


    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }


	void OnTargetDied(Unit* mTarget)
    {
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

// AshtongueRogue AI
#define DEBILITATING_POISON 41978


class AshtonguerogueAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(AshtonguerogueAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    AshtonguerogueAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(DEBILITATING_POISON);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 1000;


    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }


	void OnTargetDied(Unit* mTarget)
    {
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

// AshtongueRogue AI
#define SPIRIT_MEND 42025
#define AS_CHAIN_HEAL 42025


class AshtonguespiritbinderAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(AshtonguespiritbinderAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    AshtonguespiritbinderAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(SPIRIT_MEND);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 1000;
		
		spells[1].info = dbcSpell.LookupEntry(AS_CHAIN_HEAL);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = false;
		spells[1].perctrigger = 5.0f;
		spells[1].attackstoptimer = 1000;


    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }


	void OnTargetDied(Unit* mTarget)
    {
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

// Ashtongue Channeler AI
#define AKAMA_CHANNEL 30166


class AshtongueChannelerAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(AshtongueChannelerAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    AshtongueChannelerAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		_unit->m_canMove = false;
		_unit->setAttackTimer(2000, false);
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

		Unit* SoAkama = NULL;
		SoAkama = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(442.804f, 401.362f ,118.538f , CN_SHADE_OF_AKAMA);
		if(SoAkama)
		{
			_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, SoAkama->GetGUID());
			_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, AKAMA_CHANNEL);
		}
		
    }
    
    void OnCombatStart(Unit* mTarget)
    {
    }


	void OnTargetDied(Unit* mTarget)
    {
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
		_unit->setAttackTimer(2000, false);
    }

protected:
};

void SetupShadeOfAkama(ScriptMgr * mgr)
{
	mgr->register_creature_script(CN_SHADE_OF_AKAMA, &ShadeofakamaAI::Create);
 
	mgr->register_creature_script(CN_ASHTONGUE_DEFENDER, &AshtonguedefenderAI::Create);
	mgr->register_creature_script(CN_ASHTONGUE_ELEMENTALIST, &AshtongueelementalistAI::Create);
	mgr->register_creature_script(CN_ASHTONGUE_ROGUE, &AshtonguerogueAI::Create);
	mgr->register_creature_script(CN_ASHTONGUE_SPIRITBINDER, &AshtonguespiritbinderAI::Create);
	mgr->register_creature_script(CN_ASHTONGUE_CHANNELER, &AshtongueChannelerAI::Create);
}