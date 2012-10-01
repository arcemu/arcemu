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

#pragma once

struct Quest;
class Creature;
class Item;
namespace Arcemu
{
	namespace Gossip
	{
		using namespace Arcemu;
		const unsigned DEFAULT_TXTINDEX = 2;
		const unsigned TRAINER_TALENTRESET_LVLIMIT = 10; //minimum
		const unsigned PLAYER_TALENTRESET_LVLIMIT = 10; //
		const unsigned PLAYER_DUALTALENT_LVLIMIT = 40;
		enum Icons
		{
		    ICON_CHAT = 0,
		    ICON_VENDOR,
		    ICON_FLIGHTMASTER,
		    ICON_TRAINER, //book
		    ICON_INTERACT_1, //interaction wheel
		    ICON_INTERACT_2, //interaction wheel
		    ICON_MONEY_BAG, //brown bag with yellow dot
		    ICON_TALK, //white chat bubble with black dots
		    ICON_TABARD, //tabard
		    ICON_BATTLE, //two swords
		    ICON_DOT, //yellow dot
		    ICON_CHAT_11, //This and below are most the same visual as GOSSIP_ICON_CHAT
		    ICON_CHAT_12, //but are still used for unknown reasons.
		    ICON_CHAT_13,
		    ICON_CHAT_14, // probably invalid
		    ICON_CHAT_15, // probably invalid
		    ICON_CHAT_16,
		    ICON_CHAT_17,
		    ICON_CHAT_18,
		    ICON_CHAT_19,
		    ICON_CHAT_20,
		    ICON_MAX
		};

		enum ws
		{
		    VENDOR = 1,
		    ISEEK,
		    MAGE,
		    SHAMAN,
		    WARRIOR,
		    PALADIN,
		    WARLOCK,
		    HUNTER,
		    ROGUE,
		    DRUID,
		    PRIEST,
		    TRAINING,
		    BEASTTRAINING,
		    FLIGHTMASTER,
		    AUCTIONEER,
		    INNKEEPER,
		    BANKER,
		    TABARD = 20,
		    BATTLEMASTER,
		    CLASSTRAINER_TALENTRESET,
		    PETTRAINER_TALENTRESET,
		    CLASSTRAINER_TALENTCONFIRM,
		    PETTRAINER_TALENTCONFIRM,
		    DEATHKNIGHT = 82,
		    TXTID_TALENTRESET = 5674,
		    TXTID_PETUNTRAIN = 7722,
		    TXTID_DUALSPECPURCHASE = 14136	//this is not correct.
		};

		class Menu;
		class Item
		{
				uint16 id_;
				bool coded_;
				uint8 icon_;
				uint32 boxmoney_;
				string boxmessage_;
				string text_;
			public:
				Item(size_t, uint8);
				Item(size_t, uint8, const char*, bool = false, size_t = 0, const char* = NULL);

				friend WorldPacket & operator<<(WorldPacket &, const Gossip::Item &);
				template<uint32 size>
				friend StackBuffer<size>& operator<<(StackBuffer<size>&, const Gossip::Item &);

				friend class Menu;
		};

		typedef std::vector<Gossip::Item> ItemList;
		typedef HM_NAMESPACE::hash_map<Quest*, uint8> QuestList;

		class SERVER_DECL Menu
		{
			public:
				Menu(uint64, uint32, uint32 = 0);
				Menu(Object*, uint32, uint32 = 0);

				//************************************
				// Purpose : Adds a menu item.
				// Parameter:	uint8 - the icon
				// Parameter:	const char * - the text of the item.
				// Parameter:	uint32 - the id of the item, limit is 2^16 or 0xFFFF;
				// Parameter:	bool - whether or not to retrieve text input.
				//************************************
				void AddItem(uint8, const char*, uint32, bool = false);

				//************************************
				// Purpose : Adds a menu item.
				// Parameter:	uint8 - the icon
				// Parameter:	const char * - the text of the item.
				// Parameter:	uint32 - the id of the item, limit is 2^16 or 0xFFFF;
				// Parameter:	uint32 - box money
				// Parameter:	const char* - box text
				// Parameter:	bool - whether or not to retrieve text input.
				//************************************
				void AddItem(uint8, const char*, uint32, uint32, const char*, bool = false);

