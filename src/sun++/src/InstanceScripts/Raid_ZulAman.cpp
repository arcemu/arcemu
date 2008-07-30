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

//---------------------------------------------------------------------------------------------//
//-----------------------------------Zul'aman boss scripts-------------------------------------//
//---------------------------------Not blizzlike. Not tested-----------------------------------//
//-------------------------------Some crash/mess fixes - M4ksiu--------------------------------//
//---------------------------------------------------------------------------------------------//

// NalorakkAI
#define CN_NALORAKK 23576

// Troll Form spells
#define MANGLE				44955
#define SURGE				25787	// 42402 - correct spell hits creature casting spell
// Bear Form spells
#define LACERATING_SLASH	42395
#define REND_FLESH			42397
#define DEAFENING_ROAR		42398
// Common spells
#define BRUTAL_SWIPE		42384
// Other spells
#define BERSERK				41924

enum NalorakkForms
{
	NALORAKK_FORM_TROLL,
	NALORAKK_FORM_BEAR
};

class NalorakkAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(NalorakkAI);
	SP_AI_Spell spells[7];
	bool m_spellcheck[6];

    NalorakkAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 6;
		for(int i = 0; i < nrspells; i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(MANGLE); 
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 12.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 20;

		spells[1].info = dbcSpell.LookupEntry(SURGE); 
		spells[1].targettype = TARGET_RANDOM_SINGLE;
		spells[1].instant = true;
		spells[1].perctrigger = 8.0f;
		spells[1].attackstoptimer = 1000;
		spells[1].mindist2cast = 0.0f;
		spells[1].maxdist2cast = 45.0f;
		spells[1].cooldown = 20;
		spells[1].soundid = 12071;
		spells[1].speech = "I bring da pain!";

        spells[2].info = dbcSpell.LookupEntry(BRUTAL_SWIPE); 
		spells[2].targettype = TARGET_ATTACKING; 
		spells[2].instant = true;
		spells[2].perctrigger = 2.0f;
		spells[2].attackstoptimer = 1000;
		spells[2].cooldown = 35;

		spells[3].info = dbcSpell.LookupEntry(LACERATING_SLASH);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = true;
		spells[3].perctrigger = 12.0f;
		spells[3].attackstoptimer = 1000;
		spells[3].cooldown = 20;

		spells[4].info = dbcSpell.LookupEntry(REND_FLESH); 
		spells[4].targettype = TARGET_ATTACKING;
		spells[4].instant = true;
		spells[4].perctrigger = 12.0f;
		spells[4].attackstoptimer = 1000;
		spells[4].cooldown = 12;

		spells[5].info = dbcSpell.LookupEntry(DEAFENING_ROAR); 
		spells[5].targettype = TARGET_VARIOUS;
		spells[5].instant = true;
		spells[5].perctrigger = 11.0f;
		spells[5].attackstoptimer = 1000;
		spells[5].cooldown = 12;

		spells[6].info = dbcSpell.LookupEntry(BERSERK);
		spells[6].cooldown = 600;

		_unit->SetFloatValue(OBJECT_FIELD_SCALE_X , 1.0f);

		Form = NALORAKK_FORM_TROLL;
		FormTimer = 0;
		minspell = 0;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You be dead soon enough!");
		_unit->PlaySoundToSet(12070);

		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));

		for (int i = 0; i < 6; i++)
			spells[i].casttime = 0;

		uint32 t = (uint32)time(NULL);
		spells[6].casttime = t + spells[6].cooldown;
		Form = NALORAKK_FORM_TROLL;
		FormTimer = t + 45;
		nrspells = 3;
		minspell = 0;
    }

	void OnTargetDied(Unit* mTarget)
    {
		if (_unit->GetHealthPct() > 0)	
		{
			int RandomSpeach = rand()%2;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Da Amani gonna rule again!");
				_unit->PlaySoundToSet(12076);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Now whatchoo got to say?");
				_unit->PlaySoundToSet(12075);
				break;
			}
		}
    }

    void OnCombatStop(Unit *mTarget)
    {
		if (_unit->FindAura(42377))
			_unit->RemoveAura(42377);

		_unit->SetFloatValue(OBJECT_FIELD_SCALE_X , 1.0f);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I... be waitin' on da udda side....");
		_unit->PlaySoundToSet(12077);

		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		uint32 t = (uint32)time(NULL);
		if (_unit->GetCurrentSpell() == NULL)
		{
			if (t > FormTimer)
			{
				if (Form == NALORAKK_FORM_TROLL)
				{
					_unit->setAttackTimer(1500, false);
					_unit->CastSpell(_unit, 42377, true);
					_unit->SetFloatValue(OBJECT_FIELD_SCALE_X , 1.0f);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You call on da beast, you gonna get more dan you bargain for!");
					_unit->PlaySoundToSet(12072);

					Form = NALORAKK_FORM_BEAR;
					FormTimer = t + 20;
					nrspells = 6;
					minspell = 2;
				}
				else
				{
					if (_unit->FindAura(42377))
						_unit->RemoveAura(42377);

					_unit->setAttackTimer(1500, false);
					_unit->SetFloatValue(OBJECT_FIELD_SCALE_X , 1.0f);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Make way for Nalorakk!");
					_unit->PlaySoundToSet(12073);

					Form = NALORAKK_FORM_TROLL;
					FormTimer = t + 45;
					nrspells = 3;
					minspell = 0;
				}

				return;
			}
			if (t > spells[6].casttime)
			{
				_unit->CastSpell(_unit, spells[6].info, true);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You had your chance, now it be too late!");	// check plx
				_unit->PlaySoundToSet(12074);

				spells[6].casttime = t + spells[6].cooldown;
				return;
			}
		}

		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

    void SpellCast(float val)
    {
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget() != NULL)
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i = minspell; i < nrspells; i++)
			{
				if(!spells[i].perctrigger)
					continue;
				
				if(m_spellcheck[i])
				{
					if (!spells[i].instant)
						_unit->GetAIInterface()->StopMovement(1);

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
			TargetTable.clear();
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
		}
	}

