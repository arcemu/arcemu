/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2012 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "StdAfx.h"
using namespace Arcemu;

Gossip::Item::Item(size_t itemid, uint8 icon)
{
	id_ = (uint16)itemid;
	icon_ = icon;
	text_ = "";
	boxmessage_ = "";
	boxmoney_ = 0;
	coded_ = false;
}

Gossip::Item::Item(size_t itemid, uint8 icon, const char* text, bool coded/*= false*/, size_t boxmoney/*=0*/, const char* boxmessage/*=NULL*/)
{
	id_ = (uint16)itemid;
	icon_ = icon;
	text_ = (text != NULL) ? text : "";
	coded_ = coded;
	boxmoney_ = boxmoney;
	boxmessage_ = (boxmessage != NULL) ? boxmessage : "";
}

WorldPacket & Gossip::operator<<(WorldPacket & packet, const Gossip::Item & item)
{
	packet << uint32(item.id_) << item.icon_ << item.coded_ << item.boxmoney_ << item.text_ << item.boxmessage_;
	return packet;
}

template<uint32 size>
StackBuffer<size>& Gossip::operator<<(StackBuffer<size>& packet, const Gossip::Item & item)
{
	packet << uint32(item.id_) << item.icon_ << item.coded_ << item.boxmoney_ << item.text_ << item.boxmessage_;
	return packet;
}


Gossip::Menu::Menu(uint64 Creature_Guid, uint32 Text_Id, uint32 language) : textid_(Text_Id), guid_(Creature_Guid), language_(language)
{
}

Gossip::Menu::Menu(Object* obj, uint32 textid, uint32 language)
{
	guid_ = obj->GetGUID();
	textid_ = textid;
	language_ = language;
}

void Gossip::Menu::AddItem(uint8 icon, const char* itemtext, uint32 itemid, bool coded/*=false*/)
{
	Gossip::Item item(itemid, icon);
	item.text_ = (itemtext != NULL) ? itemtext : "";
	item.coded_ = coded;
	this->itemlist_.push_back(item);
}

void Gossip::Menu::AddItem(uint8 icon, const char* itemtext, uint32 itemid, uint32 boxmoney/*=0*/, const char* boxtext/*=NULL*/, bool coded/*=false*/)
{
	Gossip::Item item(itemid, icon, itemtext, coded, boxmoney, boxtext);
	this->itemlist_.push_back(item);
}

void Gossip::Menu::RemoveItem(uint32 id)
{
	for(Gossip::ItemList::iterator itr = itemlist_.begin(); itr != itemlist_.end(); ++itr)
	{
		if((*itr).id_ == id)
		{
			itemlist_.erase(itr);
			break;
		}
	}
}

void Gossip::Menu::AddQuest(Quest* quest, uint8 icon)
{
	this->questlist_.insert(make_pair(quest, icon));
}
void Gossip::Menu::RemoveQuest(Quest* quest)
{
	Gossip::QuestList::iterator itr = questlist_.find(quest);
	if(itr != questlist_.end())
		questlist_.erase(itr);
}

WorldPacket & Gossip::operator<<(WorldPacket & packet, const Gossip::Menu & menu)
{
	packet << menu.guid_;
	packet << uint32(0);
	packet << menu.textid_;
	packet << uint32(menu.itemlist_.size());
	{
		for(Gossip::ItemList::const_iterator itr = menu.itemlist_.begin(); itr != menu.itemlist_.end(); ++itr)
			packet << *itr;
	}
	packet << uint32(menu.questlist_.size());
	{
		for(Gossip::QuestList::const_iterator itr = menu.questlist_.begin(); itr != menu.questlist_.end(); ++itr)
		{
			packet << itr->first->id << uint32(itr->second) << itr->first->min_level << itr->first->quest_flags << uint8(0);
			LocalizedQuest* lq = sLocalizationMgr.GetLocalizedQuest(itr->first->id, menu.language_);
			if(lq != NULL)
				packet << lq->Title;
			else
				packet << itr->first->title;
		}
	}
	return packet;
}

