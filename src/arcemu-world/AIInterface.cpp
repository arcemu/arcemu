/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2012 <http://www.ArcEmu.org/>
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

AIInterface::AIInterface()
	:
	m_canMove(true),
	m_WayPointsShowing(false),
	m_WayPointsShowBackwards(false),
	m_currentWaypoint(0),
	m_moveBackward(false),
	m_moveType(0),
	onGameobject(false),
	m_creatureState(STOPPED),
	m_canFlee(false),
	m_canCallForHelp(false),
	m_canRangedAttack(false),
	m_FleeHealth(0.0f),
	m_FleeDuration(0),
	m_CallForHelpHealth(0.0f),
	m_totemspelltimer(0),
	m_totemspelltime(0),
	totemspell(NULL),
	m_totalMoveTime(0),
	m_formationLinkTarget(0),
	m_formationFollowDistance(0.0f),
	m_formationFollowAngle(0.0f),
	m_formationLinkSqlId(0),
	timed_emotes(NULL),
	disable_combat(false),
	disable_melee(false),
	disable_ranged(false),
	disable_spell(false),
	disable_targeting(false),
	waiting_for_cooldown(false),
	next_spell_time(0),
	m_isGuard(false),
	m_isNeutralGuard(false),
	m_AllowedToEnterCombat(true),
	m_updateAssist(false),
	m_updateTargets(false),
	m_updateAssistTimer(1),
	m_updateTargetsTimer(TARGET_UPDATE_INTERVAL_ON_PLAYER),
	m_updateTargetsTimer2(0),
	m_nextSpell(NULL),
	m_nextTarget(0),
	m_fleeTimer(0),
	m_hasFleed(false),
	m_hasCalledForHelp(false),
	m_outOfCombatRange(50 * 50), // Where did u get this value?
	m_Unit(NULL),
	m_PetOwner(NULL),
	FollowDistance(0.0f),
	m_fallowAngle(M_PI_FLOAT / 2),
	m_AIState(STATE_IDLE),
	m_aiCurrentAgent(AGENT_NULL),
	tauntedBy(NULL),
	isTaunted(false),
	soullinkedWith(NULL),
	isSoulLinked(false),
	m_runSpeed(0.0f),
	m_flySpeed(0.0f),
	m_last_target_x(0),
	m_last_target_y(0),
	m_currentSplineUpdateCounter(0),
	m_currentMoveSplineIndex(0xFFFFFFFF),
	m_currentSplineFinalOrientation(0),
	m_splinePriority(SPLINE_PRIORITY_MOVEMENT),
	m_splineFlags(SPLINEFLAG_WALKMODE),
	m_returnX(0),
	m_returnY(0),
	m_returnZ(0),
	m_combatResetX(0),
	m_combatResetY(0),
	m_combatResetZ(0),
	m_lastFollowX(0),
	m_lastFollowY(0),
	m_UnitToFollow(0),
	m_UnitToFollow_backup(0),
	m_UnitToFear(0),
	m_timeToMove(0),
	m_timeMoved(0),
	m_moveTimer(0),
	m_FearTimer(0),
	m_WanderTimer(0),
	m_MovementState(MOVEMENTSTATE_STOP),
	m_currentHighestThreat(0),
	timed_emote_expire(0xFFFFFFFF),
	m_waypointsLoadedFromDB(false),
	m_waypoints(NULL),
	m_is_in_instance(false),
	skip_reset_hp(false)
{
	m_aiTargets.clear();
	m_assistTargets.clear();
	m_spells.clear();
}

void AIInterface::EventAiInterfaceParamsetFinish()
{
	if(timed_emotes && timed_emotes->begin() != timed_emotes->end())
	{
		next_timed_emote = timed_emotes->begin();
		timed_emote_expire = (*next_timed_emote)->expire_after;
	}
}

void AIInterface::Init(Unit* un, AIType at, MovementType mt)
{
	ARCEMU_ASSERT(at != AITYPE_PET);

	m_AIType = at;
	m_MovementType = mt;

	m_AIState = STATE_IDLE;
	m_MovementState = MOVEMENTSTATE_STOP;

	m_Unit = un;

	m_walkSpeed = m_Unit->m_walkSpeed * 0.001f; //move distance per ms time
	m_runSpeed = m_Unit->m_runSpeed * 0.001f; //move distance per ms time
	m_flySpeed = m_Unit->m_flySpeed * 0.001f;
	/*if(!m_DefaultMeleeSpell)
	{
		m_DefaultMeleeSpell = new AI_Spell;
		m_DefaultMeleeSpell->entryId = 0;
		m_DefaultMeleeSpell->spellType = 0;
		m_DefaultMeleeSpell->agent = AGENT_MELEE;
		m_DefaultSpell = m_DefaultMeleeSpell;
	}*/
	m_guardTimer = getMSTime();
}

AIInterface::~AIInterface()
{
	for(list<AI_Spell*>::iterator itr = m_spells.begin(); itr != m_spells.end(); ++itr)
		delete(*itr);
	m_spells.clear();

	deleteWaypoints();
}

void AIInterface::Init(Unit* un, AIType at, MovementType mt, Unit* owner)
{
	ARCEMU_ASSERT(at == AITYPE_PET || at == AITYPE_TOTEM);

	m_AIType = at;
	m_MovementType = mt;

	m_AIState = STATE_IDLE;
	m_MovementState = MOVEMENTSTATE_STOP;

	m_Unit = un;
	m_PetOwner = owner;

	m_walkSpeed = m_Unit->m_walkSpeed * 0.001f; //move distance per ms time
	m_runSpeed = m_Unit->m_runSpeed * 0.001f; //move/ms
	m_flySpeed = m_Unit->m_flySpeed * 0.001f;
}

void AIInterface::HandleEvent(uint32 event, Unit* pUnit, uint32 misc1)
{
	if(m_Unit == NULL) return;

	// Passive NPCs (like target dummies) shouldn't do anything.
	if(m_AIType == AITYPE_PASSIVE)
		return;

	if(event < NUM_AI_EVENTS && AIEventHandlers[event] != NULL)
		(*this.*AIEventHandlers[event])(pUnit, misc1);
}

