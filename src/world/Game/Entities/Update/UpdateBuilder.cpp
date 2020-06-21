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
#include "Game/Entities/Update/UpdateBuilder.h"

void UpdateBuilder::_BuildValuesUpdate( ByteBuffer* data, UpdateMask* updateMask, Object *object, Player* target )
{
    bool activate_quest_object = false;
    bool reset = false;
    uint32 oldflags = 0;

    if(updateMask->GetBit(OBJECT_FIELD_GUID) && target)	   // We're creating.
    {
	if(object->IsCreature())
	{
	    Creature* pThis = TO< Creature* >(object);
	    if(pThis->IsTagged() && (pThis->loot.gold || pThis->loot.items.size()))
	    {
		// Let's see if we're the tagger or not.
		oldflags = object->GetUInt32Value( UNIT_DYNAMIC_FLAGS );
		uint32 Flags = object->GetUInt32Value( UNIT_DYNAMIC_FLAGS );
		uint32 oldFlags = 0;

		if(pThis->GetTaggerGUID() == target->GetGUID())
		{
		    // Our target is our tagger.
		    oldFlags = U_DYN_FLAG_TAGGED_BY_OTHER;

		    if(Flags & U_DYN_FLAG_TAGGED_BY_OTHER)
			Flags &= ~oldFlags;

		    if(!(Flags & U_DYN_FLAG_LOOTABLE) && pThis->HasLootForPlayer(target))
			Flags |= U_DYN_FLAG_LOOTABLE;
		}
		else
		{
		    // Target is not the tagger.
		    oldFlags = U_DYN_FLAG_LOOTABLE;

		    if(!(Flags & U_DYN_FLAG_TAGGED_BY_OTHER))
			Flags |= U_DYN_FLAG_TAGGED_BY_OTHER;

		    if(Flags & U_DYN_FLAG_LOOTABLE)
			Flags &= ~oldFlags;
		}

		object->SetUInt32ValueNoUpdate( UNIT_DYNAMIC_FLAGS, Flags );

		updateMask->SetBit(UNIT_DYNAMIC_FLAGS);

		reset = true;
	    }
	}

	if(target && object->IsGameObject())
	{
	    GameObject* go = TO_GAMEOBJECT(object);
	    QuestLogEntry* qle;
	    GameObjectInfo* info;
	    if(go->HasQuests())
	    {
		std::list<QuestRelation*>::iterator itr;
		for(itr = go->QuestsBegin(); itr != go->QuestsEnd(); ++itr)
		{
		    QuestRelation* qr = (*itr);
		    if(qr != NULL)
		    {
			Quest* qst = qr->qst;
			if(qst != NULL)
			{
			    if((qr->type & QUESTGIVER_QUEST_START && !target->HasQuest(qst->id))
				    ||	(qr->type & QUESTGIVER_QUEST_END && target->HasQuest(qst->id))
			      )
			    {
				if( sQuestMgr.PlayerMeetsReqs(target, qst, false) == QMGR_QUEST_AVAILABLE )
				{
				    activate_quest_object = true;
				    break;
				}
			    }
			}
		    }
		}
	    }
	    else
	    {
		info = go->GetInfo();
		if(info && (info->goMap.size() || info->itemMap.size()))
		{
		    for(GameObjectGOMap::iterator itr = go->GetInfo()->goMap.begin(); itr != go->GetInfo()->goMap.end(); ++itr)
		    {
			qle = target->GetQuestLogForEntry(itr->first->id);
			if(qle != NULL)
			{
			    if(qle->GetQuest()->count_required_mob == 0)
				continue;
			    for(uint32 i = 0; i < 4; ++i)
			    {
				if(qle->GetQuest()->required_mob[i] == static_cast<int32>(go->GetEntry()) && qle->GetMobCount(i) < qle->GetQuest()->required_mobcount[i])
				{
				    activate_quest_object = true;
				    break;
				}
			    }
			    if(activate_quest_object)
				break;
			}
		    }

		    if(!activate_quest_object)
		    {
			for(GameObjectItemMap::iterator itr = go->GetInfo()->itemMap.begin();
				itr != go->GetInfo()->itemMap.end();
				++itr)
			{
			    for(std::map<uint32, uint32>::iterator it2 = itr->second.begin();
				    it2 != itr->second.end();
				    ++it2)
			    {
				if((qle = target->GetQuestLogForEntry(itr->first->id)) != 0)
				{
				    if(target->GetItemInterface()->GetItemCount(it2->first) < it2->second)
				    {
					activate_quest_object = true;
					break;
				    }
				}
			    }
			    if(activate_quest_object)
				break;
			}
		    }
		}
	    }
	}
    }


    if(activate_quest_object)
    {
	oldflags = object->GetUInt32Value( GAMEOBJECT_DYNAMIC );
	if(!updateMask->GetBit(GAMEOBJECT_DYNAMIC))
	    updateMask->SetBit(GAMEOBJECT_DYNAMIC);
	object->SetUInt32ValueNoUpdate( GAMEOBJECT_DYNAMIC,  1 | 8 ); // 8 to show sparkles
	reset = true;
    }

    uint16 m_valuesCount = object->GetValuesCount();

    ARCEMU_ASSERT(updateMask && updateMask->GetCount()  == m_valuesCount);
    uint32 bc;
    uint32 values_count;
    if(m_valuesCount > (2 * 0x20))    //if number of blocks > 2->  unit and player+item container
    {
	bc = updateMask->GetUpdateBlockCount();
	values_count = min<uint32>(bc * 32, m_valuesCount);

    }
    else
    {
	bc = updateMask->GetBlockCount();
	values_count = m_valuesCount;
    }

    *data << (uint8)bc;
    data->append(updateMask->GetMask(), bc * 4);

    for(uint32 index = 0; index < values_count; index ++)
    {
	if(updateMask->GetBit(index))
	{
	    *data << object->GetUInt32Value( index );
	}
    }

    if(reset)
    {
	switch(object->GetTypeId())
	{
	    case TYPEID_UNIT:
		object->SetUInt32ValueNoUpdate( UNIT_DYNAMIC_FLAGS, oldflags );
		break;
	    case TYPEID_GAMEOBJECT:
		object->SetUInt32ValueNoUpdate( GAMEOBJECT_DYNAMIC, oldflags );
		break;
	}
    }
}

