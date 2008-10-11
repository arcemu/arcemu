#include "StdAfx.h"
#include "Setup.h"

/************************************************************************/
/* Raid_Magtheridons_Lair.cpp Script									*/
/************************************************************************/

struct Coords
{
	float x;
	float y;
	float z;
	float o;
};

struct Coords channelerSpawns[]=
{
	{ -55.638000f, 1.869050f, 0.630946f, 0.000000f },
	{ -32.171600f, 39.926800f, 0.630921f, 4.940120f },
	{ 10.477100f, 24.445499f, 0.630891f, 3.894760f },
	{ 10.469200f, -19.894800f, 0.630910f, 2.555650f },
	{ -31.861300f, -35.919399f, 0.630945f, 1.314720f }
};

struct Coords cubeSpawns[]=
{
	{ 8.89169f, -19.392f, 0.630908f },
	{ -31.4977f, -34.0832f, 0.630935f },
	{ 9.39513f, 23.2389f, 0.630891f },
	{ -54.2302f, 2.32818f, 0.630945f },
	{ -31.7948f, 38.3964f, 0.630919f }
};

struct Coords Columns[]=
{
	{  17.7522f,  34.5464f,  0.144816f },
	{  19.0966f, -29.2772f,  0.133036f },
	{ -30.8852f,  46.6723f, -0.497104f },
	{ -60.2491f,  27.9361f, -0.018443f },
	{ -60.8202f, -21.9269f, -0.030260f },
	{ -29.7699f, -43.4445f, -0.522461f }
};

// Magtheridon
#define CN_MAGTHERIDON 17257
#define CN_HELLFIRE_CHANNELER 17256
#define MANTICRONCUBE 181713

#define SOUL_TRANSFER 30531
#define QUAKE 30571
#define BLAST_NOVA 30616
#define CLEAVE 37476
#define BANISH 30231
#define BANISH2 40825
#define DEBRIS 36449
#define DEBRISFALL 30632
#define DEBRISFALLEFFECT 30631
#define CAMERASHAKE 36455

