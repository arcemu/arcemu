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
#include "Setup.h"
int rajaxx;
int moamsummonkill;
//RuinsofAhnQiraj.cpp 

//Bosses AI

//Ayamiss_the_Hunter
#define AYAMISS_THE_HUNTER 15369
//npc
#define HIVEZARA_SWARMER  15546
#define HIVEZARA_WASP 15325
#define HIVE_ZARA_LARVA 15555
//spells
#define FLY 34873 //dbc untested
#define STINGER_SPRAY 25749 //dbc untested
#define POISON_STINGER  25748 //dbc untested
#define PARALYZE 25725 //untested
#define DAMAGE_ABSORB 25750 
//to do fly

class AYAMISSTHEHUNTERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(AYAMISSTHEHUNTERAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    AYAMISSTHEHUNTERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
		spells[0].info = dbcSpell.LookupEntry(PARALYZE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 2.0f;
		spells[0].attackstoptimer = 1000;


		spells[1].info = dbcSpell.LookupEntry(STINGER_SPRAY); 
		spells[1].targettype = TARGET_DESTINATION;
		spells[1].instant = true;
		spells[1].perctrigger = 2.0f;
		spells[1].attackstoptimer = 1000;


		spells[2].info = dbcSpell.LookupEntry(POISON_STINGER); 
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].perctrigger = 3.0f;
		spells[2].attackstoptimer = 1000;


        spells[3].info = dbcSpell.LookupEntry(DAMAGE_ABSORB); 
		spells[3].targettype = TARGET_SELF; 
		spells[3].instant = true;
		spells[3].perctrigger = 5.0f;
		spells[3].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		nrspells = 4;
		spmin = 0;
		timer = 120;
		fly = 1;
		larvatimer = 60;
		summon = 0;
		RegisterAIUpdateEvent(1000);
		_unit->GetAIInterface()->MoveTo(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() + 8.0f, _unit->GetOrientation());
		_unit->GetAIInterface()->m_canMove = false;
		_unit->GetAIInterface()->disable_melee = true;
    }


	void OnTargetDied(Unit* mTarget)
    {

    }

    void OnCombatStop(Unit *mTarget)
    {
		spmin = 0;
		nrspells = 4;
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
		if(_unit->GetHealthPct() <= 70 && fly)
		{
			_unit->GetAIInterface()->m_canMove = true;
			_unit->GetAIInterface()->disable_melee = false;
			fly = 0;
			
		}
		if (!timer && !summon)
		{
			for(int i=0;i<20;i++)
			{
				Creature *summons = NULL;
				summons = _unit->GetMapMgr()->GetInterface()->SpawnCreature(HIVEZARA_SWARMER, 
				(_unit->GetPositionX() + RandomFloat(5)-5), (_unit->GetPositionY() + RandomFloat(5)-5),
				_unit->GetPositionZ(), _unit->GetOrientation(),
				true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
				summons->GetAIInterface()->setOutOfCombatRange(50000);
				summons->GetAIInterface()->MoveTo(_unit->GetAIInterface()->GetNextTarget()->GetPositionX(),_unit->GetAIInterface()->GetNextTarget()->GetPositionY(),_unit->GetAIInterface()->GetNextTarget()->GetPositionZ(), 0);
				summon = 1;
			}
		}
		else
		{
		timer--;
		}
		if (!larvatimer)
		{
			Creature *summons = NULL;
			summons = _unit->GetMapMgr()->GetInterface()->SpawnCreature(HIVE_ZARA_LARVA, 
			(_unit->GetPositionX() + RandomFloat(5)-5), (_unit->GetPositionY() + RandomFloat(5)-5),
			_unit->GetPositionZ(), _unit->GetOrientation(),
			true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
			summons->GetAIInterface()->setOutOfCombatRange(50000);
			summons->GetAIInterface()->MoveTo(_unit->GetAIInterface()->GetNextTarget()->GetPositionX(),_unit->GetAIInterface()->GetNextTarget()->GetPositionY(),_unit->GetAIInterface()->GetNextTarget()->GetPositionZ(), 0);
			larvatimer = 120;
		}
		else
		{
		larvatimer--;
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
			for(int i=spmin;i<nrspells;i++)
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
	bool fly;
	int timer;
	bool summon;
	int spmin;
	int larvatimer;
};

//Buru the Gorger
#define BURU_THE_GORGER 15370
//npc
#define HIVEZARA_HATCHLING  15521
#define BURU_EGG 15514
//spells
#define CREEPING_PLAGUE 20512 //dbc untested
#define DISMEMBER 96 //dbc untested
#define SPEED  23451 //dbc untested
#define THORNS 43420 //untested
#define BURU_ABSORB 25750 


class BURUTHEGORGERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(BURUTHEGORGERAI);
	SP_AI_Spell spells[5];
	bool m_spellcheck[5];

    BURUTHEGORGERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(CREEPING_PLAGUE); 
		spells[0].targettype = TARGET_DESTINATION;
		spells[0].instant = true;
		spells[0].perctrigger = 3.0f;
		spells[0].attackstoptimer = 1000;


		spells[1].info = dbcSpell.LookupEntry(SPEED); 
		spells[1].targettype = TARGET_SELF; //not work
		spells[1].instant = true;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;


		spells[2].info = dbcSpell.LookupEntry(THORNS); //+
		spells[2].targettype = TARGET_VARIOUS;
		spells[2].instant = true;
		spells[2].perctrigger = 3.0f;
		spells[2].attackstoptimer = 1000;


        spells[3].info = dbcSpell.LookupEntry(BURU_ABSORB); 
		spells[3].targettype = TARGET_SELF; 
		spells[3].instant = true;
		spells[3].perctrigger = 3.0f;
		spells[3].attackstoptimer = 1000;

		
        spells[4].info = dbcSpell.LookupEntry(DISMEMBER); 
		spells[4].targettype = TARGET_ATTACKING; 
		spells[4].instant = true;
		spells[4].perctrigger = 3.0f;
		spells[4].attackstoptimer = 1000;
		
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		summon = 0;
		nrspells = 2;
		spmin = 0;
		RegisterAIUpdateEvent(1000);
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
		if(_unit->GetHealthPct() <= 20 && !summon)
		{
			for(int i=0;i<6;i++)
			{
				Creature *summons = NULL;
				summons = _unit->GetMapMgr()->GetInterface()->SpawnCreature(HIVEZARA_HATCHLING, 
				(_unit->GetPositionX() + RandomFloat(5)-5), (_unit->GetPositionY() + RandomFloat(5)-5),
				_unit->GetPositionZ(), _unit->GetOrientation(),
				true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
				summons->GetAIInterface()->MoveTo(_unit->GetAIInterface()->GetNextTarget()->GetPositionX(),_unit->GetAIInterface()->GetNextTarget()->GetPositionY(),_unit->GetAIInterface()->GetNextTarget()->GetPositionZ(), 0);
			}
			summon = 1;
			nrspells = 5;
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
			for(int i=spmin;i<nrspells;i++)
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
	bool summon;
	int spmin;
};

//General Rajaxx
#define GENERAL_RAJAXX 15341
//npc
#define CAPTAIN_QEEZ  15391
#define CAPTAIN_TUUBID 15392
#define CAPTAIN_DRENN 15389
#define CAPTAIN_XURREM 15390
#define MAJOR_YEGGETH 15386
#define MAJOR_PAKKON 15388
#define COLONEL_ZERRAN 15385
//spells
#define  THUNDER_CLAP 99999

class GENERALRAJAXXAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(GENERALRAJAXXAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    GENERALRAJAXXAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			rajaxx = 0;
			nrspells = 2;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(THUNDER_CLAP); 
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 1.0f;
		spells[0].attackstoptimer = 1000;

    }
	
	    void OnCombatStart(Unit* mTarget)
    {	
		soob = 0;
		rajaxx = 0;
		RegisterAIUpdateEvent(1000);
    }
    
		void OnSpawn(Unit* mTarget)
    {
		soob = 0;
		RegisterAIUpdateEvent(1000);
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
		if (rajaxx == 1 && soob == 0)
		{
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Odnomu pipec");
		soob = 1;
		}
		if (rajaxx == 2 && soob == 1)
		{
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Vtoromu pipec");
		soob = 2;
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
	int soob;
};

//Kurinnaxx
#define KURINNAXX 15348

//spells
#define MORTAL_WOUND 25646 //instant
#define SAND_TRAP 25656 //instant
#define KUR_ENRAGE  28747 //
#define THRASH 3391 //instant
#define SPEED_SLASH 24192


class KURINNAXXAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(KURINNAXXAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    KURINNAXXAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 4;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(MORTAL_WOUND); 
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 3.0f;
		spells[0].attackstoptimer = 1000;


		spells[1].info = dbcSpell.LookupEntry(SAND_TRAP); 
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].perctrigger = 2.0f;
		spells[1].attackstoptimer = 1000;


		spells[2].info = dbcSpell.LookupEntry(THRASH); 
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;
		spells[2].perctrigger = 3.0f;
		spells[2].attackstoptimer = 1000;


        spells[3].info = dbcSpell.LookupEntry(SPEED_SLASH); 
		spells[3].targettype = TARGET_VARIOUS; 
		spells[3].instant = true;
		spells[3].perctrigger = 2.0f;
		spells[3].attackstoptimer = 1000;
		
    }
    
    void OnCombatStart(Unit* mTarget)
    {	
		enrage = 0;
		RegisterAIUpdateEvent(1000);
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
		if(_unit->GetHealthPct() <= 30 && !enrage)
		{
			_unit->CastSpell(_unit, dbcSpell.LookupEntry(KUR_ENRAGE), true);
			enrage = 1;
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
	bool enrage;
};

//Moam
#define MOAM 15340

//mobs
#define MANA_FIEND 15527

//spells
#define TRAMPLE 40488 //instant
#define DRAIN_MANA 25671 //instant
#define ARCANE_EXPLOSION  25679 // instant


class MOAMAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(MOAMAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    MOAMAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(TRAMPLE); 
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 3.0f;
		spells[0].attackstoptimer = 1000;


		spells[1].info = dbcSpell.LookupEntry(DRAIN_MANA); 
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].perctrigger = 2.0f;
		spells[1].attackstoptimer = 1000;


		spells[2].info = dbcSpell.LookupEntry(ARCANE_EXPLOSION); 
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;
		spells[2].perctrigger = 3.0f;
		spells[2].attackstoptimer = 1000;
		
    }
    
    void OnCombatStart(Unit* mTarget)
    {	
		moamsummonkill = 0;
		phase = 0;
		timer = 90;
		RegisterAIUpdateEvent(1000);
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
	if (!timer && !phase)
		{
			for(int j=0;j<3;j++)
			{
				Creature *Manafiend = NULL;
				Manafiend = _unit->GetMapMgr()->GetInterface()->SpawnCreature(MANA_FIEND, 
				(_unit->GetPositionX() + RandomFloat(5)-5), (_unit->GetPositionY() + RandomFloat(5)-5),
				_unit->GetPositionZ(), _unit->GetOrientation(),
				true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
				Manafiend->GetAIInterface()->setOutOfCombatRange(5000);
			}
			timer = 90;
			phase = 1;
			moamsummonkill = 0;
		}
		if (moamsummonkill == 3)
		{
			phase = 0;
		}
		if (!phase)
		{
			timer--;
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
	bool summon;
	int timer;
	bool phase;
};

//Ossirian the Unscarred
#define OSSIRIAN_THE_UNSCARRED 15339

//spells
#define ENVELOPING_WINDS 25189 //instant
#define CURSE_OF_TONGUE 25195 //instant
#define TRACK_ELEMENTALS  20157 //
#define TORNADO 21990 //instant
#define SUPREME_POWER 17628
#define WAR_STOMP 25188

class OSSIRIANTHEUNSCARREDAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(OSSIRIANTHEUNSCARREDAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    OSSIRIANTHEUNSCARREDAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 4;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(ENVELOPING_WINDS); 
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 3.0f;
		spells[0].attackstoptimer = 1000;


		spells[1].info = dbcSpell.LookupEntry(CURSE_OF_TONGUE); 
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].perctrigger = 2.0f;
		spells[1].attackstoptimer = 1000;


		spells[2].info = dbcSpell.LookupEntry(TRACK_ELEMENTALS); 
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].perctrigger = 3.0f;
		spells[2].attackstoptimer = 1000;


        spells[3].info = dbcSpell.LookupEntry(SUPREME_POWER); 
		spells[3].targettype = TARGET_SELF; 
		spells[3].instant = true;
		spells[3].perctrigger = 2.0f;
		spells[3].attackstoptimer = 1000;
		
        spells[4].info = dbcSpell.LookupEntry(WAR_STOMP); 
		spells[4].targettype = TARGET_VARIOUS; 
		spells[4].instant = true;
		spells[4].perctrigger = 2.0f;
		spells[4].attackstoptimer = 1000;
		
        spells[5].info = dbcSpell.LookupEntry(TORNADO); 
		spells[5].targettype = TARGET_DESTINATION; 
		spells[5].instant = true;
		spells[5].perctrigger = 2.0f;
		spells[5].attackstoptimer = 1000;
		
    }
    
    void OnCombatStart(Unit* mTarget)
    {	
		enrage = 0;
		RegisterAIUpdateEvent(1000);
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
		if(_unit->GetHealthPct() <= 30 && !enrage)
		{
			_unit->CastSpell(_unit, dbcSpell.LookupEntry(KUR_ENRAGE), true);
			enrage = 1;
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
	bool enrage;
};

//mobs

//HIVEZARA WASP

#define BURN 99999 //

class HIVEZARAWASPAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(HIVEZARAWASPAI);
	SP_AI_Spell spells[1]; 
	bool m_spellcheck[1]; 
	
	HIVEZARAWASPAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 1; 
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		
		spells[0].info = dbcSpell.LookupEntry(BURN);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

	}

	void OnSpawn(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);

		Unit *target = NULL;
		target = _unit->GetAIInterface()->GetNextTarget();
		_unit->GetAIInterface()->AttackReaction(target, UNIT_FIELD_MINDAMAGE, 0);
    }

    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
    }

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		_unit->Despawn(3000,0);
		RemoveAIUpdateEvent();
	}
	
	void OnDied(Unit * mKiller)
	{
		_unit->Despawn(30000,0);
		RemoveAIUpdateEvent();
	}
	
	void AIUpdate()
	{
		float val = RandomFloat(100.0f);
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
};

//HIVEZARA_SWARMER

class HIVEZARASWARMERAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(HIVEZARASWARMERAI);
	HIVEZARASWARMERAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{

	}


    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
    }

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		_unit->Despawn(15,0);
		RemoveAIUpdateEvent();
	}
	
	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
	}
	
	void AIUpdate()
	{

	}
	
