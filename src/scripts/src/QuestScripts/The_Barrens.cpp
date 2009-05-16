/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008-2009 Arcemu Team
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

#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), textid, plr); \
    Menu->SendTo(plr);

class SCRIPT_DECL BeatenCorpse : public GossipScript
{
public:
  void GossipHello(Object * pObject, Player * plr, bool AutoSend)
  {
    GossipMenu *Menu;
    objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 3557, plr);

    if(plr->GetQuestLogForEntry(4921) != NULL)
      Menu->AddItem(0, "I inspect the body further.", 1);
    
    if(AutoSend)
      Menu->SendTo(plr);
  }

  void GossipSelectOption(Object * pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
  {      
    GossipMenu *Menu;

    switch(IntId)	
		{
			case 1:
      {
         SendQuickMenu(3558);

         QuestLogEntry *qle = plr->GetQuestLogForEntry(4921);
         if(qle == NULL)
           return;
           
         if(qle->GetMobCount(0) != 0)
           return;

         qle->SetMobCount(0, 1);
         qle->SendUpdateAddKill(0);
         qle->UpdatePlayerFields();
		  }break;
    }
  }
};
// Verog the Dervish Quest 851
class Verog_Spawned : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(Verog_Spawned);
    Verog_Spawned(Creature* pCreature) : CreatureAIScript(pCreature) {}
    void OnDied(Unit *mKiller)
    {
        if(!mKiller->IsPlayer())
        return;
        Player *plr = (Player*)mKiller;
        QuestLogEntry *qle = plr->GetQuestLogForEntry(851);
            if(qle == NULL)
            return;
        uint8 chance = (uint8)RandomUInt(3);
        switch(chance)
        {
        case 1:
            Creature*Verog=plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-1210.59, -2724.64, 106.693,3395);
            if (Verog==NULL)
            {
            _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "I am slain! Summon Verog!");
            Creature*Verog1=sEAS.SpawnCreature(plr, 3395, -1210.59, -2724.64, 106.693, 4.8264, 120000);
            Verog1->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL,"I am summoned! Intruders, come to my tent and face your death!");
            }
        }
    }
};

// The Escape Quest 863
class SCRIPT_DECL TheEscape : public QuestScript
{
public:
    void OnQuestStart( Player * pPlayer, QuestLogEntry * qLogEntry)
    {
        if( pPlayer == NULL || pPlayer->GetMapMgr() == NULL)
            return;    
        Creature*WShredder=pPlayer->GetMapMgr()->GetSqlIdCreature(356295);
        if(WShredder == NULL)
            return;
        WShredder->m_escorter = pPlayer;  
        
        WShredder->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Alright, alright! I think I can figure out how to operate this thing...");
        WShredder->SetUInt32Value(UNIT_NPC_FLAGS, 0);
        WShredder->GetAIInterface()->SetAllowedToEnterCombat(false);
        WShredder->GetAIInterface()->StopMovement(3000);
        WShredder->GetAIInterface()->setMoveType(2);
        WShredder->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, pPlayer->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
        WShredder->_setFaction();
        sEAS.WaypointCreate(WShredder,1109.15, -3104.15, 82.33, 2.37, 0, 0, 0);
        sEAS.WaypointCreate(WShredder,1099.68, -3099.87, 83.28, 1.47, 0, 0, 0);
        sEAS.WaypointCreate(WShredder,1099.20, -3086.72, 82.26, 1.60, 0, 0, 0);
        sEAS.WaypointCreate(WShredder,1096.65, -3067.67, 82.00, 1.68, 0, 0, 0);
        sEAS.WaypointCreate(WShredder,1093.13, -3049.07, 82.53, 2.00, 0, 0, 0);
        sEAS.WaypointCreate(WShredder,1086.50, -3034.86, 82.38, 0.87, 0, 0, 0);
        sEAS.WaypointCreate(WShredder,1097.37, -3021.30, 83.95, 0.63, 0, 0, 0);
        sEAS.WaypointCreate(WShredder,1111.10, -3010.41, 86.16, 0.31, 0, 0, 0);

        sEAS.WaypointCreate(WShredder,1134.75, -3003.41, 91.68, 2.04, 5000, 0, 0);
        sEAS.WaypointCreate(WShredder,1126.83, -2989.33, 91.82, 2.83, 0, 0, 0);
        sEAS.WaypointCreate(WShredder,1109.44, -2984.58, 91.67, 2.85, 0, 0, 0);
        sEAS.WaypointCreate(WShredder,1094.93, -2980.79, 91.83, 5.83, 5000, 0, 0);
    }
};
class WShredder : public CreatureAIScript
{
public:
      ADD_CREATURE_FACTORY_FUNCTION(WShredder);
      WShredder(Creature* pCreature) : CreatureAIScript(pCreature) {}
    void OnReachWP(uint32 iWaypointId, bool bForwards)
    {
        char msg[100];
        int WP;
        WP = _unit->GetAIInterface()->getCurrentWaypoint();
        WayPoint* wp = _unit->GetAIInterface()->getWayPoint(WP);
        if(iWaypointId == 1 ||iWaypointId == 2 ||iWaypointId == 3 ||iWaypointId == 4 ||iWaypointId == 5 ||iWaypointId == 6 ||iWaypointId == 7 ||iWaypointId == 8||iWaypointId == 9 ||iWaypointId == 10 ||iWaypointId == 11)
        {
            if (_unit->CombatStatus.IsInCombat())
            wp->waittime = 15000;
        }
        if(iWaypointId == 3)    
        {
            sprintf(msg, "Okay, I think I've got it, now. Follow me, %s!", _unit->m_escorter->GetName());
            if(_unit->m_escorter == NULL)
            return;
            _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, msg);
        }
        if(iWaypointId == 9)    
        {
            _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Looks like we're out of the woods, eh? Wonder what this does...");
        }
        if(iWaypointId == 10)
        {
            _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Hm... I don't think this blinking red light is a good thing...");
        }
        if(iWaypointId == 12)
        {
            sEAS.DeleteWaypoints(_unit);
            _unit->CastSpell(_unit,5162,false);
            _unit->SetStandState(STANDSTATE_DEAD);
            _unit->setDeathState(CORPSE);
            RegisterAIUpdateEvent(4000);
            if(_unit->m_escorter == NULL)
                return;
            Player* pPlayer = _unit->m_escorter;
            _unit->m_escorter = NULL;    
            QuestLogEntry *en = pPlayer->GetQuestLogForEntry(863);
            if(en && en->GetMobCount(0) != 0)
                return;
            en->SetMobCount(0, 1);
            en->SendUpdateAddKill(0);
            en->UpdatePlayerFields();
        }    
    }
     void AIUpdate()
    {
                 RemoveAIUpdateEvent();
        _unit->Despawn(1000,20000);
        _unit->GetMapMgr()->GetInterface()->SpawnCreature(3451, 1094.93, -2980.79, 91.83, 5.83, true,true,0, 0);
    }
    void OnLoad()
    {
        _unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 69);
        _unit->_setFaction();
        _unit->SetStandState(STANDSTATE_STAND);
        _unit->setDeathState(ALIVE);
    }
};

