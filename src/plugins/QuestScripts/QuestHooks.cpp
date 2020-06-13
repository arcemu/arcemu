/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009 ArcEmu Team <http://www.arcemu.org/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
 * Copyright (C) 2008 WEmu Team
 *
 * Script by Fer0x
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


#include "Setup.h"


/*************************************************************************************
**************************************************************************************
								ON QUEST ACCEPT
**************************************************************************************
**************************************************************************************/


void Hanazua(Player* pPlayer, Object* pObject)
{
	string say;
	say = "Go swiftly, ";
	say += pPlayer->GetName();
	say += ", my fate is in your hands.";
	(TO_CREATURE(pObject))->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, say.c_str());
}

void AHumbleTask(Player* pPlayer, Object* pObject)
{
	(TO_CREATURE(pObject))->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Greatmother Hawkwind gestures to the pitcher of water sitting on the edge of the well.");
}

void Yorus_Barleybrew(Player* pPlayer, Object* pObject)
{
	(TO_CREATURE(pObject))->Emote(EMOTE_ONESHOT_POINT);
	pPlayer->CastSpell(pPlayer, dbcSpell.LookupEntry(8554), true);
}

void Menara_Voidrender(Player* pPlayer, Object* pObject)
{
	(TO_CREATURE(pObject))->EventAddEmote(EMOTE_STATE_USESTANDING, 3000);
}

void Hanazua_III(Player* pPlayer, Object* pObject)
{
	(TO_CREATURE(pObject))->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Killed  Sarkoth !!");
}

void TheFamilyAndTheFishingPole(Player* pPlayer, Object* pObject)
{
	(TO_CREATURE(pObject))->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "I got some extras with me; if you pay me back what i paid for them, you can have one. I got some bait too.");
}

void MillysHarvest(Player* pPlayer, Object* pObject)
{
	(TO_CREATURE(pObject))->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "I hope for you!");
	(TO_CREATURE(pObject))->EventAddEmote(EMOTE_ONESHOT_CRY, 2000);
}

void Rest_n_Relaxation(Player* pPlayer, Object* pObject)
{
	(TO_CREATURE(pObject))->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Think of small pumpkins, my friend!!");
	(TO_CREATURE(pObject))->Emote(EMOTE_ONESHOT_CHEER);
}

void OntoGoldshireComplete(Player* pPlayer, Object* pObject)
{
	char msg[256];
	snprintf((char*)msg, 256, "You are dismissed %s . ", pPlayer->GetName());
	(TO_CREATURE(pObject))->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, msg);
	(TO_CREATURE(pObject))->Emote(EMOTE_ONESHOT_BOW);
}

void ZuluhedtheWhacked(Player* pPlayer, Object* pObject)
{
	Creature* Zuluhed = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-4206.199219f, 313.5462f, 122.907f, 11980);
	if(Zuluhed == NULL)
	{
		sEAS.SpawnCreature(pPlayer, 11980, -4206.199219f, 313.5462f, 122.907f, 1.2589f);
	}
}
//=================================================================================================

void OnQuestAccept(Player* pPlayer, Quest* pQuest, Object* pObject)
{
	if(pPlayer == NULL || pQuest == NULL || pObject == NULL || !pObject->IsInWorld() || !pPlayer->IsInWorld() || !pObject->IsCreature())
		return;

	switch(pQuest->id)
	{
		case 790:
			Hanazua_III(pPlayer, pObject);
			break;
		case 804:
			Hanazua(pPlayer, pObject);
			break;
		case 753:
			AHumbleTask(pPlayer, pObject);
			break;
		case 1699:
			Yorus_Barleybrew(pPlayer, pObject);
			break;
		case 4786:
			Menara_Voidrender(pPlayer, pObject);
			break;
		case 4963:
			Menara_Voidrender(pPlayer, pObject);
			break;
		case 1141:
			TheFamilyAndTheFishingPole(pPlayer, pObject);
			break;
		case 3904:
			MillysHarvest(pPlayer, pObject);
			break;
		case 2158:
			Rest_n_Relaxation(pPlayer, pObject);
			break;
		case 54:
			OntoGoldshireComplete(pPlayer, pObject);
			break;
		case 10872:
			ZuluhedtheWhacked(pPlayer, pObject);
			break;
	}
}

//===============================================================================================

/*************************************************************************************
**************************************************************************************
							   ON QUEST FINISHED
**************************************************************************************
**************************************************************************************/


void Hanazua_II(Player* pPlayer, Object* pObject)
{
	(TO_CREATURE(pObject))->SetStandState(STANDSTATE_KNEEL);
}

