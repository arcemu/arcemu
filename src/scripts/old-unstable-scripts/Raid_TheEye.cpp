#include "StdAfx.h"
#include "Setup.h"

/************************************************************************/
/* Raid_TheEye.cpp Script												*/
/************************************************************************/



/****************/
/*    Bosses    */
/****************/

// Void Reaver AI

#define CN_VOID_REAVER 19516

#define POUNDING 34164
#define ARCANE_ORB 34190
#define KNOCK_AWAY 36512 // maybe wrong id (maybe should be: 21737 ? ), but works
#define ENRAGE 36992 // Needs checking (as it can be wrong [or maybe IS wrong])

//TO DO: Add rest of the sounds (no idea to which action they should be used)

class VOIDREAVERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(VOIDREAVERAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    VOIDREAVERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
        spells[0].info = sSpellStore.LookupEntry(POUNDING);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 16.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = sSpellStore.LookupEntry(ARCANE_ORB);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].perctrigger = 9.0f;
		spells[1].attackstoptimer = 2000;

		spells[2].info = sSpellStore.LookupEntry(KNOCK_AWAY);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].perctrigger = 7.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = sSpellStore.LookupEntry(ENRAGE);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = true;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;
	} 

    void OnCombatStart(Unit* mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Alert, you are marked for extermination!");
		_unit->PlaySoundToSet(11213);
        RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
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
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Extermination, successful.");
				_unit->PlaySoundToSet(11215);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Imbecile life form, no longer functional.");
				_unit->PlaySoundToSet(11216);
				break;
			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Threat neutralized.");
				_unit->PlaySoundToSet(11217);
				break;
			}
		}
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

	void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Systems... shutting... down...");
        _unit->PlaySoundToSet(11214);
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		Timer = Timer + 1;

		if (Timer == 200) // Need correct attackspeed in DB to change it to correct value
		{
			_unit->CastSpell(_unit, spells[3].info, spells[3].instant);
		}

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
	int Timer;
};
/*
// High Astromancer Solarian AI


#define CN_HIGH_ASTROMANCER_SOLARIAN 18805
//Phase 1 spells
#define ARCANE_MISSILES 39414 // Should have random targeting
#define WRATH_OF_THE_ASTROMANCER 33045 // Needs random function
#define MARK_OF_SOLARIAN 33023 // not sure... but should be used on random target
//Phase 2 spells
// Just to define portal summoning + summoning creatures + creatures AI
// Add sounds to creature summoning events
//Phase 3 spells
#define VOID_BOLTS 39329 // RANDOM target, but because of lack of feature ATTACKING
#define FEAR 38154 // probably wrong id; maybe one of these are correct: 31970, 31358 (?)

// TO DO: Rewrite it to phase style.

class HIGHASTROMANCERSOLARIANAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HIGHASTROMANCERSOLARIANAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    HIGHASTROMANCERSOLARIANAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		m_phase = 1;
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
        spells[0].info = sSpellStore.LookupEntry(ARCANE_MISSILES);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = sSpellStore.LookupEntry(WRATH_OF_THE_ASTROMANCER);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 15.0f;
		spells[1].attackstoptimer = 2000;

		spells[2].info = sSpellStore.LookupEntry(MARK_OF_SOLARIAN);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].perctrigger = 10.0f;
		spells[2].attackstoptimer = 1000;
	} 

    void OnCombatStart(Unit* mTarget)
    {
		m_phase = 1;
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Whatever, Sindorai!");
		_unit->PlaySoundToSet(11134);
        RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
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
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your soul belongs to the xxxx!");
				_unit->PlaySoundToSet(11136);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "By the blood of the high born!");
				_unit->PlaySoundToSet(11137);
				break;
			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "For the sun well!");
				_unit->PlaySoundToSet(11138);
				break;
			}
		}

    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

	void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Died... W00T?!");
        _unit->PlaySoundToSet(11135);
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		float val = sRand.rand(100.0f);
        SpellCast(val);
		if (_unit->GetHealthPct() > 65)
		if (_unit->GetHealthPct() <= 65)
		if (_unit->GetHealthPct() <= 20)
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
*/


