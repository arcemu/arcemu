#include "StdAfx.h"
#include "Setup.h"

/************************************************************************/
/* ZulAman.cpp Script													*/
/************************************************************************/

//Nalorakk

#define NALORAKK 23576

//TROLL FORM
#define BRUTAL_SWIPE 42384
#define MANGLE 44955
#define SURGE 42402
//BEAR FORM
#define LACERATING_SLASH 42395
#define REND_FLESH 42397
#define DEAFENING_ROAR 42398


class NALORAKKAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(NALORAKKAI);
	SP_AI_Spell spells[6];
	bool m_spellcheck[6];

	NALORAKKAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{

		spells[0].info = dbcSpell.LookupEntry(BRUTAL_SWIPE); 
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].cooldown = 10;
		spells[0].perctrigger = 3.0f;
		spells[0].attackstoptimer = 1000;


		spells[1].info = dbcSpell.LookupEntry(MANGLE); 
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 6.0f;
		spells[1].attackstoptimer = 1000;


		spells[2].info = dbcSpell.LookupEntry(SURGE); 
		spells[2].targettype = TARGET_RANDOM_SINGLE;
		spells[2].instant = false;
		spells[2].cooldown = 3;
		spells[2].perctrigger = 6.0f;
		spells[2].attackstoptimer = 1000;


		spells[3].info = dbcSpell.LookupEntry(LACERATING_SLASH);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = false;
		spells[3].perctrigger = 7.0f;
		spells[3].attackstoptimer = 1000;

		spells[4].info = dbcSpell.LookupEntry(REND_FLESH); 
		spells[4].targettype = TARGET_ATTACKING;
		spells[4].instant = false;
		spells[4].perctrigger = 6.0f;
		spells[4].attackstoptimer = 1000;


		spells[5].info = dbcSpell.LookupEntry(DEAFENING_ROAR); 
		spells[5].targettype = TARGET_VARIOUS;
		spells[5].instant = false;
		spells[5].perctrigger = 8.0f;
		spells[5].attackstoptimer = 1000;

		for(uint32 x = 0; x < 6; x++)
			_unit->DamageDoneModPCT[x] = 0;

		nrspells = 3;
		spmin = 0;
		m_phase = 0;
		m_phasechange = 45;
		m_enrage = 600;
		enraged = 0;

		for(int i=spmin;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
	}

	void OnCombatStart(Unit* mTarget)
	{
		nrspells = 3;
		spmin = 0;
		m_phase = 0;
		m_enrage = 600;
		enraged = 0;
		m_phasechange = 45;
		_unit->PlaySoundToSet(12070);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You be dead soon enough!");
		RegisterAIUpdateEvent(1000);
	}


	void OnTargetDied(Unit* mTarget)
	{
		if (_unit->GetHealthPct() > 0)	
		{
			uint32 RandomSpeach=RandomUInt(2);
			switch (RandomSpeach)
			{
			case 0:
				_unit->PlaySoundToSet(12076);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Di Amani gonna rule again!");
				break;
			case 1:
				_unit->PlaySoundToSet(12075);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Now whatchoo got to say?");
				break;
			}
		}
	}

	void OnCombatStop(Unit *mTarget)
	{
		if (_unit->isAlive())
			_unit->Despawn(100, 2500);

		_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , 21631);  //change to Troll
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		_unit->PlaySoundToSet(12077);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I... be waitin' on da udda side....");
		RemoveAIUpdateEvent();
	}

	void AIUpdate()
	{
		m_enrage--;
		m_phasechange--;
		if (!m_enrage && !enraged)
		{
			for(uint32 x = 0; x < 6; x++)
				_unit->DamageDoneModPCT[x] = 3;

			enraged = 1;
		}
		if (!m_phasechange)
		{
			switch (m_phase)
			{
			case 0:
				//bear
				nrspells = 6;
				spmin = 3;
				m_phase = 1;
				m_phasechange = 30;
				_unit->PlaySoundToSet(12072);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You call on da beast, you gonna get more dan you bargain for!");
				_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , 706);
				break;
			case 1:
				//human
				nrspells = 3;
				spmin = 0;
				m_phase = 0;
				m_phasechange = 45;
				//_unit->PlaySoundToSet(11313);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Make way for Nalorakk!");
				_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , 21631);
				break;
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
					case TARGET_RANDOM_SINGLE:
						target = RandomTarget(false, true, spells[i].info->base_range_or_radius_sqr);
						if (target)
						{
							_unit->CastSpell(target, spells[i].info, spells[i].instant);
						}
						break;
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
	Unit *RandomTarget(bool tank,bool onlyplayer, float dist)
	{
		if (_unit->GetAIInterface()->getAITargetsCount() == 0)
			return NULL;

		std::vector<Unit*> targetTable;
		TargetMap *targets = _unit->GetAIInterface()->GetAITargets();
		TargetMap::iterator itr;
		for (itr = targets->begin(); itr != targets->end(); itr++)
		{
			Unit *temp = _unit->GetMapMgr()->GetUnit(itr->first);
			if (_unit->GetDistance2dSq(temp) <= dist)
			{
				if (((!tank && temp != _unit->GetAIInterface()->GetNextTarget()) || tank) && (!onlyplayer || (onlyplayer && temp->GetTypeId() == TYPEID_PLAYER)))
				{
					targetTable.push_back(temp);
				}
			}
		}
		if (!targetTable.size())
			return NULL;

		uint32 randt = RandomUInt(100)%targetTable.size();
		Unit * randomtarget = targetTable[randt];
		return randomtarget;
	}
protected:

	int nrspells;
	int m_phase;
	int m_phasechange;
	int m_enrage;
	int enraged;
	int spmin;
};

//Akil'zon <Eagle Avatar>

#define AKILZON 23574
#define SOARING_EAGLE 24858


#define STATIC_DISRUPTION 44008
#define CALL_LIGHTING 43661
#define GUST_OF_WIND 43621
#define ELECTRICAL_STORM 43648
#define ELECTRICAL_STORM_CLOUD 44007