uint32 UpdateBuilder::BuildCreateUpdateBlockForPlayer( ByteBuffer* data, Object* object, Player* target )
{
    uint32 count = 0;

    if( object->IsPlayer() )
    {
	if( target == object )
	{
	    count += TO_PLAYER( object )->GetItemInterface()->m_CreateForPlayer( data );
	}
    }

    uint16 flags = 0;
    uint32 flags2 = 0;

    uint8 updatetype = UPDATETYPE_CREATE_OBJECT;
    if(object->IsCorpse())
    {
	if(TO< Corpse* >(object)->GetDisplayId() == 0)
	    return 0;
	updatetype = UPDATETYPE_CREATE_YOURSELF;
    }

    // any other case
    switch(object->GetTypeId())
    {
	    // items + containers: 0x8
	case TYPEID_ITEM:
	case TYPEID_CONTAINER:
	    flags = 0x10;
	    break;

	    // player/unit: 0x68 (except self)
	case TYPEID_UNIT:
	    flags = 0x70;
	    break;

	case TYPEID_PLAYER:
	    flags = 0x70;
	    break;

	    // gameobject/dynamicobject
	case TYPEID_GAMEOBJECT:
	    flags = 0x0350;
	    if(TO< GameObject* >(object)->GetDisplayId() == 3831) flags = 0x0252; //Deeprun Tram proper flags as of 3.2.0.
	    break;

	case TYPEID_DYNAMICOBJECT:
	    flags = 0x0150;
	    break;

	case TYPEID_CORPSE:
	    flags = 0x0150;
	    break;
	    // anyone else can get fucked and die!
    }

    if(target == object)
    {
	// player creating self
	flags |= 0x01;
	updatetype = UPDATETYPE_CREATE_YOURSELF;
    }



    // gameobject stuff
    if(object->IsGameObject())
    {
//		switch( GetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_TYPEID) )
	switch(object->GetUInt32Value(GAMEOBJECT_BYTES_1))
	{
	    case GAMEOBJECT_TYPE_MO_TRANSPORT:
		{
		    if(object->GetTypeFromGUID() != HIGHGUID_TYPE_TRANSPORTER)
			return 0;   // bad transporter
		    else
			flags = 0x0352;
		}
		break;

	    case GAMEOBJECT_TYPE_TRANSPORT:
		{
		    /* deeprun tram, etc */
		    flags = 0x252;
		}
		break;

	    case GAMEOBJECT_TYPE_DUEL_ARBITER:
		{
		    // duel flags have to stay as updatetype 3, otherwise
		    // it won't animate
		    updatetype = UPDATETYPE_CREATE_YOURSELF;
		}
		break;
	}
	//The above 3 checks FAIL to identify transports, thus their flags remain 0x58, and this is BAAAAAAD! Later they don't get position x,y,z,o updates, so they appear randomly by a client-calculated path, they always face north, etc... By: VLack
	if(flags != 0x0352 && object->IsGameObject() && TO< GameObject* >(object)->GetInfo()->Type == GAMEOBJECT_TYPE_TRANSPORT && !(TO< GameObject* >(object)->GetOverrides() & GAMEOBJECT_OVERRIDE_PARENTROT))
	    flags = 0x0352;
    }

    if( object->IsVehicle() )
	flags |= UPDATEFLAG_VEHICLE;

    // build our actual update
    *data << updatetype;

    // we shouldn't be here, under any circumstances, unless we have a wowguid..
    const WoWGuid &wowGuid = object->GetNewGUID();
    ARCEMU_ASSERT(wowGuid.GetNewGuidLen() > 0);
    *data << wowGuid;

    *data << object->GetTypeId();

    _BuildMovementUpdate(data, flags, flags2, object, target);

    // we have dirty data, or are creating for ourself.
    UpdateMask updateMask;
    updateMask.SetCount(object->GetValuesCount());
    object->_SetCreateBits(&updateMask, target);

    if(object->IsGameObject() && (TO< GameObject* >(object)->GetOverrides() & GAMEOBJECT_OVERRIDE_PARENTROT))
    {
	updateMask.SetBit(GAMEOBJECT_PARENTROTATION_02);
	updateMask.SetBit(GAMEOBJECT_PARENTROTATION_03);
    }

    // this will cache automatically if needed
    _BuildValuesUpdate(data, &updateMask, object, target);

    // update count: +1 ;)
    count += 1;

    if( object->IsTransporter() )
    {
	Transporter *transporter = static_cast< Transporter* >( object );
	count += transporter->BuildCreateNPCUpdateBlockForPlayer(data, target);
    }

    return count;
}

