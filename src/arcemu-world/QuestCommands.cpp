/****************************************************************************
 *
 * General Object Type File
 * Copyright (c) 2007 Team Ascent
 *
 * This file may be distributed under the terms of the Q Public License
 * as defined by Trolltech ASA of Norway and appearing in the file
 * COPYING included in the packaging of this file.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
*/

#include "StdAfx.h"

class ChatHandler;

ARCEMU_INLINE std::string MyConvertIntToString(const int arg)
{
	stringstream out;
	out << arg;
	return out.str();
}

ARCEMU_INLINE std::string MyConvertFloatToString(const float arg)
{
	stringstream out;
	out << arg;
	return out.str();
}

string RemoveQuestFromPlayer(Player *plr, Quest *qst)
{
	std::string recout = "|cff00ff00";

	if (plr->HasQuests())
	{
		if (plr->HasFinishedQuest(qst->id))
			recout += "Player has already completed that quest.\n\n";
		else
		{
			QuestLogEntry * qLogEntry = plr->GetQuestLogForEntry(qst->id);
			if (qLogEntry)
			{	
				CALL_QUESTSCRIPT_EVENT(qLogEntry, OnQuestCancel)(plr);
				qLogEntry->Finish();

				// Remove all items given by the questgiver at the beginning
				for(uint32 i = 0; i < 4; ++i)
				{
					if(qst->receive_items[i])
						plr->GetItemInterface()->RemoveItemAmt(qst->receive_items[i], 1 );
				}
				if(qst->time > 0)
					plr->timed_quest_slot = 0;

				plr->UpdateNearbyGameObjects();
			}
			else
				recout += "No quest log entry found for that player.";
		}
	}
	else
	{
		recout += "Player has no quests to remove.";
	}

	recout += "\n\n";

	return recout;
}

bool ChatHandler::HandleQuestLookupCommand(const char * args, WorldSession * m_session)
{
	if(!*args) return false;

	string x = string(args);
	arcemu_TOLOWER(x);
	if(x.length() < 4)
	{
		RedSystemMessage(m_session, "Your search string must be at least 5 characters long.");
		return true;
	}

	BlueSystemMessage(m_session, "Starting search of quests `%s`...", x.c_str());
	uint32 t = getMSTime();

	StorageContainerIterator<Quest> * itr = QuestStorage.MakeIterator();

	Quest * i;
	uint32 count = 0;
	string y;
	string recout;

	while(!itr->AtEnd())
	{
		i = itr->Get();
		y = string(i->title);

		LocalizedQuest *li	= (m_session->language>0) ? sLocalizationMgr.GetLocalizedQuest(i->id, m_session->language) : NULL;

		std::string liName	= std::string(li ? li->Title : "");

		arcemu_TOLOWER(liName);
 		arcemu_TOLOWER(y);
 
		bool localizedFound	= false;
		if(FindXinYString(x, liName))
			localizedFound	= true;

		if(FindXinYString(x, y) || localizedFound)
 		{
 			string questid = MyConvertIntToString(i->id);
			const char * questtitle = localizedFound ? li->Title : i->title;
			recout = "|cff00ccff";
			recout += questid.c_str();
			recout += ": ";
			recout += questtitle;
			recout += "\n";
			SendMultilineMessage(m_session, recout.c_str());

			++count;
			if(count == 25)
			{
				RedSystemMessage(m_session, "More than 25 results returned. aborting.");
				break;
			}
		}
		if(!itr->Inc())
			break;
	}
	itr->Destruct();

	if (count == 0)
	{
		recout = "|cff00ccffNo matches found.\n\n";
		SendMultilineMessage(m_session, recout.c_str());
	}

	BlueSystemMessage(m_session, "Search completed in %u ms.", getMSTime() - t);

	return true;
}

bool ChatHandler::HandleQuestStatusCommand(const char * args, WorldSession * m_session)
{
	if(!*args) return false;

	Player *plr = getSelectedChar(m_session, true);
	if(!plr)
	{
		plr = m_session->GetPlayer();
		SystemMessage(m_session, "Auto-targeting self.");
	}

	uint32 quest_id = atol(args);
	std::string recout = "|cff00ff00";

	Quest * qst = QuestStorage.LookupEntry(quest_id);
	if(qst)
	{
		if (plr->HasFinishedQuest(quest_id))
			recout += "Player has already completed that quest.";
		else
		{
			QuestLogEntry * IsPlrOnQuest = plr->GetQuestLogForEntry(quest_id);
			if (IsPlrOnQuest)
				recout += "Player is currently on that quest.";
			else
				recout += "Player has NOT finished that quest.";
		}
	}
	else
	{
		recout += "Quest Id [";
		recout += args;
		recout += "] was not found and unable to add it to the player's quest log.";
	}

	recout += "\n\n";

	SendMultilineMessage(m_session, recout.c_str());

	return true;
}

