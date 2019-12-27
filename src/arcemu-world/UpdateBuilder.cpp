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
#include <UpdateBuilder.h>

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

                object->SetUInt32Value( UNIT_DYNAMIC_FLAGS, Flags );

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
        object->SetUInt32Value( GAMEOBJECT_DYNAMIC,  1 | 8 ); // 8 to show sparkles
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
                object->SetUInt32Value( UNIT_DYNAMIC_FLAGS, oldflags );
                break;
            case TYPEID_GAMEOBJECT:
                object->SetUInt32Value( GAMEOBJECT_DYNAMIC, oldflags );
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

    object->_BuildMovementUpdate(data, flags, flags2, target);

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
    return count;
}
