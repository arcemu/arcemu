#include "LUAEngine.h"


LuaGossip::LuaGossip(uint32 id)
{
	m_go_gossip_binding = NULL, m_item_gossip_binding = NULL, m_unit_gossip_binding = NULL, id_ = id;
}

LuaGossip::~LuaGossip()
{
	PLUA_INSTANCE ref = lua_instance.get();
	if(this->m_go_gossip_binding != NULL)
	{
		li::GossipInterfaceMap::iterator itr = ref->m_go_gossipInterfaceMap.find(id_);
		if(itr != ref->m_go_gossipInterfaceMap.end())
			ref->m_go_gossipInterfaceMap.erase(itr);
	}
	else if(this->m_unit_gossip_binding != NULL)
	{
		li::GossipInterfaceMap::iterator itr = ref->m_unit_gossipInterfaceMap.find(id_);
		if(itr != ref->m_unit_gossipInterfaceMap.end())
			ref->m_unit_gossipInterfaceMap.erase(itr);
	}
	else if(this->m_item_gossip_binding != NULL)
	{
		li::GossipInterfaceMap::iterator itr = ref->m_item_gossipInterfaceMap.find(id_);
		if(itr != ref->m_item_gossipInterfaceMap.end())
			ref->m_item_gossipInterfaceMap.erase(itr);
	}
}

void LuaGossip::OnHello(Object* pObject, Player* Plr)
{
	GET_LOCK;
	if(pObject->GetTypeId() == TYPEID_UNIT)
	{
		if(m_unit_gossip_binding == NULL) { RELEASE_LOCK; return; }

		lua_engine::BeginLuaFunctionCall(m_unit_gossip_binding->refs[GOSSIP_EVENT_ON_TALK]);
		push_creature(TO_CREATURE(pObject));
		push_int(GOSSIP_EVENT_ON_TALK);
		push_player(Plr);
		lua_engine::ExecuteLuaFunction(3);
	}
	else if(pObject->GetTypeId() == TYPEID_ITEM)
	{
		if(m_item_gossip_binding == NULL) { RELEASE_LOCK; return; }

		lua_engine::BeginLuaFunctionCall(m_item_gossip_binding->refs[GOSSIP_EVENT_ON_TALK]);
		push_item(TO_ITEM(pObject));
		push_int(GOSSIP_EVENT_ON_TALK);
		push_player(Plr);
		lua_engine::ExecuteLuaFunction(3);
	}
	else if(pObject->GetTypeId() == TYPEID_GAMEOBJECT)
	{
		if(m_go_gossip_binding == NULL) { RELEASE_LOCK; return; }

		lua_engine::BeginLuaFunctionCall(m_go_gossip_binding->refs[GOSSIP_EVENT_ON_TALK]);
		push_go(TO_GAMEOBJECT(pObject));
		push_int(GOSSIP_EVENT_ON_TALK);
		push_player(Plr);
		lua_engine::ExecuteLuaFunction(3);
	}
	RELEASE_LOCK
}

