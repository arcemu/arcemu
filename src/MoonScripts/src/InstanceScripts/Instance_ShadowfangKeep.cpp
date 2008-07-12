/*
 * Moon++ Scripts for arcemu MMORPG Server
 * Copyright (C) 2005-2007 arcemu Team <http://www.arcemuemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
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

/*********************************************************************************/
/* Instance_ShadowfangKeep.cpp Script											*/
/*********************************************************************************/

// Commander Springvale 
#define CN_SPRINGVALE 4278
#define DEVOTION_AURA 10290
#define HAMMER_OF_JUSTICE 32416
#define DIVINE_PROTECTION 498
// Odo the Blindwatcher
#define CN_BLINDWATCHER 4279
#define HOWLRAGE1 7481
#define HOWLRAGE2 7483
#define HOWLRAGE3 7484
// Fenrus
#define CN_FENRUS 4274
//Archmage Arugal
#define CN_ARUGAL 4275
#define SHADOWBOLT 7124
#define MINDCONTROL 7621
#define TELEPORT 10418
//Wolf Master Nandos
#define CN_NENDOS 3927
//Deathstalker Adamant
#define CN_ADAMANT 3849
//Rethilgore
#define CN_RETHILGORE 3914
//Arugals Voidwalkers
#define CN_VOIDWALKER 4627

// Commander Springvale AI
class SpringvaleAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SpringvaleAI);
    SpringvaleAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		m_fdivine = true;

        infodevotion = dbcSpell.LookupEntry(DEVOTION_AURA);
		infohammer = dbcSpell.LookupEntry(HAMMER_OF_JUSTICE);
		infodivine = dbcSpell.LookupEntry(DIVINE_PROTECTION);

    }
    
    void OnCombatStart(Unit* mTarget)
    {
        RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->CastSpell(_unit, infodevotion, false);
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
        if(_unit->GetHealthPct() <= 20 && m_fdivine)
        {
			_unit->CastSpell(_unit, infodivine, false);
            m_fdivine = false;
		}
		uint32 val = RandomUInt(1000);
        SpellCast(val);
    }

    void SpellCast(uint32 val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())//_unit->getAttackTarget())
        {
            if(m_hammerJustice)
            {
                _unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), infohammer, false);
                m_hammerJustice = false;
                return;
            }           

            if(val >= 100 && val <= 225)
            {
                _unit->setAttackTimer(1000, false);
                m_hammerJustice = true;
            }
        }
    }

protected:

    bool m_hammerJustice;
    bool m_fdivine;
    SpellEntry *infodevotion, *infohammer, *infodivine;
};

// Odo the Blindwatcher AI

class BlindWatcherAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(BlindWatcherAI);
    BlindWatcherAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		m_rage1 = m_rage2 = m_rage3 =true;

        infohrage1 = dbcSpell.LookupEntry(HOWLRAGE1);
		infohrage2 = dbcSpell.LookupEntry(HOWLRAGE2);
		infohrage3 = dbcSpell.LookupEntry(HOWLRAGE3);
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
        if(_unit->GetHealthPct() <= 75 && m_rage1)
        {
			_unit->CastSpell(_unit, infohrage1, false);
            m_rage1 = false;
		}
		else if(_unit->GetHealthPct() <= 45 && m_rage2)
        {
            _unit->RemoveAura(HOWLRAGE1);
			_unit->CastSpell(_unit, infohrage2, false);
            m_rage2 = false;
		}
		else if(_unit->GetHealthPct() <= 20 && m_rage3)
        {
            _unit->RemoveAura(HOWLRAGE2);
			_unit->CastSpell(_unit, infohrage3, false);
            m_rage3 = false;
		}
    }
protected:
    bool m_rage1,m_rage2,m_rage3;
    SpellEntry *infohrage1, *infohrage2, *infohrage3;
};

// Fenrus the Devourer AI

class FenrusAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(FenrusAI);
    FenrusAI(Creature* pCreature) : CreatureAIScript(pCreature){}
    void OnDied(Unit * mKiller)
    {
	   _unit->SendChatMessageAlternateEntry(4275, CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Who dares interfere with the Sons of Arugal?");
	   _unit->PlaySoundToSet(5791);
	   _unit->GetMapMgr()->GetInterface()->SpawnCreature(4627, -154.274368f, 2177.196533f, 128.448517f, 5.760980f, true, false, 0, 0);
	   _unit->GetMapMgr()->GetInterface()->SpawnCreature(4627, -142.647537f, 2181.019775f, 128.448410f, 4.178475f, true, false, 0, 0);
	   _unit->GetMapMgr()->GetInterface()->SpawnCreature(4627, -139.146774f, 2168.201904f, 128.448364f, 2.650803f, true, false, 0, 0);
	   _unit->GetMapMgr()->GetInterface()->SpawnCreature(4627, -150.860092f, 2165.156250f, 128.448502f, 0.999966f, true, false, 0, 0);
	   
    }

protected:
};

//Voidwalker AI

class VoidWalkerAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(VoidWalkerAI);
    VoidWalkerAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{}
	
	/*void OnDamageTaken(Unit* mAttacker, float fAmount)
	{
		Mob1 = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-154.274368f, 2177.196533f, 128.448517f, 4627);
		Mob2 = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-142.647537f, 2181.019775f, 128.448410f, 4627);               
		Mob3 = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-139.146774f, 2168.201904f, 128.448364f, 4627);
		Mob4 = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-150.860092f, 2165.156250f, 128.448502f, 4627);
		
	}*/

    void OnDied(Unit * mKiller)
    {		
		//if((!Mob1->IsInWorld() || !Mob1->isAlive()) && (!Mob2->IsInWorld() || !Mob2->isAlive()) && (!Mob3->IsInWorld() || !Mob3->isAlive()) && (!Mob4->IsInWorld() || !Mob4->isAlive()))
		//{
			GameObject * pDoor = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-129.034f, 2166.16f, 129.187f, 18972);
			if(pDoor == 0)
			return;
			pDoor->SetUInt32Value(GAMEOBJECT_STATE, 0);
		//}
	}
protected:
	/*Unit* Mob1;
	Unit* Mob2;
	Unit* Mob3;
	Unit* Mob4;*/
};


// Archmage Arugal AI

class ArugalAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ArugalAI);
    ArugalAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
        m_shadownbolt = m_mindcontrol = m_teleport = true;
        infoshadowbolt = dbcSpell.LookupEntry(SHADOWBOLT);
		infomindcontrol = dbcSpell.LookupEntry(MINDCONTROL);
		infoteleport = dbcSpell.LookupEntry(TELEPORT);

    }
    
    void OnCombatStart(Unit* mTarget)
    {
        RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));

		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You, too, shall serve!");
        _unit->PlaySoundToSet(5793);
    }
    
    void OnTargetDied(Unit* mTarget)
    {
        _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Another falls!");
        _unit->PlaySoundToSet(5795);
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
		uint32 val = RandomUInt(1000);
        SpellCast(val);
    }

    void SpellCast(uint32 val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())//_unit->getAttackTarget())
        {
            if(m_shadownbolt)
            {
                _unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), infoshadowbolt, false);
                m_shadownbolt = false;
                return;
            }
            if(m_mindcontrol)
            {
                _unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), infomindcontrol, false);
                m_mindcontrol = false;
                _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Release your rage!");
                _unit->PlaySoundToSet(5797);
                return;
            }    
            if(m_teleport)
            {
                _unit->CastSpell(_unit, infoteleport, false);
                m_teleport = false;
                return;
            }               

            if(val >= 120 && val <= 225)
            {
                _unit->setAttackTimer(2000, false);
                m_shadownbolt = true;
            }
            else if(val > 300 && val <= 350)
            {
                _unit->setAttackTimer(4000, false);
                m_mindcontrol = true;
            }
            else if(val > 350 && val <= 500)
            {
                _unit->setAttackTimer(4000, false);
                m_teleport = true;
            }
        }
    }

protected:

   bool m_shadownbolt;
   bool m_mindcontrol;
   bool m_teleport;
   SpellEntry *infoshadowbolt, *infomindcontrol, *infoteleport;
};

//Wolf Master Nandos AI

class NandosAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(NandosAI);
    NandosAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
    }

    void OnDied(Unit * mKiller)
    {
		GameObject * pDoor = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-118.11f, 2161.86f, 155.678f, 18971);
		if(pDoor == 0)
		return;
		pDoor->SetUInt32Value(GAMEOBJECT_STATE, 0);
    }
protected:
};

//Rethilgore AI

class RETHILGOREAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(RETHILGOREAI);
    RETHILGOREAI(Creature* pCreature) : CreatureAIScript(pCreature){}
    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessageAlternateEntry(3849, CHAT_MSG_MONSTER_SAY, LANG_GUTTERSPEAK, "About time someone killed the wretch.");
		_unit->SendChatMessageAlternateEntry(3850, CHAT_MSG_MONSTER_SAY, LANG_COMMON, "For once I agree with you... scum.");      // dont know the allys text yet
    }
protected:
};

void SetupShadowfangKeep(ScriptMgr * mgr)
{
	//creature scripts
	mgr->register_creature_script(CN_NENDOS, &NandosAI::Create);
	mgr->register_creature_script(CN_VOIDWALKER, &VoidWalkerAI::Create);
	mgr->register_creature_script(CN_VOIDWALKER, &VoidWalkerAI::Create);
	mgr->register_creature_script(CN_RETHILGORE, &RETHILGOREAI::Create);
    mgr->register_creature_script(CN_SPRINGVALE, &SpringvaleAI::Create);
    mgr->register_creature_script(CN_BLINDWATCHER, &BlindWatcherAI::Create);
    mgr->register_creature_script(CN_FENRUS, &FenrusAI::Create);
    mgr->register_creature_script(CN_ARUGAL, &ArugalAI::Create);
}