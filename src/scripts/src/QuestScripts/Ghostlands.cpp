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

// Escape from the Catacombs Quest 9212
class SCRIPT_DECL ScapeCatacombs : public QuestScript
{
public:
    void OnQuestStart( Player * pPlayer, QuestLogEntry * qLogEntry)
    {
        if( pPlayer == NULL || pPlayer->GetMapMgr() == NULL)
            return;
        GameObject * CageDoor= pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(7544.91, -7359.97, 162.44, 181152);
        Creature* Lilatha = pPlayer->GetMapMgr()->GetSqlIdCreature(41028);
        if(Lilatha == NULL)
            return;
        //CageDoor->SetUInt32Value(GAMEOBJECT_STATE, 0);
        TimedEvent *te1 = TimedEvent::Allocate(this, new CallbackP1<ScapeCatacombs, GameObject*>(this, &ScapeCatacombs::DoorTrigger, CageDoor), 0, 5000, 1);  
        CageDoor->event_AddEvent(te1);
        Lilatha->m_escorter = pPlayer;  
        Lilatha->GetAIInterface()->setMoveType(2);
        Lilatha->GetAIInterface()->setWaypointToMove(0);
        Lilatha->GetAIInterface()->SetAllowedToEnterCombat(false);
        Lilatha->GetAIInterface()->StopMovement(3000);
        Lilatha->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Let's go");
        Lilatha->SetUInt32Value(UNIT_NPC_FLAGS, 0);
        Lilatha->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, pPlayer->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
        Lilatha->_setFaction();
        sEAS.CreateCustomWaypointMap(Lilatha);
        sEAS.WaypointCreate(Lilatha,7549.33, -7359.91, 162.30, 0.00, 0, 0, 16122);
        sEAS.WaypointCreate(Lilatha,7558.39, -7359.79, 161.73, 5.63, 0, 0, 16122);
        sEAS.WaypointCreate(Lilatha,7570.13, -7369.35, 161.73, 0.59, 0, 256, 16122);
        sEAS.WaypointCreate(Lilatha,7583.67, -7359.17, 162.26, 0.03, 0, 256, 16122);
        sEAS.WaypointCreate(Lilatha,7600.83, -7358.63, 162.26, 5.74, 0, 0, 16122);
        sEAS.WaypointCreate(Lilatha,7612.82, -7365.42, 162.35, 5.82, 0, 0, 16122);
        sEAS.WaypointCreate(Lilatha,7633.08, -7375.32, 162.35, 4.87, 0, 0, 16122);
        sEAS.WaypointCreate(Lilatha,7637.33, -7401.16, 162.14, 4.72, 0, 0, 16122);
        sEAS.WaypointCreate(Lilatha,7637.71, -7428.67, 162.69, 4.72, 0, 0, 16122);
        sEAS.WaypointCreate(Lilatha,7638.22, -7461.55, 163.34, 4.72, 5000, 0, 16122);
        sEAS.WaypointCreate(Lilatha,7639.39, -7480.21, 159.37, 4.83, 0, 0, 16122);
        sEAS.WaypointCreate(Lilatha,7642.41, -7502.61, 154.00, 4.10, 0, 0, 16122);
        sEAS.WaypointCreate(Lilatha,7628.98, -7523.71, 153.58, 4.05, 4000, 0, 16122);
        sEAS.WaypointCreate(Lilatha,7619.02, -7537.16, 151.78, 4.05, 0, 256, 16122);
        sEAS.WaypointCreate(Lilatha,7610.49, -7549.48, 149.85, 4.03, 0, 256, 16122);
        sEAS.WaypointCreate(Lilatha,7599.84, -7562.48, 146.10, 4.03, 0, 256, 16122);
        sEAS.WaypointCreate(Lilatha,7588.82, -7575.95, 148.03, 4.16, 0, 256, 16122);
        sEAS.WaypointCreate(Lilatha,7580.16, -7591.29, 148.60, 4.04, 0, 256, 16122);
        sEAS.WaypointCreate(Lilatha,7566.65, -7608.10, 145.93, 4.44, 0, 256, 16122);
        sEAS.WaypointCreate(Lilatha,7563.14, -7622.38, 150.58, 4.55, 0, 256, 16122);
        sEAS.WaypointCreate(Lilatha,7560.71, -7637.86, 151.27, 4.81, 0, 256, 16122);
        sEAS.WaypointCreate(Lilatha,7562.76, -7659.50, 151.25, 6.10, 0, 256, 16122);
        sEAS.WaypointCreate(Lilatha,7590.54, -7662.60, 151.65, 6.00, 5000, 256, 16122);
        sEAS.WaypointCreate(Lilatha,7607.70, -7667.36, 153.99, 6.00, 3000, 0, 16122);
        sEAS.WaypointCreate(Lilatha,7611.17, -7669.34, 153.99, 5.79, 0, 0, 16122);
    }
    void DoorTrigger (GameObject * CageDoorClose)
    {
    //CageDoorClose->SetUInt32Value(GAMEOBJECT_STATE,1);
    }
};
class Ranger_Lilatha : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(Ranger_Lilatha);
    Ranger_Lilatha(Creature* pCreature) : CreatureAIScript(pCreature) {}

    void OnReachWP(uint32 iWaypointId, bool bForwards)
    {
        int WP;
        WP = _unit->GetAIInterface()->getCurrentWaypoint();
        WayPoint* wp = _unit->GetAIInterface()->getWayPoint(WP);
        if (iWaypointId == 1||2)
        {
            if (_unit->CombatStatus.IsInCombat())
            wp->waittime = 15000; //Feel free to extend the time if it is too challenging for you
        }
        if(iWaypointId == 13)
        {
            _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Finally I can see the Farstrider Enclave from here, we must hurry!");
            _unit->Emote(EMOTE_ONESHOT_POINT);
        }
        if(iWaypointId == 23)
        {
            _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Thank you for saving my life and bringing me back to safety");
            _unit->Emote(EMOTE_ONESHOT_KNEEL);
        }
        if(iWaypointId == 24)
        {
            _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Captain Helios, I've been rescued from the Amani catacombs. Reporting for duty, sir!");
        }
        if(iWaypointId == 25)
        {
            _unit->Despawn(10000,1000);
            sEAS.DeleteWaypoints(_unit);
            Creature*Helios = _unit->GetMapMgr()->GetSqlIdCreature(41405);
            if (Helios == NULL)
                return;
            Helios->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Lilatha, get someone to look at those injuries. Thank you for bringing her back safely.");
            Helios->Emote(EMOTE_ONESHOT_NO);
            _unit->Emote(EMOTE_ONESHOT_SALUTE);
            if(_unit->m_escorter == NULL)
                return;
            Player* plr = _unit->m_escorter;
            _unit->m_escorter = NULL;    
            QuestLogEntry *en = plr->GetQuestLogForEntry(9212);
            if(en && en->GetMobCount(0) == 0)
            {
                en->SetMobCount(0, 1);
                en->SendUpdateAddKill(0);
                en->UpdatePlayerFields();
                en->SendQuestComplete();
            }
        }
    }
     void OnLoad()
      {
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        _unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 1602);
        _unit->_setFaction();
        _unit->SetUInt32Value(UNIT_NPC_FLAGS, 2);    
      }
};

