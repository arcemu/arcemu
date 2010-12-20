#pragma  once
//#include "StdAfx.h"
#include "LUAEngine.h"

class LuaQuest : public QuestScript
{
	public:
	uint32 entry;
	PObjectBinding m_binding;
		LuaQuest(uint32 id) : QuestScript(), entry(id) {}
		~LuaQuest()
		{
			for(li::QuestInterfaceMap::iterator itr = lua_instance->m_questInterfaceMap.find(entry); itr != lua_instance->m_questInterfaceMap.upper_bound(entry); ++itr)
			{
				if(itr->second == this)
				{
					lua_instance->m_questInterfaceMap.erase(itr);
					break;
				}
			}
			for(li::ObjectFRefMap::iterator it,itr = lua_instance->m_questFRefs.find(entry); itr != lua_instance->m_questFRefs.upper_bound(entry); )
			{
				it = itr++;
				cleanup_varparam(it->second, lua_state);
				lua_instance->m_questFRefs.erase(it);
			}
		}

		void OnQuestStart(Player* mTarget, QuestLogEntry *qLogEntry)
		{

			NULL_BINDING_CHECK
			lua_engine::BeginLuaFunctionCall(m_binding->refs[QUEST_EVENT_ON_ACCEPT]);
			push_unit(mTarget);
			push_int(qLogEntry->GetQuest()->id);
			lua_engine::ExecuteLuaFunction(2);
			RELEASE_LOCK
		}

		void OnQuestComplete(Player* mTarget, QuestLogEntry *qLogEntry)
		{

			NULL_BINDING_CHECK
				lua_engine::BeginLuaFunctionCall(m_binding->refs[QUEST_EVENT_ON_COMPLETE]);
			push_unit(mTarget);
			push_int(qLogEntry->GetQuest()->id);
			lua_engine::ExecuteLuaFunction(2);
			RELEASE_LOCK
		}
		void OnQuestCancel(Player* mTarget)
		{
			NULL_BINDING_CHECK
				lua_engine::BeginLuaFunctionCall(m_binding->refs[QUEST_EVENT_ON_CANCEL]);
			push_unit(mTarget);
			lua_engine::ExecuteLuaFunction(1);
			RELEASE_LOCK
		}
		void OnGameObjectActivate(uint32 entry, Player* mTarget, QuestLogEntry *qLogEntry)
		{
			NULL_BINDING_CHECK
				lua_engine::BeginLuaFunctionCall(m_binding->refs[QUEST_EVENT_GAMEOBJECT_ACTIVATE]);
			push_int(entry);
			push_unit(mTarget);
			push_int(qLogEntry->GetQuest()->id);
			lua_engine::ExecuteLuaFunction(3);
			RELEASE_LOCK
		}
		void OnCreatureKill(uint32 entry, Player* mTarget, QuestLogEntry *qLogEntry)
		{
			NULL_BINDING_CHECK
				lua_engine::BeginLuaFunctionCall(m_binding->refs[QUEST_EVENT_ON_CREATURE_KILL]);
			push_int(entry);
			push_unit(mTarget);
			push_int(qLogEntry->GetQuest()->id);
			lua_engine::ExecuteLuaFunction(3);
			RELEASE_LOCK
		}
		void OnExploreArea(uint32 areaId, Player* mTarget, QuestLogEntry *qLogEntry)
		{
			NULL_BINDING_CHECK
				lua_engine::BeginLuaFunctionCall(m_binding->refs[QUEST_EVENT_ON_EXPLORE_AREA]);
			push_int(areaId);
			push_unit(mTarget);
			push_int(qLogEntry->GetQuest()->id);
			lua_engine::ExecuteLuaFunction(3);
			RELEASE_LOCK
		}
		void OnPlayerItemPickup(uint32 itemId, uint32 totalCount, Player* mTarget, QuestLogEntry *qLogEntry)
		{
			NULL_BINDING_CHECK
			lua_engine::BeginLuaFunctionCall(m_binding->refs[QUEST_EVENT_ON_PLAYER_ITEMPICKUP]);
			push_int(itemId);
			push_int(totalCount);
			push_unit(mTarget);
			push_int(qLogEntry->GetQuest()->id);
			lua_engine::ExecuteLuaFunction(4);
			RELEASE_LOCK
		}
	};

namespace lua_engine
{
	QuestScript * createluaquest(uint32 id)
	{
		LuaQuest * pLua = NULL;
		li::ObjectBindingMap::iterator itr = lua_instance->m_questBinding.find(id);
		PObjectBinding pBinding = (itr != lua_instance->m_questBinding.end() ) ? itr->second : NULL;
		if(pBinding != NULL)
		{
			pLua = new LuaQuest(id);
			pLua->m_binding = pBinding;
			lua_instance->m_questInterfaceMap.insert( make_pair(id, pLua) );
		}
		return pLua;
	}
	
}
