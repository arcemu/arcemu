/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009-2013 ArcEmu Team <http://www.arcemu.org/>
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

//general defines
#define MAP_KALIMDOR 1
#define ZONE_SILITHUS 1377
#define WORLDSTATE_SILITHUS_SILITHYST_MAX 2317
#define WORLDSTATE_SILITHUS_GATHERED_ALLIANCE 2313
#define WORLDSTATE_SILITHUS_GATHERED_HORDE 2314

#define SILITHYST_SPELL 29519

// gameobjects
#define SILITHYST_MOUND      181597
#define SILITHYST_GEYSER   181598

// buffs
#define TRACES_OF_SILITHYST 29534
#define CENARION_FAVOR 30754

// how much we need to win?
#define SILITHYST_MAX 200

// areatriggers
#define ALLIANCE_RETURN 4162
#define HORDE_RETURN 4168

// quests
#define ALLIANCE_SILITHYST_QUEST 9419
#define HORDE_SILITHYST_QUEST 9422

// rewards
#define REWARD_HONOR 19

#define REWARD_REPUTATION_FACTION 609
#define REWARD_REPUTATION_VALUE 20

uint8 silithyst_gathered[2] = { 0, 0 };

uint8 winners = 3; // set to neutral as indicator that its neither alliance nor horde

/* initial worldstates should be in db
 * kalimdor, silithus, WORLDSTATE_SILITHUS_SILITHYST_MAX, SILITHYST_MAX
 * kalimdor, silithus, WORLDSTATE_SILITHUS_GATHERED_ALLIANCE, 0
 * kalimdor, silithus, WORLDSTATE_SILITHUS_GATHERED_HORDE, 0
 * INSERT INTO worldstate_templates VALUES (1,1377,2317,200);
 * INSERT INTO worldstate_templates VALUES (1,1377,2313,0);
 * INSERT INTO worldstate_templates VALUES (1,1377,2314,0);
*/

void SilithusEnterWorldHook(Player *pPlayer)
{
   if(pPlayer->GetMapId() == MAP_KALIMDOR && pPlayer->GetZoneId() == ZONE_SILITHUS)
      if(pPlayer->HasAura(CENARION_FAVOR))
         if(pPlayer->GetTeam() == winners)
            pPlayer->RemoveAura(CENARION_FAVOR);
}

void SilithusZoneHook(Player *plr, uint32 Zone, uint32 OldZone)
{
   if(Zone == ZONE_SILITHUS && winners == plr->GetTeam())
   {
      plr->CastSpell(plr, dbcSpell.LookupEntry(CENARION_FAVOR), true);
   }
   else if(OldZone == ZONE_SILITHUS && winners == plr->GetTeam())
   {
      plr->RemoveAura(CENARION_FAVOR);
   }
}

