#include "../../interpreter/LUAEngine.h"
/*
 * stack.hpp - Copyright (C) 2007 by Nathan Reed
 * Type-dispatch functions for manipulating the Lua stack.
 */

/*
 * These totally generic functions are unimplemented, causing a compiler
 * error if they are called.  We don't know how to send objects of arbitrary
 * types to and from Lua.  Following are specializations of this structure
 * that allow specific types to be handled in specific ways.
 */
template <typename T>
struct tdstack
{
	static void push (lua_State *L, T data)
	{
		lua_pushinteger(L, static_cast<lua_Integer>(data) );
	}
	static T get (lua_State *L, int index)
	{
		return static_cast<T>(luaL_checkint(L, index) );
	}
};
template <typename T>
struct tdstack<T*>
{
	static void push (lua_State *L, T * data)
	{
		if(data != NULL)
		{
			//created a shared ptr from ud block.
			T ** ud_ptr = (T**)lua_newuserdata(L, sizeof(T**) );
			luaL_getmetatable(L, classname<T>::name());
			lua_setmetatable(L, -2);
			*ud_ptr = data;
		}
		else	//otherwise push a nil object
			lua_pushnil(L);
	}

	
	//************************************
	// Purpose : Takes the userdata the at given idx and reassigns its metatable and pushes it to the top of the stack.
	// Parameter:	lua_State * L
	// Parameter:	int idx
	// Return : void
	//************************************
	static void reassignto(lua_State*L, int idx)
	{
		if( lua_type(L, idx) == LUA_TUSERDATA)
		{
			lua_pushvalue(L, idx); //make sure it's at the top
			luaL_getmetatable(L, classname<T>::name() );
			lua_setmetatable(L, -2);
		}
	}

	static void push(lua_State *L, pair< typename std::set<T*>::iterator, typename std::set<T*>::iterator > itr)
	{
		lua_newtable(L);
		ptrdiff_t index = 1;
		for(; itr.first != itr.second; ++itr.first)
		{
			tdstack<T*>::push(L, (*itr.first) );
			lua_rawseti(L, -2, index++);
		}
	}
	static T * get (lua_State *L, int index)
	{
		return *(T**)checkclass(L, index, classname<T>::name());
	}
};

/*
 * Pointers and references: getting is done by retrieving the address from
 * the Lua-owned shared_ptr, but pushing is not allowed since luabridge
 * has no idea of the ownership semantics of these objects.  You can only
 * push shared_ptrs, not naked pointers and references.
 */

template <typename T>
struct tdstack <const T *>
{
	static void push (lua_State *L, const T *data)
	{
		if(data != NULL)
		{
			const T** ud_ptr = (const T**)lua_newuserdata(L, sizeof(const T**) );
			std::string constname = std::string("const ") + classname<T>::name();
			luaL_getmetatable(L, constname.c_str());
			lua_setmetatable(L, -2);
			*ud_ptr = data;
		}
		else
			lua_pushnil(L);
	}
	static const T* get (lua_State *L, int index)
	{
		std::string constname = std::string("const ") + classname<T>::name();
		return *(const T**)checkclass(L, index, constname.c_str());
	}
};

template <typename T>
struct tdstack <T* const>
{
	static void push (lua_State *L, T * const data)
	{
		if(data != NULL)
		{
			const T ** ud = lua_newuserdata(L, sizeof(const T**) );
			luaL_getmetatable(L, classname<T>::name() );
			lua_setmetatable(L,-2);
			*ud = data;
		}
		else
			lua_pushnil(L);
	}

	static T* const get (lua_State *L, int index)
	{
		return *(T**)checkclass(L, index, classname<T>::name() );
	}
};

template <typename T>
struct tdstack <const T* const>
{
	static void push (lua_State *L, const T * const data)
	{
		if(data != NULL)
		{
			const T ** ud = lua_newuserdata(L, sizeof(const T**) );
			std::string constname = std::string("const ") + classname<T>::name();
			luaL_getmetatable(L, constname.c_str());
			lua_setmetatable(L, -2);
			*ud = (const T*)data;
		}
		else
			lua_pushnil(L);
	}
	static const T* const get (lua_State *L, int index)
	{
		std::string constname = std::string("const ") + classname<T>::name();
		return *(const T**)checkclass(L, index, constname.c_str() );
	}
};