				//************************************
				// Purpose : Removes the item with the specified id.
				// Parameter:	uint32 - the id of the item.
				//************************************
				void RemoveItem(uint32);

				//************************************
				// Purpose : Adds a quest item to the menu
				// Parameter:	Quest * - the quest
				// Parameter:	uint8 - the icon
				//************************************
				void AddQuest(Quest*, uint8);

				//************************************
				// Purpose : Removes a quest.
				// Parameter:	Quest * - quest to remove.
				// Return : void
				//************************************
				void RemoveQuest(Quest*);

				//************************************
				// Purpose : Fills the packet with the menu data.
				// Parameter:	WorldPacket & - the packet to fill
				//************************************
				void BuildPacket(WorldPacket &) const;
				void BuildPacket(WorldPacket*) const;

				template<uint32 size>
				void BuildPacket(StackBuffer<size> &) const;

				uint32 getTextID() const { return textid_; }
				uint32 getLanguage() const { return language_; }

				ARCEMU_INLINE void setTextID(uint32 textid) { textid_ = textid; }
				ARCEMU_INLINE void setLanguage(uint32 language) { language_ = language; }

				//************************************
				// Purpose : Creates an worldpacket SMSG_GOSSIP_MESSAGE packet, fills it and sends it to the specified player.
				// Parameter:	Player * - player to send to.
				// Return : void
				//************************************
				void Send(Player*) const;

				//************************************
				// Purpose : Creates a stackworldpacket SMSG_GOSSIP_MESSAGE, fills it and sends it to the specified player.
				// Parameter:	Player * - player to send to.
				// Return : void
				//************************************
				template<uint32>
				void StackSend(Player*) const;

				//************************************
				// Purpose :	Sends a menu with just the text id and no options.
				// Parameter:	uint64 - the creature guid.
				// Parameter:	size_t - the text id.
				// Parameter:	Player * - the player to send to.
				//************************************
				static void SendSimpleMenu(uint64, size_t, Player*);

				//************************************
				// Purpose : Sends a menu with a given text id and menu item.
				// Parameter : uint32 - creature guid
				// Parameter : size_t - txt id
				// Parameter : Player* - player to send to.
				// Parameter : size_t - item id
				// Parameter : uint8 - icon
				// Parameter : const string * - item text
				// Parameter : size_t - required money
				// Parameter : const string * - money text
				// Paramater : uint8 extra
				//************************************
				static void SendQuickMenu(uint64, size_t, Player*, size_t, uint8, const char*, size_t = 0, const char * = NULL, uint8 = 0);

				//************************************
				// Purpose : Simply sends out an SMSG_GOSSIP_COMPLETE packet.
				// Parameter:	Plr *
				// Return : void
				//************************************
				static void Complete(Player*);

				friend WorldPacket & operator<<(WorldPacket &, const Gossip::Menu &);

				template<uint32 size>
				friend StackBuffer<size>& operator<<(StackBuffer<size>&, const Gossip::Menu &);

			protected:
				uint32 textid_;
				uint32 language_;	//For localized quest texts.
				uint64 guid_;
				Gossip::ItemList itemlist_;	//Contains non quest items.
				Gossip::QuestList questlist_;	//Contains the quests, it's filled up then added to the packet when it comes to send.
		};

		class SERVER_DECL Script
		{
			public:
				Script() {}
				virtual ~Script() {}

				virtual void OnHello(Object* pObject, Player* Plr) = 0;
				virtual void OnSelectOption(Object* pObject, Player* Plr, uint32 Id, const char* EnteredCode) {}
				virtual void OnEnd(Object* pObject, Player* Plr) {}
				virtual void Destroy();

				static Script* GetInterface(Creature*);
				static Script* GetInterface(::Item*);
				static Script* GetInterface(GameObject*);
		};
		//Spirit Healer Dialog.
		class SERVER_DECL SpiritHealer : public Script
		{
			public:
				SpiritHealer() {}
				virtual ~SpiritHealer() {}
				void OnHello(Object* pObject, Player* Plr);
		};

