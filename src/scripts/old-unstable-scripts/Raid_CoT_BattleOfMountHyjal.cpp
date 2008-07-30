#include "StdAfx.h"
#include "Setup.h"

/************************************************************************/
/* Raid_CoT_BattleOfMountHyjal.cpp Script								*/
/************************************************************************/

/*****************************/
/*                           */
/*         Boss AIs          */
/*                           */
/*****************************/

/*  * Might of the Defiler - Smashed into the ground for 6000 fire damage, 
		and 3000 every second for 10 seconds.
    * Grip of the Legion - 250,000 damage over 5 mins. [833.33dps].
    * Finger of Death - 20,000 shadow damage on a single target. Archimonde 
		will begin casting this if nobody is in melee range.
    * Soul Charge - All damage taken increased by 50%. Non-dispellable.
    * Doomfire - Leaves a trail of fire on the ground, which does 2400 fire 
		damage per second (occasionally feared people run into these and die) */

// ArchimondeAI

#define CN_ARCHIMONDE 17968

#define MIGHT_OF_THE_DEFILER 0 // can't find :|?
#define GRIP_OF_THE_LEGION 31972
#define FINGER_OF_DEATH 31984 // 31984 or 32111 // should be casted only when no one in melee range
#define SOUL_CHARGE 32054
#define DOOMFIRE 31944 // 31945 -> 31943 -> 31944 // bugged as it's targetting only caster :O
// TO DO: Add rest of the sounds (first I must gather infos about to which spell or
// event there are connected.

class ARCHIMONDEAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ARCHIMONDEAI);
	SP_AI_Spell spells[5];
	bool m_spellcheck[5];

    ARCHIMONDEAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 5;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		} 
		spells[0].info = sSpellStore.LookupEntry(MIGHT_OF_THE_DEFILER);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = sSpellStore.LookupEntry(GRIP_OF_THE_LEGION);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 6.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = sSpellStore.LookupEntry(FINGER_OF_DEATH);
		spells[2].targettype = TARGET_ATTACKING; 
		spells[2].instant = false;
		spells[2].perctrigger = 6.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = sSpellStore.LookupEntry(SOUL_CHARGE);
		spells[3].targettype = TARGET_VARIOUS;
		spells[3].instant = false;
		spells[3].perctrigger = 7.0f;
		spells[3].attackstoptimer = 1000;

		spells[4].info = sSpellStore.LookupEntry(DOOMFIRE);
		spells[4].targettype = TARGET_ATTACKING;
		spells[4].instant = true;
		spells[4].perctrigger = 7.0f;
		spells[4].attackstoptimer = 2000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your resistance is insignificant.");
		_unit->PlaySoundToSet(10987);
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
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your soul will languish for eternity.");
				_unit->PlaySoundToSet(10993);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "All creation will be devoured!");
				_unit->PlaySoundToSet(10994);
				break;
			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I am the coming of the end!");
				_unit->PlaySoundToSet(10995);
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
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "No, it cannot be! Nooo!");
		_unit->PlaySoundToSet(10992);
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
void SetupBattleOfMountHyjal(ScriptMgr * mgr)
{
    mgr->register_creature_script(CN_ARCHIMONDE, &ARCHIMONDEAI::Create);
}
