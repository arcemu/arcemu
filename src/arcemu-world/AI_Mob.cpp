#include "StdAfx.h"

MobAI::MobAI(Unit * self) : AIInterface(self)
{
	m_ShootSpell = NULL;
	m_nextTarget = 0;
	m_aiAgent = AGENT_NULL;
	m_currentWaypoint = 0;
	m_WaypointFlags = 0;
	m_type = AITYPE_MOB;
	timed_emotes = NULL;
	timed_emote_expire = 0xFFFFFFFF;
	m_waypoints = NULL;
	memset(&m_prevTargetLocation, 0, sizeof(LocationVector) );
	m_autoWipeRange = 100;
	m_mostHated = 0;
	m_mostHatedThreat = 0;
	m_secondHated = 0;
	m_secondHatedThreat = 0;
	misc_attributes = 0;
}
void MobAI::addSpell(AI_Spell * nspell)
{
	if(nspell != NULL)
		m_spells.push_back( nspell);
}
void MobAI::removeSpell(AI_Spell* oSpell)
{
	SpellStore::iterator itr = m_spells.begin();
	for(; itr != m_spells.end(); ++itr)
	{
		if( (*itr) == oSpell)
		{
			m_spells.erase(itr);
			break;
		}
	}
}
void MobAI::HandleEvent(uint32 event, Unit* pUnit, uint32 misc)
{
	if(getAIState() != STATE_EVADE)
	{
		switch(event)
		{
			//Divided up the code for readability.
		case EVENT_ENTERCOMBAT:
			OnCombatStart(pUnit);
			break;
		case EVENT_LEAVECOMBAT:
			OnCombatStop(pUnit);
			break;
		case EVENT_UNITDIED:
			OnDeath(pUnit);
			break;
		case EVENT_DAMAGETAKEN:
			{
				if(TO_CREATURE(m_Self)->has_combat_text )
					objmgr.HandleMonsterSayEvent( TO_CREATURE(m_Self), MONSTER_SAY_EVENT_ON_DAMAGE_TAKEN );
				SpellStore::iterator itr = m_spells.begin();
				for(; itr != m_spells.end(); ++itr)
				{
					if( (*itr) != NULL && (*itr)->event_type == EVENT_DAMAGETAKEN)
						m_spellDeck.push_back( (*itr));
				}
				if(pUnit != NULL) 
				{
					CALL_SCRIPT_EVENT(m_Self, OnDamageTaken)(pUnit, misc);
					if(!modThreatByPtr(pUnit, misc))
						addNewTarget(pUnit->GetGUID(),misc);
					pUnit->CombatStatus.OnDamageDealt(m_Self);
				}
			}break;
		case EVENT_FOLLOWOWNER:
			{
				setAIState(STATE_FOLLOWING);
				StopMovement(0);
				setUnitToFollow( (pUnit != NULL) ? pUnit->GetGUID() : 0);
				setFollowDistance(misc);
				setFollowAngle( (float)M_PI);
				wipeHateList();
				setHasPanicked(false);
				setHasCalledHelp(false);
				setNextTarget(NULL);
			}break;

		case EVENT_FEAR:
			{
				if(pUnit != NULL) 
				{
					CALL_SCRIPT_EVENT(m_Self, OnFear)(pUnit, 0);
					setAIState(STATE_FEAR);
					StopMovement(0);
					setHasPanicked(false);
					setHasCalledHelp(false);
					setAIMoveFlags(AIMOVESTATE_RUN);
					disableSpells();
				}
			}break;

		case EVENT_UNFEAR:
			{
				disableSpells(false);
				setAIState(STATE_IDLE); // we need this to prevent permanent fear, wander, and other problems
				StopMovement(0);
			}break;

		case EVENT_WANDER:
			{
				if(pUnit != NULL) 
				{
					setAIState(STATE_WANDER);
					StopMovement(0);
					setHasPanicked(false);
					setHasCalledHelp(false);
					disableSpells();
					setAIMoveFlags(AIMOVESTATE_WALK);
				}
			}break;

		case EVENT_UNWANDER:
			{
				disableSpells(false);
				setAIState(STATE_IDLE); // we need this to prevent permanent fear, wander, and other problems
				StopMovement(0);
			}break;
		}
	}
}
void MobAI::OnCombatStart(Unit * enemy)
{
	if( enemy == NULL || enemy->IsDead() || m_Self->IsDead() ) return;

	// set the target first
	if(enemy && enemy->GetInstanceID() == m_Self->GetInstanceID())
		m_Self->SetTargetGUID( enemy->GetGUID());

	StopMovement(0);
	setNextTarget(enemy);
	m_mostHated = enemy->GetGUID();
	m_secondHated = m_mostHated;
	m_mostHatedThreat = 1;
	m_secondHatedThreat = 1;
	/* send the message */
	Creature * self = TO_CREATURE(m_Self);
	if( self->has_combat_text )
		objmgr.HandleMonsterSayEvent( self, MONSTER_SAY_EVENT_ENTER_COMBAT );

	CALL_SCRIPT_EVENT(m_Self, OnCombatStart)(enemy);

	if( self->m_spawn && ( self->m_spawn->channel_target_go || self->m_spawn->channel_target_creature))
	{
		m_Self->SetChannelSpellId( 0 );
		m_Self->SetChannelSpellTargetGUID( 0 );
	}

	// Stop the emote - change to fight emote
	m_Self->SetEmoteState(EMOTE_STATE_READY1H );
	m_returnToVector.x = m_Self->GetPositionX();
	m_returnToVector.y = m_Self->GetPositionY();
	m_returnToVector.z = m_Self->GetPositionZ();

	//Update speed
	setAIMoveFlags(AIMOVESTATE_RUN);

	// dismount if mounted
	m_Self->SetMount(0);
	setAIState(STATE_ATTACKING);
	if(m_Self->GetMapMgr() && m_Self->GetMapMgr()->GetMapInfo() && m_Self->GetMapMgr()->GetMapInfo()->type == INSTANCE_RAID)
	{
		if(self->GetCreatureInfo() && self->GetCreatureInfo()->Rank == 3)
			m_Self->GetMapMgr()->AddCombatInProgress(m_Self->GetGUID());
	}
	if (enemy->IsPlayer() && TO_PLAYER(enemy)->InGroup())
	{
		modThreatByPtr(enemy, 1);
		Group *pGroup = static_cast<Player*>(enemy)->GetGroup();

		Player *pGroupGuy;
		GroupMembersSet::iterator itr;
		pGroup->Lock();
		for(uint32 i = 0; i < pGroup->GetSubGroupCount(); i++) {
			for(itr = pGroup->GetSubGroup(i)->GetGroupMembersBegin(); itr != pGroup->GetSubGroup(i)->GetGroupMembersEnd(); ++itr)
			{
				pGroupGuy = (*itr)->m_loggedInPlayer;
				if( pGroupGuy && pGroupGuy->isAlive() && m_Self->GetMapMgr() == pGroupGuy->GetMapMgr() && pGroupGuy->GetDistanceSq(enemy) <= 40*40) //50 yards for now. lets see if it works
					m_Self->GetAIInterface()->AttackReaction(pGroupGuy, 1, 0);
			}
		}
		pGroup->Unlock();
	}
	//Zack : Put mob into combat animation. Take out weapons and start to look serious :P
	m_Self->smsg_AttackStart(enemy );
	SpellStore::iterator itr = m_spells.begin();
	//For all spells that get cast on combat, lets go ahead and cast them.
	for(; itr != m_spells.end(); ++itr)
	{
		if( (*itr) != NULL && (*itr)->event_type == EVENT_ENTERCOMBAT)
			m_spellDeck.push_back( (*itr) );
	}
}
void MobAI::OnCombatStop(Unit * enemy)
{
	//if( enemy == NULL ) return;	

	Unit* target = NULL;
	Creature * self = TO_CREATURE(m_Self);
	if (m_Self->GetMapMgr() && m_Self->GetMapMgr()->GetMapInfo() )
	{
		switch (m_Self->GetMapMgr()->GetMapInfo()->type)
		{
		case INSTANCE_NULL:
		case INSTANCE_BATTLEGROUND:
			if(m_autoWipeRange && calcDistanceFromHome() < m_autoWipeRange)
				target = findTarget();
			break;
		case INSTANCE_RAID:
		case INSTANCE_NONRAID:
		case INSTANCE_MULTIMODE:
			target = findTarget();
			break;
		}

		if(target != NULL)
		{
			AttackReaction(target, 1, 0);
			return;
		}
	}
	if(self->has_combat_text )
		objmgr.HandleMonsterSayEvent(self, MONSTER_SAY_EVENT_ON_COMBAT_STOP );

	if(self->original_emotestate )
		m_Self->SetEmoteState(self->original_emotestate );
	else
		m_Self->SetEmoteState(0 );
	if(self->m_spawn)
	{
		if(self->m_spawn->channel_target_go)
			sEventMgr.AddEvent(self, &Creature::ChannelLinkUpGO, self->m_spawn->channel_target_go, EVENT_CREATURE_CHANNEL_LINKUP, 1000, 5, 0 );

		if(self->m_spawn->channel_target_creature)
			sEventMgr.AddEvent(self, &Creature::ChannelLinkUpCreature, self->m_spawn->channel_target_creature, EVENT_CREATURE_CHANNEL_LINKUP, 1000, 5, 0 );
	}

	setAIMoveFlags(AIMOVESTATE_RUN);
	setHasPanicked(false);
	setHasCalledHelp(false);
	setNextTarget(NULL);
	m_mostHated = 0;
	m_mostHatedThreat = 0;
	m_secondHated = 0;
	m_secondHatedThreat = 0;
	m_Self->CombatStatus.Vanished();

	setAIState(STATE_EVADE);
	StopMovement(0);
	CALL_SCRIPT_EVENT(m_Self, OnCombatStop)(getUnitToFollow());
	setUnitToFollow(0);
	if(m_Self->isAlive() )
	{
		if(m_returnToVector.x > FLOAT_E && m_returnToVector.y > FLOAT_E && m_returnToVector.z > FLOAT_E)
			MoveTo(m_returnToVector.x,m_returnToVector.y,m_returnToVector.z,m_Self->GetSpawnO());
		else
		{
			MoveTo(m_Self->GetSpawnX(),m_Self->GetSpawnY(),m_Self->GetSpawnZ(),m_Self->GetSpawnO());
			m_returnToVector.x = m_Self->GetSpawnX();
			m_returnToVector.y = m_Self->GetSpawnY();
			m_returnToVector.z = m_Self->GetSpawnZ();
		}

		//clear tagger.
		self->UnTag();
		self->SetUInt32Value(UNIT_DYNAMIC_FLAGS,self->GetUInt32Value(UNIT_DYNAMIC_FLAGS) & ~(U_DYN_FLAG_TAGGED_BY_OTHER |U_DYN_FLAG_LOOTABLE));
	}

	if(self->GetMapMgr() && m_Self->GetMapMgr()->GetMapInfo() && self->GetMapMgr()->GetMapInfo()->type == INSTANCE_RAID)
	{
		if(self->GetCreatureInfo() && self->GetCreatureInfo()->Rank == 3)
			self->GetMapMgr()->RemoveCombatInProgress(self->GetGUID());
	}
	if( self->GetProto() && self->m_spawn )
		self->SetMount(self->m_spawn->MountedDisplayID );
	//Zack : not sure we need to send this. Did not see it in the dumps since mob died eventually but it seems logical to make this
	m_Self->smsg_AttackStop( enemy );
	SpellStore::iterator itr = m_spells.begin();
	//For all spells that get cast on combat stop, lets go ahead and cast them.
	for(; itr != m_spells.end(); ++itr)
	{
		if( (*itr) != NULL && (*itr)->event_type == EVENT_LEAVECOMBAT)
			m_spellDeck.push_back( (*itr) );
	}
}
void MobAI::OnDeath(Unit * enemy)
{
	if( enemy == NULL ) return;
	Creature * self = TO_CREATURE(m_Self);
	if( self->has_combat_text )
		objmgr.HandleMonsterSayEvent( self, MONSTER_SAY_EVENT_ON_DIED );

	CALL_SCRIPT_EVENT(m_Self, OnDied)(enemy);
	CALL_INSTANCE_SCRIPT_EVENT( m_Self->GetMapMgr(), OnCreatureDeath )(self, enemy );

	setAIState(STATE_IDLE);
	StopMovement(0);
	StopMovement(0);
	wipeHateList();
	setHasPanicked(false);
	setHasCalledHelp(false);
	disableSpells();
	setNextTarget(NULL);
	m_currentWaypoint = 0;

	Instance *pInstance = NULL;
	if( m_Self->GetMapMgr() )
		pInstance = m_Self->GetMapMgr()->pInstance;

	if(pInstance != NULL && (pInstance->m_mapInfo->type == INSTANCE_RAID 
		|| pInstance->m_mapInfo->type == INSTANCE_NONRAID 
		|| pInstance->m_mapInfo->type == INSTANCE_MULTIMODE ) )
	{
		InstanceBossInfoMap *bossInfoMap = objmgr.m_InstanceBossInfoMap[m_Self->GetMapMgr()->GetMapId()];
		bool found = false;

		if(IS_PERSISTENT_INSTANCE(pInstance) && bossInfoMap != NULL && self->GetProto())
		{
			uint32 npcGuid = self->GetProto()->Id;
			InstanceBossInfoMap::const_iterator bossInfo = bossInfoMap->find(npcGuid);
			if(bossInfo != bossInfoMap->end())
			{
				found = true;
				pInstance->m_killedNpcs.insert( npcGuid );
				pInstance->SaveToDB();
				for(InstanceBossTrashList::iterator trash = bossInfo->second->trash.begin(); trash != bossInfo->second->trash.end(); ++trash)
				{
					Creature *c = m_Self->GetMapMgr()->GetSqlIdCreature((*trash));
					if(c != NULL)
						c->m_noRespawn = true;
				}
				if(!pInstance->m_persistent)
				{
					pInstance->m_persistent = true;
					pInstance->SaveToDB();
					for(PlayerStorageMap::iterator itr = m_Self->GetMapMgr()->m_PlayerStorage.begin(); itr != m_Self->GetMapMgr()->m_PlayerStorage.end(); ++itr)
						(*itr).second->SetPersistentInstanceId(pInstance);
				}
			}
		}

		if (!found) 
		{
			// No instance boss information ... so fallback ...
			uint32 npcGuid = self->GetSQL_id();
			pInstance->m_killedNpcs.insert( npcGuid );
			pInstance->SaveToDB();
		}
	}
	if(m_Self->GetMapMgr() && m_Self->GetMapMgr()->GetMapInfo() && m_Self->GetMapMgr()->GetMapInfo()->type == INSTANCE_RAID)
	{
		if(self->GetCreatureInfo() && self->GetCreatureInfo()->Rank == 3)
			m_Self->GetMapMgr()->RemoveCombatInProgress(m_Self->GetGUID());
	}
}
void MobAI::OnUpdate()
{
	float tdist = 0.0f;

	_UpdateTimer(AIINTERFACE_UPDATE_INTERVAL);
	updateTargets();
	uint8 state = getAIState();
	if(m_Self->isAlive() )
	{
		/* Easier to read with a switch statement */
		switch(state)
		{
		case STATE_IDLE:
		case STATE_EVADE:
		case STATE_FOLLOWING:
		case STATE_FEAR:
		case STATE_FLEEING:
		case STATE_WANDER:
		case STATE_SCRIPTMOVE:
		case STATE_SCRIPTIDLE:
			break;
		default:
			updateCombat(AIINTERFACE_UPDATE_INTERVAL);
		}
	}

	_UpdateMovement(AIINTERFACE_UPDATE_INTERVAL);
	handleMovement();
	if(state == STATE_EVADE)
	{
		tdist = m_Self->CalcDistance(m_returnToVector.x,m_returnToVector.y,m_returnToVector.z);
		if(tdist <= 2.0f)
		{
			setAIState(STATE_IDLE);
			m_returnToVector.x = m_returnToVector.y = m_returnToVector.z = 0.0f;
			setAIMoveFlags(AIMOVESTATE_WALK);
			if(canResetHp() )
				m_Self->SetHealth(m_Self->GetMaxHealth());
		}
	}

	if (state == STATE_IDLE && m_creatureState == STOPPED && !m_nextTarget && m_Self->isAlive() )
	{
		if ( timed_emote_expire <= AIINTERFACE_UPDATE_INTERVAL ) // note that creature might go idle and p_time might get big next time ...We do not skip emotes because of lost time
		{
			if ( (*next_timed_emote)->type == 1) //standstate
			{
				m_Self->SetStandState( (uint8)(*next_timed_emote)->value);
				m_Self->SetUInt32Value ( UNIT_NPC_EMOTESTATE, 0 );
			}
			else if ( (*next_timed_emote)->type == 2) //emotestate
			{
				m_Self->SetUInt32Value ( UNIT_NPC_EMOTESTATE, (*next_timed_emote)->value );
				m_Self->SetStandState( 0 );
			}
			else if ( (*next_timed_emote)->type == 3) //oneshot emote
			{
				m_Self->SetUInt32Value ( UNIT_NPC_EMOTESTATE, 0 );
				m_Self->SetStandState( 0 );
				m_Self->Emote( (EmoteType)(*next_timed_emote)->value );         // Animation
			}
			if ( (*next_timed_emote)->msg )
				m_Self->SendChatMessage((*next_timed_emote)->msg_type, (*next_timed_emote)->msg_lang, (*next_timed_emote)->msg);

			timed_emote_expire = (*next_timed_emote)->expire_after; //should we keep lost time ? I think not 
			++next_timed_emote;
			if ( next_timed_emote == timed_emotes->end() )
				next_timed_emote = timed_emotes->begin();
		}
		else 
			timed_emote_expire -= AIINTERFACE_UPDATE_INTERVAL;
	}
}
void MobAI::_UpdateTimer(uint32 p_time)
{
	if(m_updateAssistTimer > p_time)
		m_updateAssistTimer -= p_time;
	else
	{
		setBehavior(BEHAVIOR_UPDATEASSISTARGETS);
		m_updateAssistTimer = TARGET_UPDATE_INTERVAL_ON_PLAYER * 2 - m_updateAssistTimer - p_time;
	}

	if(m_updateTargetsTimer > p_time)
		m_updateTargetsTimer -= p_time;
	else
	{
		setBehavior(BEHAVIOR_UPDATETARGETS);
		m_updateTargetsTimer = TARGET_UPDATE_INTERVAL_ON_PLAYER * 2 - m_updateTargetsTimer - p_time;
	}
}