template<uint32 size>
StackBuffer<size>& Gossip::operator<<(StackBuffer<size> & packet, const Gossip::Menu & menu)
{
	packet << menu.guid_;
	packet << uint32(0);
	packet << menu.textid_;
	packet << uint32(menu.itemlist_.size());
	{
		for(Gossip::ItemList::const_iterator itr = menu.itemlist_.begin(); itr != menu.itemlist_.end(); ++itr)
			packet << *itr;
	}
	packet << uint32(menu.questlist_.size());
	{
		string title;
		for(Gossip::QuestList::const_iterator itr = menu.questlist_.begin(); itr != menu.questlist_.end(); ++itr)
		{
			packet << itr->first->id << uint32(itr->second) << itr->first->min_level << itr->first->quest_flags << uint8(0);
			LocalizedQuest* lq = sLocalizationMgr.GetLocalizedQuest(itr->first->id, menu.language_);
			if(lq != NULL)
				title = lq->Title;
			else
				title = itr->first->title;
			packet << title;
		}
	}
	return packet;
}

void Gossip::Menu::BuildPacket(WorldPacket & packet) const
{
	packet << *this;
}
void Gossip::Menu::BuildPacket(WorldPacket* packet) const
{
	*packet << *this;
}

void Gossip::Menu::Send(Player* plr) const
{
	WorldPacket packet(SMSG_GOSSIP_MESSAGE, 512);
	BuildPacket(packet);
	plr->GetSession()->SendPacket(&packet);
}

template<uint32 size>
void Gossip::Menu::StackSend(Player* plr) const
{
	StackWorldPacket<size> packet(SMSG_GOSSIP_MESSAGE);
	packet << static_cast<Gossip::Menu>(*this);
	plr->GetSession()->SendPacket(&packet);
}

void Gossip::Menu::SendSimpleMenu(uint64 guid, size_t txt_id, Player* plr)
{
	StackWorldPacket<32> packet(SMSG_GOSSIP_MESSAGE);
	packet << guid << uint32(0) << uint32(txt_id) << uint32(0) << uint32(0);
	plr->GetSession()->SendPacket(&packet);
}

void Gossip::Menu::SendQuickMenu(uint64 guid, size_t textid, Player* Plr, size_t itemid, uint8 itemicon, const char* itemtext, size_t requiredmoney/*=0*/, const char* moneytext/*=NULL*/, uint8 extra/*=0*/)
{
	StackWorldPacket<64> packet(SMSG_GOSSIP_MESSAGE);
	string itemtexts = (itemtext != NULL) ? itemtext : "";
	string moneytexts = (moneytext != NULL) ? moneytext : "";
	packet << guid << uint32(0) << uint32(textid) << uint32(1) << uint32(itemid) << itemicon << extra << uint32(requiredmoney) << itemtexts;
	if(moneytext != NULL)
		packet << moneytexts;
	else
		packet << uint8(0);
	packet << uint32(0);
	Plr->GetSession()->SendPacket(&packet);
}

void Gossip::Menu::Complete(Player* plr)
{
	plr->GetSession()->OutPacket(SMSG_GOSSIP_COMPLETE, 0, NULL);
}