void LuaGossip::OnSelectOption(Object* pObject, Player* Plr, uint32 Id, const char* EnteredCode)
{
	GET_LOCK;
	if(pObject->GetTypeId() == TYPEID_UNIT)
	{
		if(m_unit_gossip_binding == NULL) { RELEASE_LOCK; return; }

		lua_engine::BeginLuaFunctionCall(m_unit_gossip_binding->refs[GOSSIP_EVENT_ON_SELECT_OPTION]);
		push_creature(TO_CREATURE(pObject));
		push_int(GOSSIP_EVENT_ON_SELECT_OPTION);
		push_player(Plr);
		push_int(Id);
		push_str(EnteredCode);
		lua_engine::ExecuteLuaFunction(5);
	}
	else if(pObject->GetTypeId() == TYPEID_ITEM)
	{
		if(m_item_gossip_binding == NULL) { RELEASE_LOCK; return; }
		lua_engine::BeginLuaFunctionCall(m_item_gossip_binding->refs[GOSSIP_EVENT_ON_SELECT_OPTION]);
		push_item(TO_ITEM(pObject));
		push_int(GOSSIP_EVENT_ON_SELECT_OPTION);
		push_player(Plr);
		push_int(Id);
		push_str(EnteredCode);
		lua_engine::ExecuteLuaFunction(5);
	}
	else if(pObject->GetTypeId() == TYPEID_GAMEOBJECT)
	{
		if(m_go_gossip_binding == NULL) { RELEASE_LOCK; return; }
		lua_engine::BeginLuaFunctionCall(m_go_gossip_binding->refs[GOSSIP_EVENT_ON_SELECT_OPTION]);
		push_go(TO_GAMEOBJECT(pObject));
		push_int(GOSSIP_EVENT_ON_SELECT_OPTION);
		push_player(Plr);
		push_int(Id);
		push_str(EnteredCode);
		lua_engine::ExecuteLuaFunction(5);
	}
	RELEASE_LOCK
}

void LuaGossip::Destroy()
{
	delete this;
}

namespace lua_engine
{
	Arcemu::Gossip::Script* createunitgossipInterface(uint32 id)
	{
		LuaGossip* pLua = NULL;
		PLUA_INSTANCE ref = lua_instance.get();
		//First check if we have registered a binding.
		li::ObjectBindingMap::iterator itr = ref->m_unitGossipBinding.find(id);
		PObjectBinding pBinding = (itr != ref->m_unitGossipBinding.end()) ? itr->second : NULL;
		//Don't bother creating an interface if we don't have a binding.
		if(pBinding != NULL)
		{
			//Check if we had already created an interface, if so, we just update its binding to point to the newly registered one.
			li::GossipInterfaceMap::iterator itr = ref->m_unit_gossipInterfaceMap.find(id);
			if(itr != ref->m_unit_gossipInterfaceMap.end())
				pLua = itr->second;
			else
			{
				pLua = new LuaGossip(id);
				//All units w/ the same id will share this interface.
				ref->m_unit_gossipInterfaceMap.insert(make_pair(id, pLua));
			}
			pLua->m_unit_gossip_binding = pBinding;
		}
		return pLua;
	}
	Arcemu::Gossip::Script* createitemgossipInterface(uint32 id)
	{
		LuaGossip* pLua = NULL;
		PLUA_INSTANCE ref = lua_instance.get();
		li::ObjectBindingMap::iterator itr = ref->m_itemGossipBinding.find(id);
		PObjectBinding pBinding = (itr != ref->m_itemGossipBinding.end()) ? itr->second : NULL;
		if(pBinding != NULL)
		{
			li::GossipInterfaceMap::iterator itr = ref->m_item_gossipInterfaceMap.find(id);

			if(itr != ref->m_item_gossipInterfaceMap.end())
				pLua = itr->second;
			else
			{
				pLua = new LuaGossip(id);
				ref->m_item_gossipInterfaceMap.insert(make_pair(id, pLua));
			}
			pLua->m_item_gossip_binding = pBinding;
		}
		return pLua;
	}
	Arcemu::Gossip::Script* creategogossipInterface(uint32 id)
	{
		LuaGossip* pLua = NULL;
		PLUA_INSTANCE ref = lua_instance.get();
		li::ObjectBindingMap::iterator itr = ref->m_goGossipBinding.find(id);
		PObjectBinding pBinding = (itr != ref->m_goGossipBinding.end()) ? itr->second : NULL;
		if(pBinding != NULL)
		{
			li::GossipInterfaceMap::iterator itr = ref->m_go_gossipInterfaceMap.find(id);
			if(itr != ref->m_go_gossipInterfaceMap.end())
				pLua = itr->second;
			else
			{
				pLua = new LuaGossip(id);
				ref->m_go_gossipInterfaceMap.insert(make_pair(id, pLua));
			}
			pLua->m_go_gossip_binding = pBinding;
		}
		return pLua;
	}
}

