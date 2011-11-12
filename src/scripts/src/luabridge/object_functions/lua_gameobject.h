#pragma once
#include "StdAfx.h"

class lua_go : public GameObject
{
	public:
		//************************************
		// Purpose :	Serves as a callback function for the go events.
		// Parameter:	ptrdiff_t - function ref.
		// Parameter:	variadic_parameter * - extra parameters to pass as paramaters to the lua function when calling it.
		// Parameter : lua_stack - a lua_State *
		//************************************
		void CallScriptEngineFunction(variadic_parameter* parameters)
		{
			PLUA_INSTANCE li_ = lua_instance.get();
			//locate the reference and clean up
			std::pair<LUA_INSTANCE::ObjectFRefMap::iterator, LUA_INSTANCE::ObjectFRefMap::iterator> frefs = li_->m_goFRefs.equal_range(GetLowGUID());
			for(; frefs.first != frefs.second; ++frefs.first)
				if(frefs.first->second == parameters)
					break;

			//Do nothing if we don't have a reference to this events' parameters.
			if(frefs.first != frefs.second)
			{
				ptrdiff_t base = lua_gettop(li_->lu) + 2;
				//push the function and it's parameters
				luabridge::tdstack<variadic_parameter*>::push(li_->lu, parameters);
				//push the self object
				luabridge::tdstack<lua_go*>::push(li_->lu, this);
				//move the self object to this index right after the pushed function
				if(lua_gettop(li_->lu) > base)
					lua_insert(li_->lu, base);
				//call the lua function.
				if(lua_pcall(li_->lu, (parameters->count - 1), 0, 0))
					lua_engine::report(li_->lu);

				//check repeats for expired events.
				variadic_node* repeatsnode = parameters->head_node->next;
				if(repeatsnode->val.bewl > 0 && --repeatsnode->val.bewl == 0)
				{
					//erase it since we no longer need to keep track of it
					li_->m_goFRefs.erase(frefs.first);

					//release resources
					if(parameters != NULL)
						cleanup_varparam(parameters, li_->lu);
				}
			}
		}

		//************************************
		// Purpose :	Registers an event that calls 'CallScriptEngineFunction' after the given duration.
		// Parameter:	lua_function - function ref
		// Parameter:	uint32 - the interval between calls.
		// Parameter:	uint32 - times to repeat(0 for infinite)
		// Parameter:	variadic_parameter * - extra parameters
		//************************************
		void RegisterScriptEngineFunction(lua_function ref , uint32 interval, uint32 repeats, variadic_parameter* extra, lua_stack stack)
		{
			ptrdiff_t fRef = (ptrdiff_t)ref;
			//valid function?
			if(fRef != LUA_REFNIL)
			{
				//store the function ref inside the parameters so we can keep track of it and any other parameters
				variadic_parameter* parameters;
				if(extra != NULL)
					parameters = extra;
				else
					parameters = new variadic_parameter;
				parameters->count += 2;
				variadic_node* frefnode = new variadic_node;
				variadic_node* repeatsnode = new variadic_node;
				//create a node of function type.
				frefnode->type = LUA_TFUNCTION;
				frefnode->val.obj_ref = fRef;
				//create a node storing repeats
				repeatsnode->type = CUSTOM_TYPE_REPEATS_ARG;
				repeatsnode->val.bewl = repeats;
				//Switch the head nodes
				if(parameters->head_node != NULL)
					repeatsnode->next = parameters->head_node; //set our parameters to follow the repeats node
				parameters->head_node = frefnode; //set our function node as the root
				frefnode->next = repeatsnode; //set our repeats node to follow the function node.
				sEventMgr.AddEvent(this, &lua_go::CallScriptEngineFunction, parameters, EVENT_LUA_GAMEOBJ_EVENTS, interval, repeats, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
				//keep a reference to the allocated block so we can free it.
				lua_instance.get()->m_goFRefs.insert(make_pair(this->GetLowGUID(), parameters));
			}
			else
				luaL_error((lua_State*)stack, "expecting valid function.");
		}

		//************************************
		// Purpose :	Removes any script engine events.
		//************************************
		void RemoveScriptEngineEvents()
		{
			PLUA_INSTANCE li_ = lua_instance.get();
			sEventMgr.RemoveEvents(this, EVENT_LUA_GAMEOBJ_EVENTS);
			//Remove any stored references
			std::pair<LUA_INSTANCE::ObjectFRefMap::iterator , LUA_INSTANCE::ObjectFRefMap::iterator > frefs = li_->m_goFRefs.equal_range(this->GetLowGUID());
			for(; frefs.first != frefs.second; ++frefs.first)
				cleanup_varparam(frefs.first->second, li_->lu);

			li_->m_goFRefs.erase(this->GetLowGUID());
		}

		void AddLoot(uint32 itemid, uint32 mincount, uint32 maxcount, lua_stack s)
		{
			lua_State* stack = (lua_State*)s;
			bool perm = false;
			float chance = 0.0f;
			if( lua_gettop( stack ) != 3 || lua_gettop( stack ) != 5 )
				return;

			if( ( lua_gettop( stack ) == 5 ) && ( lua_type(stack, 5) == LUA_TBOOLEAN ) )
			{
				perm = (lua_toboolean(stack, 5) == 1) ? true : false;
				if(lua_type(stack, 6) == LUA_TNUMBER)
					chance = (float)luaL_checknumber(stack, 6);

				QueryResult* result = WorldDatabase.Query("SELECT * FROM loot_gameobjects WHERE entryid = %u, itemid = %u", GetEntry(), itemid);
				if(result == NULL)
					WorldDatabase.Execute("REPLACE INTO loot_gameobjects VALUES (%u, %u, %f, 0, 0, 0, %u, %u )", GetEntry(), itemid, chance, mincount, maxcount );
				else
					delete result;
			}
			lootmgr.AddLoot(&loot, itemid, mincount, maxcount );
		}

		uint32 GetSpawnId()
		{
			return (m_spawn ? m_spawn->id : 0);
		}
};
