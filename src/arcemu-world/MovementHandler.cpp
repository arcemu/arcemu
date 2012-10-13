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
#define SWIMMING_TOLERANCE_LEVEL -0.08f
#define MOVEMENT_PACKET_TIME_DELAY 500

#ifdef WIN32

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#define DELTA_EPOCH_IN_USEC  11644473600000000ULL
uint32 TimeStamp()
{
	//return timeGetTime();

	FILETIME ft;
	uint64 t;
	GetSystemTimeAsFileTime(&ft);

	t = (uint64)ft.dwHighDateTime << 32;
	t |= ft.dwLowDateTime;
	t /= 10;
	t -= DELTA_EPOCH_IN_USEC;

	return uint32(((t / 1000000L) * 1000) + ((t % 1000000L) / 1000));
}

uint32 mTimeStamp()
{
	return timeGetTime();
}

#else

uint32 TimeStamp()
{
	struct timeval tp;
	gettimeofday(&tp, NULL);
	return (tp.tv_sec * 1000) + (tp.tv_usec / 1000);
}

uint32 mTimeStamp()
{
	struct timeval tp;
	gettimeofday(&tp, NULL);
	return (tp.tv_sec * 1000) + (tp.tv_usec / 1000);
}

#endif

void WorldSession::HandleMoveWorldportAckOpcode(WorldPacket & recv_data)
{
	GetPlayer()->SetPlayerStatus(NONE);
	if(_player->IsInWorld())
	{
		// get outta here
		return;
	}
	LOG_DEBUG("WORLD: got MSG_MOVE_WORLDPORT_ACK.");

	if(_player->m_CurrentTransporter && _player->GetMapId() != _player->m_CurrentTransporter->GetMapId())
	{
		/* wow, our pc must really suck. */
		Transporter* pTrans = _player->m_CurrentTransporter;

		float c_tposx = pTrans->GetPositionX() + _player->transporter_info.x;
		float c_tposy = pTrans->GetPositionY() + _player->transporter_info.y;
		float c_tposz = pTrans->GetPositionZ() + _player->transporter_info.z;


		_player->SetMapId(pTrans->GetMapId());
		_player->SetPosition(c_tposx, c_tposy, c_tposz, _player->GetOrientation());

		WorldPacket dataw(SMSG_NEW_WORLD, 20);

		dataw << pTrans->GetMapId();
		dataw << c_tposx;
		dataw << c_tposy;
		dataw << c_tposz;
		dataw << _player->GetOrientation();

		SendPacket(&dataw);
	}
	else
	{
		_player->m_TeleportState = 2;
		_player->AddToWorld();
	}
}

void WorldSession::HandleMoveTeleportAckOpcode(WorldPacket & recv_data)
{
	WoWGuid guid;
	recv_data >> guid;
	if(guid == _player->GetGUID())
	{
		if(sWorld.antihack_teleport && !(HasGMPermissions() && sWorld.no_antihack_on_gm) && _player->GetPlayerStatus() != TRANSFER_PENDING)
		{
			/* we're obviously cheating */
			sCheatLog.writefromsession(this, "Used teleport hack, disconnecting.");
			Disconnect();
			return;
		}

		if(sWorld.antihack_teleport && !(HasGMPermissions() && sWorld.no_antihack_on_gm) && _player->m_position.Distance2DSq(_player->m_sentTeleportPosition) > 625.0f)	/* 25.0f*25.0f */
		{
			/* cheating.... :( */
			sCheatLog.writefromsession(this, "Used teleport hack {2}, disconnecting.");
			Disconnect();
			return;
		}

		LOG_DEBUG("WORLD: got MSG_MOVE_TELEPORT_ACK.");
		GetPlayer()->SetPlayerStatus(NONE);
		if(GetPlayer()->m_rooted <= 0)
			GetPlayer()->SetMovement(MOVE_UNROOT, 5);
		_player->SpeedCheatReset();

		std::list<Pet*> summons = _player->GetSummons();
		for(std::list<Pet*>::iterator itr = summons.begin(); itr != summons.end(); ++itr)
		{
			// move pet too
			(*itr)->SetPosition((GetPlayer()->GetPositionX() + 2), (GetPlayer()->GetPositionY() + 2), GetPlayer()->GetPositionZ(), M_PI_FLOAT);
		}
		if(_player->m_sentTeleportPosition.x != 999999.0f)
		{
			_player->m_position = _player->m_sentTeleportPosition;
			_player->m_sentTeleportPosition.ChangeCoords(999999.0f, 999999.0f, 999999.0f);
		}
	}

}

