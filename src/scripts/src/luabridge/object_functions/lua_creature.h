#pragma once

class lua_creature : public Creature
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
			std::pair<LUA_INSTANCE::ObjectFRefMap::iterator, LUA_INSTANCE::ObjectFRefMap::iterator> frefs = li_->m_creatureFRefs.equal_range(this->GetLowGUID());
			for(; frefs.first != frefs.second; ++frefs.first)
				if(frefs.first->second == parameters)
					break;

			//Do nothing if we don't have a reference to this event's parameters.
			if(frefs.first != frefs.second)
			{
				ptrdiff_t base = lua_gettop(li_->lu) + 2;
				//push the function and it's parameters
				luabridge::tdstack<variadic_parameter*>::push(li_->lu, parameters);
				//push the self object
				luabridge::tdstack<lua_creature*>::push(li_->lu, this);
				//move the self object to this index right after the pushed function
				if(lua_gettop(li_->lu) > base)
					lua_insert(li_->lu, base);
				//call the lua function ignoring the repeats node.
				if(lua_pcall(li_->lu, (parameters->count - 1), 0, 0))
					lua_engine::report(li_->lu);

				//retrieve the repeats node and check if this event has expired.
				variadic_node* repeatsnode = parameters->head_node->next;
				ASSERT(repeatsnode != NULL);

				if(repeatsnode->val.bewl > 0 && --repeatsnode->val.bewl == 0) //event has expired, clean up after it.
				{
					//erase it since we no longer need to keep track of it
					li_->m_creatureFRefs.erase(frefs.first);
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
				parameters->count += 2; //we are adding a function node and a repeats node to our var params.
				variadic_node* frefnode = new variadic_node;
				variadic_node* repeatsnode = new variadic_node;
				//create a node of function type.
				frefnode->type = LUA_TFUNCTION;
				frefnode->val.obj_ref = fRef;
				//create a node storing repeats.
				repeatsnode->type = CUSTOM_TYPE_REPEATS_ARG;
				repeatsnode->val.bewl = repeats;
				//Switch the head nodes
				if(parameters->head_node != NULL)
					repeatsnode->next = parameters->head_node; //link our passed parameters to follow our repeats node.
				parameters->head_node = frefnode; //make our function node the root.
				frefnode->next = repeatsnode; //link our repeats node following the function node.
				sEventMgr.AddEvent(this, &lua_creature::CallScriptEngineFunction, parameters, EVENT_LUA_CREATURE_EVENTS, interval, repeats, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
				//keep a reference to the allocated block so we can free it.
				lua_instance.get()->m_creatureFRefs.insert(make_pair(this->GetLowGUID(), parameters));
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
			sEventMgr.RemoveEvents(this, EVENT_LUA_CREATURE_EVENTS);
			//Remove any stored references
			std::pair<LUA_INSTANCE::ObjectFRefMap::iterator, LUA_INSTANCE::ObjectFRefMap::iterator> frefs = li_->m_creatureFRefs.equal_range(this->GetLowGUID());
			for(; frefs.first != frefs.second; ++frefs.first)
				cleanup_varparam(frefs.first->second, li_->lu);

			li_->m_creatureFRefs.erase(this->GetLowGUID());

		}
};