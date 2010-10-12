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
//Paroxysm : modified to expect pointer types.
template <typename T>
struct tdstack
{
	static void push (lua_State *L, T data)
	{
		if(sizeof(T) <= 4)
			lua_pushinteger(L, (int)data);
		else
			lua_pushnil(L);
	}
	static T get (lua_State *L, int index)
	{
		return (T)luaL_checkint(L,index);
	}
};
template <typename T>
struct tdstack<T*>
{
	static void push (lua_State *L, T * data)
	{
		//created a shared ptr from ud block.
		T ** ud_ptr = (T**)lua_newuserdata(L, sizeof(T**) );
		luaL_getmetatable(L, classname<T>::name());
		lua_setmetatable(L, -2);
		*ud_ptr = data;
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
		const T** ud_ptr = (const T**)lua_newuserdata(L, sizeof(const T**) );
		std::string constname = std::string("const ") + classname<T>::name();
		luaL_getmetatable(L, constname.c_str());
		lua_setmetatable(L, -2);
		*ud_ptr = data;
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
		const T ** ud = lua_newuserdata(L, sizeof(const T**) );
		luaL_getmetatable(L, classname<T>::name() );
		lua_setmetatable(L,-2);
		*ud = data;
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
		const T ** ud = lua_newuserdata(L, sizeof(const T**) );
		std::string constname = std::string("const ") + classname<T>::name();
		luaL_getmetatable(L, constname.c_str());
		lua_setmetatable(L, -2);
		*ud = (const T*)data;
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
	static void push (lua_State *L, void *)
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
	static float & get (lua_State *L, int index)
	{
		if(lua_type(L, index) == LUA_TLIGHTUSERDATA)
			return *(float*)lua_touserdata(L,index);
		luaL_error(L, "expected light userdata, got %",luaL_typename(L,index) );
		return *(float*)NULL;
	}
};
template <>
struct tdstack <const float&>
{
	static void push (lua_State *L, const float & data)
	{
		lua_pushlightuserdata(L, (void*)&data);
	}
	static const float & get (lua_State *L, int index)
	{
		if(lua_type(L, index) == LUA_TLIGHTUSERDATA)
			return *(const float*)lua_touserdata(L,index);
		luaL_error(L, "expected light userdata, got %",luaL_typename(L,index) );
		return *(const float*)NULL;
	}
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
		luaL_checktype(L, index, LUA_TBOOLEAN);
		// In MSC, disable "bool to int conversion" warning
#		ifdef _MSC_VER
#			pragma warning (push)
#			pragma warning (disable: 4800)
#		endif

		return (bool)lua_toboolean(L, index);

#		ifdef _MSC_VER
#			pragma warning (pop)
#		endif
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
		return luaL_checkstring(L, index);
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
		return std::string(luaL_checkstring(L, index));
	}
};

//custom type lua_function, uses lua_ref to create a reference to a lua function.
template<>
struct tdstack<lua_function>
{
	static void push(lua_State * l, lua_function ref)
	{
		lua_getref(l, (int)ref);
	}
	static lua_function get(lua_State *L, int index)
	{
		int fref = LUA_REFNIL;
		int type = lua_type(L,index);
		if( type == LUA_TFUNCTION)
		{
			lua_pushvalue(L,index);
			fref = lua_ref(L,true);
		}
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
		lua_getref(l, (int)ref);
	}
	static lua_obj get(lua_State *L, int index)
	{
		int ref = LUA_REFNIL;
		switch(lua_type(L, index) )
		{
		case LUA_TFUNCTION:
		case LUA_TUSERDATA:
		case LUA_TLIGHTUSERDATA:
			lua_pushvalue(L,index);
			ref = lua_ref(L,true);
			break;
		}
		//return the reference as a lua_function type.
		return (lua_obj)ref;
	}
};