void MobAI::updateTargets()
{
	if(getTargettingDisable() ) return;
	AI_State state = (AI_State)getAIState();

    AssistTargetSet::iterator i, i2;
	TargetMap::iterator itr, it2;

	// Find new Assist Targets and remove old ones
	if(state == STATE_FLEEING)
		findFriends(10.0f);
	///else if(state != STATE_IDLE && state != STATE_SCRIPTIDLE)
		//findFriends(8.0f);

	if( hasBehavior(BEHAVIOR_UPDATEASSISTARGETS) )
	{
		clearBehavior(BEHAVIOR_UPDATEASSISTARGETS);
		Unit * obj = NULL;
		for(i = m_assistTargets.begin(); i != m_assistTargets.end();)
		{
			i2 = i++;
			obj = m_Self->GetMapMgr()->GetUnit((*i2));
			if( obj == NULL || obj->event_GetInstanceID() != m_Self->event_GetInstanceID() ||
				!obj->isAlive() || m_Self->CalcDistance(obj) >= 50.0f || !obj->CombatStatus.IsInCombat() || !(obj->m_phase & m_Self->m_phase) )
			{
				m_assistTargets.erase( i2 );
			}
		}
	}

	if( hasBehavior(BEHAVIOR_UPDATETARGETS) )
	{
		clearBehavior(BEHAVIOR_UPDATETARGETS);

		lockTargets(true);
		for(itr = m_aiTargets.begin(); itr != m_aiTargets.end();)
		{
			it2 = itr++;

			Unit * target = m_Self->GetMapMgr()->GetUnit( it2->first );
			if(target == NULL || !target->IsInWorld() || !target->isAlive() || !(target->m_phase & m_Self->m_phase) )
				m_aiTargets.erase( it2 );
			else 
			{
				bool instance = false;
				if (m_Self->GetMapMgr() && m_Self->GetMapMgr()->GetMapInfo())
				{
					switch (m_Self->GetMapMgr()->GetMapInfo()->type)
					{
					case INSTANCE_RAID:
					case INSTANCE_NONRAID:
					case INSTANCE_MULTIMODE:
						instance = true;
						break;
					}
				}

				if( target->event_GetInstanceID() != m_Self->event_GetInstanceID() 
					|| ( (!instance && m_Self->CalcDistance(target) >= 50.0f)  ) )
				{
					m_aiTargets.erase( it2 );
				}
			}
		}
		if(canUpdateMostHated() )
		{
			uint64 tempTank = 0;
			uint64 offTank = 0;
			uint64 guid = 0;
			uint64 threat = 0;
			uint32 tankThreat = 0;
			uint32 offtankThreat = 0;
			//Now we update our most hated and second hated.
			for(itr = m_aiTargets.begin(); itr != m_aiTargets.end(); ++itr)
			{
				guid = itr->first;
				threat = itr->second;
				if(threat > offtankThreat && threat > tankThreat)
					tempTank = guid;
				if(threat > offtankThreat && threat < tankThreat)
					offTank = guid;

			}
			if(tempTank)
			{
				m_mostHated = tempTank;
				m_mostHatedThreat = tankThreat;
			}
			if(offTank)
			{
				m_secondHated = offTank;
				m_secondHatedThreat = offtankThreat;
			}
			setCanUpdateMostHated(false);
		}
	}
	if(m_aiTargets.size() == 0)
	{
		switch(state)
		{
		case STATE_IDLE:
		case STATE_ATTACKING:
		case STATE_CASTING:
		case STATE_FOLLOWING:
			findTarget();
		break;
		}
	}
}

