/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008 Arcemu Team
 * Copyright (C) 2007 Moon++ <http://www.moonplusplus.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */




#ifndef EASYFUNC_H
#define EASYFUNC_H

// Uncomment if you want debug texts
// #define ENABLE_DEBUG_TEXT

#include "StdAfx.h"

#pragma warning(disable:4305) // warning C4305: 'argument' : truncation from 'double' to 'float'
#pragma warning(disable:4267) // warning C4267: '=' : conversion from 'size_t' to 'uint32', possible loss of data

#define CREATE_QUESTSCRIPT(cl) (QuestScript*)new cl
#define CREATE_GOSSIPSCRIPT(cl) (GossipScript*)new cl
#define CREATE_GAMEOBJECT_SCRIPT(cl) &cl::Create
#define CREATE_CREATURESCRIPT(cl) &cl::Create


class SCRIPT_DECL EasyFunctions
{
public:
  static EasyFunctions GetInstance()
  {
    static EasyFunctions easy_singleton;
    return easy_singleton;
  }

  void PrintMessage(const char *text, ...)
  {
#ifdef ENABLE_DEBUG_TEXT
    va_list arg;
    va_start(arg, text);

    printf("[EAS] ");
    vprintf(text, arg);
    putc('\n', stdout);

    va_end(arg);
#endif
  }
  
  void WaypointCreate(Creature *pCreature, float x, float y, float z, float o, uint32 waittime, uint32 flags, uint32 modelid)
  {
    PrintMessage("Function call: WaypointCreate()");
    if(pCreature == NULL)
      return;

    if(!pCreature->m_custom_waypoint_map)
    {
      pCreature->m_custom_waypoint_map = new WayPointMap;
      pCreature->GetAIInterface()->SetWaypointMap(pCreature->m_custom_waypoint_map);
    }

    if(!modelid)
      modelid = pCreature->GetUInt32Value(UNIT_FIELD_DISPLAYID);

    WayPoint * wp = new WayPoint;
    wp->id = pCreature->m_custom_waypoint_map->size() + 1;
    wp->x = x;
    wp->y = y;
    wp->z = z;
    wp->o = o;
    wp->flags = flags;
    wp->backwardskinid = modelid;
    wp->forwardskinid = modelid;
    wp->backwardemoteid = wp->forwardemoteid = 0;
    wp->backwardemoteoneshot = wp->forwardemoteoneshot = false;
    wp->waittime = waittime;

    pCreature->m_custom_waypoint_map->push_back(wp);
  }
  
  void DeleteWaypoints(Creature *creat)
  {
    PrintMessage("Function call: DeleteWaypoints()");
    if(creat == NULL)
      return;

    if(creat->m_custom_waypoint_map == NULL)
      return;

    WayPointMap::iterator i = creat->m_custom_waypoint_map->begin();
    
    for(; i != creat->m_custom_waypoint_map->end(); ++i)
    {
      if((*i) != NULL)
        delete (*i);
    }

    creat->m_custom_waypoint_map->clear();
  }

  Creature *SpawnCreature(Player *pThis, uint32 entry, float posX, float posY, float posZ, float posO, uint32 duration = 0)
  {
    PrintMessage("Function call: SpawnCreature()");
    if(pThis == NULL)
      return NULL;
      
    CreatureProto *p = CreatureProtoStorage.LookupEntry(entry);
    
    if(p == NULL)
      return NULL;

    Creature *pCreature = pThis->GetMapMgr()->CreateCreature(entry);
    pCreature->spawnid = 0;
    pCreature->m_spawn = 0;
    pCreature->Load(p, posX, posY, posZ);
    pCreature->SetMapId(pThis->GetMapId());
    pCreature->SetOrientation(posO);
    pCreature->Despawn(duration, 0);
    pCreature->SetInstanceID(pThis->GetInstanceID());
    pCreature->PushToWorld(pThis->GetMapMgr());

    return pCreature;
  }
  
  void DeleteSpawned(Creature *creat)
  {
    PrintMessage("Function call: DeleteSpawned()");
    if(creat == NULL)
      return;

    creat->Despawn(0, 0);
    creat->SafeDelete();
  }

