/*
* ArcEmu MMORPG Server
* Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
* Copyright (C) 2008-2014 <http://www.ArcEmu.org/>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Affero General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Affero General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*/
#include "StdAfx.h"
namespace Arcemu{
	GO_QuestGiver::GO_QuestGiver() : GameObject(){
	}
	GO_QuestGiver::GO_QuestGiver(uint64 GUID) : GameObject(GUID){
		m_quests = NULL;
	}
	GO_QuestGiver::~GO_QuestGiver(){
	}
	void GO_QuestGiver::InitAI(){
		LoadQuests();
		if (myScript == NULL)
			myScript = sScriptMgr.CreateAIScriptClassForGameObject(GetEntry(), this);
	}
	void GO_QuestGiver::DeleteQuest(QuestRelation *Q){
		for (list< QuestRelation* >::iterator itr = m_quests->begin(); itr != m_quests->end(); ++itr){
			QuestRelation *qr = *itr;
			if ((qr->type == Q->type) && (qr->qst == Q->qst)){
				delete qr;
				m_quests->erase(itr);
				break;
			}
		}
	}
	Quest* GO_QuestGiver::FindQuest(uint32 quest_id, uint8 quest_relation){
		for (list< QuestRelation* >::iterator itr = m_quests->begin(); itr != m_quests->end(); ++itr){
			QuestRelation *qr = *itr;
			if ((qr->qst->id == quest_id) && ((qr->type & quest_relation) != 0)){
				return qr->qst;
			}
		}
		return NULL;
	}
	uint16 GO_QuestGiver::GetQuestRelation(uint32 quest_id){
		uint16 quest_relation = 0;
		for (list< QuestRelation* >::iterator itr = m_quests->begin(); itr != m_quests->end(); ++itr){
			QuestRelation *qr = *itr;
			if ((qr != NULL) && (qr->qst->id == quest_id))
				quest_relation |= qr->type;
		}
		return quest_relation;
	}
}