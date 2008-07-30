#include "StdAfx.h"
#include "Setup.h"

/************************************************************************/
/* Instance_Botanica.cpp Script		                                    */
/************************************************************************/

// Bloodwarder Protector AI

#define CN_BLOOD_PROTECTOR 17993

#define CRYSTAL_STRIKE 29765 // 1 target

class BloodProtectorAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(BloodProtectorAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    BloodProtectorAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 1;

		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
		// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 
        spells[0].info = sSpellStore.LookupEntry(CRYSTAL_STRIKE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000; // 1sec

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


// Bloodwarder Mender AI

#define CN_BLOOD_MENDER

/*  * Healer
    * Casts Shadow Word: Pain and Mind Blast
    * Mind Control these for Holy Fury buff (+295 spell damage for 30 minutes, shows as DIVINE fury on the pet bar). Can be spellstolen. 
*/

// Bloodwarder Greenkeeper AI

#define CN_BLOOD_GREENKEEPER 18419

#define GREENKEEPER_FURY 39121

class BloodGreenkeeperAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(BloodGreenkeeperAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    BloodGreenkeeperAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 1;

		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
		// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 
        spells[0].info = sSpellStore.LookupEntry(GREENKEEPER_FURY);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000; // 1sec

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


// Sunseeker Chemist AI
#define CN_SUN_CHEMIST 19486

#define FLAME_BREATH 18435
#define POISON_CLOUD 37469

class SunchemistAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(SunchemistAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    SunchemistAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 2;

		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
		// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 
        spells[0].info = sSpellStore.LookupEntry(FLAME_BREATH);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 3000; // 1sec

		spells[1].info = sSpellStore.LookupEntry(POISON_CLOUD);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = false;
		spells[1].perctrigger = 5.0f;
		spells[1].attackstoptimer = 2000; // 1sec
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


// Sunseeker Researcher AI

#define CN_SUN_RESEARCHER 18421

#define POISON_SHIELD 39027 // self
#define MIND_SHOCK 34352 // 1 target
#define FROST_SHOCK 39062 // 1 target
#define FLAME_SHOCK 22423 // 1 target

class SunResearcherAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(SunResearcherAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    SunResearcherAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 4;

		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
		// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 
        spells[0].info = sSpellStore.LookupEntry(POISON_SHIELD);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = false;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000; // 1sec

		spells[1].info = sSpellStore.LookupEntry(MIND_SHOCK);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].perctrigger = 5.0f;
		spells[1].attackstoptimer = 2000; // 1sec

		spells[2].info = sSpellStore.LookupEntry(FROST_SHOCK);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = false;
		spells[2].perctrigger = 5.0f;
		spells[2].attackstoptimer = 2000; // 1sec

		spells[3].info = sSpellStore.LookupEntry(FLAME_SHOCK);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = false;
		spells[3].perctrigger = 10.0f;
		spells[3].attackstoptimer = 2000; // 1sec
    }
    
    void OnCombatStart(Unit* mTarget)
    {
        RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->CastSpell(_unit, spells[0].info, spells[0].instant);
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

/**************************/
/*                        */
/*       Boss AIs         */
/*                        */
/**************************/

// Commander Sarannis AI

#define CN_COMMANDER_SARANNIS 17976	// spawn adds (or maybe write spell which will be harder)

#define ARCANE_RESONANCE 34794
#define ARCANE_DEVASTATION 34799
#define SUMMON_REINFORCEMENTS 34803	// it's dummy (sss) and must be scripted separately

class CommanderSarannisAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(CommanderSarannisAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    CommanderSarannisAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		GuardAdds = false;
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = sSpellStore.LookupEntry(ARCANE_RESONANCE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 7.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = sSpellStore.LookupEntry(ARCANE_DEVASTATION);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].cooldown = -1;
		spells[1].perctrigger = 15.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = sSpellStore.LookupEntry(SUMMON_REINFORCEMENTS);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;
		spells[2].cooldown = -1;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		GuardAdds = false;
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Step forward! I will see that you are appropriate welcomed.");	// needs checks!
		_unit->PlaySoundToSet(11071);
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit *mTarget)
    {
		GuardAdds = false;
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
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
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Oh, stop your <missing_word>!");	// :|
				_unit->PlaySoundToSet(11072);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Mission accomplished!");
				_unit->PlaySoundToSet(11073);
				break;
			}
		}
    }

    void OnDied(Unit * mKiller)
    {
		GuardAdds = false;
		CastTime();
       RemoveAIUpdateEvent();
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I have not yet... begun to...");	// ??
		_unit->PlaySoundToSet(11079);
    }

    void AIUpdate()
	{
		if (_unit->GetHealthPct() <= 50 && GuardAdds == false)
		{
			GuardAdds = true;	// need to add guard spawning =/
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Guards, rally! Cut these invaders down!");	// not sure
			_unit->PlaySoundToSet(11078);
		}
		float val = sRand.rand(100.0f);
		SpellCast(val);
    }

	void ArcaneSound()
	{
		int RandomArcane;
		RandomArcane=rand()%30;
		switch (RandomArcane)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You are no longer dealing with some <missing_word>.");
			_unit->PlaySoundToSet(11076);
			break;
		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "<missing_text>!");
			_unit->PlaySoundToSet(11077);
			break;
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

					ArcaneSound();

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

	bool GuardAdds;
	int nrspells;
};

// High Botanist Freywinn AI

#define CN_HIGH_BOTANIST_FREYWINN 17975	

#define PLANT_RED_SEEDLING 34763
#define PLANT_GREEN_SEEDLING 34761
#define PLANT_WHITE_SEEDLING 34759
#define PLANT_BLUE_SEEDLING 34762
#define SUMMON_FRAYER_PROTECTOR 34557
#define TREE_FORM 34551
#define TRANQUILITY 34550

class HighBotanistFreywinnAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(HighBotanistFreywinnAI);
	SP_AI_Spell spells[7];
	bool m_spellcheck[7];

    HighBotanistFreywinnAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		PlantTimer = 10;
		nrspells = 7;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = sSpellStore.LookupEntry(PLANT_RED_SEEDLING);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = sSpellStore.LookupEntry(PLANT_GREEN_SEEDLING);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].cooldown = -1;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = sSpellStore.LookupEntry(PLANT_WHITE_SEEDLING);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;
		spells[2].cooldown = -1;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = sSpellStore.LookupEntry(PLANT_BLUE_SEEDLING);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = true;
		spells[3].cooldown = -1;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;

		spells[4].info = sSpellStore.LookupEntry(SUMMON_FRAYER_PROTECTOR);
		spells[4].targettype = TARGET_SELF;
		spells[4].instant = true;
		spells[4].cooldown = -1;
		spells[4].perctrigger = 5.0f;
		spells[4].attackstoptimer = 1000;

		spells[5].info = sSpellStore.LookupEntry(TREE_FORM);
		spells[5].targettype = TARGET_SELF;
		spells[5].instant = true;
		spells[5].cooldown = 40;
		spells[5].perctrigger = 0.0f;
		spells[5].attackstoptimer = 1000;

		spells[6].info = sSpellStore.LookupEntry(TRANQUILITY);
		spells[6].targettype = TARGET_VARIOUS;
		spells[6].instant = false;
		spells[6].cooldown = -1;
		spells[6].perctrigger = 0.0f;
		spells[6].attackstoptimer = 1000;


    }
    
    void OnCombatStart(Unit* mTarget)
    {
		PlantTimer = 10;
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "What are you doin'? These <missing_word> are very delicate!");	// needs checks!
		_unit->PlaySoundToSet(11144);
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit *mTarget)
    {
		PlantTimer = 10;
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
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
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your life circle is now concluded!");
				_unit->PlaySoundToSet(11145);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You will feed the worms!");
				_unit->PlaySoundToSet(11146);
				break;
			}
		}
    }

    void OnDied(Unit * mKiller)
    {
		PlantTimer = 10;
		CastTime();
       RemoveAIUpdateEvent();
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Those <missing_word> must be preserved!");	// ??
		_unit->PlaySoundToSet(11149);
    }

    void AIUpdate()
	{
		PlantTimer--;
		if (!PlantTimer)
		{
			PlantColorSeedling();
		}

		float val = sRand.rand(100.0f);
		SpellCast(val);
    }

	void PlantColorSeedling()
	{
		PlantTimer=rand()%6+5;	//5-10 sec (as in my DB attack time is 1000)
		uint32 RandomPlant;
		RandomPlant=rand()%4;
		switch (RandomPlant)
		{
		case 0:
			{
			_unit->CastSpell(_unit, spells[0].info, spells[0].instant);
			}break;
		case 1:
			{
			_unit->CastSpell(_unit, spells[1].info, spells[1].instant);
			}break;
		case 2:
			{
			_unit->CastSpell(_unit, spells[2].info, spells[2].instant);
			}break;
		case 4:
			{
			_unit->CastSpell(_unit, spells[3].info, spells[3].instant);
			}break;
		}
	}

	void TreeSound()
	{
		uint32 RandomTree;
		RandomTree=rand()%2;
		switch (RandomTree)
		{
		case 0:
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "<missing_text>");
				_unit->PlaySoundToSet(11147);
			}break;
		case 1:
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Nature bends to my will!");
				_unit->PlaySoundToSet(11148);
			}break;
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

					if (m_spellcheck[5] == true)
					{
						TreeSound();
						m_spellcheck[6] = true;
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

	uint32 PlantTimer;
	int nrspells;
};