bool ChatHandler::HandleQuestStartCommand(const char * args, WorldSession * m_session)
{
	if(!*args) return false;

	Player *plr = getSelectedChar(m_session, true);
	if(!plr)
	{
		plr = m_session->GetPlayer();
		SystemMessage(m_session, "Auto-targeting self.");
	}

	uint32 quest_id = atol(args);
	std::string recout = "|cff00ff00";

	Quest * qst = QuestStorage.LookupEntry(quest_id);
	if(qst)
	{
		if (plr->HasFinishedQuest(quest_id))
			recout += "Player has already completed that quest.";
		else
		{
			QuestLogEntry * IsPlrOnQuest = plr->GetQuestLogForEntry(quest_id);
			if (IsPlrOnQuest)
				recout += "Player is currently on that quest.";
			else
			{
				int32 open_slot = plr->GetOpenQuestSlot();

				if (open_slot == -1)
				{
					sQuestMgr.SendQuestLogFull(plr);
					recout += "Player's quest log is full.";
				}
				else
				{
					sGMLog.writefromsession( m_session, "started quest %u [%s] for player %s", qst->id, qst->title, plr->GetName() );

					QuestLogEntry *qle = new QuestLogEntry();
					qle->Init(qst, plr, (uint32)open_slot);
					qle->UpdatePlayerFields();
		
					// If the quest should give any items on begin, give them the items.
					for(uint32 i = 0; i < 4; ++i)
					{
						if(qst->receive_items[i])
						{
							Item *item = objmgr.CreateItem( qst->receive_items[i], plr);
							if(!plr->GetItemInterface()->AddItemToFreeSlot(item))
								item->DeleteMe();
						}
					}

					if(qst->srcitem && qst->srcitem != qst->receive_items[0])
					{
						Item * item = objmgr.CreateItem( qst->srcitem, plr);
						if(item)
						{
							item->SetUInt32Value(ITEM_FIELD_STACK_COUNT, qst->srcitemcount ? qst->srcitemcount : 1);
							if(!plr->GetItemInterface()->AddItemToFreeSlot(item))
								item->DeleteMe();
						}
					}
				

					//if(qst->count_required_item || qst_giver->GetTypeId() == TYPEID_GAMEOBJECT)	// gameobject quests deactivate
					//	plr->UpdateNearbyGameObjects();
					//ScriptSystem->OnQuestEvent(qst, static_cast< Creature* >( qst_giver ), _player, QUEST_EVENT_ON_ACCEPT);
				
					sHookInterface.OnQuestAccept( plr, qst, NULL );

					recout += "Quest has been added to the player's quest log.";
				}
			}
		}
	}
	else
	{
		recout += "Quest Id [";
		recout += args;
		recout += "] was not found and unable to add it to the player's quest log.";
	}

	recout += "\n\n";

	SendMultilineMessage(m_session, recout.c_str());

	return true;
}

bool ChatHandler::HandleQuestFinishCommand(const char * args, WorldSession * m_session)
{
	if(!*args) return false;

	Player *plr = getSelectedChar(m_session, true);
	if(!plr)
	{
		plr = m_session->GetPlayer();
		SystemMessage(m_session, "Auto-targeting self.");
	}

	uint32 quest_id = atol(args);
	std::string recout = "|cff00ff00";

	Quest * qst = QuestStorage.LookupEntry(quest_id);
	if(qst)
	{
		if (plr->HasFinishedQuest(quest_id))
			recout += "Player has already completed that quest.\n\n";
		else
		{
			QuestLogEntry * IsPlrOnQuest = plr->GetQuestLogForEntry(quest_id);
			if (IsPlrOnQuest)
			{	
				uint32 giver_id = 0;
				std::string my_query = "";

				my_query = "SELECT id FROM creature_quest_starter WHERE quest = " + string(args);
				QueryResult *creatureResult = WorldDatabase.Query(my_query.c_str());

				if(creatureResult)
				{
					Field *creatureFields = creatureResult->Fetch();
					giver_id = creatureFields[0].GetUInt32();
					delete creatureResult;
				}
				else
				{
					my_query = "SELECT id FROM gameobject_quest_starter WHERE quest = " + string(args);
					QueryResult *objectResult = WorldDatabase.Query(my_query.c_str());
					if(objectResult)
					{
						Field *objectFields = objectResult->Fetch();
						giver_id = objectFields[0].GetUInt32();
						delete objectResult;
					}
				}

				if(giver_id == 0)
					SystemMessage(m_session, "Unable to find quest giver creature or object.");
				else
				{
					// I need some way to get the guid without targeting the creature or looking through all the spawns...
					Object *quest_giver = 0;

					for(uint32 guid=1; guid < plr->GetMapMgr()->m_CreatureArraySize; guid++)
					{
						Creature *pCreature = plr->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
						if(pCreature)
						{
							if(pCreature->GetEntry() == giver_id) //found creature
							{
								quest_giver = (Object*)pCreature;
								guid = plr->GetMapMgr()->m_CreatureArraySize;
							}
						}
					}

					if(quest_giver)
					{
						GreenSystemMessage(m_session, "Found a quest_giver creature.");
						//WorldPacket data;
						//sQuestMgr.BuildOfferReward(&data, qst, quest_giver, 1);
						//m_session->SendPacket(&data);
						sQuestMgr.GiveQuestRewardReputation(plr, qst, quest_giver);
					}
					else
						RedSystemMessage(m_session, "Unable to find quest_giver object.");
				}

				sQuestMgr.GenerateQuestXP(plr, qst);
				sQuestMgr.BuildQuestComplete(plr, qst);

				IsPlrOnQuest->Finish();
				recout += "Player was on that quest, but has now completed it.";
			}
			else
				recout += "The quest has now been completed for that player.";
			
			sGMLog.writefromsession( m_session, "completed quest %u [%s] for player %s", quest_id, qst->title, plr->GetName() );
			plr->AddToFinishedQuests(quest_id);
		}
	}
	else
	{
		recout += "Quest Id [";
		recout += args;
		recout += "] was not found and unable to add it to the player's quest log.";
	}

	recout += "\n\n";

	SendMultilineMessage(m_session, recout.c_str());

	return true;
}