/*
English Worldstrings as of 08.16.2009

entry	text
1 	I would like to browse your goods.
2 	I seek
3 	mage
4 	shaman
5 	warrior
6 	paladin
7 	warlock
8 	hunter
9 	rogue
10 	druid
11 	priest
12 	training
13 	Train me in the ways of the beast.
14 	Give me a ride.
15 	I would like to make a bid.
16 	Make this inn your home.
17 	I would like to check my deposit box.
18 	Bring me back to life.
19 	How do I create a guild/arena team?
20 	I want to create a guild crest.
21 	I would like to go to the battleground.
22 	I would like to reset my talents.
23 	I wish to untrain my pet.
24 	I understand, continue.
25 	Yes, please do.
26 	This instance is unavailable.
27 	You must have The Burning Crusade Expansion to access this content.
28 	Heroic mode unavailable for this instance.
29 	You must be in a raid group to pass through here.
30 	You do not have the required attunement to pass through here.
31 	You must be at least level %u to pass through here.
32 	You must be in a party to pass through here.
33 	You must be level 70 to enter heroic mode.
34 	-
35 	You must have the item, `%s` to pass through here.
36 	You must have the item, UNKNOWN to pass through here.
37 	What can I teach you, $N?
38 	Alterac Valley
39 	Warsong Gulch
40 	Arathi Basin
41 	Arena 2v2
42 	Arena 3v3
43 	Arena 5v5
44 	Eye of the Storm
45 	Unknown Battleground
46 	One minute until the battle for %s begins!
47 	Thirty seconds until the battle for %s begins!
48 	Fifteen seconds until the battle for %s begins!
49 	The battle for %s has begun!
50 	Arena
51 	You have tried to join an invalid instance id.
52 	Your queue on battleground instance id %u is no longer valid. Reason: Instance Deleted.
53 	You cannot join this battleground as it has already ended.
54 	Your queue on battleground instance %u is no longer valid, the instance no longer exists.
55 	Sorry, raid groups joining battlegrounds are currently unsupported.
56 	You must be the party leader to add a group to an arena.
57 	You must be in a team to join rated arena.
58 	You have too many players in your party to join this type of arena.
59 	Sorry, some of your party members are not level 70.
60 	One or more of your party members are already queued or inside a battleground.
61 	One or more of your party members are not members of your team.
62 	Welcome to
63 	Horde
64 	Alliance
65 	[ |cff00ccffAttention|r ] Welcome! A new challenger (|cff00ff00{%d}|r - |cffff0000%s|r) has arrived and joined into |cffff0000%s|r,their force has already been increased.
66 	This instance is scheduled to reset on
67 	Auto loot passing is now %s
68 	On
69 	Off
70 	Hey there, $N. How can I help you?
71 	You are already in an arena team.
72 	That name is already in use.
73 	You already have an arena charter.
74 	A guild with that name already exists.
75 	You already have a guild charter.
76 	Item not found.
77 	Target is of the wrong faction.
78 	Target player cannot sign your charter for one or more reasons.
79 	You have already signed that charter.
80 	You don't have the required amount of signatures to turn in this petition.
81 	You must have Wrath of the Lich King Expansion to access this content.
82  Deathknight
*/

void Gossip::Script::Destroy()
{
	delete this;
}

Gossip::Script* Gossip::Script::GetInterface(Creature* creature)
{
	Gossip::Script* script = sScriptMgr.get_creature_gossip(creature->GetEntry());
	if(script != NULL)
		return script;

	if(creature->isSpiritHealer())
		return &sScriptMgr.spirithealerScript_;
	else if(creature->isInnkeeper())
		return &sScriptMgr.innkeeperScript_;
	else if(creature->isBanker())
		return &sScriptMgr.bankerScript_;
	else if(creature->isClass())
		return &sScriptMgr.classtrainerScript_;
	else if(creature->isTrainer())
	{
		::Trainer* traininfo = creature->GetTrainer();

		if(traininfo != NULL)	//Seems to happen.
		{
			if(traininfo->TrainerType == TRAINER_TYPE_PET)
				return &sScriptMgr.pettrainerScript_;
			else
				return &sScriptMgr.trainerScript_;
		}
	}
	else if(creature->isTabardDesigner())
		return &sScriptMgr.tabardScript_;
	else if(creature->isTaxi())
		return &sScriptMgr.flightmasterScript_;
	else if(creature->isStableMaster())
		return &sScriptMgr.stablemasterScript_;
	else if(creature->isBattleMaster())
		return &sScriptMgr.battlemasterScript_;
	else if(creature->isAuctioner())
		return &sScriptMgr.auctioneerScript_;
	else if(creature->isCharterGiver())
		return &sScriptMgr.chartergiverScript_;
	else if(creature->isVendor())
		return &sScriptMgr.vendorScript_;

	return &sScriptMgr.genericScript_;
}
Gossip::Script* Gossip::Script::GetInterface(::Item* item)
{
	return sScriptMgr.get_item_gossip(item->GetEntry());
}
Gossip::Script* Gossip::Script::GetInterface(GameObject* go)
{
	return sScriptMgr.get_go_gossip(go->GetEntry());
}

/*
	SPIRIT HEALER
	*/
void Arcemu::Gossip::SpiritHealer::OnHello(Object* pObject, Player* Plr)
{
	Plr->GetSession()->SendSpiritHealerRequest(TO_CREATURE(pObject));
}