class MagtheridonAI : public CreatureAIScript, public EventableObject
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(MagtheridonAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

	MagtheridonAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = true;
		} 
		spells[0].info = dbcSpell.LookupEntry(CLEAVE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 20;
		spells[0].perctrigger = 2.0f;
		spells[0].attackstoptimer = 3000;
		GameObject *cube;
		for (int i = 0; i < 5; i++)
		{
			cube = _unit->GetMapMgr()->GetInterface()->SpawnGameObject(MANTICRONCUBE, cubeSpawns[i].x, cubeSpawns[i].y, cubeSpawns[i].z, 0, true, 0, 0);
			if ( cube != NULL )
				cubes.push_back(cube);
		}
		Creature *channeler;
		for (int i = 0; i < 5; i++)
		{
			channeler = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_HELLFIRE_CHANNELER, channelerSpawns[i].x, channelerSpawns[i].y, channelerSpawns[i].z,  channelerSpawns[i].o, true, false, 0, 0);
			if ( channeler == NULL )
				continue;
			channeler->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, _unit->GetGUID());
			channeler->SetUInt32Value(UNIT_CHANNEL_SPELL, 30207);
			channelers.push_back(channeler);
			channelerAlive[i] = true;
		}

		_unit->AddAuraVisual(BANISH2, 1, false);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		door = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-69.500000f, 2.000000f, -0.000000f, 183847);
		m_phase = 1;
		enrage = 0;
		timer_enrage = 1320;
		_unit->DamageDoneModPCT[0] = 0;
		m_eventstarted = false;
		timer_banish = 120;
		RegisterAIUpdateEvent(1000);
	}
	void OnCombatStart(Unit* mTarget)
	{
		timer_blastNova = 0;
		timer_quake = 10;
		timer_debris = 0;
	}

	void OnCombatStop(Unit *mTarget)
	{
		m_eventstarted = false;
		sEventMgr.RemoveEvents(this);
		if (_unit->isAlive())
		{
			for (int i = 0; i < 5; i++)
			{
				channelers[i]->Despawn(100, 0);
				cubes[i]->Despawn(0);
			}
			_unit->Despawn(100, 2500);
			if (m_phase == 3)
			{
				GameObject* falling;
				for (int i = 0; i < 6; i++)
				{
					falling = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(Columns[i].x, Columns[i].y, Columns[i].z, 184634+i);
					if (falling)
						falling->SetUInt32Value(GAMEOBJECT_STATE, 1);
				}

				falling = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(0.0f, 0.0f, 0.0f, 184653);
				if (falling)
					falling->SetUInt32Value(GAMEOBJECT_STATE, 1);
			}
		}
		if ( door != NULL )
			door->SetUInt32Value(GAMEOBJECT_STATE, 0);

		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		sEventMgr.RemoveEvents(this);
		_unit->PlaySoundToSet(10258);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The Legion... will consume you... all....");
		if ( door != NULL )
			door->SetUInt32Value(GAMEOBJECT_STATE, 0);
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit* mTarget)
	{
		if (_unit->GetHealthPct() > 0)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Did you think me weak? Soft? Who is the weak one now?!");
			_unit->PlaySoundToSet(10255);
		}
	}

	void AIUpdate()
	{
		if (m_eventstarted)
		{
			if (channelerAlive[1] || channelerAlive[2] || channelerAlive[3] || channelerAlive[4] || channelerAlive[5])
			{
				for (int i = 0; i < 5; i++)
				{
					if (!channelers[i]->isAlive() && channelerAlive[i])
					{
						channelerAlive[i] = false;
						for (int x = 0; x < 5; x++)
						{
							if (channelers[x]->isAlive() && (channelers[i] != channelers[x]))
							{
								Aura *aura = new Aura();
								if (aura)
								{
									aura->Init(dbcSpell.LookupEntry(SOUL_TRANSFER), -1, channelers[x], channelers[x]);
								channelers[x]->AddAura(aura);
								}
							}
						}
					}
				}
			}
			if (!timer_enrage && !enrage)
			{
				_unit->DamageDoneModPCT[0] = 2;
				enrage = 1;
			}
			else
				timer_enrage--;
		}

		switch(m_phase)
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

	void PhaseOne()
	{
		if (m_eventstarted)
		{
			timer_banish--;
			if (!timer_banish)
			{
				Unit *target = GetPlayerCount();
				if (target)
				{
					ModifyAIUpdateEvent(1000);
					_unit->PlaySoundToSet(10253);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I... am... unleashed!");
					m_phase = 2;
					_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
					_unit->RemoveAuraVisual(BANISH2, 1);
					_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
					_unit->GetAIInterface()->AttackReaction(target, 1, 0);
				}
				else
					EventStop(NULL);
			}
			else if (timer_banish == 60)
				_unit->SendChatMessage(CHAT_MSG_MONSTER_EMOTE, LANG_UNIVERSAL, " is nearly free of his bounds!");
		}
		else
		{
			if (!timer_speech)
			{
				int val = RandomUInt(100)%20;
				if (!val)
					RandomSpeech();
			}
			else
				timer_speech--;
		}
	}
	void PhaseTwo()
	{
		if(_unit->GetHealthPct() <= 30)
		{
			_unit->PlaySoundToSet(10257);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I will not be taken so easily. Let the walls of this prison tremble... and FALL!!!");
			_unit->CastSpell(_unit, CAMERASHAKE, true);
			_unit->GetAIInterface()->StopMovement(5000);
			_unit->setAttackTimer(5000, false);
			//sEventMgr.AddEvent(this, &MagtheridonAI::Debris, EVENT_SCRIPT_UPDATE_EVENT, 4000, 1, 0); //Disabled for now as AOE still hits two times --> 11k dmg -.-
			m_phase = 3;
			GameObject* falling;
			for (int i = 0; i < 6; i++)
			{
				falling = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(Columns[i].x, Columns[i].y, Columns[i].z, 184634+i);
				if (falling)
					falling->SetUInt32Value(GAMEOBJECT_STATE, 0);
			}

			falling = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(0.0f, 0.0f, 0.0f, 184653);
			if (falling)
				falling->SetUInt32Value(GAMEOBJECT_STATE, 0);
		}
		else
		{
			if (!quake)
			{
				timer_quake++;
				timer_blastNova++;
				float val = (float)RandomFloat(100.0f);
				SpellCast(val);

				if(timer_blastNova > 54)
				{
					_unit->SendChatMessage(CHAT_MSG_MONSTER_EMOTE, LANG_UNIVERSAL, " begins to cast Blast Nova!");
					_unit->GetAIInterface()->StopMovement(12000);
					_unit->setAttackTimer(12000, false);
					_unit->CastSpell(_unit, BLAST_NOVA, false);
					timer_blastNova = 0;

				}
				else if (timer_quake > 50)
				{
					_unit->setAttackTimer(9000, false);
					_unit->GetAIInterface()->StopMovement(9000);
					_unit->CastSpell(_unit, QUAKE, true);
					timer_quake = 0;
					quake = 1;
				}
			}
			else
			{
				if (quake <= 6)
				{
					_unit->CastSpell(_unit, QUAKE, true);
					quake++;
				}
				else
				{
					quake = 0;
				}
			}
		}
	}
	void PhaseThree()
	{
		if (!quake)
		{
			timer_quake++;
			timer_blastNova++;
			timer_debris++;
			float val = (float)RandomFloat(100.0f);
			SpellCast(val);

			if(timer_blastNova > 56)
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_EMOTE, LANG_UNIVERSAL, " begins to cast Blast Nova!");
				_unit->GetAIInterface()->StopMovement(12000);
				_unit->setAttackTimer(12000, false);
				_unit->CastSpell(_unit, BLAST_NOVA, false);
				timer_blastNova = 0;

			}
			else if (timer_quake > 50)
			{
				_unit->setAttackTimer(8000, false);
				_unit->GetAIInterface()->StopMovement(8000);
				_unit->CastSpell(_unit, QUAKE, true);
				timer_quake = 0;
				quake = 1;
			}
			else if (timer_debris > 60)
			{
				Unit *target = RandomTarget(false, true, 6400);
				if (target)
				{
					_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), dbcSpell.LookupEntry(DEBRISFALL), true);
					sEventMgr.AddEvent(this, &MagtheridonAI::DebrisFallEffect, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), EVENT_SCRIPT_UPDATE_EVENT, 4000, 1, 0);
				}
				timer_debris = 0;
			}
		}
		else
		{
			if (quake <= 6)
			{
				_unit->CastSpell(_unit, QUAKE, true);
				quake++;
			}
			else
			{
				quake = 0;
			}
		}
	}

	void RandomSpeech()
	{
		timer_speech = 120;
		int val = RandomUInt(100)%6;
		switch (val)
		{
		case 0:
			_unit->PlaySoundToSet(10247);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Wretched, meddling insects! Release me, and perhaps I will grant you a merciful death!");
			break;
		case 1:
			_unit->PlaySoundToSet(10248);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Vermin! Leeches! Take my blood and choke on it!");
			break;
		case 2:
			_unit->PlaySoundToSet(10249);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Illidan is an arrogant fool! I will crush him and reclaim Outland as my own!");
			break;
		case 3:
			_unit->PlaySoundToSet(10250);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Away, you mindless parasites! My blood is my own!");
			break;
		case 4:
			_unit->PlaySoundToSet(10251);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "How long do you believe your pathetic sorcery can hold me?");
			break;
		case 5:
			_unit->PlaySoundToSet(10252);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "My blood will be the end of you!");
			break;
		}
	}

	void EventStart(Unit* mTarget)
	{
		if (!m_eventstarted)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_EMOTE, LANG_UNIVERSAL, "'s bonds begin to weaken!");
			m_eventstarted = true;
			if (door == NULL)
				door = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-69.500000f, 2.000000f, -0.000000f, 183847);
			if (door != NULL)
				door->SetUInt32Value(GAMEOBJECT_STATE, 1);
			for (int i = 0; i < 5; i++)
			{
				if (channelers[i] != NULL)
				{
					if (!channelers[i]->GetAIInterface()->GetAllowedToEnterCombat())
						channelers[i]->GetAIInterface()->SetAllowedToEnterCombat(true);
					channelers[i]->GetAIInterface()->AttackReaction(mTarget, 1, 0);
				}
			}
		}
	}
	void EventStop(Unit* mTarget)
	{
		if (m_eventstarted)
		{
			m_eventstarted = false;
			if (_unit->isAlive())
			{
				for (int i = 0; i < 5; i++)
				{
					if (channelers[i] != NULL)
						channelers[i]->Despawn(100, 0);
					if (cubes[i] != NULL)
						cubes[i]->Despawn(0);
				}
				if (door != NULL)
					door->SetUInt32Value(GAMEOBJECT_STATE, 0);
				RemoveAIUpdateEvent();
				_unit->Despawn(100, 2500);
			}
		}
	}
	void Debris()
	{
		_unit->CastSpell(_unit, DEBRIS, true);
	}

	void DebrisFallEffect(float x, float y, float z)
	{
		_unit->CastSpellAoF(x, y, z, dbcSpell.LookupEntry(DEBRISFALLEFFECT), true);
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
		if (targetTable.empty())
			return NULL;

		uint32 randt = RandomUInt(100)%targetTable.size();
		Unit * randomtarget = targetTable[randt];
		return randomtarget;
	}

	Unit *GetPlayerCount()
	{
		if (_unit->GetInRangePlayersCount() == 0)
			return NULL;

		std::vector<Unit*> targetTable;
		TargetMap::iterator itr;
		for (set<Player*>::iterator itr = _unit->GetInRangePlayerSetBegin(); itr != _unit->GetInRangePlayerSetEnd(); itr++)
		{
			Unit *temp = (Unit*)(*itr);
			if (temp->isAlive())
				targetTable.push_back(temp);
		}
		if (targetTable.empty())
			return NULL;

		uint32 randt = RandomUInt(100)%targetTable.size();
		Unit * randomtarget = targetTable[randt];
		return randomtarget;
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
	int m_phase;
	int nrspells;
	int timer_blastNova;
	int timer_speech;
	int timer_enrage;
	int timer_quake;
	int timer_banish;
	int timer_debris;
	int quake;
	int enrage;

	std::vector<Creature*> channelers;
	std::vector<GameObject*> cubes;
	bool channelerAlive[5];

	bool m_eventstarted;

	GameObject *door;
};

