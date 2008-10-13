/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008 Arcemu Team
 * Copyright (C) 2007 Moon++ <http://www.moonplusplus.com/>
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

/************************************************************************/
/* Instance_Deadmines.cpp Script										*/
/************************************************************************/

// Rhakzor 
#define rhk_slam 6304
class RhahkZorAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(RhahkZorAI);
    RhahkZorAI(Creature* pCreature) : CreatureAIScript(pCreature) 
	{
		slam = dbcSpell.LookupEntry(rhk_slam);
	}

    void OnDied(Unit * mKiller)
    {
        // Find "Factory Door"
        // GetGameObjectNearestCoords works here too.
		RemoveAIUpdateEvent();

        GameObject * pDoor = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(
           -190.860092f, -456.332184f, 54.496822f, 13965);
        if(pDoor == 0)
            return;

        // Open the door
        pDoor->SetUInt32Value(GAMEOBJECT_STATE, 0);
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnCombatStart(Unit* mTarget)
    {
        _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Van Cleef pay big for your heads!");
        _unit->PlaySoundToSet(5774);
		timer = 10000 + RandomUInt(4000);
		RegisterAIUpdateEvent(timer);
    }

    void AIUpdate()
	{
		timer = 10000 + RandomUInt(4000);

		Unit *target = NULL;
		target = _unit->GetAIInterface()->GetNextTarget();
		_unit->CastSpell(target, slam, true);
		RemoveAIUpdateEvent();
		RegisterAIUpdateEvent(timer);
	}

    void Destroy()
    {
        delete (RhahkZorAI*)this;
    }

protected:
	SpellEntry *slam;
	int timer;
};


// Sneeds Shredder -> spawn sneed on death.

#define terrify 7399
#define distracting_pain 3603
class SneedsShredderAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(SneedsShredderAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    SneedsShredderAI(Creature* pCreature) : CreatureAIScript(pCreature) 
	{
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
        spells[0].info = dbcSpell.LookupEntry(distracting_pain);
		spells[0].targettype = TARGET_ATTACKING;	
		spells[0].instant = true;
		spells[0].cooldown = 6;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 500;

        fear = dbcSpell.LookupEntry(terrify);
	}

    void OnDied(Unit * mKiller)
    {
		RemoveAIUpdateEvent();
		spawnposx = _unit->GetPositionX();
		spawnposy = _unit->GetPositionY();
		spawnposz = _unit->GetPositionZ();
		spawnfacing = _unit->GetOrientation();

		_unit->GetMapMgr()->GetInterface()->SpawnCreature(643, spawnposx, spawnposy, spawnposz, spawnfacing, true, false, 0, 0);
    }

    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));    // it is 1500
	}

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void Destroy()
    {
        delete (SneedsShredderAI*)this;
    }

	void AIUpdate()
	{
		fearcount++;
		timer = 12 + RandomUInt(3);
		if(fearcount >= timer && _unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			Unit *plr = NULL;
			plr = _unit->GetAIInterface()->GetNextTarget();
			_unit->CastSpell(plr, fear, true);
			fearcount = 0;
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
					case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant);
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

protected:
	float spawnposx, spawnposy, spawnposz, spawnfacing;
	int nrspells, fearcount;
	int timer;
	SpellEntry * fear;
};

// Sneed -> open door on death.

class SneedAI : public CreatureAIScript
{
public:
    SneedAI(Creature* pCreature) : CreatureAIScript(pCreature) {}
    void OnDied(Unit * mKiller)
    {
        // Find "Mast Room Door"
        // GetGameObjectNearestCoords works here too.

        GameObject * pDoor = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(
            -289.691650f, -535.988953f, 49.440678f, 16400);
        if(pDoor == 0)
            return;

        // Open the door
        pDoor->SetUInt32Value(GAMEOBJECT_STATE, 0);
    }

    void Destroy()
    {
        delete (SneedAI*)this;
    }

    static CreatureAIScript * Create(Creature * c) { return new SneedAI(c); }
};