bool ChatHandler::HandleQuestItemCommand(const char * args, WorldSession * m_session)
{
	if(!*args) return false;

	std::string my_item_lookup = "SELECT item, item_count FROM gameobject_quest_item_binding WHERE quest = " + string(args);

	QueryResult *result = WorldDatabase.Query(my_item_lookup.c_str());

	std::string recout;

	if(!result)
	{
		recout = "|cff00ccffNo matches found.\n\n";
		SendMultilineMessage(m_session, recout.c_str());
		return true;
	}

	recout = "|cff00ff00Quest item matches: itemid: count -> Name\n\n";
	SendMultilineMessage(m_session, recout.c_str());

	uint32 count = 0;

	do
	{
		Field *fields = result->Fetch();
		uint32 id = fields[0].GetUInt32();
		string itemid  = MyConvertIntToString(id);
		string itemcnt = MyConvertIntToString(fields[1].GetUInt32());
		ItemPrototype* tmpItem = ItemPrototypeStorage.LookupEntry(id);
		recout = "|cff00ccff";
		recout += itemid.c_str();
		recout += ": ";
		recout += itemcnt.c_str();
		recout += " -> ";
		recout += tmpItem->Name1;
		recout += "\n";

		SendMultilineMessage(m_session, recout.c_str());

		count++;
		
		if(count == 25)
		{
			RedSystemMessage(m_session, "More than 25 results returned. aborting.");
			break;
		}
	}while (result->NextRow());

	delete result;

	if (count == 0)
	{
		recout = "|cff00ccffNo matches found.\n\n";
		SendMultilineMessage(m_session, recout.c_str());
	}

	return true;
}

bool ChatHandler::HandleQuestGiverCommand(const char * args, WorldSession * m_session)
{
	if(!*args) return false;

	std::string recout;

	std::string my_query1 = "SELECT id FROM creature_quest_starter WHERE quest = " + string(args);
	QueryResult *objectResult1 = WorldDatabase.Query(my_query1.c_str());

	if(objectResult1)
	{
		Field *fields = objectResult1->Fetch();
		std::string creatureId1 = MyConvertIntToString(fields[0].GetUInt32());

		delete objectResult1;

		std::string creatureName1 = "N/A";
		CreatureInfo *creatureResult1 = CreatureNameStorage.LookupEntry(atol(creatureId1.c_str()));
		if (creatureResult1)
		{
			creatureName1 = creatureResult1->Name;

			my_query1 = "SELECT id FROM creature_spawns WHERE entry = " + creatureId1;
			QueryResult *spawnResult1 = WorldDatabase.Query(my_query1.c_str());

			string spawnId1;
			if(spawnResult1)
			{
				Field *fields = spawnResult1->Fetch();
				spawnId1 = fields[0].GetString();

				delete spawnResult1;
			}
			else
				spawnId1 = "N/A";

			recout = "|cff00ccffQuest Starter found: creature id, spawnid, name\n\n";
			SendMultilineMessage(m_session, recout.c_str());

			recout = "|cff00ccff";
			recout += creatureId1.c_str();
			recout += ", ";
			recout += spawnId1.c_str();
			recout += ", ";
			recout += creatureName1.c_str();
			recout += "\n\n";
			SendMultilineMessage(m_session, recout.c_str());
		}
		else
		{
			recout = "|cff00ccffNo creature quest starter info found.\n\n";
			SendMultilineMessage(m_session, recout.c_str());
		}

	}
	else
	{
		recout = "|cff00ccffNo creature quest starters found.\n\n";
		SendMultilineMessage(m_session, recout.c_str());
	}

	std::string my_query2 = "SELECT id FROM gameobject_quest_starter WHERE quest = " + string(args);
	QueryResult *objectResult2 = WorldDatabase.Query(my_query2.c_str());

	if(objectResult2)
	{
		Field *fields = objectResult2->Fetch();
		std::string itemId2 = MyConvertIntToString(fields[0].GetUInt32());

		delete objectResult2;

		std::string itemName2 = "N/A";
		ItemPrototype *itemResult2 = ItemPrototypeStorage.LookupEntry(atol(itemId2.c_str()));
		if (itemResult2)
		{
			itemName2 = itemResult2->Name1;

			my_query2 = "SELECT id FROM gameobject_spawns WHERE entry = " + itemId2;
			QueryResult *spawnResult2 = WorldDatabase.Query(my_query2.c_str());

			string spawnId2;
			if(spawnResult2)
			{
				Field *fields = spawnResult2->Fetch();
				spawnId2 = fields[0].GetString();

				delete spawnResult2;
			}
			else
				spawnId2 = "N/A";

			recout = "|cff00ccffQuest starter found: object id, spawnid, name\n\n";
			SendMultilineMessage(m_session, recout.c_str());

			recout = "|cff00ccff";
			recout += itemId2.c_str();
			recout += ", ";
			recout += spawnId2.c_str();
			recout += ", ";
			recout += itemName2.c_str();
			recout += "\n\n";
			SendMultilineMessage(m_session, recout.c_str());
		}
		else
		{
			recout = "|cff00ccffNo object quest starter info found.\n\n";
			SendMultilineMessage(m_session, recout.c_str());
		}
	}
	else
	{
		recout = "|cff00ccffNo object quest starters found.\n\n";
		SendMultilineMessage(m_session, recout.c_str());
	}

	return true;
}