int CreateLuaEvent(lua_function fref, int delay, int repeats, variadic_parameter* params)
{
	int ref = LUA_REFNIL;
	PLUA_INSTANCE li_ = lua_instance.get();
	if(li_->map != NULL && delay > 0 && (ptrdiff_t)fref != LUA_REFNIL)
	{
		//embed the function ref and repeats as part of our parameters.
		params = (params == NULL) ? new variadic_parameter : params;
		variadic_node* func_node = new variadic_node;
		variadic_node* repeats_node = new variadic_node;
		repeats_node->type = CUSTOM_TYPE_REPEATS_ARG;
		repeats_node->val.bewl = repeats;
		func_node->type = LUA_TFUNCTION;
		func_node->val.obj_ref = (ptrdiff_t)fref;
		//make the function the new head node.
		func_node->next = repeats_node;
		repeats_node->next = params->head_node;
		params->head_node = func_node;
		//update args count
		params->count += 2;
		TimedEvent* ev = TimedEvent::Allocate(li_->map, new CallBackFunctionP1<variadic_parameter*>(&lua_engine::ExecuteLuaFunction, params), 0, delay, repeats);
		ev->eventType  = LUA_EVENTS_END + (ptrdiff_t)fref; //Create custom reference by adding the ref number to the max lua event type to get a unique reference for every function.
		li_->map->event_AddEvent(ev);
		li_->m_globalFRefs.insert(params);
	}
	return ref;
}
void lua_engine::ExecuteLuaFunction(variadic_parameter* params)
{
	if(params != NULL)
	{
		PLUA_INSTANCE li_ = lua_instance.get();
		if(li_->m_globalFRefs.find(params) != li_->m_globalFRefs.end())
		{
			lua_State* lu = li_->lu;
			//place the function on the stack.
			ReferenceHandler::getReference(lu, params->head_node->val.obj_ref);
			int arg_cnt = params->count - 2;

			//retrieve the repeats.
			variadic_node* function_node = params->head_node;
			ptrdiff_t repeats = params->head_node->next->val.bewl;
			/*	Prepare to push arguments, 1st assign the head node to the actual arguments registered to this function */
			params->head_node = function_node->next->next;
			//subtract the function n repeat node from arg count.
			params->count -= 2;
			//Now we push all args.
			luabridge::tdstack<variadic_parameter*>::push(lu, params);
			params->head_node = function_node;
			params->count += 2;
			//call the function
			if(lua_pcall(lu, arg_cnt, 0, 0))
				report(lu);
			//if it's not an infinite/one time call event.
			if(repeats > 1)
				//decrement repeats and put it back in the params.
				function_node->next->val.bewl = (int)--repeats;
			else if(repeats == 1)
			{
				//remove this function from storage.
				li_->m_globalFRefs.erase(params);
				//clean up the rest of the args
				cleanup_varparam(params, lu);
			}
		}
	}
}
void ModifyLuaEventInterval(int32 ref, uint32 newInterval)
{
	PLUA_INSTANCE li_ = lua_instance.get();
	if(li_ != NULL && ref > LUA_REFNIL)
		//Easy interval modification.
		sEventMgr.ModifyEventTime(li_->map, (size_t)ref + LUA_EVENTS_END, newInterval);
}
static void DestroyLuaEvent(int32 ref)
{
	PLUA_INSTANCE li_ = lua_instance.get();
	if(li_ != NULL && ref > LUA_REFNIL)
	{
		//Simply remove the reference, CallFunctionByReference will find the reference has been freed and skip any processing.
		ReferenceHandler::removeReference(li_->lu, (ptrdiff_t)ref);
		for(li::References::iterator itr = li_->m_globalFRefs.begin(); itr != li_->m_globalFRefs.end(); ++itr)
		{
			if((*itr) != NULL && (*itr)->head_node->type == LUA_TFUNCTION && (*itr)->head_node->val.obj_ref == (ptrdiff_t)ref)
			{
				li_->m_globalFRefs.erase(itr);
				break;
			}
		}
		sEventMgr.RemoveEvents(li_->map , (size_t)ref + LUA_EVENTS_END);
	}
}