void AreatriggerHook(Player *pPlayer, uint32 triggerID)
{
   if( (triggerID == ALLIANCE_RETURN && pPlayer->GetTeam() != TEAM_ALLIANCE) || (triggerID == HORDE_RETURN && pPlayer->GetTeam() != TEAM_HORDE) )
      return;

   if( !pPlayer->HasAura( SILITHYST_SPELL ) ) // if we doesn't have it, do nothing
      return;

   pPlayer->RemoveAura( SILITHYST_SPELL );

   /* Rewards */
   pPlayer->CastSpell(pPlayer, TRACES_OF_SILITHYST, true);
   pPlayer->m_honorPoints += REWARD_HONOR;
   pPlayer->ModStanding(REWARD_REPUTATION_FACTION, REWARD_REPUTATION_VALUE);

   uint32 quest = pPlayer->GetTeam() == TEAM_ALLIANCE ? ALLIANCE_SILITHYST_QUEST : HORDE_SILITHYST_QUEST;
   if(pPlayer->HasQuest(quest))
   {
      // testing needed if these quests are completed automatically or need to be marked done here
   }

   // count scores and update worldstate
   silithyst_gathered[ pPlayer->GetTeam() ]++;
   uint32 worldstate = pPlayer->GetTeam() == TEAM_ALLIANCE ? WORLDSTATE_SILITHUS_GATHERED_ALLIANCE : WORLDSTATE_SILITHUS_GATHERED_HORDE;
   pPlayer->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_SILITHUS, 0, worldstate , silithyst_gathered[ pPlayer->GetTeam() ] );
   if( silithyst_gathered[ pPlayer->GetTeam() ] < SILITHYST_MAX )
      return;

   // we got here, so the team of this player has won the current race   
   winners = pPlayer->GetTeam();

   // reset race so other team has the chance now
   *((uint16*)silithyst_gathered) = 0;
   pPlayer->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_SILITHUS, 0, WORLDSTATE_SILITHUS_SILITHYST_MAX, SILITHYST_MAX);
   pPlayer->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_SILITHUS, 0, WORLDSTATE_SILITHUS_GATHERED_ALLIANCE, 0);
   pPlayer->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_SILITHUS, 0, WORLDSTATE_SILITHUS_GATHERED_HORDE, 0);

   // distribute the winner buff and notifify about the result of the race
   WorldPacket *data = winners == TEAM_ALLIANCE ? sChatHandler.FillSystemMessageData("The Alliance has collected 200 silithyst!") : sChatHandler.FillSystemMessageData("The Horde has collected 200 silithyst!");
   for(PlayerStorageMap::iterator itr = pPlayer->GetMapMgr()->m_PlayerStorage.begin(); itr != pPlayer->GetMapMgr()->m_PlayerStorage.end(); itr++)
   {
      itr->second->SendPacket(data);
      if(itr->second->GetTeam() == winners)
         itr->second->CastSpell(itr->second, CENARION_FAVOR, true);
      else
         itr->second->RemoveAura(CENARION_FAVOR);
   }
}

class SilithystPickup : public GameObjectAIScript
{
public:
   ADD_GAMEOBJECT_FACTORY_FUNCTION(SilithystPickup)
   SilithystPickup(GameObject *goinstance) : GameObjectAIScript(goinstance) {}

   void OnActivate(Player *pPlayer)
   {
      if( pPlayer->HasAura( SILITHYST_SPELL ) )
         return;
   
      pPlayer->CastSpell(pPlayer, SILITHYST_SPELL, true);
      _gameobject->Despawn(0, 0);
   }
};

void DropFlag(Aura *aura)
{
   Log.Notice("WorldPvP - Silithus", "removed aura created by spell %u", aura->GetSpellId());
   if(aura->GetSpellId() != SILITHYST_SPELL)
      return;

   if(aura->GetUnitCaster() && aura->GetUnitCaster()->IsPlayer())
      Log.Notice("WorldPvP - Silithus", "aura caster was a player");
   
   if(!aura->GetTarget()->IsPlayer())
   {   Log.Notice("WorldPvP - Silithus", "aura target was a player");
      return;}

   Player *pPlayer = TO_PLAYER(aura->GetTarget());

   uint32 triggerID = pPlayer->GetTeam() == TEAM_ALLIANCE ? ALLIANCE_RETURN : HORDE_RETURN;
   // we have to use AreaTrigger.dbc here
   AreaTrigger* pAreaTrigger = AreaTriggerStorage.LookupEntry(triggerID);
   if(pAreaTrigger)
      if( pPlayer->CalcDistance(pAreaTrigger->x,pAreaTrigger->y,pAreaTrigger->z) > 5.0f )
         pPlayer->GetMapMgr()->GetInterface()->SpawnGameObject(SILITHYST_MOUND, pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0, true, 0, 0);
}

void SetupPvPSilithus(ScriptMgr* mgr)
{
   mgr->register_gameobject_script(SILITHYST_GEYSER, &SilithystPickup::Create);
   mgr->register_gameobject_script(SILITHYST_MOUND, &SilithystPickup::Create);

   mgr->register_hook(SERVER_HOOK_EVENT_ON_ENTER_WORLD, (void *)&SilithusEnterWorldHook);
   mgr->register_hook(SERVER_HOOK_EVENT_ON_ZONE, (void *)&SilithusZoneHook);
   mgr->register_hook(SERVER_HOOK_EVENT_ON_AREATRIGGER, (void *)&AreatriggerHook);
   mgr->register_hook(SERVER_HOOK_EVENT_ON_AURA_REMOVE, (void *)&DropFlag);
}