void _HandleBreathing(MovementInfo & movement_info, Player* _player, WorldSession* pSession)
{

	// no water breathing is required
	if(!sWorld.BreathingEnabled || _player->FlyCheat || _player->m_bUnlimitedBreath || !_player->isAlive() || _player->GodModeCheat)
	{
		// player is flagged as in water
		if(_player->m_UnderwaterState & UNDERWATERSTATE_SWIMMING)
			_player->m_UnderwaterState &= ~UNDERWATERSTATE_SWIMMING;

		// player is flagged as under water
		if(_player->m_UnderwaterState & UNDERWATERSTATE_UNDERWATER)
		{
			_player->m_UnderwaterState &= ~UNDERWATERSTATE_UNDERWATER;
			WorldPacket data(SMSG_START_MIRROR_TIMER, 20);
			data << uint32(TIMER_BREATH) << _player->m_UnderwaterTime << _player->m_UnderwaterMaxTime << int32(-1) << uint32(0);

			pSession->SendPacket(&data);
		}

		// player is above water level
		if(pSession->m_bIsWLevelSet)
		{
			if((movement_info.z + _player->m_noseLevel) > pSession->m_wLevel)
			{
				_player->RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_LEAVE_WATER);

				// unset swim session water level
				pSession->m_bIsWLevelSet = false;
			}
		}

		return;
	}

	//player is swimming and not flagged as in the water
	if(movement_info.flags & MOVEFLAG_SWIMMING && !(_player->m_UnderwaterState & UNDERWATERSTATE_SWIMMING))
	{
		_player->RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_ENTER_WATER);

		// get water level only if it was not set before
		if(!pSession->m_bIsWLevelSet)
		{
			// water level is somewhere below the nose of the character when entering water
			pSession->m_wLevel = movement_info.z + _player->m_noseLevel * 0.95f;
			pSession->m_bIsWLevelSet = true;
		}

		_player->m_UnderwaterState |= UNDERWATERSTATE_SWIMMING;
	}

	// player is not swimming and is not stationary and is flagged as in the water
	if(!(movement_info.flags & MOVEFLAG_SWIMMING) && (movement_info.flags != MOVEFLAG_MOVE_STOP) && (_player->m_UnderwaterState & UNDERWATERSTATE_SWIMMING))
	{
		// player is above water level
		if((movement_info.z + _player->m_noseLevel) > pSession->m_wLevel)
		{
			_player->RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_LEAVE_WATER);

			// unset swim session water level
			pSession->m_bIsWLevelSet = false;

			_player->m_UnderwaterState &= ~UNDERWATERSTATE_SWIMMING;
		}
	}

	// player is flagged as in the water and is not flagged as under the water
	if(_player->m_UnderwaterState & UNDERWATERSTATE_SWIMMING && !(_player->m_UnderwaterState & UNDERWATERSTATE_UNDERWATER))
	{
		//the player is in the water and has gone under water, requires breath bar.
		if((movement_info.z + _player->m_noseLevel) < pSession->m_wLevel)
		{
			_player->m_UnderwaterState |= UNDERWATERSTATE_UNDERWATER;
			WorldPacket data(SMSG_START_MIRROR_TIMER, 20);
			data << uint32(TIMER_BREATH) << _player->m_UnderwaterTime << _player->m_UnderwaterMaxTime << int32(-1) << uint32(0);

			pSession->SendPacket(&data);
		}
	}

	// player is flagged as in the water and is flagged as under the water
	if(_player->m_UnderwaterState & UNDERWATERSTATE_SWIMMING && _player->m_UnderwaterState & UNDERWATERSTATE_UNDERWATER)
	{
		//the player is in the water but their face is above water, no breath bar needed.
		if((movement_info.z + _player->m_noseLevel) > pSession->m_wLevel)
		{
			_player->m_UnderwaterState &= ~UNDERWATERSTATE_UNDERWATER;
			WorldPacket data(SMSG_START_MIRROR_TIMER, 20);
			data << uint32(TIMER_BREATH) << _player->m_UnderwaterTime << _player->m_UnderwaterMaxTime << int32(10) << uint32(0);
			pSession->SendPacket(&data);
		}
	}

	// player is flagged as not in the water and is flagged as under the water
	if(!(_player->m_UnderwaterState & UNDERWATERSTATE_SWIMMING) && _player->m_UnderwaterState & UNDERWATERSTATE_UNDERWATER)
	{
		//the player is out of the water, no breath bar needed.
		if((movement_info.z + _player->m_noseLevel) > pSession->m_wLevel)
		{
			_player->m_UnderwaterState &= ~UNDERWATERSTATE_UNDERWATER;
			WorldPacket data(SMSG_START_MIRROR_TIMER, 20);
			data << uint32(TIMER_BREATH) << _player->m_UnderwaterTime << _player->m_UnderwaterMaxTime << int32(10) << uint32(0);
			pSession->SendPacket(&data);
		}
	}

}