//Used to clean up any pending events when restarting.
void DestroyAllLuaEvents(PLUA_INSTANCE instance)
{
	//Clean up for all events.
	li::References::iterator itr = instance->m_globalFRefs.begin();
	ptrdiff_t ref = LUA_REFNIL;
	GET_LOCK;
	for(; itr != instance->m_globalFRefs.end(); ++itr)
	{
		if((*itr) != NULL && (*itr)->head_node != NULL && (*itr)->head_node->type == LUA_TFUNCTION)
		{
			ref = (*itr)->head_node->val.obj_ref;
			sEventMgr.RemoveEvents(World::getSingletonPtr(), ref + LUA_EVENTS_END);
			cleanup_varparam((*itr), instance->lu);
		}
	}
	instance->m_globalFRefs.clear();
}
static void GetRegistryTable(const char* name, lua_stack stack)
{
	if(name == NULL)
		lua_pushvalue((lua_thread)stack, LUA_REGISTRYINDEX);
	else
	{
		lua_getfield((lua_thread)stack, LUA_REGISTRYINDEX, name);
		if(lua_type((lua_thread)stack, -1) != LUA_TTABLE)
		{
			lua_pop((lua_thread)stack, 1);
			lua_pushnil((lua_thread) stack);
		}
	}
}

//Lua object casting methods
static void lua_toplayer(Object* obj, lua_stack s)
{
	if(obj != NULL && obj->IsPlayer())
		luabridge::tdstack<Player*>::reassignto((lua_thread)s, 1);
	else
		luabridge::tdstack<void>::push((lua_thread)s);
}
static void lua_togo(Object* obj, lua_stack s)
{
	if(obj != NULL && obj->IsGameObject())
		luabridge::tdstack<GameObject*>::reassignto((lua_thread)s, 1);
	else
		luabridge::tdstack<void>::push((lua_thread)s);
}
static void lua_toitem(Object* obj, lua_stack s)
{
	if(obj != NULL && obj->IsItem())
		luabridge::tdstack<Item*>::reassignto((lua_thread)s, 1);
	else
		luabridge::tdstack<void>::push((lua_thread)s);
}
static void lua_tocontainer(Object* obj, lua_stack s)
{
	if(obj != NULL && obj->IsContainer())
		luabridge::tdstack<Container*>::reassignto((lua_thread)s, 1);
	else
		luabridge::tdstack<void>::push((lua_thread)s);
}
static void lua_tounit(Object* obj, lua_stack s)
{
	if(obj != NULL && obj->IsUnit())
		luabridge::tdstack<Unit*>::reassignto((lua_thread)s, 1);
	else
		luabridge::tdstack<void>::push((lua_thread)s);
}
static void lua_tocreature(Object* obj, lua_stack s)
{
	if(obj != NULL && obj->IsCreature())
		luabridge::tdstack<Creature*>::reassignto((lua_thread)s, 1);
	else
		luabridge::tdstack<void>::push((lua_thread)s);
}

static bool include(const char* filename, bool once)
{
	PLUA_INSTANCE ref = lua_instance.get();
	if(!ref->scripts_.isLoaded(filename))
	{
		if(lua_engine::loadScript(filename))
			ref->scripts_.add(filename);
		else
			return false;
	}
	else if(once)
		luaL_error(ref->lu, "%s has already been loaded!", filename);
	return true;
}

#include "GlobalFunctions.h"

