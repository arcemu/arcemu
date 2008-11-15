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
						casta->CastSpell(Plr, 31808, true);
					else if(Plr->GetStandingRank(942) == 5)
						casta->CastSpell(Plr, 31810, true);
					else if(Plr->GetStandingRank(942) == 6)
						casta->CastSpell(Plr, 31811, true);
					else if(Plr->GetStandingRank(942) == 7)
						casta->CastSpell(Plr, 31815, true);

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
						casta->CastSpell(Plr, 31807, true);
					else if(Plr->GetStandingRank(942) == 5)
						casta->CastSpell(Plr, 31814, true);
					else if(Plr->GetStandingRank(942) == 6)
						casta->CastSpell(Plr, 31813, true);
					else if(Plr->GetStandingRank(942) == 7)
						casta->CastSpell(Plr, 31812, true);

				}break;
			}
		}
	}

	void Destroy()
	{
		delete this;
	}
};

void SetupZangarmarsh(ScriptMgr * mgr)
{
	GossipScript * AMark = (GossipScript*) new AncientMarks();
	mgr->register_gossip_script(17900, AMark);	// Ashyen Ancient of Lore
	mgr->register_gossip_script(17901, AMark);	// Keleth Ancient of War
}
