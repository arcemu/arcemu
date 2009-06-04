/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2009 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "StdAfx.h"

#ifndef UNIX
#include <cmath>
#endif


#ifdef WIN32
#define HACKY_CRASH_FIXES 1		// SEH stuff
#endif

AIInterface::AIInterface()
:
m_waypoints(NULL),
m_canMove(true),
m_destinationX(0),
m_destinationY(0),
m_destinationZ(0),
m_nextPosX(0),
m_nextPosY(0),
m_nextPosZ(0),
UnitToFollow(NULL),
FollowDistance(0.0f),
m_fallowAngle(float(M_PI/2)),
m_timeToMove(0),
m_timeMoved(0),
m_moveTimer(0),
m_WayPointsShowing(false),
m_WayPointsShowBackwards(false),
m_currentWaypoint(0),
m_moveBackward(false),
m_moveType(0),
m_moveRun(false),
m_moveSprint(false),
m_moveFly(false),
m_creatureState(STOPPED),
m_canCallForHelp(false),
m_hasCalledForHelp(false),
m_fleeTimer(0),
m_FleeDuration(0),
m_canFlee(false),
m_hasFleed(false),
m_canRangedAttack(false),
m_FleeHealth(0.0f),
m_CallForHelpHealth(0.0f),
m_AIState(STATE_IDLE),

m_updateAssist(false),
m_updateTargets(false),
m_updateAssistTimer(1),
m_updateTargetsTimer(TARGET_UPDATE_INTERVAL_ON_PLAYER),
m_updateTargetsTimer2(0),

m_nextSpell(NULL),
m_nextTarget(0),
totemspell(NULL),
m_Unit(NULL),
m_PetOwner(NULL),
m_aiCurrentAgent(AGENT_NULL),
m_runSpeed(0.0f),
m_flySpeed(0.0f),
UnitToFear(NULL),
m_outOfCombatRange(2500), // Where did u get this value?

tauntedBy(NULL),
isTaunted(false),
soullinkedWith(NULL),
isSoulLinked(false),
m_AllowedToEnterCombat(true),
m_totalMoveTime(0),
m_lastFollowX(0),
m_lastFollowY(0),
m_FearTimer(0),
m_WanderTimer(0),
m_totemspelltime(0),
m_totemspelltimer(0),
m_formationFollowAngle(0.0f),
m_formationFollowDistance(0.0f),
m_formationLinkTarget(0),
m_formationLinkSqlId(0),
m_currentHighestThreat(0),

disable_combat(false),

disable_melee(false),
disable_ranged(false),
disable_spell(false),

disable_targeting(false),

next_spell_time(0),
waiting_for_cooldown(false),
UnitToFollow_backup(NULL),
m_isGuard(false),
m_isNeutralGuard(false),
m_is_in_instance(false),
skip_reset_hp(false),
timed_emotes(NULL),
timed_emote_expire(0xFFFFFFFF),
m_MovementState(MOVEMENTSTATE_STOP)

#ifdef HACKY_SERVER_CLIENT_POS_SYNC
,
moved_for_attack(false)
#endif
{
	m_aiTargets.clear();
	m_assistTargets.clear();
	m_spells.clear();
}

void AIInterface::EventAiInterfaceParamsetFinish()
{
	if( timed_emotes && timed_emotes->begin() != timed_emotes->end() )
	{
		next_timed_emote = timed_emotes->begin();
		timed_emote_expire = (*next_timed_emote)->expire_after;
	}
}

void AIInterface::Init(Unit *un, AIType at, MovementType mt)
{
	ASSERT(at != AITYPE_PET);

	m_AIType = at;
	m_MovementType = mt;

	m_AIState = STATE_IDLE;
	m_MovementState = MOVEMENTSTATE_STOP;

	m_Unit = un;

	m_walkSpeed = m_Unit->m_walkSpeed*0.001f;//move distance per ms time 
	m_runSpeed = m_Unit->m_runSpeed*0.001f;//move distance per ms time 
	m_flySpeed = m_Unit->m_flySpeed * 0.001f;
	/*if(!m_DefaultMeleeSpell)
	{
		m_DefaultMeleeSpell = new AI_Spell;
		m_DefaultMeleeSpell->entryId = 0;
		m_DefaultMeleeSpell->spellType = 0;
		m_DefaultMeleeSpell->agent = AGENT_MELEE;
		m_DefaultSpell = m_DefaultMeleeSpell;
	}*/
	m_sourceX = un->GetPositionX();
	m_sourceY = un->GetPositionY();
	m_sourceZ = un->GetPositionZ();
	m_guardTimer = getMSTime();
}

AIInterface::~AIInterface()
{
	for(list<AI_Spell*>::iterator itr = m_spells.begin(); itr != m_spells.end(); ++itr)
			delete (*itr);
	m_spells.clear();
}

void AIInterface::Init(Unit *un, AIType at, MovementType mt, Unit *owner)
{
	ASSERT(at == AITYPE_PET || at == AITYPE_TOTEM);

	m_AIType = at;
	m_MovementType = mt;

	m_AIState = STATE_IDLE;
	m_MovementState = MOVEMENTSTATE_STOP;

	m_Unit = un;
	m_PetOwner = owner;

	m_walkSpeed = m_Unit->m_walkSpeed*0.001f;//move distance per ms time 
	m_runSpeed = m_Unit->m_runSpeed*0.001f;//move/ms
	m_flySpeed = m_Unit->m_flySpeed*0.001f;
	m_sourceX = un->GetPositionX();
	m_sourceY = un->GetPositionY();
	m_sourceZ = un->GetPositionZ();
}

void AIInterface::HandleEvent(uint32 event, Unit* pUnit, uint32 misc1)
{
	if( m_Unit == NULL ) return;

	if(m_AIState != STATE_EVADE)
	{
		switch(event)
		{
		case EVENT_ENTERCOMBAT:
			{
				if( pUnit == NULL || pUnit->IsDead() || m_Unit->IsDead() ) return;

				// set the target first
				if(pUnit && pUnit->GetInstanceID() == m_Unit->GetInstanceID())
				{
					m_Unit->SetUInt64Value(UNIT_FIELD_TARGET, pUnit->GetGUID());
				}
				/* send the message */
				if( m_Unit->GetTypeId() == TYPEID_UNIT )
				{
					if( static_cast< Creature* >( m_Unit )->has_combat_text )
						objmgr.HandleMonsterSayEvent( static_cast< Creature* >( m_Unit ), MONSTER_SAY_EVENT_ENTER_COMBAT );

					CALL_SCRIPT_EVENT(m_Unit, OnCombatStart)(pUnit);

					if( static_cast< Creature* >( m_Unit )->m_spawn && ( static_cast< Creature* >( m_Unit )->m_spawn->channel_target_go || static_cast< Creature* >( m_Unit )->m_spawn->channel_target_creature))
					{
						m_Unit->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);
						m_Unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
					}
				}

				// Stop the emote - change to fight emote
				m_Unit->SetUInt32Value( UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY1H );
				m_returnX = m_Unit->GetPositionX();
				m_returnY = m_Unit->GetPositionY();
				m_returnZ = m_Unit->GetPositionZ();

				m_moveRun = true; //run to the target

				// dismount if mounted
				m_Unit->SetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID, 0);

				if(m_AIState != STATE_ATTACKING)
					StopMovement(0);

				m_AIState = STATE_ATTACKING;
				if(m_Unit->GetMapMgr() && m_Unit->GetMapMgr()->GetMapInfo() && m_Unit->GetMapMgr()->GetMapInfo()->type == INSTANCE_RAID)
				{
					if(m_Unit->GetTypeId() == TYPEID_UNIT)
					{
						if(static_cast<Creature*>(m_Unit)->GetCreatureInfo() && static_cast<Creature*>(m_Unit)->GetCreatureInfo()->Rank == 3)
						{
							 m_Unit->GetMapMgr()->AddCombatInProgress(m_Unit->GetGUID());
						}
					}
				}
				if (pUnit->IsPlayer() && static_cast<Player*>(pUnit)->InGroup())
				{
					m_Unit->GetAIInterface()->modThreatByPtr(pUnit, 1);
					Group *pGroup = static_cast<Player*>(pUnit)->GetGroup();

					Player *pGroupGuy;
					GroupMembersSet::iterator itr;
					pGroup->Lock();
					for(uint32 i = 0; i < pGroup->GetSubGroupCount(); i++) {
						for(itr = pGroup->GetSubGroup(i)->GetGroupMembersBegin(); itr != pGroup->GetSubGroup(i)->GetGroupMembersEnd(); ++itr)
						{
							pGroupGuy = (*itr)->m_loggedInPlayer;
							if( pGroupGuy && pGroupGuy->isAlive() && m_Unit->GetMapMgr() == pGroupGuy->GetMapMgr() && pGroupGuy->GetDistanceSq(pUnit) <= 40*40) //50 yards for now. lets see if it works
							{
								m_Unit->GetAIInterface()->AttackReaction(pGroupGuy, 1, 0);
							}
						}
					}
					pGroup->Unlock();
				}
				//Zack : Put mob into combat animation. Take out weapons and start to look serious :P
				m_Unit->smsg_AttackStart( pUnit );
			}break;
		case EVENT_LEAVECOMBAT:
			{
				if( pUnit == NULL ) return;	
				
				if( pUnit->IsCreature() )
				{
					if( pUnit->IsDead() )
						pUnit->RemoveAllAuras();
					else
						pUnit->RemoveNegativeAuras();
				}

				Unit* target = NULL;
				if (m_Unit->GetMapMgr() && m_Unit->GetMapMgr()->GetMapInfo())
				{
					switch (m_Unit->GetMapMgr()->GetMapInfo()->type)
					{
					case INSTANCE_NULL:
					case INSTANCE_BATTLEGROUND:
						if (m_outOfCombatRange && _CalcDistanceFromHome() < m_outOfCombatRange)
							target = FindTarget();
						break;

					case INSTANCE_RAID:
					case INSTANCE_NONRAID:
					case INSTANCE_ARENA:
						target = FindTarget();
						break;
					}

					if(target != NULL)
					{
						AttackReaction(target, 1, 0);
						return;
					}
				}

				//cancel spells that we are casting. Should remove bug where creatures cast a spell after they died
//				CancelSpellCast();
				// restart emote
				if(m_Unit->GetTypeId() == TYPEID_UNIT)
				{
					if( static_cast< Creature* >( m_Unit )->has_combat_text )
						objmgr.HandleMonsterSayEvent( static_cast< Creature* >( m_Unit ), MONSTER_SAY_EVENT_ON_COMBAT_STOP );

					if( static_cast< Creature* >( m_Unit )->original_emotestate )
						m_Unit->SetUInt32Value( UNIT_NPC_EMOTESTATE, static_cast< Creature* >( m_Unit )->original_emotestate );
					else
						m_Unit->SetUInt32Value( UNIT_NPC_EMOTESTATE, 0 );

					if(static_cast<Creature*>(m_Unit)->m_spawn && (static_cast< Creature* >( m_Unit )->m_spawn->channel_target_go || static_cast< Creature* >( m_Unit )->m_spawn->channel_target_creature ) )
					{
						if(static_cast<Creature*>(m_Unit)->m_spawn->channel_target_go)
							sEventMgr.AddEvent( static_cast< Creature* >( m_Unit ), &Creature::ChannelLinkUpGO, static_cast< Creature* >( m_Unit )->m_spawn->channel_target_go, EVENT_CREATURE_CHANNEL_LINKUP, 1000, 5, 0 );

						if(static_cast<Creature*>(m_Unit)->m_spawn->channel_target_creature)
							sEventMgr.AddEvent( static_cast< Creature* >( m_Unit ), &Creature::ChannelLinkUpCreature, static_cast< Creature* >( m_Unit )->m_spawn->channel_target_creature, EVENT_CREATURE_CHANNEL_LINKUP, 1000, 5, 0 );
					}
				}

				//reset ProcCount
				//ResetProcCounts();
				m_moveSprint = true;
				LockAITargets(true);
				m_aiTargets.clear();
				LockAITargets(false);
				m_fleeTimer = 0;
				m_hasFleed = false;
				m_hasCalledForHelp = false;
				m_nextSpell = NULL;
				SetNextTarget( (Unit*)NULL );
				m_Unit->CombatStatus.Vanished();

				if(m_AIType == AITYPE_PET)
				{
					m_AIState = STATE_FOLLOWING;
					UnitToFollow = m_PetOwner;
					FollowDistance = 3.0f;
					m_lastFollowX = m_lastFollowY = 0;
					if( m_Unit->IsPet() )
					{
						static_cast< Pet* >( m_Unit )->SetPetAction( PET_ACTION_FOLLOW );
						if( m_Unit->isAlive() && m_Unit->IsInWorld() )
						{
							static_cast< Pet* >( m_Unit )->HandleAutoCastEvent( AUTOCAST_EVENT_LEAVE_COMBAT );
						}
					}
					HandleEvent(EVENT_FOLLOWOWNER, 0, 0);
				}
				else
				{
					m_AIState = STATE_EVADE;

					Unit* SavedFollow = UnitToFollow;
					UnitToFollow = NULL;
					FollowDistance = 0.0f;
					m_lastFollowX = m_lastFollowY = 0;

					if(m_Unit->isAlive())
					{
						if(m_returnX != 0.0f && m_returnY != 0.0f && m_returnZ != 0.0f)
							MoveTo(m_returnX,m_returnY,m_returnZ,m_Unit->GetSpawnO());
						else
						{
							MoveTo(m_Unit->GetSpawnX(),m_Unit->GetSpawnY(),m_Unit->GetSpawnZ(),m_Unit->GetSpawnO());
							m_returnX=m_Unit->GetSpawnX();
							m_returnY=m_Unit->GetSpawnY();
							m_returnZ=m_Unit->GetSpawnZ();
						}

						Creature *aiowner = static_cast<Creature*>(m_Unit);
						//clear tagger.
						aiowner->Tagged = false;
						aiowner->TaggerGuid = 0;
						aiowner->SetUInt32Value(UNIT_DYNAMIC_FLAGS,aiowner->GetUInt32Value(UNIT_DYNAMIC_FLAGS) & ~(U_DYN_FLAG_TAGGED_BY_OTHER |U_DYN_FLAG_LOOTABLE));
						aiowner->m_lootMethod = -1;
					}
					CALL_SCRIPT_EVENT(m_Unit, OnCombatStop)(SavedFollow);
				}

				if(m_Unit->GetMapMgr() && m_Unit->GetMapMgr()->GetMapInfo() && m_Unit->GetMapMgr()->GetMapInfo()->type == INSTANCE_RAID)
				{
					if(m_Unit->GetTypeId() == TYPEID_UNIT)
					{
						if(static_cast<Creature*>(m_Unit)->GetCreatureInfo() && static_cast<Creature*>(m_Unit)->GetCreatureInfo()->Rank == 3)
						{
							  m_Unit->GetMapMgr()->RemoveCombatInProgress(m_Unit->GetGUID());
						}
					}
				}

				// Remount if mounted
				if(m_Unit->GetTypeId() == TYPEID_UNIT)
				{
					Creature *creature = static_cast< Creature* >( m_Unit );
					if( creature->GetProto() && creature->m_spawn )
						m_Unit->SetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID, creature->m_spawn->MountedDisplayID );
						//m_Unit->SetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID, static_cast< Creature* >( m_Unit )->GetSpawnO->MountedDisplayID );
				}
				//Zack : not sure we need to send this. Did not see it in the dumps since mob died eventually but it seems logical to make this
				m_Unit->smsg_AttackStop( pUnit );
			}break;
		case EVENT_DAMAGETAKEN:
			{
				if( pUnit == NULL ) return;

				if( static_cast< Creature* >( m_Unit )->has_combat_text )
					objmgr.HandleMonsterSayEvent( static_cast< Creature* >( m_Unit ), MONSTER_SAY_EVENT_ON_DAMAGE_TAKEN );

				CALL_SCRIPT_EVENT(m_Unit, OnDamageTaken)(pUnit, float(misc1));
				if(!modThreatByPtr(pUnit, misc1))
				{
					m_aiTargets.insert(TargetMap::value_type(pUnit->GetGUID(), misc1));
				}
				pUnit->CombatStatus.OnDamageDealt( m_Unit );
			}break;
		case EVENT_FOLLOWOWNER:
			{
				m_AIState = STATE_FOLLOWING;
				if(m_Unit->IsPet())
					((Pet*)m_Unit)->SetPetAction(PET_ACTION_FOLLOW);
				UnitToFollow = m_PetOwner;
				m_lastFollowX = m_lastFollowY = 0;
				FollowDistance = 4.0f;

				LockAITargets(true);
				m_aiTargets.clear();
				LockAITargets(false);
				m_fleeTimer = 0;
				m_hasFleed = false;
				m_hasCalledForHelp = false;
				m_nextSpell = NULL;
				SetNextTarget( (Unit*)NULL );
				m_moveRun = true;
			}break;

		case EVENT_FEAR:
			{
				if( pUnit == NULL ) return;

				m_FearTimer = 0;
				SetUnitToFear(pUnit);

				CALL_SCRIPT_EVENT(m_Unit, OnFear)(pUnit, 0);
				m_AIState = STATE_FEAR;
				StopMovement(1);

				UnitToFollow_backup = UnitToFollow;
				UnitToFollow = NULL;
				m_lastFollowX = m_lastFollowY = 0;
				FollowDistance_backup = FollowDistance;
				FollowDistance = 0.0f;

				LockAITargets(true);
				m_aiTargets.clear(); // we'll get a new target after we are unfeared
				LockAITargets(false);
				m_fleeTimer = 0;
				m_hasFleed = false;
				m_hasCalledForHelp = false;

				// update speed
				m_moveRun = true;
				getMoveFlags();

				SetNextSpell( NULL );
				SetNextTarget( (Unit*)NULL);
			}break;

		case EVENT_UNFEAR:
			{
				UnitToFollow = UnitToFollow_backup;
				FollowDistance = FollowDistance_backup;
				m_AIState = STATE_IDLE; // we need this to prevent permanent fear, wander, and other problems

				SetUnitToFear(NULL);
				StopMovement(1);
			}break;

		case EVENT_WANDER:
			{
				if( pUnit == NULL ) return;

				m_WanderTimer = 0;

				//CALL_SCRIPT_EVENT(m_Unit, OnWander)(pUnit, 0); FIX ME
				m_AIState = STATE_WANDER;
				StopMovement(1);

				UnitToFollow_backup = UnitToFollow;
				UnitToFollow = NULL;
				m_lastFollowX = m_lastFollowY = 0;
				FollowDistance_backup = FollowDistance;
				FollowDistance = 0.0f;

				LockAITargets(true);
				m_aiTargets.clear(); // we'll get a new target after we are unwandered
				LockAITargets(false);
				m_fleeTimer = 0;
				m_hasFleed = false;
				m_hasCalledForHelp = false;

				// update speed
				m_moveRun = true;
				getMoveFlags();

				SetNextSpell(NULL);
				SetNextTarget( (Unit*)NULL );
			}break;

		case EVENT_UNWANDER:
			{
				UnitToFollow = UnitToFollow_backup;
				FollowDistance = FollowDistance_backup;
				m_AIState = STATE_IDLE; // we need this to prevent permanent fear, wander, and other problems

				StopMovement(1);
			}break;

		default:
			{
			}break;
		}
	}

	//Should be able to do this stuff even when evading
	switch(event)
	{
		case EVENT_UNITDIED:
		{
			if( pUnit == NULL ) return;

			if( static_cast< Creature* >( m_Unit )->has_combat_text )
				objmgr.HandleMonsterSayEvent( static_cast< Creature* >( m_Unit ), MONSTER_SAY_EVENT_ON_DIED );

			CALL_SCRIPT_EVENT(m_Unit, OnDied)(pUnit);
			m_AIState = STATE_IDLE;

			StopMovement(0);
			LockAITargets(true);
			m_aiTargets.clear();
			LockAITargets(false);
			UnitToFollow = NULL;
			m_lastFollowX = m_lastFollowY = 0;
			UnitToFear = NULL;
			FollowDistance = 0.0f;
			m_fleeTimer = 0;
			m_hasFleed = false;
			m_hasCalledForHelp = false;
			m_nextSpell = NULL;

			SetNextTarget( (Unit*)NULL );
			//reset ProcCount
			//ResetProcCounts();
		
			//reset waypoint to 0
			m_currentWaypoint = 0;
			
			// There isn't any need to do any attacker checks here, as
			// they should all be taken care of in DealDamage

			//removed by Zack : why do we need to go to our master if we just died ? On next spawn we will be spawned near him after all
/*			if(m_AIType == AITYPE_PET)
			{
				SetUnitToFollow(m_PetOwner);
				SetFollowDistance(3.0f);
				HandleEvent(EVENT_FOLLOWOWNER, m_Unit, 0);
			}*/

			Instance *pInstance = NULL;
			if( m_Unit->GetMapMgr() )
				pInstance = m_Unit->GetMapMgr()->pInstance;

			if(m_Unit->GetMapMgr()
				&& m_Unit->GetTypeId() == TYPEID_UNIT 
				&& !m_Unit->IsPet() 
				&& pInstance 
				&& (pInstance->m_mapInfo->type == INSTANCE_RAID 
				|| pInstance->m_mapInfo->type == INSTANCE_NONRAID 
				|| pInstance->m_mapInfo->type == INSTANCE_ARENA ))
			{
				InstanceBossInfoMap *bossInfoMap = objmgr.m_InstanceBossInfoMap[m_Unit->GetMapMgr()->GetMapId()];
				Creature *pCreature = static_cast< Creature* >( m_Unit );
				bool found = false;

				if(IS_PERSISTENT_INSTANCE(pInstance) && bossInfoMap != NULL && pCreature->GetProto())
				{
					uint32 npcGuid = pCreature->GetProto()->Id;
					InstanceBossInfoMap::const_iterator bossInfo = bossInfoMap->find(npcGuid);
					if(bossInfo != bossInfoMap->end())
					{
						found = true;
						m_Unit->GetMapMgr()->pInstance->m_killedNpcs.insert( npcGuid );
						m_Unit->GetMapMgr()->pInstance->SaveToDB();
						for(InstanceBossTrashList::iterator trash = bossInfo->second->trash.begin(); trash != bossInfo->second->trash.end(); ++trash)
						{
							Creature *c = m_Unit->GetMapMgr()->GetSqlIdCreature((*trash));
							if(c != NULL)
								c->m_noRespawn = true;
						}
						if(!pInstance->m_persistent)
						{
							pInstance->m_persistent = true;
							pInstance->SaveToDB();
							for(PlayerStorageMap::iterator itr = m_Unit->GetMapMgr()->m_PlayerStorage.begin(); itr != m_Unit->GetMapMgr()->m_PlayerStorage.end(); ++itr)
							{
								(*itr).second->SetPersistentInstanceId(pInstance);
							}
						}
					}
				}

				if (found == false) {
					// No instance boss information ... so fallback ...
					uint32 npcGuid = pCreature->GetSQL_id();
					m_Unit->GetMapMgr()->pInstance->m_killedNpcs.insert( npcGuid );
					m_Unit->GetMapMgr()->pInstance->SaveToDB();
				}
			}
			if(m_Unit->GetMapMgr() && m_Unit->GetMapMgr()->GetMapInfo() && m_Unit->GetMapMgr()->GetMapInfo()->type == INSTANCE_RAID)
			{
				if(m_Unit->GetTypeId() == TYPEID_UNIT)
				{
					if(static_cast<Creature*>(m_Unit)->GetCreatureInfo() && static_cast<Creature*>(m_Unit)->GetCreatureInfo()->Rank == 3)
					{
						m_Unit->GetMapMgr()->RemoveCombatInProgress(m_Unit->GetGUID());
					}
				}
			}


			//remove negative auras
			//if( m_Unit->IsCreature() )
			//	m_Unit->RemoveNegativeAuras();

		}break;
	}
}

