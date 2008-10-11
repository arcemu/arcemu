/*
 * OpenarcemuScripts for Openarcemu MMORPG Server
 * Copyright (C) 2008 OpenarcemuScripts Team
 * Copyright (C) 2008 WEmu Team
 * Based on Moon++ Scripts for arcemu MMORPG Server
 * Copyright (C) 2007-2008 Moon++ Team
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

class SCRIPT_DECL DraeneiFishingNet : public GossipScript
{
public:
  void GossipHello(Object * pObject, Player* Plr, bool AutoSend)
  {
	Item *item = (Item*)pObject;
	uint32 spellid = item->GetProto()->Spells[0].Id;
	Plr->CastSpell(Plr, dbcSpell.LookupEntry(spellid), false);

	QuestLogEntry *qle = Plr->GetQuestLogForEntry(9452);
	if(qle == NULL)
	  return;

	uint8 chance = RandomUInt(10);
	if(chance <= 3)
	{
		if(Plr == NULL || Plr->GetMapMgr() == NULL || Plr->GetMapMgr()->GetInterface() == NULL)
			return;
	  sEAS.SpawnCreature(Plr, 17102, Plr->GetPositionX(), Plr->GetPositionY(), Plr->GetPositionZ(), Plr->GetOrientation(), 3*60*1000);
	  return;
	}

	sEAS.AddItem(23614, Plr);

	qle->SendUpdateAddKill(1);
	qle->UpdatePlayerFields();

	Plr->Gossip_Complete();
  }
};

class SCRIPT_DECL TotemofCoo : public QuestScript 
{ 
public:

	void OnQuestStart( Player * mTarget, QuestLogEntry * qLogEntry)
	{
		if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL )
			return;
		float SSX = mTarget->GetPositionX();
		float SSY = mTarget->GetPositionY();
		float SSZ = mTarget->GetPositionZ();


		Creature* creat = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ, 17360);
		if(creat == NULL)
			return;		
		creat->m_escorter = mTarget;   
		creat->GetAIInterface()->setMoveType(11);
		creat->GetAIInterface()->StopMovement(3000);
		creat->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Follow me I shall result you on a place.!");
		creat->SetUInt32Value(UNIT_NPC_FLAGS, 0);

		sEAS.CreateCustomWaypointMap(creat);
		sEAS.WaypointCreate(creat,-4174.025879f, -12512.800781f, 44.361458f, 2.827430f, 0, 256, 0);
		sEAS.WaypointCreate(creat,-4078.135986f, -12535.500977f, 43.066765f, 5.949394f, 0, 256, 6801);
		sEAS.WaypointCreate(creat,-4040.495361f, -12565.537109f, 43.698250f, 5.592041f, 0, 256, 6801);
		sEAS.WaypointCreate(creat,-4009.526367f, -12598.929688f, 53.168480f, 5.434962f, 0, 256, 6801);
		sEAS.WaypointCreate(creat,-3981.581543f, -12635.541602f, 63.896046f, 5.332861f, 0, 256, 6801);
		sEAS.WaypointCreate(creat,-3953.170410f, -12680.391602f, 75.433006f, 5.218981f, 0, 256, 6801);
		sEAS.WaypointCreate(creat,-3924.324951f, -12741.846680f, 95.187035f, 5.124734f, 0, 256, 6801);
		sEAS.WaypointCreate(creat,-3920.791260f, -12746.218750f, 96.887978f, 3.271200f, 0, 256, 0);
	}
};

class Totem_of_Akida : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(Totem_of_Akida);
	Totem_of_Akida(Creature* pCreature) : CreatureAIScript(pCreature) {}

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		if(iWaypointId == 8)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Here he");
			_unit->Despawn(5000,1000);
			sEAS.DeleteWaypoints(_unit);
			if(_unit->m_escorter == NULL)
				return;
			Player* plr = _unit->m_escorter;
			_unit->m_escorter = NULL;
			plr->GetQuestLogForEntry(9539)->SendQuestComplete();
		}
	}
};

void SetupAzuremystIsle(ScriptMgr * mgr)
{
	mgr->register_item_gossip_script(23654, CREATE_GOSSIPSCRIPT(DraeneiFishingNet));
  	mgr->register_creature_script(17360, &Totem_of_Akida::Create);
	mgr->register_quest_script(9539, CREATE_QUESTSCRIPT(TotemofCoo));
}