void MobAI::updateCombat(uint32 p_time)
{
	if( getCombatDisable() ) return;
	if( (m_autoWipeRange && m_Self->CalcDistance(m_returnToVector.x,m_returnToVector.y,m_returnToVector.z) > m_autoWipeRange) )
	{
		HandleEvent( EVENT_LEAVECOMBAT, m_Self, 0 );
		return;
	}

	//just make sure we are not hitting self. 
	// This was reported as an exploit.Should never occur anyway
	if( getNextTarget() == m_Self )
		setNextTarget( getMostHated() );

	//AI_State state = (AI_State)getAIState();
	Unit * nextTarget = getNextTarget();

	// If creature is very far from spawn point return to spawnpoint
	// If at instance don't return -- this is wrong ... instance creatures always returns to spawnpoint, dunno how do you got this idea. 
	// If at instance returns to spawnpoint after empty agrolist
	
	if(getNextTarget() == NULL || getNextTarget() != getMostHated() )
	{
		setNextTarget( getMostHated() );
		if( getNextTarget() == NULL )
		{
			//If no target, then check for instance.
			if(isInInstance() )
				setNextTarget(findTarget() );
			//If we still don't have one, then go out of combat.
			if(getNextTarget() == NULL)
			{
				HandleEvent( EVENT_LEAVECOMBAT, m_Self, 0 );
				return;
			}
		}
	}
	else
	{
		/*	Validate our current target.
		Clear it if :
			- not in world
			- isn't alive.
			- can't see it(different phases)
			- not in the same instance id
		*/
		if(!nextTarget->IsInWorld() || !nextTarget->isAlive() || !(nextTarget->m_phase & m_Self->m_phase) 
			|| nextTarget->event_GetInstanceID() != m_Self->event_GetInstanceID() 
			|| !TO_CREATURE(m_Self)->CanSee(nextTarget) )
		{
			setNextTarget(NULL);
			return;
		}
	}
	if (sWorld.Collision) 
	{
		if(getNextTarget() != NULL )
		{
			if(!isFlying() )
			{
				float target_land_z = CollideInterface.GetHeight(m_Self->GetMapId(), getNextTarget()->GetPositionX(), getNextTarget()->GetPositionY(), getNextTarget()->GetPositionZ() + 2.0f);
				if ( target_land_z == NO_WMO_HEIGHT )
					target_land_z = m_Self->GetMapMgr()->GetLandHeight(getNextTarget()->GetPositionX(), getNextTarget()->GetPositionY());

				if (fabs(getNextTarget()->GetPositionZ() - target_land_z) > calcCombatRange(getNextTarget(), false))
				{
					if ( getNextTarget()->GetTypeId() != TYPEID_PLAYER )
					{
						if ( target_land_z > m_Self->GetMapMgr()->GetWaterHeight(getNextTarget()->GetPositionX(), getNextTarget()->GetPositionY() ) )
						{
							HandleEvent( EVENT_LEAVECOMBAT, m_Self, 0); //bugged npcs, probably db fault
							return;
						}
					}
					else if (TO_PLAYER(getNextTarget())->GetSession() != NULL)
					{
						MovementInfo * mi = TO_PLAYER(getNextTarget())->GetSession()->GetMovementInfo();

						if ( mi != NULL && !(mi->flags & MOVEFLAG_FALLING) && !(mi->flags & MOVEFLAG_SWIMMING) && !(mi->flags & MOVEFLAG_LEVITATE) )
						{
							HandleEvent( EVENT_LEAVECOMBAT, m_Self, 0);
							return;
						}
					}
				}
			}
		}
	}
	if(!m_Self->IsCasting() )
	{
		//So we simply, call the agent that is currently processing.
		uint8 procAgent = getProcessingAgent();
		//Also check if we can bypass this agent.
		if(procAgent != AGENT_NULL && hasBehavior(BEHAVIOR_ISPROCESSINGAGENT) )
		{
			switch(procAgent)
			{
			case AGENT_SPELL:
				OnAgentSpell();
				break;
			case AGENT_MELEE:
				OnAgentMelee();
				break;
			case AGENT_RANGED:
				OnAgentRanged();
				break;
			}
		}
		else
		{
			//Check which agents we can use
			bool melee = ( hasMeleeAgent() && !getMeleeDisable() );
			bool ranged = ( hasRangedAgent() && !getRangedDisable() );
			bool spell = ( hasSpellAgent() && !getSpellDisable() && !hasBehavior(BEHAVIOR_BLOCKSPELLSPAMMING) );
			bool panic = hasPanicAgent();
			bool help = hasCallHelpAgent();
			//Panic or Call Help agents have priority over the rest.
			if(panic || help)
			{
				if(panic )
				{
					if(!hasBehavior(BEHAVIOR_PANIC_EFFECT) )
					{
						if(!hasPanicked() )
						{
							uint32 hp = m_Self->GetHealthPct();
							if( hp <= getPanicHp() )
							{
								OnAgentPanic();
								return;
							}
						}
						//Our behavior bit cleared after panic duration, now clear fleeing state.
						else if(getAIState() == STATE_FLEEING)
						{
							HandleEvent(EVENT_UNFEAR,NULL,0);
							return;
						}
					}
				}
				if(help)
				{
					uint32 hp = m_Self->GetHealthPct();
					if(!hasCalledHelp() && hp <= getCallHelpHp() )
					{
						OnAgentCallForHelp();
						return;
					}
				}
			}
			//Spells have top priority.
			updateSpellCooldowns();
			if(spell && m_spellDeck.size() )
				OnAgentSpell();
			else
			{
				// If we have both ranged and melee.
				if(ranged && melee)
				{
					float dist = m_Self->CalcDistance(getNextTarget());
					//If we have any ranged spells, grab that otherwise grab our shoot spell and lets check distance requirements.
					AI_Spell * rangedSpell = (m_rangedSpellDeck.size() && m_rangedSpellDeck.front() != NULL ) ? m_rangedSpellDeck.front() : getShootSpell();
					if(rangedSpell != NULL)
					{
						//If we are close to our target, then simply melee it.
						if(dist < rangedSpell->minrange)
							ranged = false;
					}
					else
					{
						//If we are close to our target, then simply melee it.
						if(dist < 8.0f)
							ranged = false;
					}
					//If we are too far, we still prefer to range it, so we move closer but prefer ranging.
					if(ranged)
						OnAgentRanged();
					else
						OnAgentMelee();
				}
				else
				{
					//We either have melee or ranged but not both.
					if(melee)
						OnAgentMelee();
					else if(ranged)
						OnAgentRanged();
				}
			}
		}
	}
}