void AIInterface::Update(uint32 p_time)
{
	float tdist;
	if(m_AIType == AITYPE_TOTEM)
	{
		assert(totemspell != 0);
		if(p_time >= m_totemspelltimer)
		{
			Spell *pSpell = SpellPool.PooledNew();
			if (!pSpell)
				return;
			pSpell->Init(m_Unit, totemspell, true, 0);
			SpellCastTargets targets(0);
			if(!GetNextTarget() ||
				(GetNextTarget() && 
					(!m_Unit->GetMapMgr()->GetUnit(GetNextTarget()->GetGUID()) || 
					!GetNextTarget()->isAlive() ||
					!IsInrange(m_Unit,GetNextTarget(),pSpell->GetProto()->base_range_or_radius_sqr) ||
					!isAttackable(m_Unit, GetNextTarget(),!(pSpell->GetProto()->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED))
					)
				)
				)
			{
				//we set no target and see if we managed to fid a new one
				SetNextTarget( (Unit*)NULL );
				//something happened to our target, pick another one
				pSpell->GenerateTargets(&targets);
				if(targets.m_targetMask & TARGET_FLAG_UNIT)
					SetNextTarget( targets.m_unitTarget );
			}
			if(GetNextTarget())
			{
				SpellCastTargets targets(GetNextTarget()->GetGUID());
				pSpell->prepare(&targets);
				// need proper cooldown time!
				m_totemspelltimer = m_totemspelltime;
			}
			else
			{
				SpellPool.PooledDelete(pSpell);
				pSpell = NULL;
			}
			// these will *almost always* be AoE, so no need to find a target here.
//			SpellCastTargets targets(m_Unit->GetGUID());
//			Spell * pSpell = new Spell(m_Unit, totemspell, true, 0);
//			pSpell->prepare(&targets);
			// need proper cooldown time!
//			m_totemspelltimer = m_totemspelltime;
		}
		else
		{
			m_totemspelltimer -= p_time;
		}
		return;
	}

	_UpdateTimer(p_time);
	_UpdateTargets();
	if(m_Unit->isAlive() && m_AIState != STATE_IDLE 
		&& m_AIState != STATE_FOLLOWING && m_AIState != STATE_FEAR 
		&& m_AIState != STATE_WANDER && m_AIState != STATE_SCRIPTMOVE)
	{
		if(m_AIType == AITYPE_PET )
		{
			if(!m_Unit->bInvincible && m_Unit->IsPet()) 
			{
				Pet * pPet = static_cast<Pet*>(m_Unit);
	
				if(pPet->GetPetAction() == PET_ACTION_ATTACK || pPet->GetPetState() != PET_STATE_PASSIVE)
				{
					_UpdateCombat(p_time);
				}
			}
			//we just use any creature as a pet guardian
			else if(!m_Unit->IsPet())
			{
				_UpdateCombat(p_time);
			}
		}
		else
		{
			_UpdateCombat(p_time);
		}
	}

	_UpdateMovement(p_time);
	if(m_AIState==STATE_EVADE)
	{
		tdist = m_Unit->GetDistanceSq(m_returnX,m_returnY,m_returnZ);
		if(tdist <= 4.0f/*2.0*/)
		{
			m_AIState = STATE_IDLE;
			m_returnX = m_returnY = m_returnZ = 0.0f;
			m_moveRun = false;
			//removed by zack : in scripted events if we keep reducing this it will bug the world out !
			//On Blizz it will return to previous wp but we can accept the fact that it will move on to next one
			/*
			if(hasWaypoints())
			{
				if(m_moveBackward)
				{
					if(m_currentWaypoint != GetWayPointsCount()-1)
						m_currentWaypoint++;
				}
				else
				{
					if(m_currentWaypoint != 0)
						m_currentWaypoint--;
				}
			}
			*/
			// Set health to full if they at there last location before attacking
			if(m_AIType != AITYPE_PET&&!skip_reset_hp)
				m_Unit->SetUInt32Value(UNIT_FIELD_HEALTH,m_Unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH));
		}
		else
		{
			if( m_creatureState == STOPPED )
			{
				// return to the home
				if( m_returnX == 0.0f && m_returnY == 0.0f )
				{
					m_returnX = m_Unit->GetSpawnX();
					m_returnY = m_Unit->GetSpawnY();
					m_returnZ = m_Unit->GetSpawnZ();
				}

				MoveTo(m_returnX, m_returnY, m_returnZ, m_Unit->GetSpawnO());
			}
		}
	}

	if(m_fleeTimer)
	{
		if(m_fleeTimer > p_time)
		{
			m_fleeTimer -= p_time;
			_CalcDestinationAndMove(GetNextTarget(), 5.0f);
		}
		else
		{
			m_fleeTimer = 0;
			SetNextTarget(FindTargetForSpell(m_nextSpell));
		}
	}

	//Pet Dismiss after a certain distance away
	/*if(m_AIType == AITYPE_PET && m_PetOwner != NULL)
	{
		float dist = (m_Unit->GetInstanceID() == m_PetOwner->GetInstanceID()) ? 
			m_Unit->GetDistanceSq(m_PetOwner) : 99999.0f;

		if(dist > 8100.0f) //90 yard away we Dismissed
		{
			DismissPet();
			return;
		}
	}*/

	if ( !GetNextTarget() && !m_fleeTimer && m_creatureState == STOPPED && m_AIState == STATE_IDLE && m_Unit->isAlive() )
	{
		if ( timed_emote_expire <= p_time ) // note that creature might go idle and p_time might get big next time ...We do not skip emotes because of lost time
		{
			if ( (*next_timed_emote)->type == 1) //standstate
			{
				m_Unit->SetStandState( (*next_timed_emote)->value );
				m_Unit->SetUInt32Value ( UNIT_NPC_EMOTESTATE, 0 );
			}
			else if ( (*next_timed_emote)->type == 2) //emotestate
			{
				m_Unit->SetUInt32Value ( UNIT_NPC_EMOTESTATE, (*next_timed_emote)->value );
				m_Unit->SetStandState( 0 );
			}
			else if ( (*next_timed_emote)->type == 3) //oneshot emote
			{
				m_Unit->SetUInt32Value ( UNIT_NPC_EMOTESTATE, 0 );
				m_Unit->SetStandState( 0 );
				m_Unit->Emote( (EmoteType)(*next_timed_emote)->value );         // Animation
			}
			if ( (*next_timed_emote)->msg )
				m_Unit->SendChatMessage((*next_timed_emote)->msg_type, (*next_timed_emote)->msg_lang, (*next_timed_emote)->msg);

			timed_emote_expire = (*next_timed_emote)->expire_after; //should we keep lost time ? I think not 
			next_timed_emote++;
			if ( next_timed_emote == timed_emotes->end() )
				next_timed_emote = timed_emotes->begin();
		}
		else 
			timed_emote_expire -= p_time;
	}
}

void AIInterface::_UpdateTimer(uint32 p_time)
{
	if(m_updateAssistTimer > p_time)
	{
		m_updateAssistTimer -= p_time;
	}else
	{
		m_updateAssist = true;
		m_updateAssistTimer = TARGET_UPDATE_INTERVAL_ON_PLAYER * 2 - m_updateAssistTimer - p_time;
	}

	if(m_updateTargetsTimer > p_time)
	{
		m_updateTargetsTimer -= p_time;
	}else
	{
		m_updateTargets = true;
		m_updateTargetsTimer = TARGET_UPDATE_INTERVAL_ON_PLAYER * 2 - m_updateTargetsTimer - p_time;
	}
}

void AIInterface::_UpdateTargets()
{
	if( m_Unit->IsPlayer() || (m_AIType != AITYPE_PET && disable_targeting ))
		return;
	if( ( ( Creature* )m_Unit )->GetCreatureInfo() && ( ( Creature* )m_Unit )->GetCreatureInfo()->Type == CRITTER )
		return;

	if(  m_Unit->GetMapMgr() == NULL )
		return; 

	AssistTargetSet::iterator i, i2;
	TargetMap::iterator itr, it2;

	// Find new Assist Targets and remove old ones
	if(m_AIState == STATE_FLEEING)
	{
		FindFriends(100.0f/*10.0*/);
	}
	else if(m_AIState != STATE_IDLE && m_AIState != STATE_SCRIPTIDLE)
	{
		FindFriends(64.0f/*8.0f*/);
	}

	if( m_updateAssist )
	{
		m_updateAssist = false;
	/*	deque<Unit*> tokill;

		//modified for vs2005 compatibility
		for(i = m_assistTargets.begin(); i != m_assistTargets.end(); ++i)
		{
			if(m_Unit->GetDistanceSq((*i)) > 2500.0f|| !(*i)->isAlive() || !(*i)->CombatStatus.IsInCombat())
			{
				tokill.push_back(*i);
			}
		}

		for(deque<Unit*>::iterator i2 = tokill.begin(); i2 != tokill.end(); ++i2)
			m_assistTargets.erase(*i2);*/

		for(i = m_assistTargets.begin(); i != m_assistTargets.end();)
		{
			i2 = i++;
			if((*i2) == NULL || (*i2)->event_GetCurrentInstanceId() != m_Unit->event_GetCurrentInstanceId() ||
				!(*i2)->isAlive() || m_Unit->GetDistanceSq((*i2)) >= 2500.0f || !(*i2)->CombatStatus.IsInCombat() )
			{
				m_assistTargets.erase( i2 );
			}
		}
	}

	if( m_updateTargets )
	{
		m_updateTargets = false;
		/*deque<Unit*> tokill;

		//modified for vs2005 compatibility
		for(itr = m_aiTargets.begin(); itr != m_aiTargets.end();++itr)
		{
			if(!itr->first->isAlive() || m_Unit->GetDistanceSq(itr->first) >= 6400.0f)
			{
				tokill.push_back(itr->first);
			}
		}
		for(deque<Unit*>::iterator itr = tokill.begin(); itr != tokill.end(); ++itr)
			m_aiTargets.erase((*itr));
		tokill.clear();*/

		LockAITargets(true);

		for(itr = m_aiTargets.begin(); itr != m_aiTargets.end();)
		{
			it2 = itr++;

			Unit *ai_t = m_Unit->GetMapMgr()->GetUnit( it2->first );
			if (ai_t == NULL) {
				m_aiTargets.erase( it2 );
			} else {
				bool instance = false;
				if (m_Unit->GetMapMgr() && m_Unit->GetMapMgr()->GetMapInfo())
				{
					switch (m_Unit->GetMapMgr()->GetMapInfo()->type)
					{
					case INSTANCE_RAID:
					case INSTANCE_NONRAID:
					case INSTANCE_ARENA:
						instance = true;
						break;
					}
				}

				if( ai_t->event_GetCurrentInstanceId() != m_Unit->event_GetCurrentInstanceId() || !ai_t->isAlive() || (!instance && m_Unit->GetDistanceSq(ai_t) >= 6400.0f)) {
					m_aiTargets.erase( it2 );
				}
			}
		}

		LockAITargets(false);
		
		if(m_aiTargets.size() == 0 
			&& m_AIState != STATE_IDLE && m_AIState != STATE_FOLLOWING 
			&& m_AIState != STATE_EVADE && m_AIState != STATE_FEAR 
			&& m_AIState != STATE_WANDER && m_AIState != STATE_SCRIPTIDLE)
		{
			if (m_Unit->GetMapMgr() && m_Unit->GetMapMgr()->GetMapInfo())
			{
				Unit* target = NULL;
				switch (m_Unit->GetMapMgr()->GetMapInfo()->type)
				{
				case INSTANCE_RAID:
				case INSTANCE_NONRAID:
				case INSTANCE_ARENA:
					target = FindTarget();
					break;

				default:
					if (m_outOfCombatRange && _CalcDistanceFromHome() < m_outOfCombatRange)
						target = FindTarget();
					break;
				}

				if(target != NULL)
					AttackReaction(target, 1, 0);
			}
		}
		else if( m_aiTargets.size() == 0 && (m_AIType == AITYPE_PET && (m_Unit->IsPet() && static_cast<Pet*>(m_Unit)->GetPetState() == PET_STATE_AGGRESSIVE) || (!m_Unit->IsPet() && disable_melee == false ) ) )
		{
			 Unit* target = FindTarget();
			 if( target )
			 {
				 AttackReaction(target, 1, 0);
			 }
		}
	}
	// Find new Targets when we are ooc
	if((m_AIState == STATE_IDLE || m_AIState == STATE_SCRIPTIDLE) && m_assistTargets.size() == 0)
	{
		Unit* target = FindTarget();
		if(target)
		{
			AttackReaction(target, 1, 0);
		}
	}
}