class AKILZONAI : public CreatureAIScript, public EventableObject
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(AKILZONAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

	AKILZONAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(STATIC_DISRUPTION);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = false;
		spells[0].perctrigger = 4.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(CALL_LIGHTING);
		spells[1].targettype = TARGET_RANDOM_SINGLE;
		spells[1].instant = false;
		spells[1].perctrigger = 5.0f;
		spells[1].attackstoptimer = 1000;


		spells[2].info = dbcSpell.LookupEntry(GUST_OF_WIND);
		spells[2].targettype = TARGET_RANDOM_SINGLE;
		spells[2].instant = false;
		spells[2].perctrigger = 9.0f;
		spells[2].attackstoptimer = 1000;

		for(uint32 x = 0; x < 6; x++)
			_unit->DamageDoneModPCT[x] = 0;

		m_electicalstorm = 50 + RandomUInt(10);
		m_enrage = 600;
		enraged = 0;
	}

	void OnCombatStart(Unit* mTarget)
	{
		m_electicalstorm = 50 + RandomUInt(10);
		m_enrage = 600;
		enraged = 0;
		_unit->PlaySoundToSet(12013);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I be da predator! You da prey!");
		RegisterAIUpdateEvent(1000);
	}


	void OnTargetDied(Unit* mTarget)
	{
		if (_unit->GetHealthPct() > 0)	
		{
			uint32 RandomSpeach = RandomUInt(2);
			switch (RandomSpeach)
			{
			case 0:
				_unit->PlaySoundToSet(12018);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Stop your cryin'!");
				break;
			case 1:
				_unit->PlaySoundToSet(12017);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Ya got nothin'!");
				break;
			}
		}
	}

	void OnCombatStop(Unit *mTarget)
	{
		if (_unit->isAlive())
			_unit->Despawn(100, 2500);

		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		_unit->PlaySoundToSet(12019);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You can't... kill... me spirit!");
		RemoveAIUpdateEvent();
	}

	void AIUpdate()
	{
		m_enrage--;
		if (!m_enrage && !enraged)
		{
			for(uint32 x = 0; x < 6; x++)
				_unit->DamageDoneModPCT[x] = 5;
			enraged = 1;
		}
		m_electicalstorm--;
		if (!m_electicalstorm)
		{
			m_electicalstorm = 50 + RandomUInt(10);
			Player *target = (Player *) RandomTarget(false, true, 10000);
			if (target)
			{
				_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), dbcSpell.LookupEntry(ELECTRICAL_STORM_CLOUD), true);
				target->SafeTeleport(target->GetMapId(), target->GetInstanceID(), target->GetPositionX(), target->GetPositionY(), target->GetPositionZ() + 10.0f, target->GetOrientation());
				_unit->CastSpell(target, ELECTRICAL_STORM, false);
				sEventMgr.AddEvent(this, &AKILZONAI::summonEagles, EVENT_SCRIPT_UPDATE_EVENT, 10000, 1, 0);
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
					case TARGET_RANDOM_SINGLE:
						target = RandomTarget(false, true, spells[i].info->base_range_or_radius_sqr);
						if (target)
						{
							_unit->CastSpell(target, spells[i].info, spells[i].instant);
						}
						break;
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
	Unit *RandomTarget(bool tank,bool onlyplayer, float dist)
	{
		if (_unit->GetAIInterface()->getAITargetsCount() == 0)
			return NULL;

		std::vector<Unit*> targetTable;
		TargetMap *targets = _unit->GetAIInterface()->GetAITargets();
		TargetMap::iterator itr;
		for (itr = targets->begin(); itr != targets->end(); itr++)
		{
			Unit *temp = _unit->GetMapMgr()->GetUnit(itr->first);
			if (_unit->GetDistance2dSq(temp) <= dist)
			{
				if (((!tank && temp != _unit->GetAIInterface()->GetNextTarget()) || tank) && (!onlyplayer || (onlyplayer && temp->GetTypeId() == TYPEID_PLAYER)))
				{
					targetTable.push_back(temp);
				}
			}
		}
		if (!targetTable.size())
			return NULL;

		uint32 randt = RandomUInt(100)%targetTable.size();
		Unit * randomtarget = targetTable[randt];
		return randomtarget;
	}
	void summonEagles()
	{
		_unit->PlaySoundToSet(12019);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Feed, me bruddahs!");
		Creature *cre = NULL;
		for (int x = 0; x < 6;x++)
		{
			cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(SOARING_EAGLE, _unit->GetPositionX() + RandomFloat(5) - 5, _unit->GetPositionY() + RandomFloat(5) - 5, _unit->GetPositionZ(), _unit->GetOrientation(), true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
			cre->GetAIInterface()->setOutOfCombatRange(5000);
		}

	}
protected:

	int nrspells;
	int m_electicalstorm;
	int m_enrage;
	int enraged;
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
		spells[0].perctrigger = 3.0f;
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
		_unit->Despawn(100,0);
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		_unit->Despawn(100,0);
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

//Halazzi <Lynx Avatar>

#define HALAZZI 23577
#define HALAZZILYNX 24143
#define CORRUPTED_LIGHTING_TOTEM 24224 

//Phase 1
#define SABER_LASH 43267 //43267 //43268 ///40810 //40816
//Phase 2
#define FLAME_SHOCK 43303
#define EARTH_SHOCK 43305

class HALAZZIAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(HALAZZIAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

	HALAZZIAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{

		spells[0].info = dbcSpell.LookupEntry(SABER_LASH);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].cooldown = 10;
		spells[0].perctrigger = 1.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(FLAME_SHOCK);
		spells[1].targettype = TARGET_RANDOM_SINGLE; 
		spells[1].instant = false;
		spells[1].perctrigger = 3.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(EARTH_SHOCK);
		spells[2].targettype = TARGET_RANDOM_SINGLE; 
		spells[2].instant = false;
		spells[2].perctrigger = 3.0f;
		spells[2].attackstoptimer = 1000;

		enrage = false;
		m_enrage = 600;
		m_phase =1;
		m_phasestep = 1;
		lynx = NULL;
		nrspells = 1;
		spmin = 0;

		for(uint32 x = 0; x < 6; x++)
			_unit->DamageDoneModPCT[x] = 0;

		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
	}

	void OnCombatStart(Unit* mTarget)
	{
		nrspells = 1;
		spmin = 0;
		m_phase =1;
		m_phasestep = 1;
		enrage = false;
		m_enrage = 600;
		lynx = NULL;

		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Get on your knees and bow to da fang and claw!");
		_unit->PlaySoundToSet(12020);

		GameObject *door2 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(369.33f, 1057.30f, 10.06f, 186304);
		if (door2 != NULL)
			door2->SetUInt32Value(GAMEOBJECT_STATE, 1);

		RegisterAIUpdateEvent(1000);
	}


	void OnTargetDied(Unit* mTarget)
	{
		if (_unit->GetHealthPct() > 0)	
		{
			uint32 RandomSpeach=RandomUInt(2);
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
		if (_unit->isAlive())
			_unit->Despawn(100, 2500);

		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();

		GameObject *door2 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(369.33f, 1057.30f, 10.06f, 186304);
		if (door2 != NULL)
			door2->SetUInt32Value(GAMEOBJECT_STATE, 0);
	}

	void OnDied(Unit * mKiller)
	{
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Chaga... choka'jinn.");
		_unit->PlaySoundToSet(12028);
		RemoveAIUpdateEvent();

		GameObject *door = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(305.53f, 1117.75f, 10.19f, 186303);
		if (door != NULL)
			door->SetUInt32Value(GAMEOBJECT_STATE, 0);

		GameObject *door2 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(369.33f, 1057.30f, 10.06f, 186304);
		if (door2 != NULL)
			door2->SetUInt32Value(GAMEOBJECT_STATE, 0);
	}

	void AIUpdate()
	{
		m_enrage--;
		if (!enrage && !m_enrage)
		{
			for(uint32 x = 0; x < 6; x++)
				_unit->DamageDoneModPCT[x] = 5;

			enrage = true;
		}

		float val = RandomFloat(100.0f);
		SpellCast(val);

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
		}
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
					case TARGET_RANDOM_SINGLE:
						target = RandomTarget(false, true, spells[i].info->base_range_or_radius_sqr);
						if (target)
						{
							_unit->CastSpell(target, spells[i].info, spells[i].instant);
						}
						break;
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
	Unit *RandomTarget(bool tank,bool onlyplayer, float dist)
	{
		if (_unit->GetAIInterface()->getAITargetsCount() == 0)
			return NULL;

		std::vector<Unit*> targetTable;
		TargetMap *targets = _unit->GetAIInterface()->GetAITargets();
		TargetMap::iterator itr;
		for (itr = targets->begin(); itr != targets->end(); itr++)
		{
			Unit *temp = _unit->GetMapMgr()->GetUnit(itr->first);
			if (_unit->GetDistance2dSq(temp) <= dist)
			{
				if (((!tank && temp != _unit->GetAIInterface()->GetNextTarget()) || tank) && (!onlyplayer || (onlyplayer && temp->GetTypeId() == TYPEID_PLAYER)))
				{
					targetTable.push_back(temp);
				}
			}
		}
		if (!targetTable.size())
			return NULL;

		uint32 randt = RandomUInt(100)%targetTable.size();
		Unit * randomtarget = targetTable[randt];
		return randomtarget;
	}

	void PhaseOne()
	{
		switch (m_phasestep)
		{
		case 1:
			if (_unit->GetHealthPct() <= 75)
			{
				spmin = 1;
				nrspells = 3;
				m_phase = 2;
				m_totemcd = 50 + RandomUInt(10);
				_unit->SetUInt32Value(UNIT_FIELD_HEALTH, _unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH));
				if (_unit->GetMapMgr() != NULL)
					lynx = _unit->GetMapMgr()->GetInterface()->SpawnCreature(HALAZZILYNX, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 0.0f, true, false, 0, 0);
			}
			break;
		case 2:
			if (_unit->GetHealthPct() <= 50)
			{
				spmin = 1;
				nrspells = 3;
				m_phase = 2;
				_unit->SetUInt32Value(UNIT_FIELD_HEALTH, _unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH));
				if (_unit->GetMapMgr() != NULL)
					lynx = _unit->GetMapMgr()->GetInterface()->SpawnCreature(HALAZZILYNX, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 0.0f, true, false, 0, 0);
			}
			break;
		case 3:
			if (_unit->GetHealthPct() <= 25)
			{
				spmin = 1;
				nrspells = 3;
				m_phase = 2;
				_unit->SetUInt32Value(UNIT_FIELD_HEALTH, _unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH));
				if (_unit->GetMapMgr() != NULL)
					lynx = _unit->GetMapMgr()->GetInterface()->SpawnCreature(HALAZZILYNX, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 0.0f, true, false, 0, 0);
			}
			break;
		}
	}
	void PhaseTwo()
	{
		m_totemcd--;
		if (_unit->GetHealthPct() <= 20 || (lynx != NULL && lynx->GetHealthPct() <= 20))
		{
			switch (m_phasestep)
			{
			case 1:
				nrspells = 1;
				spmin = 0;
				m_phase =1;
				m_phasestep = 2;
				if (lynx != NULL)
					lynx->Despawn(100, 0);
				_unit->SetUInt32Value(UNIT_FIELD_HEALTH, (uint32)(_unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH)*0.75));
				break;
			case 2:
				nrspells = 1;
				spmin = 0;
				m_phase =1;
				m_phasestep = 3;
				if (lynx != NULL)
					lynx->Despawn(100, 0);
				_unit->SetUInt32Value(UNIT_FIELD_HEALTH, (uint32)(_unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH)*0.50));
				break;
			case 3:
				if (lynx != NULL)
					lynx->Despawn(100, 0);
				_unit->SetUInt32Value(UNIT_FIELD_HEALTH, (uint32)(_unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH)*0.25));
				m_phase = 3;
				spmin = 0;
				nrspells = 3;
				break;
			}
		}
		if (!m_totemcd)
		{
			if (_unit->GetMapMgr() != NULL)
				Creature *cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CORRUPTED_LIGHTING_TOTEM, _unit->GetPositionX()+2, _unit->GetPositionY()+2, _unit->GetPositionZ(), _unit->GetOrientation(), true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0);
			m_totemcd = 35 + RandomUInt(10);
		}
	}
	void PhaseThree()
	{
		m_totemcd--;
		if (!m_totemcd)
		{
			if (_unit->GetMapMgr() != NULL)
				Creature *cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CORRUPTED_LIGHTING_TOTEM, _unit->GetPositionX()+2, _unit->GetPositionY()+2, _unit->GetPositionZ(), _unit->GetOrientation(), true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0);
			m_totemcd = 15 + RandomUInt(10);
		}
	}