// Hellfire Warder
#define CN_HELLFIRE_WARDER 18829

#define SHADOW_BOLT_VOLLEY	39175
#define DEATH_COIL			41070
#define RAIN_OF_FIRE		37465

class HellfireWarderAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(HellfireWarderAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

	HellfireWarderAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		} 

		spells[0].info = dbcSpell.LookupEntry(SHADOW_BOLT_VOLLEY);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 9.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(DEATH_COIL);
		spells[1].targettype = TARGET_ATTACKING;	
		spells[1].instant = true;
		spells[1].perctrigger = 8.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(RAIN_OF_FIRE);
		spells[2].targettype = TARGET_RANDOM_DESTINATION; // Needs checking
		spells[2].instant = false;
		spells[2].perctrigger = 5.0f;
		spells[2].attackstoptimer = 1000;

		_unit->SetStandState(8);

	}
	void OnCombatStart(Unit* mTarget)
	{
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
						target = _unit->GetAIInterface()->GetNextTarget();
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					case TARGET_RANDOM_DESTINATION:
						target = RandomTarget(true, true, spells[i].info->base_range_or_radius_sqr);
						if (target)
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
		if (targetTable.empty())
			return NULL;

		uint32 randt = RandomUInt(100)%targetTable.size();
		Unit * randomtarget = targetTable[randt];
		return randomtarget;
	}