///====================================================================
///  Desc: Updates Combat Status of m_Unit
///====================================================================
void AIInterface::_UpdateCombat(uint32 p_time)
{
	if( m_AIType != AITYPE_PET && disable_combat )
		return;

	//just make sure we are not hitting self. This was reported as an exploit.Should never occur anyway
	if( GetNextTarget() == m_Unit )
		SetNextTarget( GetMostHated() );

	uint16 agent = m_aiCurrentAgent;

	// If creature is very far from spawn point return to spawnpoint
	// If at instance don't return -- this is wrong ... instance creatures always returns to spawnpoint, dunno how do you got this idea. 
	// If at instance returns to spawnpoint after empty agrolist
	if(	m_AIType != AITYPE_PET 
		&& m_AIState != STATE_EVADE
		&& m_AIState != STATE_SCRIPTMOVE
		&& !m_is_in_instance
		&& (m_outOfCombatRange && m_Unit->GetDistanceSq(m_returnX,m_returnY,m_returnZ) > m_outOfCombatRange) )
	{
		HandleEvent( EVENT_LEAVECOMBAT, m_Unit, 0 );
	}
	else if( GetNextTarget() == NULL && m_AIState != STATE_FOLLOWING && m_AIState != STATE_SCRIPTMOVE )
	{
//		SetNextTarget(FindTargetForSpell(m_nextSpell));
		if( m_is_in_instance )
			SetNextTarget( FindTarget() );
		else 
			SetNextTarget( GetMostHated() );

		if( GetNextTarget() == NULL )
		{
			HandleEvent( EVENT_LEAVECOMBAT, m_Unit, 0 );
		}
	}

#ifdef HACKY_SERVER_CLIENT_POS_SYNC
	if( moved_for_attack && GetNextTarget() && m_creatureState != MOVING )
	{
		//make sure we reached the exact desired location. 
		// due to combat updates creature might interrupt moving and start attacking and does not get to destination making us get out of range errors
		if(	m_destinationX )
		{
			m_Unit->m_position.x = m_destinationX;
			m_Unit->m_position.y = m_destinationY;
		}
		//send a forced update position to client
		StopMovement(0); 
		//no need to update position until mob moves to nev target
		moved_for_attack = false;
	}
#endif

	if (sWorld.Collision) {
		float target_land_z=0.0f;
		if ( m_Unit->GetMapMgr() != NULL && GetNextTarget() != NULL )
		{
			if (!m_moveFly)
			{
				target_land_z = CollideInterface.GetHeight(m_Unit->GetMapId(), GetNextTarget()->GetPositionX(), GetNextTarget()->GetPositionY(), GetNextTarget()->GetPositionZ() + 2.0f);
				if ( target_land_z == NO_WMO_HEIGHT )
					target_land_z = m_Unit->GetMapMgr()->GetLandHeight(GetNextTarget()->GetPositionX(), GetNextTarget()->GetPositionY());

				if (fabs(GetNextTarget()->GetPositionZ() - target_land_z) > _CalcCombatRange(GetNextTarget(), false))
				{
					if ( GetNextTarget()->GetTypeId() != TYPEID_PLAYER )
					{
						if ( target_land_z > m_Unit->GetMapMgr()->GetWaterHeight(GetNextTarget()->GetPositionX(), GetNextTarget()->GetPositionY()) )
							HandleEvent( EVENT_LEAVECOMBAT, m_Unit, 0); //bugged npcs, probably db fault
					}
					else if (static_cast<Player*>(GetNextTarget())->GetSession() != NULL)
					{
						MovementInfo* mi=static_cast<Player*>(GetNextTarget())->GetSession()->GetMovementInfo();

						if ( mi != NULL && !(mi->flags & MOVEFLAG_FALLING) && !(mi->flags & MOVEFLAG_SWIMMING) && !(mi->flags & MOVEFLAG_LEVITATE))
							HandleEvent( EVENT_LEAVECOMBAT, m_Unit, 0);
					}
				}
			}
		}
	}

	if ( GetNextTarget() != NULL && GetNextTarget()->GetTypeId() == TYPEID_UNIT && m_AIState == STATE_EVADE)
		HandleEvent( EVENT_LEAVECOMBAT, m_Unit, 0);
#ifdef HACKY_CRASH_FIXES
	bool cansee = (GetNextTarget() != NULL) ? CheckCurrentTarget() : NULL;

#else
	bool cansee;
	if(GetNextTarget() && GetNextTarget()->event_GetCurrentInstanceId() == m_Unit->event_GetCurrentInstanceId())
	{
		if( m_Unit->GetTypeId() == TYPEID_UNIT )
			cansee = static_cast< Creature* >( m_Unit )->CanSee( GetNextTarget() );
		else
			cansee = static_cast< Player* >( m_Unit )->CanSee( GetNextTarget() );
	}
	else 
	{
		if( GetNextTarget() )
			SetNextTarget( (Unit*)NULL );			// corrupt pointer

		cansee = false;
	}
#endif
	if( cansee && GetNextTarget() && GetNextTarget()->isAlive() && m_AIState != STATE_EVADE && !m_Unit->IsCasting() )
	{
		if( agent == AGENT_NULL || ( m_AIType == AITYPE_PET && !m_nextSpell ) ) // allow pets autocast
		{
			if( !m_nextSpell )
				m_nextSpell = this->getSpell();

			/*
			if(!m_nextSpell && waiting_for_cooldown)
			{
				// don't start running to the target for melee if we're waiting for a cooldown. 
				return;
			}
			*/

			if(m_canFlee && !m_hasFleed 
				&& ((m_Unit->GetUInt32Value(UNIT_FIELD_HEALTH) / m_Unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH)) < m_FleeHealth ))
				agent = AGENT_FLEE;
			else if(m_canCallForHelp 
				&& !m_hasCalledForHelp 
				/*&& (m_CallForHelpHealth > (m_Unit->GetUInt32Value(UNIT_FIELD_HEALTH) / (m_Unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH) > 0 ? m_Unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH) : 1)))*/)
				agent = AGENT_CALLFORHELP;
			else if(m_nextSpell)
			{
				if(m_nextSpell->agent != AGENT_NULL)
				{
					agent = m_nextSpell->agent;
				}
				else
				{
					agent = AGENT_MELEE;
				}
			}
			else
			{
				agent = AGENT_MELEE;
			}
		}
		if(agent == AGENT_RANGED || agent == AGENT_MELEE)
		{
			if(m_canRangedAttack)
			{
				agent = AGENT_MELEE;
				if(GetNextTarget()->GetTypeId() == TYPEID_PLAYER)
				{
					float dist = m_Unit->GetDistanceSq(GetNextTarget());
					if( static_cast< Player* >( GetNextTarget() )->m_currentMovement == MOVE_ROOT || dist >= 64.0f )
					{
						agent =  AGENT_RANGED;
					}
				}
				else if( GetNextTarget()->m_canMove == false || m_Unit->GetDistanceSq(GetNextTarget()) >= 64.0f )
				{
				   agent = AGENT_RANGED;
				}
			}
			else
			{
				agent = AGENT_MELEE;
			}
		}

		if(this->disable_melee && agent == AGENT_MELEE)
			agent = AGENT_NULL;
		  
		if(this->disable_ranged && agent == AGENT_RANGED)
			agent = AGENT_NULL;

		if(this->disable_spell && agent == AGENT_SPELL)
			agent = AGENT_NULL;

		switch(agent)
		{
		case AGENT_MELEE:
			{
				float combatReach[2]; // Calculate Combat Reach
				float distance = m_Unit->CalcDistance(GetNextTarget());

				combatReach[0] = GetNextTarget()->GetModelHalfSize();
				combatReach[1] = _CalcCombatRange(GetNextTarget(), false);

				if(	(
//					distance >= combatReach[0] &&  //removed by Zack. You can create an exploit with this that creature will never attack
					distance <= combatReach[1] + DISTANCE_TO_SMALL_TO_WALK ) 
//					|| gracefull_hit_on_target == GetNextTarget() 
					) // Target is in Range -> Attack
				{
//					gracefull_hit_on_target = NULL;
					if(UnitToFollow != NULL)
					{
						UnitToFollow = NULL; //we shouldn't be following any one
						m_lastFollowX = m_lastFollowY = 0;
						//m_Unit->setAttackTarget(NULL);  // remove ourselves from any target that might have been followed
					}
					
					FollowDistance = 0.0f;
//					m_moveRun = false;
					//FIX ME: offhand shit
					if(m_Unit->isAttackReady(false) && !m_fleeTimer)
					{
						m_creatureState = ATTACKING;
						bool infront = m_Unit->isInFront(GetNextTarget());

						if(!infront) // set InFront
						{
							//prevent mob from rotating while stunned
							if(!m_Unit->IsStunned ())
							{
								setInFront(GetNextTarget());
								infront = true;
							}							
						}
						if(infront)
						{
							m_Unit->setAttackTimer(0, false);
#ifdef ENABLE_CREATURE_DAZE
							//we require to know if strike was successful. If there was no dmg then target cannot be dazed by it
							Unit *t_unit = GetNextTarget();
							if( !t_unit )
								return; //omg lol, in seconds we lost target. This might be possible due to the Eventrelocated
							uint32 health_before_strike = t_unit->GetUInt32Value(UNIT_FIELD_HEALTH);
#endif
							m_Unit->Strike( GetNextTarget(), ( agent == AGENT_MELEE ? MELEE : RANGED ), NULL, 0, 0, 0, false, false );
#ifdef ENABLE_CREATURE_DAZE
							//now if the target is facing his back to us then we could just cast dazed on him :P
							//as far as i know dazed is casted by most of the creatures but feel free to remove this code if you think otherwise
							if(GetNextTarget() && m_Unit->m_factionDBC &&
								!(m_Unit->m_factionDBC->RepListId == -1 && m_Unit->m_faction->FriendlyMask==0 && m_Unit->m_faction->HostileMask==0) /* neutral creature */
								&& GetNextTarget()->IsPlayer() && !m_Unit->IsPet() && health_before_strike>GetNextTarget()->GetUInt32Value(UNIT_FIELD_HEALTH)
								&& Rand(m_Unit->get_chance_to_daze(GetNextTarget())))
							{
								float our_facing=m_Unit->calcRadAngle(m_Unit->GetPositionX(),m_Unit->GetPositionY(),GetNextTarget()->GetPositionX(),GetNextTarget()->GetPositionY());
								float his_facing=GetNextTarget()->GetOrientation();
								if(fabs(our_facing-his_facing)<CREATURE_DAZE_TRIGGER_ANGLE && !GetNextTarget()->HasAura(CREATURE_SPELL_TO_DAZE))
								{
									SpellEntry *info = dbcSpell.LookupEntry(CREATURE_SPELL_TO_DAZE);
									Spell *sp = SpellPool.PooledNew();
									if (!sp)
										return;
									sp->Init(m_Unit, info, false, NULL);
									SpellCastTargets targets;
									targets.m_unitTarget = GetNextTarget()->GetGUID();
									sp->prepare(&targets);
								}
							}
#endif
						}
					}
				}
				else // Target out of Range -> Run to it
				{
					//calculate next move
//					float dist = combatReach[1]; //this is theoretically right but annoying formula in game
//					float dist = combatReach[1] - m_Unit->GetFloatValue( UNIT_FIELD_COMBATREACH ); //ignore our combat reach, make sure target (player) can reach us first.

					//practical tests show that we really should try to jump on target to get good results :S
					//simply ignore combat reach and move as close as visually not annoying 
					float dist;
					if( m_Unit->GetModelHalfSize() > GetNextTarget()->GetModelHalfSize() )
						dist = m_Unit->GetModelHalfSize(); 
					else 
						dist = GetNextTarget()->GetModelHalfSize();

					//removed by Zack. You can create an exploit with this that creature will never attack
//					if (distance<combatReach[0]) //if we are inside one each other
//						dist = -(combatReach[1] - distance);
//					gracefull_hit_on_target = GetNextTarget(); // this is an exploit where you manage to move the exact speed that mob will reposition itself all the time

					m_moveRun = true;
					_CalcDestinationAndMove(GetNextTarget(), dist);
				}
			}break;
		case AGENT_RANGED:
			{
				float combatReach[2]; // Calculate Combat Reach
				float distance = m_Unit->CalcDistance(GetNextTarget());

				combatReach[0] = 8.0f;
				combatReach[1] = 30.0f;

				if(distance >= combatReach[0] && distance <= combatReach[1]) // Target is in Range -> Attack
				{
					if(UnitToFollow != NULL)
					{
						UnitToFollow = NULL; //we shouldn't be following any one
						m_lastFollowX = m_lastFollowY = 0;
						//m_Unit->setAttackTarget(NULL);  // remove ourselves from any target that might have been followed
					}
					
					FollowDistance = 0.0f;
//					m_moveRun = false;
					//FIX ME: offhand shit
					if(m_Unit->isAttackReady(false) && !m_fleeTimer)
					{
						m_creatureState = ATTACKING;
						bool infront = m_Unit->isInFront(GetNextTarget());

						if(!infront) // set InFront
						{
							//prevent mob from rotating while stunned
							if(!m_Unit->IsStunned ())
							{
								setInFront(GetNextTarget());
								infront = true;
							}							
						}

						if(infront)
						{
							m_Unit->setAttackTimer(0, false);
							SpellEntry *info = dbcSpell.LookupEntry(SPELL_RANGED_GENERAL);
							if(info)
							{
								Spell *sp = SpellPool.PooledNew();
								if (!sp)
									return;
								sp->Init(m_Unit, info, false, NULL);
								SpellCastTargets targets;
								targets.m_unitTarget = GetNextTarget()->GetGUID();
								sp->prepare(&targets);
								//Lets make spell handle this
								//m_Unit->Strike( GetNextTarget(), ( agent == AGENT_MELEE ? MELEE : RANGED ), NULL, 0, 0, 0 );
							}
						}
					}
				}
				else // Target out of Range -> Run to it
				{
					//calculate next move
					float dist;

					if(distance < combatReach[0])// Target is too near
						dist = 9.0f;
					else
						dist = 20.0f;

					m_moveRun = true;
					_CalcDestinationAndMove(GetNextTarget(), dist);
				}
			}break;
		case AGENT_SPELL:
			{
				if(!m_nextSpell || !GetNextTarget())
					return;  // this shouldn't happen

				SpellCastTime *sd = dbcSpellCastTime.LookupEntry(m_nextSpell->spell->CastingTimeIndex);

				float distance = m_Unit->CalcDistance(GetNextTarget());
				bool los = true;

				if (sWorld.Collision) {
					los = CollideInterface.CheckLOS(m_Unit->GetMapId(), m_Unit->GetPositionNC(),GetNextTarget()->GetPositionNC());
				}
				if(los 
					&& ( ( distance <= m_nextSpell->maxrange + m_Unit->GetModelHalfSize() 
//					&& distance >= m_nextSpell->minrange 
					) 
							|| m_nextSpell->maxrange == 0) ) // Target is in Range -> Attack
				{
					SpellEntry* spellInfo = m_nextSpell->spell;

					/* if in range stop moving so we don't interrupt the spell */
					//do not stop for instant spells
					if(sd && GetCastTime(sd) != 0)
						StopMovement(0);

/*					if(m_nextSpell->procCount)
						m_nextSpell->procCount--;*/

					SpellCastTargets targets = setSpellTargets(spellInfo, GetNextTarget());
					uint32 targettype = m_nextSpell->spelltargetType;
					switch(targettype)
					{
					case TTYPE_CASTER:
					case TTYPE_SINGLETARGET:
						{
							CastSpell(m_Unit, spellInfo, targets);
							break;
						}
					case TTYPE_SOURCE:
						{
							m_Unit->CastSpellAoF(targets.m_srcX,targets.m_srcY,targets.m_srcZ, spellInfo, true);
							break;
						}
					case TTYPE_DESTINATION:
						{
							m_Unit->CastSpellAoF(targets.m_destX,targets.m_destY,targets.m_destZ, spellInfo, true);
							break;
						}
					default:
						sLog.outError("AI Agents: Targettype of AI agent spell %u for creature %u not set", spellInfo->Id, static_cast< Creature* >( m_Unit )->GetCreatureInfo()->Id );
					}
					// CastSpell(m_Unit, spellInfo, targets);
					if(m_nextSpell&&m_nextSpell->cooldown)
						m_nextSpell->cooldowntime = getMSTime() + m_nextSpell->cooldown;

					next_spell_time = (uint32)UNIXTIME + MOB_SPELLCAST_GLOBAL_COOLDOWN;

					m_nextSpell = NULL;
				}
				else // Target out of Range -> Run to it
				{
					//calculate next move
					m_moveRun = true;
					float close_to_enemy = 0.0f;
					if( distance > m_nextSpell->maxrange )
						close_to_enemy = m_nextSpell->maxrange - DISTANCE_TO_SMALL_TO_WALK ;
					else if( distance < m_nextSpell->minrange )
						close_to_enemy = m_nextSpell->minrange + DISTANCE_TO_SMALL_TO_WALK ;

					if( close_to_enemy < 0 )
						close_to_enemy = 0;
						
					_CalcDestinationAndMove(GetNextTarget(), close_to_enemy ); //if we make exact movement we will never position perfectly
					/*Destination* dst = _CalcDestination(GetNextTarget(), dist);
					MoveTo(dst->x, dst->y, dst->z,0);
					delete dst;*/
				}
			}break;
		case AGENT_FLEE:
			{
				//float dist = 5.0f;

				m_moveRun = false;
				if(m_fleeTimer == 0)
					m_fleeTimer = m_FleeDuration;

				/*Destination* dst = _CalcDestination(GetNextTarget(), dist);
				MoveTo(dst->x, dst->y, dst->z,0);
				delete dst;*/
				_CalcDestinationAndMove(GetNextTarget(), 5.0f);
				if(!m_hasFleed)
					CALL_SCRIPT_EVENT(m_Unit, OnFlee)(GetNextTarget());

				m_AIState = STATE_FLEEING;
				//removed by Zack : somehow creature starts to attack self. Just making sure it is not this one
//				m_nextTarget = m_Unit;
//				m_Unit->SetUInt64Value(UNIT_FIELD_TARGET, 0);
				SetNextTarget( (Unit*)NULL );

				WorldPacket data( SMSG_MESSAGECHAT, 100 );
				string msg = "%s attempts to run away in fear!";
				data << (uint8)CHAT_MSG_CHANNEL;
				data << (uint32)LANG_UNIVERSAL;
				data << (uint32)( strlen( static_cast< Creature* >( m_Unit )->GetCreatureInfo()->Name ) + 1 );
				data << static_cast< Creature* >( m_Unit )->GetCreatureInfo()->Name;
				data << (uint64)0;
				data << (uint32)(msg.size() + 1);
				data << msg;
				data << uint8(0);

				m_Unit->SendMessageToSet(&data, false);

				//m_Unit->SendChatMessage(CHAT_MSG_MONSTER_EMOTE, LANG_UNIVERSAL, msg);
				//sChatHandler.FillMessageData(&data, CHAT_MSG_MONSTER_EMOTE, LANG_UNIVERSAL, msg, m_Unit->GetGUID());			   
			   
				m_hasFleed = true;
			}break;
		case AGENT_CALLFORHELP:
			{
				FindFriends( 64.0f /*8.0f*/ );
				m_hasCalledForHelp = true; // We only want to call for Help once in a Fight.
				if( m_Unit->GetTypeId() == TYPEID_UNIT )
						objmgr.HandleMonsterSayEvent( static_cast< Creature* >( m_Unit ), MONSTER_SAY_EVENT_CALL_HELP );
				CALL_SCRIPT_EVENT( m_Unit, OnCallForHelp )();
			}break;
		}
	}
	else if( !GetNextTarget() || GetNextTarget()->GetInstanceID() != m_Unit->GetInstanceID() || !GetNextTarget()->isAlive() || !cansee )
	{
		SetNextTarget( (Unit*)NULL );
		// no more target
		//m_Unit->setAttackTarget(NULL);
	}
}

void AIInterface::DismissPet()
{
	/*
	if(m_AIType != AITYPE_PET)
		return;

	if(!m_PetOwner)
		return;
	
	if(m_PetOwner->GetTypeId() != TYPEID_PLAYER)
		return;

	if(m_Unit->GetUInt32Value(UNIT_CREATED_BY_SPELL) == 0)
		static_cast< Player* >( m_PetOwner )->SetFreePetNo(false, (int)m_Unit->GetUInt32Value(UNIT_FIELD_PETNUMBER));
	static_cast< Player* >( m_PetOwner )->SetPet(NULL);
	static_cast< Player* >( m_PetOwner )->SetPetName("");
	
	//FIXME:Check hunter pet or not
	//FIXME:Check enslaved creature
	m_PetOwner->SetUInt64Value(UNIT_FIELD_SUMMON, 0);
	
	WorldPacket data;
	data.Initialize(SMSG_PET_SPELLS);
	data << (uint64)0;
	static_cast< Player* >( m_PetOwner )->GetSession()->SendPacket(&data);
	
	sEventMgr.RemoveEvents(((Creature*)m_Unit));
	if(m_Unit->IsInWorld())
	{
		m_Unit->RemoveFromWorld();
	}
	//setup an event to delete the Creature
	sEventMgr.AddEvent(((Creature*)this->m_Unit), &Creature::DeleteMe, EVENT_DELETE_TIMER, 1, 1);*/
}

