

#include "LUAEngine.h"

LuaQuest::LuaQuest(uint32 id) : entry(id), QuestScript() {}
LuaQuest::~LuaQuest()
{
	PLUA_INSTANCE li_ = lua_instance.get();

	for(std::pair<li::QuestInterfaceMap::iterator, li::QuestInterfaceMap::iterator> interfaces = li_->m_questInterfaceMap.equal_range(entry); interfaces.first != interfaces.second; ++interfaces.first)
	{
		if(interfaces.first->second == this)
		{
			li_->m_questInterfaceMap.erase(interfaces.first);
			break;
		}
	}
	li::ObjectFRefMap::iterator it;
	for(std::pair<li::ObjectFRefMap::iterator, li::ObjectFRefMap::iterator> frefs = li_->m_questFRefs.equal_range(entry); frefs.first != frefs.second;)
	{
		it = frefs.first++;
		cleanup_varparam(it->second, li_->lu);
		li_->m_questFRefs.erase(it);
	}
}

void LuaQuest::OnQuestStart(Player* mTarget, QuestLogEntry* qLogEntry)
{
	NULL_BINDING_CHECK
	lua_engine::BeginLuaFunctionCall(m_binding->refs[QUEST_EVENT_ON_ACCEPT]);
	push_player(mTarget);
	push_int(qLogEntry->GetQuest()->id);
	lua_engine::ExecuteLuaFunction(2);
	RELEASE_LOCK
}

void LuaQuest::OnQuestComplete(Player* mTarget, QuestLogEntry* qLogEntry)
{
	NULL_BINDING_CHECK
	lua_engine::BeginLuaFunctionCall(m_binding->refs[QUEST_EVENT_ON_COMPLETE]);
	push_player(mTarget);
	push_int(qLogEntry->GetQuest()->id);
	lua_engine::ExecuteLuaFunction(2);
	RELEASE_LOCK
}

void LuaQuest::OnQuestCancel(Player* mTarget)
{
	NULL_BINDING_CHECK
	lua_engine::BeginLuaFunctionCall(m_binding->refs[QUEST_EVENT_ON_CANCEL]);
	push_player(mTarget);
	lua_engine::ExecuteLuaFunction(1);
	RELEASE_LOCK
}

void LuaQuest::OnGameObjectActivate(uint32 entry, Player* mTarget, QuestLogEntry* qLogEntry)
{
	NULL_BINDING_CHECK
	lua_engine::BeginLuaFunctionCall(m_binding->refs[QUEST_EVENT_GAMEOBJECT_ACTIVATE]);
	push_int(entry);
	push_player(mTarget);
	push_int(qLogEntry->GetQuest()->id);
	lua_engine::ExecuteLuaFunction(3);
	RELEASE_LOCK
}

void LuaQuest::OnCreatureKill(uint32 entry, Player* mTarget, QuestLogEntry* qLogEntry)
{
	NULL_BINDING_CHECK
	lua_engine::BeginLuaFunctionCall(m_binding->refs[QUEST_EVENT_ON_CREATURE_KILL]);
	push_int(entry);
	push_player(mTarget);
	push_int(qLogEntry->GetQuest()->id);
	lua_engine::ExecuteLuaFunction(3);
	RELEASE_LOCK
}

void LuaQuest::OnExploreArea(uint32 areaId, Player* mTarget, QuestLogEntry* qLogEntry)
{
	NULL_BINDING_CHECK
	lua_engine::BeginLuaFunctionCall(m_binding->refs[QUEST_EVENT_ON_EXPLORE_AREA]);
	push_int(areaId);
	push_player(mTarget);
	push_int(qLogEntry->GetQuest()->id);
	lua_engine::ExecuteLuaFunction(3);
	RELEASE_LOCK
}

void LuaQuest::OnPlayerItemPickup(uint32 itemId, uint32 totalCount, Player* mTarget, QuestLogEntry* qLogEntry)
{
	NULL_BINDING_CHECK
	lua_engine::BeginLuaFunctionCall(m_binding->refs[QUEST_EVENT_ON_PLAYER_ITEMPICKUP]);
	push_int(itemId);
	push_int(totalCount);
	push_player(mTarget);
	push_int(qLogEntry->GetQuest()->id);
	lua_engine::ExecuteLuaFunction(4);
	RELEASE_LOCK
}

namespace lua_engine
{
	QuestScript* createluaquest(uint32 id)
	{
		LuaQuest* pLua = NULL;
		PLUA_INSTANCE ref = lua_instance.get();
		li::ObjectBindingMap::iterator itr = ref->m_questBinding.find(id);
		PObjectBinding pBinding = (itr != ref->m_questBinding.end()) ? itr->second : NULL;
		if(pBinding != NULL)
		{
			pLua = new LuaQuest(id);
			pLua->m_binding = pBinding;
			ref->m_questInterfaceMap.insert(make_pair(id, pLua));
		}
		return pLua;
	}

	void bindQuestMethods(luabridge::module & m)
	{
		m	.class_<QuestLogEntry>("QuestLogEntry")
		.method(&QuestLogEntry::CanBeFinished, "CanBeFinished", "canBeFinished", "canbefinished", NULL)
		.method(&QuestLogEntry::Complete, "Complete", "complete", NULL)
		.method(&QuestLogEntry::UpdatePlayerFields, "UpdatePlayerFields", "updatePlayerFields", "updateplayerfields", NULL)
		.method(&QuestLogEntry::SetTrigger, "SetTrigger", "setTrigger", "settrigger", NULL)
		.method(&QuestLogEntry::SetMobCount, "SetMobCount", "setMobCount", "setmobcount", NULL)
		.method(&QuestLogEntry::IncrementMobCount, "IncrementMobCount", "incrementMobCount", "incrementmobcount", NULL)
		.method(&QuestLogEntry::IsUnitAffected, "IsUnitAffected", "isUnitAffected", "isunitaffected", NULL)
		.method(&QuestLogEntry::IsCastQuest, "IsCastQuest", "isCastQuest", "iscastquest", NULL)
		.method(&QuestLogEntry::IsEmoteQuest, "IsEmoteQuest", "isEmoteQuest", "isemotequest", NULL)
		.method(&QuestLogEntry::AddAffectedUnit, "AddAffectedUnit", "addAffectedUnit", "addaffectedunit", NULL)
		.method(&QuestLogEntry::ClearAffectedUnits, "ClearAffectedUnits", "clearAffectedUnits", "clearaffectedunits", NULL)
		.method(&QuestLogEntry::SetSlot, "SetSlot", "setSlot", "setslot", NULL)
		.method(&QuestLogEntry::Finish, "Finish", "finish", NULL)
		.method(&QuestLogEntry::SendQuestComplete, "SendQuestComplete", "sendQuestComplete", "sendquestcomplete", NULL)
		.method(&QuestLogEntry::SendUpdateAddKill, "SendUpdateAddKill", "sendUpdateAddKill", "sendupdateaddkill", NULL)
		.method(&QuestLogEntry::GetMobCount, "GetMobCount", "getMobCount", "getmobcount", NULL)
		.method(&QuestLogEntry::GetExploredAreas, "GetExploredAreas", "getExploredAreas", "getexploredareas", NULL)
		.method(&QuestLogEntry::GetBaseField, "GetBaseField", "getBaseField", "getbasefield", NULL)
		.method(&QuestLogEntry::GetSlot, "GetSlot", "getSlot", "getslot", NULL);
	}

}