/*
	VENDORS
	*/
void Arcemu::Gossip::Vendor::OnHello(Object* pObject, Player* Plr)
{
	Creature* creature = TO_CREATURE(pObject);
	uint32 Text = objmgr.GetGossipTextForNpc(creature->GetEntry());
	if(NpcTextStorage.LookupEntry(Text) == NULL)
		Text = Gossip::DEFAULT_TXTINDEX;

	VendorRestrictionEntry* vendor = VendorRestrictionEntryStorage.LookupEntry(creature->GetProto()->Id);

	Gossip::Menu menu(creature->GetGUID(), Text, Plr->GetSession()->language);

	if(!Plr->CanBuyAt(vendor))
		menu.setTextID(vendor->cannotbuyattextid);
	else
		menu.AddItem(Gossip::ICON_VENDOR, Plr->GetSession()->LocalizedWorldSrv(Gossip::VENDOR), 1, false);

	sQuestMgr.FillQuestMenu(creature, Plr, menu); //add any quests we have.

	menu.StackSend<256>(Plr);
}

void Arcemu::Gossip::Vendor::OnSelectOption(Object* pObject, Player* Plr, uint32 Id, const char* EnteredCode)
{
	Plr->GetSession()->SendInventoryList(TO_CREATURE(pObject));
}

/*
	TRAINER
	*/

void Arcemu::Gossip::Trainer::OnHello(Object* pObject, Player* Plr)
{
	Creature* trainer = TO_CREATURE(pObject);
	::Trainer* trainerinfo = trainer->GetTrainer();
	uint32 Text = objmgr.GetGossipTextForNpc(trainer->GetEntry());
	if(NpcTextStorage.LookupEntry(Text) == NULL)
		Text = Gossip::DEFAULT_TXTINDEX;

	string name = trainer->GetCreatureInfo()->Name;
	string::size_type pos = name.find(" ");	  // only take first name

	if(pos != string::npos)
		name = name.substr(0, pos);

	Gossip::Menu menu(trainer->GetGUID(), Text, Plr->GetSession()->language);
	if(trainerinfo != NULL)
	{
		if(! Plr->CanTrainAt(trainerinfo))
			menu.setTextID(trainerinfo->Cannot_Train_GossipTextId);
		else
		{
			// I seek
			string msg = string(Plr->GetSession()->LocalizedWorldSrv(Gossip::ISEEK));
			msg += string(Plr->GetSession()->LocalizedWorldSrv(Gossip::TRAINING)) + ", " + name + ".";
			menu.AddItem(Gossip::ICON_TRAINER, msg.c_str(), 1);

			if(trainer->isVendor())
			{
				VendorRestrictionEntry* vendor = VendorRestrictionEntryStorage.LookupEntry(trainer->GetProto()->Id);
				if(Plr->CanBuyAt(vendor))
					menu.AddItem(Gossip::ICON_VENDOR, Plr->GetSession()->LocalizedWorldSrv(Gossip::VENDOR), 2);
			}
		}
	}
	sQuestMgr.FillQuestMenu(trainer, Plr, menu);
	menu.StackSend<256>(Plr);
}

void Arcemu::Gossip::Trainer::OnSelectOption(Object* pObject, Player* Plr, uint32 Id, const char* EnteredCode)
{
	if(1 == Id)
		Plr->GetSession()->SendTrainerList(TO_CREATURE(pObject));
	else
		Plr->GetSession()->SendInventoryList(TO_CREATURE(pObject));
}

/*
	TAXIMASTER
	*/

void Arcemu::Gossip::FlightMaster::OnHello(Object* pObject, Player* Plr)
{
	Creature* flightmaster = TO_CREATURE(pObject);
	uint32 Text = objmgr.GetGossipTextForNpc(flightmaster->GetEntry());
	if(NpcTextStorage.LookupEntry(Text) == NULL)
		Text = Gossip::DEFAULT_TXTINDEX;

	Gossip::Menu menu(pObject->GetGUID(), Text, Plr->GetSession()->language);
	menu.AddItem(Gossip::ICON_FLIGHTMASTER, Plr->GetSession()->LocalizedWorldSrv(Gossip::FLIGHTMASTER), 1);
	sQuestMgr.FillQuestMenu(flightmaster, Plr, menu);

	menu.StackSend<256>(Plr);
}