struct MovementFlagName
{
	uint32 flag;
	const char* name;
};

static MovementFlagName MoveFlagsToNames[] =
{
	{ MOVEFLAG_MOVE_STOP, "MOVEFLAG_MOVE_STOP" },
	{ MOVEFLAG_MOVE_FORWARD, "MOVEFLAG_MOVE_FORWARD" },
	{ MOVEFLAG_MOVE_BACKWARD, "MOVEFLAG_MOVE_BACKWARD" },
	{ MOVEFLAG_STRAFE_LEFT, "MOVEFLAG_STRAFE_LEFT" },
	{ MOVEFLAG_STRAFE_RIGHT, "MOVEFLAG_STRAFE_RIGHT" },
	{ MOVEFLAG_TURN_LEFT, "MOVEFLAG_TURN_LEFT" },
	{ MOVEFLAG_TURN_RIGHT, "MOVEFLAG_TURN_RIGHT" },
	{ MOVEFLAG_PITCH_DOWN, "MOVEFLAG_PITCH_DOWN" },
	{ MOVEFLAG_PITCH_UP, "MOVEFLAG_PITCH_UP" },
	{ MOVEFLAG_WALK, "MOVEFLAG_WALK" },
	{ MOVEFLAG_TRANSPORT, "MOVEFLAG_TRANSPORT" },
	{ MOVEFLAG_NO_COLLISION, "MOVEFLAG_NO_COLLISION" },
	{ MOVEFLAG_ROOTED, "MOVEFLAG_ROOTED" },
	{ MOVEFLAG_REDIRECTED, "MOVEFLAG_REDIRECTED" },
	{ MOVEFLAG_FALLING, "MOVEFLAG_FALLING" },
	{ MOVEFLAG_FALLING_FAR, "MOVEFLAG_FALLING_FAR" },
	{ MOVEFLAG_FREE_FALLING, "MOVEFLAG_FREE_FALLING" },
	{ MOVEFLAG_TB_PENDING_STOP, "MOVEFLAG_TB_PENDING_STOP" },
	{ MOVEFLAG_TB_PENDING_UNSTRAFE, "MOVEFLAG_TB_PENDING_UNSTRAFE" },
	{ MOVEFLAG_TB_PENDING_FALL, "MOVEFLAG_TB_PENDING_FALL" },
	{ MOVEFLAG_TB_PENDING_FORWARD, "MOVEFLAG_TB_PENDING_FORWARD" },
	{ MOVEFLAG_TB_PENDING_BACKWARD, "MOVEFLAG_TB_PENDING_BACKWARD" },
	{ MOVEFLAG_SWIMMING, "MOVEFLAG_SWIMMING" },
	{ MOVEFLAG_FLYING_PITCH_UP, "MOVEFLAG_FLYING_PITCH_UP" },
	{ MOVEFLAG_CAN_FLY, "MOVEFLAG_CAN_FLY" },
	{ MOVEFLAG_AIR_SUSPENSION, "MOVEFLAG_AIR_SUSPENSION" },
	{ MOVEFLAG_AIR_SWIMMING, "MOVEFLAG_AIR_SWIMMING" },
	{ MOVEFLAG_SPLINE_MOVER, "MOVEFLAG_SPLINE_MOVER" },
	{ MOVEFLAG_SPLINE_ENABLED, "MOVEFLAG_SPLINE_ENABLED" },
	{ MOVEFLAG_WATER_WALK, "MOVEFLAG_WATER_WALK" },
	{ MOVEFLAG_FEATHER_FALL, "MOVEFLAG_FEATHER_FALL" },
	{ MOVEFLAG_LEVITATE, "MOVEFLAG_LEVITATE" },
	{ MOVEFLAG_LOCAL, "MOVEFLAG_LOCAL" },
};

static const uint32 nmovementflags = sizeof(MoveFlagsToNames) / sizeof(MovementFlagName);