protected:	

	int timer;
};

//Hive'Zara Larva 


class HIVEZARALARVAAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(HIVEZARALARVAAI);
	HIVEZARALARVAAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
	}


    void OnCombatStart(Unit* mTarget)
    {
		Timer = 60;
		RegisterAIUpdateEvent(1000);
    }

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		//_unit->Despawn(15,0);
		//RemoveAIUpdateEvent();
	}
	
	void OnDied(Unit * mKiller)
	{
		
		Creature *summons = NULL;
		summons = _unit->GetMapMgr()->GetInterface()->SpawnCreature(HIVEZARA_WASP, 
		_unit->GetPositionX(), _unit->GetPositionY(),
		_unit->GetPositionZ(), _unit->GetOrientation(),
		true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
		_unit->Despawn(15,0);
		RemoveAIUpdateEvent();
	}
	
	void AIUpdate()
	{
		if(!Timer)
		{
			Creature *summons = NULL;
			summons = _unit->GetMapMgr()->GetInterface()->SpawnCreature(HIVEZARA_WASP, 
			_unit->GetPositionX(), _unit->GetPositionY(),
			_unit->GetPositionZ(), _unit->GetOrientation(),
			true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
			_unit->Despawn(15,0);
			RemoveAIUpdateEvent();
		}
		else
		{
			Timer--;
		}
	}
	
protected:	

	int nrspells;
	int Timer;
};

//Captain Qeez

//summons
#define SWARMGUARD_NEEDLER 15344
#define QIRAJI_WARRIOR 15387
//spell
#define CQ_INTIMIDATING_SHOUT 19134  //instant


class CAPTAINQEEZAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(CAPTAINQEEZAI);
	SP_AI_Spell spells[1]; 
	bool m_spellcheck[1]; 
	
	CAPTAINQEEZAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 1; 
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		
		spells[0].info = dbcSpell.LookupEntry(CQ_INTIMIDATING_SHOUT);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 2.0f;
		spells[0].attackstoptimer = 1000;

	}

    void OnCombatStart(Unit* mTarget)
    {
		timer = 10;
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
		rajaxx++;
		_unit->Despawn(60000,0);
		RemoveAIUpdateEvent();
	}
	
	void AIUpdate()
	{
	if (!timer && !summon)
		{
			for(int j=0;j<2;j++)
			{
				Creature *SwarmguardNeedler = NULL;
				SwarmguardNeedler = _unit->GetMapMgr()->GetInterface()->SpawnCreature(SWARMGUARD_NEEDLER, 
				(_unit->GetPositionX() + RandomFloat(5)-5), (_unit->GetPositionY() + RandomFloat(5)-5),
				_unit->GetPositionZ(), _unit->GetOrientation(),
				true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
				SwarmguardNeedler->GetAIInterface()->setOutOfCombatRange(5000);
			}
			for(int j=0;j<4;j++)
			{
				Creature *QirajiWarrior = NULL;
				QirajiWarrior = _unit->GetMapMgr()->GetInterface()->SpawnCreature(QIRAJI_WARRIOR, 
				(_unit->GetPositionX() + RandomFloat(5)-5), (_unit->GetPositionY() + RandomFloat(5)-5),
				_unit->GetPositionZ(), _unit->GetOrientation(),
				true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
				QirajiWarrior->GetAIInterface()->setOutOfCombatRange(5000);
			}			
			summon = 1;
		}
		else 
		{
			timer--;
		}
		float val = RandomFloat(100.0f);
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
	int timer;
	bool summon;
};

//Captain Tuubid


//spells
#define CT_ATTACK_ORDER 25471  //instant


class CAPTAINTUUBIDAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(CAPTAINTUUBIDAI);
	SP_AI_Spell spells[1]; 
	bool m_spellcheck[1]; 
	
	CAPTAINTUUBIDAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 1; 
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		
		spells[0].info = dbcSpell.LookupEntry(CT_ATTACK_ORDER);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 3.0f;
		spells[0].attackstoptimer = 1000;

	}

    void OnCombatStart(Unit* mTarget)
    {
		timer = 10;
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
		rajaxx++;
		_unit->Despawn(60000,0);
		RemoveAIUpdateEvent();
	}
	
	void AIUpdate()
	{
	if (!timer && !summon)
		{
			for(int j=0;j<3;j++)
			{
				Creature *SwarmguardNeedler = NULL;
				SwarmguardNeedler = _unit->GetMapMgr()->GetInterface()->SpawnCreature(SWARMGUARD_NEEDLER, 
				(_unit->GetPositionX() + RandomFloat(5)-5), (_unit->GetPositionY() + RandomFloat(5)-5),
				_unit->GetPositionZ(), _unit->GetOrientation(),
				true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
				SwarmguardNeedler->GetAIInterface()->setOutOfCombatRange(5000);
			}
			for(int j=0;j<3;j++)
			{
				Creature *QirajiWarrior = NULL;
				QirajiWarrior = _unit->GetMapMgr()->GetInterface()->SpawnCreature(QIRAJI_WARRIOR, 
				(_unit->GetPositionX() + RandomFloat(5)-5), (_unit->GetPositionY() + RandomFloat(5)-5),
				_unit->GetPositionZ(), _unit->GetOrientation(),
				true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
				QirajiWarrior->GetAIInterface()->setOutOfCombatRange(5000);
			}			
			summon = 1;
		}
		else 
		{
			timer--;
		}
		float val = RandomFloat(100.0f);
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
	int timer;
	bool summon;
};

//Captain Drenn


//spells
#define CD_HURRICANE 27530  //instant


class CAPTAINDRENNAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(CAPTAINDRENNAI);
	SP_AI_Spell spells[1]; 
	bool m_spellcheck[1]; 
	
	CAPTAINDRENNAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 1; 
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		
		spells[0].info = dbcSpell.LookupEntry(CD_HURRICANE);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 3.0f;
		spells[0].attackstoptimer = 1000;

	}

    void OnCombatStart(Unit* mTarget)
    {
		timer = 10;
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
		rajaxx++;
		_unit->Despawn(60000,0);
		RemoveAIUpdateEvent();
	}
	
	void AIUpdate()
	{
	if (!timer && !summon)
		{
			for(int j=0;j<5;j++)
			{
				Creature *SwarmguardNeedler = NULL;
				SwarmguardNeedler = _unit->GetMapMgr()->GetInterface()->SpawnCreature(SWARMGUARD_NEEDLER, 
				(_unit->GetPositionX() + RandomFloat(5)-5), (_unit->GetPositionY() + RandomFloat(5)-5),
				_unit->GetPositionZ(), _unit->GetOrientation(),
				true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
				SwarmguardNeedler->GetAIInterface()->setOutOfCombatRange(5000);
			}
			for(int j=0;j<1;j++)
			{
				Creature *QirajiWarrior = NULL;
				QirajiWarrior = _unit->GetMapMgr()->GetInterface()->SpawnCreature(QIRAJI_WARRIOR, 
				(_unit->GetPositionX() + RandomFloat(5)-5), (_unit->GetPositionY() + RandomFloat(5)-5),
				_unit->GetPositionZ(), _unit->GetOrientation(),
				true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
				QirajiWarrior->GetAIInterface()->setOutOfCombatRange(5000);
			}			
			summon = 1;
		}
		else 
		{
			timer--;
		}
		float val = RandomFloat(100.0f);
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
	int timer;
	bool summon;
};

//Captain Xurrem


//spells
#define CX_SHOCKWAVE 25425  //instant


class CAPTAINXURREMAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(CAPTAINXURREMAI);
	SP_AI_Spell spells[1]; 
	bool m_spellcheck[1]; 
	
	CAPTAINXURREMAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 1; 
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		
		spells[0].info = dbcSpell.LookupEntry(CX_SHOCKWAVE);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 3.0f;
		spells[0].attackstoptimer = 1000;

	}

    void OnCombatStart(Unit* mTarget)
    {
		timer = 10;
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
		rajaxx++;
		_unit->Despawn(60000,0);
		RemoveAIUpdateEvent();
	}
	
	void AIUpdate()
	{
	if (!timer && !summon)
		{
			for(int j=0;j<3;j++)
			{
				Creature *SwarmguardNeedler = NULL;
				SwarmguardNeedler = _unit->GetMapMgr()->GetInterface()->SpawnCreature(SWARMGUARD_NEEDLER, 
				(_unit->GetPositionX() + RandomFloat(5)-5), (_unit->GetPositionY() + RandomFloat(5)-5),
				_unit->GetPositionZ(), _unit->GetOrientation(),
				true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
				SwarmguardNeedler->GetAIInterface()->setOutOfCombatRange(5000);
			}
			for(int j=0;j<3;j++)
			{
				Creature *QirajiWarrior = NULL;
				QirajiWarrior = _unit->GetMapMgr()->GetInterface()->SpawnCreature(QIRAJI_WARRIOR, 
				(_unit->GetPositionX() + RandomFloat(5)-5), (_unit->GetPositionY() + RandomFloat(5)-5),
				_unit->GetPositionZ(), _unit->GetOrientation(),
				true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
				QirajiWarrior->GetAIInterface()->setOutOfCombatRange(5000);
			}			
			summon = 1;
		}
		else 
		{
			timer--;
		}
		float val = RandomFloat(100.0f);
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
	int timer;
	bool summon;
};

//Major Yeggeth


//spells
#define MY_BLESSING_OF_PROTECTION 23415  //instant


class MAJORYEGGETHAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(MAJORYEGGETHAI);
	SP_AI_Spell spells[1]; 
	bool m_spellcheck[1]; 
	
	MAJORYEGGETHAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 1; 
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		
		spells[0].info = dbcSpell.LookupEntry(MY_BLESSING_OF_PROTECTION);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 3.0f;
		spells[0].attackstoptimer = 1000;

	}

    void OnCombatStart(Unit* mTarget)
    {
		timer = 10;
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
		rajaxx++;
		_unit->Despawn(60000,0);
		RemoveAIUpdateEvent();
	}
	
	void AIUpdate()
	{
	if (!timer && !summon)
		{
			for(int j=0;j<4;j++)
			{
				Creature *SwarmguardNeedler = NULL;
				SwarmguardNeedler = _unit->GetMapMgr()->GetInterface()->SpawnCreature(SWARMGUARD_NEEDLER, 
				(_unit->GetPositionX() + RandomFloat(5)-5), (_unit->GetPositionY() + RandomFloat(5)-5),
				_unit->GetPositionZ(), _unit->GetOrientation(),
				true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
				SwarmguardNeedler->GetAIInterface()->setOutOfCombatRange(5000);
			}
			for(int j=0;j<2;j++)
			{
				Creature *QirajiWarrior = NULL;
				QirajiWarrior = _unit->GetMapMgr()->GetInterface()->SpawnCreature(QIRAJI_WARRIOR, 
				(_unit->GetPositionX() + RandomFloat(5)-5), (_unit->GetPositionY() + RandomFloat(5)-5),
				_unit->GetPositionZ(), _unit->GetOrientation(),
				true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
				QirajiWarrior->GetAIInterface()->setOutOfCombatRange(5000);
			}			
			summon = 1;
		}
		else 
		{
			timer--;
		}
		float val = RandomFloat(100.0f);
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
	int timer;
	bool summon;
};

//Major Pakkon


//spells
#define MP_SWEEPING_SLAM 25322  //instant


class MAJORPAKKONAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(MAJORPAKKONAI);
	SP_AI_Spell spells[1]; 
	bool m_spellcheck[1]; 
	
	MAJORPAKKONAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 1; 
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		
		spells[0].info = dbcSpell.LookupEntry(MP_SWEEPING_SLAM);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 3.0f;
		spells[0].attackstoptimer = 1000;

	}

    void OnCombatStart(Unit* mTarget)
    {
		timer = 10;
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
		rajaxx++;
		_unit->Despawn(60000,0);
		RemoveAIUpdateEvent();
	}
	
	void AIUpdate()
	{
	if (!timer && !summon)
		{
			for(int j=0;j<2;j++)
			{
				Creature *SwarmguardNeedler = NULL;
				SwarmguardNeedler = _unit->GetMapMgr()->GetInterface()->SpawnCreature(SWARMGUARD_NEEDLER, 
				(_unit->GetPositionX() + RandomFloat(5)-5), (_unit->GetPositionY() + RandomFloat(5)-5),
				_unit->GetPositionZ(), _unit->GetOrientation(),
				true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
				SwarmguardNeedler->GetAIInterface()->setOutOfCombatRange(5000);
			}
			for(int j=0;j<4;j++)
			{
				Creature *QirajiWarrior = NULL;
				QirajiWarrior = _unit->GetMapMgr()->GetInterface()->SpawnCreature(QIRAJI_WARRIOR, 
				(_unit->GetPositionX() + RandomFloat(5)-5), (_unit->GetPositionY() + RandomFloat(5)-5),
				_unit->GetPositionZ(), _unit->GetOrientation(),
				true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
				QirajiWarrior->GetAIInterface()->setOutOfCombatRange(5000);
			}			
			summon = 1;
		}
		else 
		{
			timer--;
		}
		float val = RandomFloat(100.0f);
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
	int timer;
	bool summon;
};

//Colonel Zerran


//spells
#define CZ_SWEEPING_SLAM 25322  //instant


class COLONELZERRANAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(COLONELZERRANAI);
	SP_AI_Spell spells[1]; 
	bool m_spellcheck[1]; 
	
	COLONELZERRANAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 1; 
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		
		spells[0].info = dbcSpell.LookupEntry(MP_SWEEPING_SLAM);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 3.0f;
		spells[0].attackstoptimer = 1000;

	}

    void OnCombatStart(Unit* mTarget)
    {
		timer = 10;
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
		rajaxx++;
		_unit->Despawn(60000,0);
		RemoveAIUpdateEvent();
	}
	
	void AIUpdate()
	{
	if (!timer && !summon)
		{
			for(int j=0;j<3;j++)
			{
				Creature *SwarmguardNeedler = NULL;
				SwarmguardNeedler = _unit->GetMapMgr()->GetInterface()->SpawnCreature(SWARMGUARD_NEEDLER, 
				(_unit->GetPositionX() + RandomFloat(5)-5), (_unit->GetPositionY() + RandomFloat(5)-5),
				_unit->GetPositionZ(), _unit->GetOrientation(),
				true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
				SwarmguardNeedler->GetAIInterface()->setOutOfCombatRange(5000);
			}
			for(int j=0;j<3;j++)
			{
				Creature *QirajiWarrior = NULL;
				QirajiWarrior = _unit->GetMapMgr()->GetInterface()->SpawnCreature(QIRAJI_WARRIOR, 
				(_unit->GetPositionX() + RandomFloat(5)-5), (_unit->GetPositionY() + RandomFloat(5)-5),
				_unit->GetPositionZ(), _unit->GetOrientation(),
				true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
				QirajiWarrior->GetAIInterface()->setOutOfCombatRange(5000);
			}			
			summon = 1;
		}
		else 
		{
			timer--;
		}
		float val = RandomFloat(100.0f);
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
	int timer;
	bool summon;
};

//Swarmguard Needler



class SWARMGUARDNEEDLERAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(SWARMGUARDNEEDLERAI);
	
	SWARMGUARDNEEDLERAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
	}

    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
    }

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		_unit->Despawn(60,0);
		RemoveAIUpdateEvent();
	}
	
	void OnDied(Unit * mKiller)
	{
		_unit->Despawn(60000,0);
		RemoveAIUpdateEvent();
	}
	
	void AIUpdate()
	{
	}
	
};

//Qiraji Warrior



class QIRAJIWARRIORAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(QIRAJIWARRIORAI);
	
	QIRAJIWARRIORAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
	}

    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
    }

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		_unit->Despawn(60,0);
		RemoveAIUpdateEvent();
	}
	
	void OnDied(Unit * mKiller)
	{
		_unit->Despawn(60000,0);
		RemoveAIUpdateEvent();
	}
	
	void AIUpdate()
	{
	}
	
};

