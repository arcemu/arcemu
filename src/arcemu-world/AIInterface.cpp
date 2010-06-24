/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2010 <http://www.ArcEmu.org/>
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

AIInterface::AIInterface(Unit * self)
{
	m_Self = self;
	m_timeToMove = 0;
	m_timeMoved = 0;
	m_moveTimer = 0;
	m_moveMode = AIMOVESTATE_RUN;
	m_creatureState = STOPPED;
	m_AIState = STATE_IDLE;
	m_updateAssistTimer = 1;
	m_updateTargetsTimer = TARGET_UPDATE_INTERVAL_ON_PLAYER;
	m_updateTargetsTimer2 = 0;
	m_runSpeed = 0.0f;
	m_flySpeed = 0.0f;
	m_totalMoveTime = 0;
	m_disableFlag = 0;
	m_walkSpeed = m_Self->m_walkSpeed*0.001f;//move distance per ms time 
	m_runSpeed = m_Self->m_runSpeed*0.001f;//move distance per ms time 
	m_flySpeed = m_Self->m_flySpeed * 0.001f;
}
float AIInterface::calcAggroRange(Unit* target)
{
	if(!TO_CREATURE(m_Self)->CanSee(target) )
		return 0;
	float baseAR[17] = {19.0f, 18.5f, 18.0f, 17.5f, 17.0f, 16.5f, 16.0f, 15.5f, 15.0f, 14.5f, 12.0f, 10.5f, 8.5f,  7.5f,  6.5f,  6.5f, 5.0f};
	// Lvl Diff -8 -7 -6 -5 -4 -3 -2 -1 +0 +1 +2  +3  +4  +5  +6  +7  +8
	// Arr Pos   0  1  2  3  4  5  6  7  8  9 10  11  12  13  14  15  16
	int32 lvlDiff = target->getLevel() - m_Self->getLevel();
	int32 realLvlDiff = lvlDiff;
	if (lvlDiff > 8)
		lvlDiff = 8;
	if (lvlDiff < -8)
		lvlDiff = -8;

	// Retrieve aggrorange from table
	float AggroRange = baseAR[lvlDiff + 8];

	// Check to see if the target is a player mining a node
	bool isMining = false;
	if (target->IsPlayer())
	{
		if (target->IsCasting())
		{
			// If nearby miners weren't spotted already we'll give them a little surprise.
			Spell * sp = target->GetCurrentSpell();
			if (sp->GetProto()->Effect[0] == SPELL_EFFECT_OPEN_LOCK && sp->GetProto()->EffectMiscValue[0] == LOCKTYPE_MINING)
				isMining = true;
		}
	}

	// If the target is of a much higher level the aggro range must be scaled down, unless the target is mining a nearby resource node
	if (realLvlDiff > 8 && !isMining)
		AggroRange += AggroRange * ((lvlDiff - 8) * 5 / 100);

	// Multiply by elite value
	if (m_Self->IsCreature() && TO_CREATURE(m_Self)->GetCreatureInfo() && TO_CREATURE(m_Self)->GetCreatureInfo()->Rank > 0)
		AggroRange *= ( TO_CREATURE(m_Self)->GetCreatureInfo()->Rank ) * 1.50f;

	// Cap Aggro range at 40.0f
	if(AggroRange > 40.0f)
		AggroRange = 40.0f;

	// SPELL_AURA_MOD_DETECT_RANGE
	int32 modDetectRange = target->getDetectRangeMod(m_Self->GetGUID());
	AggroRange += modDetectRange;
	if(target->IsPlayer())
		AggroRange += static_cast< Player* >(target)->DetectedRange;

	// Re-check if aggro range exceeds Minimum/Maximum caps
	if (AggroRange < 3.0f)
		AggroRange = 3.0f;
	else if(AggroRange > 40.0f)
		AggroRange = 40.0f;

	return AggroRange;
}

void AIInterface::calcDestinationAndMove(Unit * target, float distance)
{
	if(m_moveMode == AIMOVESTATE_CANTMOVE || m_Self->IsStunned() )
	{
		StopMovement(0); //Just Stop
		return;
	}
	float localX = m_Self->GetPositionX();
	float localY = m_Self->GetPositionY();
	if( target != NULL && (target->IsUnit() || target->IsPlayer() ) )
	{
		float targetsX = target->GetPositionX();
		float targetsY = target->GetPositionY();
		float targetsZ = target->GetPositionZ();
		float angle = m_Self->calcRadAngle(localX,localY,targetsX,targetsY);
		float newx = distance * cosf(angle);
		float newy = distance * sinf(angle);

		if(target->IsPlayer() && TO_PLAYER(target)->m_isMoving)
		{
			// cater for moving player vector based on orientation
			newx -= cosf(target->GetOrientation());
			newy -= sinf(target->GetOrientation());
		}
		MoveTo(localX+newx, localY+newy, targetsZ, angle);
	}
}