void WorldSession::HandleMovementOpcodes(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	bool moved = true;

	if(_player->GetCharmedByGUID() || _player->GetPlayerStatus() == TRANSFER_PENDING || _player->GetTaxiState() || _player->getDeathState() == JUST_DIED)
		return;

	// spell cancel on movement, for now only fishing is added
	Object* t_go = _player->m_SummonedObject;
	if(t_go)
	{
		if(t_go->GetEntry() == GO_FISHING_BOBBER)
			TO_GAMEOBJECT(t_go)->EndFishing(GetPlayer(), true);
	}

	/************************************************************************/
	/* Clear standing state to stand.				                        */
	/************************************************************************/
	if(recv_data.GetOpcode() == MSG_MOVE_START_FORWARD)
		_player->SetStandState(STANDSTATE_STAND);

	/************************************************************************/
	/* Make sure the packet is the correct size range.                      */
	/************************************************************************/
	//if(recv_data.size() > 80) { Disconnect(); return; }

	/************************************************************************/
	/* Read Movement Data Packet                                            */
	/************************************************************************/
	WoWGuid guid;
	recv_data >> guid;
	movement_info.init(recv_data);

	if(guid != m_MoverWoWGuid.GetOldGuid())
	{
		return;
	}
	
	// Player is in control of some entity, so we move that instead of the player
	Unit *mover = _player->GetMapMgr()->GetUnit( m_MoverWoWGuid.GetOldGuid() );
	if( mover == NULL )
		return;

	/* Anti Multi-Jump Check */
	if(recv_data.GetOpcode() == MSG_MOVE_JUMP && _player->jumping == true && !GetPermissionCount())
	{
		sCheatLog.writefromsession(this, "Detected jump hacking");
		Disconnect();
		return;
	}
	if(recv_data.GetOpcode() == MSG_MOVE_FALL_LAND || movement_info.flags & MOVEFLAG_SWIMMING)
		_player->jumping = false;
	if(!_player->jumping && (recv_data.GetOpcode() == MSG_MOVE_JUMP || movement_info.flags & MOVEFLAG_FALLING))
		_player->jumping = true;

	/************************************************************************/
	/* Update player movement state                                         */
	/************************************************************************/

	uint16 opcode = recv_data.GetOpcode();
	switch(opcode)
	{
		case MSG_MOVE_START_FORWARD:
		case MSG_MOVE_START_BACKWARD:
			_player->moving = true;
			break;
		case MSG_MOVE_START_STRAFE_LEFT:
		case MSG_MOVE_START_STRAFE_RIGHT:
			_player->strafing = true;
			break;
		case MSG_MOVE_JUMP:
			_player->jumping = true;
			break;
		case MSG_MOVE_STOP:
			_player->moving = false;
			break;
		case MSG_MOVE_STOP_STRAFE:
			_player->strafing = false;
			break;
		case MSG_MOVE_FALL_LAND:
			_player->jumping = false;
			break;

		default:
			moved = false;
			break;
	}

#if 0

	LOG_DETAIL("Got %s", g_worldOpcodeNames[ opcode ].name);

	LOG_DETAIL("Movement flags");
	for(uint32 i = 0; i < nmovementflags; i++)
		if((movement_info.flags & MoveFlagsToNames[ i ].flag) != 0)
			LOG_DETAIL("%s", MoveFlagsToNames[ i ].name);

#endif

	if(moved)
	{
		if(!_player->moving && !_player->strafing && !_player->jumping)
		{
			_player->m_isMoving = false;
		}
		else
		{
			_player->m_isMoving = true;
		}
	}

	// Rotating your character with a hold down right click mouse button
	if(_player->GetOrientation() != movement_info.orientation)
		_player->isTurning = true;
	else
		_player->isTurning = false;


	/************************************************************************/
	/* Anti-Fly Hack Checks       - Alice : Disabled for now                                           */
	/************************************************************************/
	/*if( sWorld.antihack_flight && ( recv_data.GetOpcode() == CMSG_MOVE_FLY_START_AND_END || recv_data.GetOpcode() == CMSG_FLY_PITCH_DOWN_AFTER_UP ) && !( movement_info.flags & MOVEFLAG_SWIMMING || movement_info.flags & MOVEFLAG_FALLING || movement_info.flags & MOVEFLAG_FALLING_FAR || movement_info.flags & MOVEFLAG_FREE_FALLING ) && _player->flying_aura == 0 )
	{
		if( sWorld.no_antihack_on_gm && _player->GetSession()->HasGMPermissions() )
		{
			// Do nothing.
		}
		else
		{
			_player->BroadcastMessage( "Flyhack detected. In case the server is wrong then make a report how to reproduce this case. You will be logged out in 5 seconds." );
			sEventMgr.AddEvent( _player, &Player::_Kick, EVENT_PLAYER_KICK, 5000, 1, 0 );
		}
	} */

	if(!(HasGMPermissions() && sWorld.no_antihack_on_gm) && !_player->GetCharmedUnitGUID())
	{
		/************************************************************************/
		/* Anti-Teleport                                                        */
		/************************************************************************/
		if(sWorld.antihack_teleport && _player->m_position.Distance2DSq(movement_info.x, movement_info.y) > 3025.0f
		        && _player->m_runSpeed < 50.0f && !_player->transporter_info.guid)
		{
			sCheatLog.writefromsession(this, "Disconnected for teleport hacking. Player speed: %f, Distance traveled: %f", _player->m_runSpeed, sqrt(_player->m_position.Distance2DSq(movement_info.x, movement_info.y)));
			Disconnect();
			return;
		}
	}

	//update the detector
	if(sWorld.antihack_speed && !_player->GetTaxiState() && _player->transporter_info.guid == 0 && !_player->GetSession()->GetPermissionCount())
	{
		// simplified: just take the fastest speed. less chance of fuckups too
		float speed = (_player->flying_aura) ? _player->m_flySpeed : (_player->m_swimSpeed > _player-> m_runSpeed) ? _player->m_swimSpeed : _player->m_runSpeed;

		_player->SDetector->AddSample(movement_info.x, movement_info.y, getMSTime(), speed);

		if(_player->SDetector->IsCheatDetected())
			_player->SDetector->ReportCheater(_player);
	}

	/************************************************************************/
	/* Remove Emote State                                                   */
	/************************************************************************/
	if(_player->GetEmoteState())
		_player->SetEmoteState(0);

	/************************************************************************/
	/* Make sure the co-ordinates are valid.                                */
	/************************************************************************/
	if(!((movement_info.y >= _minY) && (movement_info.y <= _maxY)) || !((movement_info.x >= _minX) && (movement_info.x <= _maxX)))
	{
		Disconnect();
		return;
	}

	/************************************************************************/
	/* Dump movement flags - Wheee!                                         */
	/************************************************************************/
#if 0
	LOG_DEBUG("=========================================================");
	LOG_DEBUG("Full movement flags: 0x%.8X", movement_info.flags);
	uint32 z, b;
	for(z = 1, b = 1; b < 32;)
	{
		if(movement_info.flags & z)
			LOG_DEBUG("   Bit %u (0x%.8X or %u) is set!", b, z, z);

		z <<= 1;
		b += 1;
	}
	LOG_DEBUG("=========================================================");
#endif

	/************************************************************************/
	/* Orientation dumping                                                  */
	/************************************************************************/
#if 0
	LOG_DEBUG("Packet: 0x%03X (%s)", recv_data.GetOpcode(), LookupName(recv_data.GetOpcode(), g_worldOpcodeNames));
	LOG_DEBUG("Orientation: %.10f", movement_info.orientation);
#endif

	/************************************************************************/
	/* Calculate the timestamp of the packet we have to send out            */
	/************************************************************************/
	size_t pos = (size_t)m_MoverWoWGuid.GetNewGuidLen() + 1;
	uint32 mstime = mTimeStamp();
	int32 move_time;
	if(m_clientTimeDelay == 0)
		m_clientTimeDelay = mstime - movement_info.time;

	/************************************************************************/
	/* Copy into the output buffer.                                         */
	/************************************************************************/
	if(_player->m_inRangePlayers.size())
	{
		move_time = (movement_info.time - (mstime - m_clientTimeDelay)) + MOVEMENT_PACKET_TIME_DELAY + mstime;
		memcpy(&movement_packet[0], recv_data.contents(), recv_data.size());
		movement_packet[pos + 6] = 0;

		/************************************************************************/
		/* Distribute to all inrange players.                                   */
		/************************************************************************/
		for(set<Object*>::iterator itr = _player->m_inRangePlayers.begin(); itr != _player->m_inRangePlayers.end(); ++itr)
		{

			Player* p = TO< Player* >((*itr));

			*(uint32*)&movement_packet[pos + 6] = uint32(move_time + p->GetSession()->m_moveDelayTime);

#if defined(ENABLE_COMPRESSED_MOVEMENT) && defined(ENABLE_COMPRESSED_MOVEMENT_FOR_PLAYERS)
			if(_player->GetPositionNC().Distance2DSq(p->GetPosition()) >= World::m_movementCompressThreshold)
				p->AppendMovementData(recv_data.GetOpcode(), uint16(recv_data.size() + pos), movement_packet);
			else
				p->GetSession()->OutPacket(recv_data.GetOpcode(), uint16(recv_data.size() + pos), movement_packet);
#else
			p->GetSession()->OutPacket(recv_data.GetOpcode(), uint16(recv_data.size() + pos), movement_packet);
#endif
		}
	}

	/************************************************************************/
	/* Hack Detection by Classic	                                        */
	/************************************************************************/
	if(!movement_info.transGuid && recv_data.GetOpcode() != MSG_MOVE_JUMP && !_player->FlyCheat && !_player->flying_aura && !(movement_info.flags & MOVEFLAG_SWIMMING || movement_info.flags & MOVEFLAG_FALLING) && movement_info.z > _player->GetPositionZ() && movement_info.x == _player->GetPositionX() && movement_info.y == _player->GetPositionY())
	{
		WorldPacket data(SMSG_MOVE_UNSET_CAN_FLY, 13);
		data << _player->GetNewGUID();
		data << uint32(5);
		SendPacket(&data);
	}

	if((movement_info.flags & MOVEFLAG_AIR_SWIMMING) && !(movement_info.flags & MOVEFLAG_SWIMMING) && !(_player->flying_aura || _player->FlyCheat))
	{
		WorldPacket data(SMSG_MOVE_UNSET_CAN_FLY, 13);
		data << _player->GetNewGUID();
		data << uint32(5);
		SendPacket(&data);
	}

	/************************************************************************/
	/* Falling damage checks                                                */
	/************************************************************************/

	if(_player->blinked)
	{
		_player->blinked = false;
		_player->m_fallDisabledUntil = UNIXTIME + 5;
		_player->SpeedCheatDelay(2000);   //some say they managed to trigger system with knockback. Maybe they moved in air ?
	}
	else
	{
		if(recv_data.GetOpcode() == MSG_MOVE_FALL_LAND)
		{
			// player has finished falling
			//if z_axisposition contains no data then set to current position
			if(!mover->z_axisposition)
				mover->z_axisposition = movement_info.z;

			// calculate distance fallen
			uint32 falldistance = float2int32(mover->z_axisposition - movement_info.z);
			if(mover->z_axisposition <= movement_info.z)
				falldistance = 1;
			/*Safe Fall*/
			if((int)falldistance > mover->m_safeFall)
				falldistance -= mover->m_safeFall;
			else
				falldistance = 1;

			//checks that player has fallen more than 12 units, otherwise no damage will be dealt
			//falltime check is also needed here, otherwise sudden changes in Z axis position, such as using !recall, may result in death
			if( mover->isAlive() && !mover->bInvincible && ( falldistance > 12 ) && !mover->m_noFallDamage &&
				( ( mover->GetGUID() != _player->GetGUID() ) || ( !_player->GodModeCheat && ( UNIXTIME >= _player->m_fallDisabledUntil ) ) ) )
			{
				// 1.7% damage for each unit fallen on Z axis over 13
				uint32 health_loss = static_cast< uint32 >( mover->GetHealth() * ( falldistance - 12 ) * 0.017f );

				if( health_loss >= mover->GetHealth() )
					health_loss = mover->GetHealth();
#ifdef ENABLE_ACHIEVEMENTS
				else if( ( falldistance >= 65 ) && ( mover->GetGUID() == _player->GetGUID() ) )
				{
					// Rather than Updating achievement progress every time fall damage is taken, all criteria currently have 65 yard requirement...
					// Achievement 964: Fall 65 yards without dying.
					// Achievement 1260: Fall 65 yards without dying while completely smashed during the Brewfest Holiday.
					_player->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_FALL_WITHOUT_DYING, falldistance, Player::GetDrunkenstateByValue(_player->GetDrunkValue()), 0);
				}
#endif

				mover->SendEnvironmentalDamageLog(mover->GetGUID(), DAMAGE_FALL, health_loss);
				mover->DealDamage(mover, health_loss, 0, 0, 0);

				//_player->RemoveStealth(); // cebernic : why again? lost stealth by AURA_INTERRUPT_ON_ANY_DAMAGE_TAKEN already.
			}
			mover->z_axisposition = 0.0f;
		}
		else
			//whilst player is not falling, continuously update Z axis position.
			//once player lands this will be used to determine how far he fell.
			if(!(movement_info.flags & MOVEFLAG_FALLING))
				mover->z_axisposition = movement_info.z;
	}

	/************************************************************************/
	/* Transporter Setup                                                    */
	/************************************************************************/
	if( ( mover->transporter_info.guid != 0 ) && ( movement_info.transGuid.GetOldGuid() == 0 ) ){
		/* we left the transporter we were on */

		Transporter *transporter = objmgr.GetTransporter( Arcemu::Util::GUID_LOPART( mover->transporter_info.guid ) );
		if( transporter != NULL )
			transporter->RemovePassenger( mover );

		mover->transporter_info.guid = 0;
		_player->SpeedCheatReset();

	}else{
		if( movement_info.transGuid.GetOldGuid() != 0 ){

			if( mover->transporter_info.guid == 0 ){
				Transporter *transporter = objmgr.GetTransporter( Arcemu::Util::GUID_LOPART( movement_info.transGuid ) );
				if( transporter != NULL )
					transporter->AddPassenger( mover );
				
				/* set variables */
				mover->transporter_info.guid = movement_info.transGuid;
				mover->transporter_info.flags = movement_info.transUnk;
				mover->transporter_info.x = movement_info.transX;
				mover->transporter_info.y = movement_info.transY;
				mover->transporter_info.z = movement_info.transZ;
			
			}else{
				/* no changes */
				mover->transporter_info.flags = movement_info.transUnk;
				mover->transporter_info.x = movement_info.transX;
				mover->transporter_info.y = movement_info.transY;
				mover->transporter_info.z = movement_info.transZ;
			}
		}
	}
	
	/*float x = movement_info.x - movement_info.transX;
	float y = movement_info.y - movement_info.transY;
	float z = movement_info.z - movement_info.transZ;
	Transporter* trans = _player->m_CurrentTransporter;
	if(trans) sChatHandler.SystemMessageToPlr(_player, "Client t pos: %f %f\nServer t pos: %f %f   Diff: %f %f", x,y, trans->GetPositionX(), trans->GetPositionY(), trans->CalcDistance(x,y,z), trans->CalcDistance(movement_info.x, movement_info.y, movement_info.z));*/

	/************************************************************************/
	/* Anti-Speed Hack Checks                                               */
	/************************************************************************/



	/************************************************************************/
	/* Breathing System                                                     */
	/************************************************************************/
	_HandleBreathing(movement_info, _player, this);

	/************************************************************************/
	/* Remove Spells                                                        */
	/************************************************************************/
	uint32 flags = 0;
	if((movement_info.flags & MOVEFLAG_MOTION_MASK) != 0)
		flags |= AURA_INTERRUPT_ON_MOVEMENT;

	if(!(movement_info.flags & MOVEFLAG_SWIMMING || movement_info.flags & MOVEFLAG_FALLING))
		flags |= AURA_INTERRUPT_ON_LEAVE_WATER;
	if(movement_info.flags & MOVEFLAG_SWIMMING)
		flags |= AURA_INTERRUPT_ON_ENTER_WATER;
	if((movement_info.flags & MOVEFLAG_TURNING_MASK) || _player->isTurning)
		flags |= AURA_INTERRUPT_ON_TURNING;
	if(movement_info.flags & MOVEFLAG_REDIRECTED)
		flags |= AURA_INTERRUPT_ON_JUMP;

	_player->RemoveAurasByInterruptFlag(flags);

	/************************************************************************/
	/* Update our position in the server.                                   */
	/************************************************************************/

	// Player is the active mover
	if(m_MoverWoWGuid.GetOldGuid() == _player->GetGUID())
	{

		if(_player->m_CurrentTransporter == NULL)
		{
			if(!_player->SetPosition(movement_info.x, movement_info.y, movement_info.z, movement_info.orientation))
			{
				//extra check to set HP to 0 only if the player is dead (KillPlayer() has already this check)
				if(_player->isAlive())
				{
					_player->SetHealth(0);
					_player->KillPlayer();
				}

				MapInfo* pMapinfo = WorldMapInfoStorage.LookupEntry(_player->GetMapId());
				if(pMapinfo != NULL)
				{

					if(pMapinfo->type == INSTANCE_NULL || pMapinfo->type == INSTANCE_BATTLEGROUND)
					{
						_player->RepopAtGraveyard(_player->GetPositionX(), _player->GetPositionY(), _player->GetPositionZ(), _player->GetMapId());
					}
					else
					{
						_player->RepopAtGraveyard(pMapinfo->repopx, pMapinfo->repopy, pMapinfo->repopz, pMapinfo->repopmapid);
					}
				}
				else
				{
					_player->RepopAtGraveyard(_player->GetPositionX(), _player->GetPositionY(), _player->GetPositionZ(), _player->GetMapId());
				}//Teleport player to graveyard. Stops players from QQing..
			}
		}
	}
	else
	{
		if( !mover->isRooted() )
			mover->SetPosition(movement_info.x, movement_info.y, movement_info.z, movement_info.orientation);
	}
}

