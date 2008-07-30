#include "StdAfx.h"
#include "Setup.h"

/************************************************************************/
/* Instance_HellfireRamparts.cpp Script		                            */
/************************************************************************/

// Bleeding Hollow ArcherAI

#define CN_BLEEDING_HOLLOW_ARCHER 17270

#define MULTI_SHOT 18651
#define AIMED_SHOT 30614
#define DAZED 1604

class BLEEDINGHOLLOWARCHERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(BLEEDINGHOLLOWARCHERAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    BLEEDINGHOLLOWARCHERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
        spells[0].info = sSpellStore.LookupEntry(MULTI_SHOT);
		spells[0].targettype = TARGET_ATTACKING;	// changed from VARIOUS to prevent crashes when caster kills itself by using this ability on low hp
		spells[0].instant = true;
		spells[0].cooldown = 35;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = sSpellStore.LookupEntry(AIMED_SHOT);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].cooldown = 25;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

        spells[2].info = sSpellStore.LookupEntry(DAZED);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].cooldown = 15;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;


    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnTargetDied(Unit* mTarget)
    {
    }

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
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

	int nrspells;
};

// Bleeding Hollow DarkcasterAI

#define CN_BLEEDING_HOLLOW_DARKCASTER 17269

#define SCORCH 36807	// DBC: 15241 // not sure
#define RAIN_OF_FIRE 36808	// DBC: 20754 same here

class BLEEDINGHOLLOWDARKCASTERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(BLEEDINGHOLLOWDARKCASTERAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    BLEEDINGHOLLOWDARKCASTERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = sSpellStore.LookupEntry(SCORCH);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].cooldown = 13;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = sSpellStore.LookupEntry(RAIN_OF_FIRE);
		spells[1].targettype = TARGET_DESTINATION;
		spells[1].instant = false;
		spells[1].cooldown = 30;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;


    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnTargetDied(Unit* mTarget)
    {
    }

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
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

	int nrspells;
};

// Bleeding Hollow ScryerAI

#define CN_BLEEDING_HOLLOW_SCRYER 17478

#define FEAR 30615
#define SHADOW_BOLT_SCRYER 12471 // not sure to those two
#define FEL_INFUSION 30659 // not sure if it's using it

class BLEEDINGHOLLOWSCRYERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(BLEEDINGHOLLOWSCRYERAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    BLEEDINGHOLLOWSCRYERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
        spells[0].info = sSpellStore.LookupEntry(FEAR);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].cooldown = 35;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = sSpellStore.LookupEntry(SHADOW_BOLT_SCRYER);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].cooldown = 10;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

        spells[2].info = sSpellStore.LookupEntry(FEL_INFUSION);
		spells[2].targettype = TARGET_VARIOUS;	// why it targets enemy :O
		spells[2].instant = true;
		spells[2].cooldown = 25;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;


    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnTargetDied(Unit* mTarget)
    {
    }

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
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

	int nrspells;
};

// Bonechewer BeastmasterAI

#define CN_BONECHEWER_BEASTMASTER 17455

#define BATTLE_SHOUT 30635
#define UPPERCUT 10966		// not sure to any spell
#define DAZED_BEASTMASTER 1604
// in a certain ammount of time he calls in about 2-4 more dogs.
class BONECHEWERBEASTMASTERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(BONECHEWERBEASTMASTERAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    BONECHEWERBEASTMASTERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
        spells[0].info = sSpellStore.LookupEntry(BATTLE_SHOUT);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].cooldown = 45;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = sSpellStore.LookupEntry(UPPERCUT);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].cooldown = 30;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

        spells[2].info = sSpellStore.LookupEntry(DAZED_BEASTMASTER);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].cooldown = 15;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;


    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnTargetDied(Unit* mTarget)
    {
    }

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
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

	int nrspells;
};

// Bonechewer DestroyerAI

#define CN_BONECHEWER_DESTROYER 17271