protected:

	int nrspells;
};

// Hellfire Channeler
#define CN_HELLFIRE_CHANNELER 17256

#define SHADOW_BOLT_VOLLEY 39175
#define DARK_MENDING 30528
#define BURNING_ABYSSAL 30511

class HellfireChannelerAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(HellfireChannelerAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

	HellfireChannelerAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		} 

		spells[0].info = dbcSpell.LookupEntry(SHADOW_BOLT_VOLLEY);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].cooldown = 10;
		spells[0].perctrigger = 1.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(DARK_MENDING);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = false;
		spells[1].cooldown = 20;
		spells[1].perctrigger = 3.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(BURNING_ABYSSAL);
		spells[2].targettype = TARGET_RANDOM_SINGLE;
		spells[2].instant = true;
		spells[2].cooldown = 20;
		spells[2].perctrigger = 1.0f;
		spells[2].attackstoptimer = 1000;
		spells[2].cooldown = 30;

		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		magtheridon = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-22.657900f, 2.159050f, -0.345542f, 17257);
	}
	void OnCombatStart(Unit* mTarget)
	{
		_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
		_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);
		RegisterAIUpdateEvent(1000);
		if (magtheridon)
		{
			CreatureAIScript *mob_script = magtheridon->GetScript();
			((MagtheridonAI*)mob_script)->EventStart(mTarget);
		}
	}

	void OnCombatStop(Unit *mTarget)
	{
		if (_unit->isAlive() && magtheridon->isAlive() && !magtheridon->CombatStatus.IsInCombat())
		{
			CreatureAIScript *mob_script = magtheridon->GetScript();
			((MagtheridonAI*)mob_script)->EventStop(mTarget);
		}
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

	void OnDamageTaken(Unit* mAttacker, float fAmount)
	{
		if (!_unit->GetAIInterface()->GetAllowedToEnterCombat())
		{
			_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
			_unit->GetAIInterface()->AttackReaction(mAttacker, (uint32)fAmount, 0);
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
		if (targetTable.empty())
			return NULL;

		uint32 randt = RandomUInt(100)%targetTable.size();
		Unit * randomtarget = targetTable[randt];
		return randomtarget;
	}

protected:
	int nrspells;
	Creature *magtheridon;
};

// Burning Abyssal AI

#define CN_BURNINGABYSSAL 17454

#define FIRE_BLAST 37110

class BurningAbyssalAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(BurningAbyssalAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

	BurningAbyssalAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		} 

		spells[0].info = dbcSpell.LookupEntry(FIRE_BLAST);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;

	}

	void OnCombatStart(Unit* mTarget)
	{
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
		_unit->Despawn(100, 0);
	}

	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
		_unit->Despawn(100, 0);
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
						target = _unit->GetAIInterface()->GetNextTarget();
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					case TARGET_RANDOM_SINGLE:
						target = RandomTarget(true, true, spells[i].info->base_range_or_radius_sqr);
						if (target)
						{
							_unit->CastSpell(target, spells[i].info, spells[i].instant);
						}
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
		if (targetTable.empty())
			return NULL;

		uint32 randt = RandomUInt(100)%targetTable.size();
		Unit * randomtarget = targetTable[randt];
		return randomtarget;
	}

