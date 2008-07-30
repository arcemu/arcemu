/*
 * WEmu Scripts for WEmu MMORPG Server
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


#include "StdAfx.h"
#include "Setup.h"
#include "../EAS/EasyFunctions.h"


/*************************************************************************************
**************************************************************************************
								ON QUEST ACCEPT
**************************************************************************************
**************************************************************************************/
/*

void Hanazua(Player * pPlayer, Object * pObject)
{
	string say;
	say = "Go swiftly, ";
	say+= pPlayer->GetName();
	say+= ", my fate is in your hands.";
	((Creature*)pObject)->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, say.c_str());
}

void AHumbleTask(Player * pPlayer, Object * pObject)
{
	((Creature*)pObject)->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Greatmother Hawkwind gestures to the pitcher of water sitting on the edge of the well.");
}

void Yorus_Barleybrew(Player * pPlayer, Object * pObject)
{
	((Creature*)pObject)->Emote(EMOTE_ONESHOT_POINT);
	pPlayer->CastSpell(pPlayer, dbcSpell.LookupEntry(8554), true);
}

void Menara_Voidrender(Player * pPlayer, Object * pObject)
{
	((Creature*)pObject)->EventAddEmote(EMOTE_STATE_USESTANDING, 3000);
}

void Hanazua_III(Player* pPlayer, Object* pObject)
{
	((Creature*)pObject)->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Killed  Sarkoth !!");
}

void TheFamilyAndTheFishingPole(Player* pPlayer, Object* pObject)
{
	((Creature*)pObject)->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "I got some extras with me; if you pay me back what i paid for them, you can have one. I got some bait too.");
}

void MillysHarvest(Player * pPlayer, Object * pObject)
{
	((Creature*)pObject)->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "I hope for you!");
	((Creature*)pObject)->EventAddEmote(EMOTE_ONESHOT_CRY, 2000);
}

void Rest_n_Relaxation(Player * pPlayer, Object * pObject)
{
	((Creature*)pObject)->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Think of small pumpkins, my friend!!");
	((Creature*)pObject)->Emote(EMOTE_ONESHOT_CHEER	);
}

void OntoGoldshireComplete(Player* pPlayer, Object* pObject)
{
	char msg[256];
	snprintf((char*)msg, 256, "You are dismissed %s . ", pPlayer->GetName());
	((Creature*)pObject)->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, msg);
	((Creature*)pObject)->Emote(EMOTE_ONESHOT_BOW);
}

//=================================================================================================

void OnQuestAccept(Player * pPlayer, Quest * pQuest, Object * pObject)
{
	if(!pPlayer || !pObject || !pQuest)
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
	}
}

//===============================================================================================



/*************************************************************************************
**************************************************************************************
							   ON QUEST FINISHED
**************************************************************************************
**************************************************************************************/
/*

void Hanazua_II(Player * pPlayer, Object * pObject)
{
	((Creature*)pObject)->SetStandState(STANDSTATE_KNEEL);
}

void Wishock(Player * pPlayer, Object * pObject)
{
	((Creature*)pObject)->SetStandState(STANDSTATE_DEAD);
	pPlayer->Emote(EMOTE_STATE_LAUGH);
	((Creature*)pObject)->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Arrgh...");
}

void CapturedMountaineer(Player * pPlayer, Object * pObject)
{
	string say = "I raise my brew and hope to be rid of the likes of you!  Cheers, you no good scoundrel, ";
	say+=pPlayer->GetName();
	say+="!";
	((Creature*)pObject)->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, say.c_str());
}

void PlaguedLands(Player* pPlayer, Object* pObject)
{
	((Creature*)pObject)->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Be cleansed!");
}

void DeeprunRatRoundup(Player* pPlayer, Object* pObject)
{
	((Creature*)pObject)->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Into the box me pretties! Thas it. One by one ye go.");
}

void MaybellComplete(Player* pPlayer, Object* pObject)
{
	((Creature*)pObject)->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Here goes nothing...");
	((Creature*)pObject)->Emote(EMOTE_ONESHOT_CRY);
	((Creature*)pObject)->Despawn(5000, 30000);
}

//========================================================================================

void OnQuestFinished(Player * pPlayer, Quest * pQuest, Object * pObject)
{
	if(!pPlayer || !pObject || !pQuest)
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
//=========================================================================================
*/
void SetupQuestHooks(ScriptMgr * mgr)
{
	//mgr->register_hook(SERVER_HOOK_EVENT_ON_QUEST_ACCEPT, (void *)&OnQuestAccept);
	//mgr->register_hook(SERVER_HOOK_EVENT_ON_QUEST_FINISHED, (void *)&OnQuestFinished);
}
