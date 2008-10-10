#include "StdAfx.h"
#include "Setup.h"

/************************************************************************/
/* Raid_GruulsLair.cpp Script											*/
/************************************************************************/
// Lair BruteAI
#define CN_LAIR_BRUTE 19389

#define MORTALSTRIKE	35054
#define CLEAVE			31345
#define CHARGE			35754

class LairBruteAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(LairBruteAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

	LairBruteAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		} 

		spells[0].info = dbcSpell.LookupEntry(MORTALSTRIKE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 9.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(CLEAVE);
		spells[1].targettype = TARGET_ATTACKING;	
		spells[1].instant = true;
		spells[1].perctrigger = 8.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(CHARGE);
		spells[2].targettype = TARGET_ATTACKING; // Needs checking
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

// Gronn PriestAI
#define CN_GRONN_PRIEST 21350

#define PSYCHICSCREAM	34322       //33130 - death coil
#define RENEW			36679
#define HEAL_GRONN_PRIEST 36678		// it's instead of PRAYER_OF_HEALING
//#define PRAYER_OF_HEALING	33152	// couldn't find infos about it so disabled

class GronnPriestAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(GronnPriestAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

	GronnPriestAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		} 

		spells[0].info = dbcSpell.LookupEntry(PSYCHICSCREAM);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(RENEW);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].perctrigger = 6.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(HEAL_GRONN_PRIEST);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = false;
		spells[2].perctrigger = 5.0f;
		spells[2].attackstoptimer = 5000;

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

/*****************************/
/*                           */
/*         Boss AIs          */
/*                           */
/*****************************/

// MaulgarAI
#define CN_HIGH_KING_MAULAGR 18831
#define CN_OLM_THE_SUMMONER 18834
#define CN_BLINDEYE_THE_SEER 18836
#define CN_KIGGLER_THE_CRAZED 18835
#define CN_KROSH_FIREHAND 18832

#define ARCINGSMASH		38761
#define MIGHTYBLOW		33230
#define WHIRLWIND		33239
#define ENRAGE			34970 // no idea about id, can be also: 34970, 37023 and others as nowhere is told which Enrage it should be

class HighKingMaulgarAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(HighKingMaulgarAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

	HighKingMaulgarAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		} 

		spells[0].info = dbcSpell.LookupEntry(ARCINGSMASH);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(MIGHTYBLOW);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 3.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(WHIRLWIND);
		spells[2].targettype = TARGET_VARIOUS; // Should attk party member with second the highest aggro in melee range
		spells[2].instant = true;
		spells[2].perctrigger = 3.0f;
		spells[2].attackstoptimer = 1000;

		summoner = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_OLM_THE_SUMMONER, 150.858536f, 185.975540f, -11.091244f, 3.3f, true, false, 0, 0);
		mage = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_KROSH_FIREHAND, 148.062469f, 179.211060f, -10.799615f, 3.3f, true, false, 0, 0);
		priest = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BLINDEYE_THE_SEER, 149.994278f, 203.461243f, -9.410825f, 3.3f, true, false, 0, 0);
		shaman = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_KIGGLER_THE_CRAZED, 147.571167f, 209.742279f, -7.748757f, 3.3f, true, false, 0, 0);

		m_eventstarted = false;
		enraged = false;
	}
	void OnCombatStart(Unit* mTarget)
	{
		EventStart(mTarget);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Gronn are the real power in Outland!");
		_unit->PlaySoundToSet(11367);
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		m_eventstarted = false;
		if (_unit->isAlive())
		{
			if (summoner != NULL)
				summoner->Despawn(100, 0);
			if (mage != NULL)
				mage->Despawn(100, 0);
			if (priest != NULL)
				priest->Despawn(100, 0);
			if (shaman != NULL)
				shaman->Despawn(100, 0);
			_unit->Despawn(100, 2500);
		}
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Gruul will... crush you!");
		_unit->PlaySoundToSet(11376);
		RemoveAIUpdateEvent();

		GameObject * pDoor = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(95.26f, 251.836f, 0.47f, 183817);
		if(pDoor)
			pDoor->SetUInt32Value(GAMEOBJECT_STATE, 0);
	}

	void OnTargetDied(Unit* mTarget)
	{
		if (_unit->GetHealthPct() > 0)	// Hack to prevent double yelling (OnDied and OnTargetDied when creature is dying)
		{
			int RandomSpeach = RandomUInt(1000)%3;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You're not so tough after all!");
				_unit->PlaySoundToSet(11373);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Whahaha!"); // more accurate?
				_unit->PlaySoundToSet(11374);
				break;
			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Maulgar is king!");
				_unit->PlaySoundToSet(11375);
				break;
			}
		}
	}

	void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
		if (!enraged && _unit->GetHealthPct() <= 50)
		{
			enraged = true;
			_unit->CastSpell(_unit, ENRAGE, true);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You will not defeat the hand of Gruul!");
			_unit->PlaySoundToSet(11368);
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
	void EventStart(Unit* mTarget)
	{
		if (!m_eventstarted)
		{
			m_eventstarted = true;
			_unit->GetAIInterface()->AttackReaction(mTarget, 1, 0);
			if (summoner != NULL)
				summoner->GetAIInterface()->AttackReaction(mTarget, 1, 0);
			if (mage != NULL)
				mage->GetAIInterface()->AttackReaction(mTarget, 1, 0);
			if (priest != NULL)
				priest->GetAIInterface()->AttackReaction(mTarget, 1, 0);
			if (shaman != NULL)
				shaman->GetAIInterface()->AttackReaction(mTarget, 1, 0);
		}
	}
	void EventStop(Unit* mTarget)
	{
		if (m_eventstarted)
		{
			m_eventstarted = false;
			if (_unit->isAlive())
			{
				if (summoner != NULL)
					summoner->Despawn(100, 0);
				if (mage != NULL)
					mage->Despawn(100, 0);
				if (priest != NULL)
					priest->Despawn(100, 0);
				if (shaman != NULL)
					shaman->Despawn(100, 0);
				RemoveAIUpdateEvent();
				_unit->Despawn(100, 2500);
			}
		}
	}

protected:
	int nrspells;
	bool m_eventstarted;
	bool enraged;
	Creature *summoner;
	Creature *mage;
	Creature *priest;
	Creature *shaman;
};