// Gilnid -> Opens Foundary door on death.
#define molten_metal 5213        
#define melt_ore 5159       
class GilnidAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(GilnidAI);
    GilnidAI(Creature* pCreature) : CreatureAIScript(pCreature) 
	{
	molten = dbcSpell.LookupEntry(molten_metal);
	melt = dbcSpell.LookupEntry(melt_ore);
	}
	
    void OnDied(Unit * mKiller)
    {
		RemoveAIUpdateEvent();
		
		// Find "Foundry Door"
        // GetGameObjectNearestCoords works here too.
		GameObject * pDoor = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-176.569f, -577.640991f, 19.311600f, 16399);
        if(pDoor == 0)
            return;

        // Open the door
        pDoor->SetUInt32Value(GAMEOBJECT_STATE, 0);
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
	
	void AIUpdate()
	{
		meltcount++;
		moltencount++;
		timer1 = 12 + RandomUInt(3);
		timer2 = 17 + RandomUInt(3);

		if(moltencount >= timer1 && _unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			Unit *plr = NULL;
			plr = _unit->GetAIInterface()->GetNextTarget();
			_unit->CastSpell(plr, molten, true);
			moltencount = 0;
		}
		if(meltcount >= timer2 && _unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			Unit *plr = NULL;
			plr = _unit->GetAIInterface()->GetNextTarget();
			_unit->CastSpell(plr, melt, true);
			meltcount = 0;
		}
	}

    void Destroy()
    {
        delete (GilnidAI*)this;
    }

protected:
	SpellEntry * molten;
	SpellEntry * melt;
	int timer1, timer2;
	int meltcount, moltencount;
};

// vancleef


class VanCleefAI : public CreatureAIScript
{
public:
    VanCleefAI(Creature *pCreature) : CreatureAIScript(pCreature)
    {
        mPhase = 0;
    }

    void OnCombatStart(Unit* mTarget)
    {
        _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "None may challenge the brotherhood.");
        _unit->PlaySoundToSet(5780);    // EdwinVanCleefAggro01.wav
    }

    void OnTargetDied(Unit* mTarget)
    {
        char msg[200];
        if(mTarget->GetTypeId() == TYPEID_PLAYER)
            sprintf(msg, "And stay down, %s.", static_cast<Player*>(mTarget)->GetName());
        else if (mTarget->GetTypeFromGUID() == HIGHGUID_TYPE_PET)
            sprintf(msg, "And stay down, %s.", static_cast<Pet*>(mTarget)->GetName().c_str());

        _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, msg);
        _unit->PlaySoundToSet(5781);    // EdwinVanCleefSlay01.wav
    }

    void OnDamageTaken(Unit* mAttacker, float fAmount)
    {
        if(fAmount < 5) return;

        // <100% hp -> We go to phase 1
        if(_unit->GetHealthPct() <= 75 && mPhase == 0) {
            ChangeToPhase1();
        }

        // <67% hp -> We go to phase 2
        if(_unit->GetHealthPct() <= 50 && mPhase == 1) {
            ChangeToPhase2();
        }

        // <34% hp -> We go to phase 3
        if(_unit->GetHealthPct() <= 25 && mPhase == 2) {
            ChangeToPhase3();
        }
    }

    void ChangeToPhase1()
    {
        // Set phase var
        mPhase = 1;
        
        // Play the sound, and send text.
        _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Lap dogs, all of you.");
        _unit->PlaySoundToSet(5782);    // EdwinVanCleefHealth01.wav
    }

    void ChangeToPhase2()
    {
        // Set phase var
        mPhase = 2;

        // Play sound, and send text.
        _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Fools! Our cause is righteous.");
        _unit->PlaySoundToSet(5783);    // EdwinVanCleefHealth02.wav
    }

    void ChangeToPhase3()
    {
        // Set phase var
        mPhase = 3;

        // Play sound, and send text.
        _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "The brotherhood shall remain.");
        _unit->PlaySoundToSet(5784);    // EdwinVanCleefHealth03.wav
    }

    void Destroy()
    {
        delete (VanCleefAI*)this;
    }

    static CreatureAIScript * Create(Creature * c) { return new VanCleefAI(c); }

protected:
    uint32 mPhase;  // NPC has 3 phases
};