namespace lua_engine
{
	void bindGlobalMethods(luabridge::module & m)
	{
		m	.function(&CreateLuaEvent, "CreateLuaEvent", "createEvent", "createevent", "createvent", NULL) //wiki listed
		.function(&ModifyLuaEventInterval, "ModifyLuaEventInterval", "ModifyLuaEventTimer", "modifyeventtime", "modifyEventTime", NULL)  //wiki listed
		.function(&DestroyLuaEvent, "DestroyLuaEvent", "destroyevent", "destroyEvent", NULL)  //wiki listed
		//Used to retrieve object method tables.
		.function(&GetRegistryTable, "getregistry", "GetRegistry", "getRegistry", NULL)
		//Casting global functions
		.function(&lua_toplayer, "TO_PLAYER", "to_player", "toplayer", "TOPLAYER", NULL)  //wiki listed
		.function(&lua_tocreature, "TO_CREATURE", "TOCREATURE", "to_creature", "tocreature", NULL)  //wiki listed
		.function(&lua_toitem, "TO_ITEM", "TOITEM", "toitem", "to_item", NULL)
		.function(&lua_tocontainer, "TO_CONTAINER", "TOCONTAINER", "tocontainer", "to_container", NULL)
		.function(&lua_tounit, "TO_UNIT", "TOUNIT", "to_unit", "tounit", NULL)
		.function(&lua_togo, "TO_GAMEOBJECT", "TOGAMEOBJECT", "to_gameobject", "togameobject", NULL)
		.function(&GetPlayer, "GetPlayer", "getPlayer", "getplayer", NULL)
		.function(&enginename, "GetEngineName", "getEngineName", "getenginename")
		.function(&engineversion, "GetEngineVersion", "getEngineVersion", "getengineversion", "revision", NULL)
		.function(&logcolor, "logcolor", "setConsoleColor", "SetConsoleColor", "setconsolecolor", NULL)
		.function(&SendWorldMessage, "SendWorldMessage", "sendWorldMessage", "sendworldmessage", NULL)
		.function(&ReloadTable, "reloadTable", "ReloadTable", "reloadtable", NULL)
		.function(&ReloadLuaEngine, "reloadLuaEngine", "ReloadLuaEngine", "reloadEngine", "reloadengine", NULL)
		.function(&GetPlayersInWorld, "getPlayersInWorld", "GetPlayersInWorld",  "getplayersinworld", NULL)
		.function(&Rehash, "Rehash", "rehash", NULL)
		.function(&GetArcemuRevision, "GetArcemuRevision", "getrevision", NULL)
		.function(&GetPlayersInMap, "GetPlayersInMap", "getPlayersInMap", "getplayersinmap", NULL)
		.function(&GetPlayersInZone, "GetPlayersInZone", "getplayersinzone", "getPlayersInZone", NULL)
		.function(&luabit_and, "bit_and", "bitand", "bAnd", NULL)
		.function(&luabit_or, "bit_or", "bitor", "bOr", NULL)
		.function(&luabit_xor, "bit_xor", "bitxor", "bXor", "xor", NULL)
		.function(&luabit_not, "bit_not", "bitnot", "bNot", NULL)
		.function(&bit_shiftleft, "bit_shiftleft", "shl", NULL)
		.function(&bit_shiftright, "bit_shiftright", "shr", NULL)
		.function(&GetPlatform, "GetPlatform", "getplatform", "getPlatform", NULL)
		.function(&SendPacketToZone, "SendPacketToZone", "sendpackettozone", "sendpacketozone", NULL)
		.function(&SendPacketToInstance, "sendPacketToInstance", "SendPacketToInstance", NULL)
		.function(&SendPacketToWorld, "SendPacketToWorld", "sendPacketToWorld", "sendpacketoworld", NULL)
		.function(&SendPacketToChannel, "sendpacketochannel", "sendpackettochannel", "SendPacketToChannel", "sendPacketToChannel", NULL)
		.function(&GetInstanceCreature, "GetInstanceCreature", "getInstanceCreature", "getinstancecreature", NULL)
		.function(&GetInstancePlayerCount, "GetInstancePlayerCount", "getInstancePlayerCount", "getinstanceplayercount", NULL)
		.function(&GetPlayersInInstance, "GetPlayersInInstance", "getPlayersInInstance", "getplayersininstance", NULL)
		.function(&include, "include", "Include", "INCLUDE", NULL)
		.function(&WorldDBQuery, "WorldDBQuery", "worlddbquery", NULL)
		.function(&CharDBQuery, "CharDBQuery", "chardbquery", NULL)
		.function(&NumberToGUID, "NumberToGUID", "TO_UINT64", "TO_GUID", NULL)
		.function(&isFriendly, "isFriendly", "IsFriendly", NULL)
		.function(&isHostile, "isHostile", "IsHostile", NULL)
		.function(&isNeutral, "isNeutral", "IsNeutral", NULL)
		.function(&isAttackable, "isAttackable", "IsAttackable", NULL);


		m	.class_<DBCStorage<SpellEntry> >("dbcSpell")
		.method(&DBCStorage<SpellEntry>::LookupEntry, "LookupEntry", "lookupentry", NULL)
		.method(&DBCStorage<SpellEntry>::GetNumRows, "GetNumRows", "getNumRows", "getnumrows", NULL)
		.method(&DBCStorage<SpellEntry>::LookupEntryForced, "LookupEntryForced", "lookupentryforced", NULL);
		//now we overwrite our dbcSpell static table with dbcSpell storage object.
		luabridge::tdstack<DBCStorage<SpellEntry>*>::push(m.L, &dbcSpell);
		lua_setglobal(m.L, "dbcSpell");

		//Expose the world database and character database.
		m	.class_<Database>("WoW_Database")
		.method("Query", &Database::QueryNA)
		.method("Execute", &Database::ExecuteNA);

		luabridge::tdstack<Database*>::push(m.L, Database_World);
		lua_setglobal(m.L, "WorldDB");
		luabridge::tdstack<Database*>::push(m.L, Database_Character);
		lua_setglobal(m.L, "CharacterDB");

		typedef void (MailSystem::*SendAutomatedMessage)(uint32,uint64,uint64,string,string,uint32,uint32,uint64,uint32,MailCheckMask,uint32);
		SendAutomatedMessage _MailSystemSendAutomatedMessage = &MailSystem::SendAutomatedMessage;

		m	.class_<MailSystem>("MailSystem")
		.method("SendAutomatedMessage", _MailSystemSendAutomatedMessage);
		luabridge::tdstack<MailSystem*>::push(m.L, MailSystem::getSingletonPtr());
		lua_setglobal(m.L, "sMailSystem");

		m	.class_<ObjectMgr>("ObjectMgr")
		.method(&ObjectMgr::GetPlayerInfo, "GetPlayerInfo", "getPlayerInfo", "getplayerinfo", NULL)
		.method(&ObjectMgr::GetPlayerInfoByName, "GetPlayerInfoByName", "getPlayerInfoByName", "getplayerinfobyname", NULL)
		.method(&ObjectMgr::GetPlayerCreateInfo, "GetPlayerCreateInfo", "getPlayerCreateInfo", "getplayercreateinfo", NULL)
		.method(&ObjectMgr::GetGroupByLeader, "GetGroupByLeader", "getGroupByLeader", "getgroupbyleader", NULL)
		.method(&ObjectMgr::GetGroupById, "GetGroupById", "GetGroupByID" "getGroupById", "getGroupByID", "getgroupbyid", NULL)
		.method(&ObjectMgr::GetTotalGuildCount, "GetTotalGuildCount", "getTotalGuildCount", "gettotalguildcount", NULL)
		.method(&ObjectMgr::GetGuild, "GetGuild", "getGuild", "getguild", NULL)
		.method(&ObjectMgr::GetGuildByLeaderGuid, "GetGuildByLeaderGuid", "getGuildByLeaderGuid", "GetGuildByLeaderGUID", "getGuildByLeaderGUID", "getguildbyleaderguid", NULL)
		.method(&ObjectMgr::GetGuildByGuildName, "GetGuildByGuildName", "getGuildByGuildName", "getguildbyguildname", NULL);

		luabridge::tdstack<ObjectMgr*>::push(m.L, ObjectMgr::getSingletonPtr());
		lua_setglobal(m.L, "objmgr");
		luabridge::tdstack<ObjectMgr*>::push(m.L, ObjectMgr::getSingletonPtr());
		lua_setglobal(m.L, "sObjectMgr");


		/*m	.class_<World>("World") //probably not the best idea to expose this
			.method( &World::getHordePlayerCount, "GetHordePlayerCount", "getHordePlayerCount", "gethordeplayercount", NULL)
			.method( &World::getAlliancePlayerCount, "GetAlliancePlayerCount", "getAlliancePlayerCount", "getallianceplayercount", NULL)
			.method( &World::getPlayerCount, "GetPlayerCount", "getPlayerCount", "getplayercount", NULL)
			.method( &World::Rehash, "Rehash", "rehash", NULL)
			.method( &World::FindSession, "FindSession", "findSession", "findsession", NULL)
			.method( &World::FindSessionByName, "FindSessionByName", "findSessionByName", "findsessionbyname", NULL)
			.method( &World::GetPlayerLimit, "GetPlayerLimit", "getPlayerLimit", "getplayerlimit", NULL)
			.method( &World::SetPlayerLimit, "SetPlayerLimit", "setPlayerLimit", "setplayerlimit", NULL)
			.method( &World::SetMotd, "SetMotd", "setMotd", "setmotd", NULL)
			.method( &World::GetMotd, "GetMotd", "getMotd", "getmotd", NULL)
			.method( &World::GetGameTime, "GetGameTime", "getGameTime", "getgametime", NULL)
			.method( &World::SendWorldText, "SendWorldText", "sendWorldText", "sendworldtext", NULL)
			.method( &World::SendWorldWideScreenText, "SendWorldWideScreenText", "sendWorldWideScreenText", "sendworldwidescreentext", NULL)
			.method( &World::SendGlobalMessage, "SendGlobalMessage", "sendGlobalMessage", "sendglobalmessage", NULL)
			.method( &World::SendZoneMessage, "SendZoneMessage", "sendZoneMessage", "sendzonemessage", NULL)
			.method( &World::SendInstanceMessage, "SendInstanceMessage", "sendInstanceMessage", "sendinstancemessage", NULL)
			.method( &World::SendFactionMessage, "SendFactionMessage", "sendFactionMessage", "sendfactionmessage", NULL)
			.method( &World::SendGamemasterMessage, "SendGamemasterMessage", "sendGamemasterMessage", "sendgamemastermessage", NULL)
			.method( &World::SendGMWorldText, "SendGMWorldText", "sendGMWorldText", "sendgmworldtext", NULL)
			.method( &World::SendDamageLimitTextToGM, "SendDamageLimitTextToGM", "sendDamageLimitTextToGM", "senddamagelimittexttogm", NULL)
			.method( &World::SendBCMessageByID, "SendBCMessageByID", "sendBCMessageByID", "sendbcmessagebyid", NULL)
			.method( &World::SendLocalizedWorldText, "SendLocalizedWorldText", "sendLocalizedWorldText", "sendlocalizedworldtext", NULL)
			.method( &World::SendZoneUnderAttackMsg, "SendZoneUnderAttackMsg", "sendZoneUnderAttackMsg", "sendzoneunderattackmsg", NULL)
			.method( &World::GetUptime, "GetUptime", "getUptime", "getuptime", NULL)
			.method( &World::GetStartTime, "GetStartTime", "getStartTime", "getstarttime", NULL)
			.method( &World::setRate, "SetRate", "setRate", "setrate", NULL)
			.method( &World::getRate, "GetRate", "getRate", "getrate", NULL)
			.method( &World::getIntRate, "GetIntRate", "getIntRate", "getintrate", NULL)
			.method( &World::setIntRate, "SetIntRate", "setIntRate", "setintrate", NULL)
			.method( &World::GenerateName, "GenerateName", "generateName", "generatename", NULL)
			.method( &World::SaveAllPlayers, "SaveAllPlayers", "saveAllPlayers", "saveallplayers", NULL)
			.method( &World::SetKickAFKPlayerTime, "SetKickAFKPlayerTime", "setKickAFKPlayerTime", "setkickafkplayertime", NULL)
			.method( &World::GetKickAFKPlayerTime, "GetKickAFKPlayerTime", "getKickAFKPlayerTime", "getkickafkplayertime", NULL)
			.method( &World::GetRealmType, "GetRealmType", "getRealmType", "getrealmtype", NULL);
			.property_ro( "MapPath", &World::MapPath )
			.property_ro( "vMapPath", &World::vMapPath )
			.property_rw( "BreathingEnabled", &World::BreathingEnabled )
			.property_rw( "SpeedhackProtection", &World::SpeedhackProtection )
			.property_rw( "StartingLevel", &World::StartingLevel )
			.property_rw( "ExtraTalents", &World::ExtraTalents )
			.property_rw( "MaxProfs", &World::MaxProfs )
			.property_rw( "DKStartTalentPoints", &World::DKStartTalentPoints )
			.property_rw( "Collision", &World::Collision )
			.property_rw( "DisableFearMovement", &World::DisableFearMovement )
			.property_rw( "flood_lines", &World::flood_lines )
			.property_rw( "flood_seconds", &World::flood_seconds )
			.property_rw( "flood_message", &World::flood_message )
			.property_rw( "show_gm_in_who_list", &World::show_gm_in_who_list )
			.property_rw( "interfaction_chat", &World::interfaction_chat )
			.property_rw( "interfaction_group", &World::interfaction_group )
			.property_rw( "interfaction_guild", &World::interfaction_guild )
			.property_rw( "interfaction_trade", &World::interfaction_trade )
			.property_rw( "interfaction_friend", &World::interfaction_friend )
			.property_rw( "interfaction_misc", &World::interfaction_misc )
			.property_rw( "crossover_chars", &World::crossover_chars )
			.property_rw( "antiMasterLootNinja", &World::antiMasterLootNinja )
			.property_rw( "gamemaster_listOnlyActiveGMs", &World::gamemaster_listOnlyActiveGMs )
			.property_rw( "gamemaster_hidePermissions", &World::gamemaster_hidePermissions )
			.property_rw( "gamemaster_startonGMIsland", &World::gamemaster_startonGMIsland )
			.property_rw( "Arena_Season", &World::Arena_Season )
			.property_rw( "Arena_Progress", &World::Arena_Progress )
			.property_rw( "announce_tag", &World::announce_tag )
			.property_rw( "GMAdminTag", &World::GMAdminTag )
			.property_rw( "NameinAnnounce", &World::NameinAnnounce )
			.property_rw( "NameinWAnnounce", &World::NameinWAnnounce )
			.property_rw( "announce_output", &World::announce_output )
			.property_rw( "announce_tagcolor", &World::announce_tagcolor )
			.property_rw( "announce_gmtagcolor", &World::announce_gmtagcolor )
			.property_rw( "announce_namecolor", &World::announce_namecolor )
			.property_rw( "announce_msgcolor", &World::announce_msgcolor )
			.property_rw( "ann_namecolor", &World::ann_namecolor )
			.property_rw( "ann_gmtagcolor", &World::ann_gmtagcolor )
			.property_rw( "ann_tagcolor", &World::ann_tagcolor )
			.property_rw( "ann_msgcolor", &World::ann_msgcolor )
			.property_rw( "antihack_teleport", &World::antihack_teleport )
			.property_rw( "antihack_speed", &World::antihack_speed )
			.property_rw( "antihack_flight", &World::antihack_flight );

		luabridge::tdstack<World*>::push(m.L, World::getSingletonPtr());
		lua_setglobal(m.L, "sWorld"); */

	}
}