void WorldSession::HandleMoveTimeSkippedOpcode(WorldPacket & recv_data)
{

}

void WorldSession::HandleMoveNotActiveMoverOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	WoWGuid guid;
	recv_data >> guid;

	if(guid == m_MoverWoWGuid)
		return;

	movement_info.init(recv_data);

	if((guid != uint64(0)) && (guid == _player->GetCharmedUnitGUID()))
		m_MoverWoWGuid = guid;
	else
		m_MoverWoWGuid.Init(_player->GetGUID());

	// set up to the movement packet
	movement_packet[0] = m_MoverWoWGuid.GetNewGuidMask();
	memcpy(&movement_packet[1], m_MoverWoWGuid.GetNewGuid(), m_MoverWoWGuid.GetNewGuidLen());
}


void WorldSession::HandleSetActiveMoverOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	// set current movement object
	uint64 guid;
	recv_data >> guid;

	if(guid != m_MoverWoWGuid.GetOldGuid())
	{
		// make sure the guid is valid and we aren't cheating
		if(!(_player->m_CurrentCharm == guid) &&
		        !(_player->GetGUID() == guid))
		{
			if( _player->GetCurrentVehicle()->GetOwner()->GetGUID() != guid )
				return;
		}

		// generate wowguid
		if(guid != 0)
			m_MoverWoWGuid.Init(guid);
		else
			m_MoverWoWGuid.Init(_player->GetGUID());

		// set up to the movement packet
		movement_packet[0] = m_MoverWoWGuid.GetNewGuidMask();
		memcpy(&movement_packet[1], m_MoverWoWGuid.GetNewGuid(), m_MoverWoWGuid.GetNewGuidLen());
	}
}

