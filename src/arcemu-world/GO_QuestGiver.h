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
#ifndef GO_QUESTGIVER_HPP
#define GO_QUESTGIVER_HPP
namespace Arcemu{
	/////////////////////////////////////////////////////////////////////////
	//class GO_QuestGiver
	// class implementing Type 2 (QUESTGIVER) GameObjects
	//
	/////////////////////////////////////////////////////////////////////////
	class GO_QuestGiver : public GameObject{
	public:
		GO_QuestGiver();
		GO_QuestGiver(uint64 GUID);
		~GO_QuestGiver();
		void InitAI();
		/////////////////////////////////////////////////////////////////////
		//bool HasQuests()
		// Tells if the GO has any quests
		//
		//Parameters
		// None
		//
		//Return Value
		// Returns true if the GO has quests
		// Returns false otherwise
		//
		//
		/////////////////////////////////////////////////////////////////////
		bool HasQuests(){
			if (m_quests == NULL)
				return false;
			if (m_quests->size() == 0)
				return false;
			return true;
		};
		////////////////////////////////////////////////////////////////////
		//uint32 NumOfQuests()
		// Returns the number of quests
		//
		//Parameters
		// None
		//
		//Return Value
		// Returns the number of quests if the GO has quests
		// Returns 0 otherwise
		//
		//
		////////////////////////////////////////////////////////////////////
		uint32 NumOfQuests(){
			return static_cast< uint32 >(m_quests->size());
		}
		////////////////////////////////////////////////////////////////////
		//void AddQuest( QuestRelation *Q )
		// Adds a quest to the GO
		//
		//Parameters
		// QuestRelation *Q - Pointer to the QuestRelation to add
		//
		//Return Value
		// None
		//
		//
		////////////////////////////////////////////////////////////////////
		void AddQuest(QuestRelation *Q){
			m_quests->push_back(Q);
		}
		////////////////////////////////////////////////////////////////////
		//void DeleteQuest( QuestRelation *Q )
		// Removes a Quest from the GO
		//
		//Parameters
		// QuestRelation *Q - Pointer to the QuestRelation to remove
		//
		//Return Value
		// None
		//
		//
		////////////////////////////////////////////////////////////////////
		void DeleteQuest(QuestRelation *Q);
		////////////////////////////////////////////////////////////////////
		//Quest *FindQuest( uint32 quest_id, uint8 quest_relation )
		// Searches for a QuestRelation in the GO and if found,
		// returns the Quest
		//
		//Parameter
		// uint32 quest_id - Identifier of the Quest
		// uint8 quest_relation - QuestRelation type
		//
		//Return Value
		// Returns the Quest on success
		// Returns NULL on failure
		//
		////////////////////////////////////////////////////////////////////
		Quest *FindQuest(uint32 quest_id, uint8 quest_relation);
		////////////////////////////////////////////////////////////////////
		//uint16 GetQuestRelation( uint32 quest_id )
		// Finds the Quest with quest_id in the GO (if there's such), and
		// returns it's QuestRelation type
		//
		//Paramaters
		// uint32 quest_id - Identifier of the Quest
		//
		//Return Value
		// Returns the QuestRelation type on success
		// Returns 0 on failure
		//
		//
		////////////////////////////////////////////////////////////////////
		uint16 GetQuestRelation(uint32 quest_id);
		///////////////////////////////////////////////////////////////////
		//std::list< QuestRelation* >::iterator QuestsBegin()
		// Returns an iterator to the GO's QuestRelation list beginning
		//
		//
		//Parameters
		// None
		//
		//Return Value
		// Returns an iterator to the QuestRelation list's beginning
		//
		//
		///////////////////////////////////////////////////////////////////
		std::list< QuestRelation* >::iterator QuestsBegin(){
			return m_quests->begin();
		};
		///////////////////////////////////////////////////////////////////
		//std::list< QuestRelation* >::iterator QuestsEnd()
		// Returns an iterator to the GO's QuestRelation list end
		//
		//
		//Parameters
		// None
		//
		//Return Value
		// Returns an iterator to the QuestRelation list's end
		//
		//
		///////////////////////////////////////////////////////////////////
		std::list< QuestRelation* >::iterator QuestsEnd(){
			return m_quests->end();
		};
		///////////////////////////////////////////////////////////////////
		//void SetQuestList( std::list< QuestRelation* >* qst_lst )
		// Initializes the QuestRelation list with another
		//
		//Parameters
		// std::list< QuestRelation* >* qst_lst - pointer to the other list
		//
		//Return Value
		// None
		//
		//
		///////////////////////////////////////////////////////////////////
		void SetQuestList(std::list< QuestRelation* >* qst_lst){
			m_quests = qst_lst;
		};
	private:
		///////////////////////////////////////////////////////////////////
		//void LoadQuests()
		// Loads the QuestRelations from QuestMgr for this GO
		//
		//Parameters
		// None
		//
		//Return Value
		// None
		//
		//
		///////////////////////////////////////////////////////////////////
		void LoadQuests(){ sQuestMgr.LoadGOQuests(this); }
		std::list< QuestRelation * >* m_quests;
	};
}
#endif