//MANA_FIEND


//spells
#define MF_ARCANE_EXPLOSION 25679  //instant
#define COUNTERSPELL 19715

class MANAFIENDAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(MANAFIENDAI);
	SP_AI_Spell spells[2]; 
	bool m_spellcheck[2]; 
	
	MANAFIENDAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 2; 
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		
		spells[0].info = dbcSpell.LookupEntry(MF_ARCANE_EXPLOSION);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 3.0f;
		spells[0].attackstoptimer = 1000;
		
		spells[1].info = dbcSpell.LookupEntry(COUNTERSPELL);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].perctrigger = 3.0f;
		spells[1].attackstoptimer = 1000;

	}

    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
    }

	void OnCombatStop(Unit *mTarget)
	{
		_unit->Despawn(6000,0);
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}
	
	void OnDied(Unit * mKiller)
	{
		_unit->Despawn(60000,0);
		RemoveAIUpdateEvent();
	}
	
	void AIUpdate()
	{
		float val = RandomFloat(100.0f);
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
};

//BURU_EGG


class BURUEGGAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(BURUEGGAI);
	BURUEGGAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
	}


    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
		_unit->GetAIInterface()->disable_melee = true;
    }

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		//_unit->Despawn(15,0);
		//RemoveAIUpdateEvent();
	}
	
	void OnDied(Unit * mKiller)
	{
		for(int i=0;i<2;i++)
		{
			Creature *summons = NULL;
			summons = _unit->GetMapMgr()->GetInterface()->SpawnCreature(HIVEZARA_HATCHLING, 
			_unit->GetPositionX(), _unit->GetPositionY(),
			_unit->GetPositionZ(), _unit->GetOrientation(),
			true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0);
			summons->GetAIInterface()->setOutOfCombatRange(50000);
		}
		_unit->Despawn(15,0);
		RemoveAIUpdateEvent();
	}
	
	void AIUpdate()
	{

	}
	
};