void Arcemu::Gossip::FlightMaster::OnSelectOption(Object* pObject, Player* Plr, uint32 Id, const char* EnteredCode)
{
	Plr->GetSession()->SendTaxiList(TO_CREATURE(pObject));
}

/*
	AUCTIONEER
	*/
void Arcemu::Gossip::Auctioneer::OnHello(Object* pObject, Player* Plr)
{
	Creature* auctioneer = TO_CREATURE(pObject);
	uint32 Text = objmgr.GetGossipTextForNpc(auctioneer->GetEntry());
	if(NpcTextStorage.LookupEntry(Text) == NULL)
		Text = Gossip::DEFAULT_TXTINDEX;
	//auctioneers don't offer quests.
	Gossip::Menu::SendQuickMenu(pObject->GetGUID(), Text, Plr, 1, Gossip::ICON_VENDOR, Plr->GetSession()->LocalizedWorldSrv(Gossip::AUCTIONEER));
}

void Arcemu::Gossip::Auctioneer::OnSelectOption(Object* pObject, Player* Plr, uint32 Id, const char* EnteredCode)
{
	Plr->GetSession()->SendAuctionList(TO_CREATURE(pObject));
}

/*
	INN KEEPERS
	*/
void Arcemu::Gossip::InnKeeper::OnHello(Object* pObject, Player* Plr)
{
	Creature* innkeeper = TO_CREATURE(pObject);
	uint32 Text = objmgr.GetGossipTextForNpc(innkeeper->GetEntry());
	if(NpcTextStorage.LookupEntry(Text) == NULL)
		Text = Gossip::DEFAULT_TXTINDEX;
	Gossip::Menu menu(pObject->GetGUID(), Text, Plr->GetSession()->language);
	menu.AddItem(Gossip::ICON_CHAT, Plr->GetSession()->LocalizedWorldSrv(Gossip::INNKEEPER), 1);
	//inn keepers can sell stuff
	if(innkeeper->isVendor())
	{
		VendorRestrictionEntry* vendor = VendorRestrictionEntryStorage.LookupEntry(innkeeper->GetProto()->Id);
		if(Plr->CanBuyAt(vendor))
			menu.AddItem(Gossip::ICON_VENDOR, Plr->GetSession()->LocalizedWorldSrv(Gossip::VENDOR), 2);
	}
	sQuestMgr.FillQuestMenu(innkeeper, Plr, menu);
	menu.StackSend<256>(Plr);
}

void Arcemu::Gossip::InnKeeper::OnSelectOption(Object* pObject, Player* Plr, uint32 Id, const char* EnteredCode)
{
	if(1 == Id)
		Plr->GetSession()->SendInnkeeperBind(TO_CREATURE(pObject));
	else
		Plr->GetSession()->SendInventoryList(TO_CREATURE(pObject));
}

/*
	BATTLE MASTER
	*/

void Arcemu::Gossip::BattleMaster::OnHello(Object* pObject, Player* Plr)
{
	Creature* battlemaster = TO_CREATURE(pObject);
	uint32 Text = objmgr.GetGossipTextForNpc(battlemaster->GetEntry());
	if(NpcTextStorage.LookupEntry(Text) == NULL)
		Text = Gossip::DEFAULT_TXTINDEX;
	Gossip::Menu menu(battlemaster->GetGUID(), Text, Plr->GetSession()->language);
	menu.AddItem(Gossip::ICON_BATTLE, Plr->GetSession()->LocalizedWorldSrv(Gossip::BATTLEMASTER), 1);
	sQuestMgr.FillQuestMenu(battlemaster, Plr, menu);
	menu.StackSend<256>(Plr);
}

void Arcemu::Gossip::BattleMaster::OnSelectOption(Object* pObject, Player* Plr, uint32 Id, const char* EnteredCode)
{
	Plr->GetSession()->SendBattlegroundList(TO_CREATURE(pObject), 0);
}

/*
	BANKER
	*/