protected:

	int nrspells;
	int spmin;
	int m_phase;
	int m_phasestep;
	uint32 m_totemcd;
	int m_enrage;
	bool enrage;
	Creature *lynx;
};

//Corrupted Lightning Totem
#define CLT_LIGHTING 43301 //43526 //43301  //43661 //


class CLTOTEMAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(CLTOTEMAI);

	CLTOTEMAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		_unit->GetAIInterface()->m_canMove = false;
	}

	void OnCombatStart(Unit* mTarget)
	{
		RegisterAIUpdateEvent(2000);
	}


	void OnTargetDied(Unit* mTarget)
	{

	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->Despawn(100,0);
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
		_unit->Despawn(1000,0);
	}

	void AIUpdate()
	{
		if (_unit->GetCurrentSpell() == NULL)
			_unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), CLT_LIGHTING, false);
	}
protected:
};

//Lynx Spirit
#define LYNXFLURRY 43290
#define LYNXSHREDARMOR 43243


class LYNXSPIRITAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(LYNXSPIRITAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

	LYNXSPIRITAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		spells[0].info = dbcSpell.LookupEntry(LYNXFLURRY);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = false;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(LYNXSHREDARMOR);
		spells[1].targettype = TARGET_ATTACKING; 
		spells[1].instant = false;
		spells[1].perctrigger = 14.0f;
		spells[1].attackstoptimer = 1000;

		nrspells = 2;
		spmin = 0;

		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
	}

	void OnCombatStart(Unit* mTarget)
	{
		RegisterAIUpdateEvent(2000);
	}


	void OnTargetDied(Unit* mTarget)
	{

	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->Despawn(100,0);
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
					case TARGET_RANDOM_SINGLE:
						target = RandomTarget(false, true, spells[i].info->base_range_or_radius_sqr);
						if (target)
						{
							_unit->CastSpell(target, spells[i].info, spells[i].instant);
						}
						break;
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
	Unit *RandomTarget(bool tank,bool onlyplayer, float dist)
	{
		if (_unit->GetAIInterface()->getAITargetsCount() == 0)
			return NULL;

		std::vector<Unit*> targetTable;
		TargetMap *targets = _unit->GetAIInterface()->GetAITargets();
		TargetMap::iterator itr;
		for (itr = targets->begin(); itr != targets->end(); itr++)
		{
			Unit *temp = _unit->GetMapMgr()->GetUnit(itr->first);
			if (_unit->GetDistance2dSq(temp) <= dist)
			{
				if (((!tank && temp != _unit->GetAIInterface()->GetNextTarget()) || tank) && (!onlyplayer || (onlyplayer && temp->GetTypeId() == TYPEID_PLAYER)))
				{
					targetTable.push_back(temp);
				}
			}
		}
		if (!targetTable.size())
			return NULL;

		uint32 randt = RandomUInt(100)%targetTable.size();
		Unit * randomtarget = targetTable[randt];
		return randomtarget;
	}

protected:
	int nrspells;
	int spmin;
};