void AIInterface::Update(uint32 p_time)
{
	float tdist;
	if(m_AIType == AITYPE_TOTEM)
	{
		_UpdateTotem(p_time);
		return;
	}

	_UpdateTimer(p_time);
	_UpdateTargets();

	if(m_Unit->isAlive() && m_AIState != STATE_IDLE
	        && m_AIState != STATE_FOLLOWING && m_AIState != STATE_FEAR
	        && m_AIState != STATE_WANDER && m_AIState != STATE_SCRIPTMOVE)
	{
		if(m_AIType == AITYPE_PET)
		{
			if(!m_Unit->bInvincible && m_Unit->IsPet())
			{
				Pet* pPet = TO< Pet* >(m_Unit);

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

	UpdateMovementSpline();
	_UpdateMovement(p_time);

	if(m_AIState == STATE_EVADE)
	{
		tdist = m_Unit->GetDistanceSq(m_returnX, m_returnY, m_returnZ);
		if(tdist <= 4.0f)
		{
			m_AIState = STATE_IDLE;
			m_returnX = m_returnY = m_returnZ = 0.0f;
			m_combatResetX = m_combatResetY = m_combatResetZ = 0.0f;
			SetWalk();

			if(m_AIType != AITYPE_PET && !skip_reset_hp)
				m_Unit->SetHealth(m_Unit->GetMaxHealth());
		}
		else
		{
			if(m_creatureState == STOPPED)
			{
				// return to the home
				if(m_returnX == 0.0f && m_returnY == 0.0f)
				{
					SetReturnPosition();

				}

				MoveEvadeReturn();
			}
		}
	}

	if(m_fleeTimer)
	{
		if(m_fleeTimer > p_time)
		{
			m_fleeTimer -= p_time;
			_CalcDestinationAndMove(getNextTarget(), 5.0f);
		}
		else
		{
			m_fleeTimer = 0;
			setNextTarget(FindTargetForSpell(m_nextSpell));
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

	if(!getNextTarget() && !m_fleeTimer && m_creatureState == STOPPED && m_AIState == STATE_IDLE && m_Unit->isAlive())
	{
		if(timed_emote_expire <= p_time)    // note that creature might go idle and p_time might get big next time ...We do not skip emotes because of lost time
		{
			if((*next_timed_emote)->type == 1)   //standstate
			{
				m_Unit->SetStandState(static_cast<uint8>((*next_timed_emote)->value));
				m_Unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
			}
			else if((*next_timed_emote)->type == 2)   //emotestate
			{
				m_Unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, (*next_timed_emote)->value);
				m_Unit->SetStandState(0);
			}
			else if((*next_timed_emote)->type == 3)   //oneshot emote
			{
				m_Unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
				m_Unit->SetStandState(0);
				m_Unit->Emote((EmoteType)(*next_timed_emote)->value);           // Animation
			}
			if((*next_timed_emote)->msg)
				m_Unit->SendChatMessage((*next_timed_emote)->msg_type, (*next_timed_emote)->msg_lang, (*next_timed_emote)->msg);

			timed_emote_expire = (*next_timed_emote)->expire_after; //should we keep lost time ? I think not
			++next_timed_emote;
			if(next_timed_emote == timed_emotes->end())
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
	}
	else
	{
		m_updateAssist = true;
		m_updateAssistTimer = TARGET_UPDATE_INTERVAL_ON_PLAYER * 2 - m_updateAssistTimer - p_time;
	}

	if(m_updateTargetsTimer > p_time)
	{
		m_updateTargetsTimer -= p_time;
	}
	else
	{
		m_updateTargets = true;
		m_updateTargetsTimer = TARGET_UPDATE_INTERVAL_ON_PLAYER * 2 - m_updateTargetsTimer - p_time;
	}
}

void AIInterface::_UpdateTargets()
{
	if(m_Unit->IsPlayer() || (m_AIType != AITYPE_PET && disable_targeting))
		return;
	if(TO_CREATURE(m_Unit)->GetCreatureInfo()->Type == UNIT_TYPE_CRITTER && TO_CREATURE(m_Unit)->GetType() != CREATURE_TYPE_GUARDIAN)
		return;

	if(m_Unit->GetMapMgr() == NULL)
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

	if(m_updateAssist)
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
			        !(*i2)->isAlive() || m_Unit->GetDistanceSq((*i2)) >= 2500.0f || !(*i2)->CombatStatus.IsInCombat() || !((*i2)->m_phase & m_Unit->m_phase))
			{
				m_assistTargets.erase(i2);
			}
		}
	}

	if(m_updateTargets)
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

			Unit* ai_t = m_Unit->GetMapMgr()->GetUnit(it2->first);
			if(ai_t == NULL)
			{
				m_aiTargets.erase(it2);
			}
			else
			{
				bool instance = false;
				if(m_Unit->GetMapMgr() && m_Unit->GetMapMgr()->GetMapInfo())
				{
					switch(m_Unit->GetMapMgr()->GetMapInfo()->type)
					{
						case INSTANCE_RAID:
						case INSTANCE_NONRAID:
						case INSTANCE_MULTIMODE:
							instance = true;
							break;
					}
				}

				if(ai_t->event_GetCurrentInstanceId() != m_Unit->event_GetCurrentInstanceId() || !ai_t->isAlive() || ((!instance && m_Unit->GetDistanceSq(ai_t) >= 6400.0f) || !(ai_t->m_phase & m_Unit->m_phase)))
				{
					m_aiTargets.erase(it2);
				}
			}
		}

		LockAITargets(false);

		if( disable_combat )
			return;

		if(m_aiTargets.size() == 0
		        && m_AIState != STATE_IDLE && m_AIState != STATE_FOLLOWING
		        && m_AIState != STATE_EVADE && m_AIState != STATE_FEAR
		        && m_AIState != STATE_WANDER && m_AIState != STATE_SCRIPTIDLE)
		{
			if(m_Unit->GetMapMgr() && m_Unit->GetMapMgr()->GetMapInfo())
			{
				Unit* target = NULL;
				switch(m_Unit->GetMapMgr()->GetMapInfo()->type)
				{
					case INSTANCE_RAID:
					case INSTANCE_NONRAID:
					case INSTANCE_MULTIMODE:
						target = FindTarget();
						break;

					default:
						if(m_outOfCombatRange && _CalcDistanceFromHome() < m_outOfCombatRange)
							target = FindTarget();
						break;
				}

				if(target != NULL)
					AttackReaction(target, 1, 0);
			}
		}
		else if(m_aiTargets.size() == 0 && ((m_AIType == AITYPE_PET && (m_Unit->IsPet() && TO< Pet* >(m_Unit)->GetPetState() == PET_STATE_AGGRESSIVE)) || (!m_Unit->IsPet() && disable_melee == false)))
		{
			Unit* target = FindTarget();
			if(target)
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
	if(m_AIType != AITYPE_PET && disable_combat)
		return;

	//just make sure we are not hitting self.
	// This was reported as an exploit.Should never occur anyway
	if(getNextTarget() == m_Unit)
		setNextTarget(GetMostHated());

	uint16 agent = static_cast<uint16>(m_aiCurrentAgent);

	// If creature is very far from spawn point return to spawnpoint
	// If at instance don't return -- this is wrong ... instance creatures always returns to spawnpoint, dunno how do you got this idea.
	// If at instance returns to spawnpoint after empty agrolist
	Unit* nextTarget = getNextTarget();
	if(m_AIType != AITYPE_PET
	        && m_AIState != STATE_EVADE
	        && m_AIState != STATE_SCRIPTMOVE
	        && !m_is_in_instance
	        && (m_outOfCombatRange && m_Unit->GetDistanceSq(m_combatResetX, m_combatResetY, m_combatResetZ) > m_outOfCombatRange))
	{
		HandleEvent(EVENT_LEAVECOMBAT, m_Unit, 0);
	}
	else if(nextTarget == NULL && m_AIState != STATE_FOLLOWING && m_AIState != STATE_SCRIPTMOVE)
	{
//		SetNextTarget(FindTargetForSpell(m_nextSpell));
		if(m_is_in_instance)
			setNextTarget(FindTarget());
		else
			setNextTarget(GetMostHated());

		if(getNextTarget() == NULL)
		{
			HandleEvent(EVENT_LEAVECOMBAT, m_Unit, 0);
		}
	}
	else if(nextTarget != NULL && !(nextTarget->m_phase & m_Unit->m_phase))     // the target or we changed phase, stop attacking
	{
		if(m_is_in_instance)
			setNextTarget(FindTarget());
		else
			setNextTarget(GetMostHated());
		if(getNextTarget() == NULL)
		{
			HandleEvent(EVENT_LEAVECOMBAT, m_Unit, 0);
		}
	}

	if(sWorld.Collision)
	{

		if(m_Unit->GetMapMgr() != NULL && getNextTarget() != NULL)
		{
			if(!Flying())
			{
				float target_land_z = m_Unit->GetMapMgr()->GetLandHeight(getNextTarget()->GetPositionX(), getNextTarget()->GetPositionY(), getNextTarget()->GetPositionZ());

				if(fabs(getNextTarget()->GetPositionZ() - target_land_z) > _CalcCombatRange(getNextTarget(), false))
				{
					if(!getNextTarget()->IsPlayer())
					{
						if(target_land_z > m_Unit->GetMapMgr()->GetLiquidHeight(getNextTarget()->GetPositionX(), getNextTarget()->GetPositionY()))
							HandleEvent(EVENT_LEAVECOMBAT, m_Unit, 0);  //bugged npcs, probably db fault
					}
					else if(TO< Player* >(getNextTarget())->GetSession() != NULL)
					{
						MovementInfo* mi = TO_PLAYER(getNextTarget())->GetSession()->GetMovementInfo();

						if(mi->flags & MOVEFLAG_AIR_SWIMMING)
							HandleEvent(EVENT_LEAVECOMBAT, m_Unit, 0);
					}
				}
			}
		}
	}

	if(getNextTarget() != NULL && getNextTarget()->IsCreature() && m_AIState == STATE_EVADE)
		HandleEvent(EVENT_LEAVECOMBAT, m_Unit, 0);

	bool cansee = false;
	if(getNextTarget())
	{
		if(getNextTarget()->event_GetCurrentInstanceId() == m_Unit->event_GetCurrentInstanceId())
		{
			if(m_Unit->IsCreature())
				cansee = TO< Creature* >(m_Unit)->CanSee(getNextTarget());
			else
				cansee = TO< Player* >(m_Unit)->CanSee(getNextTarget());
		}
		else
		{
			resetNextTarget();
		}
	}

	if(cansee && getNextTarget() && getNextTarget()->isAlive() && m_AIState != STATE_EVADE && !m_Unit->IsCasting())
	{
		if(agent == AGENT_NULL || (m_AIType == AITYPE_PET && !m_nextSpell))     // allow pets autocast
		{
			if(!m_nextSpell)
				m_nextSpell = this->getSpell();

			/*
			if(!m_nextSpell && waiting_for_cooldown)
			{
				// don't start running to the target for melee if we're waiting for a cooldown.
				return;
			}
			*/

			if(m_canFlee && !m_hasFleed
			        && ((m_Unit->GetHealth() / m_Unit->GetMaxHealth()) < m_FleeHealth))
				agent = AGENT_FLEE;
			else if(m_canCallForHelp
			        && !m_hasCalledForHelp
			        /*&& (m_CallForHelpHealth > (m_Unit->GetHealth() / (m_Unit->GetMaxHealth() > 0 ? m_Unit->GetMaxHealth() : 1)))*/)
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
				if(getNextTarget()->IsPlayer())
				{
					float dist = m_Unit->GetDistanceSq(getNextTarget());
					if(TO< Player* >(getNextTarget())->m_currentMovement == MOVE_ROOT || dist >= 64.0f)
					{
						agent =  AGENT_RANGED;
					}
				}
				else if(getNextTarget()->m_canMove == false || m_Unit->GetDistanceSq(getNextTarget()) >= 64.0f)
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
					float distance = m_Unit->CalcDistance(getNextTarget());

					combatReach[0] = getNextTarget()->GetModelHalfSize();
					combatReach[1] = _CalcCombatRange(getNextTarget(), false);

					if((
//					distance >= combatReach[0] &&  //removed by Zack. You can create an exploit with this that creature will never attack
					            distance <= combatReach[1] + DISTANCE_TO_SMALL_TO_WALK)
//					|| gracefull_hit_on_target == GetNextTarget()
					  ) // Target is in Range -> Attack
					{
//					gracefull_hit_on_target = NULL;
//					m_moveRun = false;
						//FIX ME: offhand shit
						if(m_Unit->isAttackReady(false) && !m_fleeTimer)
						{
							m_creatureState = ATTACKING;
							bool infront = m_Unit->isInFront(getNextTarget());

							if(!infront) // set InFront
							{
								//prevent mob from rotating while stunned
								if(!m_Unit->IsStunned())
								{
									setInFront(getNextTarget());
									infront = true;
								}
							}
							if(infront)
							{
								m_Unit->setAttackTimer(0, false);
#ifdef ENABLE_CREATURE_DAZE
								//we require to know if strike was successful. If there was no dmg then target cannot be dazed by it
								Unit* t_unit = getNextTarget();
								if(!t_unit)
									return; //omg lol, in seconds we lost target. This might be possible due to the Eventrelocated
								uint32 health_before_strike = t_unit->GetHealth();
#endif
								if( m_Unit->GetOnMeleeSpell() != 0 ){
									m_Unit->CastOnMeleeSpell();
								}else
									m_Unit->Strike(getNextTarget(), (agent == AGENT_MELEE ? MELEE : RANGED), NULL, 0, 0, 0, false, false);

#ifdef ENABLE_CREATURE_DAZE
								//now if the target is facing his back to us then we could just cast dazed on him :P
								//as far as i know dazed is casted by most of the creatures but feel free to remove this code if you think otherwise
								if(getNextTarget() && m_Unit->m_factionDBC &&
								        !(m_Unit->m_factionDBC->RepListId == -1 && m_Unit->m_faction->FriendlyMask == 0 && m_Unit->m_faction->HostileMask == 0) /* neutral creature */
								        && getNextTarget()->IsPlayer() && !m_Unit->IsPet() && health_before_strike > getNextTarget()->GetHealth()
								        && Rand(m_Unit->get_chance_to_daze(getNextTarget())))
								{
									float our_facing = m_Unit->calcRadAngle(m_Unit->GetPositionX(), m_Unit->GetPositionY(), getNextTarget()->GetPositionX(), getNextTarget()->GetPositionY());
									float his_facing = getNextTarget()->GetOrientation();
									if(fabs(our_facing - his_facing) < CREATURE_DAZE_TRIGGER_ANGLE && !getNextTarget()->HasAura(CREATURE_SPELL_TO_DAZE))
									{
										SpellEntry* info = dbcSpell.LookupEntry(CREATURE_SPELL_TO_DAZE);
										Spell* sp = sSpellFactoryMgr.NewSpell(m_Unit, info, false, NULL);
										SpellCastTargets targets;
										targets.m_unitTarget = getNextTarget()->GetGUID();
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
//					float dist = combatReach[1] - m_Unit->GetCombatReach(); //ignore our combat reach, make sure target (player) can reach us first.

						//practical tests show that we really should try to jump on target to get good results :S
						//simply ignore combat reach and move as close as visually not annoying
						float dist;
						if(m_Unit->GetModelHalfSize() > getNextTarget()->GetModelHalfSize())
							dist = m_Unit->GetModelHalfSize();
						else
							dist = getNextTarget()->GetModelHalfSize();

						//removed by Zack. You can create an exploit with this that creature will never attack
//					if (distance<combatReach[0]) //if we are inside one each other
//						dist = -(combatReach[1] - distance);
//					gracefull_hit_on_target = GetNextTarget(); // this is an exploit where you manage to move the exact speed that mob will reposition itself all the time

						SetRun();
						_CalcDestinationAndMove(getNextTarget(), dist);
					}
				}
				break;
			case AGENT_RANGED:
				{
					float combatReach[2]; // Calculate Combat Reach
					float distance = m_Unit->CalcDistance(getNextTarget());

					combatReach[0] = 8.0f;
					combatReach[1] = 30.0f;

					if(distance >= combatReach[0] && distance <= combatReach[1]) // Target is in Range -> Attack
					{
//					m_moveRun = false;
						//FIX ME: offhand shit
						if(m_Unit->isAttackReady(false) && !m_fleeTimer)
						{
							m_creatureState = ATTACKING;
							bool infront = m_Unit->isInFront(getNextTarget());

							if(!infront) // set InFront
							{
								//prevent mob from rotating while stunned
								if(!m_Unit->IsStunned())
								{
									setInFront(getNextTarget());
									infront = true;
								}
							}

							if(infront)
							{
								m_Unit->setAttackTimer(0, false);
								SpellEntry* info = dbcSpell.LookupEntryForced(SPELL_RANGED_GENERAL);
								if(info)
								{
									Spell* sp = sSpellFactoryMgr.NewSpell(m_Unit, info, false, NULL);
									SpellCastTargets targets;
									targets.m_unitTarget = getNextTarget()->GetGUID();
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

						SetRun();
						_CalcDestinationAndMove(getNextTarget(), dist);
					}
				}
				break;
			case AGENT_SPELL:
				{
					if(!m_nextSpell || !getNextTarget())
						return;  // this shouldn't happen

					SpellCastTime* sd = dbcSpellCastTime.LookupEntry(m_nextSpell->spell->CastingTimeIndex);

					float distance = m_Unit->CalcDistance(getNextTarget());
					bool los = true;

					if(sWorld.Collision)
					{
						los = CollideInterface.CheckLOS(m_Unit->GetMapId(), m_Unit->GetPositionNC(), getNextTarget()->GetPositionNC());
					}
					if(los
					        && ((distance <= m_nextSpell->maxrange + m_Unit->GetModelHalfSize()
//					&& distance >= m_nextSpell->minrange
					            )
					            || m_nextSpell->maxrange == 0))  // Target is in Range -> Attack
					{
						SpellEntry* spellInfo = m_nextSpell->spell;

						/* if in range stop moving so we don't interrupt the spell */
						//do not stop for instant spells
						if(sd && GetCastTime(sd) != 0)
							StopMovement(0);

						/*					if(m_nextSpell->procCount)
												m_nextSpell->procCount--;*/

						SpellCastTargets targets = setSpellTargets(spellInfo, getNextTarget());
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
									m_Unit->CastSpellAoF(targets.m_srcX, targets.m_srcY, targets.m_srcZ, spellInfo, true);
									break;
								}
							case TTYPE_DESTINATION:
								{
									m_Unit->CastSpellAoF(targets.m_destX, targets.m_destY, targets.m_destZ, spellInfo, true);
									break;
								}
							default:
								LOG_ERROR("AI Agents: Targettype of AI agent spell %u for creature %u not set", spellInfo->Id, TO< Creature* >(m_Unit)->GetCreatureInfo()->Id);
						}
						// CastSpell(m_Unit, spellInfo, targets);
						if(m_nextSpell && m_nextSpell->cooldown)
							m_nextSpell->cooldowntime = getMSTime() + m_nextSpell->cooldown;

						next_spell_time = (uint32)UNIXTIME + MOB_SPELLCAST_GLOBAL_COOLDOWN;

						m_nextSpell = NULL;
					}
					else // Target out of Range -> Run to it
					{
						//calculate next move
						SetRun();
						float close_to_enemy = 0.0f;
						if(distance > m_nextSpell->maxrange)
							close_to_enemy = m_nextSpell->maxrange - DISTANCE_TO_SMALL_TO_WALK ;
						else if(distance < m_nextSpell->minrange)
							close_to_enemy = m_nextSpell->minrange + DISTANCE_TO_SMALL_TO_WALK ;

						if(close_to_enemy < 0)
							close_to_enemy = 0;

						_CalcDestinationAndMove(getNextTarget(), close_to_enemy);  //if we make exact movement we will never position perfectly
						/*Destination* dst = _CalcDestination(GetNextTarget(), dist);
						MoveTo(dst->x, dst->y, dst->z,0);
						delete dst;*/
					}
				}
				break;
			case AGENT_FLEE:
				{
					//float dist = 5.0f;

					SetWalk();
					if(m_fleeTimer == 0)
						m_fleeTimer = m_FleeDuration;

					/*Destination* dst = _CalcDestination(GetNextTarget(), dist);
					MoveTo(dst->x, dst->y, dst->z,0);
					delete dst;*/
					_CalcDestinationAndMove(getNextTarget(), 5.0f);
					if(!m_hasFleed)
						CALL_SCRIPT_EVENT(m_Unit, OnFlee)(getNextTarget());

					m_AIState = STATE_FLEEING;
					//removed by Zack : somehow creature starts to attack self. Just making sure it is not this one
//				m_nextTarget = m_Unit;
//				m_Unit->SetUInt64Value(UNIT_FIELD_TARGET, 0);
					resetNextTarget();

					WorldPacket data(SMSG_MESSAGECHAT, 100);
					string msg = "%s attempts to run away in fear!";
					data << (uint8)CHAT_MSG_CHANNEL;
					data << (uint32)LANG_UNIVERSAL;
					data << (uint32)(strlen(TO< Creature* >(m_Unit)->GetCreatureInfo()->Name) + 1);
					data << TO< Creature* >(m_Unit)->GetCreatureInfo()->Name;
					data << (uint64)0;
					data << (uint32)(msg.size() + 1);
					data << msg;
					data << uint8(0);

					m_Unit->SendMessageToSet(&data, false);

					//m_Unit->SendChatMessage(CHAT_MSG_MONSTER_EMOTE, LANG_UNIVERSAL, msg);
					//sChatHandler.FillMessageData(&data, CHAT_MSG_MONSTER_EMOTE, LANG_UNIVERSAL, msg, m_Unit->GetGUID());

					m_hasFleed = true;
				}
				break;
			case AGENT_CALLFORHELP:
				{
					FindFriends(64.0f /*8.0f*/);
					m_hasCalledForHelp = true; // We only want to call for Help once in a Fight.
					if(m_Unit->IsCreature())
						TO< Creature* >(m_Unit)->HandleMonsterSayEvent(MONSTER_SAY_EVENT_CALL_HELP);
					CALL_SCRIPT_EVENT(m_Unit, OnCallForHelp)();
				}
				break;
		}
	}
	else if(!getNextTarget() || getNextTarget()->GetInstanceID() != m_Unit->GetInstanceID() || !getNextTarget()->isAlive() || !cansee)
	{
		resetNextTarget();
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

	if(m_Unit->GetCreatedBySpell() == 0)
		TO< Player* >( m_PetOwner )->SetFreePetNo(false, (int)m_Unit->GetUInt32Value(UNIT_FIELD_PETNUMBER));
	TO< Player* >( m_PetOwner )->SetPet(NULL);
	TO< Player* >( m_PetOwner )->SetPetName("");

	//FIXME:Check hunter pet or not
	//FIXME:Check enslaved creature
	m_PetOwner->SetUInt64Value(UNIT_FIELD_SUMMON, 0);

	WorldPacket data;
	data.Initialize(SMSG_PET_SPELLS);
	data << (uint64)0;
	TO< Player* >( m_PetOwner )->GetSession()->SendPacket(&data);

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
	if(m_AIState == STATE_EVADE || !pUnit || !pUnit->isAlive() || m_Unit->IsDead() || ( m_Unit == pUnit ) || ( m_AIType == AITYPE_PASSIVE ) || disable_combat )
		return;

	if(sWorld.Collision && pUnit->IsPlayer())
	{
		if(m_Unit->GetMapMgr() != NULL)
		{
			if(!Flying())
			{
				float target_land_z = m_Unit->GetMapMgr()->GetLandHeight(pUnit->GetPositionX(), pUnit->GetPositionY(), pUnit->GetPositionZ());

				if(fabs(pUnit->GetPositionZ() - target_land_z) > _CalcCombatRange(pUnit, false))
				{
					if(!pUnit->IsPlayer() && target_land_z > m_Unit->GetMapMgr()->GetLiquidHeight(pUnit->GetPositionX(), pUnit->GetPositionY()))
						return;
					else if(TO< Player* >(pUnit)->GetSession() != NULL)
					{
						MovementInfo* mi = TO< Player* >(pUnit)->GetSession()->GetMovementInfo();

						if(mi != NULL && !(mi->flags & MOVEFLAG_FALLING) && !(mi->flags & MOVEFLAG_SWIMMING) && !(mi->flags & MOVEFLAG_LEVITATE))
							return;
					}
				}
			}
		}
	}

	if(pUnit->IsPlayer() && TO< Player* >(pUnit)->GetMisdirectionTarget() != 0)
	{
		Unit* mTarget = m_Unit->GetMapMgr()->GetUnit(TO< Player* >(pUnit)->GetMisdirectionTarget());
		if(mTarget != NULL && mTarget->isAlive())
			pUnit = mTarget;
	}

	if((m_AIState == STATE_IDLE || m_AIState == STATE_FOLLOWING) && m_Unit->GetAIInterface()->GetAllowedToEnterCombat())
	{
		WipeTargetList();

		HandleEvent(EVENT_ENTERCOMBAT, pUnit, 0);
	}

	if(m_AIState == STATE_UNFEARED)
	{
		//we're unfeared resume combat
		HandleEvent(EVENT_ENTERCOMBAT, pUnit, 1);
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
	if(caster->getClass() == PALADIN)
		threat = threat / 2; //Paladins only get 50% threat per heal than other classes

	if(sp != NULL)
		threat += (threat * caster->GetGeneratedThreatModifyer(sp->School) / 100);

	if(threat < 1)
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
		if(caster->IsPlayer() && victim->IsPlayer())
		{
			if(TO< Player* >(caster)->GetGroup() == TO< Player* >(victim)->GetGroup())
			{
				// get victim into combat since they are both
				// in the same party
				if(isHostile(m_Unit, victim))
					m_aiTargets.insert(TargetMap::value_type(victim->GetGUID(), 1));
			}
		}
	}
}

void AIInterface::OnDeath(Object* pKiller)
{
	if(pKiller->IsUnit())
		HandleEvent(EVENT_UNITDIED, TO< Unit* >(pKiller), 0);
	else
		HandleEvent(EVENT_UNITDIED, m_Unit, 0);
}

//function is designed to make a quick check on target to decide if we can attack it
bool AIInterface::UnsafeCanOwnerAttackUnit(Unit* pUnit)
{
	if(!isHostile(m_Unit, pUnit))
		return false;

	if(!pUnit->isAlive())
		return false;

	if(!(pUnit->m_phase & m_Unit->m_phase))   //Not in the same phase
		return false;

	//do not agro units that are faking death. Should this be based on chance ?
	if(pUnit->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_FEIGN_DEATH))
		return false;

	//don't attack owner
	if(m_Unit->GetCreatedByGUID() == pUnit->GetGUID())
		return false;

	//don't agro neutrals
	if((pUnit->IsPlayer() || pUnit->IsPet())
	        && m_Unit->m_factionDBC
	        && m_Unit->m_factionDBC->RepListId == -1
	        && m_Unit->m_faction->HostileMask == 0
	        && m_Unit->m_faction->FriendlyMask == 0
	  )
		return false;
	else if((m_Unit->IsPlayer() || m_Unit->IsPet())
	        && pUnit->m_factionDBC
	        && pUnit->m_factionDBC->RepListId == -1
	        && pUnit->m_faction->HostileMask == 0
	        && pUnit->m_faction->FriendlyMask == 0
	       )
		return false;

	//make sure we do not agro flying stuff
	if(abs(pUnit->GetPositionZ() - m_Unit->GetPositionZ()) > _CalcCombatRange(pUnit, false))
		return false; //blizz has this set to 250 but uses pathfinding

	return true;
}

//this function might be slow but so it should not be spammed
//!!!this function has been reported the biggest bottleneck on emu in 2008 07 04
Unit* AIInterface::FindTarget()
{
	// find nearest hostile Target to attack
	if(!m_AllowedToEnterCombat)
		return NULL;

	if(m_Unit->GetMapMgr() == NULL)
		return NULL;

	Unit* target = NULL;
	Unit* critterTarget = NULL;
	float distance = 999999.0f; // that should do it.. :p
	std::set<Object*>::iterator itr, itr2;
	std::set< Object* >::iterator pitr, pitr2;
	Unit* pUnit;
	float dist;

	// Don't remove this please! - dfighter
	/*
	if( m_AIType == AITYPE_PET ){
	    printf("I'm a pet and I'm looking for targets, RAWR!\n");
	}
	*/

	//target is immune to all form of attacks, cant attack either.
	// not attackable creatures sometimes fight enemies in scripted fights though
	if(m_Unit->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NOT_ATTACKABLE_2))
	{
		return NULL;
	}

	// Start of neutralguard snippet
	if(m_isNeutralGuard)
	{
		Player* tmpPlr;
		for(std::set< Object*>::iterator itrPlr = m_Unit->GetInRangePlayerSetBegin(); itrPlr != m_Unit->GetInRangePlayerSetEnd(); ++itrPlr)
		{
			tmpPlr = TO< Player* >(*itrPlr);

			if(tmpPlr == NULL)
				continue;
			if(tmpPlr->IsDead())
				continue;
			if(tmpPlr->GetTaxiState())
				continue;
			if(tmpPlr->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_FEIGN_DEATH))
				continue;
			if(tmpPlr->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9))
				continue;
			if(tmpPlr->m_invisible)
				continue;
			if(!tmpPlr->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_UNKNOWN2))    //PvP Guard Attackable.
				continue;
			if(!(tmpPlr->m_phase & m_Unit->m_phase))   //Not in the same phase, skip this target
				continue;

			dist = m_Unit->GetDistanceSq(tmpPlr);

			if(dist > 2500.0f)
				continue;
			if(distance > dist)
			{
				if(sWorld.Collision)
				{
					if(CollideInterface.CheckLOS(m_Unit->GetMapId(), m_Unit->GetPositionNC(), tmpPlr->GetPositionNC()))
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
			m_Unit->m_runSpeed = m_Unit->m_base_runSpeed * 2.0f;
			AttackReaction(target, 1, 0);

			m_Unit->SendAIReaction();

			return target;
		}
		distance = 999999.0f; //Reset Distance for normal check
	}
	// End of neutralguard snippet

	//we have a high chance that we will agro a player
	//this is slower then oppfaction list BUT it has a lower chance that contains invalid pointers
	for(pitr2 = m_Unit->GetInRangePlayerSetBegin(); pitr2 != m_Unit->GetInRangePlayerSetEnd();)
	{
		pitr = pitr2;
		++pitr2;

		pUnit = TO< Player* >(*pitr);

		if(UnsafeCanOwnerAttackUnit(pUnit) == false)
			continue;

		//on blizz there is no Z limit check
		dist = m_Unit->GetDistance2dSq(pUnit);

		if(dist > distance)	 // we want to find the CLOSEST target
			continue;

		if(dist <= _CalcAggroRange(pUnit))
		{
			if(sWorld.Collision)
			{
				if(m_Unit->GetMapMgr()->InLineOfSight(m_Unit->GetPositionX(), m_Unit->GetPositionY(), m_Unit->GetPositionZ() + 2, pUnit->GetPositionX(), pUnit->GetPositionY(), pUnit->GetPositionZ() + 2))
				{
					distance = dist;
					target = pUnit;
				}
			}
			else
			{
				distance = dist;
				target = pUnit;
			}
		}
	}

	//a lot less times are check inter faction mob wars :)
	if(m_updateTargetsTimer2 < getMSTime())
	{
		m_updateTargetsTimer2 = getMSTime() + TARGET_UPDATE_INTERVAL;

		for(itr2 = m_Unit->GetInRangeSetBegin(); itr2 != m_Unit->GetInRangeSetEnd();)
		{
			itr = itr2;
			++itr2;

			if(!(*itr)->IsUnit())
				continue;

			pUnit = TO< Unit* >(*itr);

			if(UnsafeCanOwnerAttackUnit(pUnit) == false)
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

			if(dist <= _CalcAggroRange(pUnit))
			{
				if(sWorld.Collision)
				{
					if(m_Unit->GetMapMgr()->InLineOfSight(m_Unit->GetPositionX(), m_Unit->GetPositionY(), m_Unit->GetPositionZ() + 2, pUnit->GetPositionX(), pUnit->GetPositionY(), pUnit->GetPositionZ() + 2))
					{
						distance = dist;
						target = pUnit;
					}
				}
				else
				{
					distance = dist;
					target = pUnit;
				}
			}
		}
	}

	if(!target)
	{
		target = critterTarget;
	}

	if(target)
	{
		/*		if(m_isGuard)
				{
					m_Unit->m_runSpeed = m_Unit->m_base_runSpeed * 2.0f;
					m_fastMove = true;
				}*/

		AttackReaction(target, 1, 0);

		m_Unit->SendAIReaction();

		if(target->GetCreatedByGUID() != 0)
		{
			uint64 charmer = target->GetCharmedByGUID();

			Unit* target2 = m_Unit->GetMapMgr()->GetPlayer(Arcemu::Util::GUID_LOPART(charmer));

			if(target2)
			{
				AttackReaction(target2, 1, 0);
			}
		}
	}
	return target;
}

Unit* AIInterface::FindTargetForSpell(AI_Spell* sp)
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
			if(m_Unit->GetHealthPct() / 100.0f <= sp->floatMisc1) // Heal ourselves cause we got too low HP
			{
				m_Unit->SetTargetGUID(0);
				return m_Unit;
			}
			for(AssistTargetSet::iterator i = m_assistTargets.begin(); i != m_assistTargets.end(); i++)
			{
				if(!(*i)->isAlive())
				{
					continue;
				}
				if((*i)->GetHealthPct() / 100.0f <= sp->floatMisc1) // Heal ourselves cause we got too low HP
				{
					m_Unit->SetTargetGUID((*i)->GetGUID());
					return (*i); // heal Assist Target which has low HP
				}
			}
		}
		if(sp->spellType == STYPE_BUFF)
		{
			m_Unit->SetTargetGUID(0);
			return m_Unit;
		}
	}

	return GetMostHated();
}

bool AIInterface::FindFriends(float dist)
{

	if(m_Unit->GetMapMgr() == NULL)
		return false;

	bool result = false;
	TargetMap::iterator it;

	std::set<Object*>::iterator itr;
	Unit* pUnit;

	for(itr = m_Unit->GetInRangeSetBegin(); itr != m_Unit->GetInRangeSetEnd(); itr++)
	{
		if(!(*itr)->IsUnit())
			continue;

		pUnit = TO< Unit* >(*itr);
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

		if(!(pUnit->m_phase & m_Unit->m_phase))   //We can't help a friendly unit if it is not in our phase
			continue;

		if(isCombatSupport(m_Unit, pUnit) && (pUnit->GetAIInterface()->getAIState() == STATE_IDLE || pUnit->GetAIInterface()->getAIState() == STATE_SCRIPTIDLE))      //Not sure
		{
			if(m_Unit->GetDistanceSq(pUnit) < dist)
			{
				if(m_assistTargets.count(pUnit) == 0)
				{
					result = true;
					m_assistTargets.insert(pUnit);
				}

				LockAITargets(true);

				for(it = m_aiTargets.begin(); it != m_aiTargets.end(); ++it)
				{
					Unit* ai_t = m_Unit->GetMapMgr()->GetUnit(it->first);
					if(ai_t && pUnit->GetAIInterface() && isHostile(ai_t, pUnit))
						pUnit->GetAIInterface()->AttackReaction(ai_t, 1, 0);
				}

				LockAITargets(false);
			}
		}
	}

	uint32 family = TO_CREATURE(m_Unit)->GetCreatureInfo()->Type;

	CreatureProto* pt = TO< Creature* >(m_Unit)->GetProto();

	uint32 summonguard = 0;

	summonguard = pt->summonguard;

	if(family == UNIT_TYPE_HUMANOID && summonguard > 0 && getMSTime() > m_guardTimer && !IS_INSTANCE(m_Unit->GetMapId()))
	{
		m_guardTimer = getMSTime() + 15000;
		uint16 AreaId = m_Unit->GetMapMgr()->GetAreaID(m_Unit->GetPositionX(), m_Unit->GetPositionY());
		AreaTable* at = dbcArea.LookupEntryForced(AreaId);
		if(!at)
			return result;

		ZoneGuardEntry* zoneSpawn;
		if(at->ZoneId != 0)
			zoneSpawn = ZoneGuardStorage.LookupEntry(at->ZoneId);
		else
			zoneSpawn = ZoneGuardStorage.LookupEntry(at->AreaId);

		if(!zoneSpawn) return result;

		uint32 team = 1; // horde default
		if(isAlliance(m_Unit))
			team = 0;

		uint32 guardid = zoneSpawn->AllianceEntry;
		if(team == 1) guardid = zoneSpawn->HordeEntry;
		if(!guardid) return result;

		CreatureInfo* ci = CreatureNameStorage.LookupEntry(guardid);
		if(!ci)
			return result;

		float x = m_Unit->GetPositionX() + (float)((float)(rand() % 150 + 100) / 1000.0f);
		float y = m_Unit->GetPositionY() + (float)((float)(rand() % 150 + 100) / 1000.0f);
		float z = m_Unit->GetMapMgr()->GetLandHeight(x, y, m_Unit->GetPositionZ() + 2);

		if(fabs(z - m_Unit->GetPositionZ()) > 10.0f)
			z = m_Unit->GetPositionZ();


		CreatureProto* cp = CreatureProtoStorage.LookupEntry(guardid);
		if(!cp) return result;

		uint8 spawned = 0;

		std::set< Object* >::iterator hostileItr = m_Unit->GetInRangePlayerSetBegin();
		for(; hostileItr != m_Unit->GetInRangePlayerSetEnd(); hostileItr++)
		{

			Player* p = TO< Player* >(*hostileItr);

			if(spawned >= 3)
				break;

			if(!isHostile(p, m_Unit))
				continue;

			if(spawned == 0)
			{
				uint32 languageid = (team == 0) ? LANG_COMMON : LANG_ORCISH;
				m_Unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, languageid, "Guards!");
			}

			Creature* guard = m_Unit->GetMapMgr()->CreateCreature(guardid);
			guard->Load(cp, x, y, z);
			guard->SetZoneId(m_Unit->GetZoneId());
			guard->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP); /* shitty DBs */
			guard->m_noRespawn = true;

			if(guard->CanAddToWorld())
			{
				uint32 t = RandomUInt(8) * 1000;
				if(t == 0)
					guard->PushToWorld(m_Unit->GetMapMgr());
				else
					sEventMgr.AddEvent(guard, &Creature::AddToWorld, m_Unit->GetMapMgr(), EVENT_UNK, t, 1, 0);
			}
			else
			{
				guard->DeleteMe();

				return result;
			}

			sEventMgr.AddEvent(guard, &Creature::SetGuardWaypoints, EVENT_UNK, 10000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			sEventMgr.AddEvent(guard, &Creature::DeleteMe, EVENT_CREATURE_SAFE_DELETE, 60 * 5 * 1000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			spawned++;
		}
	}

	return result;
}

float AIInterface::_CalcAggroRange(Unit* target)
{
	//float baseAR = 15.0f; // Base Aggro Range
	//                    -8     -7     -6      -5    -4      -3     -2     -1     0      +1     +2     +3    +4     +5     +6     +7    +8
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
	if(!TO_CREATURE(m_Unit)->CanSee(target))
		return 0;

	// Retrieve aggrorange from table
	float AggroRange = baseAR[lvlDiff + 8];

	// Check to see if the target is a player mining a node
	bool isMining = false;
	if(target->IsPlayer())
	{
		if(target->IsCasting())
		{
			// If nearby miners weren't spotted already we'll give them a little surprise.
			Spell* sp = target->GetCurrentSpell();
			if(sp->GetProto()->Effect[0] == SPELL_EFFECT_OPEN_LOCK && sp->GetProto()->EffectMiscValue[0] == LOCKTYPE_MINING)
			{
				isMining = true;
			}
		}
	}

	// If the target is of a much higher level the aggro range must be scaled down, unless the target is mining a nearby resource node
	if(realLvlDiff > 8 && !isMining)
	{
		AggroRange += AggroRange * ((lvlDiff - 8) * 5 / 100);
	}

	// Multiply by elite value
	if(m_Unit->IsCreature() && TO_CREATURE(m_Unit)->GetCreatureInfo()->Rank > 0)
	{
		AggroRange *= (TO_CREATURE(m_Unit)->GetCreatureInfo()->Rank) * 1.50f;
	}

	// Cap Aggro range at 40.0f
	if(AggroRange > 40.0f)
	{
		AggroRange = 40.0f;
	}

	/*  //printf("aggro range: %f , stealthlvl: %d , detectlvl: %d\n",AggroRange,target->GetStealthLevel(),m_Unit->m_stealthDetectBonus);
		if(! ((Creature*)m_Unit)->CanSee(target))
		{
			AggroRange = 0;
		//	AggroRange *= ( 100.0f - (target->m_stealthLevel - m_Unit->m_stealthDetectBonus)* 20.0f ) / 100.0f;
		}
	*/

	// SPELL_AURA_MOD_DETECT_RANGE
	int32 modDetectRange = target->getDetectRangeMod(m_Unit->GetGUID());
	AggroRange += modDetectRange;
	if(target->IsPlayer())
	{
		AggroRange += TO< Player* >(target)->DetectedRange;
	}

	// Re-check if aggro range exceeds Minimum/Maximum caps
	if(AggroRange < 3.0f)
	{
		AggroRange = 3.0f;
	}
	if(AggroRange > 40.0f)
	{
		AggroRange = 40.0f;
	}

	return (AggroRange * AggroRange);
}

void AIInterface::_CalcDestinationAndMove(Unit* target, float dist)
{
	if(!m_canMove || m_Unit->IsStunned())
	{
		StopMovement(0); //Just Stop
		return;
	}

	float newx, newy, newz;

	if(target != NULL)
	{
		newx = target->GetPositionX();
		newy = target->GetPositionY();
		newz = target->GetPositionZ();

		//avoid eating bandwidth with useless movement packets when target did not move since last position
		//this will work since it turned into a common myth that when you pull mob you should not move :D
		if(abs(m_last_target_x - newx) < DISTANCE_TO_SMALL_TO_WALK
		        && abs(m_last_target_y - newy) < DISTANCE_TO_SMALL_TO_WALK && m_creatureState == MOVING)
			return;
		m_last_target_x = newx;
		m_last_target_y = newy;

		float angle = m_Unit->calcAngle(m_Unit->GetPositionX(), m_Unit->GetPositionY(), newx, newy) * M_PI_FLOAT / 180.0f;
		float x = dist * cosf(angle);
		float y = dist * sinf(angle);

		if(target->IsPlayer() && TO_PLAYER(target)->m_isMoving)
		{
			// cater for moving player vector based on orientation
			x -= cosf(target->GetOrientation());
			y -= sinf(target->GetOrientation());
		}

		newx -= x;
		newy -= y;
	}
	else
	{
		newx = m_Unit->GetPositionX();
		newy = m_Unit->GetPositionY();
		newz = m_Unit->GetPositionZ();
	}

	if(!Move(newx, newy, newz))
	{
		//todo: enter evade mode if creature, not pet, not totem
	}
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

	float selfreach = m_Unit->GetCombatReach();
	float targetradius;
//	targetradius = target->GetBoundingRadius(); //this is plain wrong. Represents i have no idea what :)
	targetradius = target->GetModelHalfSize();
	float selfradius;
//	selfradius = m_Unit->GetBoundingRadius(); //this is plain wrong. Represents i have no idea what :)
	selfradius = m_Unit->GetModelHalfSize();
//	float targetscale = target->GetScale();
//	float selfscale = m_Unit->GetScale();

//	range = ((((targetradius*targetradius)*targetscale) + selfreach) + ((selfradius*selfscale) + rang));
	range = targetradius + selfreach + selfradius + rang;
//	if(range > 28.29f) range = 28.29f;
	return range;
}

float AIInterface::_CalcDistanceFromHome()
{
	if(m_AIType == AITYPE_PET)
	{
		return m_Unit->GetDistanceSq(m_PetOwner);
	}
	else if(m_Unit->IsCreature())
	{
		if(m_combatResetX != 0 && m_combatResetY != 0)
			return m_Unit->GetDistanceSq(m_combatResetX, m_combatResetY, m_combatResetZ);
		if(m_returnX != 0.0f && m_returnY != 0.0f)
		{
			return m_Unit->GetDistanceSq(m_returnX, m_returnY, m_returnZ);
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
		0x00000100 - Teleport
		0x00001000 - Run
		0x00000200 - Fly - OLD FLAG, IS THIS STILL VALID?
		0x00003000 - Fly
		some comments on that 0x00000300 - Fly = 0x00000100 | 0x00000200

	waypoint's:
		TODO.... as they somehow seemed to be changed long time ago..

*************************************************************************************************************/

void AIInterface::SendMoveToPacket()
{
	WorldPacket data(SMSG_MONSTER_MOVE, 100);

	data << m_Unit->GetNewGUID();
	data << uint8(0); //vehicle seat index

	if(m_currentMoveSpline.size() == 0)
	{
		//We're not moving, if here we've recently stopped, tell clients so
		data << float(m_Unit->GetPositionX());
		data << float(m_Unit->GetPositionY());
		data << float(m_Unit->GetPositionZ());
		data << uint32(getMSTime());
		data << uint8(1); //stop
	}
	else
	{
		SplinePoint & splinestart = m_currentMoveSpline[0];
		data << splinestart.pos.x;
		data << splinestart.pos.y;
		data << splinestart.pos.z;
		data << splinestart.setoff;

		if(m_currentSplineFinalOrientation != 0)
			data << uint8(4) << m_currentSplineFinalOrientation;
		else
			data << uint8(0);
		data << m_splineFlags;
		data << m_currentSplineTotalMoveTime;

		if(m_splineFlags & SPLINEFLAG_TRAJECTORY)
		{
			data << m_splinetrajectoryVertical;
			data << m_splinetrajectoryTime;
		}

		data << uint32(m_currentMoveSpline.size() - 1);

		SplinePoint & finalpoint = m_currentMoveSpline[m_currentMoveSpline.size() - 1];
		data << finalpoint.pos.x;
		data << finalpoint.pos.y;
		data << finalpoint.pos.z;

		if(HasSplineFlag(SPLINEFLAG_FLYING | SPLINEFLAG_CATMULLROM))
		{
			for(uint32 i = 1; i < m_currentMoveSpline.size() - 1; ++i)
			{
				data << m_currentMoveSpline[i].pos.x;
				data << m_currentMoveSpline[i].pos.y;
				data << m_currentMoveSpline[i].pos.z;
			}
		}
		else
		{
			float midx = (finalpoint.pos.x + splinestart.pos.x) * 0.5f;
			float midy = (finalpoint.pos.y + splinestart.pos.y) * 0.5f;
			float midz = (finalpoint.pos.z + splinestart.pos.z) * 0.5f;

			for(uint32 i = 1; i < m_currentMoveSpline.size() - 1; ++i)
			{
				float tmpx = (midx - m_currentMoveSpline[i].pos.x) * 4;
				float tmpy = (midy - m_currentMoveSpline[i].pos.y) * 4;
				float tmpz = (midz - m_currentMoveSpline[i].pos.z) * 4;

				//pack it
				data << uint32(int(tmpx) & 0x7FF | ((int(tmpy) & 0x7FF) << 11) | ((int(tmpz) & 0x3FF) << 22));
			}
		}
	}
	m_Unit->SendMessageToSet(&data, true);
}

bool AIInterface::StopMovement(uint32 time)
{
	if( m_Unit->GetCurrentVehicle() != NULL )
		return true;

	m_splinePriority = SPLINE_PRIORITY_MOVEMENT;
	if(m_Unit->GetMapMgr() != NULL)
		UpdateMovementSpline();
	m_moveTimer = time; //set pause after stopping

	//Clear current spline
	m_currentMoveSpline.clear();
	m_currentMoveSplineIndex = 1;
	m_currentSplineUpdateCounter = 0;
	m_currentSplineTotalMoveTime = 0;
	m_currentSplineFinalOrientation = 0;

	if(m_Unit->GetMapMgr() != NULL)
		SendMoveToPacket();
	return true;
}

bool AIInterface::MoveTo(float x, float y, float z, float o)
{
	if(!m_canMove || m_Unit->IsStunned())
	{
		StopMovement(0); //Just Stop
		return false;
	}
	if(!Move(x, y, z, o))
		return false;
	if(m_creatureState != MOVING)
		UpdateMove();
	return true;
}

bool AIInterface::IsFlying()
{
	if(Flying())
		return true;
	if(m_Unit->IsPlayer())
		return TO_PLAYER(m_Unit)->FlyCheat;
	return false;
}

void AIInterface::UpdateSpeeds()
{
	if(HasWalkMode(WALKMODE_SPRINT))
		m_runSpeed = (m_Unit->m_runSpeed + 5.0f) * 0.001f;
	if(HasWalkMode(WALKMODE_RUN))
		m_runSpeed = m_Unit->m_runSpeed * 0.001f;

	m_walkSpeed = m_Unit->m_walkSpeed * 0.001f;
	m_flySpeed = m_Unit->m_flySpeed * 0.001f;
}

void AIInterface::UpdateMove()
{
	m_creatureState = MOVING;
}

void AIInterface::SendCurrentMove(Player* plyr)
{
	if(m_currentMoveSplineIndex >= m_currentMoveSpline.size())
		return;

	SplinePoint & start = m_currentMoveSpline[0];
	uint32 timepassed = getMSTime() - start.setoff;

	ByteBuffer* splineBuf = new ByteBuffer(20 * 4);
	*splineBuf << uint32(0); // spline flags
	*splineBuf << uint32(timepassed); //Time Passed (start Position)
	*splineBuf << uint32(m_currentSplineTotalMoveTime); //Total Time
	*splineBuf << uint32(0); //Unknown
	*splineBuf << float(0); //unk
	*splineBuf << float(0); //unk

	*splineBuf << float(0); //trajectory parabolic soeed
	*splineBuf << uint32(0); //trajectory time

	if(m_currentMoveSpline.size() < 4)  //client requires 4, lets generate shit for it
	{
		*splineBuf << uint32(m_currentMoveSpline.size() + 1 /* 1 fake start */ + 2 /* 2 fake ends */); //Spline Count
		SplinePoint & end = m_currentMoveSpline[m_currentMoveSpline.size() - 1];

		*splineBuf << start.pos.x << start.pos.y << start.pos.z;
		for(uint32 i = 0; i < m_currentMoveSpline.size(); ++i)
		{
			*splineBuf << m_currentMoveSpline[i].pos.x;
			*splineBuf << m_currentMoveSpline[i].pos.y;
			*splineBuf << m_currentMoveSpline[i].pos.z;
		}
		*splineBuf << end.pos.x << end.pos.y << end.pos.z + 0.1f;
		*splineBuf << end.pos.x << end.pos.y << end.pos.z + 0.2f;
		*splineBuf << uint8(0);
		*splineBuf << end.pos.x << end.pos.y << end.pos.z;
	}
	else
	{
		*splineBuf << uint32(m_currentMoveSpline.size());
		for(uint32 i = 0; i < m_currentMoveSpline.size(); ++i)
		{
			*splineBuf << m_currentMoveSpline[i].pos.x;
			*splineBuf << m_currentMoveSpline[i].pos.y;
			*splineBuf << m_currentMoveSpline[i].pos.z;
		}
		SplinePoint & end = m_currentMoveSpline[m_currentMoveSpline.size() - 1];
		*splineBuf << uint8(0);
		*splineBuf << end.pos.x << end.pos.y << end.pos.z;
	}

	plyr->AddSplinePacket(m_Unit->GetGUID(), splineBuf);
}

bool AIInterface::setInFront(Unit* target) // not the best way to do it, though
{
	//angle the object has to face
	float angle = m_Unit->calcAngle(m_Unit->GetPositionX(), m_Unit->GetPositionY(), target->GetPositionX(), target->GetPositionY());
	//Change angle slowly 2000ms to turn 180 deg around
	if(angle > 180) angle += 90;
	else angle -= 90; //angle < 180
	m_Unit->getEasyAngle(angle);
	//Convert from degrees to radians (180 deg = PI rad)
	float orientation = angle / (180 / M_PI_FLOAT);
	//Update Orientation Server Side
	m_Unit->SetPosition(m_Unit->GetPositionX(), m_Unit->GetPositionY(), m_Unit->GetPositionZ(), orientation);

	return m_Unit->isInFront(target);
}

void AIInterface::addWayPoint(WayPoint* wp)
{
	if(wp == NULL)
		return;

	if(!addWayPointUnsafe(wp))
	{
		LOG_ERROR("WayPoint ID %u wasn't added to Unit ID %x due to an error occurred in AIInterface::addWayPoint()", wp->id, GetUnit()->GetGUID());
		delete wp;
	}
}

bool AIInterface::addWayPointUnsafe(WayPoint* wp)
{
	if(!m_waypoints)
		m_waypoints = new WayPointMap ;
	if(!wp)
		return false;
	if(wp->id == 0)
		return false; //not valid id

	if(m_waypoints->size() <= wp->id)
		m_waypoints->resize(wp->id + 1);

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
	if(newwpid == 0)
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
				delete(*itr);

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
	for(itr = m_waypoints->begin(); itr != m_waypoints->end(); ++itr)
	{
		if((*itr) != NULL)
		{
			wp = *itr;
			Creature* c = TO_CREATURE(GetUnit());    // yes this is terrible

			//Create
			Creature* pWayPoint = new Creature((uint64)HIGHGUID_TYPE_WAYPOINT << 32 | wp->id);
			pWayPoint->CreateWayPoint(wp->id, pPlayer->GetMapId(), wp->x, wp->y, wp->z, 0);
			pWayPoint->SetCreatureInfo(c->GetCreatureInfo());
			pWayPoint->SetCreatureProto(c->GetProto());
			pWayPoint->SetEntry(1);
			pWayPoint->SetScale(0.5f);
			if(Backwards)
			{
				uint32 DisplayID = (wp->backwardskinid == 0) ? GetUnit()->GetNativeDisplayId() : wp->backwardskinid;
				pWayPoint->SetDisplayId(DisplayID);
				pWayPoint->SetEmoteState(wp->backwardemoteid);
			}
			else
			{
				uint32 DisplayID = (wp->forwardskinid == 0) ? GetUnit()->GetNativeDisplayId() : wp->forwardskinid;
				pWayPoint->SetDisplayId(DisplayID);
				pWayPoint->SetEmoteState(wp->forwardemoteid);
			}
			pWayPoint->setLevel(wp->id);
			pWayPoint->SetUInt32Value(UNIT_NPC_FLAGS, 0);
			pWayPoint->SetFaction(pPlayer->GetFaction());
			pWayPoint->SetHealth(1);
			pWayPoint->SetMaxHealth(1);
			pWayPoint->SetStat(STAT_STRENGTH, wp->flags);

			//Create on client
			ByteBuffer buf(3000);
			uint32 count = pWayPoint->BuildCreateUpdateBlockForPlayer(&buf, pPlayer);
			pPlayer->PushCreationData(&buf, count);

			//root the object
			WorldPacket data1;
			data1.Initialize(SMSG_FORCE_MOVE_ROOT);
			data1 << pWayPoint->GetNewGUID();
			pPlayer->GetSession()->SendPacket(&data1);

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

	for(itr = m_waypoints->begin(); itr != m_waypoints->end(); ++itr)
	{
		if((*itr) != NULL)
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
	if(!GetUnit()->IsCreature()) return false;

	WorldDatabase.Execute("DELETE FROM creature_waypoints WHERE spawnid = %u", TO_CREATURE(GetUnit())->GetSQL_id());
	WayPointMap::const_iterator itr;
	WayPoint* wp = NULL;
	std::stringstream ss;

	for(itr = m_waypoints->begin(); itr != m_waypoints->end(); ++itr)
	{
		if((*itr) == NULL)
			continue;

		wp = (*itr);

		//Save
		ss.rdbuf()->str("");

		ss << "INSERT INTO creature_waypoints ";
		ss << "(spawnid,waypointid,position_x,position_y,position_z,waittime,flags,forwardemoteoneshot,forwardemoteid,backwardemoteoneshot,backwardemoteid,forwardskinid,backwardskinid) VALUES (";
		ss << TO_CREATURE(GetUnit())->GetSQL_id() << ", ";
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
		WorldDatabase.Execute(ss.str().c_str());
	}
	return true;
}

void AIInterface::deleteWaypoints()
{
	//if m_waypoints was loaded from DB, then it's shared between other AIInterface instances and it will be deleted by ObjectMgr::~ObjectMgr()
	if(!m_waypoints || m_waypointsLoadedFromDB)
		return;

	for(WayPointMap::iterator itr = m_waypoints->begin(); itr != m_waypoints->end(); ++itr)
	{
		delete(*itr);
	}
	m_waypoints->clear();
	delete m_waypoints;
	m_waypoints = NULL;
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
	if(m_Unit->GetCurrentSpell() != NULL)
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
			if(MoveDone())
			{
				if(m_moveType == MOVEMENTTYPE_WANTEDWP)//We reached wanted wp stop now
					m_moveType = MOVEMENTTYPE_DONTMOVEWP;

				float wayO = 0.0f;

				if((GetWayPointsCount() != 0) && (m_AIState == STATE_IDLE || m_AIState == STATE_SCRIPTMOVE)) //if we attacking don't use wps
				{
					WayPoint* wp = getWayPoint(getCurrentWaypoint());
					if(wp)
					{
						CALL_SCRIPT_EVENT(m_Unit, OnReachWP)(wp->id, !m_moveBackward);
						TO_CREATURE(m_Unit)->HandleMonsterSayEvent(MONSTER_SAY_EVENT_RANDOM_WAYPOINT);

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
								if(GetUnit()->GetEmoteState() != wp->forwardemoteid)
								{
									GetUnit()->SetEmoteState(wp->forwardemoteid);
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
								if(GetUnit()->GetEmoteState() != wp->backwardemoteid)
								{
									GetUnit()->SetEmoteState(wp->backwardemoteid);
								}
							}
						}
					}
					else
						m_moveTimer = RandomUInt(HasWalkMode(WALKMODE_RUN) ? 5000 : 10000); // wait before next move
				}

				m_creatureState = STOPPED;
				SetWalk();

				if(m_MovementType == MOVEMENTTYPE_DONTMOVEWP)
					m_Unit->SetOrientation(wayO);

				m_timeMoved = 0;
				m_timeToMove = 0;
			}
		}
	}
	else if(m_creatureState == STOPPED && (m_AIState == STATE_IDLE || m_AIState == STATE_SCRIPTMOVE) && !m_moveTimer && !m_timeToMove && getUnitToFollow() == NULL) //creature is stopped and out of Combat
	{
		if(sWorld.getAllowMovement() == false) //is creature movement enabled?
			return;

		if(m_Unit->GetDisplayId() == 5233) //if Spirit Healer don't move
			return;

		// do we have a formation?
		if(m_formationLinkSqlId != 0)
		{
			if(m_formationLinkTarget == 0)
			{
				// haven't found our target yet
				Creature* c = TO< Creature* >(m_Unit);
				if(!c->haslinkupevent)
				{
					// register linkup event
					c->haslinkupevent = true;
					sEventMgr.AddEvent(c, &Creature::FormationLinkUp, m_formationLinkSqlId,
					                   EVENT_CREATURE_FORMATION_LINKUP, 1000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
				}
			}
			else
			{
				// we've got a formation target, set unittofollow to this
				SetUnitToFollow(m_formationLinkTarget);
				FollowDistance = m_formationFollowDistance;
				m_fallowAngle = m_formationFollowAngle;
			}
		}
		if(getUnitToFollow() == NULL)
		{
			// no formation, use waypoints
			int destpoint = -1;

			// If creature has no waypoints just wander aimlessly around spawnpoint
			if(GetWayPointsCount() == 0) //no waypoints
			{
				/*	if(m_moveRandom)
				{
				if((rand()%10)== 0)
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
				else if(m_moveType == MOVEMENTTYPE_CIRCLEWP)  //random move is not on lets follow the path in circles
				{
					// 1 -> 10 then 1 -> 10
					m_currentWaypoint++;
					if(m_currentWaypoint > GetWayPointsCount()) m_currentWaypoint = 1;  //Happens when you delete last wp seems to continue ticking
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
					if(m_currentWaypoint > GetWayPointsCount()) m_currentWaypoint = 1;  //Happens when you delete last wp seems to continue ticking
					if(m_currentWaypoint == GetWayPointsCount())  // Are we on the last waypoint? if so walk back
						m_moveBackward = true;
					if(m_currentWaypoint == 1)  // Are we on the first waypoint? if so lets goto the second waypoint
						m_moveBackward = false;
					if(!m_moveBackward)  // going 1..n
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
							if((wp->forwardskinid != 0) && (GetUnit()->GetDisplayId() != wp->forwardskinid))
							{
								GetUnit()->SetDisplayId(wp->forwardskinid);
								GetUnit()->EventModelChange();
							}
						}
						else
						{
							if((wp->backwardskinid != 0) && (GetUnit()->GetDisplayId() != wp->backwardskinid))
							{
								GetUnit()->SetDisplayId(wp->backwardskinid);
								GetUnit()->EventModelChange();
							}
						}

						if(wp->flags & 512)
							SetFly();
						else if(wp->flags & 256)
							SetRun();
						else
							SetWalk();
						MoveTo(wp->x, wp->y, wp->z, 0);
					}
				}
			}
		}
	}

	//Fear Code
	Unit* unitToFear = getUnitToFear();
	if(m_AIState == STATE_FEAR && unitToFear != NULL && m_creatureState == STOPPED)
	{
		if(getMSTime() > m_FearTimer)   // Wait at point for x ms ;)
		{
			float Fx;
			float Fy;
			float Fz;

			if(sWorld.DisableFearMovement)
			{
				if(m_Unit->GetMapId() == 529 || m_Unit->GetMapId() == 566 ||
				   m_Unit->GetMapId() == 489 || m_Unit->GetMapId() == 572 ||
				   m_Unit->GetMapId() == 562 || m_Unit->GetMapId() == 559 ||
				   m_Unit->GetMapId() == 617 || m_Unit->GetMapId() == 618)
				{
					return;
				}
			}
			// Calculate new angle to target.
			float Fo = m_Unit->calcRadAngle(unitToFear->GetPositionX(), unitToFear->GetPositionY(), m_Unit->GetPositionX(), m_Unit->GetPositionY());
			Fo += RandomFloat(M_PI_FLOAT / 2);

			float dist = m_Unit->CalcDistance(unitToFear);
			if(dist > 30.0f || (Rand(25) && dist > 10.0f))	// not too far or too close
			{
				if(m_Unit->GetMapId() == 572 || m_Unit->GetMapId() == 562 || m_Unit->GetMapId() == 559 ||
				   m_Unit->GetMapId() == 617 || m_Unit->GetMapId() == 618 )   //GET MAP ID
				{
					Fx = m_Unit->GetPositionX();
					Fy = m_Unit->GetPositionY();
				}
				else
				{
					Fx = m_Unit->GetPositionX() - (RandomFloat(15.f) + 5.0f) * cosf(Fo);
					Fy = m_Unit->GetPositionY() - (RandomFloat(15.f) + 5.0f) * sinf(Fo);
				}
			}
			else
			{
				Fx = m_Unit->GetPositionX() + (RandomFloat(20.f) + 5.0f) * cosf(Fo);
				Fy = m_Unit->GetPositionY() + (RandomFloat(20.f) + 5.0f) * sinf(Fo);
			}
			// Check if this point is in water.
			float wl = m_Unit->GetMapMgr()->GetLiquidHeight(Fx, Fy);
//			uint8 wt = m_Unit->GetMapMgr()->GetWaterType(Fx, Fy);

			if(sWorld.Collision)
			{
				Fz = CollideInterface.GetHeight(m_Unit->GetMapId(), Fx, Fy, m_Unit->GetPositionZ() + 2.0f);
				if(Fz == NO_WMO_HEIGHT)
					Fz = m_Unit->GetMapMgr()->GetADTLandHeight(Fx, Fy);
				else
				{
					if(CollideInterface.GetFirstPoint(m_Unit->GetMapId(), m_Unit->GetPositionX(), m_Unit->GetPositionY(), m_Unit->GetPositionZ() + 2.0f,
					                                  Fx, Fy, Fz + 2.0f, Fx, Fy, Fz, -1.0f))
					{
						//Fz = CollideInterface.GetHeight(m_Unit->GetMapId(), Fx, Fy, m_Unit->GetPositionZ() + 2.0f);
					}
				}

				if(fabs(m_Unit->GetPositionZ() - Fz) > 10.0f ||
				        (wl != 0.0f && Fz < wl))		// in water
				{
					m_FearTimer = getMSTime() + 500;
				}
				else if(CollideInterface.CheckLOS(m_Unit->GetMapId(), m_Unit->GetPositionX(), m_Unit->GetPositionY(), m_Unit->GetPositionZ() + 2.0f, Fx, Fy, Fz))
				{
					MoveTo(Fx, Fy, Fz, Fo);
					m_FearTimer = m_totalMoveTime + getMSTime() + 400;
				}
				else
				{
					StopMovement(0);
				}
			}
			else
			{
				Fz = m_Unit->GetMapMgr()->GetADTLandHeight(Fx, Fy);
				if(fabs(m_Unit->GetPositionZ() - Fz) > 4 || (Fz != 0.0f && Fz < (wl - 2.0f)))
					m_FearTimer = getMSTime() + 100;
				else
				{
					SetRun(); //fear = run bitch run
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
		float wanderZ = m_Unit->GetMapMgr()->GetLandHeight(wanderX, wanderY, m_Unit->GetPositionZ() + 2);

		CollideInterface.GetFirstPoint(m_Unit->GetMapId(), m_Unit->GetPositionX(), m_Unit->GetPositionY(), m_Unit->GetPositionZ() + 2, wanderX, wanderY, wanderZ, wanderX, wanderY, wanderZ, -1);
		MoveTo(wanderX, wanderY, wanderZ, wanderO);
		m_WanderTimer = getMSTime() + m_totalMoveTime + 300; // time till next move (+ pause)
	}

	//Unit Follow Code
	Unit* unitToFollow = getUnitToFollow();
	if(unitToFollow != NULL)
	{
		if(unitToFollow->event_GetCurrentInstanceId() != m_Unit->event_GetCurrentInstanceId())
		{
			m_UnitToFollow = 0;
		}
		else
		{
			if(m_AIState == STATE_IDLE || m_AIState == STATE_FOLLOWING)
			{
				float dist = m_Unit->GetDistanceSq(unitToFollow);

				// re-calculate orientation based on target's movement
				if(m_lastFollowX != unitToFollow->GetPositionX() ||
				        m_lastFollowY != unitToFollow->GetPositionY())
				{
					float dx = unitToFollow->GetPositionX() - m_Unit->GetPositionX();
					float dy = unitToFollow->GetPositionY() - m_Unit->GetPositionY();
					if(dy != 0.0f)
					{
						float angle = atan2(dx, dy);
						m_Unit->SetOrientation(angle);
					}
					m_lastFollowX = unitToFollow->GetPositionX();
					m_lastFollowY = unitToFollow->GetPositionY();
				}

				if(dist > (FollowDistance * FollowDistance)) //if out of range
				{
					m_AIState = STATE_FOLLOWING;

					if(dist > 25.0f) //25 yard away lets run else we will loose the them
						SetRun();
					else
						SetWalk();

					if(m_AIType == AITYPE_PET || (m_UnitToFollow == m_formationLinkTarget)) //Unit is Pet/formation
					{
						if(dist > 900.0f/*30*/)
							SetSprint();

						float delta_x = unitToFollow->GetPositionX();
						float delta_y = unitToFollow->GetPositionY();
						float d = 3;
						if(m_formationLinkTarget != 0)
							d = m_formationFollowDistance;

						MoveTo(delta_x + (d * (cosf(m_fallowAngle + unitToFollow->GetOrientation()))),
						       delta_y + (d * (sinf(m_fallowAngle + unitToFollow->GetOrientation()))),
						       unitToFollow->GetPositionZ(), unitToFollow->GetOrientation());
					}
					else
					{
						_CalcDestinationAndMove(unitToFollow, FollowDistance);
					}
				}
			}
		}
	}
}

void AIInterface::CastSpell(Unit* caster, SpellEntry* spellInfo, SpellCastTargets targets)
{
	ARCEMU_ASSERT(spellInfo != NULL);
	if(m_AIType != AITYPE_PET && disable_spell)
		return;

	// Stop movement while casting.
	m_AIState = STATE_CASTING;
#ifdef _AI_DEBUG
	LOG_DEBUG("AI DEBUG: Unit %u casting spell %s on target "I64FMT, caster->GetEntry(),
	          sSpellStore.LookupString(spellInfo->Name), targets.m_unitTarget);
#endif

	//i wonder if this will lead to a memory leak :S
	Spell* nspell = sSpellFactoryMgr.NewSpell(caster, spellInfo, false, NULL);
	nspell->prepare(&targets);
}

SpellEntry* AIInterface::getSpellEntry(uint32 spellId)
{
	SpellEntry* spellInfo = dbcSpell.LookupEntry(spellId);

	if(!spellInfo)
	{
		LOG_ERROR("WORLD: unknown spell id %i", spellId);
		return NULL;
	}

	return spellInfo;
}

SpellCastTargets AIInterface::setSpellTargets(SpellEntry* spellInfo, Unit* target)
{
	SpellCastTargets targets;
	targets.m_unitTarget = target ? target->GetGUID() : 0;
	targets.m_itemTarget = 0;
	targets.m_srcX = targets.m_destX = m_Unit->GetPositionX();
	targets.m_srcY = targets.m_destY = m_Unit->GetPositionY();
	targets.m_srcZ = targets.m_destZ = m_Unit->GetPositionZ();

	if(m_nextSpell->spelltargetType == TTYPE_SINGLETARGET)
	{
		targets.m_targetMask = TARGET_FLAG_UNIT;
	}
	else if(m_nextSpell->spelltargetType == TTYPE_SOURCE)
	{
		targets.m_targetMask = TARGET_FLAG_SOURCE_LOCATION;
//		targets.m_srcX = m_Unit->GetPositionX();
//		targets.m_srcY = m_Unit->GetPositionY();
//		targets.m_srcZ = m_Unit->GetPositionZ();
	}
	else if(m_nextSpell->spelltargetType == TTYPE_DESTINATION)
	{
		targets.m_targetMask = TARGET_FLAG_DEST_LOCATION;
		if(target != NULL)
		{
			targets.m_destX = target->GetPositionX();
			targets.m_destY = target->GetPositionY();
			targets.m_destZ = target->GetPositionZ();
		}
		else
		{
			targets.m_destX = m_Unit->GetPositionX();
			targets.m_destY = m_Unit->GetPositionY();
			targets.m_destZ = m_Unit->GetPositionZ();
		}
	}
	else if(m_nextSpell->spelltargetType == TTYPE_CASTER)
	{
		targets.m_targetMask = TARGET_FLAG_UNIT;
		targets.m_unitTarget = m_Unit->GetGUID();
	}

	return targets;
}

AI_Spell* AIInterface::getSpell()
{
	if(next_spell_time > (uint32)UNIXTIME)
		return NULL;

	waiting_for_cooldown = false;

	// look at our spells
	AI_Spell*   sp = NULL;
	AI_Spell*   def_spell = NULL;
	uint32 cool_time = 0;
	uint32 cool_time2;
	uint32 nowtime = getMSTime();

	if(m_Unit->IsPet())
	{
		sp = def_spell = TO< Pet* >(m_Unit)->HandleAutoCastEvent();
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
				cool_time2 = sp->cooldowntime - nowtime;
				if(!cool_time || cool_time2 < cool_time)
					cool_time = cool_time2;

				waiting_for_cooldown = true;
				continue;
			}

			if(sp->procCount && sp->procCounter >= sp->procCount)
				continue;

			if(sp->agent == AGENT_SPELL)
			{
				if(sp->spellType == STYPE_BUFF)
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
					if(def_spell != 0)
						continue;

					// cast the spell at requested percent.
					if(sp->procChance >= 100 || Rand(sp->procChance))
					{
						//focus/mana requirement
						switch(sp->spell->powerType)
						{
							case POWER_TYPE_MANA:
								if(m_Unit->GetPower(POWER_TYPE_MANA) < sp->spell->manaCost)
									continue;
								break;

							case POWER_TYPE_FOCUS:
								if(m_Unit->GetPower(POWER_TYPE_FOCUS) < sp->spell->manaCost)
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
						if(!cool_time || cool_time2 < cool_time)
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
	LOG_DEBUG("AI DEBUG: Returning no spell for unit %u", m_Unit->GetEntry());
#endif
	return 0;
}

void AIInterface::addSpellToList(AI_Spell* sp)
{
	if(!sp || !sp->spell)
		return;

	AI_Spell* sp2 = new AI_Spell;
	memcpy(sp2, sp, sizeof(AI_Spell));
	m_spells.push_back(sp2);
}

uint32 AIInterface::getThreatByGUID(uint64 guid)
{

	if(m_Unit->GetMapMgr() == NULL)
		return 0;

	Unit* obj = m_Unit->GetMapMgr()->GetUnit(guid);
	if(obj)
		return getThreatByPtr(obj);

	return 0;
}

uint32 AIInterface::getThreatByPtr(Unit* obj)
{
	if(!obj  || m_Unit->GetMapMgr() == NULL)
		return 0;
	TargetMap::iterator it = m_aiTargets.find(obj->GetGUID());
	if(it != m_aiTargets.end())
	{
		Unit* tempUnit = m_Unit->GetMapMgr()->GetUnit(it->first);
		if(tempUnit)
			return it->second + tempUnit->GetThreatModifyer();
		else
			return it->second;

	}
	return 0;
}

//should return a valid target
Unit* AIInterface::GetMostHated()
{
	if(m_Unit->GetMapMgr() == NULL)
		return NULL;

	Unit* ResultUnit = NULL;

	//override mosthated with taunted target. Basic combat checks are made for it.
	//What happens if we can't see tauntedby unit ?
	ResultUnit = getTauntedBy();
	if(ResultUnit != NULL)
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
		Unit* ai_t = m_Unit->GetMapMgr()->GetUnit(itr->first);

		if(!ai_t || ai_t->GetInstanceID() != m_Unit->GetInstanceID() || !ai_t->isAlive() || !isAttackable(m_Unit, ai_t))
		{
			if(getNextTarget() == ai_t)
				resetNextTarget();

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
Unit* AIInterface::GetSecondHated()
{
	if(m_Unit->GetMapMgr() == NULL)
		return NULL;

	Unit* ResultUnit = GetMostHated();

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
		Unit* ai_t = m_Unit->GetMapMgr()->GetUnit(itr->first);
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
	if(!m_aiTargets.size())
		return false;

	if(m_Unit->GetMapMgr() == NULL)
		return false;

	Unit* obj = m_Unit->GetMapMgr()->GetUnit(guid);
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
		if(it->second < 1)
			it->second = 1;

		tempthreat = it->second + obj->GetThreatModifyer();
		if(tempthreat < 1)
			tempthreat = 1;
		if(tempthreat > m_currentHighestThreat)
		{
			// new target!
			if(!isTaunted)
			{
				m_currentHighestThreat = tempthreat;
				setNextTarget(obj);
			}
		}
	}
	else
	{
		m_aiTargets.insert(make_pair(obj->GetGUID(), mod));

		tempthreat = mod + obj->GetThreatModifyer();
		if(tempthreat < 1)
			tempthreat = 1;
		if(tempthreat > m_currentHighestThreat)
		{
			if(!isTaunted)
			{
				m_currentHighestThreat = tempthreat;
				setNextTarget(obj);
			}
		}
	}

	LockAITargets(false);

	if(obj == getNextTarget())
	{
		// check for a possible decrease in threat.
		if(mod < 0)
		{
			setNextTarget(GetMostHated());
			//if there is no more new targets then we can walk back home ?
			if(!getNextTarget())
				HandleEvent(EVENT_LEAVECOMBAT, m_Unit, 0);
		}
	}
	return true;
}

void AIInterface::RemoveThreatByGUID(uint64 guid)
{
	if(!m_aiTargets.size())
		return;

	if(m_Unit->GetMapMgr() == NULL)
		return;

	Unit* obj = m_Unit->GetMapMgr()->GetUnit(guid);
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
		if(obj == getNextTarget())
		{
			setNextTarget(GetMostHated());
			//if there is no more new targets then we can walk back home ?
			if(!getNextTarget())
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
	resetNextTarget();

	m_nextSpell = NULL;
	m_currentHighestThreat = 0;
	LockAITargets(true);
	m_aiTargets.clear();
	LockAITargets(false);
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
			modThreatByPtr(caster, abs(m_currentHighestThreat - oldthreat) + 1); //we need to be the most hated at this moment
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
		setNextTarget(GetMostHated());
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
	if(!target)
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
	if(target == NULL)
		return;

	if(target->GetGUID() == getUnitToFollowGUID())		    // fix for crash here
	{
		m_UnitToFollow = 0;
		m_lastFollowX = m_lastFollowY = 0;
		FollowDistance = 0;
	}

	if(target->GetGUID() == getUnitToFollowBackupGUID())
	{
		m_UnitToFollow_backup = 0;
	}

	AssistTargetSet::iterator  itr = m_assistTargets.find(target);
	if(itr != m_assistTargets.end())
		m_assistTargets.erase(itr);


	LockAITargets(true);

	TargetMap::iterator it2 = m_aiTargets.find(target->GetGUID());
	if(it2 != m_aiTargets.end() || target == getNextTarget())
	{
		target->CombatStatus.RemoveAttacker(m_Unit, m_Unit->GetGUID());
		m_Unit->CombatStatus.RemoveAttackTarget(target);

		if(it2 != m_aiTargets.end())
		{
			m_aiTargets.erase(it2);
		}

		if(target == getNextTarget())	  // no need to cast on these.. mem addresses are still the same
		{
			resetNextTarget();
			m_nextSpell = NULL;

			// find the one with the next highest threat
			GetMostHated();
		}
	}

	LockAITargets(false);

	if(target->IsCreature())
	{
		it2 = target->GetAIInterface()->m_aiTargets.find(m_Unit->GetGUID());
		if(it2 != target->GetAIInterface()->m_aiTargets.end())
		{
			target->GetAIInterface()->LockAITargets(true);
			target->GetAIInterface()->m_aiTargets.erase(it2);
			target->GetAIInterface()->LockAITargets(false);
		}

		if(target->GetAIInterface()->getNextTarget() == m_Unit)
		{
			target->GetAIInterface()->resetNextTarget();
			target->GetAIInterface()->m_nextSpell = NULL;
			target->GetAIInterface()->GetMostHated();
		}

		if(target->GetAIInterface()->getUnitToFollowGUID() == m_Unit->GetGUID())
			target->GetAIInterface()->m_UnitToFollow = 0;
	}

	if(target->GetGUID() == getUnitToFearGUID())
		m_UnitToFear = 0;

	if(tauntedBy == target)
		tauntedBy = NULL;
}

uint32 AIInterface::_CalcThreat(uint32 damage, SpellEntry* sp, Unit* Attacker)
{
	if(!Attacker)
		return 0; // No attacker means no threat and we prevent crashes this way

	if(m_Unit->m_faction != NULL && Attacker->m_faction != NULL)
		if(isSameFaction(m_Unit, Attacker))
			return 0;

	int32 mod = 0;
	if(sp != NULL && sp->ThreatForSpell != 0)
	{
		mod = sp->ThreatForSpell;
	}
	if(sp != NULL && sp->ThreatForSpellCoef != 0.0f)
		mod += int32(damage * sp->ThreatForSpellCoef);
	else
		mod += damage;

	if(sp != NULL && sp->SpellGroupType && Attacker)
	{
		SM_FIValue(Attacker->SM_FThreat, &mod, sp->SpellGroupType);
		SM_PIValue(Attacker->SM_PThreat, &mod, sp->SpellGroupType);
	}

	if(Attacker->getClass() == ROGUE)
		mod = int32(mod * 0.71); // Rogues generate 0.71x threat per damage.

	// modify threat by Buffs
	if(sp != NULL)
		mod += (mod * Attacker->GetGeneratedThreatModifyer(sp->School) / 100);
	else
		mod += (mod * Attacker->GetGeneratedThreatModifyer(0) / 100);

	if(mod < 1)
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
	resetNextTarget();
	m_UnitToFear = 0;
	m_UnitToFollow = 0;
	tauntedBy = 0;

	//Clear targettable
	for(set<Object*>::iterator itr = m_Unit->GetInRangeSetBegin(); itr != m_Unit->GetInRangeSetEnd(); ++itr)
		if((*itr)->IsUnit() && TO< Unit* >(*itr)->GetAIInterface())
			TO< Unit* >(*itr)->GetAIInterface()->RemoveThreatByPtr(m_Unit);
}

void AIInterface::ResetProcCounts()
{
	for(list<AI_Spell*>::iterator itr = m_spells.begin(); itr != m_spells.end(); ++itr)
		if((*itr)->procCount)
			(*itr)->procCounter = 0;
}

void AIInterface::EventChangeFaction(Unit* ForceAttackersToHateThisInstead)
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
	if(ForceAttackersToHateThisInstead == NULL)
	{
		for(set<Object*>::iterator itr = m_Unit->GetInRangeSetBegin(); itr != m_Unit->GetInRangeSetEnd(); ++itr)
			if((*itr)->IsUnit() && TO< Unit* >(*itr)->GetAIInterface())
				TO< Unit* >(*itr)->GetAIInterface()->RemoveThreatByPtr(m_Unit);

		resetNextTarget();
	}
	else
	{
		for(set<Object*>::iterator itr = m_Unit->GetInRangeSetBegin(); itr != m_Unit->GetInRangeSetEnd(); ++itr)
			if((*itr)->IsUnit() && TO< Unit* >(*itr)->GetAIInterface()
			        && TO< Unit* >(*itr)->GetAIInterface()->getThreatByPtr(m_Unit))   //this guy will join me in fight since I'm telling him "sorry i was controlled"
			{
				TO< Unit* >(*itr)->GetAIInterface()->modThreatByPtr(ForceAttackersToHateThisInstead, 10);   //just aping to be bale to hate him in case we got nothing else
				TO< Unit* >(*itr)->GetAIInterface()->RemoveThreatByPtr(m_Unit);
			}

		modThreatByPtr(ForceAttackersToHateThisInstead, 1);
		setNextTarget(ForceAttackersToHateThisInstead);
	}
}

void AIInterface::WipeCurrentTarget()
{
	Unit* nextTarget = getNextTarget();
	if(nextTarget)
	{
		LockAITargets(true);
		TargetMap::iterator itr = m_aiTargets.find(nextTarget->GetGUID());
		if(itr != m_aiTargets.end())
			m_aiTargets.erase(itr);
		LockAITargets(false);

		if(nextTarget->GetGUID() == getUnitToFollowGUID())
			m_UnitToFollow = 0;

		if(nextTarget->GetGUID() == getUnitToFollowBackupGUID())
			m_UnitToFollow_backup = 0;
	}

	resetNextTarget();
}

Unit* AIInterface::getNextTarget()
{
	if(m_nextTarget && m_Unit && m_Unit->GetMapMgr()) return m_Unit->GetMapMgr()->GetUnit(m_nextTarget);
	return NULL;
}

void AIInterface::setNextTarget(Unit* nextTarget)
{
	if(nextTarget)
		setNextTarget(nextTarget->GetGUID());
	else
		resetNextTarget();
}

void AIInterface::setNextTarget(uint64 nextTarget)
{
	m_nextTarget = nextTarget;
	m_Unit->SetTargetGUID(m_nextTarget);
}

void AIInterface::resetNextTarget()
{
	m_nextTarget = 0;
	m_Unit->SetTargetGUID(0);
}

Unit* AIInterface::getUnitToFollow()
{
	if(m_UnitToFollow == 0)
		return NULL;
	Unit* unit = m_Unit->GetMapMgrUnit(m_UnitToFollow);
	if(unit == NULL)
		m_UnitToFollow = 0;
	return unit;
}

Unit* AIInterface::getUnitToFollowBackup()
{
	if(m_UnitToFollow_backup == 0)
		return NULL;
	Unit* unit = m_Unit->GetMapMgrUnit(m_UnitToFollow_backup);
	if(unit == NULL)
		m_UnitToFollow_backup = 0;
	return unit;
}

Unit* AIInterface::getUnitToFear()
{
	if(m_UnitToFear == 0)
		return NULL;
	Unit* unit = m_Unit->GetMapMgrUnit(m_UnitToFear);
	if(unit == NULL)
		m_UnitToFear = 0;
	return unit;
}

Creature* AIInterface::getFormationLinkTarget()
{
	if(m_formationLinkTarget == 0)
		return NULL;
	Creature* creature = m_Unit->GetMapMgrCreature(m_formationLinkTarget);
	if(creature == NULL)
		m_formationLinkTarget = 0;
	return creature;
}

void AIInterface::LoadWaypointMapFromDB(uint32 spawnid)
{
	m_waypoints = objmgr.GetWayPointMap(spawnid);
	if(m_waypoints != NULL && m_waypoints->size() != 0)
		m_waypointsLoadedFromDB = true;
}

void AIInterface::SetWaypointMap(WayPointMap* m, bool delete_old_map)
{
	if(m_waypoints == m)
		return;

	if(delete_old_map)
		deleteWaypoints();

	m_waypoints = m;
	m_waypointsLoadedFromDB = false;
}

void AIInterface::_UpdateTotem(uint32 p_time)
{
	ARCEMU_ASSERT(totemspell != 0);
	if(p_time >= m_totemspelltimer)
	{
		Spell* pSpell = sSpellFactoryMgr.NewSpell(m_Unit, totemspell, true, 0);
		Unit* nextTarget = getNextTarget();
		if(nextTarget == NULL ||
		        (!m_Unit->GetMapMgr()->GetUnit(nextTarget->GetGUID()) ||
		         !nextTarget->isAlive() ||
		         !IsInrange(m_Unit, nextTarget, pSpell->GetProto()->base_range_or_radius_sqr) ||
		         !isAttackable(m_Unit, nextTarget, !(pSpell->GetProto()->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED))
		        )
		  )
		{
			//we set no target and see if we managed to fid a new one
			resetNextTarget();
			//something happened to our target, pick another one
			SpellCastTargets targets(0);
			pSpell->GenerateTargets(&targets);
			if(targets.m_targetMask & TARGET_FLAG_UNIT)
				setNextTarget(targets.m_unitTarget);
		}
		nextTarget = getNextTarget();
		if(nextTarget)
		{
			SpellCastTargets targets(nextTarget->GetGUID());
			pSpell->prepare(&targets);
			// need proper cooldown time!
			m_totemspelltimer = m_totemspelltime;
		}
		else
		{
			delete pSpell;
			pSpell = NULL;
		}
		// these will *almost always* be AoE, so no need to find a target here.
		//			SpellCastTargets targets(m_Unit->GetGUID());
		//			Spell * pSpell = sSpellFactoryMgr.NewSpell(m_Unit, totemspell, true, 0);
		//			pSpell->prepare(&targets);
		// need proper cooldown time!
		//			m_totemspelltimer = m_totemspelltime;
	}
	else
	{
		m_totemspelltimer -= p_time;
	}
}

void AIInterface::UpdateMovementSpline()
{
	if(m_currentMoveSpline.size() == 0 || m_Unit->GetMapMgr()->mLoopCounter == m_currentSplineUpdateCounter)
		return;

	if(m_currentMoveSplineIndex >= m_currentMoveSpline.size())
	{
		m_creatureState = STOPPED;
		return;
	}

	m_currentSplineUpdateCounter = m_Unit->GetMapMgr()->mLoopCounter;

	SplinePoint & current = m_currentMoveSpline[m_currentMoveSplineIndex];
	SplinePoint & prev = m_currentMoveSpline[m_currentMoveSplineIndex - 1];

	G3D::Vector3 newpos;

	float o = atan2(current.pos.x - prev.pos.x, current.pos.y - prev.pos.y);

	uint32 curmstime = getMSTime();

	if(curmstime >= current.arrive)
	{
		newpos = current.pos;
		++m_currentMoveSplineIndex;
		m_currentSplineUpdateCounter = 0;
	}
	else
		newpos = prev.pos - ((prev.pos - current.pos) * float(curmstime - current.setoff) / float(current.arrive - current.setoff));

	m_Unit->SetPosition(newpos.x, newpos.y, newpos.z, o);

	//current spline is finished, attempt to move along next
	if(m_currentSplineUpdateCounter == 0)
	{
		if(MoveDone())
			OnMoveCompleted();
		else
			UpdateMovementSpline();
	}
}

bool AIInterface::Move(float & x, float & y, float & z, float o /*= 0*/)
{
	if(m_splinePriority > SPLINE_PRIORITY_MOVEMENT)
		return false;
	//Make sure our position is up to date
	UpdateMovementSpline();

	//Clear current spline
	m_currentMoveSpline.clear();
	m_currentMoveSplineIndex = 1;
	m_currentSplineUpdateCounter = 0;
	m_currentSplineTotalMoveTime = 0;
	m_currentSplineFinalOrientation = o;

	//Add new points
#ifdef TEST_PATHFINDING
	if(!Flying())
	{
		if(!CreatePath(x, y, z))
		{
			StopMovement(0); //old spline is probly still active on client, need to keep in sync
			return false;
		}
	}
	else
	{
		AddSpline(m_Unit->GetPositionX(), m_Unit->GetPositionY(), m_Unit->GetPositionZ());
		AddSpline(x, y, z);
	}
#else
	AddSpline(m_Unit->GetPositionX(), m_Unit->GetPositionY(), m_Unit->GetPositionZ());
	AddSpline(x, y, z);
#endif


	SendMoveToPacket();
	return true;
}

void AIInterface::AddSpline(float x, float y, float z)
{
	SplinePoint p;
	p.pos = G3D::Vector3(x, y, z);

	if(m_currentMoveSpline.size() == 0)
	{
		//this is first point just insert it, it's always our position for future points
		p.setoff = getMSTime();
		p.arrive = getMSTime(); //now
		m_currentMoveSpline.push_back(p);
		return;
	}

	SplinePoint & prev = m_currentMoveSpline[m_currentMoveSpline.size() - 1];

	float dx = x - prev.pos.x;
	float dy = y - prev.pos.y;
	float dz = z - prev.pos.z;
	float dist = sqrt(dx * dx + dy * dy + dz * dz);

	uint32 movetime;

	if(HasSplineFlag(SPLINEFLAG_FLYING))
		movetime = (uint32)(dist / m_flySpeed);
	else if(HasSplineFlag(SPLINEFLAG_WALKMODE))
	{
		if(HasWalkMode(WALKMODE_SPRINT) || HasWalkMode(WALKMODE_RUN))
			movetime = (uint32)(dist / m_runSpeed);
		else if(HasWalkMode(WALKMODE_WALK))
			movetime = (uint32)(dist / m_walkSpeed);
		else
		{
			LOG_ERROR("Added a spline with unhandled spline flag: %X", m_splineFlags);
			//setting movetime to default value of 1 second. Change if to either a return; or something more meaningful
			//but don't leave movetime uninitialized...
			movetime = 1.0f;
		}
	}
	else
	{
		LOG_ERROR("Added a spline with unhandled spline flag: %X", m_splineFlags);
		//setting movetime to default value of 1 second. Change if to either a return; or something more meaningful
		//but don't leave movetime uninitialized...
		movetime = 1.0f;
	}

	p.setoff = prev.arrive;
	p.arrive = prev.arrive + movetime;
	m_currentSplineTotalMoveTime += movetime;

	m_currentMoveSpline.push_back(p);
}

bool AIInterface::CreatePath(float x, float y, float z, bool onlytest /*= false*/)
{
	//make sure current spline is updated

	NavMeshData* nav = CollideInterface.GetNavMesh(m_Unit->GetMapId());

	if(nav == NULL)
		return false;

	float start[3] = { m_Unit->GetPositionY(), m_Unit->GetPositionZ(), m_Unit->GetPositionX() };
	float end[3] = { y, z, x };
	float extents[3] = { 3, 5, 3 };

	dtQueryFilter filter;
	filter.setIncludeFlags(NAV_GROUND | NAV_WATER | NAV_SLIME | NAV_MAGMA);

	dtPolyRef startref, endref;
	nav->query->findNearestPoly(start, extents, &filter, &startref, NULL);
	nav->query->findNearestPoly(end, extents, &filter, &endref, NULL);


	if(startref == 0 || endref == 0)
		return false;

	dtPolyRef path[256];
	int pathcount;

	if(nav->query->findPath(startref, endref, start, end, &filter, path, &pathcount, 256) != DT_SUCCESS)
		return false;

	if(pathcount == 0 || path[pathcount - 1] != endref)
		return false;

	float points[MAX_PATH_LENGTH * 3];
	int32 pointcount;
	bool usedoffmesh;

	if(findSmoothPath(start, end, path, pathcount, points, &pointcount, usedoffmesh, MAX_PATH_LENGTH, nav->mesh, nav->query, filter) != DT_SUCCESS)
		return false;

	//add to spline
	if(!onlytest)
	{
		for(int32 i = 0; i < pointcount; ++i)
			AddSpline(points[i * 3 + 2], points[i * 3 + 0], points[i * 3 + 1]);
	}
	return true;
}

dtStatus AIInterface::findSmoothPath(const float* startPos, const float* endPos, const dtPolyRef* polyPath, const uint32 polyPathSize, float* smoothPath, int* smoothPathSize, bool & usedOffmesh, const uint32 maxSmoothPathSize, dtNavMesh* mesh, dtNavMeshQuery* query, dtQueryFilter & filter)
{
	*smoothPathSize = 0;
	uint32 nsmoothPath = 0;
	usedOffmesh = false;

	dtPolyRef polys[MAX_PATH_LENGTH];
	memcpy(polys, polyPath, sizeof(dtPolyRef)*polyPathSize);
	uint32 npolys = polyPathSize;

	float iterPos[VERTEX_SIZE], targetPos[VERTEX_SIZE];
	if(DT_SUCCESS != query->closestPointOnPolyBoundary(polys[0], startPos, iterPos))
		return DT_FAILURE;

	if(DT_SUCCESS != query->closestPointOnPolyBoundary(polys[npolys - 1], endPos, targetPos))
		return DT_FAILURE;

	dtVcopy(&smoothPath[nsmoothPath * VERTEX_SIZE], iterPos);
	nsmoothPath++;

	// Move towards target a small advancement at a time until target reached or
	// when ran out of memory to store the path.
	while(npolys && nsmoothPath < maxSmoothPathSize)
	{
		// Find location to steer towards.
		float steerPos[VERTEX_SIZE];
		unsigned char steerPosFlag;
		dtPolyRef steerPosRef = 0;

		if(!getSteerTarget(iterPos, targetPos, SMOOTH_PATH_SLOP, polys, npolys, steerPos, steerPosFlag, steerPosRef, query))
			break;

		bool endOfPath = (steerPosFlag & DT_STRAIGHTPATH_END) != 0;
		bool offMeshConnection = (steerPosFlag & DT_STRAIGHTPATH_OFFMESH_CONNECTION) != 0;

		// Find movement delta.
		float delta[VERTEX_SIZE];
		dtVsub(delta, steerPos, iterPos);
		float len = dtSqrt(dtVdot(delta, delta));
		// If the steer target is end of path or off-mesh link, do not move past the location.
		if((endOfPath || offMeshConnection) && len < SMOOTH_PATH_STEP_SIZE)
			len = 1.0f;
		else
			len = SMOOTH_PATH_STEP_SIZE / len;

		float moveTgt[VERTEX_SIZE];
		dtVmad(moveTgt, iterPos, delta, len);

		// Move
		float result[VERTEX_SIZE];
		const static uint32 MAX_VISIT_POLY = 16;
		dtPolyRef visited[MAX_VISIT_POLY];

		uint32 nvisited = 0;
		query->moveAlongSurface(polys[0], iterPos, moveTgt, &filter, result, visited, (int*)&nvisited, MAX_VISIT_POLY);
		npolys = fixupCorridor(polys, npolys, MAX_PATH_LENGTH, visited, nvisited);

		query->getPolyHeight(visited[nvisited - 1], result, &result[1]);
		dtVcopy(iterPos, result);

		// Handle end of path and off-mesh links when close enough.
		if(endOfPath && inRangeYZX(iterPos, steerPos, SMOOTH_PATH_SLOP, 2.0f))
		{
			// Reached end of path.
			dtVcopy(iterPos, targetPos);
			if(nsmoothPath < maxSmoothPathSize)
			{
				dtVcopy(&smoothPath[nsmoothPath * VERTEX_SIZE], iterPos);
				nsmoothPath++;
			}
			break;
		}
		else if(offMeshConnection && inRangeYZX(iterPos, steerPos, SMOOTH_PATH_SLOP, 2.0f))
		{
			// Reached off-mesh connection.
			usedOffmesh = true;

			// Advance the path up to and over the off-mesh connection.
			dtPolyRef prevRef = 0;
			dtPolyRef polyRef = polys[0];
			uint32 npos = 0;
			while(npos < npolys && polyRef != steerPosRef)
			{
				prevRef = polyRef;
				polyRef = polys[npos];
				npos++;
			}

			for(uint32 i = npos; i < npolys; ++i)
				polys[i - npos] = polys[i];

			npolys -= npos;

			// Handle the connection.
			float startPos[VERTEX_SIZE], endPos[VERTEX_SIZE];
			if(DT_SUCCESS == mesh->getOffMeshConnectionPolyEndPoints(prevRef, polyRef, startPos, endPos))
			{
				if(nsmoothPath < maxSmoothPathSize)
				{
					dtVcopy(&smoothPath[nsmoothPath * VERTEX_SIZE], startPos);
					nsmoothPath++;
				}
				// Move position at the other side of the off-mesh link.
				dtVcopy(iterPos, endPos);
				query->getPolyHeight(polys[0], iterPos, &iterPos[1]);
			}
		}

		// Store results.
		if(nsmoothPath < maxSmoothPathSize)
		{
			dtVcopy(&smoothPath[nsmoothPath * VERTEX_SIZE], iterPos);
			nsmoothPath++;
		}
	}

	*smoothPathSize = nsmoothPath;

	// this is most likely loop
	return nsmoothPath < maxSmoothPathSize ? DT_SUCCESS : DT_FAILURE;
}

bool AIInterface::getSteerTarget(const float* startPos, const float* endPos, const float minTargetDist, const dtPolyRef* path, const uint32 pathSize, float* steerPos, unsigned char & steerPosFlag, dtPolyRef & steerPosRef, dtNavMeshQuery* query)
{
	// Find steer target.
	static const uint32 MAX_STEER_POINTS = 3;
	float steerPath[MAX_STEER_POINTS * VERTEX_SIZE];
	unsigned char steerPathFlags[MAX_STEER_POINTS];
	dtPolyRef steerPathPolys[MAX_STEER_POINTS];
	uint32 nsteerPath = 0;
	dtStatus dtResult = query->findStraightPath(startPos, endPos, path, pathSize,
	                    steerPath, steerPathFlags, steerPathPolys, (int*)&nsteerPath, MAX_STEER_POINTS);
	if(!nsteerPath || DT_SUCCESS != dtResult)
		return false;

	// Find vertex far enough to steer to.
	uint32 ns = 0;
	while(ns < nsteerPath)
	{
		// Stop at Off-Mesh link or when point is further than slop away.
		if((steerPathFlags[ns] & DT_STRAIGHTPATH_OFFMESH_CONNECTION) ||
		        !inRangeYZX(&steerPath[ns * VERTEX_SIZE], startPos, minTargetDist, 1000.0f))
			break;
		ns++;
	}
	// Failed to find good point to steer to.
	if(ns >= nsteerPath)
		return false;

	dtVcopy(steerPos, &steerPath[ns * VERTEX_SIZE]);
	steerPos[1] = startPos[1];  // keep Z value
	steerPosFlag = steerPathFlags[ns];
	steerPosRef = steerPathPolys[ns];

	return true;
}

uint32 AIInterface::fixupCorridor(dtPolyRef* path, const uint32 npath, const uint32 maxPath, const dtPolyRef* visited, const uint32 nvisited)
{
	int32 furthestPath = -1;
	int32 furthestVisited = -1;

	// Find furthest common polygon.
	for(int32 i = npath - 1; i >= 0; --i)
	{
		bool found = false;
		for(int32 j = nvisited - 1; j >= 0; --j)
		{
			if(path[i] == visited[j])
			{
				furthestPath = i;
				furthestVisited = j;
				found = true;
			}
		}
		if(found)
			break;
	}

	// If no intersection found just return current path.
	if(furthestPath == -1 || furthestVisited == -1)
		return npath;

	// Concatenate paths.

	// Adjust beginning of the buffer to include the visited.
	uint32 req = nvisited - furthestVisited;
	uint32 orig = uint32(furthestPath + 1) < npath ? furthestPath + 1 : npath;
	uint32 size = npath - orig > 0 ? npath - orig : 0;
	if(req + size > maxPath)
		size = maxPath - req;

	if(size)
		memmove(path + req, path + orig, size * sizeof(dtPolyRef));

	// Store visited
	for(uint32 i = 0; i < req; ++i)
		path[i] = visited[(nvisited - 1) - i];

	return req + size;
}

void AIInterface::EventEnterCombat(Unit* pUnit, uint32 misc1)
{
	if(m_AIState == STATE_EVADE)
		return;
	if(pUnit == NULL || pUnit->IsDead() || m_Unit->IsDead()) return;

	// set the target first
	if(pUnit->GetInstanceID() == m_Unit->GetInstanceID())
	{
		m_Unit->SetTargetGUID(pUnit->GetGUID());
	}
	/* send the message */
	if(m_Unit->IsCreature())
	{
		Creature* creature = TO_CREATURE(m_Unit);
		creature->HandleMonsterSayEvent(MONSTER_SAY_EVENT_ENTER_COMBAT);

		CALL_SCRIPT_EVENT(m_Unit, OnCombatStart)(pUnit);

		if(creature->m_spawn && (creature->m_spawn->channel_target_go || creature->m_spawn->channel_target_creature))
		{
			m_Unit->SetChannelSpellId(0);
			m_Unit->SetChannelSpellTargetGUID(0);
		}
	}

	// Stop the emote - change to fight emote
	m_Unit->SetEmoteState(EMOTE_STATE_READY1H);

	if(misc1 == 0)
	{
		SetReturnPosition();

		m_combatResetX = pUnit->GetPositionX();
		m_combatResetY = pUnit->GetPositionY();
		m_combatResetZ = pUnit->GetPositionZ();
	}


	// dismount if mounted
	if(m_Unit->IsCreature() && !(TO_CREATURE(m_Unit)->GetCreatureInfo()->Flags1 & CREATURE_FLAG1_FIGHT_MOUNTED))
		m_Unit->SetMount(0);

	if(m_AIState != STATE_ATTACKING)
		StopMovement(0);

	m_AIState = STATE_ATTACKING;
	if(m_Unit->GetMapMgr() && m_Unit->GetMapMgr()->GetMapInfo() && m_Unit->GetMapMgr()->GetMapInfo()->type == INSTANCE_RAID)
	{
		if(m_Unit->IsCreature())
		{
			if(TO< Creature* >(m_Unit)->GetCreatureInfo()->Rank == 3)
			{
				m_Unit->GetMapMgr()->AddCombatInProgress(m_Unit->GetGUID());
			}
		}
	}
	if(pUnit->IsPlayer() && TO< Player* >(pUnit)->InGroup())
	{
		m_Unit->GetAIInterface()->modThreatByPtr(pUnit, 1);
		Group* pGroup = TO< Player* >(pUnit)->GetGroup();

		Player* pGroupGuy;
		GroupMembersSet::iterator itr;
		pGroup->Lock();
		for(uint32 i = 0; i < pGroup->GetSubGroupCount(); i++)
		{
			for(itr = pGroup->GetSubGroup(i)->GetGroupMembersBegin(); itr != pGroup->GetSubGroup(i)->GetGroupMembersEnd(); ++itr)
			{
				pGroupGuy = (*itr)->m_loggedInPlayer;
				if(pGroupGuy && pGroupGuy->isAlive() && m_Unit->GetMapMgr() == pGroupGuy->GetMapMgr() && pGroupGuy->GetDistanceSq(pUnit) <= 40 * 40) //50 yards for now. lets see if it works
				{
					m_Unit->GetAIInterface()->AttackReaction(pGroupGuy, 1, 0);
				}
			}
		}
		pGroup->Unlock();
	}
	//Zack : Put mob into combat animation. Take out weapons and start to look serious :P
	m_Unit->smsg_AttackStart(pUnit);
}

void AIInterface::EventLeaveCombat(Unit* pUnit, uint32 misc1)
{
	if(m_AIState == STATE_EVADE)
		return;
	if(pUnit == NULL) return;

	if(pUnit->IsCreature())
	{
		if(pUnit->IsDead())
			pUnit->RemoveAllAuras();
		else
			pUnit->RemoveNegativeAuras();
	}

	Unit* target = NULL;
	if(m_Unit->GetMapMgr() && m_Unit->GetMapMgr()->GetMapInfo())
	{
		switch(m_Unit->GetMapMgr()->GetMapInfo()->type)
		{
			case INSTANCE_NULL:
			case INSTANCE_BATTLEGROUND:
				if(m_outOfCombatRange && _CalcDistanceFromHome() < m_outOfCombatRange)
					target = FindTarget();
				break;

			case INSTANCE_RAID:
			case INSTANCE_NONRAID:
			case INSTANCE_MULTIMODE:
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
	if(m_Unit->IsCreature())
	{
		Creature* creature = TO_CREATURE(m_Unit);
		creature->HandleMonsterSayEvent(MONSTER_SAY_EVENT_ON_COMBAT_STOP);

		if(creature->original_emotestate)
			m_Unit->SetEmoteState(creature->original_emotestate);
		else
			m_Unit->SetEmoteState(0);

		if(creature->m_spawn && (creature->m_spawn->channel_target_go || creature->m_spawn->channel_target_creature))
		{
			if(creature->m_spawn->channel_target_go)
				sEventMgr.AddEvent(creature, &Creature::ChannelLinkUpGO, creature->m_spawn->channel_target_go, EVENT_CREATURE_CHANNEL_LINKUP, 1000, 5, 0);

			if(creature->m_spawn->channel_target_creature)
				sEventMgr.AddEvent(creature, &Creature::ChannelLinkUpCreature, creature->m_spawn->channel_target_creature, EVENT_CREATURE_CHANNEL_LINKUP, 1000, 5, 0);
		}
	}

	//reset ProcCount
	//ResetProcCounts();
	SetSprint();
	LockAITargets(true);
	m_aiTargets.clear();
	LockAITargets(false);
	m_fleeTimer = 0;
	m_hasFleed = false;
	m_hasCalledForHelp = false;
	m_nextSpell = NULL;
	resetNextTarget();
	m_Unit->CombatStatus.Vanished();

	if(m_AIType == AITYPE_PET)
	{
		m_AIState = STATE_FOLLOWING;
		SetUnitToFollow(m_PetOwner);
		FollowDistance = 3.0f;
		m_lastFollowX = m_lastFollowY = 0;
		if(m_Unit->IsPet())
		{
			TO< Pet* >(m_Unit)->SetPetAction(PET_ACTION_FOLLOW);
			if(m_Unit->isAlive() && m_Unit->IsInWorld())
			{
				TO< Pet* >(m_Unit)->HandleAutoCastEvent(AUTOCAST_EVENT_LEAVE_COMBAT);
			}
		}
		HandleEvent(EVENT_FOLLOWOWNER, 0, 0);
	}
	else
	{
		m_AIState = STATE_EVADE;

		Unit* SavedFollow = getUnitToFollow();

		if(m_Unit->isAlive())
		{
			if(SavedFollow == NULL)
			{
				SetReturnPosition();
				MoveEvadeReturn();
			}
			else
				m_AIState = STATE_FOLLOWING;

			Creature* aiowner = TO< Creature* >(m_Unit);
			//clear tagger.
			aiowner->UnTag();
			aiowner->SetUInt32Value(UNIT_DYNAMIC_FLAGS, aiowner->GetUInt32Value(UNIT_DYNAMIC_FLAGS) & ~(U_DYN_FLAG_TAGGED_BY_OTHER | U_DYN_FLAG_LOOTABLE));
		}
		CALL_SCRIPT_EVENT(m_Unit, OnCombatStop)(SavedFollow);
	}

	if(m_Unit->GetMapMgr() && m_Unit->GetMapMgr()->GetMapInfo() && m_Unit->GetMapMgr()->GetMapInfo()->type == INSTANCE_RAID)
	{
		if(m_Unit->IsCreature())
		{
			if(TO< Creature* >(m_Unit)->GetCreatureInfo()->Rank == 3)
			{
				m_Unit->GetMapMgr()->RemoveCombatInProgress(m_Unit->GetGUID());
			}
		}
	}

	// Remount if mounted
	if(m_Unit->IsCreature())
	{
		Creature* creature = TO< Creature* >(m_Unit);
		if(creature->m_spawn)
			m_Unit->SetMount(creature->m_spawn->MountedDisplayID);
		//m_Unit->SetMount(TO< Creature* >( m_Unit )->GetSpawnO->MountedDisplayID );
	}
	//Zack : not sure we need to send this. Did not see it in the dumps since mob died eventually but it seems logical to make this
	m_Unit->smsg_AttackStop(pUnit);
}

void AIInterface::EventDamageTaken(Unit* pUnit, uint32 misc1)
{
	if(m_AIState == STATE_EVADE)
		return;
	if(pUnit == NULL) return;

	if(m_Unit->IsCreature())
		TO< Creature* >(m_Unit)->HandleMonsterSayEvent(MONSTER_SAY_EVENT_ON_DAMAGE_TAKEN);

	pUnit->RemoveAura(24575);

	CALL_SCRIPT_EVENT(m_Unit, OnDamageTaken)(pUnit, misc1);
	if(!modThreatByPtr(pUnit, misc1))
	{
		m_aiTargets.insert(TargetMap::value_type(pUnit->GetGUID(), misc1));
	}
	pUnit->CombatStatus.OnDamageDealt(m_Unit);
}

void AIInterface::EventFollowOwner(Unit* pUnit, uint32 misc1)
{
	if(m_AIState == STATE_EVADE)
		return;
	m_AIState = STATE_FOLLOWING;
	if(m_Unit->IsPet())
		TO< Pet* >(m_Unit)->SetPetAction(PET_ACTION_FOLLOW);
	SetUnitToFollow(m_PetOwner);
	m_lastFollowX = m_lastFollowY = 0;
	FollowDistance = 4.0f;

	LockAITargets(true);
	m_aiTargets.clear();
	LockAITargets(false);
	m_fleeTimer = 0;
	m_hasFleed = false;
	m_hasCalledForHelp = false;
	m_nextSpell = NULL;
	resetNextTarget();
	SetRun();
}

void AIInterface::EventFear(Unit* pUnit, uint32 misc1)
{
	if(pUnit == NULL) return;

	m_FearTimer = 0;
	SetUnitToFear(pUnit);

	CALL_SCRIPT_EVENT(m_Unit, OnFear)(pUnit, 0);
	m_AIState = STATE_FEAR;
	StopMovement(1);

	m_UnitToFollow_backup = m_UnitToFollow;
	m_UnitToFollow = 0;
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
	SetRun();
	SetNextSpell(NULL);
	resetNextTarget();
}

void AIInterface::EventUnfear(Unit* pUnit, uint32 misc1)
{
	if(m_AIState == STATE_EVADE)
		return;
	m_UnitToFollow = m_UnitToFollow_backup;
	FollowDistance = FollowDistance_backup;
	m_AIState = STATE_UNFEARED; // let future reactions put us back into combat without bugging return positions

	m_UnitToFear = 0;
	StopMovement(1);
}

void AIInterface::EventWander(Unit* pUnit, uint32 misc1)
{
	if(m_AIState == STATE_EVADE)
		return;
	if(pUnit == NULL) return;

	m_WanderTimer = 0;

	//CALL_SCRIPT_EVENT(m_Unit, OnWander)(pUnit, 0); FIX ME
	m_AIState = STATE_WANDER;
	StopMovement(1);

	m_UnitToFollow_backup = m_UnitToFollow;
	m_UnitToFollow = 0;
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
	SetRun();

	SetNextSpell(NULL);
	resetNextTarget();
}

void AIInterface::EventUnwander(Unit* pUnit, uint32 misc1)
{
	if(m_AIState == STATE_EVADE)
		return;
	m_UnitToFollow = m_UnitToFollow_backup;
	FollowDistance = FollowDistance_backup;
	m_AIState = STATE_IDLE; // we need this to prevent permanent fear, wander, and other problems

	StopMovement(1);
}

void AIInterface::EventUnitDied(Unit* pUnit, uint32 misc1)
{
	if(pUnit == NULL) return;

	if(m_Unit->IsCreature())
		TO< Creature* >(m_Unit)->HandleMonsterSayEvent(MONSTER_SAY_EVENT_ON_DIED);

	CALL_SCRIPT_EVENT(m_Unit, OnDied)(pUnit);
	if(m_Unit->IsCreature())
		CALL_INSTANCE_SCRIPT_EVENT(m_Unit->GetMapMgr(), OnCreatureDeath)(TO_CREATURE(m_Unit), pUnit);

	m_AIState = STATE_IDLE;

	StopMovement(0);
	LockAITargets(true);
	m_aiTargets.clear();
	LockAITargets(false);
	m_UnitToFollow = 0;
	m_lastFollowX = m_lastFollowY = 0;
	m_UnitToFear = 0;
	FollowDistance = 0.0f;
	m_fleeTimer = 0;
	m_hasFleed = false;
	m_hasCalledForHelp = false;
	m_nextSpell = NULL;

	resetNextTarget();

	// dismount if we are mounted
	m_Unit->SetMount(0);

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

	Instance* pInstance = NULL;
	if(m_Unit->GetMapMgr())
		pInstance = m_Unit->GetMapMgr()->pInstance;

	if(m_Unit->GetMapMgr()
	        && m_Unit->IsCreature()
	        && !m_Unit->IsPet()
	        && pInstance
	        && (pInstance->m_mapInfo->type == INSTANCE_RAID
	            || pInstance->m_mapInfo->type == INSTANCE_NONRAID
	            || pInstance->m_mapInfo->type == INSTANCE_MULTIMODE))
	{
		InstanceBossInfoMap* bossInfoMap = objmgr.m_InstanceBossInfoMap[m_Unit->GetMapMgr()->GetMapId()];
		Creature* pCreature = TO< Creature* >(m_Unit);
		bool found = false;

		if(IS_PERSISTENT_INSTANCE(pInstance) && bossInfoMap != NULL)
		{
			uint32 npcGuid = pCreature->GetProto()->Id;
			InstanceBossInfoMap::const_iterator bossInfo = bossInfoMap->find(npcGuid);
			if(bossInfo != bossInfoMap->end())
			{
				found = true;
				m_Unit->GetMapMgr()->pInstance->m_killedNpcs.insert(npcGuid);
				m_Unit->GetMapMgr()->pInstance->SaveToDB();
				for(InstanceBossTrashList::iterator trash = bossInfo->second->trash.begin(); trash != bossInfo->second->trash.end(); ++trash)
				{
					Creature* c = m_Unit->GetMapMgr()->GetSqlIdCreature((*trash));
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

		if(found == false)
		{
			// No instance boss information ... so fallback ...
			uint32 npcGuid = pCreature->GetSQL_id();
			m_Unit->GetMapMgr()->pInstance->m_killedNpcs.insert(npcGuid);
			m_Unit->GetMapMgr()->pInstance->SaveToDB();
		}
	}
	if(m_Unit->GetMapMgr() && m_Unit->GetMapMgr()->GetMapInfo() && m_Unit->GetMapMgr()->GetMapInfo()->type == INSTANCE_RAID)
	{
		if(m_Unit->IsCreature())
		{
			if(TO< Creature* >(m_Unit)->GetCreatureInfo()->Rank == 3)
			{
				m_Unit->GetMapMgr()->RemoveCombatInProgress(m_Unit->GetGUID());
			}
		}
	}


	//remove negative auras
	//if( m_Unit->IsCreature() )
	//	m_Unit->RemoveNegativeAuras();
}

void AIInterface::EventHostileAction(Unit* pUnit, uint32 misc1)
{
	m_combatResetX = m_Unit->GetPositionX();
	m_combatResetY = m_Unit->GetPositionY();
	m_combatResetZ = m_Unit->GetPositionZ();
}

void AIInterface::MoveKnockback(float x, float y, float z, float horizontal, float vertical)
{
	HandleEvent(EVENT_FORCEREDIRECTED, NULL, 0);
	m_splinePriority = SPLINE_PRIORITY_REDIRECTION;

	//Clear current spline
	m_currentMoveSpline.clear();
	m_currentMoveSplineIndex = 1;
	m_currentSplineUpdateCounter = 0;
	m_currentSplineTotalMoveTime = 0;
	m_currentSplineFinalOrientation = 0;

	m_splinetrajectoryTime = 0;
	m_splinetrajectoryVertical = vertical;

	SetRun();
	m_runSpeed *= 3;
	//lets say vertical being 7.5 would give us 100% of our speed towards target
	//anything higher gets a proportional loss of speed
	//this is for spells which knock you up high, but for a short distance
	float speedmod = vertical / 7.5;
	m_runSpeed /= speedmod;


	AddSpline(m_Unit->GetPositionX(), m_Unit->GetPositionY(), m_Unit->GetPositionZ());
	AddSpline(x, y, z);

	AddSplineFlag(SPLINEFLAG_TRAJECTORY | SPLINEFLAG_KNOCKBACK);

	SendMoveToPacket();

	//fix run speed
	UpdateSpeeds();
}

void AIInterface::OnMoveCompleted()
{
	uint32 splineflags = m_splineFlags;

	//remove flags that are temporary
	RemoveSplineFlag(SPLINEFLAG_DONE | SPLINEFLAG_TRAJECTORY | SPLINEFLAG_KNOCKBACK);

	//reset spline priority so other movements can happen
	m_splinePriority = SPLINE_PRIORITY_MOVEMENT;

	//we've been knocked somewhere without entering combat, move back
	if(m_AIState == STATE_IDLE && m_returnX != 0.0f && m_returnY != 0.0f && m_returnZ != 0.0f)
	{
		m_AIState = STATE_EVADE;
		MoveEvadeReturn();
	}
}

void AIInterface::MoveEvadeReturn()
{
	if(!MoveTo(m_returnX, m_returnY, m_returnZ, m_Unit->GetSpawnO()) && m_splinePriority == SPLINE_PRIORITY_MOVEMENT)
	{
		m_Unit->SetPosition(m_returnX, m_returnY, m_returnZ, m_Unit->GetSpawnO());
		StopMovement(0);
	}
}

void AIInterface::EventForceRedirected(Unit* pUnit, uint32 misc1)
{
	if(m_AIState == STATE_IDLE)
		SetReturnPosition();
}

void AIInterface::MoveJump(float x, float y, float z, float o /*= 0*/)
{
	m_splinePriority = SPLINE_PRIORITY_REDIRECTION;

	//Clear current spline
	m_currentMoveSpline.clear();
	m_currentMoveSplineIndex = 1;
	m_currentSplineUpdateCounter = 0;
	m_currentSplineTotalMoveTime = 0;
	m_currentSplineFinalOrientation = o;

	m_splinetrajectoryTime = 0;
	m_splinetrajectoryVertical = 5; //we don't have values for this afaik

	SetRun();
	m_runSpeed *= 3;

	AddSpline(m_Unit->GetPositionX(), m_Unit->GetPositionY(), m_Unit->GetPositionZ());
	AddSpline(x, y, z);

	AddSplineFlag(SPLINEFLAG_TRAJECTORY);

	SendMoveToPacket();

	//fix run speed
	UpdateSpeeds();
}

void AIInterface::SetReturnPosition()
{
	if(m_returnX != 0.0f && m_returnY != 0.0f && m_returnZ != 0.0f)  //already returning somewhere
		return;
	m_returnX = m_Unit->GetSpawnX();
	m_returnY = m_Unit->GetSpawnY();
	m_returnZ = m_Unit->GetSpawnZ();
}

bool AIInterface::MoveCharge(float x, float y, float z)
{
	m_splinePriority = SPLINE_PRIORITY_REDIRECTION;

	//Clear current spline
	m_currentMoveSpline.clear();
	m_currentMoveSplineIndex = 1;
	m_currentSplineUpdateCounter = 0;
	m_currentSplineTotalMoveTime = 0;
	m_currentSplineFinalOrientation = 0;

	SetRun();

	m_runSpeed *= 3.5f;

#ifdef TEST_PATHFINDING
	if(!Flying())
	{
		if(!CreatePath(x, y, z))
		{
			StopMovement(0); //old spline is probly still active on client, need to keep in sync
			return false;
		}
	}
	else
	{
		AddSpline(m_Unit->GetPositionX(), m_Unit->GetPositionY(), m_Unit->GetPositionZ());
		AddSpline(x, y, z);
	}
#else
	AddSpline(m_Unit->GetPositionX(), m_Unit->GetPositionY(), m_Unit->GetPositionZ());
	AddSpline(x, y, z);
#endif

	UpdateSpeeds(); //reset run speed

	SendMoveToPacket();
	return true;
}

void AIInterface::MoveTeleport(float x, float y, float z, float o /*= 0*/)
{
	m_currentMoveSpline.clear();
	m_currentMoveSplineIndex = 1;
	m_currentSplineUpdateCounter = 0;
	m_currentSplineTotalMoveTime = 0;
	m_currentSplineFinalOrientation = o;

	AddSplineFlag(SPLINEFLAG_DONE);

	AddSpline(m_Unit->GetPositionX(), m_Unit->GetPositionY(), m_Unit->GetPositionZ());
	AddSpline(x, y, z);

	SendMoveToPacket();

	//complete move
	m_currentMoveSpline.clear();
	m_Unit->SetPosition(x, y, z, o);
}
