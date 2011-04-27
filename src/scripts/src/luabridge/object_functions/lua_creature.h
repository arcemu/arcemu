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
   void CallScriptEngineFunction(variadic_parameter * parameters)
   {
	   PLUA_INSTANCE li_ = lua_instance.get();
	   //locate the reference and clean up
	   LUA_INSTANCE::ObjectFRefMap::iterator
		   itr = li_->m_creatureFRefs.find( GetLowGUID() ),
		   itend = li_->m_creatureFRefs.upper_bound( GetLowGUID() );
	   for(; itr != itend; ++itr)
		   if(itr->second == parameters)
			   break;

	   //Do nothing if we don't have a reference to this event's parameters.
	   if(itr != itend)
	   {
		   ptrdiff_t base = lua_gettop( li_->lu )+2;
		   //push the function and it's parameters
		   luabridge::tdstack<variadic_parameter*>::push(li_->lu, parameters);
		   //push the self object
		   luabridge::tdstack<lua_creature*>::push(li_->lu, this);
		   //move the self object to this index right after the pushed function
		   if(lua_gettop(li_->lu) > base)
			   lua_insert(li_->lu, base);
		   //call the lua function.
		   if(lua_pcall(li_->lu, (parameters->count), 0, 0) )
			   lua_engine::report(li_->lu);
		   //erase it since we no longer need to keep track of it
		   li_->m_creatureFRefs.erase(itr);
		   //release resources
		   if(parameters != NULL)
			   cleanup_varparam(parameters, li_->lu);
	   }
   }

   //************************************
   // Purpose :	Registers an event that calls 'CallScriptEngineFunction' after the given duration.
   // Parameter:	lua_function - function ref
   // Parameter:	uint32 - the interval between calls.
   // Parameter:	uint32 - times to repeat(0 for infinite)
   // Parameter:	variadic_parameter * - extra parameters
   //************************************
   void RegisterScriptEngineFunction(lua_function ref , uint32 interval, uint32 repeats, variadic_parameter * extra, lua_stack stack)
   {
	   ptrdiff_t fRef = (ptrdiff_t)ref;
	   //valid function?
	   if(fRef != LUA_REFNIL)
	   {
		   //store the function ref inside the parameters so we can keep track of it and any other parameters
		   variadic_parameter * parameters;
		   if(extra != NULL)
			   parameters = extra;
		   else
			   parameters = new variadic_parameter;
		   ++parameters->count;
		   variadic_node * frefnode = new variadic_node;
		   //create a node of function type.
		   frefnode->type = LUA_TFUNCTION;
		   frefnode->val.obj_ref = fRef;
		   //Switch the head nodes
		   if(parameters->head_node != NULL)
			   frefnode->next = parameters->head_node;
		   parameters->head_node = frefnode;
		   sEventMgr.AddEvent(this, &lua_creature::CallScriptEngineFunction, parameters, EVENT_LUA_CREATURE_EVENTS, interval, repeats, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		   //keep a reference to the allocated block so we can free it.
		   lua_instance.get()->m_creatureFRefs.insert( make_pair( this->GetLowGUID(), parameters) );
	   }
	   else
		   luaL_error( (lua_State*)stack, "expecting valid function.");
   }

   //************************************
   // Purpose :	Removes any script engine events.
   //************************************
   void RemoveScriptEngineEvents()
   {
	   PLUA_INSTANCE li_ = lua_instance.get();
	   sEventMgr.RemoveEvents(this, EVENT_LUA_CREATURE_EVENTS);
	   //Remove any stored references
	   LUA_INSTANCE::ObjectFRefMap::iterator 
		   itr = li_->m_creatureFRefs.find(this->GetLowGUID() ) , 
		   itend = li_->m_creatureFRefs.upper_bound(this->GetLowGUID() );
	 
	   for(; itr != itend; ++itr)
		   cleanup_varparam( itr->second, li_->lu );

	     li_->m_creatureFRefs.erase( this->GetLowGUID() );

   }

   Unit * GetRandomEnemy()
   {
	   std::vector< Unit*> enemies;
	   enemies.reserve( this->GetInRangeOppFactsSize() );
	   for( std::set< Object*>::iterator itr = this->GetInRangeOppFactsSetBegin(); itr != this->GetInRangeOppFactsSetEnd(); ++itr)
	   {
		   if( (*itr)->IsUnit() )
			   enemies.push_back( TO_UNIT( *itr) );
	   }
	   Unit * result = NULL;
	   if( enemies.size() )
		   result = enemies[RandomUInt( enemies.size()- 1) ];
	   return result;
   }

   Unit * GetRandomFriend()
   {
	   std::vector< Unit*> allies;
	   for( std::set< Object*>::iterator itr = this->GetInRangeSameFactsSetBegin(); itr != this->GetInRangeSameFactsSetEnd(); ++itr)
	   {
		   if( (*itr)->IsUnit() )
			   allies.push_back( TO_UNIT( *itr) );
	   }
	   Unit * result = NULL;
	   if( allies.size() )
		   result = allies[RandomUInt( allies.size()- 1) ];
	   return result;
   }

   Player * GetRandomPlayer(int flag)
   {
	   const unsigned SHORT_DISTANCE = 8;
	   const unsigned LONG_DISTANCE = 30;
	   std::vector< Player*> players;
	   int dist;
	   Unit * maintank;
	   std::set< Object*>::iterator itr = this->GetInRangePlayerSetBegin();
	   std::set< Object*>::iterator itend = this->GetInRangePlayerSetEnd();
	   switch(flag)
	   {
	   case RANDOM_ANY:
		   for(; itr != itend; ++itr)
			   players.push_back( TO_PLAYER( *itr) );
		   break;
	   case RANDOM_IN_SHORTRANGE:
		   for(; itr != itend; ++itr)
			   if( float2int32( this->CalcDistance(*itr) ) <= SHORT_DISTANCE)
				   players.push_back( TO_PLAYER(*itr) );
		   break;
	   case RANDOM_IN_MIDRANGE:
		   for(; itr != itend; ++itr)
		   {
			   dist = float2int32( this->CalcDistance( *itr) );
			   if(dist > SHORT_DISTANCE && dist < LONG_DISTANCE)
				   players.push_back( TO_PLAYER(*itr) );
		   }
		   break;
	   case RANDOM_IN_LONGRANGE:
		   for(; itr != itend; ++itr)
			   if( float2int32( this->CalcDistance(*itr) ) >= LONG_DISTANCE)
				   players.push_back( TO_PLAYER(*itr) );
		   break;
	   case RANDOM_WITH_MANA:
		   for(; itr != itend; ++itr)
			   if( TO_PLAYER( (*itr) )->GetPowerType() == POWER_TYPE_MANA )
				   players.push_back( TO_PLAYER( (*itr) ) );
		   break;
	   case RANDOM_WITH_RAGE:
		   for(; itr != itend; ++itr)
			   if( TO_PLAYER( (*itr) )->GetPowerType() == POWER_TYPE_RAGE)
				   players.push_back( TO_PLAYER( (*itr) ) );
		   break;
	   case RANDOM_WITH_ENERGY:
		   for(; itr != itend; ++itr)
			   if( TO_PLAYER( (*itr) )->GetPowerType() == POWER_TYPE_ENERGY)
				   players.push_back( TO_PLAYER( (*itr) ) );
		   break;
	   case RANDOM_NOT_MAINTANK:
		   maintank = this->GetAIInterface()->GetMostHated();
		   if(maintank != NULL)
		   {
			   for(; itr != itend; ++itr)
				   if( (*itr) != maintank )
					   players.push_back( TO_PLAYER( (*itr) ) );
		   }
		   break;
	   default:
		   break;
	   }

	   Player * result = NULL;
	   if( players.size() )
		   result = players[ RandomUInt( players.size()-1 ) ];

	   return result;
   }

   Unit * GetClosestFriend()
   {
	   float closest_distance = 9999.99f;
	   float current_distance;
	   Object * closest_friend = NULL;
	   for( std::set<Object*>::iterator itr = this->GetInRangeSameFactsSetBegin(); itr != this->GetInRangeSameFactsSetEnd(); ++itr)
	   {
		   current_distance = this->CalcDistance( *itr);
		   if(current_distance <= closest_distance )
		   {
			   closest_friend = *itr;
			   closest_distance = current_distance;
		   }
	   }
	   return TO_UNIT(closest_friend);
   }

   Unit * GetClosestEnemy()
   {
	   float closest_distance = 9999.99f;
	   float current_distance;
	   Object * closest_enemy = NULL;
	   for( std::set<Object*>::iterator itr = this->GetInRangeOppFactsSetBegin(); itr != this->GetInRangeOppFactsSetEnd(); ++itr)
	   {
		   current_distance = this->CalcDistance( *itr);
		   if(current_distance <= closest_distance)
		   {
			   closest_enemy = *itr;
			   closest_distance = current_distance;
		   }
	   }
	   return TO_UNIT(closest_enemy);
   }

   Player * GetClosestPlayer()
   {
	   float closest_distance = 9999.99f;
	   float current_distance;
	   Object * closest_player = NULL;
	   for( std::set<Object*>::iterator itr = this->GetInRangePlayerSetBegin(); itr != this->GetInRangePlayerSetEnd(); ++itr)
	   {
		   current_distance = this->CalcDistance( *itr);
		   if(current_distance <= closest_distance)
		   {
			   closest_player = *itr;
			   closest_distance = current_distance;
		   }
	   }
	   return TO_PLAYER(closest_player);
   }
};