template <typename T>
struct tdstack <T&>
{
	static void push (lua_State *L, T &data)
	{
		lua_pushlightuserdata(L, &data);
	}
	static T& get (lua_State *L, int index)
	{
		if(lua_type(L, index) == LUA_TLIGHTUSERDATA)
			return *(T*)lua_touserdata(L, index);
		luaL_error(L, "expect light user data, got %s",luaL_typename(L,index) );
		//unreachable code, but there to make compiler happy.
		return *(T*)NULL;
	}
};

template <typename T>
struct tdstack <const T&>
{
	static void push (lua_State *L, const T &data)
	{
		lua_pushlightuserdata(L, (void*)&data);
	}
	static const T& get (lua_State *L, int index)
	{
		if(lua_type(L,index) == LUA_TLIGHTUSERDATA)
			return *(T*)lua_touserdata(L,index);
		luaL_error(L, "expect light user data, got %s",luaL_typename(L,index) );
		return *(const T*)NULL;
	}
};

/*
 * Primitive types, including const char * and std::string
 */
template <>
struct tdstack <void>
{
	static void push (lua_State *L)
	{
		lua_pushnil(L);
	}
	static void get (lua_State *L, int index);
};

template <>
struct tdstack <float>
{
	static void push (lua_State *L, float data)
	{
		lua_pushnumber(L, (lua_Number)data);
	}
	static float get (lua_State *L, int index)
	{
		return (float)luaL_checknumber(L, index);
	}
};
template <>
struct tdstack <const float>
{
	static void push (lua_State *L, const float data)
	{
		lua_pushnumber(L, (lua_Number)data);
	}
	static const float get (lua_State *L, int index)
	{
		return (const float)luaL_checknumber(L, index);
	}
};
template <>
struct tdstack <float&>
{
	static void push (lua_State *L, float & data)
	{
		lua_pushnumber(L, data);
	}
	static float & get (lua_State *L, int index);
	/*{
		if(lua_type(L, index) == LUA_TLIGHTUSERDATA)
			return *(float*)lua_touserdata(L,index);
		luaL_error(L, "expected light userdata, got %",luaL_typename(L,index) );
		return *(float*)NULL;
	}*/
};
template <>
struct tdstack <const float&>
{
	static void push (lua_State *L, const float & data)
	{
		lua_pushnumber(L, data);
	}
	static const float & get (lua_State *L, int index);
	/*{
		if(lua_type(L, index) == LUA_TLIGHTUSERDATA)
			return *(const float*)lua_touserdata(L,index);
		luaL_error(L, "expected light userdata, got %",luaL_typename(L,index) );
		return *(const float*)NULL;
	}*/
};


template <>
struct tdstack <double>
{
	static void push (lua_State *L, double data)
	{
		lua_pushnumber(L, (lua_Number)data);
	}
	static double get (lua_State *L, int index)
	{
		return (double)luaL_checknumber(L, index);
	}
};

template <>
struct tdstack <signed char>
{
	static void push (lua_State *L, char data)
	{
		lua_pushinteger(L, data);
	}
	static char get (lua_State *L, int index)
	{
		return (char)luaL_checkint(L, index);
	}
};
template <>
struct tdstack <unsigned char>
{
	static void push (lua_State *L, unsigned char data)
	{
		lua_pushinteger(L, data);
	}
	static unsigned char get (lua_State *L, int index)
	{
		return (unsigned char)luaL_checkint(L, index);
	}
};
template <>
struct tdstack <unsigned short>
{
	static void push (lua_State *L, unsigned short data)
	{
		lua_pushinteger(L, data);
	}
	static unsigned short get (lua_State *L, int index)
	{
		return (unsigned short)luaL_checkint(L, index);
	}
};
template <>
struct tdstack <signed short>
{
	static void push (lua_State *L, short data)
	{
		lua_pushinteger(L, data);
	}
	static short get (lua_State *L, int index)
	{
		return (short)luaL_checkint(L, index);
	}
};