void AIInterface::AttackReaction(Unit* pUnit, uint32 damage_dealt, uint32 spellId)
{
	if( m_AIState == STATE_EVADE || !pUnit || !pUnit->isAlive() || m_Unit->IsDead() || m_Unit == pUnit )
		return;

	if( sWorld.Collision && pUnit->IsPlayer() )
	{
		float target_land_z=0.0f;
		if ( m_Unit->GetMapMgr() != NULL )
		{
			if (!m_moveFly)
			{
				target_land_z = CollideInterface.GetHeight(m_Unit->GetMapId(), pUnit->GetPositionX(), pUnit->GetPositionY(), pUnit->GetPositionZ() + 2.0f);
				if ( target_land_z == NO_WMO_HEIGHT )
					target_land_z = m_Unit->GetMapMgr()->GetLandHeight(pUnit->GetPositionX(), pUnit->GetPositionY());

				if (fabs(pUnit->GetPositionZ() - target_land_z) > _CalcCombatRange(pUnit, false) )
				{
					if ( pUnit->GetTypeId()!=TYPEID_PLAYER && target_land_z > m_Unit->GetMapMgr()->GetWaterHeight(pUnit->GetPositionX(), pUnit->GetPositionY()) )
						return;
					else if( static_cast<Player*>(pUnit)->GetSession() != NULL )
					{
						MovementInfo* mi=static_cast<Player*>(pUnit)->GetSession()->GetMovementInfo();

						if ( mi != NULL && !(mi->flags & MOVEFLAG_FALLING) && !(mi->flags & MOVEFLAG_SWIMMING) && !(mi->flags & MOVEFLAG_LEVITATE))
							return;
					}
				}
			}
		}
	}

	if (pUnit->GetTypeId() == TYPEID_PLAYER && static_cast<Player *>(pUnit)->GetMisdirectionTarget() != 0)
	{
		Unit *mTarget = m_Unit->GetMapMgr()->GetUnit(static_cast<Player *>(pUnit)->GetMisdirectionTarget());
		if (mTarget != NULL && mTarget->isAlive())
			pUnit = mTarget;
	}

	if( (m_AIState == STATE_IDLE || m_AIState == STATE_FOLLOWING) && m_Unit->GetAIInterface()->GetAllowedToEnterCombat())
	{
		WipeTargetList();
		
		HandleEvent(EVENT_ENTERCOMBAT, pUnit, 0);
	}

	HandleEvent(EVENT_DAMAGETAKEN, pUnit, _CalcThreat(damage_dealt, spellId ? dbcSpell.LookupEntryForced(spellId) : NULL, pUnit));
}

void AIInterface::HealReaction(Unit* caster, Unit* victim, SpellEntry* sp, uint32 amount)
{
	if(!caster || !victim)
		return;

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
		if(isHostile(m_Unit, caster))
			m_aiTargets.insert(TargetMap::value_type(caster->GetGUID(), threat));
	}
	else if(casterInList && victimInList) // both are in combat already
		modThreatByPtr(caster, threat);

	else // caster is in Combat already but victim is not
	{
		modThreatByPtr(caster, threat);
		// both are players so they might be in the same group
		if( caster->GetTypeId() == TYPEID_PLAYER && victim->GetTypeId() == TYPEID_PLAYER )
		{
			if( static_cast< Player* >( caster )->GetGroup() == static_cast< Player* >( victim )->GetGroup() )
			{
				// get victim into combat since they are both
				// in the same party
				if( isHostile( m_Unit, victim ) )
					m_aiTargets.insert( TargetMap::value_type( victim->GetGUID(), 1 ) );
			}
		}
	}
}

void AIInterface::OnDeath(Object* pKiller)
{
	if(pKiller->GetTypeId() == TYPEID_PLAYER || pKiller->GetTypeId() == TYPEID_UNIT)
		HandleEvent(EVENT_UNITDIED, static_cast<Unit*>(pKiller), 0);
	else
		HandleEvent(EVENT_UNITDIED, m_Unit, 0);
}

//function is designed to make a quick check on target to decide if we can attack it
bool AIInterface::UnsafeCanOwnerAttackUnit(Unit *pUnit)
{
	if( !isHostile(m_Unit,pUnit ) )
		return false;

	if( !pUnit->isAlive() )
		return false;

	if( pUnit->bInvincible )
		return false;

	//do not agro units that are faking death. Should this be based on chance ?
	if( pUnit->HasFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_FEIGN_DEATH ) )
		return false;

	//don't attack owner
	if( m_Unit->GetUInt64Value(UNIT_FIELD_CREATEDBY) == pUnit->GetGUID() )
		return false; 

	//don't agro neutrals
	if( ( pUnit->IsPlayer() || pUnit->IsPet() )
		&& m_Unit->m_factionDBC
		&& m_Unit->m_factionDBC->RepListId == -1 
		&& m_Unit->m_faction->HostileMask == 0 
		&& m_Unit->m_faction->FriendlyMask == 0
		)
			return false;
	else if( ( m_Unit->IsPlayer() || m_Unit->IsPet() )
			&& pUnit->m_factionDBC
			&& pUnit->m_factionDBC->RepListId == -1 
			&& pUnit->m_faction->HostileMask == 0 
			&& pUnit->m_faction->FriendlyMask == 0
			)
			return false;

	//make sure we do not agro flying stuff
	if( abs( pUnit->GetPositionZ() - m_Unit->GetPositionZ() ) > _CalcCombatRange( pUnit, false ) )
		return false; //blizz has this set to 250 but uses pathfinding

	return true;
}

//this function might be slow but so it should not be spammed
//!!!this function has been reported the biggest bottleneck on emu in 2008 07 04
Unit* AIInterface::FindTarget()
{// find nearest hostile Target to attack
	if( !m_AllowedToEnterCombat ) 
		return NULL;

	if(  m_Unit->GetMapMgr() == NULL )
		return NULL; 

	Unit* target = NULL;
	Unit* critterTarget = NULL;
	float distance = 999999.0f; // that should do it.. :p
//	float crange;
//	float z_diff;

	std::set<Object*>::iterator itr, itr2;
	std::set<Player *>::iterator pitr, pitr2;
//	Object *pObj;
	Unit *pUnit;
	float dist;
	
	/* Commented due to no use
	bool pvp=true;
	if(m_Unit->GetTypeId()==TYPEID_UNIT&&((Creature*)m_Unit)->GetCreatureInfo()&&((Creature*)m_Unit)->GetCreatureInfo()->Civilian)
		pvp=false;*/

	//target is immune to all form of attacks, cant attack either.
	// not attackable creatures sometimes fight enemies in scripted fights though
	if(m_Unit->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NOT_ATTACKABLE_2))
	{
		return 0;
	}

	if (m_isNeutralGuard)
	{
		Player *tmpPlr;
		for (std::set<Player*>::iterator itrPlr = m_Unit->GetInRangePlayerSetBegin(); itrPlr != m_Unit->GetInRangePlayerSetEnd(); ++itrPlr)
		{
			tmpPlr = (*itrPlr);
			if (tmpPlr == NULL)
				continue;
			if (tmpPlr->GetTaxiState())
				continue;
			if (tmpPlr->bInvincible)
				continue;
			if (tmpPlr->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_FEIGN_DEATH))
				continue;
			if (tmpPlr->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9))
				continue;
			if (tmpPlr->m_invisible)
				continue;
			if( !tmpPlr->HasFlag( PLAYER_FLAGS, 0x100) )//PvP Guard Attackable.
				continue;

			dist = m_Unit->GetDistanceSq(tmpPlr);

			if (dist > 2500.0f)
				continue;
			if (distance > dist)
			{
				if (sWorld.Collision) {
					if( CollideInterface.CheckLOS( m_Unit->GetMapId(), m_Unit->GetPositionNC(), tmpPlr->GetPositionNC() ) )
					{
						distance = dist;
						target = static_cast<Unit*>(tmpPlr);
					}
				} else {
					distance = dist;
					target = static_cast<Unit*>(tmpPlr);
				}
			}
		}
		if (target)
		{
			m_Unit->m_runSpeed = m_Unit->m_base_runSpeed * 2.0f;
			AttackReaction(target, 1, 0);

			WorldPacket data(SMSG_AI_REACTION, 12);
			data << m_Unit->GetGUID() << uint32(2);		// Aggro sound
			static_cast< Player* >( target )->GetSession()->SendPacket( &data );

			return target;
		}
		distance = 999999.0f; //Reset Distance for normal check
	}

	//we have a high chance that we will agro a player
	//this is slower then oppfaction list BUT it has a lower chance that contains invalid pointers
	for( pitr2 = m_Unit->GetInRangePlayerSetBegin(); pitr2 != m_Unit->GetInRangePlayerSetEnd(); )
	{
		pitr = pitr2;
		++pitr2;

		pUnit = *pitr;

		if( UnsafeCanOwnerAttackUnit( pUnit ) == false )
			continue;

		//on blizz there is no Z limit check 
		dist = m_Unit->GetDistance2dSq(pUnit);

		if(dist > distance)	 // we want to find the CLOSEST target
			continue;
	
		if(dist <= _CalcAggroRange(pUnit) )
		{
			if (sWorld.Collision) {
				if( CollideInterface.CheckLOS( m_Unit->GetMapId( ), m_Unit->GetPositionNC( ), pUnit->GetPositionNC( ) ) )
				{
					distance = dist;
					target = pUnit;
				}
			} else {
				distance = dist;
				target = pUnit;
			}
		}
	}

	//a lot less times are check inter faction mob wars :)
	if( m_updateTargetsTimer2 < getMSTime() )
	{
		m_updateTargetsTimer2 = getMSTime() + TARGET_UPDATE_INTERVAL;
		m_Unit->AquireInrangeLock(); //make sure to release lock before exit function !
		for( itr2 = m_Unit->GetInRangeSetBegin(); itr2 != m_Unit->GetInRangeSetEnd(); )
		{
			itr = itr2;
			++itr2;

			if( !(*itr)->IsUnit() )
				continue;

			pUnit = static_cast< Unit* >( (*itr) );

			if( UnsafeCanOwnerAttackUnit( pUnit ) == false )
				continue;

			//on blizz there is no Z limit check 
			dist = m_Unit->GetDistance2dSq(pUnit);

			if(pUnit->m_faction && pUnit->m_faction->Faction == 28)// only Attack a critter if there is no other Enemy in range
			{
				if(dist < 225.0f)	// was 10
					critterTarget = pUnit;
				continue;
			}

			if(dist > distance)	 // we want to find the CLOSEST target
				continue;
	
			if(dist <= _CalcAggroRange(pUnit) )
			{
				if (sWorld.Collision) {
					if( CollideInterface.CheckLOS( m_Unit->GetMapId( ), m_Unit->GetPositionNC( ), pUnit->GetPositionNC( ) ) )
					{
						distance = dist;
						target = pUnit;
					}
				} else {
					distance = dist;
					target = pUnit;
				}
			}
		}
		m_Unit->ReleaseInrangeLock();
	}

	if( !target )
	{
		target = critterTarget;
	}

	if( target )
	{
/*		if(m_isGuard)
		{
			m_Unit->m_runSpeed = m_Unit->m_base_runSpeed * 2.0f;
			m_fastMove = true;
		}*/

		AttackReaction(target, 1, 0);
		if(target->IsPlayer())
		{
			WorldPacket data(SMSG_AI_REACTION, 12);
			data << m_Unit->GetGUID() << uint32(2);		// Aggro sound
			static_cast< Player* >( target )->GetSession()->SendPacket( &data );
		}
		if(target->GetUInt32Value(UNIT_FIELD_CREATEDBY) != 0)
		{
			Unit* target2 = m_Unit->GetMapMgr()->GetPlayer(target->GetUInt32Value(UNIT_FIELD_CREATEDBY));
			/*if(!target2)
			{
				target2 = sObjHolder.GetObject<Player>(target->GetUInt32Value(UNIT_FIELD_CREATEDBY));
			}*/
			if(target2)
			{
				AttackReaction(target2, 1, 0);
			}
		}
	}
	return target;
}

Unit* AIInterface::FindTargetForSpell(AI_Spell *sp)
{
	/*if(!m_Unit) return NULL;*/

	/*if(!sp)
	{
		m_Unit->SetUInt64Value(UNIT_FIELD_TARGET, 0);
		return NULL;
	}*/

	TargetMap::iterator itr, itr2;

	if(sp)
	{
		if(sp->spellType == STYPE_HEAL)
		{
			uint32 cur = m_Unit->GetUInt32Value(UNIT_FIELD_HEALTH) + 1;
			uint32 max = m_Unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH) + 1;
			float healthPercent = float(cur) / float(max);
			if(healthPercent <= sp->floatMisc1) // Heal ourselves cause we got too low HP
			{
				m_Unit->SetUInt64Value(UNIT_FIELD_TARGET, 0);
				return m_Unit;
			}
			for(AssistTargetSet::iterator i = m_assistTargets.begin(); i != m_assistTargets.end(); i++)
			{
				if(!(*i)->isAlive())
				{
					continue;
				}
				cur = (*i)->GetUInt32Value(UNIT_FIELD_HEALTH);
				max = (*i)->GetUInt32Value(UNIT_FIELD_MAXHEALTH);
				healthPercent = float(cur) / float(max);
				if(healthPercent <= sp->floatMisc1) // Heal ourselves cause we got too low HP
				{
					m_Unit->SetUInt64Value(UNIT_FIELD_TARGET, (*i)->GetGUID());
					return (*i); // heal Assist Target which has low HP
				}
			}
		}
		if(sp->spellType == STYPE_BUFF)
		{
			m_Unit->SetUInt64Value(UNIT_FIELD_TARGET, 0);
			return m_Unit;
		}
	}

	return GetMostHated();
}

bool AIInterface::FindFriends(float dist)
{

	if(  m_Unit->GetMapMgr() == NULL )
		return false; 

	bool result = false;
	TargetMap::iterator it;

	std::set<Object*>::iterator itr;
	Unit *pUnit;

	m_Unit->AquireInrangeLock(); //make sure to release lock before exit function !
	for( itr = m_Unit->GetInRangeSetBegin(); itr != m_Unit->GetInRangeSetEnd(); itr++ )
	{
		if(!(*itr) || (*itr)->GetTypeId() != TYPEID_UNIT)
			continue;

		pUnit = static_cast<Unit*>((*itr));
		if(!pUnit->isAlive())
			continue;

		if(pUnit->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE))
		{
			continue;
		}
		if(pUnit->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9))
		{
			continue;
		}

		if( isCombatSupport( m_Unit, pUnit ) && ( pUnit->GetAIInterface()->getAIState() == STATE_IDLE || pUnit->GetAIInterface()->getAIState() == STATE_SCRIPTIDLE ) )//Not sure
		{
			if( m_Unit->GetDistanceSq(pUnit) < dist)
			{
				if( m_assistTargets.count( pUnit ) == 0 )
				{
					result = true;
					m_assistTargets.insert(pUnit);
				}

				LockAITargets(true);

				for(it = m_aiTargets.begin(); it != m_aiTargets.end(); ++it)
				{
					Unit *ai_t = m_Unit->GetMapMgr()->GetUnit( it->first );
					if( ai_t && pUnit->GetAIInterface() && isHostile((Object*)ai_t,(Object*)pUnit) )
						pUnit->GetAIInterface()->AttackReaction( ai_t, 1, 0 );
				}

				LockAITargets(false);
			}
		}
	}
	m_Unit->ReleaseInrangeLock();

	// check if we're a civilian, in which case summon guards on a despawn timer
	uint8 civilian = (((Creature*)m_Unit)->GetCreatureInfo()) ? (((Creature*)m_Unit)->GetCreatureInfo()->Civilian) : 0;
	uint32 family = (((Creature*)m_Unit)->GetCreatureInfo()) ? (((Creature*)m_Unit)->GetCreatureInfo()->Type) : 0;
	if(family == HUMANOID && civilian && getMSTime() > m_guardTimer && !IS_INSTANCE(m_Unit->GetMapId()))
	{
		m_guardTimer = getMSTime() + 15000;
		uint16 AreaId = m_Unit->GetMapMgr()->GetAreaID(m_Unit->GetPositionX(),m_Unit->GetPositionY());
		AreaTable * at = dbcArea.LookupEntry(AreaId);
		if(!at)
			return result;

		ZoneGuardEntry * zoneSpawn = ZoneGuardStorage.LookupEntry(at->ZoneId);
		if(!zoneSpawn) return result;

		uint32 team = 1; // horde default
		if(isAlliance(m_Unit))
			team = 0;

		uint32 guardid = zoneSpawn->AllianceEntry;
		if(team == 1) guardid = zoneSpawn->HordeEntry;
		if(!guardid) return result;

		CreatureInfo * ci = CreatureNameStorage.LookupEntry(guardid);
		if(!ci)
			return result;

		float x = m_Unit->GetPositionX() + (float)( (float)(rand() % 150 + 100) / 1000.0f );
		float y = m_Unit->GetPositionY() + (float)( (float)(rand() % 150 + 100) / 1000.0f );
		float z;

		if (sWorld.Collision) {
			z = CollideInterface.GetHeight(m_Unit->GetMapId(), x, y, m_Unit->GetPositionZ() + 2.0f);
			if( z == NO_WMO_HEIGHT )
				z = m_Unit->GetMapMgr()->GetLandHeight(x, y);

			if( fabs( z - m_Unit->GetPositionZ() ) > 10.0f )
				z = m_Unit->GetPositionZ();
		} else {
			z = m_Unit->GetPositionZ();
			float adt_z = m_Unit->GetMapMgr()->GetLandHeight(x, y);
			if(fabs(z - adt_z) < 3)
				z = adt_z;
		}

		CreatureProto * cp = CreatureProtoStorage.LookupEntry(guardid);
		if(!cp) return result;

		uint8 spawned = 0;
	
		std::set<Player*>::iterator hostileItr = m_Unit->GetInRangePlayerSetBegin();
		for(; hostileItr != m_Unit->GetInRangePlayerSetEnd(); hostileItr++)
		{
			if(spawned >= 3)
				break;

			if(!isHostile(*hostileItr, m_Unit))
				continue;

			if (spawned == 0)
			{
				uint32 languageid = (team == 0) ? LANG_COMMON : LANG_ORCISH;
				m_Unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, languageid, "Guards!");
			}

			Creature * guard = m_Unit->GetMapMgr()->CreateCreature(guardid);
			guard->Load(cp, x, y, z);
			guard->SetInstanceID(m_Unit->GetInstanceID());
			guard->SetZoneId(m_Unit->GetZoneId());
			guard->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP); /* shitty DBs */
			guard->m_noRespawn=true;
		
			if(guard->CanAddToWorld())
			{
				uint32 t = RandomUInt(8)*1000;
				if(t==0)
					guard->PushToWorld(m_Unit->GetMapMgr());
				else
					sEventMgr.AddEvent(guard,&Creature::AddToWorld, m_Unit->GetMapMgr(), EVENT_UNK, t, 1, 0);
			}
			else
			{
				guard->SafeDelete();
				return result;
			}
			
			sEventMgr.AddEvent(guard, &Creature::SetGuardWaypoints, EVENT_UNK, 10000, 1,0);
			sEventMgr.AddEvent(guard, &Creature::SafeDelete, EVENT_CREATURE_SAFE_DELETE, 60*5*1000, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			spawned++;
		}
	}

	return result;
}