// High Astromancer Solarian AI
// First try to make it in phase system. For now not enough luck (as I want to update spell list
// when next phase has just started.

#define CN_HIGH_ASTROMANCER_SOLARIAN 18805
//Phase 1 spells
#define ARCANE_MISSILES 39414 // Should have random targeting
#define WRATH_OF_THE_ASTROMANCER 33045 // Needs random function
#define MARK_OF_SOLARIAN 33023 // not sure... but should be used on random target
//Phase 2 spells
// Just to define portal summoning + summoning creatures + creatures AI
// Add sounds to creature summoning events
//Phase 3 spells
#define VOID_BOLTS 39329 // RANDOM target, but because of lack of feature ATTACKING
#define FEAR 38154 // probably wrong id; maybe one of these are correct: 31970, 31358 (?)

// TO DO: Rewrite it to phase style.

class HIGHASTROMANCERSOLARIANAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HIGHASTROMANCERSOLARIANAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    HIGHASTROMANCERSOLARIANAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		//m_phase = 1;
		//if (m_phase == 1)
		if (_unit->GetHealthPct() > 90 || _unit->GetHealthPct() <= 50)
		{
			nrspells = 3;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
			spells[0].info = sSpellStore.LookupEntry(ARCANE_MISSILES);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].perctrigger = 8.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = sSpellStore.LookupEntry(WRATH_OF_THE_ASTROMANCER);
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = true;
			spells[1].perctrigger = 15.0f;
			spells[1].attackstoptimer = 2000;

			spells[2].info = sSpellStore.LookupEntry(MARK_OF_SOLARIAN);
			spells[2].targettype = TARGET_ATTACKING;
			spells[2].instant = true;
			spells[2].perctrigger = 10.0f;
			spells[2].attackstoptimer = 1000;
		}
	
		//if (m_phase == 2)
		if (_unit->GetHealthPct() <= 90 && _unit->GetHealthPct() > 50)
		{
			nrspells = 0;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		}
/*
		if (m_phase == 3)
		{
			nrspells = 0;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		}*/
	} 

    void OnCombatStart(Unit* mTarget)
    {
		m_phase = 1;
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Whatever, Sindorai!");
		_unit->PlaySoundToSet(11134);
        RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
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
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your soul belongs to the xxxx!");
				_unit->PlaySoundToSet(11136);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "By the blood of the high born!");
				_unit->PlaySoundToSet(11137);
				break;
			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "For the sun well!");
				_unit->PlaySoundToSet(11138);
				break;
			}
		}
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

	void OnDied(Unit * mKiller)
    {
		m_phase = 1;
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Died... W00T?!");
        _unit->PlaySoundToSet(11135);
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {/*
		if (_unit->GetHealthPct() <= 90 && _unit->GetHealthPct() > 50)
		{
			PhaseTwo();
		}
		
		if (_unit->GetHealthPct() > 90 && _unit->GetHealthPct() < 50)
		{
			PhaseOne();
		}

		float val = sRand.rand(100.0f);
		SpellCast(val);*/
		/*
		if(_unit->GetHealthPct() <= 95)
		{
			m_phase = 2;
			float val = sRand.rand(100.0f);
			SpellCast(val);
		}

		if(_unit->GetHealthPct() <= 50 || _unit->GetHealthPct() >95)
		{
			m_phase = 1;
			float val = sRand.rand(100.0f);
			SpellCast(val);
		}*/
		float val = sRand.rand(100.0f);
		SpellCast(val);
    }

	void PhaseOne()
	{
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = sSpellStore.LookupEntry(ARCANE_MISSILES);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = sSpellStore.LookupEntry(WRATH_OF_THE_ASTROMANCER);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 15.0f;
		spells[1].attackstoptimer = 2000;

		spells[2].info = sSpellStore.LookupEntry(MARK_OF_SOLARIAN);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].perctrigger = 10.0f;
		spells[2].attackstoptimer = 1000;
	}

	void PhaseTwo()
	{
		nrspells = 0;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
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
	int m_phase;
};