template <>
struct tdstack <int>
{
	static void push (lua_State *L, int data)
	{
		lua_pushinteger(L, data);
	}
	static int get (lua_State *L, int index)
	{
		return luaL_checkint(L, index);
	}
};

template <>
struct tdstack <unsigned int>
{
	static void push (lua_State *L, int data)
	{
		lua_pushnumber(L, data);
	}
	static unsigned int get (lua_State *L, int index)
	{
		return (unsigned int)luaL_checknumber(L, index);
	}
};
template <>
struct tdstack <const unsigned int>
{
	static void push (lua_State *L, const unsigned int data)
	{
		lua_pushnumber(L, data);
	}
	static unsigned int get (lua_State *L, int index)
	{
		return (unsigned int)luaL_checknumber(L, index);
	}
};

template <>
struct tdstack <bool>
{
	static void push (lua_State *L, bool data)
	{
		lua_pushboolean(L, data ? 1 : 0);
	}
	static bool get (lua_State *L, int index)
	{
		bool ret;
		if(lua_type(L, index) == LUA_TBOOLEAN)
			ret = lua_toboolean(L, index) != 0;
		else if(lua_isnoneornil(L, index) )
			ret = false;
		else
			luaL_typerror(L, index, "bool");
		return ret;
	}
};

template <>
struct tdstack <const char *>
{
	static void push (lua_State *L, const char *data)
	{
		lua_pushstring(L, data);
	}
	static const char *get (lua_State *L, int index)
	{
		if(lua_isnoneornil(L, index) )
			return NULL;
		return luaL_checkstring(L, index);
	}
};

template <>
struct tdstack <char *>
{
	static void push (lua_State *L, char *data)
	{
		lua_pushstring(L, (const char*)data);
	}
	static char *get (lua_State *L, int index)
	{
		if(lua_isnoneornil(L, index) )
			return NULL;
		return (char*)luaL_checkstring(L, index);
	}
};

template <>
struct tdstack <std::string>
{
	static void push (lua_State *L, const std::string &data)
	{
		lua_pushstring(L, data.c_str());
	}
	static std::string get (lua_State *L, int index)
	{
		if(lua_isnoneornil(L, index) )
			return std::string("");
		return std::string(luaL_checkstring(L, index));
	}
};

template <>
struct tdstack <const std::string &>
{
	static void push (lua_State *L, const std::string &data)
	{
		lua_pushstring(L, data.c_str());
	}
	static std::string get (lua_State *L, int index)
	{
		if(lua_isnoneornil(L, index) )
			return std::string("");
		return std::string(luaL_checkstring(L, index));
	}
};

//custom type lua_function, uses lua_ref to create a reference to a lua function.
template<>
struct tdstack<lua_function>
{
	static void push(lua_State * l, lua_function ref)
	{
		ReferenceHandler::getReference(l, (ptrdiff_t)ref);
	}
	static lua_function get(lua_State *L, int index)
	{
		ptrdiff_t fref = LUA_REFNIL;
		ptrdiff_t type = lua_type(L,index);
		if( type == LUA_TFUNCTION)
			fref = ReferenceHandler::addReference(L, index);
		else if(type == LUA_TSTRING)
			fref = extractfRefFromCString(L, luaL_checkstring(L, index) );
		//return the reference as a lua_function type.
		return (lua_function)fref;
	}
};

template<>
struct tdstack<lua_obj>
{
	static void push(lua_State * l, lua_obj ref)
	{
		ReferenceHandler::getReference(l, (ptrdiff_t)ref);
	}
	static lua_obj get(lua_State *L, int index)
	{
		ptrdiff_t ref = LUA_REFNIL;
		switch(lua_type(L, index) )
		{
		case LUA_TFUNCTION:
		case LUA_TUSERDATA:
		case LUA_TLIGHTUSERDATA:
			ref = ReferenceHandler::addReference(L, index);
			break;
		}
		//return the reference as a lua_function type.
		return (lua_obj)ref;
	}
};