void Wishock(Player* pPlayer, Object* pObject)
{
	(TO_CREATURE(pObject))->SetStandState(STANDSTATE_DEAD);
	pPlayer->Emote(EMOTE_STATE_LAUGH);
	(TO_CREATURE(pObject))->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Arrgh...");
}

void CapturedMountaineer(Player* pPlayer, Object* pObject)
{
	string say = "I raise my brew and hope to be rid of the likes of you!  Cheers, you no good scoundrel, ";
	say += pPlayer->GetName();
	say += "!";
	(TO_CREATURE(pObject))->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, say.c_str());
}

void PlaguedLands(Player* pPlayer, Object* pObject)
{
	(TO_CREATURE(pObject))->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Be cleansed!");
}

void DeeprunRatRoundup(Player* pPlayer, Object* pObject)
{
	(TO_CREATURE(pObject))->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Into the box me pretties! Thas it. One by one ye go.");
}

void MaybellComplete(Player* pPlayer, Object* pObject)
{
	(TO_CREATURE(pObject))->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Here goes nothing...");
	(TO_CREATURE(pObject))->Emote(EMOTE_ONESHOT_CRY);
	(TO_CREATURE(pObject))->Despawn(5000, 30000);
}

//========================================================================================

void OnQuestFinished(Player* pPlayer, Quest* pQuest, Object* pObject)
{
	if(pPlayer == NULL || pQuest == NULL || pObject == NULL || !pObject->IsCreature())
		return;

	switch(pQuest->id)
	{
		case 790:
			Hanazua_II(pPlayer, pObject);
			break;
		case 336:
			Wishock(pPlayer, pObject);
			break;
		case 492:
			CapturedMountaineer(pPlayer, pObject);
			break;
		case 2118:
			PlaguedLands(pPlayer, pObject);
			break;
		case 6661:
			DeeprunRatRoundup(pPlayer, pObject);
			break;
		case 114:
			MaybellComplete(pPlayer, pObject);
			break;
	}
}
//===============================================================================================

/*************************************************************************************
**************************************************************************************
							   ON QUEST CANCELLED
**************************************************************************************
**************************************************************************************/

void ZuluhedtheWhackedCancel(Player* pPlayer)
{
	Creature* Zuluhed = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-4206.199219f, 313.5462f, 122.907f, 11980);
	if(Zuluhed != NULL)
	{
		Zuluhed->Despawn(0, 0);
	}
}

//========================================================================================

void OnQuestCancelled(Player* pPlayer, Quest* pQuest)
{
	if(pPlayer == NULL || pQuest == NULL)
		return;

	switch(pQuest->id)
	{
		case 10872:
			ZuluhedtheWhackedCancel(pPlayer);
			break;
	}
}
//=========================================================================================

/*************************************************************************************
**************************************************************************************
							   ON EMOTE
**************************************************************************************
**************************************************************************************/


void InnkeeperFlex(Player* pPlayer, Unit* pUnit)
{
	if(pUnit->GetEntry() == 6740)
	{
		QuestLogEntry* qle = pPlayer->GetQuestLogForEntry(8356);
		if(qle == NULL)
			return;

		qle->SetMobCount(0, qle->GetMobCount(0) + 1);
		qle->SendUpdateAddKill(0);
		qle->UpdatePlayerFields();
	}
	else if(pUnit->GetEntry() == 6929)
	{
		QuestLogEntry* qle = pPlayer->GetQuestLogForEntry(8359);
		if(qle == NULL)
			return;

		qle->SetMobCount(0, qle->GetMobCount(0) + 1);
		qle->SendUpdateAddKill(0);
		qle->UpdatePlayerFields();
	}
}

void InnkeeperDance(Player* pPlayer, Unit* pUnit)
{
	if(pUnit->GetEntry() == 6735)
	{
		QuestLogEntry* qle = pPlayer->GetQuestLogForEntry(8357);
		if(qle == NULL)
			return;

		qle->SetMobCount(0, qle->GetMobCount(0) + 1);
		qle->SendUpdateAddKill(0);
		qle->UpdatePlayerFields();
	}
	else if(pUnit->GetEntry() == 6746)
	{
		QuestLogEntry* qle = pPlayer->GetQuestLogForEntry(8360);
		if(qle == NULL)
			return;

		qle->SetMobCount(0, qle->GetMobCount(0) + 1);
		qle->SendUpdateAddKill(0);
		qle->UpdatePlayerFields();
	}
}

