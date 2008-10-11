/*
 * WEmu Scripts for WEmu MMORPG Server
 * Copyright (C) 2008 WEmu Team
 * Based on Moon++ Scripts for arcemu MMORPG Server
 * Copyright (C) 2005-2008 arcemu Team
 * Copyright (C) 2007-2008 Moon++ Team
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
#include "../EAS/EasyFunctions.h"

#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), textid, Plr); \
    Menu->SendTo(Plr);

class SCRIPT_DECL AncientMarks : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		uint32 entry = pObject->GetEntry();
		char *text = "";
		uint32 TextId = 0;

		if(entry == 17900)
		{
			text = "Grant me your mark, wise ancient.";
			TextId = 9176;
		}
		else if(entry == 17901)
		{
			text = "Grant me your mark, mighty ancient.";
			TextId = 9177;
		}

		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), TextId, Plr);

		if(Plr->HasFinishedQuest(9785) || Plr->GetQuestLogForEntry(9785))
			Menu->AddItem(0, text, 1);

		if(AutoSend)
			Menu->SendTo(Plr);
	}

	void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		if(IntId == 1)
		{
			QuestLogEntry *en = Plr->GetQuestLogForEntry(9785);
			Creature *casta = ((Creature*)pObject);
			switch (pObject->GetEntry())
			{
			case 17900:
				{
					if(en && en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
					{
						en->SetMobCount(0, 1);
						en->SendUpdateAddKill(0);
						en->UpdatePlayerFields();
					}

					if(Plr->GetStandingRank(942) == 4)
					{
						casta->CastSpell(Plr, dbcSpell.LookupEntry(31808), true);
						casta->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You have my blessing");
					}
					else if(Plr->GetStandingRank(942) == 5)
					{
						casta->CastSpell(Plr, dbcSpell.LookupEntry(31810), true);
						casta->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You have my blessing");
					}
					else if(Plr->GetStandingRank(942) == 6)
					{
						casta->CastSpell(Plr, dbcSpell.LookupEntry(31811), true);
						casta->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You have my blessing");
					}
					else if(Plr->GetStandingRank(942) == 7)
					{
						casta->CastSpell(Plr, dbcSpell.LookupEntry(31815), true);
						casta->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You have my blessing");
					}

				}break;
			case 17901:
				{
					if(en && en->GetMobCount(1) < en->GetQuest()->required_mobcount[1])
					{
						en->SetMobCount(1, 1);
						en->SendUpdateAddKill(1);
						en->UpdatePlayerFields();
					}

					Creature *casta = ((Creature*)pObject);
					if(Plr->GetStandingRank(942) == 4)
					{
						casta->CastSpell(Plr, dbcSpell.LookupEntry(31807), true);
						casta->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You have my blessing");
					}
					else if(Plr->GetStandingRank(942) == 5)
					{
						casta->CastSpell(Plr, dbcSpell.LookupEntry(31814), true);
						casta->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You have my blessing");
					}
					else if(Plr->GetStandingRank(942) == 6)
					{
						casta->CastSpell(Plr, dbcSpell.LookupEntry(31813), true);
						casta->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You have my blessing");
					}
					else if(Plr->GetStandingRank(942) == 7)
					{
						casta->CastSpell(Plr, dbcSpell.LookupEntry(31812), true);
						casta->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You have my blessing");
					}

				}break;
			}
		}
	}

	void Destroy()
	{
		delete this;
	}
};

class SCRIPT_DECL ElderKuruti : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		if(!Plr->GetItemInterface()->GetItemCount(24573, true))
		{
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9226, Plr);
			Menu->AddItem( 0, "Offer treat", 1);
			Menu->SendTo(Plr);
		}
	}

	void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		GossipMenu *Menu;
		switch(IntId)
		{
		case 1:
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9227, Plr);
				Menu->AddItem( 0, "Im a messenger for Draenei", 2);
				Menu->SendTo(Plr);
				break;
		case 2:
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9229, Plr);
				Menu->AddItem( 0, "Get message", 3);
				Menu->SendTo(Plr);
				break;
		case 3:
			if(!Plr->GetItemInterface()->GetItemCount(24573, true))
			{
				sEAS.AddItem(24573, Plr);
				if(Plr->GetItemInterface()->GetItemCount(24573, true))
					SendQuickMenu(9231);
			}
			else
				SendQuickMenu(9231);
			break;
		}
	}
};


void SetupZangarmarsh(ScriptMgr * mgr)
{
	GossipScript * AMark = (GossipScript*) new AncientMarks();
	mgr->register_gossip_script(17900, AMark);	// Ashyen Ancient of Lore
	mgr->register_gossip_script(17901, AMark);	// Keleth Ancient of War
	mgr->register_gossip_script(18197, CREATE_GOSSIPSCRIPT(ElderKuruti));
}
