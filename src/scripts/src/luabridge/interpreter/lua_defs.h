#pragma once
#include "StdAfx.h"
extern "C" 
{		// we're C++, and LUA is C, so the compiler needs to know to use C function names.
#include "../lua/lua.h"
#include "../lua/lauxlib.h"
#include "../lua/lualib.h"
};

enum QuestEvents
{
	QUEST_EVENT_ON_ACCEPT = 1,
	QUEST_EVENT_ON_COMPLETE = 2,
	QUEST_EVENT_ON_CANCEL = 3,
	QUEST_EVENT_GAMEOBJECT_ACTIVATE = 4,
	QUEST_EVENT_ON_CREATURE_KILL  = 5,
	QUEST_EVENT_ON_EXPLORE_AREA = 6,
	QUEST_EVENT_ON_PLAYER_ITEMPICKUP = 7,
	QUEST_EVENT_COUNT,
};

/** Creature Events
 */
enum CreatureEvents
{
	CREATURE_EVENT_ON_ENTER_COMBAT		= 1,
	CREATURE_EVENT_ON_LEAVE_COMBAT		= 2,
	CREATURE_EVENT_ON_TARGET_DIED		= 3,
	CREATURE_EVENT_ON_DIED		= 4,
	CREATURE_EVENT_ON_TARGET_PARRIED		= 5,
	CREATURE_EVENT_ON_TARGET_DODGED		= 6,
	CREATURE_EVENT_ON_TARGET_BLOCKED		= 7,
	CREATURE_EVENT_ON_TARGET_CRIT_HIT		= 8,
	CREATURE_EVENT_ON_PARRY		= 9,
	CREATURE_EVENT_ON_DODGED		= 10,
	CREATURE_EVENT_ON_BLOCKED		= 11,
	CREATURE_EVENT_ON_CRIT_HIT		= 12,
	CREATURE_EVENT_ON_HIT		= 13,
	CREATURE_EVENT_ON_ASSIST_TARGET_DIED		= 14,
	CREATURE_EVENT_ON_FEAR		= 15,
	CREATURE_EVENT_ON_FLEE		= 16,
	CREATURE_EVENT_ON_CALL_FOR_HELP		= 17,
	CREATURE_EVENT_ON_LOAD		= 18,
	CREATURE_EVENT_ON_REACH_WP		= 19,
	CREATURE_EVENT_ON_LOOT_TAKEN		= 20,
	CREATURE_EVENT_ON_AIUPDATE		= 21,
	CREATURE_EVENT_ON_EMOTE		= 22,
	CREATURE_EVENT_ON_DAMAGE_TAKEN = 23,
	CREATURE_EVENT_COUNT,
};

/** GameObject Events
 */
enum GameObjectEvents
{
	GAMEOBJECT_EVENT_ON_CREATE			= 1,
	GAMEOBJECT_EVENT_ON_SPAWN			= 2,
	GAMEOBJECT_EVENT_ON_LOOT_TAKEN		= 3,
	GAMEOBJECT_EVENT_ON_USE				= 4,
	GAMEOBJECT_EVENT_AIUPDATE			= 5,
	GAMEOBJECT_EVENT_ON_DESPAWN			= 6,
	GAMEOBJECT_EVENT_COUNT,
};

enum GossipEvents
{
	GOSSIP_EVENT_ON_TALK			= 1,
	GOSSIP_EVENT_ON_SELECT_OPTION	= 2,
	GOSSIP_EVENT_ON_END             = 3,
	GOSSIP_EVENT_COUNT,
};

enum RandomFlags
{
	RANDOM_ANY           = 0,
	RANDOM_IN_SHORTRANGE = 1,
	RANDOM_IN_MIDRANGE   = 2,
	RANDOM_IN_LONGRANGE  = 3,
	RANDOM_WITH_MANA     = 4,
	RANDOM_WITH_RAGE     = 5,
	RANDOM_WITH_ENERGY   = 6,
	RANDOM_NOT_MAINTANK  = 7,
	RANDOM_COUNT,
};

enum ServerHooks
{
	SERVER_HOOK_NEW_CHARACTER		= 1,
	SERVER_HOOK_KILL_PLAYER		= 2,
	SERVER_HOOK_FIRST_ENTER_WORLD	= 3,
	SERVER_HOOK_ENTER_WORLD		= 4,
	SERVER_HOOK_GUILD_JOIN			= 5,
	SERVER_HOOK_DEATH				= 6,
	SERVER_HOOK_REPOP				= 7,
	SERVER_HOOK_EMOTE				= 8,
	SERVER_HOOK_ENTER_COMBAT		= 9,
	SERVER_HOOK_CAST_SPELL			= 10,
	SERVER_HOOK_TICK				= 11,
	SERVER_HOOK_LOGOUT_REQUEST		= 12,
	SERVER_HOOK_LOGOUT				= 13,
	SERVER_HOOK_QUEST_ACCEPT		= 14,
	SERVER_HOOK_ZONE				= 15,
	SERVER_HOOK_CHAT				= 16,
	SERVER_HOOK_LOOT				= 17,
	SERVER_HOOK_GUILD_CREATE		= 18,
	SERVER_HOOK_ENTER_WORLD_2		= 19,
	SERVER_HOOK_CHARACTER_CREATE	= 20,
	SERVER_HOOK_QUEST_CANCELLED	= 21,
	SERVER_HOOK_QUEST_FINISHED		= 22,
	SERVER_HOOK_HONORABLE_KILL		= 23,
	SERVER_HOOK_ARENA_FINISH		= 24,
	SERVER_HOOK_OBJECTLOOT			= 25,
	SERVER_HOOK_AREATRIGGER		= 26,
	SERVER_HOOK_POST_LEVELUP       = 27,
	SERVER_HOOK_PRE_DIE	        = 28,	//general unit die, not only based on players
	SERVER_HOOK_ADVANCE_SKILLLINE  = 29,
	SERVER_HOOK_DUEL_FINISHED = 30,
	SERVER_HOOK_AURA_REMOVE = 31,
	SERVER_HOOK_RESURRECT = 32,

