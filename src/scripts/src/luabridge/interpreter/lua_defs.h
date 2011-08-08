#pragma once
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


//An incomplete structure that is to be treated as an size_t. We this type to specialize how tdstack::get/push methods handle them to lua.
//a specialized type treated as a reference to a lua function.
typedef struct lua_function_o {}* lua_function;
//specialized type that is used to return a reference to a variable object type.
typedef struct lua_obj_o {}* lua_obj;
//specialized type returns an index to the stack where the table is located.
typedef struct lua_table_o {}* lua_table;
//specialized type that returns the lua execution stack of the function that called the function that contains this as its parameter
typedef struct lua_stack_o {}* lua_stack;

enum custom_value_types
{
    CUSTOM_TYPE_START = LUA_TTHREAD,
    CUSTOM_TYPE_REPEATS_ARG
};

class variadic_node
{
	public:
		ptrdiff_t type;
		union
		{
			lua_Number lua_number;
			const char* lua_str;
			lua_State* thread;
			//for ud objects and functions, we store a reference to that.
			ptrdiff_t obj_ref;
			void* l_ud;
			ptrdiff_t bewl;
		} val;
		//node to the next parameter
		variadic_node* next;
		variadic_node()
		{
			type = LUA_TNONE;
			next = NULL;
			val.lua_number = 0;
		}
};
class variadic_parameter
{
	public:
		//how many parameters exist.
		ptrdiff_t count;
		//node type, represents any obtainable object.
		variadic_node* head_node;
		variadic_parameter()
		{
			count = 0;
			head_node = NULL;
		}
};

class ReferenceHandler
{
		static const char* refTable;
	public:
		static ptrdiff_t addReference(lua_State* L, ptrdiff_t index)
		{
			ptrdiff_t key = LUA_REFNIL;
			if(!lua_isnoneornil(L, index))
			{
				lua_getfield(L, LUA_REGISTRYINDEX, refTable);
				if(lua_isnil(L, -1))
				{
					lua_pop(L, 1);
					lua_newtable(L);
					lua_pushvalue(L, -1);
					lua_setfield(L, LUA_REGISTRYINDEX, refTable); // registry[refTable] = {}
				}
				assert(lua_type(L, -1) == LUA_TTABLE);
				lua_pushvalue(L, index);
				key = luaL_ref(L, -2);
				lua_pop(L, 1); //pop refTable from stack.
			}
			return key;
		}
		static void removeReference(lua_State* L, ptrdiff_t key)
		{
			lua_getfield(L, LUA_REGISTRYINDEX, refTable);
			assert(lua_type(L, -1) == LUA_TTABLE);
			luaL_unref(L, -1, key);
			lua_pop(L, 1); // pop refTable
		}
		static void getReference(lua_State* L, ptrdiff_t key)
		{
			lua_getfield(L, LUA_REGISTRYINDEX, refTable);
			assert(lua_type(L, -1) == LUA_TTABLE);
			lua_rawgeti(L, -1, key); //push to the stack the value at refTable[key]
			lua_replace(L, -2); // do stack[refTable] = refTable[key]
		}
};

//easy clean up function for var params
static void cleanup_varparam(variadic_parameter* param, lua_State* L)
{
	if(param != NULL)
	{
		variadic_node* current_node = param->head_node;
		variadic_node* next_node = NULL;
		for(; current_node != NULL;)
		{
			next_node = current_node->next;
			//For ud,function objects, since we store them as references, we have to unref them when we are done with them.
			switch(current_node->type)
			{
				case LUA_TUSERDATA:
				case LUA_TTABLE:
				case LUA_TFUNCTION:
					ReferenceHandler::removeReference(L, current_node->val.obj_ref);
					break;
				case LUA_TSTRING:
					free((void*)current_node->val.lua_str);
					break;
				default:
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
extern ptrdiff_t extractfRefFromCString(lua_State* L, const char* str);

/*static void dumpStackInfo(lua_State * L)
{
	if(L != NULL)
	{
		ptrdiff_t top = lua_gettop(L);
		for(int i = 1; i <= top; ++i)
		{
			printf("\t%d. %s", i, luaL_typename(L, i) );
			switch(lua_type(L,i) )
			{
			case LUA_TNONE:
			case LUA_TNIL:
				break;
			case LUA_TNUMBER:
				printf("(%f)", lua_tonumber(L, i));
				break;
			case LUA_TSTRING:
				printf("(\"%s\")", lua_tostring(L,i) );
				break;
			case LUA_TLIGHTUSERDATA:
			case LUA_TTABLE:
			case LUA_TUSERDATA:
			case LUA_TFUNCTION:
				printf("(%p)", lua_topointer(L,i) );
				break;
			}
			printf("\n");
		}
	}
}*/

#define INVALID_FUNCTION LUA_REFNIL
typedef lua_State* lua_thread;

class LuaCreature;
class LuaGossip;
class LuaGameObject;
class LuaInstance;
class LuaQuest;

class SpellMapEntry
{
	public:
		int32 ref;
		variadic_parameter* params;
		SpellMapEntry() : ref(LUA_REFNIL), params(NULL) {}
};
typedef SpellMapEntry* PSpellMapEntry;

class ObjectBinding
{
	public:
		lua_function refs[CREATURE_EVENT_COUNT];
		ObjectBinding()
		{
			for(size_t i = 0; i < CREATURE_EVENT_COUNT; ++i)
				refs[i] = (lua_function)LUA_REFNIL;
		}
};
typedef ObjectBinding* PObjectBinding;

class LUA_SCRIPT
{
	public:
		const void* data_;
		size_t datasize_;
		uint32 readpos_;
		std::set<int32> maps_;
		LUA_SCRIPT()
		{
			data_ = NULL;
			datasize_ = 0;
			readpos_ = 0;
			maps_.clear();
		}
		~LUA_SCRIPT()
		{
			if(data_ != NULL)
				free((void*)data_);
		}
};
typedef LUA_SCRIPT* PLUA_SCRIPT;


//object wrapper class required for uint64 numbers to support comparison metamethods etc.
template<typename T>
struct ObjectWrap
{
	T value_;
	void operator=(const T & newvalue) { value_ = newvalue; }
	ObjectWrap(const T & val = T())  : value_(val) {}
	ObjectWrap(const ObjectWrap<T> & _other) : value_(_other.value_) {}
};

template<typename T>
bool operator==(const ObjectWrap<T> & left, const ObjectWrap<T> & right)
{
	return left.value_ == right.value_;
}
template<typename T>
bool operator<(const ObjectWrap<T> & left, const ObjectWrap<T> & right)
{
	return left.value_ < right.value_;
}
template<typename T>
bool operator>(const ObjectWrap<T> & left, const ObjectWrap<T> & right)
{
	return left.value_ > right.value_;
}
template<typename T>
bool operator!=(const ObjectWrap<T> & left, const ObjectWrap<T> & right)
{
	return operator==(left, right);
}