bool ChatHandler::HandleQuestListCommand(const char * args, WorldSession * m_session)
{
	uint32 quest_giver = 0;
	if(*args)
		quest_giver = atol(args);
	else
	{
		uint64 guid = m_session->GetPlayer()->GetSelection();
		if (guid == 0)
		{
			SystemMessage(m_session, "You must target an npc or specify an id.");
			return true;
		}

		Creature *unit = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
		if(unit)
		{
			if (!unit->isQuestGiver())
			{
				SystemMessage(m_session, "Unit is not a valid quest giver.");
				return true;
			}

			if (!unit->HasQuests())
			{
				SystemMessage(m_session, "NPC does not have any quests.");
				return true;
			}

			quest_giver = unit->GetEntry();
		}
	}

	string recout = "|cff00ff00Quest matches: id: title\n\n";
	SendMultilineMessage(m_session, recout.c_str());

	uint32 count = 0;
	uint32 quest_id = 0;
	Quest * qst;
	Field *fields;

	if(quest_giver == 0)
	{
		Player *plr = getSelectedChar(m_session, true);
		if(!plr)
		{
			plr = m_session->GetPlayer();
			SystemMessage(m_session, "Auto-targeting self.");
		}

		if(plr)
		{
			if(plr->HasQuests())
			{
				QueryResult *playerResult = CharacterDatabase.Query("SELECT quest_id FROM questlog WHERE player_guid=%u", plr->GetLowGUID());
				if(playerResult)
				{
					do 
					{
						fields = playerResult->Fetch();
						quest_id = fields[0].GetUInt32();

						qst = QuestStorage.LookupEntry(quest_id);

						string qid  = MyConvertIntToString(quest_id);
						const char * qname = qst->title;

						recout = "|cff00ccff";
						recout += qid.c_str();
						recout += ": ";
						recout += qname;
						recout += "\n";

						SendMultilineMessage(m_session, recout.c_str());

						count++;
						
						if(count == 25)
						{
							RedSystemMessage(m_session, "More than 25 results returned. aborting.");
							break;
						}

					} while(playerResult->NextRow());

					delete playerResult;
				}
			}
		}
	}
	else
	{
		QueryResult *creatureResult = WorldDatabase.Query("SELECT quest FROM creature_quest_starter WHERE id = %u", quest_giver);

		if(!creatureResult)
		{
			recout = "|cff00ccffNo quests found for the specified NPC id.\n\n";
			SendMultilineMessage(m_session, recout.c_str());
			return true;
		}

		do
		{
			Field *fields = creatureResult->Fetch();
			uint32 quest_id = fields[0].GetUInt32();

			qst = QuestStorage.LookupEntry(quest_id);
			if(qst==NULL)
				continue;

			string qid  = MyConvertIntToString(quest_id);
			const char * qname = qst->title;

			recout = "|cff00ccff";
			recout += qid.c_str();
			recout += ": ";
			recout += qname;
			recout += "\n";

			SendMultilineMessage(m_session, recout.c_str());

			count++;
			
			if(count == 25)
			{
				RedSystemMessage(m_session, "More than 25 results returned. aborting.");
				break;
			}
		}while (creatureResult->NextRow());

		delete creatureResult;
	}

	if (count == 0)
	{
		recout = "|cff00ccffNo matches found.\n\n";
		SendMultilineMessage(m_session, recout.c_str());
	}

	return true;
}