protected:

	int nrspells, minspell;
	uint32 FormTimer;
	uint32 Form;
};

//Akil'zon <Eagle Avatar>
#define AKILZON 23574

#define STATIC_DISRUPTION 44008 //INSTANT
#define CALL_LIGHTING 43661 //INSTANT 
#define GUST_OF_WIND 43621 //INSTANT
#define ELECTRICAL_STORM 43648
#define SOARING_EAGLE 24858   //SUMMONS  Akil'zon <Eagle Avatar>


class AKILZONAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(AKILZONAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    AKILZONAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			nrspells = 4;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(STATIC_DISRUPTION); 
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 2.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(CALL_LIGHTING); 
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 2.0f;
		spells[1].attackstoptimer = 1000;


		spells[2].info = dbcSpell.LookupEntry(GUST_OF_WIND); //crash
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;


        spells[3].info = dbcSpell.LookupEntry(ELECTRICAL_STORM);
		spells[3].targettype = TARGET_VARIOUS;
		spells[3].instant = true;
		spells[3].perctrigger = 1.0f;
		spells[3].attackstoptimer = 1000;


    }
    
    void OnCombatStart(Unit* mTarget)
    {
		nrspells = 4;
		timesummon = 0;
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I be da predator! You da prey!");
		_unit->PlaySoundToSet(12013);
		RegisterAIUpdateEvent(1000);
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
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Stop your cryin'!");
				_unit->PlaySoundToSet(12018);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Ya got nothin'!");
				_unit->PlaySoundToSet(12017);
				break;
			}
		}
    }

    void OnCombatStop(Unit *mTarget)
    {
		nrspells = 4;
		timesummon = 0;
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You can't... kill... me spirit!");
		_unit->PlaySoundToSet(12019);
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{	
		if (timesummon > 300)
			{
				Creature *cre = NULL;
				cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(SOARING_EAGLE, 
				(_unit->GetPositionX()+RandomFloat(10)-10), (_unit->GetPositionY()+RandomFloat(10)-10),
				_unit->GetPositionZ(), _unit->GetOrientation(),
				true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
				cre->GetAIInterface()->setOutOfCombatRange(5000);
				timesummon = 0;
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Feed, me bruddahs!");
				_unit->PlaySoundToSet(12019);
			}
		timesummon = timesummon + RandomUInt(3);
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
	uint32 timesummon;
};


//Halazzi <Lynx Avatar>

#define HALAZZI 23577
#define CORRUPTED_LIGHTING_TOTEM 24224 
#define HALAZZY_ENRAGE 44779

//Phase 1
#define SABER_LASH 43267 //43267 //43268 ///40810 //40816
//Phase 2
#define FLAME_SHOCK 43303
#define EARTH_SHOCK 43305 //INSTANT , VARIOUS
//Phase 3
//Halazzi now drops his totems more frequently as well as doing more damage. 




class HALAZZIAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HALAZZIAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    HALAZZIAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(SABER_LASH);
		spells[0].targettype = TARGET_DESTINATION;
		spells[0].instant = false;
		spells[0].perctrigger = 0.5f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(FLAME_SHOCK);
		spells[1].targettype = TARGET_ATTACKING; 
		spells[1].instant = true;
		spells[1].perctrigger = 2.0f;
		spells[1].attackstoptimer = 1000;
		
		spells[2].info = dbcSpell.LookupEntry(EARTH_SHOCK);
		spells[2].targettype = TARGET_ATTACKING; 
		spells[2].instant = true;
		spells[2].perctrigger = 2.0f;
		spells[2].attackstoptimer = 1000;


    }
    
    void OnCombatStart(Unit* mTarget)
    {
		nrspells = 1;
		spmin = 0;
		phase =1;
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Get on your knees and bow to da fang and claw!");
		_unit->PlaySoundToSet(12020);
		RegisterAIUpdateEvent(1000);
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
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You cant fight da power...");
				_unit->PlaySoundToSet(12026);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You all gonna fail...");
				_unit->PlaySoundToSet(12027);
				break;
			}
		}

    }

    void OnCombatStop(Unit *mTarget)
    {
		nrspells = 1;
		spmin = 0;
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Chaga... choka'jinn.");
		_unit->PlaySoundToSet(12028);
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		if (!enrage && timer == 600)
		{
		_unit->CastSpell(_unit, dbcSpell.LookupEntry(HALAZZY_ENRAGE), true);
		enrage = 1;
		}
		if (_unit->GetHealthPct() <= 75 && phase == 1) 
		{
			spmin = 1;
			nrspells = 3;
			phase = 2;

		}
		if (_unit->GetHealthPct() <= 50 && phase == 2) 
		{
		spmin = 0;
		nrspells = 3;
		phase = 3;
		}
		if (timetotem > 300 && phase ==2)
		{
			Creature *cre = NULL;
			cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CORRUPTED_LIGHTING_TOTEM, 
			(_unit->GetPositionX()+RandomFloat(3)-3), (_unit->GetPositionY()+RandomFloat(3)-3),
			_unit->GetPositionZ(), _unit->GetOrientation(),
			true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
			cre->GetAIInterface()->setOutOfCombatRange(5000);
			_unit->CastSpell(_unit, dbcSpell.LookupEntry(43302), false);
			timetotem = 0;
		}
		if (timetotem > 150 && phase ==3)
		{
			Creature *cre = NULL;
			cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CORRUPTED_LIGHTING_TOTEM, 
			(_unit->GetPositionX()+RandomFloat(3)-3), (_unit->GetPositionY()+RandomFloat(3)-3),
			_unit->GetPositionZ(), _unit->GetOrientation(),
			true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
			cre->GetAIInterface()->setOutOfCombatRange(5000);
			timetotem = 0;
		}
		timetotem = timetotem + RandomUInt(3);
		timer++;
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
	int spmin;
	int phase;
	uint32 timetotem;
	int timer;
	bool enrage;
};