float AIInterface::calcCombatRange(Unit* target, bool ranged)
{
	float range = 0.0f;
	if(target != NULL)
	{
		float rangedthreshold = ranged ? 5.0f : 0.0f;
		float selfreach = m_Self->GetCombatReach();
		float targetradius = 0.0f;
		targetradius = target->GetModelHalfSize();
		float selfradius;
		selfradius = m_Self->GetModelHalfSize();
		range = targetradius + selfreach + selfradius + rangedthreshold;
	}
	return range;
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

void AIInterface::SendMoveToPacket(float & toX, float & toY, float & toZ, float & toO, uint32 time, uint32 MoveFlags)
{
	//this should NEVER be called directly !!!!!!
	//use MoveTo()

	WorldPacket data(SMSG_MONSTER_MOVE, 60);

	data << m_Self->GetNewGUID();
	data << uint8(1); //VLack: for 3.1.x support; I've discovered this in Mangos code while doing research on how to fix invisible mobs on 3.0.9
	data << float( m_Self->GetPositionX() );
    data << float( m_Self->GetPositionY() );
    data << float( m_Self->GetPositionZ() );
	data << uint32( getMSTime() );
	
	// Check if we have an orientation
	if(toO != 0.0f)
	{
		data << uint8( 4 );
		data << float( toO );
	} 
	else 
		data << uint8( 0 );
	data << uint32( MoveFlags );
	data << uint32( time );
	data << uint32( 1 );	  // 1 waypoint
	data << float( toX );
	data << float( toY );
	data << float( toZ );

#ifndef ENABLE_COMPRESSED_MOVEMENT_FOR_CREATURES
	m_Self->SendMessageToSet( &data, true );

#else
	if( m_Self->GetTypeId() == TYPEID_PLAYER )
		static_cast<Player*>(m_Self)->GetSession()->SendPacket(&data);

	for(set<Player*>::iterator itr = m_Self->GetInRangePlayerSetBegin(); itr != m_Self->GetInRangePlayerSetEnd(); ++itr)
	{
		if( (*itr)->GetPositionNC().Distance2DSq( m_Self->GetPosition() ) >= World::m_movementCompressThresholdCreatures )
			(*itr)->AppendMovementData( SMSG_MONSTER_MOVE, data.GetSize(), (const uint8*)data.GetBufferPointer() );
		else
			(*itr)->GetSession()->SendPacket(&data);
	}
#endif
}

void AIInterface::StopMovement(uint32 time)
{
	m_moveTimer = (int32)time; //set pause after stopping
	m_creatureState = STOPPED;

	m_destVector.x = 0, m_destVector.y  = 0, m_destVector.z = 0;
	m_timeMoved = 0, m_timeToMove = 0;

	WorldPacket data(27);
	data.SetOpcode(SMSG_MONSTER_MOVE);
	data << m_Self->GetNewGUID();
	data << uint8(0); //VLack: 3.1 SMSG_MONSTER_MOVE change...
	data << m_Self->GetPositionX() << m_Self->GetPositionY() << m_Self->GetPositionZ();
	data << getMSTime();
	data << uint8(1);   // "DontMove = 1"

	m_Self->SendMessageToSet( &data, false );
	clearBehavior(BEHAVIOR_UNITISMOVING);
}
uint32 AIInterface::calcTimeToDestination(float & x, float & y, float & z)
{
	uint32 time = 0;
	uint32 distance = (uint32)float2int32(m_Self->CalcDistance(x,y,z) );
	if( distance > 1.0f)
	{
		if( m_moveMode == AIMOVESTATE_RUN)
			time = (uint32)(distance / m_runSpeed);
		else if(m_moveMode == AIMOVESTATE_WALK)
			time = (uint32)(distance / m_walkSpeed);
		else if(m_moveMode == AIMOVESTATE_FLY)
			time = (uint32)(distance / m_flySpeed);
	}
	return time;
}

void AIInterface::MoveTo(float x, float y, float z, float o)
{
	if(m_moveMode == AIMOVESTATE_CANTMOVE || hasBehavior(BEHAVIOR_ISCASTINGMOVEBLOCK) )
	{
		if(m_creatureState == MOVING)
			StopMovement(0);
		return;
	}
	//Paroxysm : Since we are casting, we will wait 500 ms and then try to move if we are not casting.
	if(m_Self->GetCurrentSpell() != NULL)
	{
		setBehavior(BEHAVIOR_ISCASTINGMOVEBLOCK);
		eventClearBehavior(BEHAVIOR_ISCASTINGMOVEBLOCK,500);
	}
	else
	{
		m_destVector.x = x, m_destVector.y = y, m_destVector.z = z;
		if(AIType_isMob() )
		{
			//uint32 wpmovetype = TO_AIMOB(this)->getWPMoveType();
			//if(wpmovetype == MOVEMENTTYPE_DONTMOVEWP || wpmovetype == MOVEMENTTYPE_NONE)
			//{
				uint32 timeToDestination = calcTimeToDestination(x, y, z);
				if(timeToDestination)
					SendMoveToPacket(x,y,z,o, timeToDestination, getMoveFlags() );
			//}
		}
		if ( m_creatureState != MOVING )
			UpdateMove();
	}
}
void AIInterface::OnStopMovement()
{
	uint8 state = getAIState();
	//Handle evade
	if(state == STATE_EVADE)
	{
		if(m_destVector != m_returnToVector)
			MoveTo(m_returnToVector.x,m_returnToVector.y,m_returnToVector.z,m_returnToVector.o);
		else
			setAIState(STATE_IDLE); //Simply change to idle.
	}
	//Refresh our state
	state = getAIState();
	if(state == STATE_IDLE || state ==  STATE_SCRIPTIDLE)
	{
		if(hasWaypoints() )
			TO_AIMOB(this)->handleWaypointMovement();
		else if( getUnitToFollow() != NULL)
			handleFollowUnitMovement();
	}
	//Handle fearmovement
	if( (state == STATE_FLEEING || state == STATE_FEAR) && !hasBehavior(BEHAVIOR_BLOCKPANICMOVEMENT) )
		handleFearMovement();
	if( state == STATE_WANDER && !hasBehavior(BEHAVIOR_BLOCKDISORIENTMOVEMENT) )
		handleWanderMovement();
}

bool AIInterface::isFlying()
{
	bool result = false;
	if(m_moveMode == AIMOVESTATE_FLY || (m_Self->IsPlayer() && TO_PLAYER(m_Self)->FlyCheat) )
		result = true;
	return result;
}

uint32 AIInterface::getMoveFlags()
{
	uint32 MoveFlags = 0;
	static uint32 freeRoam = 0x00040000;
	static uint32 animation = 0x20000000;
	static uint32 finalO = 0x00020000;
	if(m_moveMode == AIMOVESTATE_FLY) //Fly
	{
		m_flySpeed = m_Self->m_flySpeed*0.001f;
		MoveFlags = 0x2000; //VLack: flight flag changed on 3.1.1
	}
	else if(m_moveMode == AIMOVESTATE_SPRINT) //Sprint
	{
		m_runSpeed = (m_Self->m_runSpeed+5.0f)*0.001f;
		MoveFlags = 0x0002000; //VLack: on 3.1.1 0x100 would teleport the NPC, and this is not what we want here!
	}
	else if(m_moveMode == AIMOVESTATE_RUN) //Run
	{
		m_runSpeed = m_Self->m_runSpeed*0.001f;
		MoveFlags = 0x1000; //VLack: on 3.1.1 0x100 would teleport the NPC, and this is not what we want here!
	}
	else
	{
		m_walkSpeed = m_Self->m_walkSpeed*0.001f;//move distance per ms time 
		MoveFlags = 0x1000;
	}
	//MoveFlags |= (freeRoam | finalO | animation);
	return MoveFlags;
}

void AIInterface::UpdateMove()
{
	uint32 moveTime = calcTimeToDestination(m_destVector.x, m_destVector.y, m_destVector.z);
	if(moveTime)
	{
		m_totalMoveTime = moveTime;
		m_timeToMove = moveTime;
		m_timeMoved = 0;
		if(m_moveTimer <= 0)
			m_moveTimer =  UNIT_MOVEMENT_INTERPOLATE_INTERVAL; // update every few msecs

		m_creatureState = MOVING;
	}
}

void AIInterface::SendCurrentMove(Player* plyr/*uint64 guid*/)
{
	if(m_destVector.x < FLOAT_E && m_destVector.y < FLOAT_E && m_destVector.z < FLOAT_E) return; //invalid move 
	ByteBuffer *splineBuf = new ByteBuffer(20*4);
	*splineBuf << uint32(0); // spline flags
	*splineBuf << uint32((m_totalMoveTime - m_timeToMove)+m_moveTimer); //Time Passed (start Position) //should be generated/save 
	*splineBuf << uint32(m_totalMoveTime); //Total Time //should be generated/save
	*splineBuf << uint32(0); //Unknown
	*splineBuf << uint32(4); //Spline Count	// lets try this

	*splineBuf << m_Self->GetPositionX() << m_Self->GetPositionY() << m_Self->GetPositionZ();
	*splineBuf << m_Self->GetPositionX() << m_Self->GetPositionY() << m_Self->GetPositionZ();
	*splineBuf << m_destVector.x << m_destVector.y << m_destVector.z + 0.1f;
	*splineBuf << m_destVector.x << m_destVector.y << m_destVector.z + 0.2f;
	*splineBuf << uint8(0);
	*splineBuf << m_destVector.z << m_destVector.z << m_destVector.z;
	
	plyr->AddSplinePacket(m_Self->GetGUID(), splineBuf);

}
void AIInterface::Update(uint32 p_time) 
{
	m_UpdateTime -= p_time;
	if(m_UpdateTime <= 0)
	{
		OnUpdate();
		m_UpdateTime = AIINTERFACE_UPDATE_INTERVAL;
	}
}
void AIInterface::_UpdateMovement(uint32 p_time)
{
	if(!m_Self->IsStunned() )
	{
		m_moveTimer -= p_time;
		m_timeMoved += p_time;
	}
}

void AIInterface::handleMovement()
{
	if(!sWorld.getAllowMovement() || m_moveMode == AIMOVESTATE_CANTMOVE)
		return;
	uint8 state = getAIState();
	if(m_creatureState == MOVING && m_moveTimer <= 0)
	{
		if(m_timeMoved >= m_timeToMove) //reached destination
		{
			m_creatureState = STOPPED;
			if(m_moveMode == AIMOVESTATE_SPRINT)
				m_moveMode = AIMOVESTATE_RUN;
			m_Self->SetPosition(m_destVector.x, m_destVector.y, m_destVector.z,m_destVector.o, true);
			m_destVector.x = 0.0f;
			m_destVector.y = 0.0f;
			m_destVector.z = 0.0f;
			m_timeMoved = m_timeToMove = 0;
			//Handle waypoints here.
			if( hasWaypoints() && ( state == STATE_IDLE || state == STATE_SCRIPTMOVE) )
				TO_AIMOB(this)->OnReachWp();
		}
		else
		{
			//Move Server Side Update
			float q = (float)m_timeMoved / m_timeToMove;
			float x = m_Self->GetPositionX() + (m_destVector.x - m_Self->GetPositionX()) * q;
			float y = m_Self->GetPositionY() + (m_destVector.y - m_Self->GetPositionY()) * q;
			float z = m_Self->GetPositionZ() + (m_destVector.z - m_Self->GetPositionZ()) * q;

			//Andy
			if (sWorld.Collision) 
			{
				float target_land_z= 0.0f;
				if( m_Self->GetMapMgr() != NULL )
				{
					if(m_moveMode != AIMOVESTATE_FLY)
					{
						target_land_z = CollideInterface.GetHeight(m_Self->GetMapId(), x, y, z + 2.0f);
						if ( target_land_z == NO_WMO_HEIGHT )
						{
							target_land_z = m_Self->GetMapMgr()->GetLandHeight(x, y);
							if ( target_land_z == 999999.0f )
								target_land_z = z;
						}
					}

					if ( z > m_Self->GetMapMgr()->GetWaterHeight(m_destVector.x, m_destVector.y) && target_land_z != 0.0f )
						z = target_land_z;
				}
			}
			m_Self->SetPosition(x, y, z, m_Self->GetOrientation());
			if(!hasBehavior(BEHAVIOR_UNITISMOVING) )
				setBehavior(BEHAVIOR_UNITISMOVING);
			uint32 delta_time = m_timeToMove - m_timeMoved;
			m_moveTimer = (UNIT_MOVEMENT_INTERPOLATE_INTERVAL < delta_time) ? UNIT_MOVEMENT_INTERPOLATE_INTERVAL : delta_time;
		}
		//UpdateMove();
	}
	/* So we have stopped moving and test our behavior to make sure we don't call this multiple times while standing still */
	else if(m_creatureState == STOPPED && hasBehavior(BEHAVIOR_UNITISMOVING) )
	{
		OnStopMovement();
		clearBehavior(BEHAVIOR_UNITISMOVING);
	}
}
void AIInterface::handleFearMovement()
{
	Unit * unitToFear  = getUnitToFear();
	uint32 fearTime = 0;

	if(unitToFear == NULL || m_moveMode == AIMOVESTATE_CANTMOVE)
		return;

	float Fx = 0,Fy = 0,Fz = 0;

	if( sWorld.DisableFearMovement )
	{
		if( m_Self->GetMapId() == 529|| m_Self->GetMapId() == 566 || m_Self->GetMapId() == 489 || m_Self->GetMapId() == 572 || m_Self->GetMapId() == 562 || m_Self->GetMapId() == 559 )
			return;
	}
	//Block further fear movement.
	setBehavior(BEHAVIOR_BLOCKPANICMOVEMENT);
	// Calculate new angle to target.
	float Fo = m_Self->calcRadAngle(unitToFear->GetPositionX(), unitToFear->GetPositionY(), m_Self->GetPositionX(), m_Self->GetPositionY());
	double fAngleAdd = RandomDouble(((M_PI/2) * 2)) - (M_PI/2);
	Fo += (float)fAngleAdd;

	float dist = m_Self->CalcDistance(unitToFear);
	if(dist > 30.0f || (Rand(25) && dist > 10.0f))	// not too far or too close
	{
		if( m_Self->GetMapId() == 572 || m_Self->GetMapId() == 562 || m_Self->GetMapId() == 559 ) //GET MAP ID
		{
			Fx = m_Self->GetPositionX();
			Fy = m_Self->GetPositionY();
		}
		else 
		{
			Fx = m_Self->GetPositionX() - (RandomFloat(15.f)+5.0f)*cosf(Fo);
			Fy = m_Self->GetPositionY() - (RandomFloat(15.f)+5.0f)*sinf(Fo);
		}
	}
	else
	{
		Fx = m_Self->GetPositionX() + (RandomFloat(20.f)+5.0f)*cosf(Fo);
		Fy = m_Self->GetPositionY() + (RandomFloat(20.f)+5.0f)*sinf(Fo);
	}
	// Check if this point is in water.
	float wl = m_Self->GetMapMgr()->GetWaterHeight(Fx, Fy);
	//			uint8 wt = m_Unit->GetMapMgr()->GetWaterType(Fx, Fy);

	if (sWorld.Collision) {
		Fz = CollideInterface.GetHeight(m_Self->GetMapId(), Fx, Fy, m_Self->GetPositionZ() + 2.0f);
		if( Fz == NO_WMO_HEIGHT )
			Fz = m_Self->GetMapMgr()->GetLandHeight(Fx, Fy);
		/*else
		{
			if( CollideInterface.GetFirstPoint(m_Unit->GetMapId(), m_Unit->GetPositionX(), m_Unit->GetPositionY(), m_Unit->GetPositionZ() + 2.0f,
				Fx, Fy, Fz + 2.0f, Fx, Fy, Fz, -1.0f) )
			{
				//Fz = CollideInterface.GetHeight(m_Unit->GetMapId(), Fx, Fy, m_Unit->GetPositionZ() + 2.0f);
			}
		}*/

		if( fabs( m_Self->GetPositionZ() - Fz ) > 10.0f || ( wl != 0.0f && Fz < wl ) )		// in water
			(void)0;
		else if( CollideInterface.CheckLOS(m_Self->GetMapId(), m_Self->GetPositionX(), m_Self->GetPositionY(), m_Self->GetPositionZ() + 2.0f, Fx, Fy, Fz) )
		{
			MoveTo(Fx, Fy, Fz, Fo);
			fearTime = (uint32)( m_Self->CalcDistance(Fx,Fy,Fz) / m_runSpeed );
		}
	}
	else 
	{
		Fz = m_Self->GetMapMgr()->GetLandHeight(Fx, Fy);
		if(fabs(m_Self->GetPositionZ()-Fz) > 4 || (Fz != 0.0f && Fz < (wl-2.0f)))
			fearTime = 100;
		else
		{
			MoveTo(Fx, Fy, Fz, Fo);
			fearTime = (uint32)(m_Self->CalcDistance(Fx,Fy,Fz) / m_runSpeed + 5 );
		}
	}
	//Set the fear bit in the specified time.
	if(fearTime)
		eventClearBehavior(BEHAVIOR_BLOCKPANICMOVEMENT,fearTime);
	else
	{
		//Force AIIInterface:OnStopMovement()
		eventSetBehavior(BEHAVIOR_UNITISMOVING,500);
		eventClearBehavior(BEHAVIOR_BLOCKPANICMOVEMENT,500);
	}
}
void AIInterface::handleWanderMovement()
{
	if(m_moveMode == AIMOVESTATE_CANTMOVE) return;
	uint32 nextWandert = 0;

	setBehavior(BEHAVIOR_BLOCKDISORIENTMOVEMENT);
	// calculate a random distance and angle to move
	float wanderD = RandomFloat(2.0f) + 2.0f;
	float wanderO = RandomFloat(float(M_2PI) );
	float wanderX = m_Self->GetPositionX() + wanderD * cosf(wanderO);
	float wanderY = m_Self->GetPositionY() + wanderD * sinf(wanderO);

	if (sWorld.Collision) {
		float wanderZ = CollideInterface.GetHeight(m_Self->GetMapId(), wanderX, wanderY, m_Self->GetPositionZ() + 2.0f);
		float wanderZ2 = wanderZ;
		if( wanderZ == NO_WMO_HEIGHT )
			wanderZ = m_Self->GetMapMgr()->GetLandHeight(wanderX, wanderY);
		else
		{
			if( CollideInterface.GetFirstPoint(m_Self->GetMapId(), m_Self->GetPositionX(), m_Self->GetPositionY(), m_Self->GetPositionZ() + 2.0f,
				wanderX, wanderY, wanderZ + 2.0f, wanderX, wanderY, wanderZ, -1.0f) )
			{
				//wanderZ = CollideInterface.GetHeight(m_Unit->GetMapId(), wanderX, wanderY, m_Unit->GetPositionZ() + 2.0f);
			}
			else
				wanderZ = wanderZ2;
		}

		if( fabs( m_Self->GetPositionZ() - wanderZ ) > 10.0f )
			nextWandert =getMSTime() + 1000;
		else if(CollideInterface.CheckLOS(m_Self->GetMapId(), m_Self->GetPositionX(), m_Self->GetPositionY(), m_Self->GetPositionZ() + 2.0f, wanderX, wanderY, wanderZ))
		{
			m_Self->SetOrientation(wanderO);
			MoveTo(wanderX, wanderY, wanderZ, wanderO);
			nextWandert = (uint32)(m_Self->CalcDistance(wanderX,wanderY,wanderZ) / m_walkSpeed );
		}
		else
			StopMovement(0);
	} 
	else 
	{
		float wanderZ = m_Self->GetMapMgr()->GetLandHeight(wanderX, wanderY);

		// without these next checks we could fall through the "ground" (WMO) and get stuck
		// wander won't work correctly in cities until we get some way to fix this and remove these checks
		float currentZ = m_Self->GetPositionZ();
		float landZ = m_Self->GetMapMgr()->GetLandHeight(m_Self->GetPositionX(), m_Self->GetPositionY());

		if( currentZ > landZ + 1.0f // are we more than 1yd above ground? (possible WMO)
			|| wanderZ < currentZ - 5.0f // is our destination land height too low? (possible WMO)
			|| wanderZ > currentZ + wanderD) // is our destination too high to climb?
		{
			nextWandert = 1000; // wait 1 second before we try again
		}
		else
		{
			m_Self->SetOrientation(wanderO);
			MoveTo(wanderX, wanderY, wanderZ, wanderO);
			nextWandert = (uint32)(m_Self->CalcDistance(wanderX,wanderY,wanderZ) / m_walkSpeed );
		}
	}
	if(nextWandert)
		eventClearBehavior(BEHAVIOR_BLOCKDISORIENTMOVEMENT,nextWandert);
	else
	{
		//This will cause OnStopMovement to be called.
		eventSetBehavior(BEHAVIOR_UNITISMOVING,1000);
		eventClearBehavior(BEHAVIOR_BLOCKDISORIENTMOVEMENT,1000);
	}
}
void AIInterface::handleFollowUnitMovement()
{
	Unit * unitToFollow = getUnitToFollow();
	if(m_moveMode == AIMOVESTATE_CANTMOVE) return;

	if( unitToFollow->event_GetCurrentInstanceId() != m_Self->event_GetCurrentInstanceId() )
		setUnitToFollow(0);
	else
	{
		uint8 state = getAIState();
		if(state == STATE_IDLE || state == STATE_FOLLOWING)
		{
			float dist = m_Self->CalcDistance(unitToFollow);
			if (dist > formation.formationDistance)
			{
				LocationVector loc = unitToFollow->GetPosition();
				float angle = formation.formationAngle > 0 ? formation.formationAngle : 0;
				m_AIState = STATE_FOLLOWING;

				if(dist < 10.0f) //10 yards away lets run else we will loose the them
					m_moveMode = AIMOVESTATE_WALK;
				else if( dist > 30.0f)
					m_moveMode = AIMOVESTATE_SPRINT;
				else
					m_moveMode = AIMOVESTATE_RUN;
				float destX = loc.x + formation.formationDistance * cosf(loc.o + angle);
				float destY = loc.y + formation.formationDistance * sinf(loc.o + angle);
				MoveTo(destX,destY,loc.z,loc.o);
			}
		}
	}
}
/* Used alot by the other ai types */
Unit * AIInterface::getUnitToFollow()
{
	return m_Self->GetMapMgr()->GetUnit(formation.formationHead);
}
Unit * AIInterface::getSoulLinker()
{
	return m_Self->GetMapMgr()->GetUnit(m_soulLinker);
}
Unit * AIInterface::getForcedTarget()
{
	return m_Self->GetMapMgr()->GetUnit(m_ForcedTarget);
}
Unit * AIInterface::getUnitToFear()
{
	return m_Self->GetMapMgr()->GetUnit(m_UnitToFear);
}
Unit * AIInterface::findClosestTarget( bool usecombatrange)
{
	Unit * target = NULL;
	float dist = 999999.99f;
	float cDist = 0.0f;
	for(std::set<Object*>::iterator itr = m_Self->GetInRangeOppFactsSetBegin(); itr != m_Self->GetInRangeOppFactsSetEnd(); ++itr)
	{
		if( !(*itr)->IsUnit() || TO_UNIT((*itr))->IsStealth() || !isAttackable(m_Self,(*itr)) )
			continue;
		if(usecombatrange)
			dist = calcCombatRange( TO_UNIT(*itr), false);
		cDist = m_Self->CalcDistance((*itr));
		if(cDist < dist)
		{
			dist = cDist;
			target = TO_UNIT((*itr));
		}
	}
	return target;
}
Unit * AIInterface::findRandomTarget()
{
	std::vector<Unit*> targets;
	for(std::set<Object*>::iterator itr = m_Self->GetInRangeOppFactsSetBegin(); itr != m_Self->GetInRangeOppFactsSetEnd(); ++itr)
	{
		if( !(*itr)->IsUnit() || TO_UNIT((*itr))->IsStealth() || !isAttackable(m_Self,(*itr)) )
			continue;
		targets.push_back( TO_UNIT(*itr));
	}
	Unit * ret = NULL;
	if(targets.size() )
		ret = targets[RandomUInt(targets.size()-1)];
	return ret;
}

void AIInterface::wipeReferences()
{
	setNextSpell(NULL);
	setNextTarget(NULL);
	setForcedTarget(NULL);
	memset(&formation,0,sizeof(FORMATION_DATA));
	setUnitToFear(NULL);
	setSoulLinker(0);
	StopMovement(0);
	setAIState(STATE_EVADE);
	sEventMgr.RemoveEvents(m_Self,EVENT_AIIBEHAVIORMODIFY);
}
//This is shared by both mob, mcontrol, and guardian interfaces.
void AIInterface::handleSpellCast( AI_Spell * spell, Unit * target )
{
	bool success = false;
	if(spell->type == AISPELLTYPE_AOE )
	{
		AI_AOESpell * aoespell = static_cast<AI_AOESpell*>(spell);
		uint32 rAngle = 0;
		uint16 radius = 0;
		//If we have some random range of angle or radius, we calculate them and use that.
		if(aoespell->rdata != NULL)
		{
			if(aoespell->rdata->facing[0] && aoespell->rdata->facing[1] )
			{
				uint32 dTheta = aoespell->rdata->facing[1] - aoespell->rdata->facing[0];
				rAngle = rand() % dTheta + aoespell->rdata->facing[0];
			}
			if(aoespell->rdata->radius[0] && aoespell->rdata->radius[1] )
			{
				uint16 dR = aoespell->rdata->radius[1] - aoespell->rdata->radius[0];
				radius = rand() % dR + aoespell->rdata->radius[0];
			}
		}
		//Otherwise, we just use the starting_angle and radius
		if(rAngle == 0)
			rAngle = aoespell->facing;
		if(radius == 0)
			radius = aoespell->radius;

		float calc_o = m_Self->GetOrientation() + TO_RADS(rAngle);
		const LocationVector loc = target->GetPosition();
		float x = loc.x + (radius * cos(calc_o) );
		float y = loc.y + (radius * sin(calc_o) );
		//Calculated our location, now we cast.
		success = true;
		m_Self->CastSpellAoF(x,y,loc.z,aoespell->sEntry,false);
	}
	else if(spell->type == AISPELLTYPE_SUMMON)
	{
		AI_SummonSpell * sSpell = static_cast<AI_SummonSpell*>(spell);
		bool isSummonSpell = ((sSpell->summon_type & AI_SUMMONFLAG_SUMMONENTRY_IS_SUMMONSPELL) != 0);
		uint16 radius = 0;
		uint32 angle = 0;
		if(sSpell->rdata != NULL)
		{
			if(sSpell->rdata->facing[0])
			{
				uint32 dTheta = sSpell->rdata->facing[1] - sSpell->rdata->facing[0];
				angle = rand() % dTheta + sSpell->rdata->facing[0];
			}
			if(sSpell->rdata->radius[0] )
			{
				uint16 dR = sSpell->rdata->facing[1] - sSpell->rdata->facing[0];
				radius = rand() % dR + sSpell->rdata->facing[0];
			}
		}
		if(radius == 0)
			radius = sSpell->summon_radius;
		if(angle == 0)
			angle = (uint32)sSpell->facing;
		float summon_o = m_Self->GetOrientation() + TO_RADS(angle);
		const LocationVector loc = m_Self->GetPosition();
		float x = loc.x + (radius * cos(summon_o) );
		float y = loc.y + (radius * sin(summon_o) );
		for(uint32 i = 0; i < sSpell->summon_count; ++i)
		{
			if(isSummonSpell)
				m_Self->CastSpellAoF(x,y,loc.z,dbcSpell.LookupEntryForced(sSpell->summon_entry),false);
			else
			{
				if(sSpell->summon_type == AI_SUMMONFLAG_SUMMONTYPE_CREATURE)
				{
					MapMgr * map = m_Self->GetMapMgr();
					Creature * summon = map->CreateCreature(sSpell->summon_entry);
					summon->Load(CreatureProtoStorage.LookupEntry(sSpell->summon_entry),x,y,loc.z,loc.o);
					summon->SetInstanceID(m_Self->GetInstanceID() );
					summon->SetZoneId(m_Self->GetZoneId() );
					if(sSpell->summon_faction)
						summon->SetFaction(sSpell->summon_faction);
					else
						summon->SetFaction(summon->GetProto()->Faction);
					summon->AddToWorld(map);
				}
				else if(sSpell->summon_type == AI_SUMMONFLAG_SUMMONTYPE_GAMEOBJECT)
				{
					MapMgr * map = m_Self->GetMapMgr();
					GameObject * summon = map->CreateGameObject(sSpell->summon_entry);
					summon->CreateFromProto(sSpell->summon_entry,map->GetMapId(),x,y,loc.z,loc.o);
					summon->SetInstanceID(map->GetInstanceID() );
					summon->SetUInt64Value(OBJECT_FIELD_CREATED_BY,m_Self->GetGUID() );
					summon->AddToWorld(map);
				}
			}
			//Add our dTheta if it exists.
			if(sSpell->delta_angle )
			{
				summon_o = summon_o + TO_RADS(sSpell->delta_angle);
				x = loc.x + (radius * cos(summon_o) );
				y = loc.y + (radius * sin(summon_o) );
			}
		}
		success = true;
	}
	else if(spell->type == AISPELLTYPE_SUMMON_DEST)
	{
		AI_SummonSpellDest * aoe = static_cast<AI_SummonSpellDest*>(spell);
		bool isSummonSpell = ((aoe->summon_type & AI_SUMMONFLAG_SUMMONENTRY_IS_SUMMONSPELL) != 0);
		uint16 radius = 0;
		uint32 angle = 0;
		for(uint32 i = 0; i < aoe->vectorCount; ++i)
		{
			LocationVector & target_dest = aoe->dest[i];
			if(aoe->rdata != NULL)
			{
				if(aoe->rdata->facing[0] )
				{
					uint32 dTheta = aoe->rdata->facing[1] - aoe->rdata->facing[0];
					angle = rand() % dTheta + aoe->rdata->facing[0];
				}
				if(aoe->rdata->radius[0] )
				{
					uint16 dR = aoe->rdata->radius[1] - aoe->rdata->radius[0];
					radius = rand() % dR + aoe->rdata->radius[0];
				}
			}
			else
			{
				angle = aoe->facing;
				radius = aoe->summon_radius;
			}
			float summon_o = target_dest.o + TO_RADS(angle);
			float x = target_dest.x + (radius * cos(summon_o) );
			float y = target_dest.y + (radius * sin(summon_o) );
			for(uint32 i = 0; i < aoe->summon_count; ++i)
			{
				if(isSummonSpell)
					m_Self->CastSpellAoF(x,y,target_dest.z,dbcSpell.LookupEntryForced(aoe->summon_entry),false);
				else
				{
					if(aoe->summon_type == AI_SUMMONFLAG_SUMMONTYPE_CREATURE)
					{
						MapMgr * map = m_Self->GetMapMgr();
						Creature * summon = map->CreateCreature(aoe->summon_entry);
						summon->Load(CreatureProtoStorage.LookupEntry(aoe->summon_entry),x,y,target_dest.z,target_dest.o);
						summon->SetInstanceID(m_Self->GetInstanceID() );
						summon->SetZoneId(m_Self->GetZoneId() );
						if(aoe->summon_faction)
							summon->SetFaction(aoe->summon_faction);
						else
							summon->SetFaction(summon->GetProto()->Faction);
						summon->AddToWorld(map);
					}
					else if(aoe->summon_type == AI_SUMMONFLAG_SUMMONTYPE_GAMEOBJECT)
					{
						MapMgr * map = m_Self->GetMapMgr();
						GameObject * summon = map->CreateGameObject(aoe->summon_entry);
						summon->CreateFromProto(aoe->summon_entry,map->GetMapId(),x,y,target_dest.z,target_dest.o);
						summon->SetInstanceID(map->GetInstanceID() );
						summon->SetUInt64Value(OBJECT_FIELD_CREATED_BY,m_Self->GetGUID() );
						summon->AddToWorld(map);
					}
				}
				//Add our dTheta if it exists.
				if(aoe->delta_angle )
				{
					summon_o = summon_o + TO_RADS(aoe->delta_angle);
					x = target_dest.x + (radius * cos(summon_o) );
					y = target_dest.y + (radius * sin(summon_o) );
				}
			}
			success = true;
		}
	}
	else if(spell->type == AISPELLTYPE_AOE_HEAL)
	{
		AI_AOEHealSpell * aoe = static_cast<AI_AOEHealSpell*>(spell);
		const LocationVector & target_dest = m_Self->GetPosition();
		uint16 radius = 0;
		uint32 angle = 0;
		if(aoe->rdata != NULL)
		{
			if(aoe->rdata->facing[0] )
			{
				uint32 dT = aoe->rdata->facing[1] - aoe->rdata->facing[0];
				angle = rand() % dT + aoe->rdata->facing[0];
			}
			if(aoe->rdata->radius[0] )
			{
				uint16 dR = aoe->rdata->radius[1] - aoe->rdata->radius[0];
				radius = rand() % dR + aoe->rdata->facing[0];
			}
		}
		else
		{
			radius = aoe->heal_radius;
			angle = (uint32)aoe->facing;
		}
		float cast_o = target_dest.o + TO_RADS(angle);
		float x = target_dest.x + (radius * cos(cast_o) );
		float y = target_dest.y + (radius * sin(cast_o) );
		//Now we cast.
		m_Self->CastSpellAoF(x,y,target_dest.z,aoe->sEntry,false);
		success = true;
	}
	//For everything else.
	else
	{
		m_Self->CastSpell(target,spell->sEntry,false);
		success = true;
	}
	//Handle any chat messages.
	if(success && spell->interact != NULL)
	{
		if(spell->interact->soundId)
			m_Self->PlaySoundToSet(spell->interact->soundId);
		if(spell->interact->sendtxt != NULL)
			m_Self->SendChatMessage( (uint8)spell->interact->chat_type, (uint8)spell->interact->lang_type, spell->interact->sendtxt);
	}
}