#define MORTAL_STRIKE 15708
#define KNOCK_AWAY 10101
#define SWEEPING_STRIKES 18765	// 12723
#define DAZED_DESTROYER 1604 // it's same everywhere, but I will keep it separated as they are connected with diff creatures

class BONECHEWERDESTROYERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(BONECHEWERDESTROYERAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    BONECHEWERDESTROYERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
        spells[0].info = sSpellStore.LookupEntry(MORTAL_STRIKE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 20;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = sSpellStore.LookupEntry(KNOCK_AWAY);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].cooldown = 35;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

        spells[2].info = sSpellStore.LookupEntry(SWEEPING_STRIKES);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;
		spells[2].cooldown = 40;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;

        spells[3].info = sSpellStore.LookupEntry(DAZED_DESTROYER);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = true;
		spells[3].cooldown = 15;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnTargetDied(Unit* mTarget)
    {
    }

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
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

	int nrspells;
};

// Bonechewer HungererAI

#define CN_BONECHEWER_HUNGERER 17259

#define DEMORALIZING_SHOUT 16244
#define STRIKE 14516
#define DISARM 6713	// no idea if those are correct spells
#define DAZED_HUNGERER 1604

class BONECHEWERHUNGERERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(BONECHEWERHUNGERERAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    BONECHEWERHUNGERERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
        spells[0].info = sSpellStore.LookupEntry(DEMORALIZING_SHOUT);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].cooldown = 40;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = sSpellStore.LookupEntry(STRIKE);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].cooldown = 10;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

        spells[2].info = sSpellStore.LookupEntry(DISARM);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].cooldown = 50;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;

        spells[3].info = sSpellStore.LookupEntry(DAZED_HUNGERER);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = true;
		spells[3].cooldown = 15;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnTargetDied(Unit* mTarget)
    {
    }

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
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

	int nrspells;
};

// Bonechewer RavenerAI

#define CN_BONECHEWER_RAVENER 17264

#define DAZED_RAVENGER 1604
#define KIDNEY_SHOT 30621
// Invisibility and Stealth Detection ? 18950
class BONECHEWERRAVENERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(BONECHEWERRAVENERAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    BONECHEWERRAVENERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
        spells[0].info = sSpellStore.LookupEntry(DAZED_RAVENGER);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 15;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = sSpellStore.LookupEntry(KIDNEY_SHOT);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].cooldown = 25;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnTargetDied(Unit* mTarget)
    {
    }

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
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

	int nrspells;
};

// Bonechewer RipperAI

#define CN_BONECHEWER_RIPPER 17281

#define ENRAGE 18501
#define DAZED_RIPPER 1604

class BONECHEWERRIPPERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(BONECHEWERRIPPERAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    BONECHEWERRIPPERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
        spells[0].info = sSpellStore.LookupEntry(ENRAGE);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].cooldown = 20;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = sSpellStore.LookupEntry(DAZED_RIPPER);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].cooldown = 15;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnTargetDied(Unit* mTarget)
    {
    }

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
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

	int nrspells;
};

// Fiendish HoundAI

#define CN_FIENDISH_HOUND 17540

#define DRAIN_LIFE 35748

class FIENDISHHOUNDAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(FIENDISHHOUNDAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    FIENDISHHOUNDAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
        spells[0].info = sSpellStore.LookupEntry(DRAIN_LIFE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].cooldown = 25;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnTargetDied(Unit* mTarget)
    {
    }

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
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

	int nrspells;
};

// Hellfire WatcherAI

#define CN_HELLFIRE_WATCHER 17309

#define SHADOW_WORD_PAIN 14032
#define HEAL 30643	// DBC: 12039, 30643
#define RENEW 37260 // DBC: 8362; no idea if those are correct spells // uses it also on enemy :O // DBC: 8362
// renew? other heal?

class HELLFIREWATCHERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HELLFIREWATCHERAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    HELLFIREWATCHERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
        spells[0].info = sSpellStore.LookupEntry(SHADOW_WORD_PAIN);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 30;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = sSpellStore.LookupEntry(HEAL);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = false;
		spells[1].cooldown = 45;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

        spells[2].info = sSpellStore.LookupEntry(RENEW);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].cooldown = 60;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnTargetDied(Unit* mTarget)
    {
    }

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
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

	int nrspells;
};

// Shattered Hand WarhoundAI

#define CN_SHATTERED_HAND_WARHOUND 17280

#define CARNIVOROUS_BITE 30639
#define DAZED_WARHOUND 1604
//#define FURIOUS_HOWL 30636
// Invisibility and Stealth Detection 18950 ? && Increase Spell Dam 43 17280 ?
class SHATTEREDHANDWARHOUNDAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SHATTEREDHANDWARHOUNDAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    SHATTEREDHANDWARHOUNDAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
        spells[0].info = sSpellStore.LookupEntry(CARNIVOROUS_BITE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 35;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = sSpellStore.LookupEntry(DAZED_WARHOUND);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].cooldown = 15;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;
/*
        spells[2].info = sSpellStore.LookupEntry(FURIOUS_HOWL);	// crashes server :O
		spells[2].targettype = TARGET_VARIOUS;
		spells[2].instant = true;
		spells[2].cooldown = 20;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;
*/
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnTargetDied(Unit* mTarget)
    {
    }

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
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

	int nrspells;
};

/*****************************/
/*                           */
/*         Boss AIs          */
/*                           */
/*****************************/

// Watchkeeper GargolmarAI

#define CN_WATCHKEEPER_GARGOLMAR 17306

#define SURGE 34645 // 25787 not sure
#define MORTAL_WOUND 30641 // 25646
#define	OVERPOWER 32154	// I am not sure about this spell and those down there
#define RETALIATION 22857 // DBC: 22857, 22858

class WATCHKEEPERGARGOLMARAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(WATCHKEEPERGARGOLMARAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    WATCHKEEPERGARGOLMARAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		HEAL_SOUND_LIMITER = 0;
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
        spells[0].info = sSpellStore.LookupEntry(SURGE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 40;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].speech = "Back off, pup!";
		spells[0].soundid = 10330;

        spells[1].info = sSpellStore.LookupEntry(MORTAL_WOUND);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].cooldown = 35;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

        spells[2].info = sSpellStore.LookupEntry(OVERPOWER);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].cooldown = 10;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;

        spells[3].info = sSpellStore.LookupEntry(RETALIATION);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = true;
		spells[3].cooldown = 45;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		HEAL_SOUND_LIMITER = 0;
		int RandomSpeach;
		sRand.randInt(1000);
		RandomSpeach=rand()%3;
		switch (RandomSpeach)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "What do we have here? ...");
			_unit->PlaySoundToSet(10331);
			break;
		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Heh... this may hurt a little.");
			_unit->PlaySoundToSet(10332);
			break;
		case 2:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I'm gonna enjoy this!");
			_unit->PlaySoundToSet(10333);
			break;
		}
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
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
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Say farewell!");
				_unit->PlaySoundToSet(10334);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Much too easy!");
				_unit->PlaySoundToSet(10335);
				break;
			}
		}
    }

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
		HEAL_SOUND_LIMITER = 0;
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
		HEAL_SOUND_LIMITER = 0;
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Hah..."); // needs corrections!
		_unit->PlaySoundToSet(10336);
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		if (_unit->GetHealthPct() <= 40 && !HEAL_SOUND_LIMITER)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Heal me! QUICKLY!");
			_unit->PlaySoundToSet(10329);
			HEAL_SOUND_LIMITER = 1;
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

	int HEAL_SOUND_LIMITER;
	int nrspells;
};

// Omor the UnscarredAI

#define CN_OMOR_THE_UNSCARRED 17308