// Thorngrin the Tender AI

#define CN_THORNGRIN_THE_TENDER 17978	

#define HELLFIRE 34659 // DBC: 34659, 34660
#define SACRIFICE 34661
#define ENRAGE 34670

class ThorngrinTheTenderAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(ThorngrinTheTenderAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    ThorngrinTheTenderAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		Enraged = false;
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = sSpellStore.LookupEntry(HELLFIRE);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 9.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = sSpellStore.LookupEntry(SACRIFICE);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].cooldown = -1;
		spells[1].perctrigger = 6.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = sSpellStore.LookupEntry(ENRAGE);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;
		spells[2].cooldown = -1;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		Enraged = false;
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "What aggravation is this? You will die!");
		_unit->PlaySoundToSet(11205);
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit *mTarget)
    {
		Enraged = false;
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
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
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You seek a prize, eh? How about death?");
				_unit->PlaySoundToSet(11206);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I hate to say I told you so...");
				_unit->PlaySoundToSet(11207);
				break;
			}
		}
    }

    void OnDied(Unit * mKiller)
    {
		Enraged = false;
		CastTime();
       RemoveAIUpdateEvent();
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You won't... get far.");
		_unit->PlaySoundToSet(11212);
    }

    void AIUpdate()
	{
		if (_unit->GetHealthPct() <= 20 && Enraged == false)
		{
			Enraged = true;
			_unit->CastSpell(_unit, spells[2].info, spells[2].instant);
		}

		float val = sRand.rand(100.0f);
		SpellCast(val);
    }

	void HellfireSound()
	{
		int RandomHellfire;
		RandomHellfire=rand()%2;
		switch (RandomHellfire)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I'll incinerate you!");
			_unit->PlaySoundToSet(11210);
			break;
		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Scream while you burn!");
			_unit->PlaySoundToSet(11211);
			break;
		}
	}

	void SacrificeSound()
	{
		int RandomSacrifice;
		RandomSacrifice=rand()%2;
		switch (RandomSacrifice)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your life will be mine!");
			_unit->PlaySoundToSet(11208);
			break;
		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I revel in your pain!");
			_unit->PlaySoundToSet(11209);
			break;
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

					if (m_spellcheck[1] == true)
					{
						SacrificeSound();
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

					if (m_spellcheck[0] == true)	// Hellfire
					{
						HellfireSound();
					}
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

protected:

	bool Enraged;
	int nrspells;
};

// Laj AI

#define CN_LAJ 17980	

#define ALERGIC_REACTION 34697
#define SUMMON_THORN_LASHER 34684 // DBC: 34684, 34681
#define SUMMON_THORN_FLAYER 34682 // DBC: 34685, 34682		// they should be spawned on platforms
#define TELEPORT_SELF 34673

class LajAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(LajAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    LajAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		TeleportTimer = 20;	// It's sth about that
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = sSpellStore.LookupEntry(ALERGIC_REACTION);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = sSpellStore.LookupEntry(SUMMON_THORN_LASHER);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].cooldown = -1;
		spells[1].perctrigger = 6.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = sSpellStore.LookupEntry(SUMMON_THORN_FLAYER);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;
		spells[2].cooldown = -1;
		spells[2].perctrigger = 6.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = sSpellStore.LookupEntry(TELEPORT_SELF);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = true;
		spells[3].cooldown = -1; // will take this spell separately as it needs additional coding for changing position
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		TeleportTimer = 20;
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit *mTarget)
    {
		TeleportTimer = 20;
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

	void OnTargetDied(Unit* mTarget)
    {
    }

    void OnDied(Unit * mKiller)
    {
		TeleportTimer = 20;
		CastTime();
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		TeleportTimer--;

		if (!TeleportTimer)
		{
			_unit->SetPosition(-204.125000, 391.248993, -11.194300, 0.017453);	// hmm doesn't work :S
			_unit->CastSpell(_unit, spells[3].info, spells[3].instant);
			TeleportTimer = 20;
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

	uint32 TeleportTimer;
	int nrspells;
};

// Warp Splinter AI

#define CN_WARP_SPLINTER 17977	

#define STOMP 34716
#define SUMMON_SAPLINGS 34727	// DBC: 34727, 34731, 34733, 34734, 34736, 34739, 34741 (with Ancestral Life spell 34742)	// won't work (guardian summon)
#define ARCANE_VOLLEY 34785	 //37078, 34785   // must additional script them (because Splinter eats them after 20 sec ^)
// ^ Doesn't work somehow when used by mob :O

class WarpSplinterAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(WarpSplinterAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    WarpSplinterAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {

		SummonTimer = 20;	// It's sth about that
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = sSpellStore.LookupEntry(STOMP);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = sSpellStore.LookupEntry(SUMMON_SAPLINGS);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].cooldown = -1;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = sSpellStore.LookupEntry(ARCANE_VOLLEY);
		spells[2].targettype = TARGET_VARIOUS;	// VARIOUS
		spells[2].instant = false;
		spells[2].cooldown = -1;
		spells[2].perctrigger = 12.0f;
		spells[2].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		SummonTimer = 20;
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Who disturbs this sanctuary?");
		_unit->PlaySoundToSet(11230);
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit *mTarget)
    {
		SummonTimer = 20;
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
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
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You must die! But wait: this does not-- No, no... you must die!");
				_unit->PlaySoundToSet(11231);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "What am I doing? Why do I...");
				_unit->PlaySoundToSet(11232);
				break;
			}
		}
    }

    void OnDied(Unit * mKiller)
    {
		SummonTimer = 20;
		CastTime();
       RemoveAIUpdateEvent();
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "So... confused. Do not... belong here!");
		_unit->PlaySoundToSet(11235);
    }

    void AIUpdate()
	{
		SummonTimer--;

		if (!SummonTimer)	// it will need more work on this spell in future (when this kind of spell will work)
		{
			/*for(int i=0;i<5;i++)
			{
				_unit->CastSpell(_unit, spells[1].info, spells[1].instant);
			}*/
			_unit->CastSpell(_unit, spells[1].info, spells[1].instant);
			SummonTimer = 20;
			SummonSound();
		}

		float val = sRand.rand(100.0f);
		SpellCast(val);
    }

	void SummonSound()
	{
		int RandomSummon;
		RandomSummon=rand()%2;
		switch (RandomSummon)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Children, come to me!");
			_unit->PlaySoundToSet(11233);
			break;
		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Maybe this is not-- No, we fight! Come to my aid!");
			_unit->PlaySoundToSet(11234);
			break;
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

	uint32 SummonTimer;
	int nrspells;
};

void SetupBotanica(ScriptMgr * mgr)
{
    mgr->register_creature_script(CN_BLOOD_PROTECTOR, &BloodProtectorAI::Create);
    mgr->register_creature_script(CN_BLOOD_GREENKEEPER, &BloodGreenkeeperAI::Create);
	mgr->register_creature_script(CN_SUN_CHEMIST, &SunchemistAI::Create);
    mgr->register_creature_script(CN_SUN_RESEARCHER, &SunResearcherAI::Create);
	mgr->register_creature_script(CN_COMMANDER_SARANNIS, &CommanderSarannisAI::Create);
	mgr->register_creature_script(CN_HIGH_BOTANIST_FREYWINN, &HighBotanistFreywinnAI::Create);
	mgr->register_creature_script(CN_THORNGRIN_THE_TENDER, &ThorngrinTheTenderAI::Create);
	mgr->register_creature_script(CN_LAJ, &LajAI::Create);
	mgr->register_creature_script(CN_WARP_SPLINTER, &WarpSplinterAI::Create);
}
