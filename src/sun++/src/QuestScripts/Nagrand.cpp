/*
 * Modifyed on 20 July 2008 by Angelis, from SUN++ Community.
 * Copyright (C) 2008 Sun++ Team <http://sunplusplus.info/>
 *
 * --------------------------------------------------------------------
 * Moon++ Scripts for Ascent MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
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

// Giselda The Crone Quest
class GiseldaTheCroneQAI : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(GiseldaTheCroneQAI);
    GiseldaTheCroneQAI(Creature* pCreature) : CreatureAIScript(pCreature)  {}

   void OnDied(Unit * mKiller)
   {
      if (mKiller->IsPlayer())
      {
         if (((Player*)mKiller)->GetTeam() == 0)
         {
            QuestLogEntry *en = ((Player*)mKiller)->GetQuestLogForEntry(9936);
            if(en && en->GetMobCount(1) < en->GetQuest()->required_mobcount[1])
            {
               uint32 newcount = en->GetMobCount(1) + 1;
               en->SetMobCount(1, newcount);
               en->SendUpdateAddKill(1);
               en->UpdatePlayerFields();
               return;
            }
         }
         else
         {
            QuestLogEntry *en = ((Player*)mKiller)->GetQuestLogForEntry(9935);
            if(en && en->GetMobCount(1) < en->GetQuest()->required_mobcount[1])
            {
               uint32 newcount = en->GetMobCount(1) + 1;
               en->SetMobCount(1, newcount);
               en->SendUpdateAddKill(1);
               en->UpdatePlayerFields();
               return;
            }
         }
      }
   }
};

// Stopping the Spread
bool Quest_StoppingTheSpread(uint32 i, Spell* pSpell)
{
  if(!pSpell->u_caster->IsPlayer())
   return true;

  Player *plr = (Player*)pSpell->u_caster;
  Creature *target = (Creature*)plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(plr->GetPositionX(), plr->GetPositionY() , plr->GetPositionZ(), 18240);

  if(target == NULL)
     return true;

  QuestLogEntry *qle = plr->GetQuestLogForEntry(9874);
 
  if(qle == NULL)
    return true;

  GameObject *obj = NULL;

  if(qle && qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
  {
      qle->SetMobCount(0, qle->GetMobCount(0)+1);
      qle->SendUpdateAddKill(0);
      
      obj = sEAS.SpawnGameobject(plr, 183816, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), target->GetOrientation(), 1);
      sEAS.GameobjectDelete(obj, 1*30*1000);
  }
  target->Despawn(2000, 3*60*1000);
  plr->UpdateNearbyGameObjects();
  qle->UpdatePlayerFields();
  return true;
}

void SetupNagrand(ScriptMgr * mgr)
{
   mgr->register_creature_script(17147, &GiseldaTheCroneQAI::Create);
   mgr->register_creature_script(17148, &GiseldaTheCroneQAI::Create);
   mgr->register_creature_script(18397, &GiseldaTheCroneQAI::Create);
   mgr->register_creature_script(18658, &GiseldaTheCroneQAI::Create);
   mgr->register_creature_script(17146, &GiseldaTheCroneQAI::Create);
   mgr->register_dummy_spell(32146, &Quest_StoppingTheSpread);
}