template<>
struct tdstack<lua_thread>
{
	static void push(lua_State * L, lua_thread thread)
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
	static void push(lua_State * L, lua_stack)
	{

	}
	static lua_stack get(lua_State *L, int index)
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
		variadic_node * next_node = NULL;
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
				tdstack<int>::push(L, current_node->val.obj_ref);
				break;
			case LUA_TSTRING:
				tdstack<const char*>::push(L, current_node->val.lua_str);
				break;
			case LUA_TNIL:
				tdstack<void>::push(L,NULL);
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
		//cleanup_varparam(head,L);
	}
	static variadic_parameter * get(lua_State *L, int index)
	{
		//get number of args we have.
		int max_args = lua_gettop(L);
		if(max_args <= index)
			return NULL;
		variadic_parameter * param = new variadic_parameter;
		param->count = (max_args - index);
		variadic_node * head,  *current_node;
		head = current_node = new variadic_node;
		for(int i = index; i <= max_args; ++i)
		{
			current_node->type = lua_type(L,i);
			switch(current_node->type)
			{
			case LUA_TNIL:
			case LUA_TNONE:
				current_node->val.l_ud = NULL;
				break;
			case LUA_TNUMBER:
				current_node->val.lua_number = (float)luaL_checknumber(L, i);
				break;
			case LUA_TTHREAD:
				current_node->val.thread = lua_tothread(L, i);
				break;
			case LUA_TBOOLEAN:
				current_node->val.bewl = luaL_checkint(L,i);
				break;
			case LUA_TLIGHTUSERDATA:
			case LUA_TFUNCTION:
			case LUA_TUSERDATA:
				//create a reference to the object.
				lua_pushvalue(L, i);
				current_node->val.obj_ref = lua_ref(L, true);
				break;
			case LUA_TSTRING:
				current_node->val.lua_str = luaL_checkstring(L,i);
				break;
			}
			current_node->next = new variadic_node;
			current_node = current_node->next;
		}
		//assign a null last node.
		current_node->next = NULL;
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
		if( (int)ref != 0)
		{
			lua_getref(l, (int)ref);
			lua_unref(l, (int)ref);
		}
		else
			lua_pushnil(l);
	}
	static lua_table get(lua_State *L, int index)
	{
		int ref = LUA_REFNIL;
		if(lua_type(L, index) == LUA_TTABLE)
		{
			lua_pushvalue(L, index);
			ref = lua_ref(L,true);
		}
		return (lua_table)ref;
	}
};

//Template specialization for guids

//for value type guids, we have to allocate a ud object.
template<>
struct tdstack<uint64>
{
	static void push(lua_State * L, uint64 guid)
	{
		//This is scary, can possible create a mem leak if we don't collect this through a getter method.
		uint64 * ud = (uint64*)lua_newuserdata(L, sizeof(uint64) );
		if(ud != NULL)
			*ud = guid;
	}
	static uint64 get(lua_State *L, int index)
	{
		uint64 guid = 0;
		if(lua_type(L, index) == LUA_TUSERDATA)
			guid = *(uint64*)lua_touserdata(L,index);
		return guid;
	}
};

//For reference type guids, we are in luck, we can simply push the pointer of it as a light user data and retrieve it. It's clean and we don't have to worry about gc.
template<>
struct tdstack<uint64 &>
{
	
	static void push(lua_State * L, uint64 & guid)
	{
		//This is scary, can possible create a mem leak if we don't collect this through a getter method.
		lua_pushlightuserdata(L, &guid);
	}
	
	static uint64 get(lua_State *L, int index)
	{
		if(lua_type(L, index) == LUA_TLIGHTUSERDATA)
			return *(uint64*)lua_touserdata(L,index);
		luaL_error(L,"expected lightuserdata got %s",luaL_typename(L,index) );
		return 0;
	}
};

template<>
struct tdstack<const uint64>
{
	static void push(lua_State * L, const uint64 guid)
	{
		//This is scary, can possible create a mem leak if we don't collect this through a getter method.
		uint64 * ud = (uint64*)lua_newuserdata(L, sizeof(uint64) );
		if(ud != NULL)
			*ud = guid;
	}
	static const uint64 get(lua_State *L, int index)
	{
		uint64 guid = 0;
		if(lua_type(L, index) == LUA_TUSERDATA)
			guid = *(uint64*)lua_touserdata(L,index);
		return guid;
	}
};
template<>
struct tdstack<const uint64 &>
{
	static void push(lua_State * L, const uint64 & guid)
	{
		lua_pushlightuserdata(L, (void*)&guid);
	}
	static const uint64 & get(lua_State *L, int index)
	{
		if(lua_type(L, index) == LUA_TLIGHTUSERDATA)
			return *(uint64*)lua_touserdata(L,index);
		luaL_error(L, "expected light userdata got %s", luaL_typename(L,index) );
		return *(uint64*)0x0;
	}
};


/*
 * Subclass of a type/value list, constructable from the Lua stack.
 */

template <typename Typelist, int start = 1>
struct arglist {};

template <int start>
struct arglist <nil, start>:
	public typevallist<nil>
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

