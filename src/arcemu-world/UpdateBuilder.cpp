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