// Captives at Deatholme Quest 9164
class SCRIPT_DECL Captive1 : public GossipScript
{
public:
        void GossipHello(Object* pObject, Player* pPlayer, bool AutoSend)
        {
        if(!pPlayer)
            return;
        GossipMenu *Menu;
        Creature *Captive1 = (Creature*)(pObject);
        if (Captive1 == NULL)
            return;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 8461, pPlayer);
        QuestLogEntry *en = pPlayer->GetQuestLogForEntry(9164);
        if(en && en->GetMobCount(1) == 0 && pPlayer->GetItemInterface()->GetItemCount(22628) > 0)
            Menu->AddItem( 0, "Administer the restorative draught!", 1);
        if(AutoSend)
            Menu->SendTo(pPlayer);
        }
    void GossipSelectOption(Object* pObject, Player* pPlayer, uint32 Id, uint32 IntId, const char * EnteredCode)
        {
        if(!pPlayer)
            return;
        Creature *Captive1 = (Creature*)(pObject);
        if (Captive1 == NULL)
            return;
        switch (IntId)
        {
            case 0:
                GossipHello(pObject, pPlayer, true);
                break;
            case 1:
            {
            GossipMenu *Menu;
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 8463, pPlayer);
            Menu->AddItem( 0, "You're free to go friend. The way out is safe.", 2);
            Menu->SendTo(pPlayer);
                return;
            }break;
            case 2:
            {
            QuestLogEntry *en = pPlayer->GetQuestLogForEntry(9164);
            if(en && en->GetMobCount(1) == 0)
            {
                en->SetMobCount(1, 1);
                en->SendUpdateAddKill(1);
                en->UpdatePlayerFields();
                if(!Captive1)
                    return;
                Captive1->setDeathState(ALIVE);
                Captive1->SetUInt32Value(UNIT_FIELD_FLAGS, 33554432);
                Captive1->SetStandState(STANDSTATE_STAND);
                Captive1->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Th...Thank you I thought I was going to die");
                Captive1->SetUInt32Value(UNIT_NPC_FLAGS, 0);
                Captive1->GetAIInterface()->StopMovement(3000);
                Captive1->SetUInt32Value(UNIT_NPC_EMOTESTATE, 16);
                Captive1->GetAIInterface()->setMoveType(2);
                sEAS.WaypointCreate(Captive1,6436.93, -6627.52, 107.43, 0, 0, 0, 0);
                sEAS.WaypointCreate(Captive1,6435.24, -6618.63, 107.43, 0, 0, 0, 0);
                sEAS.WaypointCreate(Captive1,6427.10, -6616.97, 109.25, 0, 0, 0, 0);
                sEAS.WaypointCreate(Captive1,6427.39, -6611.72, 111.28, 0, 0, 0, 0);
                return;
            }break;
            }
        }
    }
        void Destroy()
        {
            delete this;
        }
};
class SCRIPT_DECL Captive2 : public GossipScript
{
public:
        void GossipHello(Object* pObject, Player* pPlayer, bool AutoSend)
        {
        if(!pPlayer)
            return;
        GossipMenu *Menu;
        Creature *Captive2 = (Creature*)(pObject);
        if (Captive2 == NULL)
            return;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 8459, pPlayer);
        QuestLogEntry *en = pPlayer->GetQuestLogForEntry(9164);
        if(en && en->GetMobCount(0) == 0 && pPlayer->GetItemInterface()->GetItemCount(22628) > 0)
            Menu->AddItem( 0, "Administer the restorative draught!", 1);
        if(AutoSend)
            Menu->SendTo(pPlayer);
        }
    void GossipSelectOption(Object* pObject, Player* pPlayer, uint32 Id, uint32 IntId, const char * EnteredCode)
        {
        if(!pPlayer)
            return;
        Creature *Captive2 = (Creature*)(pObject);
        if (Captive2 == NULL)
            return;
        switch (IntId)
        {
            case 0:
                GossipHello(pObject, pPlayer, true);
                break;
            case 1:
            {
            GossipMenu *Menu;
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 8460, pPlayer);
            Menu->AddItem( 0, "The way out is safe, run quickly!", 2);
            Menu->SendTo(pPlayer);
                return;
            }break;
            case 2:
            {
            QuestLogEntry *en = pPlayer->GetQuestLogForEntry(9164);
            if(en && en->GetMobCount(0) == 0)
            {
                en->SetMobCount(0, 1);
                en->SendUpdateAddKill(0);
                en->UpdatePlayerFields();
                if(!Captive2)
                    return;
                Captive2->setDeathState(ALIVE);
                Captive2->SetUInt32Value(UNIT_FIELD_FLAGS, 33554432);
                Captive2->SetStandState(STANDSTATE_STAND);
                Captive2->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Thanks I shouldnt' ve  never left Silverpine Forest");
                Captive2->SetUInt32Value(UNIT_NPC_FLAGS, 0);
                Captive2->GetAIInterface()->StopMovement(3000);
                Captive2->SetUInt32Value(UNIT_NPC_EMOTESTATE, 16);
                Captive2->GetAIInterface()->setMoveType(2);
                sEAS.WaypointCreate(Captive2,6639.50f, -6343.87, 8.94, 6.23, 0, 0, 0);
                sEAS.WaypointCreate(Captive2,6647.37f, -6344.60, 9.13, 6.16, 0, 0, 0);
                sEAS.WaypointCreate(Captive2,6660.66f, -6345.64, 15.34, 1.41, 0, 0, 0);
                sEAS.WaypointCreate(Captive2,6662.54f, -6331.34, 20.89, 2.99, 0, 0, 0);
                return;
            }break;
            }
        }
    }
        void Destroy()
        {
            delete this;
        }
};
class SCRIPT_DECL Captive3 : public GossipScript
{
public:
        void GossipHello(Object* pObject, Player* pPlayer, bool AutoSend)
        {
        if(!pPlayer)
            return;
        GossipMenu *Menu;
        Creature *Captive3 = (Creature*)(pObject);
        if (Captive3 == NULL)
            return;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 8457, pPlayer);
        QuestLogEntry *en = pPlayer->GetQuestLogForEntry(9164);
        if(en && en->GetMobCount(3) == 0 && pPlayer->GetItemInterface()->GetItemCount(22628) > 0)
            Menu->AddItem( 0, "Administer the restorative draught!", 1);
        if(AutoSend)
            Menu->SendTo(pPlayer);
        }
    void GossipSelectOption(Object* pObject, Player* pPlayer, uint32 Id, uint32 IntId, const char * EnteredCode)
        {
        if(!pPlayer)
            return;
        Creature *Captive3 = (Creature*)(pObject);
        if (Captive3 == NULL)
            return;
        switch (IntId)
        {
            case 0:
                GossipHello(pObject, pPlayer, true);
                break;
            case 1:
            {
            GossipMenu *Menu;
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 8456, pPlayer);
            Menu->AddItem( 0, "You're free to go friend. The way out is safe.", 2);
            Menu->SendTo(pPlayer);
                return;
            }break;
            case 2:
            {
            QuestLogEntry *en = pPlayer->GetQuestLogForEntry(9164);
            if(en && en->GetMobCount(3) == 0)
            {
                en->SetMobCount(3, 1);
                en->SendUpdateAddKill(3);
                en->UpdatePlayerFields();
                if(!Captive3)
                    return;
                Captive3->setDeathState(ALIVE);
                Captive3->SetUInt32Value(UNIT_FIELD_FLAGS, 33554432);
                Captive3->SetStandState(STANDSTATE_STAND);
                Captive3->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You have my thanks!");
                Captive3->SetUInt32Value(UNIT_NPC_FLAGS, 0);
                Captive3->GetAIInterface()->StopMovement(3000);
                Captive3->SetUInt32Value(UNIT_NPC_EMOTESTATE, 16);
                Captive3->GetAIInterface()->setMoveType(2);
                sEAS.WaypointCreate(Captive3,6291.62f, -6362.97, 78.01, 1.75, 0, 0, 0);
                sEAS.WaypointCreate(Captive3,6304.57f, -6363.91, 78.02, 4.85, 0, 0, 0);
                sEAS.WaypointCreate(Captive3,6306.03f, -6371.16, 79.45, 5.69, 0, 0, 0);
                sEAS.WaypointCreate(Captive3,6312.62f, -6372.43, 81.79, 6.25, 0, 0, 0);
                return;
            }break;
            }
        }
    }
        void Destroy()
        {
            delete this;
        }
};
class CaptiveState : public CreatureAIScript
{
public:
      ADD_CREATURE_FACTORY_FUNCTION(CaptiveState);
      CaptiveState(Creature* pCreature) : CreatureAIScript(pCreature) {}
      void OnLoad()
      {
        _unit->SetUInt32Value(UNIT_NPC_FLAGS, 1);
        _unit->SetUInt32Value(UNIT_FIELD_FLAGS, 0);
        _unit->setDeathState(CORPSE);
        _unit->SetStandState(STANDSTATE_DEAD);
      }
    void OnReachWP(uint32 iWaypointId, bool bForwards)
    {
        if(iWaypointId == 4)
        {
            sEAS.DeleteWaypoints(_unit);
            _unit->Despawn(1000,180000);
        }
    }
};

void SetupGhostlands(ScriptMgr * mgr)
{
	mgr->register_creature_script(16295, &Ranger_Lilatha::Create);
    mgr->register_quest_script(9212, CREATE_QUESTSCRIPT(ScapeCatacombs));
	GossipScript * Captive1Gossip = (GossipScript*) new Captive1();
    mgr->register_gossip_script(16206, Captive1Gossip);
    GossipScript * Captive2Gossip = (GossipScript*) new Captive2();
    mgr->register_gossip_script(16208, Captive2Gossip);
    GossipScript * Captive3Gossip = (GossipScript*) new Captive3();
    mgr->register_gossip_script(16209, Captive3Gossip);
    mgr->register_creature_script(16206, &CaptiveState::Create);
    mgr->register_creature_script(16208, &CaptiveState::Create);
    mgr->register_creature_script(16209, &CaptiveState::Create);
}