  Creature *SpawnCreatureExtended(Player *pThis, uint32 entry, float posX, 
                                         float posY, float posZ, float posO, uint32 faction, 
                                         uint32 duration, bool dis_comb = false, bool dis_mel = false, 
                                         bool dis_target = false)
  {
    PrintMessage("Function call: SpawnCreatureExtended()");

    if(pThis == NULL)
      return NULL;

    CreatureProto *p = CreatureProtoStorage.LookupEntry(entry);
    if(p == NULL)
      return NULL;

    Creature *pCreature = pThis->GetMapMgr()->CreateCreature(entry);
    pCreature->spawnid = 0;
    pCreature->m_spawn = 0;
    pCreature->Load(p, posX, posY, posZ);
    pCreature->SetMapId(pThis->GetMapId());
    pCreature->SetOrientation(posO);
    pCreature->GetAIInterface()->disable_combat = dis_comb;
    pCreature->GetAIInterface()->disable_melee = dis_mel;
    pCreature->GetAIInterface()->disable_targeting = dis_target;
    pCreature->SetInstanceID(pThis->GetInstanceID());
    pCreature->PushToWorld(pThis->GetMapMgr());
    pCreature->Despawn(duration, 0);
    pCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, faction);
    pCreature->_setFaction();
    
    return pCreature;
  }
  
  void GameobjectDelete(GameObject *pC, uint32 duration)
  {
    PrintMessage("Function call: GameobjectDelete()");
    if(pC == NULL)
      return;

    TimedEvent *te = TimedEvent::Allocate(pC, new CallbackP0<GameObject>(pC, &GameObject::ExpireAndDelete), 0, duration, 1);  
    pC->event_AddEvent(te);
  }

  GameObject *SpawnGameobject(Player *plr, uint32 entry_id, float x, float y, float z, float o, float scale)
  {
    PrintMessage("Function call: SpawnGameobject()");
    if(plr == NULL)
      return NULL;

    GameObjectInfo *goi = GameObjectNameStorage.LookupEntry(entry_id);
    if(goi == NULL)
      return NULL;

    GameObject *pC = plr->GetMapMgr()->CreateGameObject(entry_id);
    //pC->spawnid=0;
    pC->m_spawn=0;
    pC->CreateFromProto(entry_id, plr->GetMapId(), (float)x, (float)y, (float)z, (float)o);
    pC->SetFloatValue(OBJECT_FIELD_SCALE_X, (float)scale);
    pC->SetMapId(plr->GetMapId());
    pC->SetInstanceID(plr->GetInstanceID());
    pC->PushToWorld(plr->GetMapMgr());

    return pC;
  }
  
  void MoveToPlayer(Player *plr, Creature *creat)
  {
    PrintMessage("Function call: MoveToPlayer()");
    if(plr == NULL)
      return;
      
    if(creat == NULL)
      return;
      
    creat->GetAIInterface()->MoveTo(plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(), plr->GetOrientation());
  }

  void CreateCustomWaypointMap(Creature *creat)
  {
    PrintMessage("Function call: CreateCustomWaypointMap()");
    if(creat == NULL)
      return;

    if(creat->m_custom_waypoint_map)
    {
      for(WayPointMap::iterator itr = creat->m_custom_waypoint_map->begin(); itr != creat->m_custom_waypoint_map->end(); ++itr)
        delete (*itr);

      delete creat->m_custom_waypoint_map;
    } 
    
    creat->m_custom_waypoint_map = new WayPointMap;
    creat->GetAIInterface()->SetWaypointMap(creat->m_custom_waypoint_map);
  }
  
  bool AddItem(uint32 entry, Player *plr, uint32 count = 1)
  {
    PrintMessage("Function call: AddItem(%u)", entry);

    if(plr == NULL)
      return false;

    ItemPrototype *proto = ItemPrototypeStorage.LookupEntry(entry);
    SlotResult slotresult;
    slotresult = plr->GetItemInterface()->FindFreeInventorySlot(proto);

    if(!slotresult.Result)
    {
      plr->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_INVENTORY_FULL);
      return false;
    }
    else
    {
      Item *pItem = objmgr.CreateItem(entry, plr);
      Item *stack = plr->GetItemInterface()->FindItemLessMax(entry, count, false);

      if(stack == NULL)
      {
        plr->GetItemInterface()->AddItemToFreeSlot(pItem);
        plr->GetSession()->SendItemPushResult(pItem, false, true, true, true, slotresult.ContainerSlot, slotresult.Slot, 1);
        return true;
      }

      stack->SetCount(stack->GetUInt32Value(ITEM_FIELD_STACK_COUNT) + count);
      stack->m_isDirty = true;
      
      plr->GetSession()->SendItemPushResult(stack, false, true, true, false, plr->GetItemInterface()->GetBagSlotByGuid(stack->GetGUID()), 0xFFFFFFFF, 1);
       
      return true;
    }

    return false;
  }
};

#define sEAS EasyFunctions::GetInstance()

#endif