void UpdateBuilder::_BuildMovementUpdate( ByteBuffer* data, uint16 flags, uint32 flags2, Object *object, Player* target )
{
	ByteBuffer* splinebuf = (object->GetTypeId() == TYPEID_UNIT) ? target->GetAndRemoveSplinePacket(object->GetGUID()) : 0;

	if(splinebuf != NULL)
	{
		flags2 |= MOVEFLAG_SPLINE_ENABLED | MOVEFLAG_MOVE_FORWARD;	   //1=move forward
		if(object->GetTypeId() == TYPEID_UNIT)
		{
			if(TO_UNIT(object)->GetAIInterface()->HasWalkMode(WALKMODE_WALK))
				flags2 |= MOVEFLAG_WALK;
		}
	}

	uint16 moveflags2 = 0; // mostly seem to be used by vehicles to control what kind of movement is allowed
	if( object->IsVehicle() ){
		Unit *u = static_cast< Unit* >( object );
		if( u->GetVehicleComponent() != NULL )
			moveflags2 |= u->GetVehicleComponent()->GetMoveFlags2();

		if(object->IsCreature() ){
			if( static_cast< Unit* >( object )->HasAuraWithName( SPELL_AURA_ENABLE_FLIGHT ) )
				flags2 |= ( MOVEFLAG_NO_COLLISION | MOVEFLAG_AIR_SWIMMING );
		}

	}

	*data << (uint16)flags;

	Player* pThis = NULL;
	MovementInfo* moveinfo = NULL;
	if(object->IsPlayer())
	{
		pThis = TO< Player* >(object);
		if(pThis->GetSession())
			moveinfo = pThis->GetSession()->GetMovementInfo();
		if(target == object)
		{
			// Updating our last speeds.
			pThis->UpdateLastSpeeds();
		}
	}
	Creature* uThis = NULL;
	if(object->IsCreature())
		uThis = TO< Creature* >(object);

	if(flags & UPDATEFLAG_LIVING)  //0x20
	{
		if(pThis && pThis->transporter_info.guid != 0)
			flags2 |= MOVEFLAG_TRANSPORT; //0x200
		else if(uThis != NULL && object->transporter_info.guid != 0 && uThis->transporter_info.guid != 0)
			flags2 |= MOVEFLAG_TRANSPORT; //0x200

		if( ( pThis != NULL ) && pThis->isRooted() )
			flags2 |= MOVEFLAG_ROOTED;
		else if( ( uThis != NULL ) && uThis->isRooted() )
			flags2 |= MOVEFLAG_ROOTED;

		if(uThis != NULL)
		{
			//		 Don't know what this is, but I've only seen it applied to spirit healers.
			//		 maybe some sort of invisibility flag? :/
			switch(object->GetEntry())
			{
				case 6491:  // Spirit Healer
				case 13116: // Alliance Spirit Guide
				case 13117: // Horde Spirit Guide
					{
						flags2 |= MOVEFLAG_WATER_WALK; //0x10000000
					}
					break;
			}

			if(uThis->GetAIInterface()->IsFlying())
				flags2 |= MOVEFLAG_NO_COLLISION; //0x400 Zack : Teribus the Cursed had flag 400 instead of 800 and he is flying all the time
			if(uThis->GetAIInterface()->onGameobject)
				flags2 |= MOVEFLAG_ROOTED;
			if(uThis->GetProto()->extra_a9_flags)
			{
//do not send shit we can't honor
#define UNKNOWN_FLAGS2 ( 0x00002000 | 0x04000000 | 0x08000000 )
				uint32 inherit = uThis->GetProto()->extra_a9_flags & UNKNOWN_FLAGS2;
				flags2 |= inherit;
			}
		}

		*data << (uint32)flags2;

		*data << (uint16)moveflags2;

		*data << Arcemu::Shared::Util::getMSTime(); // this appears to be time in ms but can be any thing. Maybe packet serializer ?

		// this stuff:
		//   0x01 -> Enable Swimming?
		//   0x04 -> ??
		//   0x10 -> disables movement compensation and causes players to jump around all the place
		//   0x40 -> disables movement compensation and causes players to jump around all the place

		//Send position data, every living thing has these
		*data << (float)object->GetPosition().x;
		*data << (float)object->GetPosition().y;
		*data << (float)object->GetPosition().z;
		*data << (float)object->GetPosition().o;

		if(flags2 & MOVEFLAG_TRANSPORT) //0x0200
		{
			*data << WoWGuid( object->transporter_info.guid );
			*data << float( object->transporter_info.x );
			*data << float( object->transporter_info.y );
			*data << float( object->transporter_info.z );
			*data << float( object->transporter_info.o );
			*data << uint32( object->transporter_info.flags );
			*data << uint8( object->transporter_info.seat );
		}


		if((flags2 & (MOVEFLAG_SWIMMING | MOVEFLAG_AIR_SWIMMING)) || (moveflags2 & MOVEFLAG2_ALLOW_PITCHING))   // 0x2000000+0x0200000 flying/swimming, || sflags & SMOVE_FLAG_ENABLE_PITCH
		{
			if(pThis && moveinfo)
				*data << moveinfo->pitch;
			else
				*data << (float)0; //pitch
		}

		if(pThis && moveinfo)
			*data << moveinfo->unklast;
		else
			*data << (uint32)0; //last fall time

		if(flags2 & MOVEFLAG_REDIRECTED)   // 0x00001000
		{
			if(moveinfo != NULL)
			{
				*data << moveinfo->redirectVelocity;
				*data << moveinfo->redirectSin;
				*data << moveinfo->redirectCos;
				*data << moveinfo->redirect2DSpeed;
			}
			else
			{
				*data << (float)0;
				*data << (float)1.0;
				*data << (float)0;
				*data << (float)0;
			}
		}

		if(object->m_walkSpeed == 0)
			*data << 8.0f;
		else
			*data << object->m_walkSpeed;	// walk speed
		if(object->m_runSpeed == 0)
			*data << 8.0f;
		else
			*data << object->m_runSpeed;	// run speed
		*data << object->m_backWalkSpeed;	// backwards walk speed
		*data << object->m_swimSpeed;		// swim speed
		*data << object->m_backSwimSpeed;	// backwards swim speed
		if(object->m_flySpeed == 0)
			*data << 8.0f;
		else
			*data << object->m_flySpeed;	// fly speed
		*data << object->m_backFlySpeed;	// back fly speed
		*data << object->m_turnRate;		// turn rate
		*data << float(7);		// pitch rate, now a constant...

		if(flags2 & MOVEFLAG_SPLINE_ENABLED)   //VLack: On Mangos this is a nice spline movement code, but we never had such... Also, at this point we haven't got this flag, that's for sure, but fail just in case...
		{
			data->append(*splinebuf);
			delete splinebuf;
		}
	}
	else //----------------------------------- No UPDATEFLAG_LIVING -----------------------------------
	{
		if(flags & UPDATEFLAG_POSITION)   //0x0100
		{
			*data << uint8(0);   //some say it is like parent guid ?
			*data << (float)object->GetPosition().x;
			*data << (float)object->GetPosition().y;
			*data << (float)object->GetPosition().z;
			*data << (float)object->GetPosition().x;
			*data << (float)object->GetPosition().y;
			*data << (float)object->GetPosition().z;
			*data << (float)object->GetPosition().o;

			if(object->IsCorpse())
				*data << (float)object->GetPosition().o; //VLack: repeat the orientation!
			else
				*data << (float)0;
		}
		else if(flags & UPDATEFLAG_HAS_POSITION)  //0x40
		{
			if(flags & UPDATEFLAG_TRANSPORT && object->GetUInt32Value( GAMEOBJECT_BYTES_1 ) == GAMEOBJECT_TYPE_MO_TRANSPORT)
			{
				*data << (float)0;
				*data << (float)0;
				*data << (float)0;
			}
			else
			{
				*data << (float)object->GetPosition().x;
				*data << (float)object->GetPosition().y;
				*data << (float)object->GetPosition().z;
			}
			*data << (float)object->GetPosition().o;
		}
	}


	if(flags & UPDATEFLAG_LOWGUID)   //0x08
		*data << object->GetLowGUID();

	if(flags & UPDATEFLAG_HIGHGUID)   //0x10
		*data << object->GetHighGUID();

	if(flags & UPDATEFLAG_HAS_TARGET)   //0x04
	{
		if(object->IsUnit())
			FastGUIDPack(*data, TO_UNIT(object)->GetTargetGUID());	//some compressed GUID
		else
			*data << uint64(0);
	}


	if(flags & UPDATEFLAG_TRANSPORT)   //0x2
	{
	*data << Arcemu::Shared::Util::getMSTime();
    }
	if( flags & UPDATEFLAG_VEHICLE ){
		uint32 vehicleid = 0;

		if( object->IsCreature() )
			vehicleid = TO< Creature* >( object )->GetProto()->vehicleid;
		else
		if( object->IsPlayer() )
			vehicleid = TO< Player* >( object )->mountvehicleid;

		*data << uint32( vehicleid );
		*data << float( object->GetOrientation() );
	}

	if(flags & UPDATEFLAG_ROTATION)   //0x0200
	{
		if(object->IsGameObject())
			*data << TO< GameObject* >(object)->m_rotation;
		else
			*data << uint64(0);   //?
	}
}