		class SERVER_DECL Vendor : public Script
		{
			public:
				Vendor() {}
				virtual ~Vendor() {}
				void OnHello(Object* pObject, Player* Plr);
				void OnSelectOption(Object* pObject, Player* Plr, uint32 Id, const char* EnteredCode);
		};
		class SERVER_DECL Trainer : public Script
		{
			public:
				Trainer() {}
				virtual ~Trainer() {}
				virtual void OnHello(Object* pObject, Player* Plr);
				virtual void OnSelectOption(Object* pObject, Player* Plr, uint32 Id, const char* EnteredCode);
		};
		class SERVER_DECL ClassTrainer : public Script
		{
			public:
				ClassTrainer() {}
				virtual ~ClassTrainer() {}
				virtual void OnHello(Object* pObject, Player* Plr);
				virtual void OnSelectOption(Object* pObject, Player* Plr, uint32 Id, const char* EnteredCode);
		};
		class SERVER_DECL PetTrainer : public Script
		{
			public:
				PetTrainer() {}
				virtual ~PetTrainer() {}
				virtual void OnHello(Object* pObject, Player* Plr);
				virtual void OnSelectOption(Object* pObject, Player* Plr, uint32 Id, const char* EnteredCode);
		};
		class SERVER_DECL FlightMaster : public Script
		{
			public:
				FlightMaster() {}
				virtual ~FlightMaster() {}
				void OnHello(Object* pObject, Player* Plr);
				void OnSelectOption(Object* pObject, Player* Plr, uint32 Id, const char* EnteredCode);
		};
		class SERVER_DECL Auctioneer: public Script
		{
			public:
				Auctioneer() {}
				virtual ~Auctioneer() {}
				void OnHello(Object* pObject, Player* Plr);
				void OnSelectOption(Object* pObject, Player* Plr, uint32 Id, const char* EnteredCode);
		};
		class SERVER_DECL InnKeeper : public Script
		{
			public:
				InnKeeper() {}
				virtual ~InnKeeper() {}
				void OnHello(Object* pObject, Player* Plr);
				void OnSelectOption(Object* pObject, Player* Plr, uint32 Id, const char* EnteredCode);
		};
		class SERVER_DECL BattleMaster : public Script
		{
			public:
				BattleMaster() {}
				virtual ~BattleMaster() {}
				void OnHello(Object* pObject, Player* Plr);
				void OnSelectOption(Object* pObject, Player* Plr, uint32 Id, const char* EnteredCode);
		};
		class SERVER_DECL Banker : public Script
		{
			public:
				Banker() {}
				virtual ~Banker() {}
				void OnHello(Object* pObject, Player* Plr);
				void OnSelectOption(Object* pObject, Player* Plr, uint32 Id, const char* EnteredCode);
		};
		class SERVER_DECL CharterGiver : public Script
		{
			public:
				CharterGiver() {}
				virtual ~CharterGiver() {}
				void OnHello(Object* pObject, Player* Plr);
				void OnSelectOption(Object* pObject, Player* Plr, uint32 Id, const char* EnteredCode);
		};
		class SERVER_DECL TabardDesigner : public Script
		{
			public:
				TabardDesigner() {}
				virtual ~TabardDesigner() {}
				void OnHello(Object* pObject, Player* Plr);
				void OnSelectOption(Object* pObject, Player* Plr, uint32 Id, const char* EnteredCode);
		};
		class SERVER_DECL StableMaster : public Script
		{
			public:
				StableMaster() {}
				virtual ~StableMaster() {}
				void OnHello(Object* pObject, Player* Plr);
				void OnSelectOption(Object* pObject, Player* Plr, uint32 Id, const char* EnteredCode);
		};
		class SERVER_DECL Generic : public Script
		{
			public:
				Generic() {}
				virtual ~Generic() {}
				void OnHello(Object* pObject, Player* Plr);
				void OnSelectOption(Object* pObject, Player* Plr, uint32 Id, const char* EnteredCode);
		};
	}
}