//Jan'alai <Dragonhawk Avatar>
#define JANALAI 23578

//Phase 1
#define FLAME_BREATH 43582 //43582,43294,43215,43140,39263,38974,38267,37638 in dbc, need test
#define FLAME_BUFFET 43299 
//#define TELEPORT ????
//Phase2 
//#define FLAME_BREATH 23461 //43582,43294,43215,43140,39263,38974,38267,37638 in dbc, need test
//#define FLAME_BUFFET 43299 
//phase3 
//#define FLAME_BREATH 23461 //43582,43294,43215,43140,39263,38974,38267,37638 in dbc, need test
//#define FLAME_BUFFET 43299 
#define JANALAI_ENRAGE 44779




class JANALAIAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(JANALAIAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    JANALAIAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(FLAME_BREATH);
		spells[0].targettype = TARGET_DESTINATION;
		spells[0].instant = true;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(FLAME_BUFFET); 
		spells[1].targettype = TARGET_ATTACKING; 
		spells[1].instant = true;
		spells[1].perctrigger = 5.0f;
		spells[1].attackstoptimer = 1000;
		
        spells[2].info = dbcSpell.LookupEntry(FLAME_BREATH);
		spells[2].targettype = TARGET_DESTINATION;
		spells[2].instant = true;
		spells[2].perctrigger = 10.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(FLAME_BUFFET);
		spells[3].targettype = TARGET_ATTACKING; 
		spells[3].instant = true;
		spells[3].perctrigger = 10.0f;
		spells[3].attackstoptimer = 1000;
		


    }
    
    void OnCombatStart(Unit* mTarget)
    {
		nrspells = 2;
		spmin = 0;
		phase =1;
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Spirits Of the wind,be your doom!");
		_unit->PlaySoundToSet(12031);
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
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "It all be over now, mon!");
				_unit->PlaySoundToSet(12036);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Da shadow gonna fall on you!");
				_unit->PlaySoundToSet(12041);
				break;
			}
		}
    }

    void OnCombatStop(Unit *mTarget)
    {
		nrspells = 2;
		spmin = 0;
		phase =1;
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Zul'jin... got a surprise for you...");
		_unit->PlaySoundToSet(12038);
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		if (_unit->GetHealthPct() <= 50 && phase == 1) 
		{
			spmin = 2;
			nrspells = 4;
			phase = 2;
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I burn ya now!");
			_unit->PlaySoundToSet(12032);
		}
		if (_unit->GetHealthPct() <= 25 && phase == 2) 
		{
		spmin = 2;
		nrspells = 4;
		phase = 3;
		_unit->CastSpell(_unit, dbcSpell.LookupEntry(JANALAI_ENRAGE), true);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Where ma hatcha? Get to work on dem eggs!");
		_unit->PlaySoundToSet(12033);
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
	int spmin;
	int phase;
};

//HEX_LORD_MALACRASS

#define HEX_LORD_MALACRASS 24239
//summons
#define GAZAKROTH 24244
#define THURG 24241 
#define LORD_RAADAN 24243 
#define DARKHEART 24246 
#define ALYSON_ANTILLE 24240
#define SLITHER 24242 
#define FENSTALKER 24245 
#define KORAGG 24247 

//spells
#define SOUL_DRAIN 41303
#define SPIRIT_BOLTS 43382 //43383 

class HEXLORDMALACRASSAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HEXLORDMALACRASSAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    HEXLORDMALACRASSAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			nrspells = 2;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(SOUL_DRAIN);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 1.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(SPIRIT_BOLTS);
		spells[1].targettype = TARGET_DESTINATION; 
		spells[1].instant = true;
		spells[1].perctrigger = 3.0f;
		spells[1].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Da shadow gonna fall on you...");
		_unit->PlaySoundToSet(12041);
		timer = 5;
		mobs = 0;
		RegisterAIUpdateEvent(1000);
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
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Azzaga choogo zinn!");
				_unit->PlaySoundToSet(12044);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Dis a nightmare ya don' wake up from!");
				_unit->PlaySoundToSet(12043);
				break;
			}
		}
	
    }

    void OnCombatStop(Unit *mTarget)
    {
		nrspells = 2;
		timer = 0;
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Dis not da end for me..");
		_unit->PlaySoundToSet(12051);
       RemoveAIUpdateEvent();

	   GameObject * pDoor = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(123.256f, 914.422f, 34.1441f, 186306);
        if(pDoor == 0)
            return;

       pDoor->SetUInt32Value(GAMEOBJECT_STATE, 0); // Open the door
    }

    void AIUpdate()
	{
		if (!timer && !mobs)
		{
			for(int j=0;j<4;j++)
			{
				int RandomSpeach;
				RandomUInt(1000);
				RandomSpeach=rand()%8;
				switch (RandomSpeach)
				{
					case 0:
						summon = GAZAKROTH;
						break;
					case 1:
						summon = DARKHEART;
						break;
					case 2:
						summon = FENSTALKER;
						break;
					case 3:
						summon = SLITHER;
						break;
					case 4:
						summon = ALYSON_ANTILLE;
						break;
					case 5:
						summon = THURG;
						break;
					case 6:
						summon = KORAGG;
						break;
					case 7:
						summon = LORD_RAADAN;
						break;
				}
				Creature *cre = NULL;
				cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(summon, 
				(_unit->GetPositionX() + RandomFloat(10)-10), (_unit->GetPositionY() + RandomFloat(10)-10),
				_unit->GetPositionZ(), _unit->GetOrientation(),
				true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
				cre->GetAIInterface()->setOutOfCombatRange(50000);
			}
			mobs = 1;
		}
		if (timer)
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
	int timer;
	uint32 summon;
	bool mobs;
};

//ZUL’JIN

#define ZULJIN 23863

//Phase 1 troll
#define GRIEVOUS_THROW 43093 
#define WHIRLWIND  44949 // 44949 - work,43442,41400, 41399,41195, 41194,41098,  // from dbc, untested
#define MODEL_TROLL 21899 
//Phase2 bear
#define CREEPING_PARALYSIS 43095 
#define OVERPOWER 43456
#define MODEL_BEAR 21736
//phase3  eagle
#define ENERGY_STORM 43983 //not Work
#define MODEL_EAGLE 22257
#define FEATHER_VORTEX 24136 //summons npc 4 begin phase
//phase4  lynx
#define CLAW_RAGE  43150 
#define LYNX_RUSH 43153
#define MODEL_LYNX 21907
//phase5  dragonhawk
#define FLAME_WHIRL 44090 //43208, 43213
#define ZJ_FLAME_BREATH 43215 
#define ZJ_FLAME_SHOCK 43303
#define MODEL_DRAGONHAWK 21901




class ZULJINAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ZULJINAI);
	SP_AI_Spell spells[10];
	bool m_spellcheck[10];

    ZULJINAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(GRIEVOUS_THROW);
		spells[0].targettype = TARGET_DESTINATION;
		spells[0].instant = false;
		spells[0].perctrigger = 1.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(WHIRLWIND); 
		spells[1].targettype = TARGET_ATTACKING; 
		spells[1].instant = false;
		spells[1].perctrigger = 1.0f;
		spells[1].attackstoptimer = 1000;
		
        spells[2].info = dbcSpell.LookupEntry(CREEPING_PARALYSIS); 
		spells[2].targettype = TARGET_VARIOUS;
		spells[2].instant = true;
		spells[2].perctrigger = 3.0f;
		spells[2].attackstoptimer = 1000;
		
        spells[3].info = dbcSpell.LookupEntry(OVERPOWER);
		spells[3].targettype = TARGET_VARIOUS; 
		spells[3].instant = true;
		spells[3].perctrigger = 3.0f;
		spells[3].attackstoptimer = 1000;

		spells[4].info = dbcSpell.LookupEntry(ENERGY_STORM); 
		spells[4].targettype = TARGET_VARIOUS;  
		spells[4].instant = true;
		spells[4].perctrigger = 3.0f;
		spells[4].attackstoptimer = 1000;
		
		spells[5].info = dbcSpell.LookupEntry(CLAW_RAGE);
		spells[5].targettype = TARGET_ATTACKING; 
		spells[5].instant = true;
		spells[5].perctrigger = 10.0f;
		spells[5].attackstoptimer = 1000;
		
		spells[6].info = dbcSpell.LookupEntry(LYNX_RUSH);
		spells[6].targettype = TARGET_ATTACKING; 
		spells[6].instant = true;
		spells[6].perctrigger = 3.0f;
		spells[6].attackstoptimer = 1000;
		
        spells[7].info = dbcSpell.LookupEntry(FLAME_WHIRL); 
		spells[7].targettype = TARGET_VARIOUS;
		spells[7].instant = true;
		spells[7].perctrigger = 4.0f;
		spells[7].attackstoptimer = 1000;

		spells[8].info = dbcSpell.LookupEntry(ZJ_FLAME_BREATH); 
		spells[8].targettype = TARGET_ATTACKING; 
		spells[8].instant = true;
		spells[8].perctrigger = 3.0f;
		spells[8].attackstoptimer = 1000;

		spells[9].info = dbcSpell.LookupEntry(ZJ_FLAME_SHOCK);
		spells[9].targettype = TARGET_ATTACKING; 
		spells[9].instant = true;
		spells[9].perctrigger = 2.0f;
		spells[9].attackstoptimer = 1000;			

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		nrspells = 2;
		spmin = 0;
		phase =1;
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Nobody badduh dan me.");
		_unit->PlaySoundToSet(12091);
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
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Di Amani Di chuga");
				_unit->PlaySoundToSet(12098);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Lot more gonna fall like you!");
				_unit->PlaySoundToSet(12099);
				break;
			}
		}
    }

    void OnCombatStop(Unit *mTarget)
    {
		nrspells = 2;
		spmin = 0;
		phase =1;
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , MODEL_TROLL);  //change to Troll
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Mebbe me fall...but da Amani empire...never gonna die...");
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		if (_unit->GetHealthPct() <= 80 && phase == 1) 
		{
			spmin = 2;
			nrspells = 4;
			phase = 2;
			_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , MODEL_BEAR);  //change to Bear
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Got me some new tricks...like me bruddah bear!");
			_unit->PlaySoundToSet(12092);
		}
		if (_unit->GetHealthPct() <= 60 && phase == 2) 
		{
			spmin = 4;
			nrspells = 5;
			phase = 3;
			for(int j=0;j<4;j++)
			{
				Creature *cre = NULL;
				cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(FEATHER_VORTEX, 
				(_unit->GetPositionX() + RandomFloat(10)-10), (_unit->GetPositionY() + RandomFloat(10)-10),
				_unit->GetPositionZ(), _unit->GetOrientation(),
				true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
				cre->GetAIInterface()->setOutOfCombatRange(50000);
			}
			_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , MODEL_EAGLE);  //change to Eagle
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Dere be no hidin' from da eagle!");
			_unit->PlaySoundToSet(12093);
		}
		if (_unit->GetHealthPct() <= 40 && phase == 3) 
		{
			spmin = 5;
			nrspells = 7;
			phase = 4;
			_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , MODEL_LYNX);  //change to Lynx
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Let me introduce to you my new bruddahs: fang and claw!");
			_unit->PlaySoundToSet(12094);
		}
		if (_unit->GetHealthPct() <= 20 && phase == 4) 
		{
			spmin = 7;
			nrspells = 10;
			phase = 5;
			_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , MODEL_DRAGONHAWK);  //change to Dragonhawk
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Ya don' have to look to da sky to see da dragonhawk!");
			_unit->PlaySoundToSet(12095);
		}
		if (_unit->GetHealthPct() <= 1 && phase == 5) 
		{
			_unit->PlaySoundToSet(12100);
			_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , MODEL_TROLL);  //change to Troll
			phase = 6;
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
	int spmin;
	int phase;
};