// Al'ar AI

#define WALK 0
#define RUN 256
#define FLY 768

#define CN_ALAR 19514

// Phase1 spells
#define FLAME_BUFFET 34121	// casted only when there is no player in melee range!
#define FLAME_QUILLS 34316	// it flies to a high point in a center of room and use this on players located on platform	// also can be many others (Idk even if this spell works =S)
#define SUMMON_PHOENIX_ADDS 18814	// Summons 3 phoenixs

// Phase2 spells
#define FLAME_PATCH 35383	// 35383, 35380;
#define METEOR 35181		// but shouldn't be instant imho
#define EMBER_BLAST 34133	// used when one of adds has low health
/*#define MELT_ARMOR		// maybe they are used, but not sure
#define RANDOM_CHARGE*/

// Other spells
#define REBIRTH 34342

struct Coords
{
    float x;
    float y;
    float z;
    float o;
};

static Coords fly[] = 
{
	{ 0, 0, 0, 0 },
	{ 337.864868, 65.702301, 33.171944, 1.329919 },	// fly 1 to ...
	{ 391.245148, 35.472462, 36.886353, 0.822089 },
	{ 392.959869, -34.031734, 33.630096, 3.342431 },
	{ 321.277008, -70.101540, 43.650482, 3.844443 },
    { 256.202911, -1.549352, 44.718426, 0.069512 },	// ... fly 5
	{ 342.090088, 55.661064, 27.638260, 0.739368},	// attack/tank positions from phase 1 from here to ...
	{ 392.815369, 31.636963, 25.414761, 0.551340 },
	{ 388.397308, -38.834595, 22.335297, 5.702067 },
	{ 333.922229, -60.645069, 24.484278, 1.454599 },	// ... to here
	{ 328.103455, -0.192393, 52.216309, 4.188326 },	// fire quills cast position	// EMOTE_STATE_WHIRLWIND = 382,
	{ 326.225647, 2.381837, -2.389485, 4.877070 }	// center of the room which is used in phase 2
};



class AlarAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(AlarAI);
	SP_AI_Spell spells[7];
	bool m_spellcheck[7];

    AlarAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		/************************ Waypoint Place ************************/

        m_entry = pCreature->GetEntry();

		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(1, 0, FLY));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(2, 0, FLY));
        _unit->GetAIInterface()->addWayPoint(CreateWaypoint(3, 0, FLY));
        _unit->GetAIInterface()->addWayPoint(CreateWaypoint(4, 0, FLY));
        _unit->GetAIInterface()->addWayPoint(CreateWaypoint(5, 0, FLY));
        _unit->GetAIInterface()->addWayPoint(CreateWaypoint(6, 0, FLY));
        _unit->GetAIInterface()->addWayPoint(CreateWaypoint(7, 0, FLY));
        _unit->GetAIInterface()->addWayPoint(CreateWaypoint(8, 0, FLY));
        _unit->GetAIInterface()->addWayPoint(CreateWaypoint(9, 0, FLY));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(10, 0, FLY));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(11, 0, FLY));

		/************************** Spells ****************************/

		nrspells = 7;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = sSpellStore.LookupEntry(FLAME_BUFFET);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = sSpellStore.LookupEntry(FLAME_QUILLS);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].cooldown = -1;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = sSpellStore.LookupEntry(SUMMON_PHOENIX_ADDS);
		spells[2].targettype = TARGET_VARIOUS;
		spells[2].instant = true;
		spells[2].cooldown = -1;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = sSpellStore.LookupEntry(FLAME_PATCH);
		spells[3].targettype = TARGET_VARIOUS;
		spells[3].instant = true;
		spells[3].cooldown = -1;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;

		spells[4].info = sSpellStore.LookupEntry(METEOR);
		spells[4].targettype = TARGET_ATTACKING;
		spells[4].instant = true;	// =(
		spells[4].cooldown = -1;
		spells[4].perctrigger = 0.0f;
		spells[4].attackstoptimer = 1000;

		spells[5].info = sSpellStore.LookupEntry(EMBER_BLAST);
		spells[5].targettype = TARGET_VARIOUS;
		spells[5].instant = true;
		spells[5].cooldown = -1;
		spells[5].perctrigger = 0.0f;
		spells[5].attackstoptimer = 1000;

		spells[6].info = sSpellStore.LookupEntry(REBIRTH);
		spells[6].targettype = TARGET_SELF;
		spells[6].instant = false;
		spells[6].cooldown = -1;
		spells[6].perctrigger = 0.0f;
		spells[6].attackstoptimer = 1000;

		/******************* Additional Settings *******************/

		Phase = 0;
		FlameQuills = false;
		Meteor = false;
		PositionChange=rand()%8+15;
		PhoenixSummon=rand()%6+17;
		_unit->GetAIInterface()->m_moveFly = true;
        _unit->GetAIInterface()->StopMovement(0);
        _unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
		_unit->GetAIInterface()->setWaypointToMove(1);
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Phase 1 Test!");
		_unit->PlaySoundToSet(11243);
		_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
		CastTime();
		Phase = 1;
		FlameQuills = false;
		Meteor = false;
		PositionChange=rand()%8+15;	// 30-45sec /*** if attack time 1000 (%15+31) ***/
		PhoenixSummon=rand()%6+17;	// 34-44sec /*** if attack time 1000 (%11+34) ***/
		FlyWay=rand()%2;
		switch (FlyWay)
		{
		case 0:	// Clock like
			{
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
				_unit->GetAIInterface()->setWaypointToMove(6);
			}break;

		case 1:	// hmm... other?
			{
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
				_unit->GetAIInterface()->setWaypointToMove(9);
			}break;
		}
		//RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));	// Taken from here to add more abilities to code
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit *mTarget)
    {
		Phase = 0;
		FlameQuills = false;
		Meteor = false;
		PhoenixSummon=rand()%6+17;
		PositionChange=rand()%8+15;
		CastTime();
        _unit->GetAIInterface()->StopMovement(0);
        _unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
		_unit->GetAIInterface()->setWaypointToMove(1);
        //_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        //_unit->GetAIInterface()->SetAIState(STATE_IDLE);
        //RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		Phase = 0;
		FlameQuills = false;
		Meteor = false;
		PositionChange=rand()%8+15;
		PhoenixSummon=rand()%6+17;
		CastTime();
       //RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		if (FlameQuills == true)
		{
			QuillsCount++;
			if (QuillsCount == 9)
			{
				FlameQuills = false;
				switch (FlyWay)
				{
				case 0:	// Clock like
					{
						_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
						_unit->GetAIInterface()->setWaypointToMove(6);
					}break;
		
				case 1:	// hmm... other?
					{
						_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
						_unit->GetAIInterface()->setWaypointToMove(9);
					}break;
				}
			}
			_unit->CastSpell(_unit, spells[1].info, spells[1].instant);
		}

		if (Meteor == true)
		{
		}
		
		else 
		{
			switch (Phase)
			{
			case 0: return;
			case 1: 
				{
					PhaseOne();
				}break;
			case 2:
				{
					PhaseTwo();
				}break;
			default:
				{
					Phase = 0;
				};
			};
		}
		//float val = sRand.rand(100.0f);
		//SpellCast(val);
    }

    void PhaseOne()
    {
		PositionChange--;
		PhoenixSummon--;

		if (_unit->GetHealthPct() == 0)
		{
			Phase = 2;
			_unit->CastSpell(_unit, spells[6].info, spells[6].instant);
		}

		if (!PhoenixSummon--)
		{
			_unit->CastSpell(_unit, spells[2].info, spells[2].instant);
			PhoenixSummon=rand()%6+17;
		}

		if (!PositionChange)
		{
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			_unit->GetAIInterface()->setWaypointToMove(NextWP);
			PositionChange=rand()%8+17;	// added 4 sec fit time + time needed to move to next pos.
		}

		else
		{
			uint32 val = sRand.randInt(100);

			if (val > 0 && val < 5)	// Flame Quills wp here!
			{
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
				_unit->GetAIInterface()->setWaypointToMove(10);
			}
		}
    }

	void PhaseTwo()
	{

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

    void OnReachWP(uint32 iWaypointId, bool bForwards)
    {
		if (Phase == 1)
		{
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			_unit->GetAIInterface()->setWaypointToMove(6);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Phase 1 Test!");
			_unit->PlaySoundToSet(11243);
		}

		switch(iWaypointId)
		{
			case 1:	// First fly point
				{
					_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
					_unit->GetAIInterface()->setWaypointToMove(2);
				}break;

			case 2:
				{
					_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
					_unit->GetAIInterface()->setWaypointToMove(3);
				}break;

			case 3:
			    {
			        _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
					_unit->GetAIInterface()->setWaypointToMove(4);
			    }break;

			case 4:
			    {
			        _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			        _unit->GetAIInterface()->setWaypointToMove(5);
			    }break;

			case 5:
			    {
			        _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			        _unit->GetAIInterface()->setWaypointToMove(1);	// Last fly point (flyback to point 1 - reset)
			    }break;

			case 6:
			    {
					_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
					_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
					_unit->GetAIInterface()->m_canMove = false;
					switch (FlyWay)
					{
					case 0:
						{
							NextWP = 7;
						}break;

					case 1:
						{	
							NextWP = 9;
						}break;
					}
				}break;
				
			case 7:
			    {
					_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
					_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
					_unit->GetAIInterface()->m_canMove = false;
					switch (FlyWay)
					{
					case 0:
						{
							NextWP = 8;
						}break;

					case 1:
						{	
							NextWP = 6;
						}break;
					}
			    }break;
		
			case 8:
		    {
				_unit->GetAIInterface()->m_canMove = false;
				_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
                _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
				switch (FlyWay)
				{
				case 0:
					{
						NextWP = 9;
					}break;
	
				case 1:
					{	
						NextWP = 7;
					}break;
				}
		    }break;

			case 9:
		    {
				_unit->GetAIInterface()->m_canMove = false;
				_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
                _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
				switch (FlyWay)
				{
				case 0:
					{
						NextWP = 6;
					}break;

				case 1:
					{	
						NextWP = 8;
					}break;
				}
			}break;

			case 10:
		    {
				_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
                _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
				if (Phase == 1)
				{
					FlameQuills = true;
					QuillsCount = 0;
				}

				if (Phase == 2)
				{
					Meteor = true;
				}

			}break;
		}
    }

    inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
    {
        WayPoint * wp = _unit->CreateWaypointStruct();
        wp->id = id;
        wp->x = fly[id].x;
        wp->y = fly[id].y;
        wp->z = fly[id].z;
        wp->o = fly[id].o;
        wp->waittime = waittime;
        wp->flags = flags;
        wp->forwardemoteoneshot = 0;
        wp->forwardemoteid = 0;
        wp->backwardemoteoneshot = 0;
        wp->backwardemoteid = 0;
        wp->forwardskinid = 0;
        wp->backwardskinid = 0;
        return wp;
    }

protected:

	bool FlameQuills;
	uint32 QuillsCount;
	bool Meteor;
	int PositionChange;
	int PhoenixSummon;
	uint32 NextWP;
    uint32 m_entry;
    uint32 FlyWay;
	uint32 Phase;
	int nrspells;
};


void SetupTheEye(ScriptMgr * mgr)
{
	mgr->register_creature_script(CN_VOID_REAVER, &VOIDREAVERAI::Create);
	mgr->register_creature_script(CN_HIGH_ASTROMANCER_SOLARIAN, &HIGHASTROMANCERSOLARIANAI::Create);
	mgr->register_creature_script(CN_ALAR, &AlarAI::Create);
}