void WorldSession::HandleMoveSplineCompleteOpcode(WorldPacket & recvPacket)
{

}

void WorldSession::HandleMountSpecialAnimOpcode(WorldPacket & recvdata)
{
	CHECK_INWORLD_RETURN

	WorldPacket data(SMSG_MOUNTSPECIAL_ANIM, 8);
	data << _player->GetGUID();
	_player->SendMessageToSet(&data, true);
}

void WorldSession::HandleWorldportOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint32 unk;
	uint32 mapid;
	float x, y, z, o;
	recv_data >> unk >> mapid >> x >> y >> z >> o;

	//printf("\nTEST: %u %f %f %f %f", mapid, x, y, z, o);

	if(!HasGMPermissions())
	{
		SendNotification("You do not have permission to use this function.");
		return;
	}

	LocationVector vec(x, y, z, o);
	_player->SafeTeleport(mapid, 0, vec);
}

void WorldSession::HandleTeleportToUnitOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint8 unk;
	Unit* target;
	recv_data >> unk;

	if(!HasGMPermissions())
	{
		SendNotification("You do not have permission to use this function.");
		return;
	}

	if((target = _player->GetMapMgr()->GetUnit(_player->GetSelection())) == NULL)
		return;

	_player->SafeTeleport(_player->GetMapId(), _player->GetInstanceID(), target->GetPosition());
}