bool ChatHandler::HandleQuestAddStartCommand(const char * args, WorldSession * m_session)
{
	if(!*args) return false;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
	{
		SystemMessage(m_session, "You must target an npc.");
		return false;
	}

	Creature *unit = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!unit)
	{
		SystemMessage(m_session, "You must target an npc.");
		return false;
	}

	if (!unit->isQuestGiver())
	{
		SystemMessage(m_session, "Unit is not a valid quest giver.");
		return false;
	}

	uint32 quest_id = atol(args);
	Quest * qst = QuestStorage.LookupEntry(quest_id);

	if (qst == NULL)
	{
		SystemMessage(m_session, "Invalid quest selected, unable to add quest to the specified NPC.");
		return false;
	}

	std::string quest_giver = MyConvertIntToString(unit->GetEntry());

	std::string my_query1 = "SELECT id FROM creature_quest_starter WHERE id = " + quest_giver + " AND quest = " + string(args);
	QueryResult *selectResult1 = WorldDatabase.Query(my_query1.c_str());
	if (selectResult1)
	{
		delete selectResult1;
		SystemMessage(m_session, "Quest was already found for the specified NPC.");
	}
	else
	{
		std::string my_insert1 = "INSERT INTO creature_quest_starter (id, quest) VALUES (" + quest_giver + "," + string(args) + ")";
		QueryResult *insertResult1 = WorldDatabase.Query(my_insert1.c_str());
		if (insertResult1)
			delete insertResult1;
	}

	std::string my_query2 = "SELECT id FROM gameobject_quest_starter WHERE id = " + quest_giver + " AND quest = " + string(args);
	QueryResult *selectResult2 = WorldDatabase.Query(my_query2.c_str());
	if (selectResult2)
		delete selectResult2;
	else
	{
		std::string my_insert2 = "INSERT INTO gameobject_quest_starter (id, quest) VALUES (" + quest_giver + "," + string(args) + ")";
		QueryResult *insertResult2 = WorldDatabase.Query(my_insert2.c_str());
		if (insertResult2)
			delete insertResult2;
	}

	sQuestMgr.LoadExtraQuestStuff();

	QuestRelation *qstrel = new QuestRelation;
	qstrel->qst = qst;
	qstrel->type = QUESTGIVER_QUEST_START;
	uint8 qstrelid = (uint8)unit->GetQuestRelation(quest_id);
	unit->FindQuest(quest_id, qstrelid);
	unit->AddQuest(qstrel);
	unit->_LoadQuests();

	const char * qname = qst->title;

	std::string recout = "|cff00ff00Added Quest to NPC as starter: ";
	recout += "|cff00ccff";
	recout += qname;
	recout += "\n\n";
	SendMultilineMessage(m_session, recout.c_str());
	sGMLog.writefromsession( m_session, "added starter of quest %u [%s] to NPC %u [%s]", qst->id, qst->title, unit->GetEntry(), unit->GetCreatureInfo()->Name );

	return true;
}

bool ChatHandler::HandleQuestAddFinishCommand(const char * args, WorldSession * m_session)
{
	if(!*args)
		return false;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
	{
		SystemMessage(m_session, "You must target an npc.");
		return false;
	}

	Creature *unit = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!unit)
	{
		SystemMessage(m_session, "You must target an npc.");
		return false;
	}

	if (!unit->isQuestGiver())
	{
		SystemMessage(m_session, "Unit is not a valid quest giver.");
		return false;
	}

	uint32 quest_id = atol(args);
	Quest * qst = QuestStorage.LookupEntry(quest_id);

	if (qst == NULL)
	{
		SystemMessage(m_session, "Invalid quest selected, unable to add quest to the specified NPC.");
		return false;
	}

	std::string quest_giver = MyConvertIntToString(unit->GetEntry());

	std::string my_query1 = "SELECT id FROM creature_quest_finisher WHERE id = " + quest_giver + " AND quest = " + string(args);
	QueryResult *selectResult1 = WorldDatabase.Query(my_query1.c_str());
	if (selectResult1)
	{
		delete selectResult1;
		SystemMessage(m_session, "Quest was already found for the specified NPC.");
	}
	else
	{
		std::string my_insert1 = "INSERT INTO creature_quest_finisher (id, quest) VALUES (" + quest_giver + "," + string(args) + ")";
		QueryResult *insertResult1 = WorldDatabase.Query(my_insert1.c_str());
		if (insertResult1)
			delete insertResult1;
	}

	std::string my_query2 = "SELECT id FROM gameobject_quest_finisher WHERE id = " + quest_giver + " AND quest = " + string(args);
	QueryResult *selectResult2 = WorldDatabase.Query(my_query2.c_str());
	if (selectResult2)
		delete selectResult2;
	else
	{
		string my_insert2 = "INSERT INTO gameobject_quest_finisher (id, quest) VALUES (" + quest_giver + "," + string(args) + ")";
		QueryResult *insertResult2 = WorldDatabase.Query(my_insert2.c_str());
		if (insertResult2)
			delete insertResult2;
	}

	sQuestMgr.LoadExtraQuestStuff();

	QuestRelation *qstrel = new QuestRelation;
	qstrel->qst = qst;
	qstrel->type = QUESTGIVER_QUEST_END;
	uint8 qstrelid = (uint8)unit->GetQuestRelation(quest_id);
	unit->FindQuest(quest_id, qstrelid);
	unit->AddQuest(qstrel);
	unit->_LoadQuests();

	const char * qname = qst->title;

	std::string recout = "|cff00ff00Added Quest to NPC as finisher: ";
	recout += "|cff00ccff";
	recout += qname;
	recout += "\n\n";
	SendMultilineMessage(m_session, recout.c_str());
	sGMLog.writefromsession( m_session, "added finisher of quest %u [%s] to NPC %u [%s]", qst->id, qst->title, unit->GetEntry(), unit->GetCreatureInfo()->Name );

	return true;
}

bool ChatHandler::HandleQuestAddBothCommand(const char * args, WorldSession * m_session)
{
	if(!*args)
		return false;

	bool bValid = ChatHandler::HandleQuestAddStartCommand(args, m_session);

	if (bValid)
		ChatHandler::HandleQuestAddFinishCommand(args, m_session);

	return true;
}