#define DEMONIC_SHIELD 31901
#define SUMMON_FIENDISH_HOUND 30707 // lack of core support
#define SHADOW_WIP 30638
#define TREACHEROUS_AURA 30695
#define BANE_OF_TREACHERY 37566 // it's heroic spell which replaces treacherous aura
#define SHADOW_BOLT 30686 // 30686, 31627, 31618, 31627 and many other possibilities
#define ORBITAL_STRIKE 30637 // Not sure here (if it uses this spell)
// TO DO: Add sound *Knock1* with spell if needed (I think it's for Orbital Strike)
// as it has knockdown effect

class OMORTHEUNSCARREDAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(OMORTHEUNSCARREDAI);
	SP_AI_Spell spells[7];
	bool m_spellcheck[7];

    OMORTHEUNSCARREDAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		DEMONIC_SHIELDCooldown = 0; // not sure if this should be casted once or after some time
		nrspells = 7;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
        spells[0].info = sSpellStore.LookupEntry(DEMONIC_SHIELD);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = sSpellStore.LookupEntry(SUMMON_FIENDISH_HOUND);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = false;
		spells[1].cooldown = 20;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;
		spells[1].speech = "<missing_text>";
		spells[1].soundid = 10277;

        spells[2].info = sSpellStore.LookupEntry(SHADOW_WIP);
		spells[2].targettype = TARGET_ATTACKING;	// should be random target
		spells[2].instant = true;
		spells[2].cooldown = 30;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;

        spells[3].info = sSpellStore.LookupEntry(TREACHEROUS_AURA);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = false;
		spells[3].cooldown = 35;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;

        spells[4].info = sSpellStore.LookupEntry(BANE_OF_TREACHERY);
		spells[4].targettype = TARGET_ATTACKING;
		spells[4].instant = false;
		spells[4].cooldown = -1;
		spells[4].perctrigger = 0.0f;
		spells[4].attackstoptimer = 1000;

        spells[5].info = sSpellStore.LookupEntry(SHADOW_BOLT);
		spells[5].targettype = TARGET_ATTACKING;
		spells[5].instant = false;
		spells[5].cooldown = 15;
		spells[5].perctrigger = 0.0f;
		spells[5].attackstoptimer = 1000;

        spells[6].info = sSpellStore.LookupEntry(ORBITAL_STRIKE);
		spells[6].targettype = TARGET_ATTACKING;
		spells[6].instant = false;
		spells[6].cooldown = 70;
		spells[6].perctrigger = 0.0f;
		spells[6].attackstoptimer = 1000;
		spells[6].speech = "<missing_text>";
		spells[6].soundid = 10278;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		DEMONIC_SHIELDCooldown = 0;
		_unit->CastSpell(_unit, spells[1].info, spells[1].instant);
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		int RandomSpeach;
		sRand.randInt(1000);
		RandomSpeach=rand()%3;
		switch (RandomSpeach)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I will not be... defeated!");
			_unit->PlaySoundToSet(10279);
			break;
		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You dare stand against me?");	// ofc they need corrections ;)
			_unit->PlaySoundToSet(10280);
			break;
		case 2:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your incidents will be your death!");
			_unit->PlaySoundToSet(10281);
			break;
		}
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
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
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Die weakling!");
				_unit->PlaySoundToSet(10282);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I am victorious!");
				_unit->PlaySoundToSet(10283);
				break;
			}
		}
    }

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
		DEMONIC_SHIELDCooldown = 0;
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "It is... not... over...");
		_unit->PlaySoundToSet(10284);
		DEMONIC_SHIELDCooldown = 0;
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		DEMONIC_SHIELDCooldown--;
		if (_unit->GetHealthPct() <= 20 && DEMONIC_SHIELDCooldown <= 0)
		{
			_unit->CastSpell(_unit, spells[0].info, spells[0].instant);
			DEMONIC_SHIELDCooldown = 30;
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

	int DEMONIC_SHIELDCooldown;
	int nrspells;
};

/*****************************/
/*                           */
/*    Vazruden the Herald	 */
/*			*Full Event*	 */
/*                           */
/*****************************/

// EVENT DEFINITIONS/VARIABLES
uint32 VAZRUDEN_THE_HERALD_EVENT_PHASE;
uint32 HERALDS_SENTRY_DEATHS;	