void MobAI::AttackReaction(Unit * pUnit, uint32 damage_dealt, uint32 spellId /* = 0 */)
{
	if (pUnit->GetTypeId() == TYPEID_PLAYER && TO_PLAYER(pUnit)->GetMisdirectionTarget() != 0)
	{
		Unit * mTarget = m_Self->GetMapMgr()->GetUnit(TO_PLAYER(pUnit)->GetMisdirectionTarget());
		if (mTarget != NULL && mTarget->isAlive())
			pUnit = mTarget;
	}
	AI_State state = (AI_State)getAIState();

	if( (state == STATE_IDLE || state == STATE_FOLLOWING) && GetAllowedToEnterCombat() )
	{
		wipeHateList();
		HandleEvent(EVENT_ENTERCOMBAT, pUnit, 0);
	}

	HandleEvent(EVENT_DAMAGETAKEN, pUnit, calcThreat(damage_dealt, spellId ? dbcSpell.LookupEntryForced(spellId) : NULL, pUnit));
}

void MobAI::HealReaction(Unit* caster, Unit* victim, SpellEntry* sp, uint32 amount)
{
	if(caster == NULL || victim == NULL) return;

	bool casterInList = false, victimInList = false;

	if(m_aiTargets.find(caster->GetGUID()) != m_aiTargets.end())
		casterInList = true;

	if(m_aiTargets.find(victim->GetGUID()) != m_aiTargets.end())
		victimInList = true;

	if(!victimInList && !casterInList) // none of the Casters is in the Creatures Threat list
		return;

	int32 threat = int32(amount / 2);
	if (caster->getClass() == PALADIN)
		threat = threat / 2; //Paladins only get 50% threat per heal than other classes

	if (sp != NULL)
		threat += (threat * caster->GetGeneratedThreatModifyer(sp->School) / 100);

	if (threat < 1)
		threat = 1;

	if(!casterInList && victimInList) // caster is not yet in Combat but victim is
	{
		// get caster into combat if he's hostile
		if(isHostile(m_Self, caster))
			addNewTarget(victim->GetGUID(), (uint32)threat);
	}
	else if(casterInList && victimInList) // both are in combat already
		modThreatByPtr(caster, threat);

	else // caster is in Combat already but victim is not
	{
		modThreatByPtr(caster, threat);
		// both are players so they might be in the same group
		if( caster->IsPlayer() && victim->IsPlayer() )
		{
			if( TO_PLAYER( caster )->GetGroup() == TO_PLAYER( victim )->GetGroup() )
			{
				// get victim into combat since they are both
				// in the same party
				if( isHostile( m_Self, victim ) )
					addNewTarget(victim->GetGUID(),1);
			}
		}
	}
}
Unit * MobAI::findTarget()
{
	Unit * target = NULL;
	float distance = 999999.99f; //100 yards default.
	std::set<Object*>::iterator itr, itr2;
	std::set< Object* >::iterator pitr, pitr2;
	Unit * pUnit = NULL;
	float dist = 0.0f;
	//target is immune to all form of attacks, cant attack either.
	// not attackable creatures sometimes fight enemies in scripted fights though
	if(m_Self->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NOT_ATTACKABLE_2))
		return NULL;

    // Start of neutralguard snippet
	if( TO_CREATURE(m_Self)->isGuard()  || TO_CREATURE(m_Self)->isNeutralGuard() )
	{
		Player *tmpPlr;
		for (std::set< Object*>::iterator itrPlr = m_Self->GetInRangePlayerSetBegin(); itrPlr != m_Self->GetInRangePlayerSetEnd(); ++itrPlr)
		{
			tmpPlr = TO_PLAYER( *itrPlr );
			//Normal guards attack those who try to pvp.
			if( TO_CREATURE(m_Self)->isGuard() && !tmpPlr->IsPvPFlagged() && !tmpPlr->IsFFAPvPFlagged() )
				continue;

			if (tmpPlr == NULL || tmpPlr->IsDead() || tmpPlr->GetTaxiState() || tmpPlr->m_invisible || !(tmpPlr->m_phase & m_Self->m_phase))
				continue;
			if (tmpPlr->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_FEIGN_DEATH | UNIT_FLAG_NOT_ATTACKABLE_2 | UNIT_FLAG_NOT_ATTACKABLE_9) )
				continue;
			if( !tmpPlr->HasFlag( PLAYER_FLAGS, PLAYER_FLAG_UNKNOWN2 ) )//PvP Guard Attackable.
				continue;

			dist = m_Self->CalcDistance(tmpPlr);
			if (distance > dist)
			{
				if (sWorld.Collision) 
				{
					if( CollideInterface.CheckLOS( m_Self->GetMapId(), m_Self->GetPositionNC(), tmpPlr->GetPositionNC() ) )
					{
						distance = dist;
						target = TO_UNIT(tmpPlr);
					}
				} 
				else 
				{
					distance = dist;
					target = TO_UNIT(tmpPlr);
				}
			}
		}
		if(target)
		{
			m_Self->m_runSpeed = m_Self->m_base_runSpeed * 2.0f;
			AttackReaction(target, 1, 0);

			WorldPacket data(SMSG_AI_REACTION, 12);
			data << m_Self->GetGUID() << uint32(2);		// Aggro sound
			static_cast< Player* >( target )->GetSession()->SendPacket( &data );

			return target;
		}
		distance = 999999.0f; //Reset Distance for normal check
	}
    // End of neutralguard snippet

	if( m_updateTargetsTimer2 < getMSTime() )
	{
		m_updateTargetsTimer2 = getMSTime() + TARGET_UPDATE_INTERVAL_ON_PLAYER;

		for( itr2 = m_Self->GetInRangeOppFactsSetBegin(); itr2 != m_Self->GetInRangeOppFactsSetEnd(); )
		{
			itr = itr2;
			++itr2;

			if( (*itr)->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_FEIGN_DEATH | UNIT_FLAG_NOT_ATTACKABLE_2 | UNIT_FLAG_NOT_ATTACKABLE_9) )
				continue;

			pUnit = TO_UNIT( *itr );

			//on blizz there is no Z limit check 
			dist = m_Self->CalcDistance(pUnit);

			if(dist <= distance && dist <= calcAggroRange(pUnit) )	 // we want to find the CLOSEST target
			{
				if (sWorld.Collision && CollideInterface.CheckLOS(m_Self->GetMapId(), m_Self->GetPositionNC(), pUnit->GetPositionNC() ) )
				{
						distance = dist;
						target = pUnit;
				} 
				else 
				{
					distance = dist;
					target = pUnit;
				}
			}
		}
	}
	if( target )
	{
		AttackReaction(target, 1, 0);
		if(target->IsPlayer())
		{
			WorldPacket data(SMSG_AI_REACTION, 12);
			data << m_Self->GetGUID() << uint32(2);		// Aggro sound
			static_cast< Player* >( target )->GetSession()->SendPacket( &data );
		}
		if(target->GetCreatedByGUID() != 0)
		{
            uint64 charmer = target->GetCharmedByGUID();

			Unit* target2 = m_Self->GetMapMgr()->GetPlayer( Arcemu::Util::GUID_LOPART( charmer ) );

			if(target2)
				AttackReaction(target2, 1, 0);
		}
	}
	return target;
}

bool MobAI::findFriends(float dist)
{
	bool result = false;
	TargetMap::iterator it;

	std::set<Object*>::iterator itr;
	Unit *pUnit = NULL;

	for( itr = m_Self->GetInRangeSetBegin(); itr != m_Self->GetInRangeSetEnd(); itr++ )
	{
        if( !(*itr)->IsUnit() )
			continue;

		pUnit = TO_UNIT(*itr);
		if(!pUnit->isAlive() || !(pUnit->m_phase & m_Self->m_phase) )
			continue;

		if(pUnit->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NOT_ATTACKABLE_2 | UNIT_FLAG_NOT_ATTACKABLE_9) )
			continue;

		if( isCombatSupport(m_Self,pUnit))
		{
			if( m_Self->CalcDistance(pUnit) < dist)
			{
				//To add as assist, check whether he's not in combat.
				if(!pUnit->CombatStatus.IsInCombat() && !m_assistTargets.count( pUnit->GetGUID() ) )
				{
					result = true;
					m_assistTargets.insert(pUnit->GetGUID() );
				}
				//Otherwise we simply add the targets to our threat list.
				lockTargets(true);
				for(it = m_aiTargets.begin(); it != m_aiTargets.end(); ++it)
				{
					Unit *ai_t = m_Self->GetMapMgr()->GetUnit( it->first );
					if( ai_t && pUnit->GetAIInterface() && isHostile(ai_t,pUnit) )
						pUnit->GetAIInterface()->AttackReaction( ai_t, 1, 0 );
				}
				lockTargets(false);
			}
		}
	}
	return result;
}

