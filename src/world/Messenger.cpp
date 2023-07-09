/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2019 <http://www.ArcEmu.org/>
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
#include "Messenger.h"
#include "MessageRouter.h"
#include "PlayerMessenger.h"

void Messenger::SendSpellLog(Object *Caster, Object *Target, uint32 Ability, uint8 SpellLogType)
{
	if(Caster == NULL || Target == NULL || Ability == 0)
		return;

	WorldPacket data(SMSG_SPELLLOGMISS, 26);

	data << uint32(Ability);					// spellid
	data << Caster->GetGUID();					// caster / player
	data << uint8(1);							// unknown but I think they are const
	data << uint32(1);						// unknown but I think they are const
	data << Target->GetGUID();					// target
	data << uint8(SpellLogType);				// spelllogtype

	Caster->SendMessageToSet(&data, true);
}

void Messenger::SendSpellNonMeleeDamageLog(Object* Caster, Object* Target, uint32 SpellID, uint32 Damage, uint8 School, uint32 AbsorbedDamage, uint32 ResistedDamage, bool PhysicalDamage, uint32 BlockedDamage, bool CriticalHit, bool bToset)
{
	if(!Caster || !Target || !SpellID)
		return;

	uint32 Overkill = 0;

	if(Damage > Target->GetUInt32Value(UNIT_FIELD_HEALTH))
		Overkill = Damage - Target->GetUInt32Value(UNIT_FIELD_HEALTH);

	WorldPacket data(SMSG_SPELLNONMELEEDAMAGELOG, 48);

	data << Target->GetNewGUID();
	data << Caster->GetNewGUID();
	data << uint32(SpellID);                      // SpellID / AbilityID
	data << uint32(Damage);                       // All Damage
	data << uint32(Overkill);					// Overkill
	data << uint8(g_spellSchoolConversionTable[School]);     // School
	data << uint32(AbsorbedDamage);               // Absorbed Damage
	data << uint32(ResistedDamage);               // Resisted Damage
	data << uint8(PhysicalDamage);        // Physical Damage (true/false)
	data << uint8(0);                     // unknown or it binds with Physical Damage
	data << uint32(BlockedDamage);		       // Physical Damage (true/false)

	// unknown const
	if(CriticalHit)
		data << uint8(7);
	else
		data << uint8(5);

	data << uint32(0);

	Caster->SendMessageToSet(&data, bToset);
}

void Messenger::SendAttackerStateUpdate( Object* Caster, Object* Target, dealdamage* Dmg, uint32 Damage, uint32 Abs, uint32 BlockedDamage, uint32 HitStatus, uint32 VState )
{
	if(!Caster || !Target || !Dmg)
		return;

	WorldPacket data(SMSG_ATTACKERSTATEUPDATE, 70);

	uint32 Overkill = 0;

	if(Damage > Target->GetUInt32Value(UNIT_FIELD_MAXHEALTH))
		Overkill = Damage - Target->GetUInt32Value(UNIT_FIELD_HEALTH);

	data << uint32(HitStatus);
	data << Caster->GetNewGUID();
	data << Target->GetNewGUID();

	data << uint32(Damage);						// Realdamage
	data << uint32(Overkill);					// Overkill
	data << uint8(1);					// Damage type counter / swing type

	data << uint32(g_spellSchoolConversionTable[Dmg->school_type]);				    // Damage school
	data << float(Dmg->full_damage);	// Damage float
	data << uint32(Dmg->full_damage);	// Damage amount

	if(HitStatus & HITSTATUS_ABSORBED)
	{
		data << uint32(Abs);				// Damage absorbed
	}

	if(HitStatus & HITSTATUS_RESIST)
	{
		data << uint32(Dmg->resisted_damage);	// Damage resisted
	}

	data << uint8(VState);
	data << uint32(0);				// can be 0,1000 or -1
	data << uint32(0);

	if(HitStatus & HITSTATUS_BLOCK)
	{
		data << uint32(BlockedDamage);		// Damage amount blocked
	}


	if(HitStatus & HITSTATUS_UNK2)
	{
		data << uint32(0);				// unknown
	}

	if(HitStatus & HITSTATUS_UNK)
	{
		data << uint32(0);
		data << float(0);
		data << float(0);
		data << float(0);
		data << float(0);
		data << float(0);
		data << float(0);
		data << float(0);
		data << float(0);

		data << float(0);   // Found in loop
		data << float(0);	// Found in loop
		data << uint32(0);
	}

	Caster->SendMessageToSet(&data, Caster->IsPlayer());
}

void Messenger::SendEnvironmentalDamageLog(Unit *unit, uint8 type, uint32 damage)
{
	WorldPacket data( SMSG_ENVIRONMENTALDAMAGELOG, 20 );
	data << uint64( unit->GetGUID() );
	data << uint8( type );
	data << uint32( damage );
	data << uint64( 0 );
	unit->SendMessageToSet( &data, true, false );
}

void Messenger::SendDamageShieldLog(Unit *unit, Unit *attacker, const DamageProc &ds)
{
	WorldPacket data( SMSG_SPELLDAMAGESHIELD, 24 );
	data << uint64( unit->GetGUID() );
	data << uint64( attacker->GetGUID() );
	data << uint32( ds.m_spellId );
	data << uint32( ds.m_damage );
	data << ( 1 << ds.m_school );
	unit->SendMessageToSet( &data, true );
}

