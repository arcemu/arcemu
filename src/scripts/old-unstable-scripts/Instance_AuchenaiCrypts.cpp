#include "StdAfx.h"
#include "Setup.h"

/************************************************************************/
/* Instance_AuchenaiCrypts.cpp Script		                            */
/************************************************************************/


/*****************************/
/*                           */
/*         Boss AIs          */
/*                           */
/*****************************/

// Shirrak the Dead WatcherAI

#define CN_SHIRRAK_THE_DEAD_WATCHER 18371

#define INHIBIT_MAGIC 32264
#define CARNIVOROUS_BITE 39382 // Also can be: 36383 or 41092
#define FOCUS_FIRE 32310 // not fully functional for now =/ Let's try: 32310, 32301 or 32300
// Hmmm... next boss without sounds?

class SHIRRAKTHEDEADWATCHERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SHIRRAKTHEDEADWATCHERAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    SHIRRAKTHEDEADWATCHERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		} 

        spells[0].info = sSpellStore.LookupEntry(INHIBIT_MAGIC);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].perctrigger = 7.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = sSpellStore.LookupEntry(CARNIVOROUS_BITE);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;				
		spells[1].perctrigger = 15.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = sSpellStore.LookupEntry(FOCUS_FIRE);
		spells[2].targettype = TARGET_DESTINATION;   // changed from attack as it wasn't working anyway
		spells[2].instant = false;				
		spells[2].perctrigger = 8.0f;
		spells[2].attackstoptimer = 1000;
		/* There should be a tag line at the end, with the name of the player Shirrak is targeting with his 'bomb' */
		spells[2].speech = "Shirrak the Dead Watcher focuses his energy";
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


// Exarch MaladaarAI

#define CN_EXARCH_MALADAAR 18373

#define SOUL_SCREAM 32421
#define SOUL_CLEAVE 32346
#define SUMMON_AVATAR_OF_THE_MARTYR 32424 // doesn't work without additional core support
#define SUMMON_DARK_SIDE 0 // Must find good spell id

class EXARCHMALADAARAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(EXARCHMALADAARAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    EXARCHMALADAARAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		SUMMON_AVATAR_OF_THE_MARTYR_LIMITER = 0;
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
        spells[0].info = sSpellStore.LookupEntry(SOUL_SCREAM);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = sSpellStore.LookupEntry(SOUL_CLEAVE);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 2000;
		spells[1].soundid = 10511;
		spells[1].speech = "Stare into the darkness of your soul!"; // not sure if it's really "stand"

		spells[2].info = sSpellStore.LookupEntry(SUMMON_AVATAR_OF_THE_MARTYR);
		spells[2].targettype = TARGET_SELF; // ?
		spells[2].instant = false;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = sSpellStore.LookupEntry(SUMMON_DARK_SIDE);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = false;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;

	} 

    void OnCombatStart(Unit* mTarget)
    {
		SUMMON_AVATAR_OF_THE_MARTYR_LIMITER = 1;
		int RandomSpeach;
		sRand.randInt(1000);
		RandomSpeach=rand()%3;
		switch (RandomSpeach)
		{
		case 0: 
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You will pay with your life!");
			_unit->PlaySoundToSet(10513);
			break;
		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "There is no turning back now!");
			_unit->PlaySoundToSet(10514);
			break;
		case 2:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Serve your penitence");
			_unit->PlaySoundToSet(10515);
			break;
		}
        RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnTargetDied(Unit* mTarget)
    {
		if (_unit->GetHealthPct() > 0)	// Hack to prevent double yelling (OnDied and OnTargetDied when creature is dying)
		{
			int RandomSpeach;
			sRand.randInt(1000);
			RandomSpeach=rand()%2;
			switch (RandomSpeach)
			{
			case 0: 
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "These wall will be your DOOM!");
				_unit->PlaySoundToSet(10516);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Haha, now you stay for eternity! Mwahahah!"); // spelling check needed !
				_unit->PlaySoundToSet(10517);
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
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "This is... Where... I belong...");
		_unit->PlaySoundToSet(10518);
       RemoveAIUpdateEvent();
	   SUMMON_AVATAR_OF_THE_MARTYR_LIMITER = 0;
    }

    void AIUpdate()
    {
		if (_unit->GetHealthPct() <= 15 && SUMMON_AVATAR_OF_THE_MARTYR_LIMITER == 0)
		{
			SUMMON_AVATAR_OF_THE_MARTYR_LIMITER = 1; // Added to prevent situations when Health bar jumps from 16 to 14% and spell is never casted
			_unit->CastSpell(_unit, spells[2].info, spells[2].instant);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Rise my fallen brothers! Take form and fight!");
			_unit->PlaySoundToSet(10512);
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

	int SUMMON_AVATAR_OF_THE_MARTYR_LIMITER;
	int nrspells;
};

void SetupAuchenaiCrypts(ScriptMgr * mgr)
{
    mgr->register_creature_script(CN_SHIRRAK_THE_DEAD_WATCHER, &SHIRRAKTHEDEADWATCHERAI::Create);
    mgr->register_creature_script(CN_EXARCH_MALADAAR, &EXARCHMALADAARAI::Create);
}