void WorldSession::HandleTeleportCheatOpcode(WorldPacket & recv_data)
{

	CHECK_INWORLD_RETURN

	float x, y, z, o;
	LocationVector vec;

	if(!HasGMPermissions())
	{
		SendNotification("You do not have permission to use this function.");
		return;
	}

	recv_data >> x >> y >> z >> o;
	vec.ChangeCoords(x, y, z, o);
	_player->SafeTeleport(_player->GetMapId(), _player->GetInstanceID(), vec);
}

MovementInfo::MovementInfo(){
	time = 0;
	pitch = 0.0f;
	redirectSin = 0.0f;
	redirectCos = 0.0f;
	redirect2DSpeed = 0.0f;
	unk11 = 0;
	unk12 = 0;
	unk13 = 0;
	unklast = 0;
	unk_230 = 0;
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	orientation = 0.0f;
	flags = 0;
	redirectVelocity = 0.0f;
	transGuid = 0;
	transX = 0.0f;
	transY = 0.0f;
	transZ = 0.0f;
	transO = 0.0f;
	transUnk = 0.0f;
	transUnk_2 = 0;
}

void MovementInfo::init(WorldPacket & data)
{
	transGuid = 0;
	unk13 = 0;
	data >> flags >> unk_230 >> time;
	data >> x >> y >> z >> orientation;

	if(flags & MOVEFLAG_TRANSPORT)
	{
		data >> transGuid >> transX >> transY >> transZ >> transO >> transUnk >> transUnk_2;
	}
	if(flags & (MOVEFLAG_SWIMMING | MOVEFLAG_AIR_SWIMMING) || unk_230 & 0x20)
	{
		data >> pitch;
	}
	if(flags & MOVEFLAG_REDIRECTED)
	{
		data >> redirectVelocity >> redirectSin >> redirectCos >> redirect2DSpeed;
	}
	if(flags & MOVEFLAG_SPLINE_MOVER)
	{
		data >> unk12;
	}

	data >> unklast;
	if(data.rpos() != data.wpos())
	{
		if(data.rpos() + 4 == data.wpos())
			data >> unk13;
		else
			LOG_DEBUG("Extra bits of movement packet left");
	}
}

void MovementInfo::write(WorldPacket & data)
{
	data << flags << unk_230 << getMSTime();

	data << x << y << z << orientation;

	if(flags & MOVEFLAG_TRANSPORT)
	{
		data << transGuid << transX << transY << transZ << transO << transUnk << transUnk_2;
	}
	if(flags & MOVEFLAG_SWIMMING)
	{
		data << pitch;
	}
	if(flags & MOVEFLAG_FALLING)
	{
		data << redirectVelocity << redirectSin << redirectCos << redirect2DSpeed;
	}
	if(flags & MOVEFLAG_SPLINE_MOVER)
	{
		data << unk12;
	}
	data << unklast;
	if(unk13)
		data << unk13;
}