float AIInterface::_CalcAggroRange(Unit* target)
{
	//float baseAR = 15.0f; // Base Aggro Range
					// -8	 -7	 -6	 -5	 -4	 -3	 -2	 -1	 0	  +1	 +2	 +3	 +4	 +5	 +6	 +7	+8
	//float baseAR[17] = {29.0f, 27.5f, 26.0f, 24.5f, 23.0f, 21.5f, 20.0f, 18.5f, 17.0f, 15.5f, 14.0f, 12.5f, 11.0f,  9.5f,  8.0f,  6.5f, 5.0f};
	float baseAR[17] = {19.0f, 18.5f, 18.0f, 17.5f, 17.0f, 16.5f, 16.0f, 15.5f, 15.0f, 14.5f, 12.0f, 10.5f, 8.5f,  7.5f,  6.5f,  6.5f, 5.0f};
	// Lvl Diff -8 -7 -6 -5 -4 -3 -2 -1 +0 +1 +2  +3  +4  +5  +6  +7  +8
	// Arr Pos   0  1  2  3  4  5  6  7  8  9 10  11  12  13  14  15  16
	int8 lvlDiff = static_cast<int8>(target->getLevel() - m_Unit->getLevel());
	uint8 realLvlDiff = lvlDiff;
	if(lvlDiff > 8)
	{
		lvlDiff = 8;
	}
	if(lvlDiff < -8)
	{
		lvlDiff = -8;
	}
	if(!((Creature*)m_Unit)->CanSee(target))
		return 0;
	
	float AggroRange = baseAR[lvlDiff + 8];
	if(realLvlDiff > 8)
	{
		AggroRange += AggroRange * ((lvlDiff - 8) * 5 / 100);
	}

	// Multiply by elite value
	if(m_Unit->IsCreature() && ((Creature*)m_Unit)->GetCreatureInfo() && ((Creature*)m_Unit)->GetCreatureInfo()->Rank > 0)
		AggroRange *= (((Creature*)m_Unit)->GetCreatureInfo()->Rank) * 1.50f;

	if(AggroRange > 40.0f) // cap at 40.0f
	{
		AggroRange = 40.0f;
	}
  /*  //printf("aggro range: %f , stealthlvl: %d , detectlvl: %d\n",AggroRange,target->GetStealthLevel(),m_Unit->m_stealthDetectBonus);
	if(! ((Creature*)m_Unit)->CanSee(target))
	{
		AggroRange =0;
	//	AggroRange *= ( 100.0f - (target->m_stealthLevel - m_Unit->m_stealthDetectBonus)* 20.0f ) / 100.0f;
	}
*/
	// SPELL_AURA_MOD_DETECT_RANGE
	int32 modDetectRange = target->getDetectRangeMod(m_Unit->GetGUID());
	AggroRange += modDetectRange;
	if(target->IsPlayer())
		AggroRange += static_cast< Player* >( target )->DetectedRange;
	if(AggroRange < 3.0f)
	{
		AggroRange = 3.0f;
	}
	if(AggroRange > 40.0f) // cap at 40.0f
	{
		AggroRange = 40.0f;
	}

	return (AggroRange*AggroRange);
}

void AIInterface::_CalcDestinationAndMove(Unit *target, float dist)
{
	if(!m_canMove || m_Unit->IsStunned())
	{
		StopMovement(0); //Just Stop
		return;
	}
	
	if( target && ( target->GetTypeId() == TYPEID_UNIT || target->GetTypeId() == TYPEID_PLAYER) )
	{
#ifdef HACKY_SERVER_CLIENT_POS_SYNC
		moved_for_attack = true;
#endif
		float ResX = target->GetPositionX();
		float ResY = target->GetPositionY();

		//avoid eating bandwidth with useless movement packets when target did not move since last position
		//this will work since it turned into a common myth that when you pull mob you should not move :D
		if( abs(m_last_target_x - ResX) < DISTANCE_TO_SMALL_TO_WALK
			&& abs(m_last_target_y - ResY) < DISTANCE_TO_SMALL_TO_WALK && m_creatureState == MOVING)
			return;
		m_last_target_x = ResX;
		m_last_target_y = ResY;

		float ResZ = target->GetPositionZ();

		float angle = m_Unit->calcAngle(m_Unit->GetPositionX(), m_Unit->GetPositionY(), ResX, ResY) * float(M_PI) / 180.0f;
		float x = dist * cosf(angle);
		float y = dist * sinf(angle);

		if(target->GetTypeId() == TYPEID_PLAYER && static_cast< Player* >( target )->m_isMoving )
		{
			// cater for moving player vector based on orientation
			x -= cosf(target->GetOrientation());
			y -= sinf(target->GetOrientation());
		}

		m_nextPosX = ResX - x;
		m_nextPosY = ResY - y;
		m_nextPosZ = ResZ;
	}
	else
	{
		target = NULL;
		m_nextPosX = m_Unit->GetPositionX();
		m_nextPosY = m_Unit->GetPositionY();
		m_nextPosZ = m_Unit->GetPositionZ();
	}

/*
	if (sWorld.Collision) {
		float target_land_z=0.0f;
		if( m_Unit->GetMapMgr() != NULL )
		{
			if(m_moveFly != true)
			{
				target_land_z = CollideInterface.GetHeight(m_Unit->GetMapId(), m_nextPosX, m_nextPosY, m_nextPosZ + 2.0f);
				if( target_land_z == NO_WMO_HEIGHT )
					target_land_z = m_Unit->GetMapMgr()->GetLandHeight(m_nextPosX, m_nextPosY);
			}
		}

		if (m_nextPosZ > m_Unit->GetMapMgr()->GetWaterHeight(m_nextPosX, m_nextPosY) && target_land_z != 0.0f)
			m_nextPosZ=target_land_z;
	}
*/

	float dx = m_nextPosX - m_Unit->GetPositionX();
	float dy = m_nextPosY - m_Unit->GetPositionY();
	if(dy != 0.0f)
	{
		float angle = atan2(dx, dy);
		m_Unit->SetOrientation(angle);
	}

	if(m_creatureState != MOVING)
		UpdateMove();
}

float AIInterface::_CalcCombatRange(Unit* target, bool ranged)
{
	if(!target)
	{
		return 0;
	}
	float range = 0.0f;
	float rang = 0.0f;
	if(ranged)
	{
		rang = 5.0f;
	}

	float selfreach = m_Unit->GetFloatValue(UNIT_FIELD_COMBATREACH);
	float targetradius;
//	targetradius = target->GetFloatValue(UNIT_FIELD_BOUNDINGRADIUS); //this is plain wrong. Represents i have no idea what :)
	targetradius = target->GetModelHalfSize();
	float selfradius;
//	selfradius = m_Unit->GetFloatValue(UNIT_FIELD_BOUNDINGRADIUS); //this is plain wrong. Represents i have no idea what :)
	selfradius = m_Unit->GetModelHalfSize();
//	float targetscale = target->GetFloatValue(OBJECT_FIELD_SCALE_X);
//	float selfscale = m_Unit->GetFloatValue(OBJECT_FIELD_SCALE_X);

//	range = ((((targetradius*targetradius)*targetscale) + selfreach) + ((selfradius*selfscale) + rang));
	range = targetradius + selfreach + selfradius + rang;
//	if(range > 28.29f) range = 28.29f;
	return range;
}

float AIInterface::_CalcDistanceFromHome()
{
	if (m_AIType == AITYPE_PET)
	{
		return m_Unit->GetDistanceSq(m_PetOwner);
	}
	else if(m_Unit->GetTypeId() == TYPEID_UNIT)
	{

		if(m_returnX !=0.0f && m_returnY != 0.0f)
		{
			return m_Unit->GetDistanceSq(m_returnX,m_returnY,m_returnZ);
		}
	}

	return 0.0f;
}

/************************************************************************************************************
SendMoveToPacket:
Comments: Some comments on the SMSG_MONSTER_MOVE packet: 
	the uint8 field:
		0: Default															known
		1: Don't move														known
		2: there is an extra 3 floats, also known as a vector				unknown
		3: there is an extra uint64 most likely a guid.						unknown
		4: there is an extra float that causes the orientation to be set.	known
		
		note:	when this field is 1. 
			there is no need to send  the next 3 uint32's as they aren't used by the client
	
	the MoveFlags:
		0x00000000 - Walk
		0x00000100 - Run
		0x00000200 - Fly
		some comments on that 0x00000300 - Fly = 0x00000100 | 0x00000200

	waypoint's:
		TODO.... as they somehow seemed to be changed long time ago..
		
*************************************************************************************************************/

void AIInterface::SendMoveToPacket(float toX, float toY, float toZ, float toO, uint32 time, uint32 MoveFlags)
{
	//this should NEVER be called directly !!!!!!
	//use MoveTo()

#ifndef USING_BIG_ENDIAN
	StackWorldPacket<60> data(SMSG_MONSTER_MOVE);
#else
	WorldPacket data(SMSG_MONSTER_MOVE, 60);
#endif
	data << m_Unit->GetNewGUID();
	data << m_Unit->GetPositionX() << m_Unit->GetPositionY() << m_Unit->GetPositionZ();
	data << getMSTime();
	
	// Check if we have an orientation
	if(toO != 0.0f)
	{
		data << uint8(4);
		data << toO;
	} else {
		data << uint8(0);
	}
	data << MoveFlags;
	data << time;
	data << uint32(1);	  // 1 waypoint
	data << toX << toY << toZ;

#ifndef ENABLE_COMPRESSED_MOVEMENT_FOR_CREATURES
	bool self = m_Unit->GetTypeId() == TYPEID_PLAYER;
	m_Unit->SendMessageToSet( &data, self );
#else
	if( m_Unit->GetTypeId() == TYPEID_PLAYER )
		static_cast<Player*>(m_Unit)->GetSession()->SendPacket(&data);

	for(set<Player*>::iterator itr = m_Unit->GetInRangePlayerSetBegin(); itr != m_Unit->GetInRangePlayerSetEnd(); ++itr)
	{
		if( (*itr)->GetPositionNC().Distance2DSq( m_Unit->GetPosition() ) >= World::m_movementCompressThresholdCreatures )
			(*itr)->AppendMovementData( SMSG_MONSTER_MOVE, data.GetSize(), (const uint8*)data.GetBufferPointer() );
		else
			(*itr)->GetSession()->SendPacket(&data);
	}
#endif
}

/*
void AIInterface::SendMoveToSplinesPacket(std::list<Waypoint> wp, bool run)
{
	if(!m_canMove)
	{
		return;
	}

	WorldPacket data;

	uint8 DontMove = 0;
	uint32 travelTime = 0;
	for(std::list<Waypoint>::iterator i = wp.begin(); i != wp.end(); i++)
	{
		travelTime += i->time;
	}

	data.Initialize( SMSG_MONSTER_MOVE );
	data << m_Unit->GetNewGUID();
	data << m_Unit->GetPositionX() << m_Unit->GetPositionY() << m_Unit->GetPositionZ();
	data << getMSTime();
	data << uint8(DontMove);
	data << uint32(run ? 0x00000100 : 0x00000000);
	data << travelTime;
	data << (uint32)wp.size();
	for(std::list<Waypoint>::iterator i = wp.begin(); i != wp.end(); i++)
	{
		data << i->x;
		data << i->y;
		data << i->z;
	}

	m_Unit->SendMessageToSet( &data, false );
}
*/
bool AIInterface::StopMovement(uint32 time)
{
	m_moveTimer = time; //set pause after stopping
	m_creatureState = STOPPED;

	m_destinationX = m_destinationY = m_destinationZ = 0;
	m_nextPosX = m_nextPosY = m_nextPosZ = 0;
	m_timeMoved = 0;
	m_timeToMove = 0;

	WorldPacket data(26);
	data.SetOpcode(SMSG_MONSTER_MOVE);
	data << m_Unit->GetNewGUID();
	data << m_Unit->GetPositionX() << m_Unit->GetPositionY() << m_Unit->GetPositionZ();
	data << getMSTime();
	data << uint8(1);   // "DontMove = 1"

	m_Unit->SendMessageToSet( &data, false );
	return true;
}

void AIInterface::MoveTo(float x, float y, float z, float o)
{
	m_sourceX = m_Unit->GetPositionX();
	m_sourceY = m_Unit->GetPositionY();
	m_sourceZ = m_Unit->GetPositionZ();

	if(!m_canMove || m_Unit->IsStunned())
	{
		StopMovement(0); //Just Stop
		return;
	}


	m_nextPosX = x;
	m_nextPosY = y;
	m_nextPosZ = z;

/*	//Andy
#ifdef COLLISION
	float target_land_z=0.0f;
	if( m_Unit->GetMapMgr() != NULL )
	{
		if(m_moveFly != true)
		{
			target_land_z = CollideInterface.GetHeight(m_Unit->GetMapId(), m_nextPosX, m_nextPosY, m_nextPosZ + 2.0f);
			if( target_land_z == NO_WMO_HEIGHT )
				target_land_z = m_Unit->GetMapMgr()->GetLandHeight(m_nextPosX, m_nextPosY);
		}
	}

	if (m_nextPosZ > m_Unit->GetMapMgr()->GetWaterHeight(m_nextPosX, m_nextPosY) && target_land_z != 0.0f)
		m_nextPosZ=target_land_z;
#endif*/

	if ( m_creatureState != MOVING )
		UpdateMove();
}

bool AIInterface::IsFlying()
{
	if(m_moveFly)
		return true;
	
	/*float z = m_Unit->GetMapMgr()->GetLandHeight(m_Unit->GetPositionX(), m_Unit->GetPositionY());
	if(z)
	{
		if(m_Unit->GetPositionZ() >= (z + 1.0f)) //not on ground? Oo
		{
			return true;
		}
	}
	return false;*/
	if( m_Unit->GetTypeId() == TYPEID_PLAYER )
		return static_cast< Player* >( m_Unit )->FlyCheat;

	return false;
}

uint32 AIInterface::getMoveFlags()
{
	uint32 MoveFlags = 0;
	if(m_moveFly == true) //Fly
	{
		m_flySpeed = m_Unit->m_flySpeed*0.001f;
		MoveFlags = 0x300;
	}
	else if(m_moveSprint == true) //Sprint
	{
		m_runSpeed = (m_Unit->m_runSpeed+5.0f)*0.001f;
		MoveFlags = 0x100;
	}
	else if(m_moveRun == true) //Run
	{
		m_runSpeed = m_Unit->m_runSpeed*0.001f;
		MoveFlags = 0x100;
	}
/*	else //Walk
	{
		m_runSpeed = m_Unit->m_walkSpeed*0.001f;
		MoveFlags = 0x000;
	}*/
	m_walkSpeed = m_Unit->m_walkSpeed*0.001f;//move distance per ms time 
	return MoveFlags;
}

void AIInterface::UpdateMove()
{
	//this should NEVER be called directly !!!!!!
	//use MoveTo()
	float distance = m_Unit->CalcDistance(m_nextPosX,m_nextPosY,m_nextPosZ);
	
	if(distance < DISTANCE_TO_SMALL_TO_WALK) 
		return; //we don't want little movements here and there

	m_destinationX = m_nextPosX;
	m_destinationY = m_nextPosY;
	m_destinationZ = m_nextPosZ;
	
	/*if(m_moveFly != true)
	{
		if(m_Unit->GetMapMgr())
		{
			float adt_Z = m_Unit->GetMapMgr()->GetLandHeight(m_destinationX, m_destinationY);
			if(fabsf(adt_Z - m_destinationZ) < 3.0f)
				m_destinationZ = adt_Z;
		}
	}*/

	//this prevents updatemovement working on this function
	//m_nextPosX = m_nextPosY = m_nextPosZ = 0;

	uint32 moveTime;
	/* #ifdef INHERIT_FOLLOWED_UNIT_SPEED vojta - this is pointless and we don't need it anymore
	if( UnitToFollow )
	{
//		moveTime = (uint32) (distance * 1000 / UnitToFollow->m_runSpeed ); //I wonder if run peed can ever drop to 0
//life sucks, due to calculations the pet will move slower with correct formulas. We add some catch-up speed
		moveTime = (uint32) (distance * 1000 / ( UnitToFollow->m_runSpeed * sqrt( distance ) ) ); //I wonder if run peed can ever drop to 0
	}
#endif */
	if(m_moveFly)
		moveTime = (uint32) (distance / m_flySpeed);
	else if(m_moveRun)
		moveTime = (uint32) (distance / m_runSpeed);
	else moveTime = (uint32) (distance / m_walkSpeed);

	m_totalMoveTime = moveTime;

	if(m_Unit->GetTypeId() == TYPEID_UNIT)
	{
		Creature *creature = static_cast<Creature*>(m_Unit);
		// check if we're returning to our respawn location. if so, reset back to default
		// orientation
		if(creature->GetSpawnX() == m_destinationX &&
			creature->GetSpawnY() == m_destinationY)
		{
			float o = creature->GetSpawnO();
			creature->SetOrientation(o);
		} else {
			// Calculate the angle to our next position

			float dx = (float)m_destinationX - m_Unit->GetPositionX();
			float dy = (float)m_destinationY - m_Unit->GetPositionY();
			if(dy != 0.0f)
			{
				float angle = atan2(dy, dx);
				m_Unit->SetOrientation(angle);
			}
		}
	}

	if (m_Unit->GetCurrentSpell() == NULL)
		SendMoveToPacket(m_destinationX, m_destinationY, m_destinationZ, m_Unit->GetOrientation(), moveTime, getMoveFlags());

	m_timeToMove = moveTime;
	m_timeMoved = 0;
	if(m_moveTimer == 0)
	{
		m_moveTimer =  UNIT_MOVEMENT_INTERPOLATE_INTERVAL; // update every few msecs
	}

	m_creatureState = MOVING;
}

void AIInterface::SendCurrentMove(Player* plyr/*uint64 guid*/)
{
	if(m_destinationX == 0.0f && m_destinationY == 0.0f && m_destinationZ == 0.0f) return; //invalid move 
	ByteBuffer *splineBuf = new ByteBuffer(20*4);
	*splineBuf << uint32(0); // spline flags
	*splineBuf << uint32((m_totalMoveTime - m_timeToMove)+m_moveTimer); //Time Passed (start Position) //should be generated/save 
	*splineBuf << uint32(m_totalMoveTime); //Total Time //should be generated/save
	*splineBuf << uint32(0); //Unknown
	*splineBuf << uint32(4); //Spline Count	// lets try this

	*splineBuf << m_sourceX << m_sourceY << m_sourceZ;
	*splineBuf << m_Unit->GetPositionX() << m_Unit->GetPositionY() << m_Unit->GetPositionZ();
	*splineBuf << m_destinationX << m_destinationY << m_destinationZ + 0.1f;
	*splineBuf << m_destinationX << m_destinationY << m_destinationZ + 0.2f;
	*splineBuf << uint8(0);
	*splineBuf << m_destinationX << m_destinationY << m_destinationZ;
	
	plyr->AddSplinePacket(m_Unit->GetGUID(), splineBuf);

	//This should only be called by Players AddInRangeObject() ONLY
	//using guid cuz when I attempted to use pointer the player was deleted when this event was called some times
	//Player* plyr = World::GetPlayer(guid);
	//if(!plyr) return;

	/*if(m_destinationX == 0.0f && m_destinationY == 0.0f && m_destinationZ == 0.0f) return; //invalid move 
	uint32 moveTime = m_timeToMove-m_timeMoved;
	//uint32 moveTime = (m_timeToMove-m_timeMoved)+m_moveTimer;
	WorldPacket data(50);
	data.SetOpcode( SMSG_MONSTER_MOVE );
	data << m_Unit->GetNewGUID();
	data << m_Unit->GetPositionX() << m_Unit->GetPositionY() << m_Unit->GetPositionZ();
	data << getMSTime();
	data << uint8(0);
	data << getMoveFlags();

	//float distance = m_Unit->CalcDistance(m_destinationX, m_destinationY, m_destinationZ);
	//uint32 moveTime = (uint32) (distance / m_runSpeed);

	data << moveTime;
	data << uint32(1); //Number of Waypoints
	data << m_destinationX << m_destinationY << m_destinationZ;
	plyr->GetSession()->SendPacket(&data);*/

}