// mr.smite
#define stomp 6432
#define hammer_buff 6436
class MrSmiteAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(MrSmiteAI);

    MrSmiteAI(Creature * pCreature) : CreatureAIScript(pCreature) 
	{
		smite_stomp = dbcSpell.LookupEntry(stomp);
		smite_buff = dbcSpell.LookupEntry(hammer_buff);
	}

    void OnCombatStart(Unit * mTarget)
	{
        phase = 0;
		move = 0;
		looting = 0;
		position = 0;
    }

    void OnDamageTaken(Unit * mAttacker, float fAmount)
    {
        if(_unit->GetHealthPct() <= 66 && phase == 0)
        {
			Playr = _unit->GetAIInterface()->GetNextTarget();

            _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You landlubbers are tougher than I thought. I'll have to improvise!");
            _unit->PlaySoundToSet(5778);
			_unit->CastSpell(Playr, smite_stomp, true);
            phase++;
			move = 1;
			position = 1;
            RegisterAIUpdateEvent(500);
        }
        else if(_unit->GetHealthPct() <= 33 && phase == 1)
        {
			Playr = _unit->GetAIInterface()->GetNextTarget();

            _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "D'ah! Now you're making me angry!");
            _unit->PlaySoundToSet(5779);
			_unit->CastSpell(Playr, smite_stomp, true);
            phase++;
			move = 1;
			position = 1;
            RegisterAIUpdateEvent(500);
        }
    }

    void OnCombatStop(Unit * mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
        phase = 0;
		looting = 0;
		move = 0;
		position = 0;
        weapons();
    }

    void OnDied(Unit * mKiller)
    {
        phase = 0;
		looting = 0;
		move = 0;
		position = 0;
        RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		if(looting == 1)
		{
			looting = 0;
			weapons();
			//_unit->Emote(EMOTE_ONESHOT_NONE);
			//_unit->SetStandState(0);
			_unit->GetAIInterface()->SetAIState(STATE_ATTACKING);
		}

		if(_unit->GetPositionX() == 1.100060f && _unit->GetPositionY() == -780.026367f && _unit->GetPositionZ() == 9.811194f && position == 1)
		{
	        //_unit->Emote(EMOTE_ONESHOT_KNEEL);
			//_unit->SetStandState(8);
			looting = 1;
			position = 0;
			ModifyAIUpdateEvent(2500);
		}

		if(move == 1)
		{
			move = 0;
			movetochest();
		}
    }

	void movetochest()
	{	
		_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
		_unit->GetAIInterface()->MoveTo(1.100060f, -780.026367f, 9.811194f, 5.452218f);
	}

    void weapons()
    {	
		// CREDITS to Skyboat on Arcemuemu.com/forums  he had some of this info on one of his releases
        switch(phase)
        {
        case 0:
            _unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY, 7420);
            _unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO, 33490690);         // no idea where to get it :S
            _unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_01, 768);			 // no idea where to get it :S
            _unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY_01, 0);
            _unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_02, 0);
            _unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_03, 0);
			_unit->SetUInt32Value(UNIT_FIELD_BASEATTACKTIME, 1483);   // 1483 is taken from NCDB creature_proto
            break;
        case 1:
            _unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY_01, 7419);
            _unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_02, 33490690);				// no idea where to get it :S
            _unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_03, 781);					// no idea where to get it :S
			_unit->SetUInt32Value(UNIT_FIELD_BASEATTACKTIME, _unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME) / 2);
            break;
        case 2:
            _unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY, 19610);
            _unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO, 33488898);			    // no idea where to get it :S
            _unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_01, 0);
            _unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY_01, 0);
            _unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_02, 0);
            _unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_03, 0);
			_unit->SetUInt32Value(UNIT_FIELD_BASEATTACKTIME, _unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME) * 2);
			_unit->CastSpell(_unit, smite_buff, true);
        }
        RemoveAIUpdateEvent();
    }

protected:
	SpellEntry * smite_stomp;
	SpellEntry * smite_buff;
	Unit * Playr;
    int phase , move, looting, position;
};


// cookie
#define acid_splash 6306
#define cookie_cooking 5174

class CookieAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(CookieAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    CookieAI(Creature* pCreature) : CreatureAIScript(pCreature) 
	{
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(acid_splash);
		spells[0].targettype = TARGET_ATTACKING;	
		spells[0].instant = true;
		spells[0].cooldown = 6;
		spells[0].perctrigger = 12.0f;
		spells[0].attackstoptimer = 100;

		heal = dbcSpell.LookupEntry(cookie_cooking);
	}

	void OnCombatStart(Unit* mTarget) 
	{
		RegisterAIUpdateEvent(1500);
	}

	void OnCombatStop(Unit* mTarget) 
	{
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
	}

	void OnDied(Unit *mKiller) 
	{
        RemoveAIUpdateEvent();
	}

	void AIUpdate() 
	{
		healcount++;
		timer = 13 + RandomUInt(3);

		if(healcount >= timer && _unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			if(_unit->GetHealthPct() <= 90)
			{
				_unit->CastSpell(_unit, heal, false);
			}
			healcount = 0;
		}
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
	}

    void Destroy()
    {
        delete (CookieAI*)this;
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
					case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant);
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
protected:
	int nrspells,healcount;
	int timer;
		SpellEntry * heal;
};

void SetupDeadmines(ScriptMgr * mgr)
{
	mgr->register_creature_script(645, &CookieAI::Create);
	mgr->register_creature_script(643, &SneedAI::Create);
    mgr->register_creature_script(644, &RhahkZorAI::Create);
    mgr->register_creature_script(642, &SneedsShredderAI::Create);
    mgr->register_creature_script(1763, &GilnidAI::Create);
    mgr->register_creature_script(639, &VanCleefAI::Create);
    mgr->register_creature_script(646, &MrSmiteAI::Create);
}