void SetupRuinsofAhnQiraj(ScriptMgr * mgr)
{
	mgr->register_creature_script(AYAMISS_THE_HUNTER, &AYAMISSTHEHUNTERAI::Create);
	mgr->register_creature_script(BURU_THE_GORGER, &BURUTHEGORGERAI::Create);
	//mgr->register_creature_script(GENERAL_RAJAXX, &GENERALRAJAXXAI::Create);
	//mgr->register_creature_script(KURINNAXX, &KURINNAXXAI::Create);
	//mgr->register_creature_script(MOAM, &MOAMAI::Create);
	//mgr->register_creature_script(OSSIRIAN_THE_UNSCARRED, &OSSIRIANTHEUNSCARREDAI::Create);
	//MOBS
	mgr->register_creature_script(HIVEZARA_WASP, &HIVEZARAWASPAI::Create);
	mgr->register_creature_script(HIVEZARA_SWARMER, &HIVEZARASWARMERAI::Create);
	mgr->register_creature_script(HIVE_ZARA_LARVA, &HIVEZARALARVAAI::Create);
	//mgr->register_creature_script(CAPTAIN_QEEZ, &CAPTAINQEEZAI::Create);
	//mgr->register_creature_script(CAPTAIN_TUUBID, &CAPTAINTUUBIDAI::Create);
	//mgr->register_creature_script(CAPTAIN_DRENN, &CAPTAINDRENNAI::Create);
	//mgr->register_creature_script(CAPTAIN_XURREM, &CAPTAINXURREMAI::Create);
	//mgr->register_creature_script(MAJOR_YEGGETH, &MAJORYEGGETHAI::Create);
	//mgr->register_creature_script(MAJOR_PAKKON, &MAJORPAKKONAI::Create);
	//mgr->register_creature_script(COLONEL_ZERRAN, &COLONELZERRANAI::Create);
	//mgr->register_creature_script(SWARMGUARD_NEEDLER, &SWARMGUARDNEEDLERAI::Create);
	//mgr->register_creature_script(QIRAJI_WARRIOR, &QIRAJIWARRIORAI::Create);
	//mgr->register_creature_script(QIRAJI_WARRIOR, &QIRAJIWARRIORAI::Create);
	//mgr->register_creature_script(MANA_FIEND, &MANAFIENDAI::Create);
	mgr->register_creature_script(BURU_EGG, &BURUEGGAI::Create);	
	
}