bool AIInterface::setInFront(Unit* target) // not the best way to do it, though
{
	//angle the object has to face
	float angle = m_Unit->calcAngle(m_Unit->GetPositionX(), m_Unit->GetPositionY(), target->GetPositionX(), target->GetPositionY() ); 
	//Change angle slowly 2000ms to turn 180 deg around
	if(angle > 180) angle += 90;
	else angle -= 90; //angle < 180
	m_Unit->getEasyAngle(angle);
	//Convert from degrees to radians (180 deg = PI rad)
	float orientation = angle / float(180 / M_PI);
	//Update Orientation Server Side
	m_Unit->SetPosition(m_Unit->GetPositionX(), m_Unit->GetPositionY(), m_Unit->GetPositionZ(), orientation);
	
	return m_Unit->isInFront(target);
}

bool AIInterface::addWayPoint(WayPoint* wp)
{
	if(!m_waypoints)
		m_waypoints = new WayPointMap ;
	if(!wp) 
		return false;
	if(wp->id <= 0)
		return false; //not valid id

	if(m_waypoints->size() <= wp->id)
		m_waypoints->resize(wp->id+1);

	if((*m_waypoints)[wp->id] == NULL)
	{
		(*m_waypoints)[wp->id] = wp;
		return true;
	}
	return false;
}

void AIInterface::changeWayPointID(uint32 oldwpid, uint32 newwpid)
{
	if(!m_waypoints)return;
	if(newwpid <= 0) 
		return; //not valid id
	if(newwpid > m_waypoints->size()) 
		return; //not valid id
	if(oldwpid > m_waypoints->size())
		return;

	if(newwpid == oldwpid) 
		return; //same spot

	//already wp with that id ?
	WayPoint* originalwp = getWayPoint(newwpid);
	if(!originalwp) 
		return;
	WayPoint* oldwp = getWayPoint(oldwpid);
	if(!oldwp) 
		return;

	oldwp->id = newwpid;
	originalwp->id = oldwpid;
	(*m_waypoints)[oldwp->id] = oldwp;
	(*m_waypoints)[originalwp->id] = originalwp;

	//SaveAll to db
	saveWayPoints();
}

void AIInterface::deleteWayPoint(uint32 wpid)
{
	if(!m_waypoints)return;
	if(wpid <= 0) 
		return; //not valid id
	if(wpid > m_waypoints->size()) 
		return; //not valid id

	WayPointMap new_waypoints;
	uint32 newpid = 1;
	for(WayPointMap::iterator itr = m_waypoints->begin(); itr != m_waypoints->end(); ++itr)
	{
		if((*itr) == NULL || (*itr)->id == wpid)
		{
			if((*itr) != NULL)
				delete (*itr);

			continue;
		}

		new_waypoints.push_back(*itr);
	}

	m_waypoints->clear();
	m_waypoints->push_back((WayPoint*)NULL);		// waypoint 0
	for(WayPointMap::iterator itr = new_waypoints.begin(); itr != new_waypoints.end(); ++itr)
	{
		(*itr)->id = newpid++;
		m_waypoints->push_back(*itr);
	}

	saveWayPoints();
}

bool AIInterface::showWayPoints(Player* pPlayer, bool Backwards)
{
	if(!m_waypoints)
		return false;

	//wpid of 0 == all
	WayPointMap::const_iterator itr;
	if(m_WayPointsShowing == true) 
		return false;

	m_WayPointsShowing = true;

	WayPoint* wp = NULL;
	for (itr = m_waypoints->begin(); itr != m_waypoints->end(); itr++)
	{
		if( (*itr) != NULL )
		{
			wp = *itr;

			//Create
			Creature* pWayPoint = new Creature((uint64)HIGHGUID_TYPE_WAYPOINT << 32 | wp->id);
			pWayPoint->CreateWayPoint(wp->id,pPlayer->GetMapId(),wp->x,wp->y,wp->z,0);
			pWayPoint->SetUInt32Value(OBJECT_FIELD_ENTRY, 300000);
			pWayPoint->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.5f);
			if(Backwards)
			{
				uint32 DisplayID = (wp->backwardskinid == 0)? GetUnit()->GetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID) : wp->backwardskinid;
				pWayPoint->SetUInt32Value(UNIT_FIELD_DISPLAYID, DisplayID);
				pWayPoint->SetUInt32Value(UNIT_NPC_EMOTESTATE, wp->backwardemoteid);
			}
			else
			{
				uint32 DisplayID = (wp->forwardskinid == 0)? GetUnit()->GetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID) : wp->forwardskinid;
				pWayPoint->SetUInt32Value(UNIT_FIELD_DISPLAYID, DisplayID);
				pWayPoint->SetUInt32Value(UNIT_NPC_EMOTESTATE, wp->forwardemoteid);
			}
			pWayPoint->SetUInt32Value(UNIT_FIELD_LEVEL, wp->id);
			pWayPoint->SetUInt32Value(UNIT_NPC_FLAGS, 0);
			pWayPoint->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE , pPlayer->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
			pWayPoint->SetUInt32Value(UNIT_FIELD_HEALTH, 1);
			pWayPoint->SetUInt32Value(UNIT_FIELD_MAXHEALTH, 1);
			pWayPoint->SetUInt32Value(UNIT_FIELD_STAT0, wp->flags);

			//Create on client
			ByteBuffer buf(3000);
			uint32 count = pWayPoint->BuildCreateUpdateBlockForPlayer(&buf, pPlayer);
			pPlayer->PushCreationData(&buf, count);

			//root the object
			WorldPacket data1;
			data1.Initialize(SMSG_FORCE_MOVE_ROOT);
			data1 << pWayPoint->GetNewGUID();
			pPlayer->GetSession()->SendPacket( &data1 );

			//Cleanup
			delete pWayPoint;
		}
	}
	return true;
}

bool AIInterface::hideWayPoints(Player* pPlayer)
{
	if(!m_waypoints)
		return false;

	//wpid of 0 == all
	if(m_WayPointsShowing != true) return false;
	m_WayPointsShowing = false;
	WayPointMap::const_iterator itr;

	// slightly better way to do this
	uint64 guid;

	for (itr = m_waypoints->begin(); itr != m_waypoints->end(); itr++)
	{
		if( (*itr) != NULL )
		{
			// avoid C4293
			guid = ((uint64)HIGHGUID_TYPE_WAYPOINT << 32) | (*itr)->id;
			WoWGuid wowguid(guid);
			pPlayer->PushOutOfRange(wowguid);
		}
	}
	return true;
}

bool AIInterface::saveWayPoints()
{
	if(!m_waypoints)return false;

	if(!GetUnit()) return false;
	if(GetUnit()->GetTypeId() != TYPEID_UNIT) return false;

	WorldDatabase.Execute("DELETE FROM creature_waypoints WHERE spawnid = %u", ((Creature*)GetUnit())->GetSQL_id());
	WayPointMap::const_iterator itr;
	WayPoint* wp = NULL;
	std::stringstream ss;

	for (itr = m_waypoints->begin(); itr != m_waypoints->end(); itr++)
	{
		if((*itr) == NULL) 
			continue;

		wp = (*itr);

		//Save
		ss.str("");
		ss << "REPLACE INTO creature_waypoints ";
		ss << "(spawnid,waypointid,position_x,position_y,position_z,waittime,flags,forwardemoteoneshot,forwardemoteid,backwardemoteoneshot,backwardemoteid,forwardskinid,backwardskinid) VALUES (";
		ss << ((Creature*)GetUnit())->GetSQL_id() << ", ";
		ss << wp->id << ", ";
		ss << wp->x << ", ";
		ss << wp->y << ", ";
		ss << wp->z << ", ";
		ss << wp->waittime << ", ";
		ss << wp->flags << ", ";
		ss << wp->forwardemoteoneshot << ", ";
		ss << wp->forwardemoteid << ", ";
		ss << wp->backwardemoteoneshot << ", ";
		ss << wp->backwardemoteid << ", ";
		ss << wp->forwardskinid << ", ";
		ss << wp->backwardskinid << ")\0";
		WorldDatabase.Query( ss.str().c_str() );
	}
	return true;
}

void AIInterface::deleteWaypoints()
{
	if(!m_waypoints)
		return;

	for(WayPointMap::iterator itr = m_waypoints->begin(); itr != m_waypoints->end(); ++itr)
	{
		if((*itr) != NULL)
			delete (*itr);
	}
	m_waypoints->clear();
}

WayPoint* AIInterface::getWayPoint(uint32 wpid)
{
	if(!m_waypoints)return NULL;
	if(wpid >= m_waypoints->size()) 
		return NULL; //not valid id

	/*WayPointMap::const_iterator itr = m_waypoints->find( wpid );
	if( itr != m_waypoints->end( ) )
		return itr->second;*/
	return m_waypoints->at(wpid);
}