uint32 MobAI::calcThreat(uint32 damage, SpellEntry * sp, Unit* Attacker)
{
	if(!Attacker)
		return 0; // No attacker means no threat and we prevent crashes this way

	if(m_Self->m_faction != NULL && Attacker->m_faction != NULL)
		if (isSameFaction(m_Self,Attacker))
			return 0;

	int32 mod = 0;
	if( sp != NULL && sp->ThreatForSpell != 0 )
	{
		mod = sp->ThreatForSpell;
	}
	if( sp != NULL && sp->ThreatForSpellCoef != 0.0f )
		mod += int32(damage * sp->ThreatForSpellCoef);
	else
		mod += damage;

	if( sp != NULL && sp->SpellGroupType && Attacker)
	{
		SM_FIValue(Attacker->SM_FThreat,&mod,sp->SpellGroupType);
		SM_PIValue(Attacker->SM_PThreat,&mod,sp->SpellGroupType);
	}

	if (Attacker->getClass() == ROGUE)
		mod = int32(mod * 0.71); // Rogues generate 0.71x threat per damage.

	// modify threat by Buffs
	if (sp != NULL)
		mod += (mod * Attacker->GetGeneratedThreatModifyer(sp->School) / 100);
	else
		mod += (mod * Attacker->GetGeneratedThreatModifyer(0) / 100);

	if (mod < 1)
		mod = 1;

	return mod;
}
void MobAI::handleWaypointMovement()
{

	int32 destpoint = -1;
	uint32 move_type = getWPMoveType();
	if(move_type == MOVEMENTTYPE_RANDOMWP) //is random move on if so move to a random waypoint
	{
		if(getWaypointsCount() > 1)
			destpoint = RandomUInt((uint32)getWaypointsCount());
	}
	else if (move_type == MOVEMENTTYPE_CIRCLEWP) //random move is not on lets follow the path in circles
	{
		// 1 -> 10 then 1 -> 10
		m_currentWaypoint++;
		if (m_currentWaypoint > getWaypointsCount()) m_currentWaypoint = 1; //Happens when you delete last wp seems to continue ticking
		destpoint = m_currentWaypoint;
	}
	else if(move_type == MOVEMENTTYPE_WANTEDWP)//Move to wanted wp
	{
		if(m_currentWaypoint)
		{
			if(getWaypointsCount() > 0)
				destpoint = m_currentWaypoint;
			else
				destpoint = -1;
		}
	}
	else if(move_type == MOVEMENTTYPE_FORWARDTHENSTOP)// move to end, then stop
	{
		++m_currentWaypoint;
		if(m_currentWaypoint > getWaypointsCount())
			destpoint = -1; //hmm maybe we should stop being path walker since we are waiting here anyway
		else
			destpoint = m_currentWaypoint;
	}
	else if(move_type != MOVEMENTTYPE_DONTMOVEWP)//4 Unused
	{
		// 1 -> 10 then 10 -> 1
		if (m_currentWaypoint > getWaypointsCount()) m_currentWaypoint = 1; //Happens when you delete last wp seems to continue ticking
		if (m_currentWaypoint == getWaypointsCount()) // Are we on the last waypoint? if so walk back
			setWpMoveBackWards();
		if (m_currentWaypoint == 1) // Are we on the first waypoint? if so lets goto the second waypoint
			setWpMoveBackWards(false);
		if (isWpMovingBackWards() )// going n..1 
			destpoint = --m_currentWaypoint;
		else				// going 1..n
			destpoint = ++m_currentWaypoint;
			
	}

	if(destpoint != -1)
	{
		WayPoint* wp = getWaypoint( (uint8)destpoint);
		if(wp)
		{
			if(!isWpMovingBackWards() && wp->forwardskinid && getUnit()->GetDisplayId() != wp->forwardskinid)
			{
				getUnit()->SetDisplayId(wp->forwardskinid);
				getUnit()->EventModelChange();
			}
			else if((wp->backwardskinid) && getUnit()->GetDisplayId() != wp->backwardskinid)
			{
				getUnit()->SetDisplayId(wp->backwardskinid);
				getUnit()->EventModelChange();
			}
			if(wp->flags == 0x100)
				m_moveMode = AIMOVESTATE_RUN;
			else if(wp->flags == 0x200)
				m_moveMode = AIMOVESTATE_SPRINT;
			else if(wp->flags == 0x300)
				m_moveMode = AIMOVESTATE_FLY;
			else
				m_moveMode = AIMOVESTATE_WALK;

			MoveTo(wp->x, wp->y, wp->z, wp->o);
		}
	}
}