void InnkeeperTrain(Player* pPlayer, Unit* pUnit)
{
	if(pUnit->GetEntry() == 6826)
	{
		QuestLogEntry* qle = pPlayer->GetQuestLogForEntry(8355);
		if(qle == NULL)
			return;

		qle->SetMobCount(0, qle->GetMobCount(0) + 1);
		qle->SendUpdateAddKill(0);
		qle->UpdatePlayerFields();
	}
	else if(pUnit->GetEntry() == 11814)
	{
		QuestLogEntry* qle = pPlayer->GetQuestLogForEntry(8358);
		if(qle == NULL)
			return;

		qle->SetMobCount(0, qle->GetMobCount(0) + 1);
		qle->SendUpdateAddKill(0);
		qle->UpdatePlayerFields();
	}
}

void InnkeeperChicken(Player* pPlayer, Unit* pUnit)
{
	if(pUnit->GetEntry() == 5111)
	{
		QuestLogEntry* qle = pPlayer->GetQuestLogForEntry(8353);
		if(qle == NULL)
			return;

		qle->SetMobCount(0, qle->GetMobCount(0) + 1);
		qle->SendUpdateAddKill(0);
		qle->UpdatePlayerFields();
	}
	else if(pUnit->GetEntry() == 6741)
	{
		QuestLogEntry* qle = pPlayer->GetQuestLogForEntry(8354);
		if(qle == NULL)
			return;

		qle->SetMobCount(0, qle->GetMobCount(0) + 1);
		qle->SendUpdateAddKill(0);
		qle->UpdatePlayerFields();
	}
}
//=========================================================================================

void OnEmote(Player* pPlayer, uint32 Emote, Unit* pUnit)
{
	pUnit = pPlayer->GetMapMgr()->GetUnit(pPlayer->GetSelection());
	if(!pUnit || !pUnit->isAlive() || pUnit->GetAIInterface()->getNextTarget())
		return;

	switch(Emote)
	{
		case EMOTE_ONESHOT_FLEX:
			InnkeeperFlex(pPlayer, pUnit);
			break;

		case EMOTE_STATE_DANCE:
			InnkeeperDance(pPlayer, pUnit);
			break;

		case EMOTE_ONESHOT_TRAIN:
			InnkeeperTrain(pPlayer, pUnit);
			break;

		case EMOTE_ONESHOT_CHICKEN:
			InnkeeperChicken(pPlayer, pUnit);
			break;
	}
}

//=========================================================================================

/*************************************************************************************
**************************************************************************************
							   ON AREATRIGGER
**************************************************************************************
**************************************************************************************/


void InvasionPointCataclysm(Player* pPlayer, uint32 AreaTrigger)
{
	QuestLogEntry* en = pPlayer->GetQuestLogForEntry(10766);
	if(en == NULL)
		return;
	pPlayer->SafeTeleport(530, 0, -2723.674561f, 1952.664673f, 146.939743f, 3.185559f);
}

void Scratches(Player* pPlayer, uint32 AreaTrigger)
{
	QuestLogEntry* qle = pPlayer->GetQuestLogForEntry(10556);
	if(qle == NULL)
		return;

	Creature*  Kaliri = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 21468);
	if(Kaliri == NULL)
		return;

	Kaliri->Despawn(0, 0);
	qle->SetMobCount(0, qle->GetMobCount(0) + 1);
	qle->SendUpdateAddKill(0);
	qle->UpdatePlayerFields();
}

//=========================================================================================

void OnAreaTrigger(Player* pPlayer, uint32 AreaTrigger)
{
	switch(AreaTrigger)
	{
		case 4546:
			{
				Scratches(pPlayer, 4546);
			}
			break;
		case 4560:
			{
				InvasionPointCataclysm(pPlayer, 4560);
			}
			break;
	}
}

//=========================================================================================

void SetupQuestHooks(ScriptMgr* mgr)
{
	mgr->register_hook(SERVER_HOOK_EVENT_ON_QUEST_ACCEPT, (void*)&OnQuestAccept);
	mgr->register_hook(SERVER_HOOK_EVENT_ON_QUEST_FINISHED, (void*)&OnQuestFinished);
	mgr->register_hook(SERVER_HOOK_EVENT_ON_QUEST_CANCELLED, (void*)&OnQuestCancelled);
	mgr->register_hook(SERVER_HOOK_EVENT_ON_EMOTE, (void*)&OnEmote);
	mgr->register_hook(SERVER_HOOK_EVENT_ON_AREATRIGGER, (void*)&OnAreaTrigger);
}