void AIInterface::_UpdateMovement(uint32 p_time)
{
	if(!m_Unit->isAlive())
	{
		StopMovement(0);
		return;
	}

	//move after finishing our current spell
	if ( m_Unit->GetCurrentSpell() != NULL )
		return;

	uint32 timediff = 0;

	if(m_moveTimer > 0)
	{
		if(p_time >= m_moveTimer)
		{
			timediff = p_time - m_moveTimer;
			m_moveTimer = 0;
		}
		else
			m_moveTimer -= p_time;
	}

	if(m_timeToMove > 0)
	{
		m_timeMoved = m_timeToMove <= p_time + m_timeMoved ? m_timeToMove : p_time + m_timeMoved;
	}

	if(m_creatureState == MOVING)
	{
		if(!m_moveTimer)
		{
			if(m_timeMoved == m_timeToMove) //reached destination
			{
/*				if(m_fastMove)
				{
					m_Unit->UpdateSpeed();
					m_fastMove = false;
				}*/

				if(m_moveType == MOVEMENTTYPE_WANTEDWP)//We reached wanted wp stop now
					m_moveType = MOVEMENTTYPE_DONTMOVEWP;

				float wayO = 0.0f;

				if((GetWayPointsCount() != 0) && (m_AIState == STATE_IDLE || m_AIState == STATE_SCRIPTMOVE)) //if we attacking don't use wps
				{
					WayPoint* wp = getWayPoint(getCurrentWaypoint());
					if(wp)
					{
						CALL_SCRIPT_EVENT(m_Unit, OnReachWP)(wp->id, !m_moveBackward);
						if(((Creature*)m_Unit)->has_waypoint_text)
							objmgr.HandleMonsterSayEvent(((Creature*)m_Unit), MONSTER_SAY_EVENT_RANDOM_WAYPOINT);

						//Lets face to correct orientation
						wayO = wp->o;
						m_moveTimer = wp->waittime; //wait before next move
						if(!m_moveBackward)
						{
							if(wp->forwardemoteoneshot)
							{
								GetUnit()->Emote(EmoteType(wp->forwardemoteid));
							}
							else
							{
								if(GetUnit()->GetUInt32Value(UNIT_NPC_EMOTESTATE) != wp->forwardemoteid)
								{
									GetUnit()->SetUInt32Value(UNIT_NPC_EMOTESTATE, wp->forwardemoteid);
								}
							}
						}
						else
						{
							if(wp->backwardemoteoneshot)
							{
								GetUnit()->Emote(EmoteType(wp->backwardemoteid));
							}
							else
							{
								if(GetUnit()->GetUInt32Value(UNIT_NPC_EMOTESTATE) != wp->backwardemoteid)
								{
									GetUnit()->SetUInt32Value(UNIT_NPC_EMOTESTATE, wp->backwardemoteid);
								}
							}
						}
					}
					else
						m_moveTimer = RandomUInt(m_moveRun ? 5000 : 10000); // wait before next move
				}

				m_creatureState = STOPPED;
				m_moveSprint = false;

				if(m_MovementType == MOVEMENTTYPE_DONTMOVEWP)
					m_Unit->SetPosition(m_destinationX, m_destinationY, m_destinationZ, wayO, true);
				else
					m_Unit->SetPosition(m_destinationX, m_destinationY, m_destinationZ, m_Unit->GetOrientation(), true);

				m_destinationX = m_destinationY = m_destinationZ = 0;
				m_timeMoved = 0;
				m_timeToMove = 0;
			}
			else
			{
				//Move Server Side Update
				float q = (float)m_timeMoved / (float)m_timeToMove;
				float x = m_Unit->GetPositionX() + (m_destinationX - m_Unit->GetPositionX()) * q;
				float y = m_Unit->GetPositionY() + (m_destinationY - m_Unit->GetPositionY()) * q;
				float z = m_Unit->GetPositionZ() + (m_destinationZ - m_Unit->GetPositionZ()) * q;

				//Andy
				if (sWorld.Collision) {
					float target_land_z=0.0f;
					if( m_Unit->GetMapMgr() != NULL )
					{
						if(m_moveFly != true)
						{
							target_land_z = CollideInterface.GetHeight(m_Unit->GetMapId(), x, y, z + 2.0f);
							if ( target_land_z == NO_WMO_HEIGHT )
							{
								target_land_z = m_Unit->GetMapMgr()->GetLandHeight(x, y);
								if ( target_land_z == 999999.0f )
									target_land_z = z;
							}
						}

						if ( z > m_Unit->GetMapMgr()->GetWaterHeight( m_nextPosX, m_nextPosY ) && target_land_z != 0.0f )
							z = target_land_z;
					}
				}

				m_Unit->SetPosition(x, y, z, m_Unit->GetOrientation());
				
				m_timeToMove -= m_timeMoved;
				m_timeMoved = 0;
				m_moveTimer = (UNIT_MOVEMENT_INTERPOLATE_INTERVAL < m_timeToMove) ? UNIT_MOVEMENT_INTERPOLATE_INTERVAL : m_timeToMove;
			}
			//**** Movement related stuff that should be done after a move update (Keeps Client and Server Synced) ****//
			//**** Process the Pending Move ****//
			if(m_nextPosX != 0.0f && m_nextPosY != 0.0f)
			{
				UpdateMove();
			}
		}
	}
	else if(m_creatureState == STOPPED && (m_AIState == STATE_IDLE || m_AIState == STATE_SCRIPTMOVE) && !m_moveTimer && !m_timeToMove && UnitToFollow == NULL) //creature is stopped and out of Combat
	{
		if(sWorld.getAllowMovement() == false) //is creature movement enabled?
			return;

		if(m_Unit->GetUInt32Value(UNIT_FIELD_DISPLAYID) == 5233) //if Spirit Healer don't move
			return;

		// do we have a formation?
		if(m_formationLinkSqlId != 0)
		{
			if(!m_formationLinkTarget)
			{
				// haven't found our target yet
				Creature * c = static_cast<Creature*>(m_Unit);
				if(!c->haslinkupevent)
				{
					// register linkup event
					c->haslinkupevent = true;
					sEventMgr.AddEvent(c, &Creature::FormationLinkUp, m_formationLinkSqlId, 
						EVENT_CREATURE_FORMATION_LINKUP, 1000, 0,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
				}
			}
			else
			{
				// we've got a formation target, set unittofollow to this
				UnitToFollow = m_formationLinkTarget;
				FollowDistance = m_formationFollowDistance;
				m_fallowAngle = m_formationFollowAngle;
			}
		}
		if(UnitToFollow == 0)
		{
			// no formation, use waypoints
			int destpoint = -1;

			// If creature has no waypoints just wander aimlessly around spawnpoint
			if(GetWayPointsCount()==0) //no waypoints
			{
				/*	if(m_moveRandom)
				{
				if((rand()%10)==0)																																	
				{																																								  
				float wanderDistance = rand()%4 + 2;
				float wanderX = ((wanderDistance*rand()) / RAND_MAX) - wanderDistance / 2;																											   
				float wanderY = ((wanderDistance*rand()) / RAND_MAX) - wanderDistance / 2;																											   
				float wanderZ = 0; // FIX ME ( I don't know how to get appropriate Z coord, maybe use client height map data)																											 

				if(m_Unit->CalcDistance(m_Unit->GetPositionX(), m_Unit->GetPositionY(), m_Unit->GetPositionZ(), ((Creature*)m_Unit)->respawn_cord[0], ((Creature*)m_Unit)->respawn_cord[1], ((Creature*)m_Unit)->respawn_cord[2])>15)																		   
				{   
				//return home																																				 
				MoveTo(((Creature*)m_Unit)->respawn_cord[0],((Creature*)m_Unit)->respawn_cord[1],((Creature*)m_Unit)->respawn_cord[2],false);
				}   
				else
				{
				MoveTo(m_Unit->GetPositionX() + wanderX, m_Unit->GetPositionY() + wanderY, m_Unit->GetPositionZ() + wanderZ,false);
				}	
				}	
				}
				*/	
				return;																																				   
			}																																						  
			else //we do have waypoints
			{
				if(m_moveType == MOVEMENTTYPE_RANDOMWP) //is random move on if so move to a random waypoint
				{
					if(GetWayPointsCount() > 1)
						destpoint = RandomUInt((uint32)GetWayPointsCount());
				}
				else if (m_moveType == MOVEMENTTYPE_CIRCLEWP) //random move is not on lets follow the path in circles
				{
					// 1 -> 10 then 1 -> 10
					m_currentWaypoint++;
					if (m_currentWaypoint > GetWayPointsCount()) m_currentWaypoint = 1; //Happens when you delete last wp seems to continue ticking
					destpoint = m_currentWaypoint;
					m_moveBackward = false;
				}
				else if(m_moveType == MOVEMENTTYPE_WANTEDWP)//Move to wanted wp
				{
					if(m_currentWaypoint)
					{
						if(GetWayPointsCount() > 0)
						{
							destpoint = m_currentWaypoint;
						}
						else
							destpoint = -1;
					}
				}
				else if(m_moveType == MOVEMENTTYPE_FORWARDTHANSTOP)// move to end, then stop
				{
					++m_currentWaypoint;
					if(m_currentWaypoint > GetWayPointsCount())
					{
						//hmm maybe we should stop being path walker since we are waiting here anyway
						destpoint = -1;
					}
					else
						destpoint = m_currentWaypoint;
				}
				else if(m_moveType != MOVEMENTTYPE_QUEST && m_moveType != MOVEMENTTYPE_DONTMOVEWP)//4 Unused
				{
					// 1 -> 10 then 10 -> 1
					if (m_currentWaypoint > GetWayPointsCount()) m_currentWaypoint = 1; //Happens when you delete last wp seems to continue ticking
					if (m_currentWaypoint == GetWayPointsCount()) // Are we on the last waypoint? if so walk back
						m_moveBackward = true;
					if (m_currentWaypoint == 1) // Are we on the first waypoint? if so lets goto the second waypoint
						m_moveBackward = false;
					if (!m_moveBackward) // going 1..n
						destpoint = ++m_currentWaypoint;
					else				// going n..1
						destpoint = --m_currentWaypoint;
				}

				if(destpoint != -1)
				{
					WayPoint* wp = getWayPoint(destpoint);
					if(wp)
					{
						if(!m_moveBackward)
						{
							if((wp->forwardskinid != 0) && (GetUnit()->GetUInt32Value(UNIT_FIELD_DISPLAYID) != wp->forwardskinid))
							{
								GetUnit()->SetUInt32Value(UNIT_FIELD_DISPLAYID, wp->forwardskinid);
								GetUnit()->EventModelChange();
							}
						}
						else
						{
							if((wp->backwardskinid != 0) && (GetUnit()->GetUInt32Value(UNIT_FIELD_DISPLAYID) != wp->backwardskinid))
							{
								GetUnit()->SetUInt32Value(UNIT_FIELD_DISPLAYID, wp->backwardskinid);
								GetUnit()->EventModelChange();
							}
						}
						m_moveFly = (wp->flags == 768) ? 1 : 0;
						m_moveRun = (wp->flags == 256) ? 1 : 0;
						MoveTo(wp->x, wp->y, wp->z, 0);
					}
				}
			}
		}
	}

	//Fear Code
	if(m_AIState == STATE_FEAR && UnitToFear != NULL && m_creatureState == STOPPED)
	{
		if(getMSTime() > m_FearTimer)   // Wait at point for x ms ;)
		{
			float Fx;
			float Fy;
			float Fz;

			if( sWorld.DisableFearMovement )
			{
				if( m_Unit->GetMapId() == 529|| m_Unit->GetMapId() == 566 || m_Unit->GetMapId() == 489 || m_Unit->GetMapId() == 572 || m_Unit->GetMapId() == 562 || m_Unit->GetMapId() == 559 )
				{
					return;	
				}
			}
			// Calculate new angle to target.
			float Fo = m_Unit->calcRadAngle(UnitToFear->GetPositionX(), UnitToFear->GetPositionY(), m_Unit->GetPositionX(), m_Unit->GetPositionY());
			double fAngleAdd = RandomDouble(((M_PI/2) * 2)) - (M_PI/2);
			Fo += (float)fAngleAdd;
			
			float dist = m_Unit->CalcDistance(UnitToFear);
			if(dist > 30.0f || (Rand(25) && dist > 10.0f))	// not too far or too close
			{
					if( m_Unit->GetMapId() == 572 || m_Unit->GetMapId() == 562 || m_Unit->GetMapId() == 559 ) //GET MAP ID
					{
						Fx = m_Unit->GetPositionX();
						Fy = m_Unit->GetPositionY();
					}
					else 
					{
						Fx = m_Unit->GetPositionX() - (RandomFloat(15.f)+5.0f)*cosf(Fo);
						Fy = m_Unit->GetPositionY() - (RandomFloat(15.f)+5.0f)*sinf(Fo);
					}
			}
			else
			{
				Fx = m_Unit->GetPositionX() + (RandomFloat(20.f)+5.0f)*cosf(Fo);
				Fy = m_Unit->GetPositionY() + (RandomFloat(20.f)+5.0f)*sinf(Fo);
			}
			// Check if this point is in water.
			float wl = m_Unit->GetMapMgr()->GetWaterHeight(Fx, Fy);
//			uint8 wt = m_Unit->GetMapMgr()->GetWaterType(Fx, Fy);

			if (sWorld.Collision) {
				Fz = CollideInterface.GetHeight(m_Unit->GetMapId(), Fx, Fy, m_Unit->GetPositionZ() + 2.0f);
				if( Fz == NO_WMO_HEIGHT )
	                Fz = m_Unit->GetMapMgr()->GetLandHeight(Fx, Fy);
				else
				{
					if( CollideInterface.GetFirstPoint(m_Unit->GetMapId(), m_Unit->GetPositionX(), m_Unit->GetPositionY(), m_Unit->GetPositionZ() + 2.0f,
						Fx, Fy, Fz + 2.0f, Fx, Fy, Fz, -1.0f) )
					{
						//Fz = CollideInterface.GetHeight(m_Unit->GetMapId(), Fx, Fy, m_Unit->GetPositionZ() + 2.0f);
					}
				}

				if( fabs( m_Unit->GetPositionZ() - Fz ) > 10.0f || 
					( wl != 0.0f && Fz < wl ) )		// in water
				{
					m_FearTimer=getMSTime() + 500;
				}
				else if( CollideInterface.CheckLOS(m_Unit->GetMapId(), m_Unit->GetPositionX(), m_Unit->GetPositionY(), m_Unit->GetPositionZ() + 2.0f, Fx, Fy, Fz) )
				{
					MoveTo(Fx, Fy, Fz, Fo);
					m_FearTimer = m_totalMoveTime + getMSTime() + 400;
				}
				else
				{
					StopMovement(0);
				}
			} else {
				Fz = m_Unit->GetMapMgr()->GetLandHeight(Fx, Fy);
				if(fabs(m_Unit->GetPositionZ()-Fz) > 4 || (Fz != 0.0f && Fz < (wl-2.0f)))
					m_FearTimer=getMSTime()+100;
				else
				{
					MoveTo(Fx, Fy, Fz, Fo);
					m_FearTimer = m_totalMoveTime + getMSTime() + 200;
				}
			}
		}
	}

	// Wander AI movement code
	if(m_AIState == STATE_WANDER && m_creatureState == STOPPED)
	{
		if(getMSTime() < m_WanderTimer) // is it time to move again?
			return;

		// calculate a random distance and angle to move
		float wanderD = RandomFloat(2.0f) + 2.0f;
		float wanderO = RandomFloat(6.283f);
		float wanderX = m_Unit->GetPositionX() + wanderD * cosf(wanderO);
		float wanderY = m_Unit->GetPositionY() + wanderD * sinf(wanderO);

		if (sWorld.Collision) {
			float wanderZ = CollideInterface.GetHeight(m_Unit->GetMapId(), wanderX, wanderY, m_Unit->GetPositionZ() + 2.0f);
			float wanderZ2 = wanderZ;
			if( wanderZ == NO_WMO_HEIGHT )
				wanderZ = m_Unit->GetMapMgr()->GetLandHeight(wanderX, wanderY);
			else
			{
				if( CollideInterface.GetFirstPoint(m_Unit->GetMapId(), m_Unit->GetPositionX(), m_Unit->GetPositionY(), m_Unit->GetPositionZ() + 2.0f,
					wanderX, wanderY, wanderZ + 2.0f, wanderX, wanderY, wanderZ, -1.0f) )
				{
					//wanderZ = CollideInterface.GetHeight(m_Unit->GetMapId(), wanderX, wanderY, m_Unit->GetPositionZ() + 2.0f);
				}
				else
					wanderZ = wanderZ2;
			}

			if( fabs( m_Unit->GetPositionZ() - wanderZ ) > 10.0f )
			{
				m_WanderTimer=getMSTime() + 1000;
			}
			else if(CollideInterface.CheckLOS(m_Unit->GetMapId(), m_Unit->GetPositionX(), m_Unit->GetPositionY(), m_Unit->GetPositionZ() + 2.0f, wanderX, wanderY, wanderZ))
			{
				m_Unit->SetOrientation(wanderO);
				MoveTo(wanderX, wanderY, wanderZ, wanderO);
				m_WanderTimer = getMSTime() + m_totalMoveTime + 300; // time till next move (+ pause)
			}
			else
			{
				StopMovement(0);
			}
		} else {
			float wanderZ = m_Unit->GetMapMgr()->GetLandHeight(wanderX, wanderY);

			// without these next checks we could fall through the "ground" (WMO) and get stuck
			// wander won't work correctly in cities until we get some way to fix this and remove these checks
			float currentZ = m_Unit->GetPositionZ();
			float landZ = m_Unit->GetMapMgr()->GetLandHeight(m_Unit->GetPositionX(), m_Unit->GetPositionY());

			if( currentZ > landZ + 1.0f // are we more than 1yd above ground? (possible WMO)
			 || wanderZ < currentZ - 5.0f // is our destination land height too low? (possible WMO)
			 || wanderZ > currentZ + wanderD) // is our destination too high to climb?
			{
				m_WanderTimer = getMSTime() + 1000; // wait 1 second before we try again
				return;
			}

			m_Unit->SetOrientation(wanderO);
			MoveTo(wanderX, wanderY, wanderZ, wanderO);
			m_WanderTimer = getMSTime() + m_totalMoveTime + 300; // time till next move (+ pause)
		}
	}

	//Unit Follow Code
	if(UnitToFollow != NULL)
	{
#if !defined(WIN32) && !defined(HACKY_CRASH_FIXES)
		if( UnitToFollow->event_GetCurrentInstanceId() != m_Unit->event_GetCurrentInstanceId() || !UnitToFollow->IsInWorld() )
			UnitToFollow = NULL;
		else
		{
#else
		__try
		{
			if( UnitToFollow->event_GetCurrentInstanceId() != m_Unit->event_GetCurrentInstanceId() || !UnitToFollow->IsInWorld() )
				UnitToFollow = NULL;
			else
			{
#endif
				if(m_AIState == STATE_IDLE || m_AIState == STATE_FOLLOWING)
				{
					float dist = m_Unit->GetDistanceSq(UnitToFollow);

					// re-calculate orientation based on target's movement
					if(m_lastFollowX != UnitToFollow->GetPositionX() ||
						m_lastFollowY != UnitToFollow->GetPositionY())
					{
						float dx = UnitToFollow->GetPositionX() - m_Unit->GetPositionX();
						float dy = UnitToFollow->GetPositionY() - m_Unit->GetPositionY();
						if(dy != 0.0f)
						{
							float angle = atan2(dx,dy);
							m_Unit->SetOrientation(angle);
						}
						m_lastFollowX = UnitToFollow->GetPositionX();
						m_lastFollowY = UnitToFollow->GetPositionY();
					}

					if (dist > (FollowDistance*FollowDistance)) //if out of range
					{
						m_AIState = STATE_FOLLOWING;
						
						if(dist > 25.0f) //25 yard away lets run else we will loose the them
							m_moveRun = true;
						else 
							m_moveRun = false;

						if(m_AIType == AITYPE_PET || UnitToFollow == m_formationLinkTarget) //Unit is Pet/formation
						{
							if(dist > 900.0f/*30*/)
								m_moveSprint = true;

							float delta_x = UnitToFollow->GetPositionX();
							float delta_y = UnitToFollow->GetPositionY();
							float d = 3;
							if(m_formationLinkTarget)
								d = m_formationFollowDistance;

							MoveTo(delta_x+(d*(cosf(m_fallowAngle+UnitToFollow->GetOrientation()))),
								delta_y+(d*(sinf(m_fallowAngle+UnitToFollow->GetOrientation()))),
								UnitToFollow->GetPositionZ(),UnitToFollow->GetOrientation());				
						}
						else
						{
							_CalcDestinationAndMove(UnitToFollow, FollowDistance);
						}
					}
				}
			}
#if defined(WIN32) && defined(HACKY_CRASH_FIXES)
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			UnitToFollow = NULL;
		}
#endif
	}
}

void AIInterface::CastSpell(Unit* caster, SpellEntry *spellInfo, SpellCastTargets targets)
{
	if( m_AIType != AITYPE_PET && disable_spell )
		return;

	// Stop movement while casting.
	m_AIState = STATE_CASTING;
#ifdef _AI_DEBUG
	sLog.outString("AI DEBUG: Unit %u casting spell %s on target "I64FMT, caster->GetEntry(), 
		sSpellStore.LookupString(spellInfo->Name), targets.m_unitTarget);
#endif

	//i wonder if this will lead to a memory leak :S
	Spell *nspell = SpellPool.PooledNew();
	if (!nspell)
		return;
	nspell->Init(caster, spellInfo, false, NULL);
	nspell->prepare(&targets);
}

SpellEntry *AIInterface::getSpellEntry(uint32 spellId)
{
	SpellEntry *spellInfo = dbcSpell.LookupEntry(spellId );

	if(!spellInfo)
	{
		sLog.outError("WORLD: unknown spell id %i\n", spellId);
		return NULL;
	}

	return spellInfo;
}

SpellCastTargets AIInterface::setSpellTargets(SpellEntry *spellInfo, Unit* target)
{
	SpellCastTargets targets;
	targets.m_unitTarget = target ? target->GetGUID() : 0;
	targets.m_itemTarget = 0;
	targets.m_srcX = targets.m_destX = m_Unit->GetPositionX();
	targets.m_srcY = targets.m_destY = m_Unit->GetPositionY();
	targets.m_srcZ = targets.m_destZ = m_Unit->GetPositionZ();

	if(m_nextSpell->spelltargetType == TTYPE_SINGLETARGET)
	{
		targets.m_targetMask = 2;
	}
	else if(m_nextSpell->spelltargetType == TTYPE_SOURCE)
	{
		targets.m_targetMask = 32;
//		targets.m_srcX = m_Unit->GetPositionX();
//		targets.m_srcY = m_Unit->GetPositionY();
//		targets.m_srcZ = m_Unit->GetPositionZ();
	}
	else if(m_nextSpell->spelltargetType == TTYPE_DESTINATION)
	{
		targets.m_targetMask = 64;
		targets.m_destX = target->GetPositionX();
		targets.m_destY = target->GetPositionY();
		targets.m_destZ = target->GetPositionZ();
	}
	else if(m_nextSpell->spelltargetType == TTYPE_CASTER)
	{
		targets.m_targetMask = 2;
		targets.m_unitTarget = m_Unit->GetGUID();
	}

	return targets;
}

AI_Spell *AIInterface::getSpell()
{
	if(next_spell_time > (uint32)UNIXTIME)
		return NULL;

	waiting_for_cooldown = false;

	// look at our spells
	AI_Spell *  sp = NULL;
	AI_Spell *  def_spell = NULL;
	uint32 cool_time=0;
	uint32 cool_time2;
	uint32 nowtime = getMSTime();

	if(m_Unit->IsPet())
	{
		sp = def_spell = ((Pet*)m_Unit)->HandleAutoCastEvent();
	}
	else
	{
		for(list<AI_Spell*>::iterator itr = m_spells.begin(); itr != m_spells.end();)
		{
			sp = *itr;
			++itr;
			if(	
//				sp->cooldowntime && //Zack : no need for this double check here
				nowtime < sp->cooldowntime 
//				&& sp->procChance >= 100 //Zack: why was this put here ? It makes mobs spam spells like no tomorrow
				)
			{
				cool_time2=sp->cooldowntime-nowtime;
				if(!cool_time || cool_time2<cool_time)
					cool_time=cool_time2;

				waiting_for_cooldown = true;
				continue;
			}

			if(sp->procCount && sp->procCounter >= sp->procCount)
				continue;

			if(sp->agent == AGENT_SPELL)
			{
				if (sp->spellType == STYPE_BUFF)
				{
					// cast the buff at requested percent only if we don't have it already
					if(sp->procChance >= 100 || Rand(sp->procChance))
					{
						if(!m_Unit->HasBuff(sp->spell->Id))
						{
							return sp;
						}
					}
				}
				else
				{
					if(def_spell!=0)
						continue;

					// cast the spell at requested percent.
					if(sp->procChance >= 100 || Rand(sp->procChance))
					{
						//focus/mana requirement
						switch(sp->spell->powerType)
						{
						case POWER_TYPE_MANA:
							if(m_Unit->GetUInt32Value(UNIT_FIELD_POWER1) < sp->spell->manaCost)
								continue;
							break;

						case POWER_TYPE_FOCUS:
							if(m_Unit->GetUInt32Value(UNIT_FIELD_POWER3) < sp->spell->manaCost)
								continue;
							break;
						}
						def_spell = sp;
						//we got a selected spell, we can exit loop now
						break;
					}
					else //we failed casting it due to given chance, we activate false cooldown on it to not spam searching this list
					{
//						sp->cooldowntime = nowtime + sp->cooldown / ( sp->procChance + 1 ); 
						cool_time2 = 2000;
						sp->cooldowntime = nowtime + cool_time2; 
						if( !cool_time || cool_time2 < cool_time )
							cool_time = cool_time2;
					}
				}
			}
		}
	}

	if(def_spell)
	{
		// set cooldown
		if(def_spell->procCount)
			def_spell->procCounter++;

		if(def_spell->cooldown)
			def_spell->cooldowntime = nowtime + def_spell->cooldown;

		waiting_for_cooldown = false;
		return def_spell;
	}

	// save some loops if waiting for cooldownz
	if(cool_time)
	{
		cool_time2 = cool_time / 1000;
		if(cool_time2)
			next_spell_time = (uint32)UNIXTIME + cool_time2;
	}
	else
	{
		next_spell_time = (uint32)UNIXTIME + MOB_SPELLCAST_REFRESH_COOLDOWN_INTERVAL;
		waiting_for_cooldown = false;
	}

#ifdef _AI_DEBUG
	sLog.outString("AI DEBUG: Returning no spell for unit %u", m_Unit->GetEntry());
#endif
	return 0;
}

void AIInterface::addSpellToList(AI_Spell *sp)
{
	if(!sp || !sp->spell)
		return;

	AI_Spell * sp2 = new AI_Spell;
	memcpy(sp2, sp, sizeof(AI_Spell));
	m_spells.push_back(sp2);
}

uint32 AIInterface::getThreatByGUID(uint64 guid)
{

	if(  m_Unit->GetMapMgr() == NULL )
		return 0; 

	Unit *obj = m_Unit->GetMapMgr()->GetUnit(guid);
	if(obj)
		return getThreatByPtr(obj);

	return 0;
}

uint32 AIInterface::getThreatByPtr(Unit* obj)
{
	if( !obj  || m_Unit->GetMapMgr() == NULL)
		return 0;
	TargetMap::iterator it = m_aiTargets.find(obj->GetGUID());
	if(it != m_aiTargets.end())
	{
		Unit *tempUnit = m_Unit->GetMapMgr()->GetUnit(it->first);
		if (tempUnit)
			return it->second + tempUnit->GetThreatModifyer();
		else
			return it->second;

	}
	return 0;
}

/*
#if defined(WIN32) && defined(HACKY_CRASH_FIXES)
__declspec(noinline) bool ___CheckTarget(Unit * ptr, Unit * him)
{
	__try
	{
		if( him->GetInstanceID() != ptr->GetInstanceID() || !him->isAlive() || !isAttackable( ptr, him ) )
		{
			return false;
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}
	return true;
}
#endif
*/

//should return a valid target
Unit *AIInterface::GetMostHated()
{
	if(  m_Unit->GetMapMgr() == NULL )
		return NULL; 

	Unit *ResultUnit=NULL;

	//override mosthated with taunted target. Basic combat checks are made for it. 
	//What happens if we can't see tauntedby unit ?
	ResultUnit = getTauntedBy();
	if(ResultUnit)
		return ResultUnit;

	pair<Unit*, int32> currentTarget;
	currentTarget.first = 0;
	currentTarget.second = -1;

	LockAITargets(true);

	TargetMap::iterator it2 = m_aiTargets.begin();
	TargetMap::iterator itr;
	for(; it2 != m_aiTargets.end();)
	{
		itr = it2;
		++it2;

		/* check the target is valid */
/*
#if defined(WIN32) && defined(HACKY_CRASH_FIXES)
		if(!___CheckTarget( m_Unit, itr->first ) )
		{
			if( m_nextTarget == itr->first )
				m_nextTarget = NULL;

			m_aiTargets.erase(itr);
			continue;
		}
#else
		if(itr->first->event_GetCurrentInstanceId() != m_Unit->event_GetCurrentInstanceId() || !itr->first->isAlive() || !isAttackable(m_Unit, itr->first))
		{
			m_aiTargets.erase(itr);
			continue;
		}
#endif
*/
		// this is a much slower version then the previous one but it causes a lot of crashes and that is above speed right now.
		Unit *ai_t = m_Unit->GetMapMgr()->GetUnit( itr->first );

		if( !ai_t || ai_t->GetInstanceID() != m_Unit->GetInstanceID() || !ai_t->isAlive() || !isAttackable( m_Unit, ai_t ) )
		{
			if( GetNextTarget() == ai_t )
				SetNextTarget( (Unit*)NULL );

			m_aiTargets.erase(itr);
			continue;
		}

		if((itr->second + ai_t->GetThreatModifyer()) > currentTarget.second)
		{
			/* new target */
			currentTarget.first = ai_t;
			currentTarget.second = itr->second + ai_t->GetThreatModifyer();
			m_currentHighestThreat = currentTarget.second;
		}

		/* there are no more checks needed here... the needed checks are done by CheckTarget() */
	}

	LockAITargets(false);

	return currentTarget.first;
}
Unit *AIInterface::GetSecondHated()
{
	if(  m_Unit->GetMapMgr() == NULL )
		return NULL; 

	Unit *ResultUnit=GetMostHated();

	pair<Unit*, int32> currentTarget;
	currentTarget.first = 0;
	currentTarget.second = -1;

	LockAITargets(true);

	TargetMap::iterator it2 = m_aiTargets.begin();
	TargetMap::iterator itr;
	for(; it2 != m_aiTargets.end();)
	{
		itr = it2;
		++it2;

		/* check the target is valid */
		Unit *ai_t = m_Unit->GetMapMgr()->GetUnit( itr->first );
		if(!ai_t || ai_t->GetInstanceID() != m_Unit->GetInstanceID() || !ai_t->isAlive() || !isAttackable(m_Unit, ai_t))
		{
			m_aiTargets.erase(itr);
			continue;
		}

		if((itr->second + ai_t->GetThreatModifyer()) > currentTarget.second &&
			ai_t != ResultUnit)
		{
			/* new target */
			currentTarget.first = ai_t;
			currentTarget.second = itr->second + ai_t->GetThreatModifyer();
			m_currentHighestThreat = currentTarget.second;
		}
	}

	LockAITargets(false);

	return currentTarget.first;
}
bool AIInterface::modThreatByGUID(uint64 guid, int32 mod)
{
	if (!m_aiTargets.size())
		return false;

	if(  m_Unit->GetMapMgr() == NULL )
		return false; 

	Unit *obj = m_Unit->GetMapMgr()->GetUnit(guid);
	if(obj)
		return modThreatByPtr(obj, mod);

	return false;
}

bool AIInterface::modThreatByPtr(Unit* obj, int32 mod)
{
	if(!obj)
		return false;

	LockAITargets(true);

	int32 tempthreat;
	TargetMap::iterator it = m_aiTargets.find(obj->GetGUID());
	if(it != m_aiTargets.end())
	{
		it->second += mod;
		if (it->second < 1)
			it->second = 1;

		tempthreat = it->second + obj->GetThreatModifyer();
		if (tempthreat < 1)
			tempthreat = 1;
		if(tempthreat > m_currentHighestThreat)
		{
			// new target!
			if(!isTaunted)
			{
				m_currentHighestThreat = tempthreat;
				SetNextTarget(obj);
			}
		}
	}
	else
	{
		m_aiTargets.insert( make_pair( obj->GetGUID(), mod ) );

		tempthreat = mod + obj->GetThreatModifyer();
		if (tempthreat < 1)
			tempthreat = 1;
		if(tempthreat > m_currentHighestThreat)
		{
			if(!isTaunted)
			{
				m_currentHighestThreat = tempthreat;
				SetNextTarget(obj);
			}
		}
	}

	LockAITargets(false);

	if(obj == GetNextTarget())
	{
		// check for a possible decrease in threat.
		if(mod < 0)
		{
			SetNextTarget(GetMostHated());
			//if there is no more new targets then we can walk back home ?
			if(!GetNextTarget())
				HandleEvent(EVENT_LEAVECOMBAT, m_Unit, 0);
		}
	}
	return true;
}

void AIInterface::RemoveThreatByGUID(uint64 guid)
{
	if (!m_aiTargets.size())
		return;

	if(  m_Unit->GetMapMgr() == NULL )
		return; 

	Unit *obj = m_Unit->GetMapMgr()->GetUnit(guid);
	if(obj)
		RemoveThreatByPtr(obj);
}

void AIInterface::RemoveThreatByPtr(Unit* obj)
{
	if(!obj)
		return;

	LockAITargets(true);

	TargetMap::iterator it = m_aiTargets.find(obj->GetGUID());
	if(it != m_aiTargets.end())
	{
		m_aiTargets.erase(it);
		//check if we are in combat and need a new target
		if(obj==GetNextTarget())
		{
			SetNextTarget(GetMostHated());
			//if there is no more new targets then we can walk back home ?
			if(!GetNextTarget())
				HandleEvent(EVENT_LEAVECOMBAT, m_Unit, 0);
		}
	}

	LockAITargets(false);
}

void AIInterface::addAssistTargets(Unit* Friend)
{
	// stop adding stuff that gives errors on linux!
	m_assistTargets.insert(Friend);
}

void AIInterface::WipeHateList()
{
	for(TargetMap::iterator itr = m_aiTargets.begin(); itr != m_aiTargets.end(); ++itr)
		itr->second = 0;
	m_currentHighestThreat = 0;
}
void AIInterface::ClearHateList() //without leaving combat
{
	for(TargetMap::iterator itr = m_aiTargets.begin(); itr != m_aiTargets.end(); ++itr)
		itr->second = 1;
	m_currentHighestThreat = 1;
}

void AIInterface::WipeTargetList()
{
	SetNextTarget( (Unit*)NULL );

	m_nextSpell = NULL;
	m_currentHighestThreat = 0;
	LockAITargets( true );
	m_aiTargets.clear();
	LockAITargets( false );
	m_Unit->CombatStatus.Vanished();
}

bool AIInterface::taunt(Unit* caster, bool apply)
{
	if(apply)
	{
		//wowwiki says that we cannot override this spell
		if(GetIsTaunted())
			return false;

		if(!caster)
		{
			isTaunted = false;
			return false;
		}

		//check if we can attack taunter. Maybe it's a hack or a bug if we fail this test
		if(isHostile(m_Unit, caster))
		{
			//check if we have to add him to our agro list
			//GetMostHated(); //update our most hated list/ Note that at this point we do not have a taunter yet. If we would have then this funtion will not give real mosthated
			int32 oldthreat = getThreatByPtr(caster);
			//make sure we rush the target anyway. Since we are not taunted yet, this will also set our target
			modThreatByPtr(caster,abs(m_currentHighestThreat-oldthreat)+1); //we need to be the most hated at this moment
//			SetNextTarget(caster);
		}
		isTaunted = true;
		tauntedBy = caster;
	}
	else
	{
		isTaunted = false;
		tauntedBy = NULL;
		//taunt is over, we should get a new target based on most hated list
		SetNextTarget(GetMostHated());
	}

	return true;
}

Unit* AIInterface::getTauntedBy()
{
	if(GetIsTaunted())
	{
		return tauntedBy;
	}
	else
	{
		return NULL;
	}
}

bool AIInterface::GetIsTaunted()
{
	if(isTaunted)
	{
		if(!tauntedBy || !tauntedBy->isAlive())
		{
			isTaunted = false;
			tauntedBy = NULL;
		}
	}
	return isTaunted;
}

void AIInterface::SetSoulLinkedWith(Unit* target)
{
	if (!target)
		return;
	soullinkedWith = target;
	isSoulLinked = true;
}

Unit* AIInterface::getSoullinkedWith()
{
	if(GetIsTaunted())
	{
		return soullinkedWith;
	}
	else
	{
		return NULL;
	}
}

bool AIInterface::GetIsSoulLinked()
{
	if(isSoulLinked)
	{
		if(!soullinkedWith || !soullinkedWith->isAlive())
		{
			isSoulLinked = false;
			soullinkedWith = NULL;
		}
	}
	return isSoulLinked;
}

void AIInterface::CheckTarget(Unit* target)
{
	if( target == NULL )
		return;

	if( target == UnitToFollow )		  // fix for crash here
	{
		UnitToFollow = NULL;
		m_lastFollowX = m_lastFollowY = 0;
		FollowDistance = 0;
	}

	if( target == UnitToFollow_backup )
	{
		UnitToFollow_backup = NULL;
	}

	AssistTargetSet::iterator  itr = m_assistTargets.find(target);
	if(itr != m_assistTargets.end())
		m_assistTargets.erase(itr);


	LockAITargets(true);

	TargetMap::iterator it2 = m_aiTargets.find( target->GetGUID() );
	if( it2 != m_aiTargets.end() || target == GetNextTarget() )
	{
		target->CombatStatus.RemoveAttacker( m_Unit, m_Unit->GetGUID() );
		m_Unit->CombatStatus.RemoveAttackTarget( target );

		if(it2 != m_aiTargets.end())
		{
			m_aiTargets.erase(it2);
		}

		if (target == GetNextTarget())	 // no need to cast on these.. mem addresses are still the same
		{
			SetNextTarget( (Unit*)NULL );
			m_nextSpell = NULL;

			// find the one with the next highest threat
			GetMostHated();
		}
	}

	LockAITargets(false);

	if( target->GetTypeId() == TYPEID_UNIT )
	{
		it2 = target->GetAIInterface()->m_aiTargets.find( m_Unit->GetGUID() );
		if( it2 != target->GetAIInterface()->m_aiTargets.end() )
		{
			target->GetAIInterface()->LockAITargets(true);
			target->GetAIInterface()->m_aiTargets.erase( it2 );
			target->GetAIInterface()->LockAITargets(false);
		}
        
		if( target->GetAIInterface()->GetNextTarget() == m_Unit )
		{
			target->GetAIInterface()->SetNextTarget( (Unit*)NULL );
			target->GetAIInterface()->m_nextSpell = NULL;
			target->GetAIInterface()->GetMostHated();
		}

		if( target->GetAIInterface()->UnitToFollow == m_Unit )
			target->GetAIInterface()->UnitToFollow = NULL;
	}

	if(target == UnitToFear)
		UnitToFear = NULL;

	if(tauntedBy == target)
		tauntedBy = NULL;
}

uint32 AIInterface::_CalcThreat(uint32 damage, SpellEntry * sp, Unit* Attacker)
{
	if(!Attacker)
		return 0; // No attacker means no threat and we prevent crashes this way

	if(m_Unit->m_faction != NULL && Attacker->m_faction != NULL)
		if (isSameFaction(m_Unit,Attacker))
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

void AIInterface::WipeReferences()
{
	m_nextSpell = 0;
	m_currentHighestThreat = 0;
	LockAITargets(true);
	m_aiTargets.clear();
	LockAITargets(false);
	SetNextTarget( (Unit*)NULL );
	UnitToFear = 0;
	UnitToFollow = 0;
	tauntedBy = 0;
	m_Unit->AquireInrangeLock(); //make sure to release lock before exit function !
	//Clear targettable
	for(set<Object*>::iterator itr = m_Unit->GetInRangeSetBegin(); itr != m_Unit->GetInRangeSetEnd(); ++itr)
		if( (*itr) && (*itr)->GetTypeId() == TYPEID_UNIT && static_cast<Unit*>(*itr)->GetAIInterface())
			static_cast<Unit*>(*itr)->GetAIInterface()->RemoveThreatByPtr( m_Unit );
	m_Unit->ReleaseInrangeLock();

}

void AIInterface::ResetProcCounts()
{
	for(list<AI_Spell*>::iterator itr = m_spells.begin(); itr != m_spells.end(); ++itr)
		if((*itr)->procCount)
			(*itr)->procCounter=0;
}

//we only cast once a spell and we will set his health and resistances. Note that this can be made with db too !
void AIInterface::Event_Summon_EE_totem(uint32 summon_duration)
{
	//some say it should inherit the level of the caster
	Unit *caster = m_Unit->GetMapMgr()->GetUnit( m_Unit->GetUInt64Value( UNIT_FIELD_CREATEDBY ) );
	uint32 new_level = 0;
	if( caster )
		new_level = caster->getLevel( );
	//timer should not reach this value thus not cast this spell again
	m_totemspelltimer = 0xEFFFFFFF;
	//creatures do not support PETs and the spell uses that effect so we force a summon guardian thing
	Unit *ourslave=m_Unit->create_guardian(15352,summon_duration,float(-M_PI*2), new_level );
	if(ourslave)
	{
		static_cast<Creature*>(ourslave)->ResistanceModPct[NATURE_DAMAGE]=100;//we should be immune to nature dmg. This can be also set in db
		static_cast<Creature*>(ourslave)->m_noRespawn = true;
		/*
		- Earth Stun (37982)
		- taunt
		*/
	}
}

//we only cast once a spell and we will set his health and resistances. Note that this can be made with db too !
void AIInterface::Event_Summon_FE_totem(uint32 summon_duration)
{
	//some say it should inherit the level of the caster
	Unit *caster = m_Unit->GetMapMgr()->GetUnit( m_Unit->GetUInt64Value( UNIT_FIELD_CREATEDBY ) );
	uint32 new_level = 0;
	if( caster )
		new_level = caster->getLevel( );
	//timer should not reach this value thus not cast this spell again
	m_totemspelltimer = 0xEFFFFFFF;
	//creatures do not support PETs and the spell uses that effect so we force a summon guardian thing
	Unit *ourslave=m_Unit->create_guardian(15438,summon_duration,float(-M_PI*2), new_level);
	if(ourslave)
	{
		//m_Unit->summonPet = ourslave;
		static_cast<Creature*>(ourslave)->ResistanceModPct[FIRE_DAMAGE]=100;//we should be immune to fire dmg. This can be also set in db
		static_cast<Creature*>(ourslave)->m_noRespawn = true;
		/*
		- also : select * from dbc_spell where name like "%fire blast%"
		- also : select * from dbc_spell where name like "%fire nova"
		*/
	}
}
/*
void AIInterface::CancelSpellCast()
{
	//hmm unit spell casting is not the same as Ai spell casting ? Have to test this
	if(m_Unit->IsCasting())
		m_Unit->m_currentSpell->safe_cancel();
	//i can see this crashing already :P.
	m_AIState = STATE_IDLE;
}
*/

void AIInterface::EventChangeFaction( Unit *ForceAttackersToHateThisInstead )
{
	m_nextSpell = 0;
	m_currentHighestThreat = 0;
	//we need a new hatred list
	LockAITargets(true);
	m_aiTargets.clear();
	LockAITargets(false);
	//we need a new assist list
	m_assistTargets.clear();
	//Clear targettable
	if( ForceAttackersToHateThisInstead == NULL )
	{
		m_Unit->AquireInrangeLock(); //make sure to release lock before exit function !
		for(set<Object*>::iterator itr = m_Unit->GetInRangeSetBegin(); itr != m_Unit->GetInRangeSetEnd(); ++itr)
			if( (*itr) && (*itr)->GetTypeId() == TYPEID_UNIT && static_cast<Unit*>(*itr)->GetAIInterface() )
				static_cast<Unit*>(*itr)->GetAIInterface()->RemoveThreatByPtr( m_Unit );
		m_Unit->ReleaseInrangeLock();
		SetNextTarget( (Unit*)NULL );
	}
	else
	{
		m_Unit->AquireInrangeLock(); //make sure to release lock before exit function !
		for(set<Object*>::iterator itr = m_Unit->GetInRangeSetBegin(); itr != m_Unit->GetInRangeSetEnd(); ++itr)
			if( (*itr) && (*itr)->GetTypeId() == TYPEID_UNIT && static_cast<Unit*>(*itr)->GetAIInterface() 
				&& static_cast<Unit*>(*itr)->GetAIInterface()->getThreatByPtr( m_Unit ) )//this guy will join me in fight since I'm telling him "sorry i was controlled"
			{
				static_cast<Unit*>(*itr)->GetAIInterface()->modThreatByPtr( ForceAttackersToHateThisInstead, 10 ); //just aping to be bale to hate him in case we got nothing else
				static_cast<Unit*>(*itr)->GetAIInterface()->RemoveThreatByPtr( m_Unit );
			}
		m_Unit->ReleaseInrangeLock();
		modThreatByPtr( ForceAttackersToHateThisInstead, 1 );
		SetNextTarget( ForceAttackersToHateThisInstead );
	}
}

bool isGuard(uint32 id)
{
	switch(id)
	{
		/* stormwind guards */
	case 68:
	case 1423:
	case 1756:
	case 15858:
	case 15859:
	case 16864:
	case 20556:
	case 18948:
	case 18949:
	case 1642:
		/* ogrimmar guards */
	case 3296:
	case 15852:
	case 15853:
	case 15854:
	case 18950:
		/* undercity guards */
	case 5624:
	case 18971:
	case 16432:
		/* exodar */
	case 16733:
	case 18815:
		/* thunder bluff */
	case 3084:
		/* silvermoon */
	case 16221:
	case 17029:
	case 16222:
		/* ironforge */
	case 727:
	case 5595:
	case 12996:
		/* darnassus? */
		{
			return true;
		}break;
	}
	return false;
}

bool isNeutralGuard(uint32 id)
{
	switch(id)
	{
		// Ratchet
	case 3502:
		// Booty Bay
	case 4624:
	case 15088:
		// Gadgetzan
	case 9460:
		// Argent Dawn
	case 11102:
	case 16378:
		// Cenarion Hold
	case 15184:
		// Moonglade
	case 11822:
		// Everlook
	case 11190:
		// Cenarion Refuge
	case 17855:
		// Throne of the elements
	case 18099:
	case 18101:
	case 18102:
		// Area 52
	case 20484:
	case 20485:
		// Cosmowrench
	case 22494:
		// Mudsprocket
	case 23636:
		// Concert Bruiser
	case 23721:
		// Shattered Sun
	case 26253:
	case 24994:
			return true;
		break;
	}

	return false;
}

void AIInterface::WipeCurrentTarget()
{
	if( GetNextTarget() )
	{
		LockAITargets( true );
		TargetMap::iterator itr = m_aiTargets.find( GetNextTarget()->GetGUID() );
		if( itr != m_aiTargets.end() )
			m_aiTargets.erase( itr );
		LockAITargets( false );
	}

	SetNextTarget( (Unit*)NULL );

	if( GetNextTarget() == UnitToFollow )
		UnitToFollow = NULL;

	if( GetNextTarget() == UnitToFollow_backup )
		UnitToFollow_backup = NULL;
}

#ifdef HACKY_CRASH_FIXES

bool AIInterface::CheckCurrentTarget()
{
	//in case target was removed from map since our last check on him
	if( GetNextTarget() == NULL )
	{
		WipeCurrentTarget();
		return false;
	}
	
	bool cansee = false;
	if( GetNextTarget()->GetInstanceID() == m_Unit->GetInstanceID())
	{
		if( m_Unit->GetTypeId() == TYPEID_UNIT )
			cansee = static_cast< Creature* >( m_Unit )->CanSee( GetNextTarget() );
		else
			cansee = static_cast< Player* >( m_Unit )->CanSee( GetNextTarget() );
	}
	else 
	{
		WipeCurrentTarget();
	}

	return cansee;
}

bool AIInterface::TargetUpdateCheck(Unit * ptr)
{
	__try
	{
		if( ptr->event_GetCurrentInstanceId() != m_Unit->event_GetCurrentInstanceId() ||
			!ptr->isAlive() || m_Unit->GetDistanceSq(ptr) >= 6400.0f )
		{
			return false;
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}

	return true;
}

#endif
Unit* AIInterface::GetNextTarget()
{
	if (m_nextTarget && m_Unit && m_Unit->GetMapMgr()) return m_Unit->GetMapMgr()->GetUnit(m_nextTarget);
	return NULL;
}

void AIInterface::SetNextTarget (Unit *nextTarget) {
	if (nextTarget)
		SetNextTarget(nextTarget->GetGUID());
	else
		SetNextTarget((uint64)NULL);
}

void AIInterface::SetNextTarget (uint64 nextTarget) 
{
	m_nextTarget = nextTarget; 
	m_Unit->SetUInt64Value(UNIT_FIELD_TARGET, m_nextTarget);
	if(nextTarget)
	{
#ifdef ENABLE_GRACEFULL_HIT
		have_graceful_hit=false;
#endif
	}
}