// Hellfire Sentry AI - mob

#define CN_HELLFIRE_SENTRY 17517

#define KIDNEY_SHOT_SENTRY 30621
// Not sure!
class HELLFIRESENTRYAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HELLFIRESENTRYAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    HELLFIRESENTRYAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		
        spells[0].info = sSpellStore.LookupEntry(KIDNEY_SHOT_SENTRY);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 25;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnTargetDied(Unit* mTarget)
    {
    }

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
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

	int nrspells;
};

// VazrudenAI

#define CN_VAZRUDEN 17537

#define REVENGE 40392 // DBC: 19130; 37517, 40392, 19130 or other

class VAZRUDENAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(VAZRUDENAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    VAZRUDENAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		PHASE_LIMITER = 2;
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
        spells[0].info = sSpellStore.LookupEntry(REVENGE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 10;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		PHASE_LIMITER = 2;
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnTargetDied(Unit* mTarget)
    {
    }

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
		PHASE_LIMITER = 2;
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		if (_unit->GetHealthPct() <= 20 && PHASE_LIMITER == 2)
		{
			VAZRUDEN_THE_HERALD_EVENT_PHASE = 3;
			PHASE_LIMITER = 3;
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

	uint32 PHASE_LIMITER;
	int nrspells;
};

// NazanAI
								// DOESN'T WORK YET!
#define WALK 0
#define RUN 256
#define FLY 768

//#define CN_NAZAN 17536
#define CN_NAZAN 17307 // 17307 = 17536 + 17537; VAZRUDEN THE HERALD = NAZAN (MOUNT) + VAZRUDEN
// Event: Phase1 spells
#define SUMMON_VAZRUDEN 30717
// Event: Phase2 spells
#define FIREBALL 30691	// DBC: 34653, 30691
#define LIQUID_FLAME 1
#define CONE_OF_FIRE 30926
// FACE HIGHEST THREAT 30700
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
	{ -1413.410034, 1744.969971,  80.900000, 0.147398 },	// fly starting position
	{ -1413.410034, 1744.969971,  92.948196, 0.147398 },	// fly !
	{ -1378.454712, 1687.340332, 110.200218, 1.017074 },	// 1 full fly in circle wp
	{ -1352.973145, 1726.131470, 110.408745, 1.297234 },	// 2
    { -1362.943970, 1767.925415, 110.101616, 5.212438 },	// 3
	{ -1415.544189, 1804.141357, 110.075363, 5.974271 },	// 4
	{ -1461.189575, 1780.554932, 110.854507, 0.460774 },	// 5
	{ -1482.489380, 1718.727783, 110.248772, 5.847037 },	// 6
	{ -1418.811646, 1676.112427, 110.405968, 0.231439 },	// 7
	{ -1413.408203, 1744.974121,  92.000000, 0.147398 }		// land place
};
//	{ -1413.410034, 1744.969971,  80.948196, 0.147398 },	// fly starting position

class NAZANAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(NAZANAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    NAZANAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		VAZRUDEN_THE_HERALD_EVENT_PHASE = 1;
		HERALDS_SENTRY_DEATHS = 0;
        m_entry = pCreature->GetEntry();
        m_useSpell = false;
        m_FireballCooldown = 5;
        m_ConeOfFireCooldown = 0;

		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(1, 2000, RUN));	//RUN
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(2, 0, FLY));	// FLY START!
        _unit->GetAIInterface()->addWayPoint(CreateWaypoint(3, 0, FLY));
        _unit->GetAIInterface()->addWayPoint(CreateWaypoint(4, 0, FLY));
        _unit->GetAIInterface()->addWayPoint(CreateWaypoint(5, 0, FLY));
        _unit->GetAIInterface()->addWayPoint(CreateWaypoint(6, 0, FLY));
        _unit->GetAIInterface()->addWayPoint(CreateWaypoint(7, 0, FLY));
        _unit->GetAIInterface()->addWayPoint(CreateWaypoint(8, 0, FLY));
        _unit->GetAIInterface()->addWayPoint(CreateWaypoint(9, 0, FLY));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(10, 1000, FLY));	// SPECIAL WP! (FOR VAZRUDEN LANDING!)

        //infoFireball = sSpellStore.LookupEntry(FIREBALL);
        infoLiquidFlame = sSpellStore.LookupEntry(LIQUID_FLAME);
        //infoConeOfFire = sSpellStore.LookupEntry(CONE_OF_FIRE);
		infoSummonVazruden = sSpellStore.LookupEntry(SUMMON_VAZRUDEN);

		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = sSpellStore.LookupEntry(FIREBALL);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].cooldown = 10;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = sSpellStore.LookupEntry(CONE_OF_FIRE);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].cooldown = 20;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

        if(/*!infoFireball || */!infoLiquidFlame ||/* !infoConeOfFire || */!infoSummonVazruden)
            m_useSpell = false;

        _unit->GetAIInterface()->setOutOfCombatRange(200000);

		VAZRUDEN_LAND = false;
		NAZAN_LAND = false;
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
        _unit->GetAIInterface()->StopMovement(0);
        _unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
		_unit->GetAIInterface()->setWaypointToMove(1);

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit *mTarget)	// not finished yet, as I must force Nazan to go into combat state
    {
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		if (VAZRUDEN_THE_HERALD_EVENT_PHASE == 3)
		{
			VAZRUDEN_THE_HERALD_EVENT_PHASE = 1;
			HERALDS_SENTRY_DEATHS = 0;
			_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			_unit->GetAIInterface()->setWaypointToMove(1);
			VAZRUDEN_LAND = false;
			NAZAN_LAND = false;
		}

		if (VAZRUDEN_THE_HERALD_EVENT_PHASE == 2)
		{
			HERALDS_SENTRY_DEATHS = 0;
			VAZRUDEN_THE_HERALD_EVENT_PHASE = 1;
			VAZRUDEN_LAND = false;
		}

        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		VAZRUDEN_THE_HERALD_EVENT_PHASE = 1;
		HERALDS_SENTRY_DEATHS = 0;
		VAZRUDEN_LAND = false;
		NAZAN_LAND = false;
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		switch (VAZRUDEN_THE_HERALD_EVENT_PHASE)
		{
		case 1: 
			{
				PhaseOne();
			}break;
		case 2:
			{
				PhaseTwo();
			}break;
		case 3: 
			{
				PhaseThree();
			}break;
		default:
			{
				VAZRUDEN_THE_HERALD_EVENT_PHASE = 1;
			};
		};
		//float val = sRand.rand(100.0f);
		//SpellCast(val);
    }

    void PhaseOne()
    {
    }

	void PhaseTwo()
	{
		m_useSpell = true;
		m_FireballCooldown--;
        if(!m_FireballCooldown && _unit->GetAIInterface()->GetNextTarget())
        {
            _unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), infoFireball, false);
            m_FireballCooldown = 5;
        }
	}

    void PhaseThree()
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

    void Fly()
    {
        _unit->Emote(EMOTE_ONESHOT_LIFTOFF);
        _unit->GetAIInterface()->m_moveFly = true;
    }

    void Land()
    {
        _unit->Emote(EMOTE_ONESHOT_LAND);
        _unit->GetAIInterface()->m_moveFly = false;
    }

    void OnReachWP(uint32 iWaypointId, bool bForwards)
    {
		if (HERALDS_SENTRY_DEATHS == 2 && VAZRUDEN_LAND == false) /*VAZRUDEN_THE_HERALD_EVENT_PHASE == 2*/
		{
			VAZRUDEN_THE_HERALD_EVENT_PHASE = 2;
			VAZRUDEN_LAND = true;
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
            _unit->GetAIInterface()->setWaypointToMove(10);
		}

		if (VAZRUDEN_THE_HERALD_EVENT_PHASE == 3 && NAZAN_LAND == false)
		{
			NAZAN_LAND = true;
            _unit->SetFloatValue(UNIT_MOD_CAST_SPEED, 1.0f);
            if(_unit->GetCurrentSpell() != NULL)				// these 3 lines added from Onyxia script
                _unit->GetCurrentSpell()->cancel();
            _unit->GetAIInterface()->m_canMove = true;
            _unit->GetAIInterface()->SetAllowedToEnterCombat(false);
            //_unit->GetAIInterface()->StopMovement(0);	// commented? this crap "despawns" creature
            _unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			_unit->GetAIInterface()->setWaypointToMove(2);
			if (_unit->GetAIInterface()->GetNextTarget())
			{
				Unit *target = NULL;
				target = _unit->GetAIInterface()->GetNextTarget();
				_unit->CastSpell(target, spells[1].info, spells[1].instant);
			}
		}

		else
		{
			switch(iWaypointId)
			{
			case 1:
				{
					if (VAZRUDEN_THE_HERALD_EVENT_PHASE == 3)
					{
						_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
						_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
						_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
						_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
						_unit->GetAIInterface()->setWaypointToMove(0);
						WorldPacket data(SMSG_MOVE_UNSET_HOVER, 13);
						data << _unit->GetNewGUID();
						data << uint32(0);
						_unit->SendMessageToSet(&data, false);
						Land();

					}

					else
					{
						_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
						_unit->GetAIInterface()->setWaypointToMove(2);
						Fly();
					}
				}break;

			case 2:
				{
					if (VAZRUDEN_THE_HERALD_EVENT_PHASE == 3)
					{
						_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
						_unit->GetAIInterface()->setWaypointToMove(1);
					}

					else
					{
						_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
						_unit->GetAIInterface()->setWaypointToMove(3);	
					}
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
			        _unit->GetAIInterface()->setWaypointToMove(6);
					/*
			        _unit->GetAIInterface()->m_canMove = false;
			        _unit->GetAIInterface()->SetAllowedToEnterCombat(true);
			        _unit->GetAIInterface()->setCurrentAgent(AGENT_SPELL);
			        //_unit->m_pacified--;
			        _unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
			        _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
			        _unit->GetAIInterface()->setWaypointToMove(0);
			        WorldPacket data(SMSG_MOVE_SET_HOVER, 13);
			        data << _unit->GetNewGUID();
			        data << uint32(0);
			        _unit->SendMessageToSet(&data, false);
			        m_currentWP = 3;*/
			    }break;

			case 6:
			    {
			        _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			        _unit->GetAIInterface()->setWaypointToMove(7);
			    }break;

			case 7:
			    {
			        _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			        _unit->GetAIInterface()->setWaypointToMove(8);
			    }break;

			case 8:
			    {
			        _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			        _unit->GetAIInterface()->setWaypointToMove(9);
			    }break;

			case 9:
			    {
			        _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			        _unit->GetAIInterface()->setWaypointToMove(3);
			    }break;

			case 10:
			    {
					if (VAZRUDEN_THE_HERALD_EVENT_PHASE == 3)
					{
						/*_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
						_unit->GetAInterface()->setCurrentAgent(AGENT_NULL);
						_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
						_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
						//_unit->GetAIInterface()->setWaypointToMove(0);
						//_unit->GetAIInterface()->m_canMove = true;
						//_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
						//_unit->GetAIInterface()->StopMovement(0);
						//_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
						//_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
						//_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
						Land();
						m_useSpell = true;
		                _unit->GetAIInterface()->SetAllowedToEnterCombat(true);
						_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
						_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
						_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
						_unit->GetAIInterface()->setWaypointToMove(0);
		                WorldPacket data(SMSG_MOVE_UNSET_HOVER, 13);
						data << _unit->GetNewGUID();
						data << uint32(0);
						_unit->SendMessageToSet(&data, false);
						Land();*/
					    //_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
						//_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
						/*nit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);*/	// this shit messes all =/ // SCRIPTIDLE
						_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
						_unit->GetAIInterface()->setWaypointToMove(9);
						/*_unit->m_pacified--;
						if(_unit->m_pacified > 0)
					    _unit->m_pacified--;*/
						/*WorldPacket data(SMSG_MOVE_UNSET_HOVER, 13);
						data << _unit->GetNewGUID();
						data << uint32(0);
						_unit->SendMessageToSet(&data, false);*/ // Hover disabled for now
						//_unit->GetAIInterface()->m_moveFly = false;
						Land();
						/* working partially
						_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
						_unit->GetAIInterface()->setWaypointToMove(0);
						Land();
						*/
					}

					if (VAZRUDEN_THE_HERALD_EVENT_PHASE == 2)
					{
						m_useSpell = true;
						_unit->CastSpell(_unit, infoSummonVazruden, true);
						_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
						_unit->GetAIInterface()->setWaypointToMove(3);
					}

				}break;

			default:
				{
                _unit->GetAIInterface()->m_canMove = false;
                _unit->GetAIInterface()->SetAllowedToEnterCombat(true);
                _unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
                _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
                _unit->GetAIInterface()->setWaypointToMove(0);
                WorldPacket data(SMSG_MOVE_SET_HOVER, 13);
                data << _unit->GetNewGUID();
                data << uint32(0);
                _unit->SendMessageToSet(&data, false);
				}break;
			}
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

    uint32 m_entry;
    bool m_useSpell;
	bool VAZRUDEN_LAND;
	bool NAZAN_LAND;
    uint32 m_FireballCooldown;
    uint32 m_ConeOfFireCooldown;
    uint32 m_fCastCount;
    uint32 m_currentWP;
    SpellEntry *infoConeOfFire, *infoLiquidFlame, *infoFireball, *infoSummonVazruden;
	int nrspells;
};

void SetupHellfireRamparts(ScriptMgr * mgr)
{
	mgr->register_creature_script(CN_BLEEDING_HOLLOW_ARCHER, &BLEEDINGHOLLOWARCHERAI::Create);
	mgr->register_creature_script(CN_BLEEDING_HOLLOW_DARKCASTER, &BLEEDINGHOLLOWDARKCASTERAI::Create);
	mgr->register_creature_script(CN_BLEEDING_HOLLOW_SCRYER, &BLEEDINGHOLLOWSCRYERAI::Create);
	mgr->register_creature_script(CN_BONECHEWER_BEASTMASTER, &BONECHEWERBEASTMASTERAI::Create);	
	mgr->register_creature_script(CN_BONECHEWER_DESTROYER, &BONECHEWERDESTROYERAI::Create);	
	mgr->register_creature_script(CN_WATCHKEEPER_GARGOLMAR, &WATCHKEEPERGARGOLMARAI::Create);
	mgr->register_creature_script(CN_BONECHEWER_HUNGERER, &BONECHEWERHUNGERERAI::Create);
	mgr->register_creature_script(CN_BONECHEWER_RAVENER, &BONECHEWERRAVENERAI::Create);
	mgr->register_creature_script(CN_BONECHEWER_RIPPER, &BONECHEWERRIPPERAI::Create);
	mgr->register_creature_script(CN_FIENDISH_HOUND, &FIENDISHHOUNDAI::Create);
	mgr->register_creature_script(CN_HELLFIRE_WATCHER, &HELLFIREWATCHERAI::Create);
	mgr->register_creature_script(CN_SHATTERED_HAND_WARHOUND, &SHATTEREDHANDWARHOUNDAI::Create);
	mgr->register_creature_script(CN_OMOR_THE_UNSCARRED, &OMORTHEUNSCARREDAI::Create);
	mgr->register_creature_script(CN_HELLFIRE_SENTRY, &HELLFIRESENTRYAI::Create);
	mgr->register_creature_script(CN_VAZRUDEN, &VAZRUDENAI::Create);
	mgr->register_creature_script(CN_NAZAN, &NAZANAI::Create);
}