bool ChatHandler::HandleQuestDelStartCommand(const char * args, WorldSession * m_session)
{
	if(!*args)
		return false;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
	{
		SystemMessage(m_session, "You must target an npc.");
		return false;
	}

	Creature *unit = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!unit)
	{
		SystemMessage(m_session, "You must target an npc.");
		return false;
	}

	if (!unit->isQuestGiver())
	{
		SystemMessage(m_session, "Unit is not a valid quest giver.");
		return false;
	}

	uint32 quest_id = atol(args);
	Quest * qst = QuestStorage.LookupEntry(quest_id);

	if (qst == NULL)
	{
		SystemMessage(m_session, "Invalid Quest selected.");
		return false;
	}

	std::string quest_giver = MyConvertIntToString(unit->GetEntry());

	std::string my_query1 = "SELECT id FROM creature_quest_starter WHERE id = " + quest_giver + " AND quest = " + string(args);
	QueryResult *selectResult1 = WorldDatabase.Query(my_query1.c_str());
	if (selectResult1)
		delete selectResult1;
	else
	{
		SystemMessage(m_session, "Quest was NOT found for the specified NPC.");
		return false;
	}

	std::string my_delete1 = "DELETE FROM creature_quest_starter WHERE id = " + quest_giver + " AND quest = " + string(args);
	QueryResult *deleteResult1 = WorldDatabase.Query(my_delete1.c_str());
	if (deleteResult1)
		delete deleteResult1;

	std::string my_query2 = "SELECT id FROM gameobject_quest_starter WHERE id = " + quest_giver + " AND quest = " + string(args);
	QueryResult *selectResult2 = WorldDatabase.Query(my_query2.c_str());
	if (selectResult2)
	{
		delete selectResult2;

		std::string my_delete2 = "DELETE FROM gameobject_quest_starter WHERE id = " + quest_giver + " AND quest = " + string(args);
		QueryResult *deleteResult2 = WorldDatabase.Query(my_delete2.c_str());
		if (deleteResult2)
			delete deleteResult2;
	}

	sQuestMgr.LoadExtraQuestStuff();

	QuestRelation *qstrel = new QuestRelation;
	qstrel->qst = qst;
	qstrel->type = QUESTGIVER_QUEST_START;
	uint8 qstrelid = (uint8)unit->GetQuestRelation(quest_id);
	unit->FindQuest(quest_id, qstrelid);
	unit->DeleteQuest(qstrel);
	unit->_LoadQuests();

	const char * qname = qst->title;

	std::string recout = "|cff00ff00Deleted Quest from NPC: ";
	recout += "|cff00ccff";
	recout += qname;
	recout += "\n\n";
	SendMultilineMessage(m_session, recout.c_str());
	sGMLog.writefromsession( m_session, "deleted starter of quest %u [%s] to NPC %u [%s]", qst->id, qst->title, unit->GetEntry(), unit->GetCreatureInfo()->Name );

	return true;
}

bool ChatHandler::HandleQuestDelFinishCommand(const char * args, WorldSession * m_session)
{
	if(!*args)
		return false;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
	{
		SystemMessage(m_session, "You must target an npc.");
		return false;
	}

	Creature *unit = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!unit)
	{
		SystemMessage(m_session, "You must target an npc.");
		return false;
	}

	if (!unit->isQuestGiver())
	{
		SystemMessage(m_session, "Unit is not a valid quest giver.");
		return false;
	}

	uint32 quest_id = atol(args);
	Quest * qst = QuestStorage.LookupEntry(quest_id);

	if (qst == NULL)
	{
		SystemMessage(m_session, "Invalid Quest selected.");
		return false;
	}

	std::string quest_giver = MyConvertIntToString(unit->GetEntry());

	std::string my_query1 = "SELECT id FROM creature_quest_finisher WHERE id = " + quest_giver + " AND quest = " + string(args);
	QueryResult *selectResult1 = WorldDatabase.Query(my_query1.c_str());
	if (selectResult1)
		delete selectResult1;
	else
	{
		SystemMessage(m_session, "Quest was NOT found for the specified NPC.");
		return true;
	}

	std::string my_delete1 = "DELETE FROM creature_quest_finisher WHERE id = " + quest_giver + " AND quest = " + string(args);
	QueryResult *deleteResult1 = WorldDatabase.Query(my_delete1.c_str());
	if (deleteResult1)
		delete deleteResult1;

	std::string my_query2 = "SELECT id FROM gameobject_quest_finisher WHERE id = " + quest_giver + " AND quest = " + string(args);
	QueryResult *selectResult2 = WorldDatabase.Query(my_query2.c_str());
	if (selectResult2)
	{
		delete selectResult2;

		std::string my_delete2 = "DELETE FROM gameobject_quest_finisher WHERE id = " + quest_giver + " AND quest = " + string(args);
		QueryResult *deleteResult2 = WorldDatabase.Query(my_delete2.c_str());
		if (deleteResult2)
			delete deleteResult2;
	}

	sQuestMgr.LoadExtraQuestStuff();

	QuestRelation *qstrel = new QuestRelation;
	qstrel->qst = qst;
	qstrel->type = QUESTGIVER_QUEST_END;
	uint8 qstrelid = (uint8)unit->GetQuestRelation(quest_id);
	unit->FindQuest(quest_id, qstrelid);
	unit->DeleteQuest(qstrel);
	unit->_LoadQuests();

	const char * qname = qst->title;

	std::string recout = "|cff00ff00Deleted Quest from NPC: ";
	recout += "|cff00ccff";
	recout += qname;
	recout += "\n\n";
	SendMultilineMessage(m_session, recout.c_str());
	sGMLog.writefromsession( m_session, "deleted finisher of quest %u [%s] to NPC %u [%s]", qst->id, qst->title, unit->GetEntry(), unit->GetCreatureInfo()->Name );

	return true;
}