//That is dirty fix it actually creates update of 1 field with
//the given value ignoring existing changes in fields and so on
//useful if we want update this field for certain players
//NOTE: it does not change fields. This is also very fast method
WorldPacket* UpdateBuilder::BuildFieldUpdatePacket(Object *object, uint32 index, uint32 value)
{
	// uint64 guidfields = GetGUID();
	// uint8 guidmask = 0;
	WorldPacket* packet = new WorldPacket(1500);
	packet->SetOpcode(SMSG_UPDATE_OBJECT);

	*packet << (uint32)1;//number of update/create blocks
//	*packet << (uint8)0;//unknown //VLack: removed for 3.1

	*packet << (uint8) UPDATETYPE_VALUES;		// update type == update
	*packet << object->GetNewGUID();

	uint32 mBlocks = index / 32 + 1;
	*packet << (uint8)mBlocks;

	for(uint32 dword_n = mBlocks - 1; dword_n; dword_n--)
		*packet << (uint32)0;

	*packet << (((uint32)(1)) << (index % 32));
	*packet << value;

	return packet;
}

void UpdateBuilder::SendFieldUpdatePacket(Player* Target, Object *object, uint32 Index, uint32 Value)
{
	ByteBuffer buf(500);
	buf << uint8(UPDATETYPE_VALUES);
	buf << object->GetNewGUID();

	uint32 mBlocks = Index / 32 + 1;
	buf << (uint8)mBlocks;

	for(uint32 dword_n = mBlocks - 1; dword_n; dword_n--)
		buf << (uint32)0;

	buf << (((uint32)(1)) << (Index % 32));
	buf << Value;

	Target->PushUpdateData(&buf, 1);
}

