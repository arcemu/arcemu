/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009 ArcEmu Team <http://www.arcemu.org/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
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

#include "Setup.h"

// -----------------------------------------------------------------------------
// Quest 12532 - Flown the Coop!
// Quest 12702 - Chicken Party! (by bartus

class ChickenEscapee : public CreatureAIScript{
public:
	ADD_CREATURE_FACTORY_FUNCTION( ChickenEscapee );
	ChickenEscapee( Creature *c ) : CreatureAIScript( c ){}
	
	void OnLoad(){
		RegisterAIUpdateEvent(1000);
	}

	void AIUpdate(){
		// Let's see if we are netted
		Aura *a = _unit->FindAura( 51959 );
		if( a != NULL ){
			Unit *Caster = a->GetUnitCaster();
			if( Caster->IsPlayer() ){
				
				QuestLogEntry *qle = TO_PLAYER( Caster )->GetQuestLogForEntry(12532);
				if( qle == NULL )
					qle = TO_PLAYER( Caster )->GetQuestLogForEntry(12702);

				if( qle != NULL ){
					// casting the spell that will create the item for the player
					_unit->CastSpell( Caster, 51037, true );
					_unit->Despawn( 1000, 360000 );
				}
			}
		}
	}
};

#define HEMET 27986
#define HADRIUS 28047
#define TAMARA 28568
#define GOSSIP_TASTETEST "Care to try Grimbooze Thunderbrew's Jungle punch?"

class SCRIPT_DECL HemetTasteTest : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player* plr, bool AutoSend)
    {
        if(plr->HasQuest(12645))
		{
			GossipMenu *Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 40002, plr);
			Menu->AddItem( 0, GOSSIP_TASTETEST, 1);
			if(AutoSend)
				Menu->SendTo(plr);
		}
    }

    void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * Code)
    {
		Creature* pCreature = (pObject->IsCreature())?(TO_CREATURE(pObject)):NULL;
		if(pCreature==NULL)
			return;

        switch(IntId)
        {
		case 1:
			{
				string msg = "Aye, I'll try it.";
				pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, msg.c_str());
                string msg2 = "That's exactly what I needed!";
                string msg3 = "It's got my vote! That'll put hair on your chest like nothing else will.";
                pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, msg2.c_str(), 2000);
                pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, msg3.c_str(), 4000);
                QuestLogEntry *qle = plr->GetQuestLogForEntry(12645);
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

class SCRIPT_DECL HadriusTasteTest : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player* plr, bool AutoSend)
    {
		if(plr->HasQuest(12645))
		{
			GossipMenu *Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 40002, plr);
			Menu->AddItem( 0, GOSSIP_TASTETEST, 1);

			if(AutoSend)
				Menu->SendTo(plr);
		}
    }

    void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * Code)
    {
		Creature* pCreature = (pObject->IsCreature())?(TO_CREATURE(pObject)):NULL;
		if(pCreature==NULL)
			return;

        switch(IntId)
        {
		case 1:
			{
				string msg = "I'm always up for something of Grimbooze's.";
				pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, msg.c_str());
                string msg2 = "Well, so far, it tastes like something my wife would drink...";
                string msg3 = "Now, there's the kick I've come to expect from Grimbooze's drinks! I like it!";
                pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, msg2.c_str(), 2000);
                pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, msg3.c_str(), 4000);
                QuestLogEntry *qle = plr->GetQuestLogForEntry(12645);
				if(qle == NULL)
					return;
				
				if(qle->GetMobCount(0) != 0)
					return;
				qle->SetMobCount(1, 1);
				qle->SendUpdateAddKill(1);
				qle->UpdatePlayerFields();
			}break;
		}
    }

};

class SCRIPT_DECL TamaraTasteTest : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player* plr, bool AutoSend)
    {
		if(plr->HasQuest(12645))
		{
			GossipMenu *Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 40002, plr);
			Menu->AddItem( 0, GOSSIP_TASTETEST, 1);

			if(AutoSend)
				Menu->SendTo(plr);
		}
    }

    void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * Code)
    {
		Creature* pCreature = (pObject->IsCreature())?(TO_CREATURE(pObject)):NULL;
		if(pCreature==NULL)
			return;

        switch(IntId)
        {
		case 1:
			{
				string msg = " Sure!";
				pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, msg.c_str());
                string msg2 = "Oh my...";
                string msg3 = "Tastes like I'm drinking... engine degreaser!";
                pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, msg2.c_str(), 2000);
                pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, msg3.c_str(), 4000);
                QuestLogEntry *qle = plr->GetQuestLogForEntry(12645);
				if(qle == NULL)
					return;
				
				if(qle->GetMobCount(0) != 0)
					return;
				qle->SetMobCount(2, 1);
				qle->SendUpdateAddKill(2);
				qle->UpdatePlayerFields();
			}break;
		}
    }

};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void SetupSholazarBasin( ScriptMgr * mgr )
{
	GossipScript * gossip1 = new HemetTasteTest();
	mgr->register_gossip_script(HEMET, gossip1);
	GossipScript * gossip2 = new HadriusTasteTest();
	mgr->register_gossip_script(HADRIUS, gossip2);
	GossipScript * gossip3 = new TamaraTasteTest();
	mgr->register_gossip_script(TAMARA, gossip3);

	mgr->register_creature_script( 28161, &ChickenEscapee::Create );
}