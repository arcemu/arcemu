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

bool ChickenNet( uint32 i, Spell* pSpell )
{
	if( !pSpell->u_caster->IsPlayer() )
        return true;

	Player* pPlayer = TO_PLAYER( pSpell->u_caster );

	QuestLogEntry *qle = pPlayer->GetQuestLogForEntry( 12532 );
	if( qle == NULL )
	{
		qle = pPlayer->GetQuestLogForEntry( 12702 );
		if( qle == NULL )
			return true;
	}

	
	if( !pSpell->GetUnitTarget()->IsCreature() )
		return true;

	Creature* Chicken = TO_CREATURE( pSpell->GetUnitTarget() );

	if( Chicken != NULL && Chicken->GetEntry() == 28161 )
	{
		Chicken->Despawn( 5000, 360000 );
		pPlayer->CastSpell( pSpell->u_caster, dbcSpell.LookupEntry( 51037 ), true );
	}

   return true;
}

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
		Creature* pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?(TO_CREATURE(pObject)):NULL;
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
		Creature* pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?(TO_CREATURE(pObject)):NULL;
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
		Creature* pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?(TO_CREATURE(pObject)):NULL;
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
	mgr->register_dummy_spell(51959, &ChickenNet);
	GossipScript * gossip1 = new HemetTasteTest();
	mgr->register_gossip_script(HEMET, gossip1);
	GossipScript * gossip2 = new HadriusTasteTest();
	mgr->register_gossip_script(HADRIUS, gossip2);
	GossipScript * gossip3 = new TamaraTasteTest();
	mgr->register_gossip_script(TAMARA, gossip3);
}