void UpdateBuilder::BuildFieldUpdatePacket(ByteBuffer* buf, Object *object, uint32 Index, uint32 Value)
{
	*buf << uint8(UPDATETYPE_VALUES);
	*buf << object->GetNewGUID();

	uint32 mBlocks = Index / 32 + 1;
	*buf << (uint8)mBlocks;

	for(uint32 dword_n = mBlocks - 1; dword_n; dword_n--)
		*buf << (uint32)0;

	*buf << (((uint32)(1)) << (Index % 32));
	*buf << Value;
}

uint32 UpdateBuilder::BuildValuesUpdateBlockForPlayer(ByteBuffer* data, Object *object, Player* target)
{
	UpdateMask updateMask;
	updateMask.SetCount(object->GetValuesCount());
	object->_SetUpdateBits(&updateMask, target);
	const WoWGuid &wowGuid = object->GetNewGUID();
	for(uint32 x = 0; x < object->GetValuesCount(); ++x)
	{
		if(updateMask.GetBit(x))
		{
			*data << (uint8) UPDATETYPE_VALUES;		// update type == update
			ARCEMU_ASSERT(wowGuid.GetNewGuidLen() > 0);
			*data << wowGuid;

			_BuildValuesUpdate(data, &updateMask, object, target);
			return 1;
		}
	}

	return 0;
}

uint32 UpdateBuilder::BuildValuesUpdateBlockForPlayer(ByteBuffer* buf, UpdateMask* mask, Object* object)
{
	const WoWGuid &wowGuid = object->GetNewGUID();
	ARCEMU_ASSERT(wowGuid.GetNewGuidLen() > 0);

	// returns: update count
	// update type == update
	*buf << (uint8) UPDATETYPE_VALUES;
	*buf << wowGuid;
	_BuildValuesUpdate(buf, mask, object, 0);

	// 1 update.
	return 1;
}