void Arcemu::Gossip::Banker::OnHello(Object* pObject, Player* Plr)
{
	Plr->GetSession()->SendBankerList(TO_CREATURE(pObject));
}

void Arcemu::Gossip::Banker::OnSelectOption(Object* pObject, Player* Plr, uint32 Id, const char* EnteredCode)
{

}

/*
	CHARTER GIVER
	*/

void Arcemu::Gossip::CharterGiver::OnHello(Object* pObject, Player* Plr)
{
	Creature* chartergiver = TO_CREATURE(pObject);
	uint32 Text = objmgr.GetGossipTextForNpc(chartergiver->GetEntry());
	if(NpcTextStorage.LookupEntry(Text) == NULL)
		Text = Gossip::DEFAULT_TXTINDEX;
	if(chartergiver->isTabardDesigner())
		Gossip::Menu::SendQuickMenu(pObject->GetGUID(), Text, Plr, 1, Gossip::ICON_CHAT, "How do I create a guild?");
	else
		Gossip::Menu::SendQuickMenu(pObject->GetGUID(), Text, Plr, 1, Gossip::ICON_CHAT, "How do I create a arena team?");
}

void Arcemu::Gossip::CharterGiver::OnSelectOption(Object* pObject, Player* Plr, uint32 Id, const char* EnteredCode)
{
	Plr->GetSession()->SendCharterRequest(TO_CREATURE(pObject));
}

/*
	TABARD DESIGNER
	*/

void Arcemu::Gossip::TabardDesigner::OnHello(Object* pObject, Player* Plr)
{
	Creature* chartergiver = TO_CREATURE(pObject);
	uint32 Text = objmgr.GetGossipTextForNpc(chartergiver->GetEntry());
	if(NpcTextStorage.LookupEntry(Text) == NULL)
		Text = Gossip::DEFAULT_TXTINDEX;
	
	Gossip::Menu menu(chartergiver->GetGUID(), Text, Plr->GetSession()->language);
	menu.AddItem( Gossip::ICON_TABARD, Plr->GetSession()->LocalizedWorldSrv(Gossip::TABARD), 1 );
	if( chartergiver->isCharterGiver() )
		menu.AddItem( Gossip::ICON_CHAT, "How do I create a guild?", 2 );
	
	if(chartergiver->isVendor())
	{
		VendorRestrictionEntry* vendor = VendorRestrictionEntryStorage.LookupEntry(chartergiver->GetProto()->Id);
		if(Plr->CanBuyAt(vendor))
			menu.AddItem(Gossip::ICON_VENDOR, Plr->GetSession()->LocalizedWorldSrv(Gossip::VENDOR), 3);
	}
	menu.Send(Plr);
}

void Arcemu::Gossip::TabardDesigner::OnSelectOption(Object* pObject, Player* Plr, uint32 Id, const char* EnteredCode)
{
	switch( Id ){
		case 1:
			Plr->GetSession()->SendTabardHelp(TO_CREATURE(pObject));
			break;
		case 2:
			if( TO_CREATURE( pObject )->isCharterGiver() )
				Plr->GetSession()->SendCharterRequest(TO_CREATURE(pObject));
			break;
		case 3:
			Plr->GetSession()->SendInventoryList(TO_CREATURE(pObject));
			break;
	}
}

/*
	STABLED MASTER
	*/
void Arcemu::Gossip::StableMaster::OnHello(Object* pObject, Player* Plr)
{
	Creature* stablemaster = TO_CREATURE(pObject);
	uint32 Text = objmgr.GetGossipTextForNpc(stablemaster->GetEntry());
	if(NpcTextStorage.LookupEntry(Text) == NULL)
		Text = Gossip::DEFAULT_TXTINDEX;
	if(Plr->getClass() == ::HUNTER)
		Gossip::Menu::SendQuickMenu(pObject->GetGUID(), Text, Plr, 1, Gossip::ICON_CHAT, "I'd like to stable my pet here.");
	else
		Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), Text, Plr);
}

void Arcemu::Gossip::StableMaster::OnSelectOption(Object* pObject, Player* Plr, uint32 Id, const char* EnteredCode)
{
	Plr->GetSession()->SendStabledPetList(pObject->GetGUID());
}


