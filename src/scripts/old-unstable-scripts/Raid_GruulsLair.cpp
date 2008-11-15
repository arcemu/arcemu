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

		spells[0].info = sSpellStore.LookupEntry(MORTALSTRIKE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 9.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = sSpellStore.LookupEntry(CLEAVE);
		spells[1].targettype = TARGET_ATTACKING;	
		spells[1].instant = true;
		spells[1].perctrigger = 8.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = sSpellStore.LookupEntry(CHARGE);
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
		float val = sRand.rand(100.0f);
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

		spells[0].info = sSpellStore.LookupEntry(PSYCHICSCREAM);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = sSpellStore.LookupEntry(RENEW);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].perctrigger = 6.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = sSpellStore.LookupEntry(HEAL_GRONN_PRIEST);
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
		float val = sRand.rand(100.0f);
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

		spells[0].info = sSpellStore.LookupEntry(LIGHTNING_BOLT);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 12.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = sSpellStore.LookupEntry(GREATER_POLYMORPH);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 8.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = sSpellStore.LookupEntry(ARCANE_EXPLOSION);
		spells[2].targettype = TARGET_VARIOUS;
		spells[2].instant = true;
		spells[2].perctrigger = 12.0f;
		spells[2].attackstoptimer = 1000; 


		spells[3].info = sSpellStore.LookupEntry(ARCANE_SHOCK);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = true;
		spells[3].perctrigger = 8.0f;
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
/*
		if(_unit->GetDistanceSq(target)<20)  <--- why do you need this??
			_unit->CastSpell(_unit, spells[2].info, spells[2].instant);
																			*/
/*		
		if(m_spellcheck[1] == true) //<^-------- Both functions crash server in this place
			_unit->GetAIInterface()->GetNextTarget();  
																			*/
		float val = sRand.rand(100.0f);
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

		spells[0].info = sSpellStore.LookupEntry(PRAYER_OF_HEALING);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = sSpellStore.LookupEntry(GREAT_POWER_WORD_SHIELD);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].perctrigger = 5.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = sSpellStore.LookupEntry(HEAL);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = false;
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
		float val = sRand.rand(100.0f);
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

		spells[0].info = sSpellStore.LookupEntry(DEATH_COIL);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 7.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = sSpellStore.LookupEntry(SUMMON_WILD_FEL_STALKER);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = false;
		spells[1].perctrigger = 2.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = sSpellStore.LookupEntry(DARK_DECAY);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].perctrigger = 10.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = sSpellStore.LookupEntry(HEAL_OLM);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = false;
		spells[3].perctrigger = 10.0f;
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
		float val = sRand.rand(100.0f);
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
// Krosh FirehandAI
#define CN_KROSH_FIREHAND 18832

#define GREAT_FIREBALL	33051
#define BALST_WAVE		33061
#define FIRE_WARD		37844
//#define FIREBALL 37463
//#define SPELLSHIELD 33054

class KroshFirehandAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(KroshFirehandAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    KroshFirehandAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		FireWardCooldown=30;
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		} 

		spells[0].info = sSpellStore.LookupEntry(GREAT_FIREBALL);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 20.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = sSpellStore.LookupEntry(BALST_WAVE);
		spells[1].targettype = TARGET_DESTINATION;
		spells[1].instant = true;
		spells[1].perctrigger = 15.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = sSpellStore.LookupEntry(FIRE_WARD);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;
	}
	void OnCombatStart(Unit* mTarget)
    {
        RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

    void OnCombatStop(Unit *mTarget)
    {
		FireWardCooldown=30;
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

	void OnDied(Unit * mKiller)
    {
		FireWardCooldown=30;
       RemoveAIUpdateEvent();
    }

	void AIUpdate()
    {
		FireWardCooldown--;
		if(!FireWardCooldown)//_unit->getAttackTarget())
        {
			_unit->CastSpell(_unit, spells[2].info, spells[2].instant);
			FireWardCooldown=30;
		}
	/*
		if(_unit->GetDistanceSq(target)<20) // bad idea, as long as I can see that
			_unit->CastSpell(_unit, spells[2].info, spells[2].instant);
																					*/
		else
		{
			float val = sRand.rand(100.0f);
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
};

// MaulgarAI
#define CN_HIGH_KING_MAULAGR 18831

#define ARCINGSMASH		38761
#define MIGHTYBLOW		33230
#define WHIRLWIND		33239
#define ENRAGE			34970 // no idea about id, can be also: 34970, 37023 and others as nowhere is told which Enrage it should be

class HighKingMaulgarAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HighKingMaulgarAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    HighKingMaulgarAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		} 

		spells[0].info = sSpellStore.LookupEntry(ARCINGSMASH);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 15.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = sSpellStore.LookupEntry(MIGHTYBLOW);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = sSpellStore.LookupEntry(WHIRLWIND);
		spells[2].targettype = TARGET_VARIOUS; // Should attk party member with second the highest aggro in melee range
		spells[2].instant = true;
		spells[2].perctrigger = 9.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = sSpellStore.LookupEntry(ENRAGE);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = true;
		spells[3].perctrigger = 5.0f;
		spells[3].attackstoptimer = 1000;
		spells[3].speech = "You will not defeat the hand of Gruul!";
		spells[3].soundid = 11368;
	}
	void OnCombatStart(Unit* mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "<missing_word> on a real power in Outland!");
		_unit->PlaySoundToSet(11367);
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
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Grull will crash you!");
		_unit->PlaySoundToSet(11376);
       RemoveAIUpdateEvent();
    }

	void OnTargetDied(Unit* mTarget)
	{
		if (_unit->GetHealthPct() > 0)	// Hack to prevent double yelling (OnDied and OnTargetDied when creature is dying)
		{
			int RandomSpeach;
			sRand.randInt(1000);
			RandomSpeach=rand()%3;
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
		float val = sRand.rand(100.0f);
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
// GruulsAI

#define CN_GRUUL_THE_DRAGONKILLER 19044

#define GROWTH            36300 
#define CAVE_IN           36240 
#define GROUND_SLAM       33525
#define SHATTER           33671	// Should be used only when Stoned
#define HURTFUL_STRIKE    33813 
#define REVERBERATION     36297 
#define STONED			  33652 
#define GRONN_LORDS_GRASP 33572 // Should be used only after Ground Slam
// % chances changed to let boss use normal attks too
// Note: Maybe we should add additional spell description option to
// define next spells.

// TO DO: Find out what rest of sounds are for and add feature to random choose between
// several spell sounds.

class GruulsTheDragonkillerAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(GruulsTheDragonkillerAI);
	SP_AI_Spell spells[8];
	bool m_spellcheck[8];

    GruulsTheDragonkillerAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		GrowthCooldown = 30;
		nrspells = 8;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		} 
		spells[0].info = sSpellStore.LookupEntry(GROWTH);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = sSpellStore.LookupEntry(CAVE_IN);
		spells[1].targettype = TARGET_DESTINATION;
		spells[1].instant = true;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = sSpellStore.LookupEntry(HURTFUL_STRIKE);
		spells[2].targettype = TARGET_ATTACKING; // Should attk party member with second the highest aggro in melee range
		spells[2].instant = true;
		spells[2].perctrigger = 3.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = sSpellStore.LookupEntry(REVERBERATION);
		spells[3].targettype = TARGET_VARIOUS;
		spells[3].instant = true;
		spells[3].perctrigger = 6.0f;
		spells[3].attackstoptimer = 1000;

		spells[4].info = sSpellStore.LookupEntry(GROUND_SLAM);
		spells[4].targettype = TARGET_DESTINATION;
		spells[4].instant = false;
		spells[4].perctrigger = 7.0f;
		spells[4].attackstoptimer = 2000;
		spells[4].speech = "Scary!"; // has 2 sounds for one spell :O
		spells[4].soundid = 11357;
		//spells[4].speech = ""No escape!; // has 2 sounds for one spell :O
		//spells[4].soundid = 11356;

		spells[5].info = sSpellStore.LookupEntry(SHATTER);
		spells[5].targettype = TARGET_VARIOUS;
		spells[5].instant = true;
		spells[5].perctrigger = 6.0f;
		spells[5].attackstoptimer = 1000;

		spells[6].info = sSpellStore.LookupEntry(STONED);
		spells[6].targettype = TARGET_SELF;
		spells[6].instant = true;
		spells[6].perctrigger = 7.0f;
		spells[6].attackstoptimer = 1000;

		spells[7].info = sSpellStore.LookupEntry(GRONN_LORDS_GRASP);
		spells[7].targettype = TARGET_SELF; // <-- not sure to that (description tells it works like that, but should it really be casted on boss?)
		spells[7].instant = true;
		spells[7].perctrigger = 6.0f;
		spells[7].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Come and die!");
		_unit->PlaySoundToSet(11355);
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->CastSpell(_unit, spells[0].info, spells[0].instant);
    }

	void OnTargetDied(Unit* mTarget)
    {
		if (_unit->GetHealthPct() > 0)	// Hack to prevent double yelling (OnDied and OnTargetDied when creature is dying)
		{
			int RandomSpeach;
			sRand.randInt(1000);
			RandomSpeach=rand()%3;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "No more!");
				_unit->PlaySoundToSet(11360);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Unworthy!");
				_unit->PlaySoundToSet(11361);
				break;
			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Die!");
				_unit->PlaySoundToSet(11362);
				break;
			}
		}
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        GrowthCooldown = 30;
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Wraaaa!"); // more correct please ? :P
		_unit->PlaySoundToSet(11363);
       RemoveAIUpdateEvent();
       GrowthCooldown = 30;
    }

    void AIUpdate()
    {
		GrowthCooldown--;
		if(!GrowthCooldown)//_unit->getAttackTarget())
        {
		_unit->CastSpell(_unit, spells[0].info, spells[0].instant);
		GrowthCooldown=30;
		}
		float val = sRand.rand(100.0f);
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

	uint32 GrowthCooldown;
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