void MobAI::OnReachWp()
{
	if(getWPMoveType() == MOVEMENTTYPE_WANTEDWP)//We reached wanted wp stop now
		setWPMoveType(MOVEMENTTYPE_DONTMOVEWP);
	WayPoint* wp = getWaypoint(getCurrentWaypoint());
	if(wp)
	{
		CALL_SCRIPT_EVENT(m_Self, OnReachWP)(wp->id, !isWpMovingBackWards());
		if( TO_CREATURE(m_Self)->has_waypoint_text)
			objmgr.HandleMonsterSayEvent(TO_CREATURE(m_Self), MONSTER_SAY_EVENT_RANDOM_WAYPOINT);

		//Lets face to correct orientation
		//float wayO = wp->o;
		m_moveTimer = (int32)wp->waittime; //wait before next move
		if(!isWpMovingBackWards() )
		{
			if(wp->forwardemoteoneshot)
				getUnit()->Emote(EmoteType(wp->forwardemoteid));
			else if(getUnit()->GetEmoteState() != wp->forwardemoteid)
				getUnit()->SetEmoteState(wp->forwardemoteid);
		}
		else
		{
			if(wp->backwardemoteoneshot)
				getUnit()->Emote(EmoteType(wp->backwardemoteid));
			else
				getUnit()->SetEmoteState(wp->backwardemoteid);
		}
	}
	else
		m_moveTimer = RandomUInt(m_moveMode == AIMOVESTATE_RUN ? 5000 : 10000); // wait before next move
}
void MobAI::OnAgentMelee()
{
	Unit * target = getNextTarget();
	float distance = m_Self->CalcDistance(target);
	float combatReach[2];
	combatReach[0] = m_Self->GetModelHalfSize();
	combatReach[1] = calcCombatRange(target, false);
	uint32 modelSize = uint32(combatReach[0]) << 1;
	float tX = target->GetPositionX();
	float tY = target->GetPositionY();
	if( ( distance < combatReach[0] || distance > combatReach[1] ) && 
		( abs(tX - m_prevTargetLocation.x) > modelSize || abs(tY - m_prevTargetLocation.y) > modelSize) )
	{
		/*	Check whether the displacement btwn the targets previous coordinates and their current coords. 
			If it's changed to more than their model size then we send a movement packet so that it's visually aligned */
		if(distance < combatReach[0] )
			calcDestinationAndMove(target, (distance + combatReach[0] ) );
		else
			calcDestinationAndMove(target, (distance - combatReach[1] ) );
		m_prevTargetLocation.x = tX;
		m_prevTargetLocation.y = tY;
			//m_prevTargetLocation.z = tZ;
		if(getProcessingAgent() != AGENT_MELEE)
		{
			setProcessingAgent(AGENT_MELEE);
			setBehavior(BEHAVIOR_ISPROCESSINGAGENT);
			//Don't try for more than 1 secs to meet positioning requirements.
			eventClearBehavior(BEHAVIOR_ISPROCESSINGAGENT,3000);
		}
	}
	else
	{
		if(!m_Self->isInFront(target) )
			m_Self->setInFront(target);

		//Base-Hand support
		if( !m_Self->IsStunned() )
		{
			//First let's check for any melee spells we currently have.
			if(m_meleeSpellDeck.size() )
			{
				AI_Spell * meleeSpell = m_meleeSpellDeck.front();
				Unit * target = getSpellTarget(meleeSpell);
				handleSpellCast(meleeSpell,target);
				m_meleeSpellDeck.pop_front();
				if(meleeSpell->rdata != NULL)
				{
					uint32 range1 = meleeSpell->rdata->interval[0];
					uint32 range2 = meleeSpell->rdata->interval[1];
					if(range1 > 0 && range2 > 0 && range1 < range2)
					{
						//Lets deal in seconds.
						if(range1 >= 1000)
							range1 /= 1000;
						if(range2 >= 1000)
							range2 /= 1000;
						uint32 dT = range2-range1;
						//Get a random between (range1-range2)
						uint32 newCd = rand() % dT + range1;
						newCd *= 1000;
						meleeSpell->cooldown = getMSTime() + newCd;
					}
				}
				else
					meleeSpell->cooldown = getMSTime() + meleeSpell->interval;
			}
			//Otherwise, simply melee.
			else if(m_Self->isReadyToAttack() )
			{
				m_Self->setAttackTimer();
				m_Self->Strike(target, MELEE, NULL, 0, 0, 0, false, false );						
			}
			//Off-hand support
			/*else if(m_Self->isReadyToAttack(true) )
			{
				m_Self->setAttackTimer(0,true);
				m_Self->Strike(target, OFFHAND, NULL, 0, 0, 0, false, false);
			}*/
			setProcessingAgent(AGENT_NULL);
		}
	}
}
void MobAI::OnAgentRanged()
{
	Unit * target = getNextTarget();
	float distance = m_Self->CalcDistance(target);
	float combatReach[2] = {8.0f,30.0f };
	AI_Spell * nSpell = NULL;
	bool spellFromDeck = false;
	uint32 modelSize = uint32(target->GetModelHalfSize() ) << 1;
	float tX = target->GetPositionX();
	float tY = target->GetPositionY();

	if(m_rangedSpellDeck.size() )
	{
		nSpell = m_rangedSpellDeck.front();
		spellFromDeck = true;
	}
	else
		nSpell = m_ShootSpell;
	if(nSpell != NULL)
	{
		combatReach[0] = nSpell->minrange;
		combatReach[1] = nSpell->maxrange;
	}
	if( (distance >= combatReach[0] && distance <= combatReach[1] ) // Target is in Range -> Attack
		&& ( abs(tX - m_prevTargetLocation.x) <= modelSize && abs(tY - m_prevTargetLocation.y) <= modelSize ) )
	{
		//Make sure we are facing towards the target.
		if(!m_Self->isInFront(target) )
			m_Self->setInFront(target);
		if(m_Self->isReadyToAttack(false,true) && !m_Self->IsStunned() )
		{
			m_Self->setAttackTimer(0, false,true);
			// Cast our ranged shot spell, otherwise generic
			if(nSpell != NULL)
				m_Self->CastSpell(target,m_ShootSpell->sEntry,false);
			else
				m_Self->CastSpell(target,dbcSpell.LookupEntryForced(SPELL_RANGED_GENERAL),false);
			//Remove it from our deque.
			if(spellFromDeck)
			{
				//Also process the cooldown.
				if( nSpell->maxcastcount != -1 )
					nSpell->currentcastcount++;
				if(nSpell->rdata != NULL)
				{
					uint32 range1 = nSpell->rdata->interval[0];
					uint32 range2 = nSpell->rdata->interval[1];
					if(range1 > 0 && range2 > 0 && range1 < range2)
					{
						//Lets deal in seconds.
						if(range1 >= 1000)
							range1 /= 1000;
						if(range2 >= 1000)
							range2 /= 1000;
						uint32 dT = range2-range1;
						//Get a random between (range1-range2)
						uint32 newCd = rand() % dT + range1;
						newCd *= 1000;
						nSpell->cooldown = getMSTime() + newCd;
					}
				}
				else
					nSpell->cooldown = getMSTime() + nSpell->interval;
				m_rangedSpellDeck.pop_front();
			}
		}
		setProcessingAgent(AGENT_NULL);
	}
	else// Target out of Range -> Run to it
	{
		//calculate next move
		/* only move if target position demands it */
		if(distance < combatReach[0])// Target is too near
			calcDestinationAndMove(target, (distance + combatReach[0]) );
		else
			calcDestinationAndMove(target, (distance - combatReach[1]) );
		m_prevTargetLocation.x = tX;
		m_prevTargetLocation.y = tY;
		if(getProcessingAgent() != AGENT_RANGED)
		{
			setProcessingAgent(AGENT_RANGED);
			setBehavior(BEHAVIOR_ISPROCESSINGAGENT);
			//Don't try for more than 1 secs to meet positioning requirements.
			eventClearBehavior(BEHAVIOR_ISPROCESSINGAGENT,1000);
		}
	}
}
void MobAI::OnAgentSpell()
{
	//Grab the top spell.
	AI_Spell * nSpell = m_spellDeck.front();
	Unit * target = getSpellTarget(nSpell);
	uint32 modelSize = uint32(target->GetModelHalfSize() ) << 1;
	float tX = target->GetPositionX();
	float tY = target->GetPositionY();
	if(target == NULL) return;

	SpellCastTime * sd = dbcSpellCastTime.LookupEntry(nSpell->sEntry->CastingTimeIndex);

	float distance = m_Self->CalcDistance(target);
	bool los = true;
	if (sWorld.Collision)
		los = CollideInterface.CheckLOS(m_Self->GetMapId(), m_Self->GetPositionNC(),getNextTarget()->GetPositionNC());

	if(los && distance <= nSpell->maxrange && distance >= nSpell->minrange &&
		( abs(tX - m_prevTargetLocation.x) <= modelSize && abs(tY - m_prevTargetLocation.y) <= modelSize ) )
	{

		/* if in range stop moving so we don't interrupt the spell */
		//do not stop for instant spells
		if(sd && GetCastTime(sd) != 0) 
			StopMovement(0);

		handleSpellCast(nSpell,target);
		//Don't bother w/ cast count for spells that are unlimited.
		if(nSpell->maxcastcount != -1)
			++nSpell->currentcastcount;
		//Handle random intervals.
		if(nSpell->rdata != NULL)
		{
			uint32 range1 = nSpell->rdata->interval[0];
			uint32 range2 = nSpell->rdata->interval[1];
			if(range1 > 0 && range2 > 0 && range1 < range2)
			{
				//Lets deal in seconds.
				if(range1 >= 1000)
					range1 /= 1000;
				if(range2 >= 1000)
					range2 /= 1000;
				uint32 dT = range2-range1;
				//Get a random between (range1-range2)
				uint32 newCd = rand() % dT + range1;
				newCd *= 1000;
				nSpell->cooldown = getMSTime() + newCd;
			}
		}
		else
			nSpell->cooldown = getMSTime() + nSpell->interval;
		//Remove the top spell :)
		m_spellDeck.pop_front();
		//We've finished all pending actions of this agent.
		setProcessingAgent(AGENT_NULL);
		//Only block if we aren't blocking to begin with, this way we don't reset previously set blocks.
		if(!hasBehavior(BEHAVIOR_BLOCKSPELLSPAMMING) )
		{
			setBehavior(BEHAVIOR_BLOCKSPELLSPAMMING);
			eventClearBehavior(BEHAVIOR_BLOCKSPELLSPAMMING,1000);
		}
	}
	else if(getProcessingAgent() != AGENT_SPELL) // Target out of Range -> Run to it
	{
		if( distance > nSpell->maxrange )
			calcDestinationAndMove(target, (distance - nSpell->maxrange) );
		else if( distance < nSpell->minrange )
			calcDestinationAndMove(target, (distance + nSpell->minrange) );
		m_prevTargetLocation.x = tX;
		m_prevTargetLocation.y = tY;
		if(getProcessingAgent() != AGENT_SPELL)
		{
			//Since we want our unit move without interrupting the agent, we set our currently processing agent.
			setProcessingAgent(AGENT_SPELL);
			setBehavior(BEHAVIOR_ISPROCESSINGAGENT);
			//Don't try for more than 1 secs to meet positioning requirements.
			eventClearBehavior(BEHAVIOR_ISPROCESSINGAGENT,1000);
		}
	}
}
void MobAI::OnAgentCallForHelp()
{
	//check if we have called.
	if(hasCalledHelp() ) return;
	//find assists within 8 yards
	findFriends(8.0f);
	setHasCalledHelp();
	//monster says.
	if(m_Self->IsCreature() )
		objmgr.HandleMonsterSayEvent( TO_CREATURE(m_Self), MONSTER_SAY_EVENT_CALL_HELP );
	CALL_SCRIPT_EVENT( m_Self, OnCallForHelp )();

	uint32 family = TO_CREATURE(m_Self)->GetCreatureInfo() ? TO_CREATURE(m_Self)->GetCreatureInfo()->Type : 0;
	CreatureProto *pt = TO_CREATURE(m_Self)->GetProto();

	uint32 summonguard = 0;

	if( pt != NULL )
		summonguard = pt->summonguard;

	if( family == UNIT_TYPE_HUMANOID && (summonguard > 0) && !IS_INSTANCE(m_Self->GetMapId() ) && hasBehavior(BEHAVIOR_CANSUMMONGUARDS) )
	{
		clearBehavior(BEHAVIOR_CANSUMMONGUARDS);
		//Allow spawning of guards in 15 secs
		eventSetBehavior(BEHAVIOR_CANSUMMONGUARDS,15000);
		uint16 AreaId = m_Self->GetMapMgr()->GetAreaID(m_Self->GetPositionX(),m_Self->GetPositionY());
		AreaTable * at = dbcArea.LookupEntryForced(AreaId);
		if(!at) return;

		ZoneGuardEntry * zoneSpawn;
		if( at->ZoneId != 0 )
			zoneSpawn = ZoneGuardStorage.LookupEntry( at->ZoneId );
		else
			zoneSpawn = ZoneGuardStorage.LookupEntry( at->AreaId );

		if(!zoneSpawn) return;

		uint32 team = 1; // horde default
		if(isAlliance(m_Self))
			team = 0;

		uint32 guardid = zoneSpawn->AllianceEntry;
		if(team == 1) guardid = zoneSpawn->HordeEntry;
		if(!guardid) return;

		CreatureInfo * ci = CreatureNameStorage.LookupEntry(guardid);
		if(!ci)
			return;

		float x = m_Self->GetPositionX() + (float)( (float)(rand() % 150 + 100) / 1000.0f );
		float y = m_Self->GetPositionY() + (float)( (float)(rand() % 150 + 100) / 1000.0f );
		float z;

		if (sWorld.Collision) {
			z = CollideInterface.GetHeight(m_Self->GetMapId(), x, y, m_Self->GetPositionZ() + 2.0f);
			if( z == NO_WMO_HEIGHT )
				z = m_Self->GetMapMgr()->GetLandHeight(x, y);

			if( fabs( z - m_Self->GetPositionZ() ) > 10.0f )
				z = m_Self->GetPositionZ();
		} else {
			z = m_Self->GetPositionZ();
			float adt_z = m_Self->GetMapMgr()->GetLandHeight(x, y);
			if(fabs(z - adt_z) < 3)
				z = adt_z;
		}

		CreatureProto * cp = CreatureProtoStorage.LookupEntry(guardid);
		if(!cp) return;

		uint8 spawned = 0;

		std::set< Object* >::iterator hostileItr = m_Self->GetInRangePlayerSetBegin();
		Player * p = NULL;
		for(; hostileItr != m_Self->GetInRangePlayerSetEnd(); hostileItr++)
		{
			p = TO_PLAYER(*hostileItr);
			if(spawned >= 3)
				break;

			if(!isHostile( p, m_Self))
				continue;

			if (!spawned)
			{
				uint32 languageid = (team == 0) ? LANG_COMMON : LANG_ORCISH;
				m_Self->SendChatMessage(CHAT_MSG_MONSTER_SAY, languageid, "Guards!");
			}

			Creature * guard = m_Self->GetMapMgr()->CreateCreature(guardid);
			guard->Load(cp, x, y, z);
			guard->SetInstanceID(m_Self->GetInstanceID());
			guard->SetZoneId(m_Self->GetZoneId());
			guard->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP); /* shitty DBs */
			guard->m_noRespawn=true;

			if(guard->CanAddToWorld())
			{
				uint32 t = RandomUInt(8)*1000;
				if(t== 0)
					guard->PushToWorld(m_Self->GetMapMgr());
				else
					sEventMgr.AddEvent(guard,&Creature::AddToWorld, m_Self->GetMapMgr(), EVENT_UNK, t, 1, 0);
			}
			else
			{
				guard->DeleteMe();
				return;
			}

			sEventMgr.AddEvent(guard, &Creature::SetGuardWaypoints, EVENT_UNK, 10000, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			sEventMgr.AddEvent(guard, &Creature::DeleteMe, EVENT_CREATURE_SAFE_DELETE, 60*5*1000, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			spawned++;
		}
	}
}
void MobAI::OnAgentPanic()
{
	//Similar event_fear but we don't call that event.
	setAIMoveFlags(AIMOVESTATE_RUN);
	CALL_SCRIPT_EVENT(m_Self, OnFlee)(getNextTarget());
	setAIState(STATE_FLEEING);
	setNextTarget(NULL);  
	setHasPanicked();
	disableSpells();
	//After executing this, updates automatically make it move.
	StopMovement(0);
	uint32 fleeDuration = getPanicDuration() * 1000;
	setBehavior(BEHAVIOR_PANIC_EFFECT);
	//Clear the behavior after panic duration ends.
	eventClearBehavior(BEHAVIOR_PANIC_EFFECT,fleeDuration);
}
Unit * MobAI::getNextTarget()
{
	Unit * ret = NULL;
	if(m_ForcedTarget)
		ret = m_Self->GetMapMgr()->GetUnit(m_ForcedTarget);
	if(ret == NULL && m_nextTarget)
		ret = m_Self->GetMapMgr()->GetUnit(m_nextTarget);
	return ret;
}
void MobAI::setNextTarget(Unit * target)
{
	if(target == NULL)
		m_nextTarget = 0;
	else
		m_nextTarget = target->GetGUID();
}
Unit * MobAI::getMostHated()
{
	return m_Self->GetMapMgr()->GetUnit(m_mostHated);
}
Unit * MobAI::getSecondHated()
{
	return m_Self->GetMapMgr()->GetUnit(m_secondHated);
}
uint32 MobAI::getThreatByGUID(uint64 guid)
{
	uint32 threat = 0;
	TargetMap::iterator itr = m_aiTargets.find(guid);
	if(itr != m_aiTargets.end() )
		threat = itr->second;
	return threat;
}
uint32 MobAI::getThreatByPtr(Unit* obj)
{
	uint64 guid = obj != NULL ? obj->GetGUID() : 0;
	uint32 threat = 0;
	TargetMap::iterator itr = m_aiTargets.find(guid);
	if(itr != m_aiTargets.end() )
		threat = itr->second;
	return threat;
}
bool MobAI::modThreatByGUID(uint64 guid, int32 mod)
{
	bool result = false;
	TargetMap::iterator itr = m_aiTargets.find(guid);
	if(itr != m_aiTargets.end() )
	{
		itr->second += mod;
		result = true;
	}
	return result;
}
bool MobAI::modThreatByPtr(Unit* obj, int32 mod)
{
	bool result = false;
	uint64 guid = obj != NULL ? obj->GetGUID() : 0;
	TargetMap::iterator itr = m_aiTargets.find(guid);
	if(itr != m_aiTargets.end() )
	{
		result = true;
		itr->second += mod;
	}
	return result;
}
void MobAI::removeThreatByGUID(uint64 guid)
{
	m_aiTargets.erase(m_aiTargets.find(guid));
}
void MobAI::removeThreatByPtr(Unit* obj)
{
	uint64 guid = obj != NULL ? obj->GetGUID() : 0;
	m_aiTargets.erase(m_aiTargets.find(guid));
}
void MobAI::addAssistTarget(Unit * nAssist)
{
	if(nAssist != NULL )
	{
		uint64 guid = nAssist->GetGUID();
		if(m_assistTargets.find(guid) == m_assistTargets.end() )
			m_assistTargets.insert(guid);
	}			
}

void MobAI::wipeHateList()
{
	m_aiTargets.clear();
	m_mostHated = 0;
	m_mostHatedThreat = 0;
	m_secondHated = 0;
	m_secondHatedThreat = 0;
	setNextTarget(NULL);
}
void MobAI::EventAiInterfaceParamsetFinish()
{
	if( timed_emotes && timed_emotes->begin() != timed_emotes->end() )
	{
		next_timed_emote = timed_emotes->begin();
		timed_emote_expire = (*next_timed_emote)->expire_after;
	}
}
void MobAI::updateSpellCooldowns()
{
	SpellStore::iterator itr = m_spells.begin();
	AI_Spell * spell = NULL;
	for(; itr != m_spells.end(); ++itr)
	{
		spell = (*itr);
		//Spells that utilize proc counts.
		if(spell->maxcastcount != -1 && spell->currentcastcount >= spell->maxcastcount) continue;
		if(getMSTime() >= spell->cooldown)
		{
			//Queue spells by their categories
			if(spell->spell_agent == AGENT_SPELL)
				m_spellDeck.push_back( spell);
			else if(spell->spell_agent == AGENT_RANGED)
				m_rangedSpellDeck.push_back(spell);
			else if(spell->spell_agent == AGENT_MELEE)
				m_meleeSpellDeck.push_back(spell);
		}
	}
}

bool MobAI::addNewTarget( uint64 guid, uint32 threat )
{
	bool ret = false;
	TargetMap::iterator itr = m_aiTargets.find(guid);
	if(itr == m_aiTargets.end() && threat)
	{
		ret = true;
		std::pair<uint64,int32> element(guid,(int32)threat);
		m_aiTargets.insert(element);
		//set to update our most/second hated.
		if( (int32)threat > m_mostHatedThreat || (int32)threat > m_secondHatedThreat)
			setCanUpdateMostHated();
	}
	return ret;
}

Unit * MobAI::getSpellTarget( AI_Spell* spell)
{
	Unit * target = NULL;
	if(spell->type == AISPELLTYPE_BUFF)
	{
		//For buffs, we choose targets that don't alrdy have the buff.
		switch(spell->spellTargeType)
		{
		case TTYPE_RANDOM_ASSISTTARGET:
			target = getRandomAssistTarget(spell);
			break;
		case TTYPE_RANDOMFRIENDLY:
			target = getRandomFriendly(spell);
			break;
		case TTYPE_FORMATION_HEAD:
			if(formation.formationHead)
				target = m_Self->GetMapMgr()->GetUnit(formation.formationHead);
			//Check if we have the aura already and prevent wasting cds.
			if(target != NULL && target->HasAura(spell->sEntry->Id) )
				//Reset it.
				target = NULL;
			break;
		}
		/* If our targets failed to meet the requirements, fall back on ourselves but only if we don't have the aura.
			Ofcourse this is only done after checking the proper targetting. */
		if(target == NULL && !m_Self->HasAura(spell->sEntry->Id) )
			target = m_Self;
	}
	else if(spell->type == AISPELLTYPE_HEAL || spell->type == AISPELLTYPE_AOE_HEAL)
	{
		AI_HealSpell * hspell = (AI_HealSpell*)spell;
		//Pick targets that meet the heal health requirements.
		switch(hspell->spellTargeType)
		{
		case TTYPE_RANDOM_ASSISTTARGET:
		case TTYPE_RANDOM_ASSISTTARGET_DESTINATION:
			target = getRandomAssistTarget(spell);
			break;
		case TTYPE_RANDOMFRIENDLY:
		case TTYPE_RANDOMFRIENDLY_DESTINATION:
			target = getRandomFriendly(hspell);
			break;
		case TTYPE_FORMATION_HEAD:
		case TTYPE_FORMATION_HEAD_DESTINATION:
			if(formation.formationHead)
				target = m_Self->GetMapMgr()->GetUnit(formation.formationHead);
			if(target != NULL && target->GetHealthPct() > hspell->heal_hp )
				target = NULL;
			break;
		}
		//Fallback to ourself only if we meet the health requirements.
		if(target == NULL && m_Self->GetHealthPct() <= hspell->heal_hp)
			target = m_Self;
	}
	//For dispels, let's check ourselves and friends if we have any negative auras.
	else if(spell->type == AISPELLTYPE_DISPEL )
	{
		uint32 start = MAX_NEGATIVE_AURAS_EXTEDED_START;
		uint32 end = MAX_NEGATIVE_AURAS_EXTEDED_END;
		uint32 dispelType = (uint32)static_cast<AI_DispelSpell*>(spell)->dispel_Type;
		//Find out which effect index contains our dispel type.
		if(dispelType )
		{
			std::list<Unit *> friendlies;
			friendlies.push_back(m_Self);
			AssistTargetSet::iterator itr = m_assistTargets.begin();
			Unit * assist = NULL;
			for(; itr != m_assistTargets.end(); ++itr)
			{
				assist = m_Self->GetMapMgr()->GetUnit( (*itr));
				if( assist != NULL)
					friendlies.push_back(assist);
			}
			bool target_found = false;
			for(std::list<Unit*>::iterator itr = friendlies.begin(); itr != friendlies.end(); ++itr)
			{
				assist = (*itr);
				for(uint32 i = start; i < end; ++i)
				{
					if( (dispelType == DISPEL_ALL && assist->m_auras[i] != NULL) ||
						(dispelType != DISPEL_ALL && assist->m_auras[i] != NULL && assist->m_auras[i]->GetSpellProto()->DispelType == dispelType) )
					{
						target = assist;
						target_found = false;
						break;
					}
				}
			}
			//All our assists and self don't have auras to remove, so we check enemies.
			if(!target_found) 
			{
				Unit * enemy = NULL;
				start = MAX_POSITIVE_AURAS_EXTEDED_START;
				end = MAX_POSITIVE_AURAS_EXTEDED_END;
				for(std::set<Object*>::iterator itr = m_Self->GetInRangeOppFactsSetBegin(); itr != m_Self->GetInRangeOppFactsSetEnd(); ++itr)
				{
					if(!(*itr)->IsUnit() || m_Self->CalcDistance( (*itr) ) > spell->maxrange )
						continue;
					enemy = TO_UNIT(*itr);
					for(uint32 i = start; i < end; ++i)
					{
						if( (dispelType == DISPEL_ALL && enemy->m_auras[i] != NULL) ||
							(dispelType != DISPEL_ALL && enemy->m_auras[i] != NULL && enemy->m_auras[i]->GetSpellProto()->DispelType == dispelType) )
						{
							target = enemy;
							break;
						}
					}
				}
			}

		}
	}
	else if(spell->type == AISPELLTYPE_DEBUFF)
	{
		switch(spell->spellTargeType)
		{
		case TTYPE_RANDOMTARGET:
		case TTYPE_RANDOMTARGET_DESTINATION:
			target = getRandomTarget(spell);
			break;
		case TTYPE_CURRENTTARGET:
		case TTYPE_CURRENTTARGET_DESTINATION:
			target = getNextTarget();
			break;
		}
		//Let's not cast it on the target if he already has the aura.
		if(target->HasAura( spell->sEntry->Id) )
			target = NULL;
	}
	else
	{
		switch(spell->spellTargeType)
		{
		case TTYPE_RANDOM_ASSISTTARGET:
		case TTYPE_RANDOM_ASSISTTARGET_DESTINATION:
			target = getRandomAssistTarget();
			break;
		case TTYPE_RANDOMFRIENDLY:
		case TTYPE_RANDOMFRIENDLY_DESTINATION:
			target = getRandomFriendly();
			break;
		case TTYPE_FORMATION_HEAD:
		case TTYPE_FORMATION_HEAD_DESTINATION:
			if(formation.formationHead)
				target = m_Self->GetMapMgr()->GetUnit(formation.formationHead);
			break;
		case TTYPE_RANDOMTARGET:
		case TTYPE_RANDOMTARGET_DESTINATION:
			target = getRandomTarget(spell);
			break;
		case TTYPE_CURRENTTARGET:
		case TTYPE_CURRENTTARGET_DESTINATION:
			target = getNextTarget();
			break;
		case TTYPE_SELF:
		case TTYPE_SELF_DESTINATION:
			target = m_Self;
			break;
		}
	}
	return target;
}

Unit * MobAI::getRandomAssistTarget(AI_Spell * spell)
{
	Unit * ret = NULL;
	if(m_assistTargets.size() )
	{
		uint32 tIndex = RandomUInt(m_assistTargets.size()-1);
		AssistTargetSet::iterator itr = m_assistTargets.begin();
		for(; tIndex > 0 && itr != m_assistTargets.end(); --tIndex, ++itr )
		{
			ret = m_Self->GetMapMgr()->GetUnit( (*itr) );
			if(ret != NULL )
			{
				if(spell != NULL)
				{
					//Fail conditions.
					if( ( (spell->type == AISPELLTYPE_HEAL  || spell->type == AISPELLTYPE_AOE_HEAL) 
						&& TO_UNIT(ret)->GetHealthPct() > static_cast<AI_HealSpell*>(spell)->heal_hp)
						|| ( spell->type == AISPELLTYPE_BUFF && TO_UNIT(ret)->HasAura(spell->sEntry->Id) ) )
					{
						++tIndex;
						continue;
					}
				}
			}
			else
				//For non existant units, lets not include them when deciding a random one.
				++tIndex;
		}
	}
	return ret;
}

Unit * MobAI::getRandomTarget(AI_Spell * spell)
{
	Unit * ret = NULL;
	if(m_aiTargets.size() )
	{
		std::vector<Unit*> targets;
		std::set<Object*>::iterator itr = m_Self->GetInRangeOppFactsSetBegin();
		for(; itr != m_Self->GetInRangeOppFactsSetEnd(); ++itr)
		{
			if( !(*itr)->IsUnit() || !isAttackable(m_Self,(*itr) ) )
				continue;
			if(spell != NULL && spell->type == AISPELLTYPE_DEBUFF && TO_UNIT(*itr)->HasAura(spell->sEntry->Id) )
					continue;

			targets.push_back( TO_UNIT(*itr));
		}
		ret = targets[RandomUInt(targets.size()-1)];
	}
	return ret;
}

Unit * MobAI::getRandomFriendly(AI_Spell * spell)
{
	Unit * ret = NULL;
	if(m_Self->GetInRangeSameFactsSize() )
	{
		std::vector<Unit*> units;
		for(std::set<Object*>::iterator itr = m_Self->GetInRangeSameFactsSetBegin(); itr != m_Self->GetInRangeSameFactsSetEnd(); ++itr)
		{
			if( !(*itr)->IsUnit() || !isFriendly(m_Self, (*itr) ) ) 
				continue;

			//Fail conditions based on spell types.
			if(spell != NULL)
			{
				if( ( (spell->type == AISPELLTYPE_HEAL || spell->type == AISPELLTYPE_AOE_HEAL)
					&& ( TO_UNIT(*itr)->GetHealthPct() > static_cast<AI_HealSpell*>(spell)->heal_hp) )
					|| ( spell->type == AISPELLTYPE_BUFF && TO_UNIT(*itr)->HasAura(spell->sEntry->Id) ) )
					continue;
			}
			units.push_back( TO_UNIT(*itr) );
		}
		ret = units[RandomUInt(units.size()-1)];
	}
	return ret;
}

void MobAI::setNextSpell( AI_Spell* nspell)
{
	if(nspell != NULL && nspell->sEntry != NULL) 
	{
		std::list<AI_Spell*>::iterator itr = m_spells.begin();
		for(; itr != m_spells.end(); ++itr)
		{
			//Hrmm wonder if this the best way to check for redundancies.
			if( (*itr)->sEntry == nspell->sEntry)
				m_spellDeck.push_back( (*itr));
		}
	}
}
float MobAI::calcDistanceFromHome()
{
	float dist = 0.0f;
	if(m_returnToVector.x > FLOAT_E && m_returnToVector.y > FLOAT_E)
		dist = m_Self->CalcDistance(m_returnToVector.x,m_returnToVector.y,m_returnToVector.z);
	return dist;
}