protected:

	int nrspells;
};

//Cube
#define MANTICRONCUBE 181713
#define CN_CUBETRIGGER 17376

#define MINDEXHAUSTION 44032
#define SHADOWGRASP 30166
#define SHADOWGRASP2 30410
class ManticronCubeAI : public GameObjectAIScript, public EventableObject
{
public:
	ManticronCubeAI(GameObject* pGameObject) : GameObjectAIScript(pGameObject)
	{
		magtheridon = NULL;
		myTrigger = NULL;
		Channeler = NULL;
		sEventMgr.AddEvent(this, &ManticronCubeAI::FindMagtheridon, EVENT_SCRIPT_UPDATE_EVENT, 5000, 1, 0);
	}

	void OnActivate(Player *pPlayer)
	{
		Aura *aura = pPlayer->FindAura(MINDEXHAUSTION);
		if (aura)
			return;
		if (myTrigger == NULL)
			myTrigger = _gameobject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_gameobject->GetPositionX(), _gameobject->GetPositionY(), _gameobject->GetPositionZ(), CN_CUBETRIGGER);
		if (magtheridon == NULL || !magtheridon->isAlive() || !magtheridon->GetAIInterface()->GetNextTarget())
			return;
		if (Channeler != NULL || myTrigger == NULL)
			return;