// Echeyakee Quest 881
bool Echeyakee(uint32 i, Spell* pSpell)
{
  Player *pPlayer = (Player*)pSpell->u_caster;
  if(!pPlayer)
    return true;
  if(!pSpell->u_caster->IsPlayer())
    return true;
  QuestLogEntry *en = pPlayer->GetQuestLogForEntry(881);
  if(en == NULL)
    return true;
  Creature*Echeyakee=pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(459.78, -3034.95, 91.68,3475);
    if (Echeyakee==NULL)
    {
    Creature * EcheyakeeSpawn = sEAS.SpawnCreature(pPlayer, 3475, 459.78, -3034.95, 91.68, 3.87,120000);
    EcheyakeeSpawn->GetAIInterface()->MoveTo(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), pPlayer->GetOrientation());
    }
    return true;
};
// Revenge of Gann Quest 849
class FlyingMachine : public GameObjectAIScript
{
public:
    FlyingMachine(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
    static GameObjectAIScript *Create(GameObject * GO) { return new FlyingMachine(GO); }

    void OnActivate(Player * pPlayer)
    {
        TimedEvent * te = TimedEvent::Allocate(this, new CallbackP1<FlyingMachine, GameObject*>(this, &FlyingMachine::Destroy, _gameobject), 0, 1, 1);  
        _gameobject->event_AddEvent(te);
    }
    void Destroy(GameObject * Machine)
    {
        WorldPacket data(SMSG_GAMEOBJECT_DESPAWN_ANIM, 8);
        data << Machine->GetGUID();
        Machine->SendMessageToSet(&data,true);
        Machine->Despawn(0,180000);
    }
};

void SetupBarrens(ScriptMgr *mgr)
{
  GossipScript *gos = (GossipScript*) new BeatenCorpse;
  mgr->register_gossip_script(10668, gos);
  mgr->register_creature_script(3275, &Verog_Spawned::Create);
    mgr->register_creature_script(3274, &Verog_Spawned::Create);
	mgr->register_quest_script(863, CREATE_QUESTSCRIPT(TheEscape));
    mgr->register_creature_script(3439, &WShredder::Create);
	mgr->register_dummy_spell(12189, &Echeyakee);
	mgr->register_gameobject_script(3644, &FlyingMachine::Create);
}