//Jan'alai <Dragonhawk Avatar>
#define JANALAI 23578
#define AMANIDRAGONHAWKHATCHLING 23598
#define FIREBOMBTARGET 23920

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
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

	JANALAIAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{

		spells[0].info = dbcSpell.LookupEntry(FLAME_BREATH);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = false;
		spells[0].perctrigger = 9.0f;
		spells[0].attackstoptimer = 1000;

		nrspells = 1;
		spmin = 0;
		softenrage = false;
		enraged = false;
		m_enrage = 600;
		m_softenrage = 300;
		m_firebombs = 35 + RandomUInt(25);
		m_adds = 14;

		for(uint32 x = 0; x < 6; x++)
			_unit->DamageDoneModPCT[x] = 0;

		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
	}

	void OnCombatStart(Unit* mTarget)
	{
		m_firebombs = 35 + RandomUInt(25);
		m_softenrage = 300;
		m_adds = 14;
		softenrage = false;
		enraged = false;
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Spirits Of the wind,be your doom!");
		_unit->PlaySoundToSet(12031);
		RegisterAIUpdateEvent(1000);
	}


	void OnTargetDied(Unit* mTarget)
	{
		if (_unit->GetHealthPct() > 0)	
		{
			uint32 RandomSpeach=RandomUInt(2);
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
		if (_unit->isAlive())
			_unit->Despawn(100, 2500);

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
		m_adds--;
		m_enrage--;
		m_firebombs--;
		m_softenrage--;
		if (!m_enrage && !enraged)
		{
			enraged = true;

			for(uint32 x = 0; x < 6; x++)
				_unit->DamageDoneModPCT[x] = 3;
		}
		if (!softenrage && (_unit->GetHealthPct() <= 25 || !m_softenrage)) 
		{
			softenrage = true;
			_unit->CastSpell(_unit, dbcSpell.LookupEntry(JANALAI_ENRAGE), true);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Where ma hatcha? Get to work on dem eggs!");
			_unit->PlaySoundToSet(12033);
		}
		if (!m_firebombs)
		{
			float x;
			float y;

			for (int z = 0; z < 16;z++)
			{
				x = -51.0f + RandomUInt(39);
				y = 1132.0f + RandomUInt(35);
				_unit->GetMapMgr()->GetInterface()->SpawnCreature(FIREBOMBTARGET, x, y, _unit->GetPositionZ(), 0.0f, true, false, 0, 0);
			}
			m_firebombs = 35 + RandomUInt(25);
		}
		if (!m_adds)
		{
			Creature *summon;
			summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(AMANIDRAGONHAWKHATCHLING, -34.23f, 1099.19f, 18.71f, 0, true, false, 0, 0);
			summon->GetAIInterface()->_CalcDestinationAndMove(_unit, 5);
			summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(AMANIDRAGONHAWKHATCHLING, -34.23f, 1099.19f, 18.71f, 0, true, false, 0, 0);
			summon->GetAIInterface()->_CalcDestinationAndMove(_unit, 5);
			summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(AMANIDRAGONHAWKHATCHLING, -33.18f, 1201.79f, 18.71f, 0, true, false, 0, 0);
			summon->GetAIInterface()->_CalcDestinationAndMove(_unit, 5);
			summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(AMANIDRAGONHAWKHATCHLING, -33.18f, 1201.79f, 18.71f, 0, true, false, 0, 0);
			summon->GetAIInterface()->_CalcDestinationAndMove(_unit, 5);
			m_adds = 80 + RandomUInt(25);
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
					case TARGET_RANDOM_SINGLE:
						target = RandomTarget(false, true, spells[i].info->base_range_or_radius_sqr);
						if (target)
						{
							_unit->CastSpell(target, spells[i].info, spells[i].instant);
						}
						break;
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
	Unit *RandomTarget(bool tank,bool onlyplayer, float dist)
	{
		if (_unit->GetAIInterface()->getAITargetsCount() == 0)
			return NULL;

		std::vector<Unit*> targetTable;
		TargetMap *targets = _unit->GetAIInterface()->GetAITargets();
		TargetMap::iterator itr;
		for (itr = targets->begin(); itr != targets->end(); itr++)
		{
			Unit *temp = _unit->GetMapMgr()->GetUnit(itr->first);
			if (_unit->GetDistance2dSq(temp) <= dist)
			{
				if (((!tank && temp != _unit->GetAIInterface()->GetNextTarget()) || tank) && (!onlyplayer || (onlyplayer && temp->GetTypeId() == TYPEID_PLAYER)))
				{
					targetTable.push_back(temp);
				}
			}
		}
		if (!targetTable.size())
			return NULL;

		uint32 randt = RandomUInt(100)%targetTable.size();
		Unit * randomtarget = targetTable[randt];
		return randomtarget;
	}
protected:

	int m_enrage;
	bool enraged;
	bool softenrage;
	int nrspells;
	int spmin;
	int m_firebombs;
	int m_softenrage;
	int m_adds;
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
#define HEX_LORD_MALACRASS_DRAIN_POWER 44131
#define HEX_LORD_MALACRASS_SPIRIT_BOLTS 43383
#define HEX_LORD_MALACRASS_SIPHON_SOUL 43501

/////////class spells
//Druid
#define HEX_LORD_MALACRASS_LIFEBLOOM 43421
#define HEX_LORD_MALACRASS_THORNS 43420
#define HEX_LORD_MALACRASS_MOONFIRE 43545
//Hunter
#define HEX_LORD_MALACRASS_EXPLOSIVE_TRAP 43444
#define HEX_LORD_MALACRASS_FREEZING_TRAP 43447
#define HEX_LORD_MALACRASS_SNAKE_TRAP 43449
//Mage
#define HEX_LORD_MALACRASS_FROSTBOLT 43428
#define HEX_LORD_MALACRASS_FROST_NOVA 43426
#define HEX_LORD_MALACRASS_ICE_LANCE 43427
//Paladin
#define HEX_LORD_MALACRASS_CONSECRATION 43429
#define HEX_LORD_MALACRASS_HOLY_LIGHT 43451
#define HEX_LORD_MALACRASS_AVENGING_WRATH 43430
//Priest
#define HEX_LORD_MALACRASS_MIND_CONTROL 43550
#define HEX_LORD_MALACRASS_PSYCHIC_SCREAM 43432
#define HEX_LORD_MALACRASS_FLASH_HEAL 43431
//Rogue
#define HEX_LORD_MALACRASS_BLIND 43433
#define HEX_LORD_MALACRASS_SLICE_AND_DICE 43547
#define HEX_LORD_MALACRASS_WOUND_POISON 39665
//Shaman
#define HEX_LORD_MALACRASS_FIRE_NOVA_TOTEM 43436
#define HEX_LORD_MALACRASS_HEALING_WAVE 43547
#define HEX_LORD_MALACRASS_CHAIN_LIGHTNING 43435
//Warlock
#define HEX_LORD_MALACRASS_CURSE_OF_DOOM 43439
#define HEX_LORD_MALACRASS_RAIN_OF_FIRE 43440
#define HEX_LORD_MALACRASS_UNSTABLE_AFFILICTION 43522 //35183 ??
//Warrior
#define HEX_LORD_MALACRASS_SPELL_REFLECTION 43443
#define HEX_LORD_MALACRASS_WHIRLWIND 43442
#define HEX_LORD_MALACRASS_MORTAL_STRIKE 43441

class HEXLORDMALACRASSAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HEXLORDMALACRASSAI);
	SP_AI_Spell spells[27];
	bool m_spellcheck[27];

    HEXLORDMALACRASSAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			nrspells = 27;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
		spells[0].info = dbcSpell.LookupEntry(HEX_LORD_MALACRASS_LIFEBLOOM);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].perctrigger = 6.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(HEX_LORD_MALACRASS_THORNS);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].perctrigger = 5.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(HEX_LORD_MALACRASS_MOONFIRE);
		spells[2].targettype = TARGET_RANDOM_SINGLE;
		spells[2].instant = true;
		spells[2].perctrigger = 5.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(HEX_LORD_MALACRASS_EXPLOSIVE_TRAP);
		spells[3].targettype = TARGET_RANDOM_DESTINATION;
		spells[3].instant = true;
		spells[3].perctrigger = 5.0f;
		spells[3].attackstoptimer = 1000;

		spells[4].info = dbcSpell.LookupEntry(HEX_LORD_MALACRASS_FREEZING_TRAP);
		spells[4].targettype = TARGET_RANDOM_DESTINATION;
		spells[4].instant = true;
		spells[4].perctrigger = 5.0f;
		spells[4].attackstoptimer = 1000;

		spells[5].info = dbcSpell.LookupEntry(HEX_LORD_MALACRASS_SNAKE_TRAP);
		spells[5].targettype = TARGET_RANDOM_DESTINATION;
		spells[5].instant = true;
		spells[5].perctrigger = 5.0f;
		spells[5].attackstoptimer = 1000;

		spells[6].info = dbcSpell.LookupEntry(HEX_LORD_MALACRASS_FROSTBOLT);
		spells[6].targettype = TARGET_RANDOM_SINGLE;
		spells[6].instant = false;
		spells[6].perctrigger = 8.0f;
		spells[6].attackstoptimer = 1000;

		spells[7].info = dbcSpell.LookupEntry(HEX_LORD_MALACRASS_FROST_NOVA);
		spells[7].targettype = TARGET_VARIOUS;
		spells[7].instant = true;
		spells[7].perctrigger = 5.0f;
		spells[7].attackstoptimer = 1000;

		spells[8].info = dbcSpell.LookupEntry(HEX_LORD_MALACRASS_ICE_LANCE);
		spells[8].targettype = TARGET_RANDOM_SINGLE;
		spells[8].instant = true;
		spells[8].perctrigger = 5.0f;
		spells[8].attackstoptimer = 1000;

		spells[9].info = dbcSpell.LookupEntry(HEX_LORD_MALACRASS_CONSECRATION);
		spells[9].targettype = TARGET_VARIOUS;
		spells[9].instant = true;
		spells[9].perctrigger = 5.0f;
		spells[9].attackstoptimer = 1000;

		spells[10].info = dbcSpell.LookupEntry(HEX_LORD_MALACRASS_HOLY_LIGHT);
		spells[10].targettype = TARGET_SELF;
		spells[10].instant = false;
		spells[10].perctrigger = 5.0f;
		spells[10].attackstoptimer = 1000;

		spells[11].info = dbcSpell.LookupEntry(HEX_LORD_MALACRASS_AVENGING_WRATH);
		spells[11].targettype = TARGET_SELF;
		spells[11].instant = true;
		spells[11].perctrigger = 5.0f;
		spells[11].attackstoptimer = 1000;

		spells[12].info = dbcSpell.LookupEntry(HEX_LORD_MALACRASS_MIND_CONTROL);
		spells[12].targettype = TARGET_RANDOM_SINGLE;
		spells[12].instant = false;
		spells[12].perctrigger = 5.0f;
		spells[12].attackstoptimer = 1000;

		spells[13].info = dbcSpell.LookupEntry(HEX_LORD_MALACRASS_PSYCHIC_SCREAM);
		spells[13].targettype = TARGET_VARIOUS;
		spells[13].instant = true;
		spells[13].perctrigger = 5.0f;
		spells[13].attackstoptimer = 1000;

		spells[14].info = dbcSpell.LookupEntry(HEX_LORD_MALACRASS_FLASH_HEAL);
		spells[14].targettype = TARGET_SELF;
		spells[14].instant = false;
		spells[14].perctrigger = 5.0f;
		spells[14].attackstoptimer = 1000;

		spells[15].info = dbcSpell.LookupEntry(HEX_LORD_MALACRASS_BLIND);
		spells[15].targettype = TARGET_RANDOM_SINGLE;
		spells[15].instant = true;
		spells[15].perctrigger = 5.0f;
		spells[15].attackstoptimer = 1000;

		spells[16].info = dbcSpell.LookupEntry(HEX_LORD_MALACRASS_SLICE_AND_DICE);
		spells[16].targettype = TARGET_SELF;
		spells[16].instant = true;
		spells[16].perctrigger = 5.0f;
		spells[16].attackstoptimer = 1000;

		spells[17].info = dbcSpell.LookupEntry(HEX_LORD_MALACRASS_WOUND_POISON);
		spells[17].targettype = TARGET_ATTACKING;
		spells[17].instant = true;
		spells[17].perctrigger = 5.0f;
		spells[17].attackstoptimer = 1000;

		spells[18].info = dbcSpell.LookupEntry(HEX_LORD_MALACRASS_FIRE_NOVA_TOTEM);
		spells[18].targettype = TARGET_SELF;
		spells[18].instant = true;
		spells[18].perctrigger = 5.0f;
		spells[18].attackstoptimer = 1000;

		spells[19].info = dbcSpell.LookupEntry(HEX_LORD_MALACRASS_HEALING_WAVE);
		spells[19].targettype = TARGET_SELF;
		spells[19].instant = false;
		spells[19].perctrigger = 5.0f;
		spells[19].attackstoptimer = 1000;

		spells[20].info = dbcSpell.LookupEntry(HEX_LORD_MALACRASS_CHAIN_LIGHTNING);
		spells[20].targettype = TARGET_RANDOM_SINGLE;
		spells[20].instant = false;
		spells[20].perctrigger = 5.0f;
		spells[20].attackstoptimer = 1000;

		spells[21].info = dbcSpell.LookupEntry(HEX_LORD_MALACRASS_CURSE_OF_DOOM);
		spells[21].targettype = TARGET_RANDOM_SINGLE;
		spells[21].instant = true;
		spells[21].perctrigger = 5.0f;
		spells[21].attackstoptimer = 1000;

		spells[22].info = dbcSpell.LookupEntry(HEX_LORD_MALACRASS_RAIN_OF_FIRE);
		spells[22].targettype = TARGET_RANDOM_DESTINATION;
		spells[22].instant = false;
		spells[22].perctrigger = 5.0f;
		spells[22].attackstoptimer = 1000;

		spells[23].info = dbcSpell.LookupEntry(HEX_LORD_MALACRASS_UNSTABLE_AFFILICTION);
		spells[23].targettype = TARGET_RANDOM_SINGLE;
		spells[23].instant = true;
		spells[23].perctrigger = 5.0f;
		spells[23].attackstoptimer = 1000;

		spells[24].info = dbcSpell.LookupEntry(HEX_LORD_MALACRASS_SPELL_REFLECTION);
		spells[24].targettype = TARGET_SELF;
		spells[24].instant = true;
		spells[24].perctrigger = 5.0f;
		spells[24].attackstoptimer = 1000;

		spells[25].info = dbcSpell.LookupEntry(HEX_LORD_MALACRASS_WHIRLWIND);
		spells[25].targettype = TARGET_VARIOUS;
		spells[25].instant = true;
		spells[25].perctrigger = 5.0f;
		spells[25].attackstoptimer = 1000;

		spells[26].info = dbcSpell.LookupEntry(HEX_LORD_MALACRASS_MORTAL_STRIKE);
		spells[26].targettype = TARGET_ATTACKING;
		spells[26].instant = true;
		spells[26].perctrigger = 5.0f;
		spells[26].attackstoptimer = 1000;
 

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Da shadow gonna fall on you...");
		_unit->PlaySoundToSet(12041);
		timer = 5;
		seconds = 0;
		mobs = 0;
		RegisterAIUpdateEvent(1000);
    }


    void OnTargetDied(Unit* mTarget)
    {
				if (_unit->GetHealthPct() > 0)	
		{
			uint32 RandomSpeach = RandomUInt(2);
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
	spmin = 0;
	nrspells = 1;
	seconds = 0;
	timer = 0;
	
	for( set<Player*>::iterator PlayerIter = _unit->GetInRangePlayerSetBegin(); PlayerIter != _unit->GetInRangePlayerSetEnd(); ++PlayerIter ) 
	{
		(*PlayerIter)->RemoveAllAuras(HEX_LORD_MALACRASS_DRAIN_POWER,0);
	}	

	if (_unit->isAlive())
		_unit->Despawn(100, 2500);

	_unit->RemoveAllAuras(HEX_LORD_MALACRASS_DRAIN_POWER+1,0);
	_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
	_unit->GetAIInterface()->SetAIState(STATE_IDLE);
	RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		for( set<Player*>::iterator PlayerIter = _unit->GetInRangePlayerSetBegin(); PlayerIter != _unit->GetInRangePlayerSetEnd(); ++PlayerIter ) 
		{
			(*PlayerIter)->RemoveAllAuras(HEX_LORD_MALACRASS_DRAIN_POWER,0);
		}	

		_unit->RemoveAllAuras(HEX_LORD_MALACRASS_DRAIN_POWER+1,0);
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
			uint32 summons[8] = { GAZAKROTH, DARKHEART, FENSTALKER, SLITHER, ALYSON_ANTILLE, THURG, KORAGG, LORD_RAADAN };
			// TODO: pick or drop 4 of those ... the code below allows to spawn 2 same mobs

			for(int j=0;j<4;j++)
			{
				uint32 RandomSpeach=RandomUInt(8);
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
				_unit->GetPositionZ() + 2.0f, _unit->GetOrientation(),
				true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
				if (cre != NULL)
					cre->GetAIInterface()->setOutOfCombatRange(50000);
			}
			mobs = 1;
		}

		// Soul Siphon
		if(seconds % 40 == 0) {
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your will belong ta me now!");
			_unit->PlaySoundToSet(12045);
			Unit *siphon_player = RandomTarget(true, true, 10000);
			_unit->CastSpell(siphon_player, HEX_LORD_MALACRASS_SIPHON_SOUL, true);

			switch (siphon_player->getClass())
			{
				case DRUID:
					nrspells = 3;
					spmin = 0;
					break;
				case HUNTER:
					nrspells = 6;
					spmin = 3;
					break;
				case MAGE:
					nrspells = 9;
					spmin = 6;
					break;
				case PALADIN:
					nrspells = 12;
					spmin = 9;
					break;
				case PRIEST:
					nrspells = 15;
					spmin = 12;
					break;
				case ROGUE:
					nrspells = 18;
					spmin = 15;
					break;
				case SHAMAN:
					nrspells = 21;
					spmin = 18;
					break;
				case WARLOCK:
					nrspells = 24;
					spmin = 21;
					break;
				case WARRIOR:
					nrspells = 27;
					spmin = 24;
					break;
				default:
					nrspells = 1;
					spmin = 0;
			}
		}

		// Spirit bolts
		if(seconds % 40 == 30) {
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your soul gonna bleed!");
			_unit->PlaySoundToSet(12047);
			_unit->CastSpell(_unit, HEX_LORD_MALACRASS_SPIRIT_BOLTS, false);
		}

		// Drain Power TODO: trigger a spell 44132 from each affected (and alive) player back to malacrass (spelleffect #140)
		if(seconds > 0 && seconds % 60 == 0) {
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Darkness comin' for you...");
			_unit->PlaySoundToSet(12046);
			_unit->CastSpell(_unit, HEX_LORD_MALACRASS_DRAIN_POWER, true);
		}


		if (timer)
		{
			timer--;
		}
		seconds++;
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
						case TARGET_RANDOM_SINGLE:
							target = RandomTarget(false, true, spells[i].info->base_range_or_radius_sqr);
							if (target)
							{
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
							} break;
						case TARGET_RANDOM_DESTINATION:
							target = RandomTarget(false, true, 10000);
							if (target != NULL)
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
	Unit *RandomTarget(bool tank,bool onlyplayer, float dist)
	{
		if (_unit->GetAIInterface()->getAITargetsCount() == 0)
			return NULL;

		std::vector<Unit*> targetTable;
		TargetMap *targets = _unit->GetAIInterface()->GetAITargets();
		for (TargetMap::iterator itr = targets->begin(); itr != targets->end(); itr++)
		{
			Unit *temp = _unit->GetMapMgr()->GetUnit(itr->first);
			if (_unit->GetDistance2dSq(temp) <= dist)
			{
				if (((!tank && temp != _unit->GetAIInterface()->GetNextTarget()) || tank) && (!onlyplayer || (onlyplayer && temp->GetTypeId() == TYPEID_PLAYER)))
				{
					targetTable.push_back(temp);
				}
			}
		}
		if (targetTable.empty())
			return NULL;

		uint32 randt = RandomUInt(100)%targetTable.size();
		Unit * randomtarget = targetTable[randt];
		return randomtarget;
	}
protected:

	int nrspells;
	int spmin;
	int timer;
	int seconds;
	uint32 summon;
	bool mobs;
};