		aura = new Aura();
		if ( aura == NULL )
			return;

		aura->Init(dbcSpell.LookupEntry(MINDEXHAUSTION), 90000, magtheridon, pPlayer);

		pPlayer->AddAura(aura);
		pPlayer->CastSpell(pPlayer, SHADOWGRASP2, false);
		myTrigger->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, magtheridon->GetGUID());
		myTrigger->SetUInt32Value(UNIT_CHANNEL_SPELL, SHADOWGRASP);
		Channeler = pPlayer;

		uint32 Counter = 0;
		for (int i = 0; i < 5; i++)
		{
			Creature *cubeTrigger = _gameobject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(cubeSpawns[i].x, cubeSpawns[i].y, cubeSpawns[i].z, 17376);
			if (cubeTrigger != NULL && (cubeTrigger->GetUInt32Value(UNIT_CHANNEL_SPELL) == SHADOWGRASP))
				Counter++;
		}

		if (Counter >= 5)
		{
			//TODO: ADD DMG BOOST
			magtheridon->PlaySoundToSet(10256);
			magtheridon->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Not again! Not again...");
			magtheridon->GetAIInterface()->disable_melee = true;
			magtheridon->GetAIInterface()->StopMovement(0);
			magtheridon->AddAuraVisual(BANISH, 1, false);
			if (magtheridon->GetCurrentSpell())
				magtheridon->GetCurrentSpell()->cancel();
			Aura *auramag;
			auramag = magtheridon->FindAura(30616);
			if (auramag)
				auramag->Remove();
			sEventMgr.AddEvent(this, &ManticronCubeAI::MagtheridonUnbanish, EVENT_SCRIPT_UPDATE_EVENT, 5000, 1, 0);
		}

		RegisterAIUpdateEvent(1000);
	}

	void FindMagtheridon()
	{
		magtheridon = magtheridon = _gameobject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-22.6579f, 2.15905f, -0.345542f, 17257);
	}

	void MagtheridonUnbanish()
	{
		//TODO: REMOVE DMG BOOST
		magtheridon->RemoveAuraVisual(BANISH, 1);
		magtheridon->GetAIInterface()->disable_melee = false;
	}

	void AIUpdate()
	{
		if (!Channeler)
			return;
		Aura* aura = Channeler->FindAura(SHADOWGRASP2);
		if (!Channeler->IsInWorld() || !Channeler->isAlive() || !aura || !magtheridon->isAlive())
		{
			myTrigger->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
			myTrigger->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);
			Channeler = NULL;
		}
	}

	static GameObjectAIScript *Create(GameObject * GO) { return new ManticronCubeAI(GO); }

protected:
	Player* Channeler;
	Unit *magtheridon;
	Unit *myTrigger;
};


//Cube Channeling Effect
#define CN_CUBETRIGGER 17376

class CubeTriggerAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(CubeTriggerAI);

	CubeTriggerAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
	}
};

void SetupMagtheridonsLair(ScriptMgr * mgr)
{
	mgr->register_creature_script(CN_HELLFIRE_WARDER, &HellfireWarderAI::Create);
	mgr->register_creature_script(CN_BURNINGABYSSAL, &BurningAbyssalAI::Create);

	mgr->register_creature_script(CN_MAGTHERIDON, &MagtheridonAI::Create);
	mgr->register_creature_script(CN_HELLFIRE_CHANNELER, &HellfireChannelerAI::Create);

	mgr->register_gameobject_script(MANTICRONCUBE, &ManticronCubeAI::Create);
	mgr->register_creature_script(CN_CUBETRIGGER, CubeTriggerAI::Create);
}