void Messenger::PlaySoundToSet( Object* object, uint32 sound_entry)
{
	WorldPacket data( SMSG_PLAY_SOUND, 4 );
	data << uint32( sound_entry );

	MessageRouter router( object );
	router.sendMessageToPlayersInRange( &data, true );
}

void Messenger::PlaySoundToPlayer( Player *player, uint32 soundId )
{
	WorldPacket data( SMSG_PLAY_SOUND, 4 );
	data << uint32( soundId );

	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendAIReaction( Object* object, uint32 reaction)
{
	WorldPacket data(SMSG_AI_REACTION, 12);
	data << uint64(object->GetGUID());
	data << uint32(reaction);
	object->SendMessageToSet(&data, false);
}

void Messenger::SendDestroyObjectToSet( Object* object )
{
	WorldPacket data(SMSG_DESTROY_OBJECT, 9);
	data << uint64(object->GetGUID());
	data << uint8(0);
	object->SendMessageToSet(&data, false);
}

void Messenger::SendDestroyObject(Player *player, Object *object)
{
	WorldPacket data( SMSG_DESTROY_OBJECT, 9 );
	data << uint64( object->GetGUID() );
	data << uint8( 0 );

	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendDestroyObject(Player *player, uint64 guid)
{
	WorldPacket data( SMSG_DESTROY_OBJECT, 9 );
	data << uint64( guid );
	data << uint8( 0 );

	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendGameObjectCustomAnim( GameObject *go, uint32 anim, Player *player )
{
	WorldPacket  data(12);
	data.Initialize(SMSG_GAMEOBJECT_CUSTOM_ANIM);
	data << uint64( go->GetGUID() );
	data << uint32(anim); // value < 4
	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendGameObjectDespawnAnim( Object* object )
{
	WorldPacket data(SMSG_GAMEOBJECT_DESPAWN_ANIM, 8);
	data << uint64( object->GetGUID() );

	MessageRouter router( object );
	router.sendMessageToPlayersInRange( &data, false );
}

void Messenger::SendBuildingDamageToSet( GameObject *go, uint32 damage, uint64 AttackerGUID, uint64 ControllerGUID, uint32 SpellID )
{
	WorldPacket data( SMSG_DESTRUCTIBLE_BUILDING_DAMAGE, 29 );

	data << WoWGuid( go->GetNewGUID() );
	data << WoWGuid( AttackerGUID );
	data << WoWGuid( ControllerGUID );
	data << uint32( damage );
	data << uint32( SpellID );
	go->SendMessageToSet( &data, false, false );
}

void Messenger::SendTransferPendingTransport( Player* player, uint32 oldMap, uint32 newMap, uint32 transportEntry )
{
	WorldPacket data( SMSG_TRANSFER_PENDING, 12 );
	data << uint32( newMap );
	data << uint32( transportEntry );
	data << uint32( oldMap );
	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendTransferPending( Player *player, uint32 mapId )
{
	WorldPacket data( SMSG_TRANSFER_PENDING, 20 );
	data << uint32( mapId );
	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendNewWorld( Player *player, uint32 mapId, const LocationVector &location )
{
	WorldPacket data( SMSG_NEW_WORLD, 20 );
	data << uint32( mapId );
	data << float( location.x );
	data << float( location.y );
	data << float( location.z );
	data << float( location.o );
	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendTransferAborted( Player *player, uint32 mapId, uint32 cause )
{
	WorldPacket data( SMSG_TRANSFER_ABORTED, 41 );
	data << uint32( mapId );
	data << uint32( cause );
	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendTeleportAck( Player *player, const LocationVector &location )
{
	WorldPacket data( MSG_MOVE_TELEPORT_ACK, 80 );
	data << player->GetNewGUID();
	data << uint32( 2 );   // flags
	data << uint32( Arcemu::Shared::Util::getMSTime() );
	data << uint16( 0 );
	data << float( 0 );
	data << float( location.x );
	data << float( location.y );
	data << float( location.z );
	data << float( location.o );
	data << uint16( 2 );
	data << uint8( 0 );
	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendTriggerCinematic( Player *player, uint32 cinematicId )
{
	//player->GetSession()->OutPacket(SMSG_TRIGGER_CINEMATIC, 4, &cinematicId );
	WorldPacket data( SMSG_TRIGGER_CINEMATIC, 4 );
	data << uint32( cinematicId );
	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendStopAttackToSet( Unit *attacker, Unit *victim )
{
	WorldPacket data( SMSG_ATTACKSTOP, 24 );
	data << attacker->GetNewGUID();
	data << victim->GetNewGUID();
	data << uint32( 0 );
	attacker->SendMessageToSet( &data, true );
}

void Messenger::SendStopAttackToSet( Unit *attacker, uint64 victim )
{
	WorldPacket data( SMSG_ATTACKSTOP, 24 );
	data << attacker->GetNewGUID();
	FastGUIDPack( data, victim );
	data << uint32( 0 );
	attacker->SendMessageToSet( &data, attacker->IsPlayer() );
}

void Messenger::SendStopAttackToPlayer( Player *player, Unit *victim )
{
	WorldPacket data( SMSG_ATTACKSTOP, 24 );
	data << victim->GetNewGUID();
	data << uint8( 0 );
	data << uint32( 0 );
	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendStartAttackToSet(Unit *attacker, Unit *victim)
{
	WorldPacket data( SMSG_ATTACKSTART, 16 );
	data << attacker->GetGUID();
	data << victim->GetGUID();
	attacker->SendMessageToSet( &data, true );
}

void Messenger::SendRootToSet( Unit *unit )
{
	WorldPacket data( SMSG_FORCE_MOVE_ROOT, 12 );
	data << unit->GetNewGUID();
	data << uint32( 1 );
	unit->SendMessageToSet( &data, true, false );
}

void Messenger::SendUnRootToSet( Unit *unit )
{
	WorldPacket data( SMSG_FORCE_MOVE_UNROOT, 12 );
	data << unit->GetNewGUID();
	data << uint32( 5 );
	unit->SendMessageToSet( &data, true, false );
}

void Messenger::SendPlaySpellVisualToSet( Unit *target, uint32 spellVisual )
{
	WorldPacket data( SMSG_PLAY_SPELL_VISUAL, 12 );
	data << uint64( target->GetGUID() );
	data << uint32( spellVisual );
	target->SendMessageToSet( &data, true );
}

void Messenger::SendEmoteMessageToSet(Unit *target, uint32 emote)
{
	WorldPacket data( SMSG_EMOTE, 12 );
	data << uint32( emote );
	data << target->GetGUID();
	target->SendMessageToSet( &data, true );
}

void Messenger::SendPowerUpdate( Unit *unit, uint32 amount, bool self )
{
	WorldPacket data( SMSG_POWER_UPDATE, 14 );
	FastGUIDPack( data, unit->GetGUID() );
	data << uint8( unit->GetPowerType() );
	data << uint32( amount );

	MessageRouter router( unit );
	router.sendMessageToPlayersInRange( &data, self );
}

uint32 Messenger::SendFullAuraUpdate(Unit *unit)
{
	WorldPacket data(SMSG_AURA_UPDATE_ALL, 200);

	data << WoWGuid(unit->GetNewGUID());

	uint32 Updates = 0;

	for(uint32 i = MAX_TOTAL_AURAS_START; i < MAX_TOTAL_AURAS_END; ++i)
	{
		Aura* aur = unit->m_auras[ i ];

		if(aur != NULL)
		{
			uint8 Flags = uint8(aur->GetAuraFlags());

			Flags = (AFLAG_EFFECT_1 | AFLAG_EFFECT_2 | AFLAG_EFFECT_3);

			if(aur->IsPositive())
				Flags |= AFLAG_CANCELLABLE;
			else
				Flags |= AFLAG_NEGATIVE;

			if(aur->GetDuration() != 0)
				Flags |= AFLAG_DURATION;

			data << uint8(aur->m_visualSlot);
			data << uint32(aur->GetSpellId());
			data << uint8(Flags);
			data << uint8(unit->getLevel());
			data << uint8(unit->m_auraStackCount[ aur->m_visualSlot ]);

			if((Flags & AFLAG_NOT_CASTER) == 0)
				data << WoWGuid(aur->GetCasterGUID());

			if(Flags & AFLAG_DURATION)
			{
				data << uint32(aur->GetDuration());
				data << uint32(aur->GetTimeLeft());
			}

			++Updates;
		}
	}
	unit->SendMessageToSet(&data, true);

	return Updates;
}

void Messenger::SendAuraUpdate( Unit *unit, uint32 AuraSlot, bool remove )
{
	Aura* aur = unit->m_auras[ AuraSlot ];

	ARCEMU_ASSERT(aur != NULL);

	WorldPacket data(SMSG_AURA_UPDATE, 30);

	if(remove)
	{
		data << WoWGuid(unit->GetGUID());
		data << uint8(aur->m_visualSlot);
		data << uint32(0);
	}
	else
	{
		uint8 flags = (AFLAG_EFFECT_1 | AFLAG_EFFECT_2 | AFLAG_EFFECT_3);

		if(aur->IsPositive())
			flags |= AFLAG_CANCELLABLE;
		else
			flags |= AFLAG_NEGATIVE;

		if(aur->GetDuration() != 0)
			flags |= AFLAG_DURATION;

		data << WoWGuid(unit->GetGUID());
		data << uint8(aur->m_visualSlot);

		data << uint32(aur->GetSpellId());
		data << uint8(flags);

		Unit* caster = aur->GetUnitCaster();
		if(caster != NULL)
			data << uint8(caster->getLevel());
		else
			data << uint8(sWorld.m_levelCap);

		data << uint8(unit->m_auraStackCount[ aur->m_visualSlot ]);

		if((flags & AFLAG_NOT_CASTER) == 0)
			data << WoWGuid(aur->GetCasterGUID());

		if(flags & AFLAG_DURATION)
		{
			data << uint32(aur->GetDuration());
			data << uint32(aur->GetTimeLeft());
		}
	}

	unit->SendMessageToSet( &data, true );
}

void Messenger::SendEnableFlightMessage(Unit *unit)
{
	WorldPacket data(SMSG_MOVE_SET_CAN_FLY, 13);
	data << unit->GetNewGUID();
	data << uint32(2);
	unit->SendMessageToSet(&data, true);
}

void Messenger::SendDisableFlightMessage(Unit *unit)
{
	WorldPacket data(SMSG_MOVE_UNSET_CAN_FLY, 13);
	data << unit->GetNewGUID();
	data << uint32(5);
	unit->SendMessageToSet(&data, true);
}

void Messenger::SendPeriodicAuraLog(const WoWGuid &CasterGUID, Unit *target, uint32 SpellID, uint32 School, uint32 Amount, uint32 abs_dmg, uint32 resisted_damage, uint32 Flags, bool is_critical)
{
	WorldPacket data(SMSG_PERIODICAURALOG, 47);

	data << target->GetNewGUID();		   // target guid
	data << CasterGUID;		   // caster guid
	data << uint32(SpellID);						// spellid
	data << uint32(1);					    // unknown? need research?
	data << uint32(Flags | 0x1);			// aura school
	data << uint32(Amount);						   // amount of done to target / heal / damage
	data << uint32(0);				   // cebernic: unknown?? needs more research, but it should fix unknown damage type with suffered.
	data << uint32(g_spellSchoolConversionTable[School]);
	data << uint32(abs_dmg);
	data << uint32(resisted_damage);
	data << uint8(is_critical);

	target->SendMessageToSet(&data, true);
}

void Messenger::SendPeriodicHealAuraLog(const WoWGuid &CasterGUID, Unit *target, uint32 SpellID, uint32 healed, uint32 over_healed, bool is_critical)
{
	WorldPacket data(SMSG_PERIODICAURALOG, 41);

	data << target->GetNewGUID();
	data << CasterGUID;
	data << SpellID;
	data << uint32(1);
	data << uint32(FLAG_PERIODIC_HEAL);
	data << uint32(healed);
	data << uint32(over_healed);
	data << uint32(0);		// I don't know what it is. maybe something related to absorbed heal?
	data << uint8(is_critical);

	target->SendMessageToSet(&data, true);
}

void Messenger::SendHopOnVehicle( Unit *unit, Unit *vehicleowner, uint32 seat )
{
	WorldPacket data(SMSG_MONSTER_MOVE_TRANSPORT, 50);
	data << unit->GetNewGUID();
	data << vehicleowner->GetNewGUID();
	data << uint8( seat );

	if( unit->IsPlayer() )
		data << uint8( 1 );
	else
		data << uint8( 0 );

	data << float( unit->GetPositionX() /* - vehicleowner->GetPositionX() */ );
	data << float( unit->GetPositionY() /* - vehicleowner->GetPositionY() */ );
	data << float( unit->GetPositionZ() /* - vehicleowner->GetPositionZ() */ );
	data << Arcemu::Shared::Util::getMSTime();
	data << uint8( 4 ); // splinetype_facing_angle
	data << float( 0.0f ); // facing angle
	data << uint32( 0x00800000 ); // splineflag transport
	data << uint32( 0 ); // movetime
	data << uint32( 1 ); // wp count
	data << float( 0.0f ); // x
	data << float( 0.0f ); // y
	data << float( 0.0f ); // z

	unit->SendMessageToSet( &data, true );
}

void Messenger::SendHopOffVehicle( Unit *unit, Unit *vehicleowner, LocationVector &landposition )
{
	WorldPacket data(SMSG_MONSTER_MOVE, 1+12+4+1+4+4+4+12+8 );
	data << unit->GetNewGUID();

	if( unit->IsPlayer() )
		data << uint8( 1 );
	else
		data << uint8( 0 );

	data << float( unit->GetPositionX() );
	data << float( unit->GetPositionY() );
	data << float( unit->GetPositionZ() );
	data << uint32( Arcemu::Shared::Util::getMSTime() );
	data << uint8( 4 /* SPLINETYPE_FACING_ANGLE */ );
	data << float( unit->GetOrientation() );                        // guess
	data << uint32( 0x01000000 /* SPLINEFLAG_EXIT_VEHICLE */ );
	data << uint32( 0 );                                      // Time in between points
	data << uint32( 1 );                                      // 1 single waypoint
	data << float( vehicleowner->GetPositionX() );
	data << float( vehicleowner->GetPositionY() );
	data << float( vehicleowner->GetPositionZ() );

	unit->SendMessageToSet(&data, true);
}

void Messenger::SendSetFacing( Unit *unit, float orientation )
{
	WorldPacket data(SMSG_MONSTER_MOVE, 100);

	data << unit->GetNewGUID();

	if( unit->IsPlayer() )
		data << uint8( 1 );
	else
		data << uint8( 0 );

	data << float( unit->GetPositionX() );
	data << float( unit->GetPositionY() );
	data << float( unit->GetPositionZ() );
	data << Arcemu::Shared::Util::getMSTime();
	data << uint8(4);
	data << float( orientation );
	data << uint32(0x1000); //move flags: run
	data << uint32(0); //movetime
	data << uint32(1); //1 point
	data << float( unit->GetPositionX() );
	data << float( unit->GetPositionY() );
	data << float( unit->GetPositionZ() );

	unit->SendMessageToSet(&data, true);
}

void Messenger::SendChatMessageToSet( Unit *unit, uint8 emote, uint32 lang, const string& name, const string& message )
{
	WorldPacket data(SMSG_MESSAGECHAT, 35 + name.length() + 1 + message.length() + 1);
	data << uint8( emote );
	data << uint32( lang );
	data << unit->GetGUID();
	data << uint32(0);			// new in 2.1.0
	data << uint32(name.length() + 1);
	data << name.c_str();
	data << uint64(0);
	data << uint32(message.length() + 1);
	data << message.c_str();
	data << uint8(0);
	unit->SendMessageToSet(&data, true);
}

void Messenger::SendChatMessageToPlayer(Unit *unit, Player *player, uint8 emote, uint32 lang, const string &name, const string &message)
{
	WorldPacket data(SMSG_MESSAGECHAT, 35 + name.length() + 1 + message.length() + 1);
	data << uint8( emote );
	data << uint32( lang );
	data << unit->GetGUID();
	data << uint32(0);			// new in 2.1.0
	data << uint32(name.length() + 1);
	data << name.c_str();
	data << uint64(0);
	data << uint32(message.length() + 1);
	data << message.c_str();
	data << uint8(0);
	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendSetSpeed( Object *object, uint8 type, float speed )
{
	WorldPacket data( 50 );

	data << object->GetNewGUID();
	data << uint32( 0 );

	if( type == RUN )
		data << uint8( 0 );

	data << float( speed );

	switch( type ){
		case WALK:{
			data.SetOpcode( SMSG_FORCE_WALK_SPEED_CHANGE );
			break;}

		case RUN:{
			data.SetOpcode(SMSG_FORCE_RUN_SPEED_CHANGE);
			break;}

		case RUNBACK:{
			data.SetOpcode(SMSG_FORCE_RUN_BACK_SPEED_CHANGE);
			break;}

		case SWIM:{
			data.SetOpcode(SMSG_FORCE_SWIM_SPEED_CHANGE);
			break;}

		case SWIMBACK:{
			data.SetOpcode(SMSG_FORCE_SWIM_BACK_SPEED_CHANGE);
			break;}

		case FLY:{
			data.SetOpcode(SMSG_FORCE_FLIGHT_SPEED_CHANGE);
			break;}

		default:
			return;
	}

	MessageRouter router( object );
	router.sendMessageToPlayersInRange( &data, true );
}

void Messenger::SendEmptyPetSpellsToPlayer( Player *player )
{
	WorldPacket data( SMSG_PET_SPELLS, 8 );
	data << uint64( 0 );
	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendPetCastFailed( Player *player, uint32 spellid, uint8 fail )
{
	WorldPacket data( SMSG_PET_CAST_FAILED, 6 );
	data << uint8( 0 );
	data << uint32( spellid );
	data << uint8( fail );
	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendPetActionFeedback(Player *player, PetActionFeedback feedback)
{
	WorldPacket data( SMSG_PET_ACTION_FEEDBACK, 4 );
	data << uint32( feedback );
	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendPetSpells( Player *player, Pet *pet )
{
	PetSpellMap* spells = pet->GetSpells();
	uint32 packetsize;

	if( pet->getExpireTime() == 0 )
		packetsize = spells->size() * 4 + 59;
	else
		packetsize = 62;

	WorldPacket data( SMSG_PET_SPELLS, packetsize );
	data << uint64( pet->GetGUID() );

	if(pet->myFamily != NULL)
		data << uint16( pet->myFamily->ID);
	else
		data << uint16(0);

	data << uint32(0);
	data << uint8(pet->GetPetState());	// 0x0 = passive, 0x1 = defensive, 0x2 = aggressive
	data << uint8(pet->GetPetAction());	// 0x0 = stay, 0x1 = follow, 0x2 = attack
	data << uint16(0);				// flags: 0xFF = disabled pet bar (eg. when pet stunned)

	// Send the actionbar
	for(uint8 i = 0; i < 10; i++)
	{
		uint32 action = pet->getActionBarEntry( i );
		if(action & 0x4000000)		// Commands
			data << uint32(action);
		else
		{
			if(action != 0)
			{
				data << uint16(action);
				data << pet->GetSpellState(action);
			}
			else
			{
				data << uint16(0);
				data << uint8(0);
				data << uint8(i + 5);
			}
		}
	}

	// we don't send spells for the water elemental so it doesn't show up in the spellbook
	if(pet->getExpireTime() == 0)
	{
		// Send the rest of the spells.
		data << uint8(spells->size());
		for(PetSpellMap::iterator itr = spells->begin(); itr != spells->end(); ++itr)
		{
			data << uint16(itr->first->Id);
			data << uint16(itr->second);
		}
	}

	data << uint8(0);

	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendPetTalents(Player *player, uint32 pointsleft, const vector<pair<uint32, uint8> > talents)
{
	WorldPacket data( SMSG_TALENTS_INFO, 50 );
	data << uint8( 1 );				// Pet talent packet identificator
	data << uint32( pointsleft );		// Unspent talent points
	data << uint8( talents.size() );

	for( vector< pair< uint32, uint8 > >::const_iterator itr = talents.begin(); itr != talents.end(); ++itr )
	{
		const std::pair< uint32, uint8 > &pair = *itr;
		data << uint32( pair.first ); // Talent Id
		data << uint8( pair.second ); // Rank
	}

	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendCreatureSpells(Player *player, Creature *creature)
{
	WorldPacket data( SMSG_PET_SPELLS, 100 );
	data << uint64(creature->GetGUID());
	data << uint16( creature->GetCreatureInfo()->Family );
	data << uint32(0);

	if( ! creature->IsVehicle() )
		data << uint32(0);
	else
		data << uint32( 0x8000101 );

	std::vector< uint32 >::iterator itr = creature->GetProto()->castable_spells.begin();

	// Send the actionbar
	for(uint32 i = 0; i < 10; ++i)
	{
		if(itr != creature->GetProto()->castable_spells.end())
		{
			uint32 spell = *itr;
			data << uint32( Arcemu::Util::MAKE_UNIT_ACTION_BUTTON( spell, i + 8 ) );
			++itr;
		}
		else
		{
			data << uint16( 0 );
			data << uint8( 0 );
			data << uint8( i + 8 );
		}
	}

	data << uint8(0);
	// cooldowns
	data << uint8(0);

	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendItemDurationUpdate( Player *player, uint64 guid, uint32 remainingTime )
{
	WorldPacket data( SMSG_ITEM_TIME_UPDATE, 12 );
	data << uint64( guid );
	data << uint32( remainingTime );
	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendEnchantmentLog(Player *player, uint32 itemId, uint32 enchantmentId)
{
	WorldPacket data( SMSG_ENCHANTMENTLOG, 25 );
	data << player->GetNewGUID();  // Item owner
	data << player->GetNewGUID(); // caster
	data << uint32( itemId );
	data << uint32( enchantmentId );
	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendEnchantTimeUpdate( Player *player, uint64 itemGUID, uint32 slot, uint32 duration )
{
	WorldPacket* data = new WorldPacket( SMSG_ITEM_ENCHANT_TIME_UPDATE, 24 );
	*data << uint64( itemGUID );
	*data << uint32( slot );
	*data << uint32( duration );
	*data << uint64( player->GetGUID() );
	PlayerMessenger::sendDelayedMessage( player, data );
}

void Messenger::SendVehicleControl( Player *player )
{
	WorldPacket data( SMSG_CONTROL_VEHICLE, 0 );
	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendClientControlUpdate( Player *player, const WoWGuid &guid, uint8 value )
{
	WorldPacket data( SMSG_CLIENT_CONTROL_UPDATE, 16 );
	data << guid;
	data << uint8( value );
	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendInventoryChangeError(Player *player, uint64 sourceItem, uint64 destinationItem, uint8 error, uint32 requiredLevel)
{
	WorldPacket data( SMSG_INVENTORY_CHANGE_FAILURE, 22 );
	data << uint8( error );
	data << uint64( sourceItem );
	data << uint64( destinationItem );
	data << uint8( 0 );

	if( ( sourceItem != 0 ) && ( requiredLevel != 0 ) )
	{
		data << uint32( requiredLevel );
	}

	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendLearnedSpell(Player *player, uint32 spell)
{
	WorldPacket data( SMSG_LEARNED_SPELL, 6 );
	data << uint32( spell );
	data << uint16( 0 );
	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendTimeSyncRequest( Player *player )
{
	WorldPacket* data = new WorldPacket( SMSG_TIME_SYNC_REQ, 4 );
	*data << uint32( 0 );
	PlayerMessenger::sendDelayedMessage( player, data );
}

void Messenger::SendSetPlayerMovementType( Player *player, uint8 type, uint32 flags )
{
	uint32 opcode = 0;

	switch( type )
	{
		case MOVE_ROOT:
			{
				opcode = SMSG_FORCE_MOVE_ROOT;
			}
			break;
		case MOVE_UNROOT:
			{
				opcode = SMSG_FORCE_MOVE_UNROOT;
			}
			break;
		case MOVE_WATER_WALK:
			{
				opcode = SMSG_MOVE_WATER_WALK;
			}
			break;
		case MOVE_LAND_WALK:
			{
				opcode = SMSG_MOVE_LAND_WALK;
			}
			break;
		default:
			break;
	}

	if( opcode == 0 )
		return;

	WorldPacket data( opcode, 13 );
	data << player->GetNewGUID();
	data << uint32( flags );

	MessageRouter router( player );
	router.sendMessageToPlayersInRange( &data, true );
}

void Messenger::SendSetPlayerSpeeds(Player *player, uint8 type, float speed, uint32 changeCount)
{
	uint32 opcode = 0;

	switch( type )
	{
		case WALK:{
			opcode = SMSG_FORCE_WALK_SPEED_CHANGE;
			break; }

		case RUN:
			{
				opcode = SMSG_FORCE_RUN_SPEED_CHANGE;
			}
			break;
		case RUNBACK:
			{
				opcode = SMSG_FORCE_RUN_BACK_SPEED_CHANGE;
			}
			break;
		case SWIM:
			{
				opcode = SMSG_FORCE_SWIM_SPEED_CHANGE;
			}
			break;
		case SWIMBACK:
			{
				opcode = SMSG_FORCE_SWIM_BACK_SPEED_CHANGE;
			}
			break;
		case FLY:
			{
				opcode = SMSG_FORCE_FLIGHT_SPEED_CHANGE;
			}
			break;
	}

	if( opcode == 0 )
		return;

	WorldPacket data( opcode, 50 );

	data << player->GetNewGUID();

	if( type != SWIMBACK )
	{
		data << uint32( changeCount );
		if(type == RUN)
			data << uint8( 1 );

		data << float( speed );
	}
	else
	{
		const LocationVector &position = player->GetPosition();

		data << uint32( 0 );
		data << uint8( 0 );
		data << uint32( Arcemu::Shared::Util::getMSTime() );
		data << float( position.x );
		data << float( position.y );
		data << float( position.z );
		data << float( position.o );
		data << uint32( 0 );
		data << float( speed );
	}

	MessageRouter router( player );
	router.sendMessageToPlayersInRange( &data, true );
}

void Messenger::SendPreResurrect( Player *player )
{
	WorldPacket data( SMSG_PRE_RESURRECT, 8 );
	FastGUIDPack( data, player->GetGUID() );

	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendDeathReleaseLocation( Player *player, uint32 mapid, const LocationVector &location )
{
	WorldPacket data( SMSG_DEATH_RELEASE_LOC, 16 );
	data << uint32( mapid );
	data << float( location.x );
	data << float( location.y );
	data << float( location.z );

	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendCorpseReclaimDelay( Player *player, uint32 delay )
{
	WorldPacket data( SMSG_CORPSE_RECLAIM_DELAY, 4 );
	data << uint32( CORPSE_RECLAIM_TIME_MS );

	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendRemoveSpell(Player *player, uint32 spell)
{
	WorldPacket data( SMSG_REMOVED_SPELL, 4 );
	data << uint32( spell );

	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendSupersedeSpell(Player *player, uint32 oldspell, uint32 newspell)
{
	WorldPacket data( SMSG_SUPERCEDED_SPELL, 8 );
	data << uint32( oldspell );
	data << uint32( newspell );

	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendClearSpellCooldown( Player *player, uint32 spell )
{
	WorldPacket data( SMSG_CLEAR_COOLDOWN, 12 );
	data << uint32( spell );
	data << uint64( player->GetGUID() );

	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendDuelRequest( Player *challengedPlayer, uint64 challengerGUID, uint64 flagGUID )
{
	WorldPacket data( SMSG_DUEL_REQUESTED, 16 );
	data << uint64( flagGUID );
	data << uint64( challengerGUID );

	PlayerMessenger::sendMessage( challengedPlayer, data );
}

void Messenger::SendDuelWinner( Player *winner, const char *loserName, uint8 condition )
{
	WorldPacket data( SMSG_DUEL_WINNER, 500 );
	data << uint8( condition );
	data << winner->GetName();
	data << loserName;

	MessageRouter router( winner );
	router.sendMessageToPlayersInRange( &data, true );
}

void Messenger::SendDuelComplete( Player *winner, bool broadcast )
{
	WorldPacket data( SMSG_DUEL_COMPLETE, 1 );
	data << uint8( 1 );

	if( broadcast )
	{
		MessageRouter router( winner );
		router.sendMessageToPlayersInRange( &data, true );
	}
	else
	{
		PlayerMessenger::sendMessage( winner, data );
	}
}

void Messenger::SendTitleEarned( Player *player, uint32 title, uint32 set )
{
	// Example title: 123 - Crusader
	WorldPacket data( SMSG_TITLE_EARNED, 8 );
	data << uint32( title );
	data << uint32( set );

	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendExploreXP( Player *player, uint32 area, uint32 xp )
{
	WorldPacket data( SMSG_EXPLORATION_EXPERIENCE, 8 );
	data << uint32( area );
	data << uint32( xp );

	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendSummonRequest( Player *player, uint64 summonerGUID, uint32 zoneId, uint32 time )
{
	WorldPacket data( SMSG_SUMMON_REQUEST, 16 );
	data << uint64( summonerGUID );
	data << uint32( zoneId );
	data << uint32( time );

	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendTriggerMovie(Player *player, uint32 movie)
{
	/// Example movie: 2 - World of Warcraft cinematic intro
	WorldPacket data( SMSG_TRIGGER_MOVIE, 4 );
	data << uint32( movie );

	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendMeetingStoneSetQueue(Player *player, uint32 dungeon, uint8 status)
{
	WorldPacket data( SMSG_MEETINGSTONE_SETQUEUE, 5 );
	data << uint32( dungeon );
	data << uint32( status );

	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendMOTD( Player *player, uint32 value, const char *text )
{
	WorldPacket data( SMSG_MOTD, 100 );

	data << uint32( value );
	data << text;

	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendMoveKnockback( Player *player, float horizontal, float vertical, bool flip )
{
	float orientation = player->GetOrientation();

	// Flip direction
	if( flip )
	{
		orientation = fmod( orientation + M_PI_FLOAT, 2 * M_PI_FLOAT );
	}

	WorldPacket data( SMSG_MOVE_KNOCK_BACK, 50 );
	data << player->GetNewGUID();
	data << uint32( Arcemu::Shared::Util::getMSTime() );
	data << float( cosf( orientation ) );
	data << float( sinf( orientation ) );
	data << float( horizontal );
	data << float( -vertical );

	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendMoveKnockback( Player *player, float angle, float horizontal, float vertical )
{
	WorldPacket data( SMSG_MOVE_KNOCK_BACK, 50 );
	data << player->GetNewGUID();
	data << uint32( Arcemu::Shared::Util::getMSTime() );
	data << float( cosf( angle ) );
	data << float( sinf( angle ) );
	data << float( horizontal );
	data << float( -vertical );

	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendAreaTriggerMessage( Player *player, const char *message )
{
	WorldPacket data( SMSG_AREA_TRIGGER_MESSAGE, 4 + strlen( message ) + 1 + 1 );
	data << uint32( 0 );
	data << message;
	data << uint8( 0 );

	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendUpdateComboPoints( Player *player, uint64 target, uint8 points )
{
	uint32 size = 9;
	if( target == 0 )
		size = 2;

	WorldPacket data( SMSG_UPDATE_COMBO_POINTS, size );
	if( target != 0 )
	{
		FastGUIDPack( data, target );
		data << uint8( points );
	}
	else
	{
		data << uint8( 0 );
		data << uint8( 0 );
	}

	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendTalentResetConfirm( Player *player, uint32 cost )
{
	WorldPacket data( MSG_TALENT_WIPE_CONFIRM, 12 );
	data << uint64( player->GetGUID() );
	data << uint32( cost );

	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendWorldStateUpdate( Player *player, uint32 worldstate, uint32 value )
{
	WorldPacket data( SMSG_UPDATE_WORLD_STATE, 8 );

	data << uint32( worldstate );
	data << uint32( value );

	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendLoginSetTimeSpeed(Player *player, uint32 gametime, float speed)
{
	WorldPacket data( SMSG_LOGIN_SETTIMESPEED, 12 ) ;
	data << uint32( gametime );
	data << float( speed );
	data << uint32( 0 );

	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendBindPointUpdate( Player *player, const LocationVector &location, uint32 map, uint32 zone )
{
	WorldPacket data( SMSG_BINDPOINTUPDATE, 20 );
	data << float( location.x );
	data << float( location.y );
	data << float( location.z );
	data << uint32( map );
	data << uint32( zone );

	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendPlayerBound( Player *player, uint64 GUID, uint32 zone )
{
	WorldPacket data( SMSG_PLAYERBOUND, 12 );
	data << uint64( GUID );
	data << uint32( zone );

	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendGossipPOI( Player *player, float x, float y, uint32 icon, uint32 flags, uint32 dataField, const char *name )
{
	size_t namelen = 0;

	if( name != NULL )
		namelen = strlen( name );

	WorldPacket data( SMSG_GOSSIP_POI, 21 + namelen );

	data << uint32( flags );
	data << float( x );
	data << float( y );
	data << uint32( icon );
	data << uint32( dataField );

	if( namelen == 0 )
		data << uint8( 0 );
	else
		data << name;

	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendLogXPGain( Player *player, uint64 guid, uint32 normalXP, uint32 restedXP )
{
	WorldPacket data( SMSG_LOG_XPGAIN, 24 );
	data << uint64( guid );
	data << uint32( normalXP );

	if( guid == 0 )
	{
		data << uint8( 1 );
		data << uint8( 0 );
	}
	else
	{
		data << uint8( 0 );
		data << uint32( restedXP );
		data << float( 1.0f );
	}

	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendSpellCooldownEvent(Player *player, uint32 spell)
{
	WorldPacket data( SMSG_COOLDOWN_EVENT, 12 );

	data << uint32( spell );
	data << uint64( player->GetGUID() );

	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendNewDrunkState( Player *player, uint32 state, uint32 item )
{
	WorldPacket data (SMSG_CROSSED_INEBRIATION_THRESHOLD, 8 + 4 + 4 );

	data << uint64( player->GetGUID() );
	data << uint32( state );
	data << uint32( item );

	MessageRouter router( player );
	router.sendMessageToPlayersInRange( &data, true );
}

void Messenger::SendLoginVerifyWorld( Player *player, uint32 map, const LocationVector &location )
{
	WorldPacket data( SMSG_LOGIN_VERIFY_WORLD, 20 );
	data << uint32( map );
	data << float( location.x );
	data << float( location.y );
	data << float( location.z );
	data << float( location.o );

	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendSetItemProficiency( Player *player, uint8 itemClass, uint32 proficiency )
{
	WorldPacket data( SMSG_SET_PROFICIENCY, 40 );

	data << uint8( itemClass );
	data << uint32( proficiency );

	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendCastResult(Player *player, CastResult result)
{
	WorldPacket data( SMSG_CAST_FAILED, 80 );

	data << uint8( result.multicast );
	data << uint32( result.spell );
	data << uint8( result.result );

	if( result.extra != 0 )
		data << uint32( result.extra );

	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendTotemCreated( Player *player, const TotemCreationInfo& totem )
{
	WorldPacket data( SMSG_TOTEM_CREATED, 17 );
	data << uint8( totem.slot );
	data << uint64( totem.GUID );
	data << uint32( totem.duration );
	data << uint32( totem.spell );

	PlayerMessenger::sendMessage( player, data );
}

void Messenger::SendPartyKillLogToSet( Player *player, uint64 GUID )
{
	WorldPacket data( SMSG_PARTYKILLLOG, 16 );
	data << uint64( player->GetGUID() );
	data << uint64( GUID );

	MessageRouter router( player );
	router.sendMessageToPlayersInRange( &data, true );
}

void Messenger::sendPageText( Player* player, const uint64 guid )
{
	WorldPacket data( SMSG_GAMEOBJECT_PAGETEXT, 8 );
	data << uint64( guid );

	PlayerMessenger::sendMessage( player, data );
}

void Messenger::sendFeatureSystemStatus( Player* player )
{
	WorldPacket data( SMSG_FEATURE_SYSTEM_STATUS, 20) ;
	data << uint8( 2 );
	data << uint8( 0 );
	PlayerMessenger::sendMessage( player, data );
}

void Messenger::sendLearnedDanceMoves( Player* player )
{
	WorldPacket data( SMSG_LEARNED_DANCE_MOVES, 8 );
	data << uint32( 0 );
	data << uint32( 0 );
	PlayerMessenger::sendMessage( player, data );
}

void Messenger::sendGuildMOTD( Player* player, const char *motd )
{
	WorldPacket data( SMSG_GUILD_EVENT, 50 );
	
	data << uint8( GUILD_EVENT_MOTD );
	data << uint8( 1 );
	
	if( motd != NULL )
		data << motd;
	else
		data << uint8( 0 );
	
	PlayerMessenger::sendMessage( player, data );
}

void Messenger::sendInventoryList( Player* player, const Creature* creature )
{
	InventoryListBuilder builder( player, creature );

	WorldPacket data;	
	builder.build( data );
	
	PlayerMessenger::sendMessage( player, data );
}

void Messenger::sendSetPhaseShift( Player* player )
{
	WorldPacket data( SMSG_SET_PHASE_SHIFT, 4 );
	data << uint32( player->GetPhase() );
	PlayerMessenger::sendMessage( player, data );
}


