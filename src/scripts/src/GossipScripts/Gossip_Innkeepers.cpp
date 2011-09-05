/*
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

#include "Setup.h"

// Event of Hallowen control
//#define EVENT_HALLOWEEN		// Decomment this for enable the event on Innkeepers

#define SPELL_TRICK_OR_TREATED  24755
#define SPELL_TREAT             24715
// -------------------------

class InnkeeperGossip : public Arcemu::Gossip::Script
{
	public:
		void OnHello(Object* pObject, Player* Plr);
		void OnSelectOption(Object* pObject, Player* Plr, uint32 Id, const char* Code);
		void Destroy() { delete this; }
};

void InnkeeperGossip::OnHello(Object* pObject, Player* Plr)
{
	Creature* pCreature = (pObject->IsCreature()) ? (TO_CREATURE(pObject)) : NULL;
	if(pCreature == NULL)
		return;
	uint32 TextID = 820;
	uint32 Text = objmgr.GetGossipTextForNpc(pCreature->GetEntry());
	if(Text != 0)
	{
		GossipText* text = NpcTextStorage.LookupEntry(Text);
		if(text != 0)
			TextID = Text;
	}
	Arcemu::Gossip::Menu menu(pCreature->GetGUID(), TextID, 0);

#ifdef	EVENT_HALLOWEEN
	if(!Plr->HasAura(SPELL_TRICK_OR_TREATED))
		menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Trick or Treat!", 4);
#endif

	if(pCreature->isVendor())
		menu.AddItem(Arcemu::Gossip::ICON_VENDOR, Plr->GetSession()->LocalizedWorldSrv(Arcemu::Gossip::VENDOR), 1);

	menu.AddItem(Arcemu::Gossip::ICON_CHAT, Plr->GetSession()->LocalizedWorldSrv(Arcemu::Gossip::INNKEEPER), 2);
	menu.AddItem(Arcemu::Gossip::ICON_CHAT, "What can I do at an inn?", 3);

	sQuestMgr.FillQuestMenu(pCreature, Plr, menu);

	menu.Send(Plr);
}

//#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), textid, Plr); \
//    Menu->SendTo(Plr);

void InnkeeperGossip::OnSelectOption(Object* pObject, Player* Plr, uint32 Id, const char* Code)
{
	Creature* pCreature = (pObject->IsCreature()) ? (TO_CREATURE(pObject)) : NULL;
	if(pCreature == NULL)
		return;

	switch(Id)
	{
		case 1:     // VENDOR
			Plr->GetSession()->SendInventoryList(pCreature);
			break;
		case 2:     // BINDER
			Plr->GetSession()->SendInnkeeperBind(pCreature);
			break;
		case 3:     // WHAT CAN I DO ?
			// Prepare second menu
			Arcemu::Gossip::Menu::SendQuickMenu(pCreature->GetGUID(), 1853, Plr, 2, Arcemu::Gossip::ICON_CHAT, Plr->GetSession()->LocalizedWorldSrv(Arcemu::Gossip::INNKEEPER));
			break;
		case 4:     // EVENT OF HALLOWEEN
			if(!Plr->HasAura(SPELL_TRICK_OR_TREATED))
			{
				pCreature->CastSpell(Plr, SPELL_TRICK_OR_TREATED, true);

				// either trick or treat, 50% chance
				if(rand() % 2)
				{
					Plr->CastSpell(Plr, SPELL_TREAT, true);
				}
				else
				{
					int32 trickspell = 0;
					switch(rand() % 9)
					{
						case 0:
							trickspell = 24753;                     // cannot cast, random 30sec
							break;
						case 1:
							trickspell = 24713;                     // lepper gnome costume
							break;
						case 2:
							if(Plr->getGender() == 0)
							{
								trickspell = 24735;                 // male ghost costume
							}
							else
							{
								trickspell = 24736;                 // female ghostcostume
							}
							break;
						case 3:
							if(Plr->getGender() == 0)
							{
								trickspell = 24711;                 // male ninja costume
							}
							else
							{
								trickspell = 24710;                 // female ninja costume
							}
							break;
						case 4:
							if(Plr->getGender() == 0)
							{
								trickspell = 24708;                 // male pirate costume
							}
							else
							{
								trickspell = 24709;                 // female pirate costume
							}
							break;
						case 5:
							trickspell = 24723;                     // skeleton costume
							break;
					}
					pCreature->CastSpell(Plr, trickspell, true);
				}
			}
			Arcemu::Gossip::Menu::Complete(Plr);
			break;
	}
}

void SetupInnkeepers(ScriptMgr* mgr)
{
	Arcemu::Gossip::Script* gs = new InnkeeperGossip();
	/* Innkeeper List */
	mgr->register_creature_gossip(15174, gs);     //Calandrath
	mgr->register_creature_gossip(18251, gs);     //Caregiver Abidaar
	mgr->register_creature_gossip(16739, gs);     //Caregiver Breel
	mgr->register_creature_gossip(16553, gs);     //Caregiver Chellan
	mgr->register_creature_gossip(18914, gs);     //Caregiver Isel
	mgr->register_creature_gossip(18906, gs);     //Caregiver Ophera Windfury
	mgr->register_creature_gossip(17553, gs);     //Caregiver Topher Loaal
	mgr->register_creature_gossip(21746, gs);     //Caretaker Aluuro
	mgr->register_creature_gossip(19352, gs);     //Dreg Cloudsweeper
	mgr->register_creature_gossip(19531, gs);     //Eyonix
	mgr->register_creature_gossip(21110, gs);     //Fizit "Doc" Clocktock
	mgr->register_creature_gossip(16602, gs);     //Floyd Pinkus
	mgr->register_creature_gossip(19470, gs);     //Gholah
	mgr->register_creature_gossip(23143, gs);     //Horus
	mgr->register_creature_gossip(11116, gs);     //Innkeeper Abeqwa
	mgr->register_creature_gossip(9501, gs);      //Innkeeper Adegwa
	mgr->register_creature_gossip(22922, gs);     //Innkeeper Aelerya
	mgr->register_creature_gossip(6740, gs);      //Innkeeper Allison
	mgr->register_creature_gossip(2352, gs);      //Innkeeper Anderson
	mgr->register_creature_gossip(6739, gs);      //Innkeeper Bates
	mgr->register_creature_gossip(18905, gs);     //Innkeeper Bazil Olof'tazun
	mgr->register_creature_gossip(1247, gs);      //Innkeeper Belm
	mgr->register_creature_gossip(19296, gs);     //Innkeeper Biribi
	mgr->register_creature_gossip(3934, gs);      //Innkeeper Boorand Plainswind
	mgr->register_creature_gossip(6727, gs);      //Innkeeper Brianna
	mgr->register_creature_gossip(7714, gs);      //Innkeeper Byula
	mgr->register_creature_gossip(18907, gs);     //Innkeeper Coryth Stoktron
	mgr->register_creature_gossip(19319, gs);     //Innkeeper Darg Bloodclaw
	mgr->register_creature_gossip(15433, gs);     //Innkeeper Delaniel
	mgr->register_creature_gossip(16458, gs);     //Innkeeper Faralia
	mgr->register_creature_gossip(295, gs);       //Innkeeper Farley
	mgr->register_creature_gossip(5111, gs);      //Innkeeper Firebrew
	mgr->register_creature_gossip(7733, gs);      //Innkeeper Fizzgrimble
	mgr->register_creature_gossip(7737, gs);      //Innkeeper Greul
	mgr->register_creature_gossip(18957, gs);     //Innkeeper Grilka
	mgr->register_creature_gossip(6928, gs);      //Innkeeper Grosk
	mgr->register_creature_gossip(6929, gs);      //Innkeeper Gryshka
	mgr->register_creature_gossip(19232, gs);     //Innkeeper Haelthol
	mgr->register_creature_gossip(6734, gs);      //Innkeeper Hearthstove
	mgr->register_creature_gossip(8931, gs);      //Innkeeper Heather
	mgr->register_creature_gossip(1464, gs);      //Innkeeper Helbrek
	mgr->register_creature_gossip(6272, gs);      //Innkeeper Janene
	mgr->register_creature_gossip(7731, gs);      //Innkeeper Jayka
	mgr->register_creature_gossip(17630, gs);     //Innkeeper Jovia
	mgr->register_creature_gossip(16542, gs);     //Innkeeper Kalarin
	mgr->register_creature_gossip(6930, gs);      //Innkeeper Karakul
	mgr->register_creature_gossip(6747, gs);      //Innkeeper Kauth
	mgr->register_creature_gossip(12196, gs);     //Innkeeper Kaylisk
	mgr->register_creature_gossip(6736, gs);      //Innkeeper Keldamyr
	mgr->register_creature_gossip(18908, gs);     //Innkeeper Kerp
	mgr->register_creature_gossip(6738, gs);      //Innkeeper Kimlya
	mgr->register_creature_gossip(11103, gs);     //Innkeeper Lyshaerya
	mgr->register_creature_gossip(6741, gs);      //Innkeeper Norman
	mgr->register_creature_gossip(6746, gs);      //Innkeeper Pala
	mgr->register_creature_gossip(19571, gs);     //Innkeeper Remi Dodoso
	mgr->register_creature_gossip(5688, gs);      //Innkeeper Renee
	mgr->register_creature_gossip(6735, gs);      //Innkeeper Saelienne
	mgr->register_creature_gossip(19495, gs);     //Innkeeper Shaunessy
	mgr->register_creature_gossip(6737, gs);      //Innkeeper Shaussiy
	mgr->register_creature_gossip(2388, gs);      //Innkeeper Shay
	mgr->register_creature_gossip(9356, gs);      //Innkeeper Shul'kar
	mgr->register_creature_gossip(7736, gs);      //Innkeeper Shyria
	mgr->register_creature_gossip(11106, gs);     //Innkeeper Sikewa
	mgr->register_creature_gossip(6807, gs);      //Innkeeper Skindle
	mgr->register_creature_gossip(5814, gs);      //Innkeeper Thulbek
	mgr->register_creature_gossip(7744, gs);      //Innkeeper Thulfram
	mgr->register_creature_gossip(6790, gs);      //Innkeeper Trelayne
	mgr->register_creature_gossip(16618, gs);     //Innkeeper Velandra
	mgr->register_creature_gossip(11118, gs);     //Innkeeper Vizzie
	mgr->register_creature_gossip(6791, gs);      //Innkeeper Wiley
	mgr->register_creature_gossip(16256, gs);     //Jessica Chambers
	mgr->register_creature_gossip(14731, gs);     //Lard
	mgr->register_creature_gossip(15397, gs);     //Marniel Amberlight
	mgr->register_creature_gossip(18913, gs);     //Matron Tikkit
	mgr->register_creature_gossip(21088, gs);     //Matron Varah
	mgr->register_creature_gossip(6778, gs);      //Melika Isenstrider
	mgr->register_creature_gossip(18245, gs);     //Merajit
	mgr->register_creature_gossip(19046, gs);     //Minalei
	mgr->register_creature_gossip(21744, gs);     //Roldemar
	mgr->register_creature_gossip(16826, gs);     //Sid Limbardi
	mgr->register_creature_gossip(6806, gs);      //Tannok Frosthammer
	mgr->register_creature_gossip(25036, gs);     //Caregiver Inaara

	//cleanup:
	//added 36 new Innkeeper's ,81 working innkeeper's now :P
	//removed Innkeeper Monica(she dos not have gossip option she is from Old Hillsbrad Foothills)
}