//Mobs

//SOARING_EAGLE Summon Akil'zon 

#define EAGLE_SWOOP 44732 //INSTANT


class SOARINGEAGLEAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SOARINGEAGLEAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    SOARINGEAGLEAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			nrspells = 1;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(EAGLE_SWOOP);
		spells[0].targettype = TARGET_DESTINATION;
		spells[0].instant = true;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
    }


	void OnTargetDied(Unit* mTarget)
    {

    }

    void OnCombatStop(Unit *mTarget)
    {
		_unit->Despawn(3000,0);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->Despawn(30000,0);
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

//Corrupted Lightning Totem
#define CLT_LIGHTING 43301 //43526 //43301  //43661 //


class CLTOTEMAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(CLTOTEMAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    CLTOTEMAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			nrspells = 1;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(CLT_LIGHTING);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;
		
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		_unit->GetAIInterface()->m_canMove = false;
		RegisterAIUpdateEvent(1000);
    }


	void OnTargetDied(Unit* mTarget)
    {

    }

    void OnCombatStop(Unit *mTarget)
    {
		_unit->Despawn(30,0);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->Despawn(3000,0);
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

//Gazakroth
#define G_FIREBOLT 39023 


class GAZAKROTHAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(GAZAKROTHAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    GAZAKROTHAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			nrspells = 1;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(G_FIREBOLT);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 2.0f;
		spells[0].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
    }


	void OnTargetDied(Unit* mTarget)
    {

    }

    void OnCombatStop(Unit *mTarget)
    {
		_unit->Despawn(3000,0);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->Despawn(30000,0);
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

//Lord Raadan
#define LR_FLAME_BREATH 43215 
#define LR_THUNDERCLAP  30633 //38537, 36706 // 36214  //44033


class LORDRAADANAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(LORDRAADANAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    LORDRAADANAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			nrspells = 1;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(LR_FLAME_BREATH);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 3.0f;
		spells[0].attackstoptimer = 1000;
		
       /* spells[1].info = dbcSpell.LookupEntry(LR_THUNDERCLAP); all types spells to crash server
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].perctrigger = 1.0f;
		spells[1].attackstoptimer = 1000;*/
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
    }


	void OnTargetDied(Unit* mTarget)
    {

    }

    void OnCombatStop(Unit *mTarget)
    {
		_unit->Despawn(3000,0);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->Despawn(30000,0);
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

//SLITHER

#define S_POISON_BOLT 38459 


class SLITHERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SLITHERAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    SLITHERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			nrspells = 1;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(S_POISON_BOLT);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 1000;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
    }


	void OnTargetDied(Unit* mTarget)
    {

    }

    void OnCombatStop(Unit *mTarget)
    {
		_unit->Despawn(3000,0);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->Despawn(30000,0);
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

//THURG

#define TH_CLEAVE 43273


class THURGAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(THURGAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    THURGAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			nrspells = 1;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(TH_CLEAVE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 2.0f;
		spells[0].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
    }


	void OnTargetDied(Unit* mTarget)
    {

    }

    void OnCombatStop(Unit *mTarget)
    {
		_unit->Despawn(3000,0);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->Despawn(30000,0);
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

//DARKHEART

#define DH_FEAR 39415
#define DH_CLEAVE 43273

class DARKHEARTAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(DARKHEARTAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    DARKHEARTAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			nrspells = 2;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(DH_CLEAVE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 3.0f;
		spells[0].attackstoptimer = 1000; 
		
        /*spells[1].info = dbcSpell.LookupEntry(DH_FEAR);   crash
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 1.0f;
		spells[1].attackstoptimer = 1000;*/
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
    }


	void OnTargetDied(Unit* mTarget)
    {

    }

    void OnCombatStop(Unit *mTarget)
    {
		_unit->Despawn(3000,0);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->Despawn(30000,0);
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

//FENSTALKER

#define FEN_TOXIC_SLIME 37615  //40818

class FENSTALKERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(FENSTALKERAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    FENSTALKERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			nrspells = 1;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(FEN_TOXIC_SLIME);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 2.0f;
		spells[0].attackstoptimer = 1000;
		

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
    }


	void OnTargetDied(Unit* mTarget)
    {

    }

    void OnCombatStop(Unit *mTarget)
    {
		_unit->Despawn(3000,0);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->Despawn(30000,0);
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

//ALYSON_ANTILLE
#define ALYS_HEAL_GRONN 36678

class ALYSONANTILLEAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ALYSONANTILLEAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    ALYSONANTILLEAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			nrspells = 1;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(ALYS_HEAL_GRONN);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = false;
		spells[0].perctrigger = 2.0f;
		spells[0].attackstoptimer = 1000;
		

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
    }


	void OnTargetDied(Unit* mTarget)
    {

    }

    void OnCombatStop(Unit *mTarget)
    {
		_unit->Despawn(3000,0);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->Despawn(30000,0);
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

//KORAGG
#define KOR_FIREBALL 40877  

class KORAGGAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(KORAGGAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    KORAGGAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			nrspells = 1;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(KOR_FIREBALL);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 2.0f;
		spells[0].attackstoptimer = 1000;
		

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
    }


	void OnTargetDied(Unit* mTarget)
    {

    }

    void OnCombatStop(Unit *mTarget)
    {
		_unit->Despawn(3000,0);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->Despawn(30000,0);
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

//FEATHER_VORTEX
#define CYCLONE_STRIKE 42495 

class FEATHERVORTEXAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(FEATHERVORTEXAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    FEATHERVORTEXAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			nrspells = 1;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(CYCLONE_STRIKE);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 1000;
		

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
    }


	void OnTargetDied(Unit* mTarget)
    {

    }

    void OnCombatStop(Unit *mTarget)
    {
		_unit->Despawn(3000,0);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->Despawn(30000,0);
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

void SetupZulAman(ScriptMgr * mgr)
{
	mgr->register_creature_script(CN_NALORAKK, &NalorakkAI::Create);
	mgr->register_creature_script(AKILZON, &AKILZONAI::Create);
	mgr->register_creature_script(HALAZZI, &HALAZZIAI::Create);
	mgr->register_creature_script(JANALAI, &JANALAIAI::Create);
	mgr->register_creature_script(HEX_LORD_MALACRASS, &HEXLORDMALACRASSAI::Create);
	mgr->register_creature_script(ZULJIN, &ZULJINAI::Create);
	//MOBS
	mgr->register_creature_script(SOARING_EAGLE, &SOARINGEAGLEAI::Create);
	mgr->register_creature_script(CORRUPTED_LIGHTING_TOTEM, &CLTOTEMAI::Create);
	mgr->register_creature_script(GAZAKROTH, &GAZAKROTHAI::Create);
	mgr->register_creature_script(LORD_RAADAN, &LORDRAADANAI::Create);
	mgr->register_creature_script(SLITHER, &SLITHERAI::Create);
	mgr->register_creature_script(THURG, &THURGAI::Create);
	mgr->register_creature_script(DARKHEART, &DARKHEARTAI::Create);
	mgr->register_creature_script(FENSTALKER, &FENSTALKERAI::Create);
	mgr->register_creature_script(ALYSON_ANTILLE, &ALYSONANTILLEAI::Create);
	mgr->register_creature_script(KORAGG, &KORAGGAI::Create);
	mgr->register_creature_script(FEATHER_VORTEX, &FEATHERVORTEXAI::Create);
}