template<>
struct tdstack<lua_thread>
{
	static void push(lua_State *, lua_thread thread)
	{
		lua_pushthread(thread);
	}
	static lua_thread get(lua_State *L, int index)
	{
		if(lua_type(L, index) == LUA_TTHREAD)
			return lua_tothread(L,index);
		else
			return NULL;
	}
};
template<>
struct tdstack<lua_stack>
{
	//nothing since we've already pushed an object on the stack.
	static void push(lua_State *, lua_stack)
	{

	}
	static lua_stack get(lua_State *L, int)
	{
		//always return the execution stack.
		return (lua_stack)L;
	}
};

template<>
struct tdstack<variadic_parameter*>
{
	static void push(lua_State * L, variadic_parameter * head)
	{
		variadic_node * current_node = head->head_node;
		//variadic_node * next_node = NULL;
		for(int i = 0; i < head->count && current_node != NULL; ++i)
		{
			switch(current_node->type)
			{
			case LUA_TBOOLEAN:
				tdstack<bool>::push(L, (current_node->val.bewl > 0) ? true : false);
				break;
			case LUA_TNUMBER:
				tdstack<double>::push(L, current_node->val.lua_number);
				break;
				//we assume the ud object contained here was already initialized with metatables etc, so we simply push it.
			case LUA_TUSERDATA:
			case LUA_TFUNCTION:
			case LUA_TTABLE:
				//simply unref the ref and our object is automatically pushed to the stack.
				if(current_node->val.obj_ref > LUA_REFNIL)
					ReferenceHandler::getReference(L, current_node->val.obj_ref);
				else
					tdstack<void>::push(L);
				break;
			case LUA_TSTRING:
				tdstack<const char*>::push(L, current_node->val.lua_str);
				break;
			case LUA_TNIL:
				tdstack<void>::push(L);
				break;
			case LUA_TLIGHTUSERDATA:
				lua_pushlightuserdata(L, current_node->val.l_ud);
				break;
			case LUA_TTHREAD:
				tdstack<lua_thread>::push(L, current_node->val.thread);
				break;
			}
			current_node = current_node->next;
		}
	}
	static variadic_parameter * get(lua_State *L, int index)
	{
		//get number of args we have.
		int max_args = lua_gettop(L);
		if(max_args < index)
			return NULL;
		variadic_parameter * param = new variadic_parameter;
		param->count = (max_args - index) +1;
		variadic_node * head,  *current_node, *previous_node;
		head = previous_node = current_node = new variadic_node;
		for(int i = index; i <= max_args; ++i)
		{
			if(current_node == NULL)
			{
				current_node = new variadic_node;
				previous_node->next = current_node;
			}

			current_node->type = lua_type(L,i);
			switch(current_node->type)
			{
			case LUA_TNIL:
			case LUA_TNONE:
				current_node->val.l_ud = NULL;
				break;
			case LUA_TSTRING:
				current_node->val.lua_str = strdup( tdstack<const char*>::get(L, i) );
				break;
			case LUA_TNUMBER:
				current_node->val.lua_number = (float)luaL_checknumber(L, i);
				break;
			case LUA_TTHREAD:
				current_node->val.thread = lua_tothread(L, i);
				break;
			case LUA_TBOOLEAN:
			 	current_node->val.bewl = lua_toboolean(L, i);
				break;
			case LUA_TLIGHTUSERDATA:
				current_node->val.l_ud = lua_touserdata(L, i);
				break;
			case LUA_TFUNCTION:
			case LUA_TUSERDATA:
			case LUA_TTABLE:
				//create a reference to the object.
				if(lua_type(L, index) != current_node->type)
					printf("\tExpected type (%i) instead got type (%i) \n", current_node->type, lua_type(L,index) );
				current_node->val.obj_ref = ReferenceHandler::addReference(L, index);
				break;
			}
			previous_node = current_node;
			current_node = current_node->next;
		}
		//assign our head node which links to the others.
		param->head_node = head;
		return param;
	}
};