bool ChatHandler::HandleQuestDelBothCommand(const char * args, WorldSession * m_session)
{
	if(!*args)
		return false;

	bool bValid = ChatHandler::HandleQuestDelStartCommand(args, m_session);

	if (bValid)
		ChatHandler::HandleQuestDelFinishCommand(args, m_session);

	return true;
}

bool ChatHandler::HandleQuestFinisherCommand(const char * args, WorldSession * m_session)
{
	if(!*args) return false;

	std::string recout;

	std::string my_query1 = "SELECT id FROM creature_quest_finisher WHERE quest = " + string(args);
	QueryResult *objectResult1 = WorldDatabase.Query(my_query1.c_str());

	if(objectResult1)
	{
		Field *fields = objectResult1->Fetch();
		std::string creatureId1 = MyConvertIntToString(fields[0].GetUInt32());

		delete objectResult1;

		std::string creatureName1 = "N/A";
		CreatureInfo *creatureResult1 = CreatureNameStorage.LookupEntry(atol(creatureId1.c_str()));

		if(creatureResult1)
		{
			creatureName1 = creatureResult1->Name;

			my_query1 = "SELECT id FROM creature_spawns WHERE entry = " + creatureId1;
			QueryResult *spawnResult1 = WorldDatabase.Query(my_query1.c_str());

			string spawnId1;
			if(spawnResult1)
			{
				Field *fields = spawnResult1->Fetch();
				spawnId1 = fields[0].GetString();

				delete spawnResult1;
			}
			else
				spawnId1 = "N/A";

			recout = "|cff00ccffQuest Finisher found: creature id, spawnid, name\n\n";
			SendMultilineMessage(m_session, recout.c_str());

			recout = "|cff00ccff";
			recout += creatureId1.c_str();
			recout += ", ";
			recout += spawnId1.c_str();
			recout += ", ";
			recout += creatureName1.c_str();
			recout += "\n\n";
			SendMultilineMessage(m_session, recout.c_str());
		}
		else
		{
			recout = "|cff00ccffNo creature quest finisher info found.\n\n";
			SendMultilineMessage(m_session, recout.c_str());
		}
	}
	else
	{
		recout = "|cff00ccffNo creature quest finishers found.\n\n";
		SendMultilineMessage(m_session, recout.c_str());
	}

	std::string my_query2 = "SELECT id FROM gameobject_quest_finisher WHERE quest = " + string(args);
	QueryResult *objectResult2 = WorldDatabase.Query(my_query2.c_str());

	if(objectResult2)
	{
		Field *fields = objectResult2->Fetch();
		std::string itemId2 = MyConvertIntToString(fields[0].GetUInt32());

		delete objectResult2;

		std::string itemName2 = "N/A";
		ItemPrototype *itemResult2 = ItemPrototypeStorage.LookupEntry(atol(itemId2.c_str()));
		if (itemResult2)
		{
			itemName2 = itemResult2->Name1;

			my_query2 = "SELECT id FROM gameobject_spawns WHERE entry = " + itemId2;
			QueryResult *spawnResult2 = WorldDatabase.Query(my_query2.c_str());

			string spawnId2;
			if(spawnResult2)
			{
				Field *fields = spawnResult2->Fetch();
				spawnId2 = fields[0].GetString();

				delete spawnResult2;
			}
			else
				spawnId2 = "N/A";

			recout = "|cff00ccffQuest Finisher found: object id, spawnid, name\n\n";
			SendMultilineMessage(m_session, recout.c_str());

			recout = "|cff00ccff";
			recout += itemId2.c_str();
			recout += ", ";
			recout += spawnId2.c_str();
			recout += ", ";
			recout += itemName2.c_str();
			recout += "\n\n";
			SendMultilineMessage(m_session, recout.c_str());
		}
		else
		{
			recout = "|cff00ccffNo object quest finisher info found.\n\n";
			SendMultilineMessage(m_session, recout.c_str());
		}
	}
	else
	{
		recout = "|cff00ccffNo object quest finishers found.\n\n";
		SendMultilineMessage(m_session, recout.c_str());
	}

	return true;
}