	SERVER_HOOK_COUNT,
};

enum InstanceHooks
{
	INSTANCE_EVENT_ON_PLAYER_DEATH = 1,
	INSTANCE_EVENT_ON_PLAYER_ENTER = 2,
	INSTANCE_EVENT_ON_AREA_TRIGGER = 3,
	INSTANCE_EVENT_ON_ZONE_CHANGE = 4,
	INSTANCE_EVENT_ON_CREATURE_DEATH = 5,
	INSTANCE_EVENT_ON_CREATURE_PUSH = 6,
	INSTANCE_EVENT_ON_GO_ACTIVATE = 7,
	INSTANCE_EVENT_ON_GO_PUSH = 8,
	INSTANCE_EVENT_ONLOAD = 9,
	INSTANCE_EVENT_DESTROY = 10,
	INSTANCE_EVENT_COUNT,
};

//Nice thing about this is that we can ignore any new core events(as long as they are added in order), it will automatically update.
enum CustomLuaEvenTypes
{
	LUA_EVENT_START = NUM_EVENT_TYPES, // Used as a placeholder
	EVENT_LUA_TIMED,
	EVENT_LUA_CREATURE_EVENTS,
	EVENT_LUA_GAMEOBJ_EVENTS,
	LUA_EVENTS_END
};

//reg type defines
#define REGTYPE_UNIT (1 << 0)
#define REGTYPE_GO (1 << 1)
#define REGTYPE_QUEST (1 << 2)
#define REGTYPE_SERVHOOK (1 << 3)
#define REGTYPE_ITEM (1 << 4)
#define REGTYPE_GOSSIP (1 << 5)
#define REGTYPE_DUMMYSPELL (1 << 6)
#define REGTYPE_INSTANCE (1 << 7)
#define REGTYPE_UNIT_GOSSIP (REGTYPE_UNIT | REGTYPE_GOSSIP)
#define REGTYPE_GO_GOSSIP (REGTYPE_GO | REGTYPE_GOSSIP)
#define REGTYPE_ITEM_GOSSIP (REGTYPE_ITEM | REGTYPE_GOSSIP)


//An empty type structure that is to be treated as an integer. We this type to specialize how tdstack::get/push methods handle them to lua.
//a specialied type treated as a reference to a lua function.
typedef struct{} *lua_function;
//specialized type that is used to return a reference to a variable object type.
typedef struct {} * lua_obj;
//specialized type returns an index to the stack where the table is located.
typedef struct{} * lua_table;
//specialized type that returns the lua execution stack of the function that called the function that contains this as its parameter
typedef struct{} * lua_stack;

enum custom_value_types
{
	CUSTOM_TYPE_START = LUA_TTHREAD,
	CUSTOM_TYPE_REPEATS_ARG
};
struct variadic_node
{
	int type;
	union 
	{
		float lua_number;
		const char * lua_str;
		lua_State * thread;
		//for ud objects and functions, we store a reference to that.
		int obj_ref;
		void * l_ud;
		int bewl;
	} val;
	//node to the next parameter
	struct variadic_node * next;
};
struct variadic_parameter
{
	//how many parameters exist.
	int count;
	//node type, represents any obtainable object.
	variadic_node * head_node;
};

//easy clean up function for var params
static void cleanup_varparam(variadic_parameter * param, lua_State * L)
{
	if(param != NULL)
	{
		variadic_node * current_node = param->head_node;
		variadic_node * next_node = NULL;
		for(int i = 0; i < param->count && current_node != NULL; ++i)
		{
			next_node = current_node->next;
			//For ud,function objects, since we store them as references, we have to unref them when we are done with them.
			switch(current_node->type)
			{
			case LUA_TUSERDATA:
			case LUA_TFUNCTION:
				lua_unref(L, current_node->val.obj_ref);
				break;
			}
			//now we can delete the node
			delete current_node;
			current_node = next_node;
		}
		delete param;
	}
}

//	Used to extract a reference to a function pointed by str, returns LUA_REFNIL if it can't.
extern int extractfRefFromCString(lua_State * L, const char* str);
//	Stores previously allocated pointers to shared_ptrs so that we can reduce the amount of userdata's that we allocate.
typedef HM_NAMESPACE::hash_map<const char*, std::deque<void*> > SHAREDPTR_POOL;
extern SHAREDPTR_POOL sp_pool;

#define INVALID_FUNCTION LUA_REFNIL
typedef lua_State* lua_thread;

class LuaCreature;
class LuaGossip;
class LuaGameObject;
class LuaInstance;
class LuaQuest;

typedef struct
{
	int ref;
	variadic_parameter * params;
} SpellMapEntry, *PSpellMapEntry;
typedef struct
{
	lua_function refs[CREATURE_EVENT_COUNT];
} ObjectBinding, *PObjectBinding;

class LuaResult : public QueryResult
{
public:
	Field * GetColumn(int clm)
	{
		if( (uint32)clm <= GetFieldCount() )
			return &Fetch()[clm];
		return NULL;
	}
};