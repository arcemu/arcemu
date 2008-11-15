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

#include "StdAfx.h"
#include "Setup.h"
#include "EAS/EasyFunctions.h"

class Flayer : public CreatureAIScript
{
public:
   Flayer(Creature *pCreature) : CreatureAIScript(pCreature) { }
   static CreatureAIScript *Create(Creature *c) { return new Flayer(c); }

   void DeleteDelay(Creature *creat)
   {
     if(creat == NULL)
       return;

     creat->SafeDelete();
   }

   void OnDied(Unit *mKiller)
   {
      if(!mKiller->IsPlayer())
        return;

      Creature *creat = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11064, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation(), true, false, 0, 0);
      
      TimedEvent *te = TimedEvent::Allocate(this, new CallbackP1<Flayer, Creature*>(this, &Flayer::DeleteDelay, creat), 0, 60000, 1);  
      creat->event_AddEvent(te);
   }

   void Destroy()
   {
      delete (Flayer *) this;
   }
};

class SCRIPT_DECL Darrowshire_Spirit : public GossipScript
{
public:
   void DeleteDelay(Creature *creat)
   {
     if(creat == NULL)
       return;

     creat->SafeDelete();
   }

   void GossipHello(Object *pObject, Player *Plr, bool AutoSend)
   {
      QuestLogEntry *en = Plr->GetQuestLogForEntry(5211);

      if(en && en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
      {
        uint32 newcount = en->GetMobCount(0) + 1;

        en->SetMobCount(0, newcount);
        en->SendUpdateAddKill(0);
        en->UpdatePlayerFields();
      }
   
      GossipMenu *Menu;
      objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 3873, Plr);

      if(AutoSend)
         Menu->SendTo(Plr);

      if(!pObject->IsCreature())
        return;

      Creature *Spirit = static_cast<Creature*>(pObject);

      Spirit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
      
      TimedEvent *te = TimedEvent::Allocate(this, new CallbackP1<Darrowshire_Spirit, Creature*>(this, &Darrowshire_Spirit::DeleteDelay, Spirit), 0, 5000, 1);  
      Spirit->event_AddEvent(te);
      Spirit->Despawn(4000, 0);
   }
   
   void Destroy()
   {
      delete this;
   }
};

class ArajTheSummoner : public CreatureAIScript
{
public:
  ADD_CREATURE_FACTORY_FUNCTION(ArajTheSummoner);
  ArajTheSummoner(Creature *pCreature) : CreatureAIScript(pCreature) { }

  void OnDied(Unit *mKiller)
  {
    if(!mKiller->IsPlayer())
      return;

    GameObject *go = sEAS.SpawnGameobject(static_cast<Player*>(mKiller), 177241, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation(), 1);
    sEAS.GameobjectDelete(go, 60000);
  }
};

void SetupEasternPlaguelands(ScriptMgr *mgr)
{
  GossipScript *gs = (GossipScript*) new Darrowshire_Spirit();

  mgr->register_gossip_script(11064, gs);
  mgr->register_creature_script(8532, &Flayer::Create);
  mgr->register_creature_script(8531, &Flayer::Create);
  mgr->register_creature_script(8530, &Flayer::Create);
  mgr->register_creature_script(1852, &ArajTheSummoner::Create);
}