bool ChatHandler::HandleQuestSpawnCommand(const char * args, WorldSession * m_session)
{
	if(!*args) return false;

	std::string recout;

	std::string my_query = "SELECT id FROM creature_quest_starter WHERE quest = " + string(args);
	QueryResult *objectResult = WorldDatabase.Query(my_query.c_str());

	string starterId;
	if(objectResult)
	{
		Field *fields = objectResult->Fetch();
		starterId = MyConvertIntToString(fields[0].GetUInt32());
	}
	else
	{
		recout = "|cff00ccffNo quest starters found.\n\n";
		SendMultilineMessage(m_session, recout.c_str());
		return true;
	}

	delete objectResult;

	std::string starterName = "N/A";
	CreatureInfo *creatureResult = CreatureNameStorage.LookupEntry(atol(starterId.c_str()));

	if(creatureResult)
		starterName = creatureResult->Name;
	else
	{
		recout = "|cff00ccffNo quest starter info found.\n\n";
		SendMultilineMessage(m_session, recout.c_str());
		return true;
	}

	my_query = "SELECT map, position_x, position_y, position_z FROM creature_spawns WHERE entry = " + starterId;
	QueryResult *spawnResult = WorldDatabase.Query(my_query.c_str());

	if(!spawnResult)
	{
		recout = "|cff00ccffNo spawn location for quest starter was found.\n\n";
		SendMultilineMessage(m_session, recout.c_str());
		return true;
	}

	Field *fields = spawnResult->Fetch();
	uint32 locmap = fields[0].GetUInt32();
	float x = fields[1].GetFloat();
	float y = fields[2].GetFloat();
	float z = fields[3].GetFloat();

	delete spawnResult;

	recout = "|cff00ccffPorting to Quest Starter/Giver: id, name\n\n";
	SendMultilineMessage(m_session, recout.c_str());

	recout = "|cff00ccff";
	recout += starterId.c_str();
	recout += ", ";
    recout += starterName.c_str();
	recout += "\n\n";
	SendMultilineMessage(m_session, recout.c_str());

	m_session->GetPlayer()->SafeTeleport(locmap, 0, LocationVector(x, y, z));

	return true;
}

bool ChatHandler::HandleQuestLoadCommand(const char * args, WorldSession * m_session)
{
	BlueSystemMessage(m_session, "Load of quests from the database has been initiated ...", "");
	uint32 t = getMSTime();

	sQuestMgr.LoadExtraQuestStuff();

	BlueSystemMessage(m_session, "Load completed in %u ms.", getMSTime() - t);

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
		return true;

	Creature *unit = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!unit)
		return true;

	if (!unit->isQuestGiver())
		return true;

	// If player targeted a questgiver assume they want the NPC reloaded, too
	unit->_LoadQuests();

	return true;
}

bool ChatHandler::HandleQuestRemoveCommand(const char * args, WorldSession * m_session)
{
	if(!*args) return false;

	Player *plr = getSelectedChar(m_session, true);
	if(!plr)
	{
		plr = m_session->GetPlayer();
		SystemMessage(m_session, "Auto-targeting self.");
	}

	string recout = "";
	uint32 quest_id = atol(args);
	Quest * qst = QuestStorage.LookupEntry(quest_id);

	if(qst)
	{
		recout = RemoveQuestFromPlayer(plr, qst);
		sGMLog.writefromsession( m_session, "removed quest %u [%s] from player %s%", qst->id, qst->title, plr->GetName() );
	}
	else
		recout = "Invalid quest selected, unable to remove.\n\n";

	SystemMessage(m_session, recout.c_str());

	return true;
}

bool ChatHandler::HandleQuestRewardCommand(const char * args, WorldSession * m_session)
{
	if(!*args) return false;

	stringstream recout;

	uint32 qu_id = atol(args);
	Quest* q = QuestStorage.LookupEntry(qu_id);
	if(q)
	{
		for( uint32 r = 0; r < q->count_reward_item; r++ )
		{
			uint32 itemid = q->reward_item[r];
			ItemPrototype *itemProto = ItemPrototypeStorage.LookupEntry(itemid);
			if(!itemProto)
			{
				recout << "Unknown item id %lu" << itemid;
				sLog.outError( "WORLD: Unknown item id 0x%08x", itemid );
			}
			else
			{
				recout << "Reward (" << itemid << "): " << GetItemLinkByProto(itemProto,m_session->language);
				if(q->reward_itemcount[r]==1)
					recout << "\n";
				else
					recout << "[x" << q->reward_itemcount[r] << "]\n";
			}
		}
		for( uint32 r = 0; r < q->count_reward_choiceitem; r++ )
		{
			uint32 itemid = q->reward_choiceitem[r];
			ItemPrototype *itemProto = ItemPrototypeStorage.LookupEntry(itemid);
			if(!itemProto)
			{
				recout << "Unknown item id %lu" << itemid;
				sLog.outError( "WORLD: Unknown item id 0x%08x", itemid );
			}
			else
			{
				recout << "Reward choice (" << itemid << "): " << GetItemLinkByProto(itemProto,m_session->language);
				if(q->reward_choiceitemcount[r] == 1)
					recout << "\n";
				else
					recout << "[x" << q->reward_choiceitemcount[r] << "]\n";
			}
		}
		if((q->count_reward_choiceitem == 0) && (q->count_reward_item == 0))
		{
			recout << "Quest " << qu_id << " has no item rewards.";
		}
	}
	else
	{
		recout << "Quest ID " << qu_id << " not found.\n";
		sLog.outError("Quest ID %lu not found.", qu_id);
	}

	SendMultilineMessage(m_session, recout.str().data());

	return true;
}