//ZUL’JIN

#define ZULJIN 23863

//Phase 1 troll
#define ZULJIN_GRIEVOUS_THROW 43093 
#define ZULJIN_WHIRLWIND 17207 // 44949 - work,43442,41400, 41399,41195, 41194,41098,  // from dbc, untested
#define ZULJIN_MODEL_TROLL 21899 
//Phase2 bear
#define ZULJIN_CREEPING_PARALYSIS 43095 
#define ZULJIN_OVERPOWER 43456
#define ZULJIN_MODEL_BEAR 21736
//phase3  eagle
#define ZULJIN_ENERGY_STORM 43983 //not Work
#define ZULJIN_MODEL_EAGLE 22257
#define ZULJIN_FEATHER_VORTEX 43112
#define ZULJIN_FEATHER_VORTEX_NPC 24136 //summons npc 4 begin phase
//phase4  lynx
#define ZULJIN_CLAW_RAGE  43150 
#define ZULJIN_LYNX_RUSH 43153
#define ZULJIN_MODEL_LYNX 21907
//phase5  dragonhawk
#define ZULJIN_FLAME_WHIRL 43213 //43208, 43213
#define ZULJIN_FLAME_BREATH 43215 
#define ZULJIN_PILLAR_OF_FIRE 43206
#define ZULJIN_MODEL_DRAGONHAWK 21901




class ZULJINAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ZULJINAI);
	SP_AI_Spell spells[10];
	bool m_spellcheck[10];

    ZULJINAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		
		spells[0].info = dbcSpell.LookupEntry(ZULJIN_GRIEVOUS_THROW);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = false;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(ZULJIN_WHIRLWIND); 
		spells[1].targettype = TARGET_SELF; 
		spells[1].instant = false;
		spells[1].perctrigger = 25.0f;
		spells[1].attackstoptimer = 1000;
		
		spells[2].info = dbcSpell.LookupEntry(ZULJIN_CREEPING_PARALYSIS); 
		spells[2].targettype = TARGET_VARIOUS;
		spells[2].instant = true;
		spells[2].perctrigger = 5.0f;
		spells[2].attackstoptimer = 1000;
		
		spells[3].info = dbcSpell.LookupEntry(ZULJIN_OVERPOWER);
		spells[3].targettype = TARGET_ATTACKING; 
		spells[3].instant = true;
		spells[3].perctrigger = 10.0f;
		spells[3].attackstoptimer = 1000;

		spells[4].info = dbcSpell.LookupEntry(ZULJIN_ENERGY_STORM); 
		spells[4].targettype = TARGET_RANDOM_DESTINATION;  
		spells[4].instant = true;
		spells[4].perctrigger = 10.0f;
		spells[4].attackstoptimer = 1000;
		
		spells[5].info = dbcSpell.LookupEntry(ZULJIN_CLAW_RAGE);
		spells[5].targettype = TARGET_RANDOM_SINGLE; 
		spells[5].instant = true;
		spells[5].perctrigger = 10.0f;
		spells[5].attackstoptimer = 1000;
		
		spells[6].info = dbcSpell.LookupEntry(ZULJIN_LYNX_RUSH);
		spells[6].targettype = TARGET_RANDOM_SINGLE; 
		spells[6].instant = true;
		spells[6].perctrigger = 10.0f;
		spells[6].attackstoptimer = 1000;
		
		spells[7].info = dbcSpell.LookupEntry(ZULJIN_FLAME_WHIRL); 
		spells[7].targettype = TARGET_VARIOUS;
		spells[7].instant = false;
		spells[7].cooldown = 12;
		spells[7].perctrigger = 10.0f;
		spells[7].attackstoptimer = 1000;

		spells[8].info = dbcSpell.LookupEntry(ZULJIN_FLAME_BREATH); 
		spells[8].targettype = TARGET_ATTACKING; 
		spells[8].instant = true;
		spells[8].perctrigger = 10.0f;
		spells[8].attackstoptimer = 1000;

		spells[9].info = dbcSpell.LookupEntry(ZULJIN_PILLAR_OF_FIRE);
		spells[9].targettype = TARGET_RANDOM_DESTINATION; 
		spells[9].instant = true;
		spells[9].perctrigger = 10.0f;
		spells[9].attackstoptimer = 1000;			

		nrspells = 2;
		spmin = 0;
		for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}

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
			uint32 RandomSpeach=RandomUInt(2);
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

		if (_unit->isAlive())
			_unit->Despawn(100, 2500);

        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , ZULJIN_MODEL_TROLL);  //change to Troll
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
			_unit->ClearHateList();
			_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , ZULJIN_MODEL_BEAR);  //change to Bear
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
				cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(ZULJIN_FEATHER_VORTEX, 
				      (_unit->GetPositionX() + RandomFloat(10)-10), (_unit->GetPositionY() + RandomFloat(10)-10),
				      _unit->GetPositionZ(), _unit->GetOrientation(),
				      true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0);

				if (cre != NULL) {
					cre->GetAIInterface()->setOutOfCombatRange(50000);
				}
			}
			_unit->ClearHateList();
			_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , ZULJIN_MODEL_EAGLE);  //change to Eagle
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Dere be no hidin' from da eagle!");
			_unit->PlaySoundToSet(12093);
		}
		if (_unit->GetHealthPct() <= 40 && phase == 3) 
		{
			spmin = 5;
			nrspells = 7;
			phase = 4;
			_unit->ClearHateList();
			_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , ZULJIN_MODEL_LYNX);  //change to Lynx
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Let me introduce to you my new bruddahs: fang and claw!");
			_unit->PlaySoundToSet(12094);
		}
		if (_unit->GetHealthPct() <= 20 && phase == 4) 
		{
			spmin = 7;
			nrspells = 10;
			phase = 5;
			_unit->ClearHateList();
			_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , ZULJIN_MODEL_DRAGONHAWK);  //change to Dragonhawk
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Ya don' have to look to da sky to see da dragonhawk!");
			_unit->PlaySoundToSet(12095);
		}
		if (_unit->GetHealthPct() <= 1 && phase == 5) 
		{
			_unit->PlaySoundToSet(12100);
			_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , ZULJIN_MODEL_TROLL);  //change to Troll
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
						case TARGET_RANDOM_DESTINATION:
							target = RandomTarget(false, true, 10000);
							if (target != NULL)
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
	Unit *RandomTarget(bool tank,bool onlyplayer, float dist)
	{
		if (_unit->GetAIInterface()->getAITargetsCount() == 0)
			return NULL;

		std::vector<Unit*> targetTable;
		TargetMap *targets = _unit->GetAIInterface()->GetAITargets();
		for (TargetMap::iterator itr = targets->begin(); itr != targets->end(); itr++)
		{
			Unit *temp = _unit->GetMapMgr()->GetUnit(itr->first);
			if (_unit->GetDistance2dSq(temp) <= dist)
			{
				if (((!tank && temp != _unit->GetAIInterface()->GetNextTarget()) || tank) && (!onlyplayer || (onlyplayer && temp->GetTypeId() == TYPEID_PLAYER)))
				{
					targetTable.push_back(temp);
				}
			}
		}
		if (targetTable.empty())
			return NULL;

		uint32 randt = RandomUInt(100)%targetTable.size();
		Unit * randomtarget = targetTable[randt];
		return randomtarget;
	}
protected:

	int nrspells;
	int spmin;
	int phase;
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
#define FEATHER_VORTEX 24136

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
	//Nalorakk
	mgr->register_creature_script(NALORAKK, &NALORAKKAI::Create);

	//Akilzon
	mgr->register_creature_script(AKILZON, &AKILZONAI::Create);
	mgr->register_creature_script(SOARING_EAGLE, &SOARINGEAGLEAI::Create);

	//Halazzi
	mgr->register_creature_script(HALAZZI, &HALAZZIAI::Create);
	mgr->register_creature_script(CORRUPTED_LIGHTING_TOTEM, &CLTOTEMAI::Create);
	mgr->register_creature_script(HALAZZILYNX, &LYNXSPIRITAI::Create);

	//Jan'alai
	mgr->register_creature_script(JANALAI, &JANALAIAI::Create);

	mgr->register_creature_script(HEX_LORD_MALACRASS, &HEXLORDMALACRASSAI::Create);
	mgr->register_creature_script(ZULJIN, &ZULJINAI::Create);
	//MOBS
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
