/*
 * WEmu Scripts for WEmu MMORPG Server
 * Copyright (C) 2008 WEmu Team
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
#include "../EAS/EasyFunctions.h"

// The Bladespire Threat Quest
class BladespireQAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(BladespireQAI);
	BladespireQAI(Creature* pCreature) : CreatureAIScript(pCreature)  {}

	void OnDied(Unit * mKiller)
	{
		if (mKiller->IsPlayer()) 
		{
			QuestLogEntry *en = ((Player*)mKiller)->GetQuestLogForEntry(10503);
			if(en && en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
			{
				uint32 newcount = en->GetMobCount(0) + 1;
				en->SetMobCount(0, newcount);
				en->SendUpdateAddKill(0);
				en->UpdatePlayerFields();
				return;
			}
		}
	}
};

class SCRIPT_DECL IntotheSoulgrinder : public QuestScript
{   
public:
	void OnQuestComplete(Player* mTarget, QuestLogEntry *qLogEntry)
	{
		Creature *qg = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 22941);
		if(qg == NULL)
			return;
		qg->GetMapMgr()->GetInterface()->SpawnCreature(23053, 2794.978271f, 5842.185547f, 35.911819f, 0, true, false, 0, 0);
	}
};

class MagnetoAura : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(MagnetoAura);
	MagnetoAura(Creature* pCreature) : CreatureAIScript(pCreature) {}

	void OnLoad()
	{
		_unit->CastSpell(_unit, 37136, true);
	}
};

class powerconv : public GameObjectAIScript
{
public:
	powerconv(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new powerconv(GO); }

	void OnActivate(Player * pPlayer)
	{
		if(!pPlayer)
			return;

		QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(10584);
		if(qle == NULL)
			return;

		Creature *magneto = sEAS.SpawnCreature(pPlayer, 21729, _gameobject->GetPositionX(), _gameobject->GetPositionY(), _gameobject->GetPositionZ(), 0, 0);
		magneto->Despawn(5*60*1000, 0);

		_gameobject->Despawn(300000);
	}
};

class NetherEgg : public GameObjectAIScript
{
public:
	NetherEgg(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new NetherEgg(GO); }

	void OnActivate(Player * pPlayer)
	{
		if(!pPlayer)
			return;

		QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(10609);
		if(qle == NULL)
			return;

		Creature *whelp = sEAS.SpawnCreature(pPlayer, 20021, _gameobject->GetPositionX(), _gameobject->GetPositionY(), _gameobject->GetPositionZ(), 0, 0);
		whelp->Despawn(5*60*1000, 0);

		_gameobject->Despawn(300000);
	}
};
		
		

class FunnyDragon : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(FunnyDragon);
	FunnyDragon(Creature* pCreature) : CreatureAIScript(pCreature) {}

	void OnLoad()
	{
		RegisterAIUpdateEvent(5000);
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->disable_melee = true;
		_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
		_unit->GetAIInterface()->m_canMove = false;
		i = 1;
	}

	void AIUpdate()
	{
		switch(i)
		{
		case 1:
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Muahahahahaha! You fool! you've released me from my banishment in the interstices between space and time!");
			}
		case 2:
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "All of Draenor shall quake beneath my feet! i Will destroy this world and reshape it in my immage!");
			}
		case 3:
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Where shall i Begin? i cannot bother myself with a worm such as yourself. Theres a World to be Conquered!");
			}
		case 4:
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "No doubt the fools that banished me are long dead. i shall take the wing and survey my new demense, Pray to whatever gods you hold dear that we do not meet again.");
				_unit->Despawn(5000, 0);
			}
		}

		i++;
	}

protected:
	uint32 i;
};

class LegionObelisk : public GameObjectAIScript
{
public:
	LegionObelisk(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new LegionObelisk(GO); }

	void OnActivate(Player * pPlayer)
	{
		if( pPlayer == NULL || pPlayer->GetMapMgr() == NULL || pPlayer->GetMapMgr()->GetInterface() == NULL )
				return;

		GameObject* obelisk1 = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(2898.92, 4759.29, 277.408, 185198);
		GameObject* obelisk2 = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(2942.3, 4752.28, 285.553, 185197);
		GameObject* obelisk3 = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(2834.39, 4856.67, 277.632, 185196);
		GameObject* obelisk4 = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(2923.37, 4840.36, 278.45, 185195);
		GameObject* obelisk5 = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(2965.75, 4835.25, 277.949, 185193);

		if( obelisk1 && obelisk2 && obelisk3 && obelisk4 && obelisk5 )
		{
			if(obelisk1->GetUInt32Value(GAMEOBJECT_STATE) == 0 && obelisk2->GetUInt32Value(GAMEOBJECT_STATE) == 0 && obelisk3->GetUInt32Value(GAMEOBJECT_STATE) == 0 && obelisk4->GetUInt32Value(GAMEOBJECT_STATE) == 0 && obelisk5->GetUInt32Value(GAMEOBJECT_STATE) == 0)
			{
				sEAS.SpawnCreature(pPlayer, 19963, 2943.59, 4779.05, 284.49, 1.89, 60*5*1000 );
			}
		}
		TimedEvent *te = TimedEvent::Allocate(this, new CallbackP1<LegionObelisk, Player*>(this, &LegionObelisk::ResetGO, pPlayer), 0, 10000, 1);
		pPlayer->event_AddEvent(te);
	}

	void ResetGO(Player* plr)
	{
		if( plr == NULL || plr->GetMapMgr() == NULL || plr->GetMapMgr()->GetInterface() == NULL )
				return;

		GameObject* obelisk1 = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(2898.92, 4759.29, 277.408, 185198);
		GameObject* obelisk2 = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(2942.3, 4752.28, 285.553, 185197);
		GameObject* obelisk3 = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(2834.39, 4856.67, 277.632, 185196);
		GameObject* obelisk4 = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(2923.37, 4840.36, 278.45, 185195);
		GameObject* obelisk5 = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(2965.75, 4835.25, 277.949, 185193);

		obelisk1->SetUInt32Value(GAMEOBJECT_STATE, 1);
		obelisk2->SetUInt32Value(GAMEOBJECT_STATE, 1);
		obelisk3->SetUInt32Value(GAMEOBJECT_STATE, 1);
		obelisk4->SetUInt32Value(GAMEOBJECT_STATE, 1);
		obelisk5->SetUInt32Value(GAMEOBJECT_STATE, 1);
	}
};

void SetupBladeEdgeMountains(ScriptMgr * mgr)
{
	mgr->register_creature_script(19995, &BladespireQAI::Create);
	mgr->register_creature_script(21296, &BladespireQAI::Create);
	mgr->register_creature_script(20765, &BladespireQAI::Create);
	mgr->register_creature_script(20766, &BladespireQAI::Create);
	mgr->register_creature_script(19998, &BladespireQAI::Create);
	mgr->register_quest_script(11000, CREATE_QUESTSCRIPT(IntotheSoulgrinder));
	mgr->register_gameobject_script(184906, &powerconv::Create);
	mgr->register_creature_script(21731, &MagnetoAura::Create);
	mgr->register_gameobject_script(184867, &NetherEgg::Create);
	mgr->register_creature_script(21823, &FunnyDragon::Create);

	mgr->register_gameobject_script( 185198, &LegionObelisk::Create);
	mgr->register_gameobject_script( 185197, &LegionObelisk::Create);
	mgr->register_gameobject_script( 185196, &LegionObelisk::Create);
	mgr->register_gameobject_script( 185195, &LegionObelisk::Create);
	mgr->register_gameobject_script( 185193, &LegionObelisk::Create);
}