/*
	PET TRAINER
	*/
void Arcemu::Gossip::PetTrainer::OnHello(Object* pObject, Player* Plr)
{
	Creature* petrain = TO_CREATURE(pObject);
	uint32 Text = objmgr.GetGossipTextForNpc(petrain->GetEntry());
	if(NpcTextStorage.LookupEntry(Text) == NULL)
		Text = Gossip::DEFAULT_TXTINDEX;

	Gossip::Menu menu(pObject->GetGUID(), Text, Plr->GetSession()->language);
	menu.AddItem(Gossip::ICON_TRAINER, Plr->GetSession()->LocalizedWorldSrv(Gossip::BEASTTRAINING), 1);
	if(Plr->getClass() == ::HUNTER && Plr->GetSummon() != NULL)
		menu.AddItem(Gossip::ICON_CHAT, Plr->GetSession()->LocalizedWorldSrv(Gossip::PETTRAINER_TALENTRESET), 2);
	sQuestMgr.FillQuestMenu(petrain, Plr, menu);

	menu.StackSend<256>(Plr);
}

void Arcemu::Gossip::PetTrainer::OnSelectOption(Object* pObject, Player* Plr, uint32 Id, const char* EnteredCode)
{
	if(1 == Id)
		Plr->GetSession()->SendTrainerList(TO_CREATURE(pObject));
	else if(2 == Id)
		Gossip::Menu::SendQuickMenu(pObject->GetGUID(), TXTID_PETUNTRAIN, Plr, 3, Gossip::ICON_CHAT, Plr->GetSession()->LocalizedWorldSrv(Gossip::PETTRAINER_TALENTRESET));
	else
	{
		Gossip::Menu::Complete(Plr);
		Plr->SendPetUntrainConfirm();
	}

}

/*
	CLASS TRAINER
	*/
void Arcemu::Gossip::ClassTrainer::OnHello(Object* pObject, Player* Plr)
{
	Creature* trainer = TO_CREATURE(pObject);
	uint32 Text = objmgr.GetGossipTextForNpc(trainer->GetEntry());
	if(NpcTextStorage.LookupEntry(Text) == NULL)
		Text = Gossip::DEFAULT_TXTINDEX;

	Gossip::Menu menu(pObject->GetGUID(), Text, Plr->GetSession()->language);
	uint8 playerclass = Plr->getClass();
	::Trainer* traininfo = trainer->GetTrainer();

	if(traininfo != NULL)	//Seems to happen
	{
		if(traininfo->RequiredClass != playerclass)
			menu.setTextID(traininfo->Cannot_Train_GossipTextId);
		else
		{
			menu.setTextID(traininfo->Can_Train_Gossip_TextId);
			string itemname = Plr->GetSession()->LocalizedWorldSrv(Gossip::ISEEK);
			string name = trainer->GetCreatureInfo()->Name;

			string::size_type pos = name.find(" ");	  // only take first name

			if(pos != string::npos)
				name = name.substr(0, pos);

			switch(playerclass)
			{
				case ::MAGE:
					itemname += string(Plr->GetSession()->LocalizedWorldSrv(Gossip::MAGE));
					break;
				case ::SHAMAN:
					itemname += string(Plr->GetSession()->LocalizedWorldSrv(Gossip::SHAMAN));
					break;
				case ::WARRIOR:
					itemname += string(Plr->GetSession()->LocalizedWorldSrv(Gossip::WARRIOR));
					break;
				case ::PALADIN:
					itemname += string(Plr->GetSession()->LocalizedWorldSrv(Gossip::PALADIN));
					break;
				case ::WARLOCK:
					itemname += string(Plr->GetSession()->LocalizedWorldSrv(Gossip::WARLOCK));
					break;
				case ::HUNTER:
					itemname += string(Plr->GetSession()->LocalizedWorldSrv(Gossip::HUNTER));
					break;
				case ::ROGUE:
					itemname += string(Plr->GetSession()->LocalizedWorldSrv(Gossip::ROGUE));
					break;
				case ::DRUID:
					itemname += string(Plr->GetSession()->LocalizedWorldSrv(Gossip::DRUID));
					break;
				case ::PRIEST:
					itemname += string(Plr->GetSession()->LocalizedWorldSrv(Gossip::PRIEST));
					break;
				case ::DEATHKNIGHT:
					itemname += string(Plr->GetSession()->LocalizedWorldSrv(Gossip::DEATHKNIGHT));
					break;
				default:
					break;
			}
			itemname += " ";
			itemname += string(Plr->GetSession()->LocalizedWorldSrv(Gossip::TRAINING)) + ", " + name + ".";

			menu.AddItem(Gossip::ICON_TRAINER, itemname.c_str(), 1);

			//talent reset option.
			if(trainer->getLevel() > Gossip::TRAINER_TALENTRESET_LVLIMIT && Plr->getLevel() > Gossip::PLAYER_TALENTRESET_LVLIMIT && trainer->GetTrainer()->RequiredClass == playerclass)
			{
				menu.AddItem(Gossip::ICON_CHAT, Plr->GetSession()->LocalizedWorldSrv(Gossip::CLASSTRAINER_TALENTRESET), 2);
				//dual speciliazation option.
				if(Plr->getLevel() >= Gossip::PLAYER_DUALTALENT_LVLIMIT && Plr->m_talentSpecsCount < 2)
					menu.AddItem(Gossip::ICON_CHAT, "Learn about Dual Talent Specialization.", 4);
			}
		}
	}
	sQuestMgr.FillQuestMenu(trainer, Plr, menu);
	menu.StackSend<256>(Plr);
}