template<>
struct tdstack<lua_table>
{
	static void push(lua_State * l, lua_table ref)
	{
		ptrdiff_t tRef = (ptrdiff_t)ref;
		if( tRef > LUA_REFNIL)
		{
			ReferenceHandler::getReference(l, (ptrdiff_t)ref);
			ReferenceHandler::removeReference(l, (ptrdiff_t)ref);
		}
		else
			lua_pushnil(l);
	}
	static lua_table get(lua_State *L, int index)
	{
		ptrdiff_t ref = LUA_REFNIL;
		if(lua_type(L, index) == LUA_TTABLE)
			ref = ReferenceHandler::addReference(L, index);
		return (lua_table)ref;
	}
};

//Template specialization for guids

template<typename T>
struct tdstack< ObjectWrap<T> >
{
	static void push(lua_State * L,  T & _value)
	{
		void * ud = lua_newuserdata(L, sizeof(ObjectWrap<T>) );
		new(ud) ObjectWrap<T>(_value);
		luaL_getmetatable(L, classname<ObjectWrap<T> >::name() );
		lua_setmetatable(L, -2);
	}

	static T get(lua_State * L, int index)
	{
		return static_cast< ObjectWrap<T>*>( checkclass(L, index, classname<ObjectWrap<T> >::name() ) )->value_;
	}
};

//For value type guids, we have to allocate a ud object.
template<>
struct tdstack<uint64>
{
	static void push(lua_State * L, uint64 guid)
	{
		if(guid == 0)
			tdstack<void>::push(L);
		else
			tdstack<ObjectWrap<uint64> >::push(L, guid);
	}
	static uint64 get(lua_State *L, int index)
	{
		return tdstack<ObjectWrap<uint64> >::get(L, index);
	}
};


template<>
struct tdstack<uint64 &>
{
	static void push(lua_State * L, uint64 & guid)
	{
		if(guid == 0)
			tdstack<void>::push(L);
		else
			tdstack<ObjectWrap<uint64> >::push(L, guid);
	}
	
	static uint64 get(lua_State *L, int index)
	{
		return tdstack<ObjectWrap<uint64> >::get(L, index);
	}
};

template<>
struct tdstack<const uint64>
{
	static void push(lua_State * L, const uint64 guid)
	{
		if(guid == 0)
			tdstack<void>::push(L);
		else
			tdstack<ObjectWrap<const uint64> >::push(L, guid);
	}
	static const uint64 get(lua_State *L, int index)
	{
		return tdstack<ObjectWrap<uint64> >::get(L, index);
	}
};
template<>
struct tdstack<const uint64 &>
{
	static void push(lua_State * L, const uint64 & guid)
	{
		if(guid == 0)
			tdstack<void>::push(L);
		else
			tdstack<ObjectWrap<const uint64> >::push(L, guid);
	}
	static const uint64 get(lua_State *L, int index)
	{
		return tdstack<ObjectWrap<const uint64> >::get(L, index);
	}
};

template<typename T>
struct tdstack< std::set<T> & >
{
	static void push(lua_State * L, std::set<T> & tree)
	{
		lua_newtable(L);
		ptrdiff_t index = 1;
		typedef typename std::set<T>::iterator iterator;
		for(iterator itr = tree.begin(); itr != tree.end(); ++itr)
		{
			tdstack<T>::push(L, (*itr) );
			lua_rawseti(L, -2, index++);
		}
	}
};


/*
 * Subclass of a type/value list, constructable from the Lua stack.
 */

template <typename Typelist, int start = 1>
struct arglist {};

template <int start>
struct arglist <NIL, start>:
	public typevallist<NIL>
{
	arglist (lua_State *L) { (void)L; }
};

template <typename Head, typename Tail, int start>
struct arglist <typelist<Head, Tail>, start>:
	public typevallist<typelist<Head, Tail> >
{
	arglist (lua_State *L):
		typevallist<typelist<Head, Tail> >
			(tdstack<Head>::get(L, start),
			arglist<Tail, start + 1>(L))
	{}
};