// Kiggler The CrazedAI
#define CN_KIGGLER_THE_CRAZED 18835

#define LIGHTNING_BOLT	36152
#define GREATER_POLYMORPH 33173
#define ARCANE_EXPLOSION 33237
#define	ARCANE_SHOCK	33175
//#define HEX				36700
//#define ARCANE_BLAST		31457	 //have none 10yd SPELL

class KigglerTheCrazedAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(KigglerTheCrazedAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

	KigglerTheCrazedAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		} 

		spells[0].info = dbcSpell.LookupEntry(LIGHTNING_BOLT);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 3.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(GREATER_POLYMORPH);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 2.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(ARCANE_EXPLOSION);
		spells[2].targettype = TARGET_VARIOUS;
		spells[2].instant = true;
		spells[2].perctrigger = 4.0f;
		spells[2].attackstoptimer = 1000; 


		spells[3].info = dbcSpell.LookupEntry(ARCANE_SHOCK);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = true;
		spells[3].perctrigger = 3.0f;
		spells[3].attackstoptimer = 1000;

		maulgar = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(148.645508f, 194.516510f, -10.934792f, CN_HIGH_KING_MAULAGR);

	}
	void OnCombatStart(Unit* mTarget)
	{
		if (maulgar)
		{
			CreatureAIScript *mob_script = maulgar->GetScript();
			((HighKingMaulgarAI*)mob_script)->EventStart(mTarget);
		}
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		if (_unit->isAlive() && maulgar && maulgar->isAlive())
		{
			CreatureAIScript *mob_script = maulgar->GetScript();
			((HighKingMaulgarAI*)mob_script)->EventStop(mTarget);
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
		/*
		if(_unit->GetDistanceSq(target)<20)  <--- why do you need this??
		_unit->CastSpell(_unit, spells[2].info, spells[2].instant);
		*/
		/*		
		if(m_spellcheck[1] == true) //<^-------- Both functions crash server in this place
		_unit->GetAIInterface()->GetNextTarget();  
		*/
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

	Unit *target;
	Creature *maulgar;
	int nrspells;
};
// Blindeye The SeerAI
#define CN_BLINDEYE_THE_SEER 18836

#define PRAYER_OF_HEALING 33152
#define GREAT_POWER_WORD_SHIELD	33147
#define HEAL 33144

class BlindeyeTheSeerAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(BlindeyeTheSeerAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

	BlindeyeTheSeerAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		} 

		spells[0].info = dbcSpell.LookupEntry(PRAYER_OF_HEALING);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 2.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(GREAT_POWER_WORD_SHIELD);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].perctrigger = 4.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(HEAL);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = false;
		spells[2].perctrigger = 3.0f;
		spells[2].attackstoptimer = 1000;

		maulgar = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(148.645508f, 194.516510f, -10.934792f, CN_HIGH_KING_MAULAGR);

	}
	void OnCombatStart(Unit* mTarget)
	{
		if (maulgar)
		{
			CreatureAIScript *mob_script = maulgar->GetScript();
			((HighKingMaulgarAI*)mob_script)->EventStart(mTarget);
		}
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		if (_unit->isAlive() && maulgar && maulgar->isAlive())
		{
			CreatureAIScript *mob_script = maulgar->GetScript();
			((HighKingMaulgarAI*)mob_script)->EventStop(mTarget);
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
	Creature *maulgar;
};
// Olm The SummonerAI
#define CN_OLM_THE_SUMMONER 18834

#define DEATH_COIL		33130 // ?
#define SUMMON_WILD_FEL_STALKER	33131 // Need to add AI for it to make it blizzlike anyway (it's wild felhunter and should be fel stalker?)
#define DARK_DECAY		33129
#define HEAL_OLM		31730	// not sure if this should be that healing spell

class OlmTheSummonerAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(OlmTheSummonerAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

	OlmTheSummonerAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		} 

		spells[0].info = dbcSpell.LookupEntry(DEATH_COIL);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 2.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(SUMMON_WILD_FEL_STALKER);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = false;
		spells[1].perctrigger = 2.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(DARK_DECAY);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].perctrigger = 3.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(HEAL_OLM);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = false;
		spells[3].perctrigger = 5.0f;
		spells[3].attackstoptimer = 1000;

		maulgar = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(148.645508f, 194.516510f, -10.934792f, CN_HIGH_KING_MAULAGR);

	}
	void OnCombatStart(Unit* mTarget)
	{
		if (maulgar)
		{
			CreatureAIScript *mob_script = maulgar->GetScript();
			((HighKingMaulgarAI*)mob_script)->EventStart(mTarget);
		}
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		if (_unit->isAlive() && maulgar && maulgar->isAlive())
		{
			CreatureAIScript *mob_script = maulgar->GetScript();
			((HighKingMaulgarAI*)mob_script)->EventStop(mTarget);
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
	Creature *maulgar;
};
// Krosh FirehandAI
#define CN_KROSH_FIREHAND 18832

#define GREAT_FIREBALL	33051
#define BLAST_WAVE		33061
#define FIRE_WARD		37844
#define SPELLSHIELD 33054

class KroshFirehandAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(KroshFirehandAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

	KroshFirehandAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		} 

		spells[0].info = dbcSpell.LookupEntry(GREAT_FIREBALL);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(BLAST_WAVE);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].perctrigger = 2.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(SPELLSHIELD);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;

		maulgar = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(148.645508f, 194.516510f, -10.934792f, CN_HIGH_KING_MAULAGR);
		FireWardCooldown = 1;

	}
	void OnCombatStart(Unit* mTarget)
	{
		if (maulgar)
		{
			CreatureAIScript *mob_script = maulgar->GetScript();
			((HighKingMaulgarAI*)mob_script)->EventStart(mTarget);
		}
		_unit->GetAIInterface()->setCurrentAgent(AGENT_SPELL);
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		if (_unit->isAlive() && maulgar && maulgar->isAlive())
		{
			CreatureAIScript *mob_script = maulgar->GetScript();
			((HighKingMaulgarAI*)mob_script)->EventStop(mTarget);
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
		FireWardCooldown--;
		if(!FireWardCooldown)
		{
			//_unit->CastSpell(_unit, SPELLSHIELD, true);
			FireWardCooldown=30;
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
			target = NULL;
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

	Unit *target;
	uint32 FireWardCooldown;
	int nrspells;
	Creature *maulgar;
};

// GruulsAI

#define CN_GRUUL_THE_DRAGONKILLER 19044

#define GROWTH				36300
#define CAVE_IN				36240
#define GROUND_SLAM			33525
#define SHATTER				33671
#define HURTFUL_STRIKE		33813
#define REVERBERATION		36297
#define STONED				33652
#define GRONN_LORDS_GRASP	33572

class GruulsTheDragonkillerAI : public CreatureAIScript, public EventableObject
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(GruulsTheDragonkillerAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

	GruulsTheDragonkillerAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		GrowthCooldown = 30;
		groundSlamcd = 45;
		hurtfulStrikecd = 20 + RandomUInt(16);
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		} 

		spells[0].info = dbcSpell.LookupEntry(CAVE_IN);
		spells[0].targettype = TARGET_RANDOM_DESTINATION;
		spells[0].instant = true;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(REVERBERATION);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].cooldown = 30;
		spells[1].instant = true;
		spells[1].perctrigger = 50.0f;
		spells[1].attackstoptimer = 1000;

	}

	void OnCombatStart(Unit* mTarget)
	{
		CastTime();
		GrowthCooldown = 30;
		groundSlamcd = 45;
		hurtfulStrikecd = 20 + RandomUInt(16);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Come and die.");
		_unit->PlaySoundToSet(11355);
		RegisterAIUpdateEvent(1000);

		GameObject * pDoor = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(166.897f, 368.226f, 16.9209f, 184662);
		if(pDoor)
			pDoor->SetUInt32Value(GAMEOBJECT_STATE, 1);
	}

	void OnTargetDied(Unit* mTarget)
	{
		if (_unit->GetHealthPct() > 0)	// Hack to prevent double yelling (OnDied and OnTargetDied when creature is dying)
		{
			int RandomSpeach = RandomUInt(1000)%3;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "No more.");
				_unit->PlaySoundToSet(11360);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Unworthy.");
				_unit->PlaySoundToSet(11361);
				break;
			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Die.");
				_unit->PlaySoundToSet(11362);
				break;
			}
		}
	}

	void OnCombatStop(Unit *mTarget)
	{
		sEventMgr.RemoveEvents(this);
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		GrowthCooldown = 30;
		RemoveAIUpdateEvent();
		_unit->RemoveAllAuras();
		GameObject * pDoor = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(166.897f, 368.226f, 16.9209f, 184662);
		if(pDoor)
			pDoor->SetUInt32Value(GAMEOBJECT_STATE, 0);
	}

	void OnDied(Unit * mKiller)
	{
		sEventMgr.RemoveEvents(this);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Wraaaa!");
		_unit->PlaySoundToSet(11363);
		RemoveAIUpdateEvent();
		GameObject * pDoor = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(166.897f, 368.226f, 16.9209f, 184662);
		if(pDoor)
			pDoor->SetUInt32Value(GAMEOBJECT_STATE, 0);
		_unit->RemoveAllAuras();
		RemoveAIUpdateEvent();
	}

	void AIUpdate()
	{
		groundSlamcd--;
		GrowthCooldown--;
		hurtfulStrikecd--;
		if(!GrowthCooldown)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_EMOTE, LANG_UNIVERSAL, " grows in size!");
			_unit->CastSpell(_unit, GROWTH, true);
			GrowthCooldown=30;
		}
		if ( groundSlamcd == 0 )
		{
			switch (RandomUInt(1000)%2)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL,"Scurry!");
				_unit->PlaySoundToSet(11357);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL,"No escape!");
				_unit->PlaySoundToSet(11356);
				break;
			}
			_unit->setAttackTimer(12000, false);
			_unit->GetAIInterface()->StopMovement(12000);
			sEventMgr.RemoveEvents(this);
			groundSlam();
			sEventMgr.AddEvent(this, &GruulsTheDragonkillerAI::stoned, EVENT_SCRIPT_UPDATE_EVENT, 7000, 1, 0);
			sEventMgr.AddEvent(this, &GruulsTheDragonkillerAI::shatter, EVENT_SCRIPT_UPDATE_EVENT, 9000, 1, 0);
			groundSlamcd = 80;
		}
		if (!hurtfulStrikecd)
		{
			hurtfulStrike();
			hurtfulStrikecd = 20 + RandomUInt(16);
		}
		else
		{
			float val = (float)RandomFloat(100.0f);
			SpellCast(val);
		}
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

	void groundSlam()
	{
		if (_unit->GetAIInterface()->getAITargetsCount() == 0)
			return;

		TargetMap *targets = _unit->GetAIInterface()->GetAITargets();
		for (TargetMap::iterator itr = targets->begin(); itr != targets->end(); itr++)
		{
			Unit *temp = _unit->GetMapMgr()->GetUnit(itr->first);
			if (temp->GetTypeId() == TYPEID_PLAYER && temp->isAlive())
			{
				knockback(temp);
				temp->CastSpell(temp, GROUND_SLAM, true);
			}
		}
	}

	void knockback(Unit *target)
	{
		if (target->GetTypeId() != TYPEID_PLAYER)
			return;

		float ori = (float)RandomFloat(6.282f);
		float dx,dy;
		float affect = 18 + RandomFloat(4.0f);

		dx = sinf(ori);
		dy = cosf(ori);

		WorldPacket data(SMSG_MOVE_KNOCK_BACK, 50);
		data << target->GetNewGUID();
		data << getMSTime();
		data << dy << dx;
		data << affect;
		data << -affect;
		static_cast<Player*>(target)->blinked = true;
		static_cast<Player*>(target)->GetSession()->SendPacket(&data);
	}

	void stoned()
	{
		if (_unit->GetAIInterface()->getAITargetsCount() == 0)
			return;

		TargetMap *targets = _unit->GetAIInterface()->GetAITargets();
		for (TargetMap::iterator itr = targets->begin(); itr != targets->end(); itr++)
		{
			Unit *temp = _unit->GetMapMgr()->GetUnit(itr->first);
			if ( temp != NULL && temp->GetTypeId() == TYPEID_PLAYER && temp->isAlive())
				temp->CastSpell(temp, STONED, true);
		}
	}

	void shatter()
	{

		TargetMap *targets = _unit->GetAIInterface()->GetAITargets();
		if ( targets == NULL )
			return;

		for ( TargetMap::iterator itr = targets->begin(); itr != targets->end(); itr++ )
		{
			if ( _unit->GetMapMgr()->GetUnit(itr->first) == NULL || _unit->GetMapMgr()->GetUnit(itr->first)->GetTypeId() != TYPEID_PLAYER || _unit->GetMapMgr()->GetUnit(itr->first)->isDead() )
				continue;

			Player* _plr = (Player*)(_unit->GetMapMgr()->GetUnit(itr->first));

			for(set<Player*>::iterator itr2 = _plr->GetInRangePlayerSetBegin(); itr2 != _plr->GetInRangePlayerSetEnd(); ++itr2)
			{
				if ( (*itr2) != NULL && (*itr2) != _plr && (*itr2)->isAlive() && _plr->GetDistance2dSq( *itr2 ) <= 400.0f )
				{
					int32 damage = int32(9000.0f - 21.5f * _plr->GetDistance2dSq(*itr2));
					SpellEntry *tempspell = dbcSpell.LookupEntry( SHATTER );
					if ( damage > 0 && tempspell != NULL)
					{
						tempspell->c_is_flags |= 0x00000040;
						tempspell->EffectBasePoints[0] = damage;
						_plr->CastSpell( *itr2, tempspell, true );
					}
				}
			}
			_plr->RemoveAura( STONED );
		}
	}

	void hurtfulStrike()
	{
		if (_unit->GetAIInterface()->getAITargetsCount() == 0)
			return;

		TargetMap *targets = _unit->GetAIInterface()->GetAITargets();
		Unit *mUnit = _unit->GetAIInterface()->GetMostHated();
		if ( mUnit == NULL || targets == NULL )
			return;

		pair<Unit*, int32> currentTarget;
		currentTarget.first = NULL;
		currentTarget.second = -1;

		TargetMap::iterator it2 = targets->begin();
		TargetMap::iterator itr;
		for( ; it2 != targets->end(); )
		{
			itr = it2;
			++it2;

			if( _unit->GetMapMgr()->GetUnit(itr->first) == NULL || _unit->GetMapMgr()->GetUnit(itr->first)->GetTypeId() != TYPEID_PLAYER || !_unit->GetMapMgr()->GetUnit(itr->first)->isAlive() || _unit->GetDistance2dSq(_unit->GetMapMgr()->GetUnit(itr->first)) >= 100.0f )
				continue;

			if( itr->second > currentTarget.second && _unit->GetMapMgr()->GetUnit(itr->first) != mUnit )
			{
				currentTarget.first = _unit->GetMapMgr()->GetUnit(itr->first);
				currentTarget.second = itr->second;
			}
		}

		if ( currentTarget.first == NULL && mUnit != NULL && _unit->GetDistance2dSq( mUnit ) <= 100.0f)
			currentTarget.first = mUnit;

		if ( currentTarget.first != NULL )
			_unit->CastSpell(currentTarget.first, HURTFUL_STRIKE, true);
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

	uint32 GrowthCooldown;
	uint32 groundSlamcd;
	uint32 hurtfulStrikecd;
	int nrspells;
};
void SetupGruulsLair(ScriptMgr * mgr)
{
	mgr->register_creature_script(CN_LAIR_BRUTE, &LairBruteAI::Create);
	mgr->register_creature_script(CN_GRONN_PRIEST, &GronnPriestAI::Create);
	mgr->register_creature_script(CN_KIGGLER_THE_CRAZED, &KigglerTheCrazedAI::Create);
	mgr->register_creature_script(CN_BLINDEYE_THE_SEER, &BlindeyeTheSeerAI::Create);
	mgr->register_creature_script(CN_OLM_THE_SUMMONER, &OlmTheSummonerAI::Create);
	mgr->register_creature_script(CN_KROSH_FIREHAND, &KroshFirehandAI::Create);
	mgr->register_creature_script(CN_HIGH_KING_MAULAGR,	&HighKingMaulgarAI::Create);
	mgr->register_creature_script(CN_GRUUL_THE_DRAGONKILLER, &GruulsTheDragonkillerAI::Create);
}