void Arcemu::Gossip::ClassTrainer::OnSelectOption(Object* pObject, Player* Plr, uint32 Id, const char* EnteredCode)
{
	const char* purchaseconfirm;
	switch(Id)
	{
		case 1:
			Plr->GetSession()->SendTrainerList(TO_CREATURE(pObject));
			break;
		case 2:
			Gossip::Menu::SendQuickMenu(pObject->GetGUID(), TXTID_TALENTRESET, Plr, 3, Gossip::ICON_CHAT, Plr->GetSession()->LocalizedWorldSrv(Gossip::CLASSTRAINER_TALENTCONFIRM), 3);
			break;
		case 3:
			Gossip::Menu::Complete(Plr);
			Plr->SendTalentResetConfirm();
			break;
		case 4:
			purchaseconfirm = "Are you sure you would like to purchase your second talent specialization?";
			Gossip::Menu::SendQuickMenu(pObject->GetGUID(), TXTID_DUALSPECPURCHASE, Plr, 5, Gossip::ICON_CHAT, "Purchase a Dual Talent Specialization.", 10000000, purchaseconfirm);
			break;
		case 5:
			if(!Plr->HasGold(10000000))
			{
				Gossip::Menu::Complete(Plr);
				Plr->GetSession()->SendNotification("You do not have enough gold to purchase a dual spec."); // I know this is not correct
			}
			else
			{
				Gossip::Menu::Complete(Plr);
				Plr->ModGold(-10000000);
				Plr->m_talentSpecsCount = 2;
				Plr->Reset_Talents();
				Plr->CastSpell(Plr, 63624, true); // Show activate spec buttons
				Plr->CastSpell(Plr, 63706, true); // Allow primary spec to be activated
				Plr->CastSpell(Plr, 63707, true); // Allow secondary spec to be activated
				Plr->SaveToDB(false); // hai gm i bought dual spec but no werk plis gief mi 1000g back - GTFO you never bought anything
			}
	}
}

void Arcemu::Gossip::Generic::OnHello(Object* pObject, Player* Plr)
{
	//Simply send quests.
	uint32 Text = objmgr.GetGossipTextForNpc(pObject->GetEntry());
	if(NpcTextStorage.LookupEntry(Text) == NULL)
		Text = Gossip::DEFAULT_TXTINDEX;
	Gossip::Menu menu(pObject->GetGUID(), Text, Plr->GetSession()->language);
	sQuestMgr.FillQuestMenu(TO_CREATURE(pObject), Plr, menu);
	menu.StackSend<256>(Plr);
}

void Arcemu::Gossip::Generic::OnSelectOption(Object* pObject, Player* Plr, uint32 Id, const char* EnteredCode)
{

}
