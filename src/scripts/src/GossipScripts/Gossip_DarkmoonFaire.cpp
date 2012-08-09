/*
===============================
The Darkmoon Faire Project
Author: Nexis
Team: Sun++ (www.sunplusplus.info)
===============================
*/

#include "Setup.h"

// Setup Carnies
#define BARK_SETUP_CARNIES_1    "Faire's a coming!"
#define BARK_SETUP_CARNIES_2    "Can't you see I have work to do here?"
#define BARK_SETUP_CARNIES_3    "Won't be long now until the Darkmoon Faire opens. Come back later and check to see if we're done."
#define BARK_SETUP_CARNIES_4    "That's right friend! The Darkmoon Faire is going to be right here. Just as soon as I unload those huge wagons, put up all the tents, erect the zoo and pour the drinks. Make sure you come back!"

class SetupCarnies_Gossip : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			GossipMenu* Menu;
			int randGossip;
			randGossip = rand() % 4;
			switch(randGossip)
			{
				case 0:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 50000, plr);
					break;

				case 1:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 50001, plr);
					break;

				case 2:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 50002, plr);
					break;

				case 3:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 50003, plr);
					break;
			}

			Menu->SendTo(plr);
		}

		void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char* Code)
		{
			if(!pObject->IsCreature())
				return;

			switch(IntId)
			{
				case 1:
					GossipHello(pObject, plr);
					break;
			}
		}

};

class SetupCarnies_Bark : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(SetupCarnies_Bark);
		SetupCarnies_Bark(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			RegisterAIUpdateEvent(120000); 			// Start initial update after: 2mins
		}

		void AIUpdate()
		{
			int randGossip;
			randGossip = rand() % 4;
			switch(randGossip)
			{
				case 0:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_SETUP_CARNIES_1);
					break;

				case 1:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_SETUP_CARNIES_2);
					break;

				case 2:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_SETUP_CARNIES_3);
					break;

				case 3:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_SETUP_CARNIES_4);
					break;
			}

			int rndTimer;
			rndTimer = rand() % 300 + 120; 			// Generate a random value between: 2-5mins
			rndTimer = rndTimer * 1000; 			// Convert to milliseconds
			ModifyAIUpdateEvent(rndTimer); 			// Modify timer to new random value
		}
};

// Active Carnies
#define BARK_ACTIVE_CARNIES_1    "Having a good time?"
#define BARK_ACTIVE_CARNIES_2    "Sure are a lot of litter bugs around here."
#define BARK_ACTIVE_CARNIES_3    "Don't forget to buy refreshments and souvenirs!"
#define BARK_ACTIVE_CARNIES_4    "The Darkmoon Faire is the greatest show on all of Azeroth!"

class ActiveCarnies_Gossip : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			GossipMenu* Menu;
			int randGossip;
			randGossip = rand() % 4;
			switch(randGossip)
			{
				case 0:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60007, plr);
					break;

				case 1:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60008, plr);
					break;

				case 2:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60009, plr);
					break;

				case 3:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60010, plr);
					break;
			}

			Menu->SendTo(plr);
		}

		void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char* Code)
		{
			if(!pObject->IsCreature())
				return;

			switch(IntId)
			{
				case 1:
					GossipHello(pObject, plr);
					break;
			}
		}

};

class ActiveCarnies_Bark : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(ActiveCarnies_Bark);
		ActiveCarnies_Bark(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			RegisterAIUpdateEvent(120000); 			// Start initial update after: 2mins
		}

		void AIUpdate()
		{
			int randGossip;
			randGossip = rand() % 4;
			switch(randGossip)
			{
				case 0:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_ACTIVE_CARNIES_1);
					break;

				case 1:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_ACTIVE_CARNIES_2);
					break;

				case 2:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_ACTIVE_CARNIES_3);
					break;

				case 3:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_ACTIVE_CARNIES_4);
					break;
			}

			int rndTimer;
			rndTimer = rand() % 180 + 60; 			// Generate a random value between: 1-3mins
			rndTimer = rndTimer * 1000; 			// Convert to milliseconds
			ModifyAIUpdateEvent(rndTimer); 			// Modify timer to new random value
		}
};

// Burth
class Burth_Gossip : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			GossipMenu* Menu;

			int randGossip;
			randGossip = rand() % 8;
			switch(randGossip)
			{
				case 0:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60000, plr);
					break;

				case 1:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60001, plr);
					break;

				case 2:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60002, plr);
					break;

				case 3:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60003, plr);
					break;

				case 4:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60004, plr);
					break;

				case 5:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60005, plr);
					break;

				case 6:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60006, plr);
					break;

				case 7:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60050, plr);
					break;
			}

			Menu->SendTo(plr);
		}

		void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char* Code)
		{
			if(!pObject->IsCreature())
				return;

			switch(IntId)
			{
				case 1:
					GossipHello(pObject, plr);
					break;
			}
		}

};

// Flik
#define BARK_FLIK_1    "It's getting away!"
#define BARK_FLIK_2    "Get back here!"
#define BARK_FLIK_3    "Hey, someone help me catch this thing!"
#define BARK_FLIK_4    "Frogs and leather balls for sale!"

class Flik_Bark : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(Flik_Bark);
		Flik_Bark(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			RegisterAIUpdateEvent(180000); 			// Start initial update after: 3mins
		}

		void AIUpdate()
		{
			int randGossip;
			randGossip = rand() % 4;
			switch(randGossip)
			{
				case 0:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_FLIK_1);
					break;

				case 1:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_FLIK_2);
					break;

				case 2:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_FLIK_3);
					break;

				case 3:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_FLIK_4);
					break;
			}

			int rndTimer;
			rndTimer = rand() % 240 + 120; 			// Generate a random value between: 2-4mins
			rndTimer = rndTimer * 1000; 			// Convert to milliseconds
			ModifyAIUpdateEvent(rndTimer); 			// Modify timer to new random value
		}
};

// Flik's Frog
class FliksFrog_Gossip : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			GossipMenu* Menu;

			int randGossip;
			randGossip = rand() % 2;
			switch(randGossip)
			{
				case 0:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60011, plr);
					break;
				case 1:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60012, plr);
					break;
			}

			Menu->SendTo(plr);
		}

		void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char* Code)
		{
			if(!pObject->IsCreature())
				return;

			switch(IntId)
			{
				case 1:
					GossipHello(pObject, plr);
					break;
			}
		}

};

// Gevas Grimegate
#define BARK_GEVAS_GRIMEGATE_1    "Tickets! Redeem your Darkmoon Faire Prize Tickets here! Wondrous and exotic prizes are waiting for you!"
#define BARK_GEVAS_GRIMEGATE_2    "Five tickets or two-hundred and fifty, it doesn't matter friend, everybody is a winner!"
#define BARK_GEVAS_GRIMEGATE_3    "All it takes is five or more and you're on your way to the most wondrous prizes on all of Azeroth. Everybody is a winner!"
#define BARK_GEVAS_GRIMEGATE_4    "Mysterious prizes await the adventurous. Step right up, step right up!"

class GevasGrimegate_Bark : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(GevasGrimegate_Bark);
		GevasGrimegate_Bark(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			RegisterAIUpdateEvent(60000); 			// Start initial update after: 1mins
		}

		void AIUpdate()
		{
			int randGossip;
			randGossip = rand() % 4;
			switch(randGossip)
			{
				case 0:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_GEVAS_GRIMEGATE_1);
					break;

				case 1:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_GEVAS_GRIMEGATE_2);
					break;

				case 2:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_GEVAS_GRIMEGATE_3);
					break;

				case 3:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_GEVAS_GRIMEGATE_4);
					break;
			}

			int rndTimer;
			rndTimer = rand() % 300 + 180; 			// Generate a random value between: 3-5mins
			rndTimer = rndTimer * 1000; 			// Convert to milliseconds
			ModifyAIUpdateEvent(rndTimer); 			// Modify timer to new random value
		}
};

// Lhara
#define BARK_LHARA_1    "Come and take a look at my wares while you still can. We're here all week, and then we're off to some far off land."
#define BARK_LHARA_2    "Darkmoon Faire Cards? See the Professor here for those. Come see me if you are a dealer in hard to come by antiquities and artifacts."
#define BARK_LHARA_3    "Every item that I have for sale I acquired myself. You have my personal guarantee of authenticity."
#define BARK_LHARA_4    "Have you any idea what I go through to bring these incomparable valuables to sell? Come see them all for yourself!"

class Lhara_Bark : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(Lhara_Bark);
		Lhara_Bark(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			RegisterAIUpdateEvent(90000); 			// Start initial update after: 1.5mins
		}

		void AIUpdate()
		{
			int randGossip;
			randGossip = rand() % 4;
			switch(randGossip)
			{
				case 0:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_LHARA_1);
					break;

				case 1:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_LHARA_2);
					break;

				case 2:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_LHARA_3);
					break;

				case 3:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_LHARA_4);
					break;
			}

			int rndTimer;
			rndTimer = rand() % 360 + 240; 			// Generate a random value between: 4-6mins
			rndTimer = rndTimer * 1000; 			// Convert to milliseconds
			ModifyAIUpdateEvent(rndTimer); 			// Modify timer to new random value
		}
};
#define TELLME 175
// Maxima Blastenheimer
class MaximaBlastenheimer_Gossip : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			GossipMenu* Menu;

			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60048, plr);
			Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(TELLME), 1);

			Menu->SendTo(plr);
		}

		void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char* Code)
		{
			if(!pObject->IsCreature())
				return;

			GossipMenu* Menu;

			switch(IntId)
			{
				case 0:
					GossipHello(pObject, plr);
					break;

				case 1:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60049, plr);
					Menu->SendTo(plr);
					break;
			}
		}

};

// Morja
#define BARK_MORJA_1    "Jubjub? Where are you, Jubjub? Oh no! Where did you go this time!"

class Morja_Bark : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(Morja_Bark);
		Morja_Bark(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			RegisterAIUpdateEvent(240000); 			// Start initial update after: 4mins
		}

		void AIUpdate()
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_MORJA_1);

			int rndTimer;
			rndTimer = rand() % 360 + 240; 			// Generate a random value between: 4-6mins
			rndTimer = rndTimer * 1000; 			// Convert to milliseconds
			ModifyAIUpdateEvent(rndTimer); 			// Modify timer to new random value
		}
};

// Music Doodad
class MusicDoodad : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(MusicDoodad);
		MusicDoodad(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			RegisterAIUpdateEvent(1000); 			// Get that music playing right away!
		}

		void AIUpdate()
		{
			_unit->PlaySoundToSet(8440);			// Play Darkmoon Faire music if player is in range.
		}
};

// Professor Thaddeus Paleo
#define BARK_PROFESSOR_THADDEUS_PALEO_1    "If you wait to come see me, there may be nothing left!"
#define BARK_PROFESSOR_THADDEUS_PALEO_2    "Have you any Darkmoon Faire Cards? Come speak with me to learn more about them if you dare!"
#define BARK_PROFESSOR_THADDEUS_PALEO_3    "Welcome one and all to the greatest show on all of Azeroth... the Darkmoon Faire! Adventure and exitement await, don't be shy! Mysteries and prizes for both the young and the old are here for the taking. And be sure to speak with the professor about any Darkmoon Cards that you have come across during your travels!"
#define BARK_PROFESSOR_THADDEUS_PALEO_4    "Come speak with me if you are a purveyor of the unusual, or if you have any Darkmoon Faire cards."
#define LETMEBROWSE 176
#define TELLMEMORE 177
#define BEASTS 178
#define PORTAL 179
#define ELEMENT 180
#define WARLORD 181
#define FURIE 182
#define LUNA 183
#define BLESS 184
#define STORM 185

class ProfessorThaddeusPaleo_Gossip : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			GossipMenu* Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60016, plr);

			if(pObject->GetUInt32Value(UNIT_NPC_FLAGS) & UNIT_NPC_FLAG_VENDOR)
				Menu->AddItem(1, plr->GetSession()->LocalizedGossipTexts(LETMEBROWSE), 1);
			Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(TELLMEMORE), 2);

			Menu->SendTo(plr);
		}

		void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char* Code)
		{
			GossipMenu* Menu;
			Creature* pCreature = (pObject->IsCreature()) ? (TO_CREATURE(pObject)) : NULL;
			if(!pObject->IsCreature())
				return;

			switch(IntId)
			{
				case 1:
					plr->GetSession()->SendInventoryList(pCreature);
					break;

				case 2:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60017, plr);

					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(BEASTS), 5);
					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(PORTAL), 6);
					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(ELEMENT), 7);
					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(WARLORD), 8);
					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(FURIE), 9);
					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(LUNA), 10);
					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(BLESS), 11);
					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(STORM), 12);

					Menu->SendTo(plr);
					break;

				case 5:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60018, plr);
					Menu->SendTo(plr);
					break;

				case 6:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60019, plr);
					Menu->SendTo(plr);
					break;

				case 7:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60020, plr);
					Menu->SendTo(plr);
					break;

				case 8:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60021, plr);
					Menu->SendTo(plr);
					break;

				case 9:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60022, plr);
					Menu->SendTo(plr);
					break;

				case 10:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60023, plr);
					Menu->SendTo(plr);
					break;

				case 11:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60024, plr);
					Menu->SendTo(plr);
					break;

				case 12:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60025, plr);
					Menu->SendTo(plr);
					break;

			}
		}

};

class ProfessorThaddeusPaleo_Bark : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(ProfessorThaddeusPaleo_Bark);
		ProfessorThaddeusPaleo_Bark(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			RegisterAIUpdateEvent(210000); 			// Start initial update after: 3.5mins
		}

		void AIUpdate()
		{
			int randGossip;
			randGossip = rand() % 4;
			switch(randGossip)
			{
				case 0:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_PROFESSOR_THADDEUS_PALEO_1);
					break;

				case 1:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_PROFESSOR_THADDEUS_PALEO_2);
					break;

				case 2:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_PROFESSOR_THADDEUS_PALEO_3);
					break;

				case 3:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_PROFESSOR_THADDEUS_PALEO_4);
					break;
			}

			int rndTimer;
			rndTimer = rand() % 180 + 300; 			// Generate a random value between: 3-5mins
			rndTimer = rndTimer * 1000; 			// Convert to milliseconds
			ModifyAIUpdateEvent(rndTimer); 			// Modify timer to new random value
		}
};

// Sayge
#define BARK_SAYGE_1    "Fear not! All fortunes can be discerned for those who know the way."
#define BARK_SAYGE_2    "The longer you wait, the less future you have for me to foretell."
#define BARK_SAYGE_3    "Come speak with me, and what once was cloudy shall become crystal clear."
#define BARK_SAYGE_4    "Come to me true believers, and see what the future holds for you!"
#define FUTURE 186
#define IS 187
#define IT 188
#define IC 189
#define IA 190
#define IE 191
#define IEX 192
#define IR 193
#define ICO 194
#define INT 195
#define IQ 196
#define IW 197
#define IWS 198
#define IWC 199
#define IWSM 200
#define IWSML 201
#define IWRS 202
#define IDLOVE 203
class Sayge_Gossip : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			GossipMenu* Menu;
			// Check to see if the player already has a buff from Sayge.
			if(plr->HasAura(23768) || plr->HasAura(23769) || plr->HasAura(23767) || plr->HasAura(23738) || plr->HasAura(23766) || plr->HasAura(23737) || plr->HasAura(23735) || plr->HasAura(23736))
			{
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60034, plr); 			// Player has buff, denied.
			}
			else
			{
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60026, plr);			// Player doesn't have any buff
				Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(FUTURE), 1);
			}

			Menu->SendTo(plr);
		}

		void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char* Code)
		{
			GossipMenu* Menu;
			if(!pObject->IsCreature())
				return;
			Creature* pCreature = TO_CREATURE(pObject);

			switch(IntId)
			{
				case 1:		// Question 1 (Initial question, always the same)
					objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60027, plr);

					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(IS), 10);
					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(IT), 11);
					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(IC), 12);
					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(IA), 13);

					Menu->SendTo(plr);
					break;

				case 10:	// Question 2 (First Answer = 1)
					objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60028, plr);

					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(IE), 14);
					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(IEX), 15);
					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(IR), 16);

					Menu->SendTo(plr);
					break;

				case 11:     // Question 2 (First Answer = 2)
					objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60029, plr);

					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(ICO), 17);
					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(INT), 18);
					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(IQ), 19);

					Menu->SendTo(plr);
					break;

				case 12:     // Question 2 (First Answer = 3)
					objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60030, plr);

					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(IW), 20);
					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(IWS), 21);
					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(IWC), 22);

					Menu->SendTo(plr);
					break;

				case 13:     // Question 2 (First Answer = 4)
					objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60031, plr);

					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(IWSM), 23);
					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(IWSML), 24);
					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(IWRS), 25);

					Menu->SendTo(plr);
					break;

					// Answers 1-#
				case 14:     // Answers: 1-1
					objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60032, plr);

					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(IDLOVE), 30);
					Menu->SendTo(plr);

					pCreature->CastSpell(plr, 23768, true);
					break;

				case 15:     // Answers: 1-2
					objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60032, plr);

					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(IDLOVE), 30);
					Menu->SendTo(plr);

					pCreature->CastSpell(plr, 23769, true);
					break;

				case 16:     // Answers: 1-3
					objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60032, plr);

					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(IDLOVE), 30);
					Menu->SendTo(plr);

					pCreature->CastSpell(plr, 23767, true);
					break;

					// Answers 2-#
				case 17:     // Answers: 2-1
					objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60032, plr);

					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(IDLOVE), 30);
					Menu->SendTo(plr);

					pCreature->CastSpell(plr, 23738, true);
					break;

				case 18:     // Answers: 2-2
					objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60032, plr);

					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(IDLOVE), 30);
					Menu->SendTo(plr);

					pCreature->CastSpell(plr, 23766, true);
					break;

				case 19:     // Answers: 2-3
					objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60032, plr);

					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(IDLOVE), 30);
					Menu->SendTo(plr);

					pCreature->CastSpell(plr, 23769, true);
					break;

					// Answers 3-#
				case 20:     // Answers: 3-1
					objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60032, plr);

					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(IDLOVE), 30);
					Menu->SendTo(plr);

					pCreature->CastSpell(plr, 23737, true);
					break;

				case 21:     // Answers: 3-2
					objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60032, plr);

					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(IDLOVE), 30);
					Menu->SendTo(plr);

					pCreature->CastSpell(plr, 23735, true);
					break;

				case 22:     // Answers: 3-3
					objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60032, plr);

					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(IDLOVE), 30);
					Menu->SendTo(plr);

					pCreature->CastSpell(plr, 23736, true);
					break;

					// Answers 4-#
				case 23:     // Answers: 4-1
					objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60032, plr);

					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(IDLOVE), 30);
					Menu->SendTo(plr);

					pCreature->CastSpell(plr, 23766, true);
					break;

				case 24:     // Answers: 4-2
					objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60032, plr);

					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(IDLOVE), 30);
					Menu->SendTo(plr);

					pCreature->CastSpell(plr, 23738, true);
					break;

				case 25:     // Answers: 4-3
					objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60032, plr);

					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(IDLOVE), 30);
					Menu->SendTo(plr);

					pCreature->CastSpell(plr, 23737, true);
					break;

				case 30:
					objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60033, plr);
					Menu->SendTo(plr);

					// Cast the fortune into the player's inventory - Not working?
					pCreature->CastSpell(plr, 23765, true);

					// TEMP fix for spell not adding item to  player's inventory.
					if(plr->GetItemInterface()->CalculateFreeSlots(ItemPrototypeStorage.LookupEntry(19422)))
					{
						plr->GetItemInterface()->AddItemToFreeSlot(objmgr.CreateItem(19422, plr)); //Darkmoon Faire Fortune
					}
					else
					{
						sChatHandler.SystemMessage(plr->GetSession(), "You do not have enough free slots for your fortune!");
					}
					break;
			}
		}

};

class Sayge_Bark : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(Sayge_Bark);
		Sayge_Bark(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			RegisterAIUpdateEvent(135000); 			// Start initial update after: 2.25mins
		}

		void AIUpdate()
		{
			int randGossip;
			randGossip = rand() % 4;
			switch(randGossip)
			{
				case 0:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_SAYGE_1);
					break;

				case 1:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_SAYGE_2);
					break;

				case 2:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_SAYGE_3);
					break;

				case 3:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_SAYGE_4);
					break;
			}

			int rndTimer;
			rndTimer = rand() % 180 + 300; 			// Generate a random value between: 3-5mins
			rndTimer = rndTimer * 1000; 			// Convert to milliseconds
			ModifyAIUpdateEvent(rndTimer); 			// Modify timer to new random value
		}
};


#define WHATCANI 204
#define WHATARE 205
#define WHATARED 206
#define WHATOTHER 207
#define MORE 210
#define WHATARETHESE 208
#define TELLME 209

// Selina Dourman
class SelinaDourman_Gossip : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			GossipMenu* Menu;

			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60035, plr);
			Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(WHATCANI), 1);
			Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(WHATARE), 2);
			Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(WHATARED), 3);
			Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(WHATOTHER), 4);

			Menu->SendTo(plr);
		}

		void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char* Code)
		{
			GossipMenu* Menu;
			if(!pObject->IsCreature())
				return;
			Creature* pCreature = TO_CREATURE(pObject);

			switch(IntId)

			{
				case 1:
					objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60036, plr);			// What can I purchase?
					Menu->SendTo(plr);
					break;

				case 2:
					objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60037, plr);			// What are Darkmoon Faire Prize Tickets and how do I get them?
					Menu->SendTo(plr);
					break;

				case 3:
					objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60038, plr);			// What are Darkmoon Cards?
					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(MORE), 10);
					Menu->SendTo(plr);
					break;

				case 4:
					objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60040, plr);			// What other things can I do at the faire?
					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(WHATARETHESE), 20);
					Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(TELLME), 21);
					Menu->SendTo(plr);
					break;

				case 10:
					objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60039, plr);			// What are Darkmoon Cards? <more>
					Menu->SendTo(plr);
					break;

				case 20:
					objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60041, plr);			// What are these Tonk Control Consoles?
					Menu->SendTo(plr);
					break;

				case 21:
					objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60042, plr);			// Tell me about the cannon.
					Menu->SendTo(plr);
					break;
			}
		}

};

// Silas Darkmoon
#define BARK_SILAS_DARKMOON_1    "Welcome one and all to the greatest show on all of Azeroth... the Darkmoon Faire! Adventure and excitement await, don't be shy! Mysteries and prizes for both the young and the old are here for the taking. And be sure to speak with the professor about any Darkmoon Cards that you have come across during your travels!"
#define BARK_SILAS_DARKMOON_2    "Come one, come all. Welcome to the Darkmoon Faire! Don't be shy. Step right up to Lhara and buy yourself an exotic artifact from far off lands. If you're one of the lucky few who have found Darkmoon Cards, have a word with Professor Paleo. Hungry? Thirsy? You're in luck! Refreshments are available right here from Sylannia and Stamp. I recommend the Darkmoon Special Reserve to wash down some Red Hot Wings. And if the future is what you seek, then run, don't walk, to speak with Sayge."
#define BARK_SILAS_DARKMOON_3    "We're back and better than ever! It's the Darkmoon Faire friend, and it's your lucky day! Sparing no expense, we've gathered wonders and treats from around the world for your delight. Wheather you're young or old, rich or poor, the Darkmoon Faire has it all! Be sure to turn in your Darkmoon Faire Prize Tickets to Gelvas Grimegate, and choose from several exotic and wonderous prizes!"
#define BARK_SILAS_DARKMOON_4    "Welcome one and all to the Darkmoon Faire, the greatest event in the world! We have it all... delicious food, strong drink, exotic artifacts, fortunes read, amazing prizes and excitement without end! Don't forget to turn in your Darkmoon Faire Prize Tickets to Gelvas Grimegate! All it takes is five or more and you're on your way to the most wonderous prizes on all of Azeroth. Everybody is a winner!"
#define BARK_SILAS_DARKMOON_5    "Everyone enjoying themselves so far? That's great! Welcome to the Darkmoon Faire, the greatest show in all of Azeroth! Make sure you speak with Yebb and his friends here while you're taking in Neblegear's Darkmoon Zoo Bizarre."
#define BARK_SILAS_DARKMOON_6    "Greetings friends, and welcome to the greatest show on Azeroth!$B$BPlease, step right up and take in all we have to offer. Ride the rides and see the sights! Amaze at the wonders that the Darkmoon Faire has uncovered in this vast and mysterious world! We have spared no expense in bringing you excitement that children of all ages will delight in!"
#define SILAS 211
class SilasDarkmoon_Gossip : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			GossipMenu* Menu;

			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60013, plr);
			Menu->AddItem(0, plr->GetSession()->LocalizedGossipTexts(SILAS), 1);

			Menu->SendTo(plr);
		}

		void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char* Code)
		{
			GossipMenu* Menu;
			if(!pObject->IsCreature())
				return;
			Creature* pCreature = TO_CREATURE(pObject);

			switch(IntId)
			{
				case 1:
					objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60014, plr);			// Silas, why is most everything at the fair free? How do you make a profit?
					Menu->SendTo(plr);
					break;
			}
		}

};

class SilasDarkmoon_Bark : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(SilasDarkmoon_Bark);
		SilasDarkmoon_Bark(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			RegisterAIUpdateEvent(180000); 			// Start initial update after: 3mins
		}

		void AIUpdate()
		{
			int randGossip;
			randGossip = rand() % 6;
			switch(randGossip)
			{
				case 0:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_SILAS_DARKMOON_1);
					break;

				case 1:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_SILAS_DARKMOON_2);
					break;

				case 2:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_SILAS_DARKMOON_3);
					break;

				case 3:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_SILAS_DARKMOON_4);
					break;

				case 4:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_SILAS_DARKMOON_5);
					break;

				case 5:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_SILAS_DARKMOON_6);
					break;
			}

			int rndTimer;
			rndTimer = rand() % 300 + 240; 			// Generate a random value between: 3-5mins
			rndTimer = rndTimer * 1000; 			// Convert to milliseconds
			ModifyAIUpdateEvent(rndTimer); 			// Modify timer to new random value
		}
};

// Stamp Thunderhorn
#define BARK_STAMP_THUNDERHORN_1    "Darkmoon Dogs fresh off the grill. Come and get em while they're hot!"
#define BARK_STAMP_THUNDERHORN_2    "Step right up! Get your Red Hot Wings right here!"
#define BARK_STAMP_THUNDERHORN_3    "I recommend the Darkmoon Special Reserve to wash down some Red Hot Wings. And if the future is what you seek, then run, don't walk, to speak with Sayge."
#define BARK_STAMP_THUNDERHORN_4    "For a limited time only, Crunchy Frog available exclusively here at the Darkmoon Faire."
#define BARK_STAMP_THUNDERHORN_5    "You've got money and we've got food. What are you waiting for?!"

class StampThunderhorn_Bark : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(StampThunderhorn_Bark);
		StampThunderhorn_Bark(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			RegisterAIUpdateEvent(180000); 			// Start initial update after: 3mins
		}

		void AIUpdate()
		{
			int randGossip;
			randGossip = rand() % 5;
			switch(randGossip)
			{
				case 0:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_STAMP_THUNDERHORN_1);
					break;

				case 1:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_STAMP_THUNDERHORN_2);
					break;

				case 2:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_STAMP_THUNDERHORN_3);
					break;

				case 3:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_STAMP_THUNDERHORN_4);
					break;

				case 4:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_STAMP_THUNDERHORN_5);
					break;
			}

			int rndTimer;
			rndTimer = rand() % 300 + 180; 			// Generate a random value between: 3-5mins
			rndTimer = rndTimer * 1000; 			// Convert to milliseconds
			ModifyAIUpdateEvent(rndTimer); 			// Modify timer to new random value
		}
};

// Sylannia
#define BARK_SYLANNIA_1    "Huzzah for the tipper! Step right up and get your delicious, cold drinks right here!"
#define BARK_SYLANNIA_2    "Just in... another batch of Darkmoon Special Reserve. Come get yours before they're all gone!"
#define BARK_SYLANNIA_3    "You there! Yes you! You look thirsty. Get over here, I have just what you need!"
#define BARK_SYLANNIA_4    "Cheap beer! Get your Cheap Beer right here!"

class Sylannia_Bark : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(Sylannia_Bark);
		Sylannia_Bark(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			RegisterAIUpdateEvent(120000); 			// Start initial update after: 2mins
		}

		void AIUpdate()
		{
			int randGossip;
			randGossip = rand() % 4;
			switch(randGossip)
			{
				case 0:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_SYLANNIA_1);
					break;

				case 1:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_SYLANNIA_2);
					break;

				case 2:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_SYLANNIA_3);
					break;

				case 3:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, BARK_SYLANNIA_4);
					break;
			}

			int rndTimer;
			rndTimer = rand() % 360 + 180; 			// Generate a random value between: 3-6mins
			rndTimer = rndTimer * 1000; 			// Convert to milliseconds
			ModifyAIUpdateEvent(rndTimer); 			// Modify timer to new random value
		}
};

void SetupDarkmoonFaireGossip(ScriptMgr* mgr)
{
	GossipScript* ActiveCarniesGossip = new ActiveCarnies_Gossip;
	//GossipScript * SetupCarniesGossip = new SetupCarnies_Gossip;
	GossipScript* BurthGossip = new Burth_Gossip;
	GossipScript* FliksFrogGossip = new FliksFrog_Gossip;
	GossipScript* MaximaBlastenheimerGossip = new MaximaBlastenheimer_Gossip;
	GossipScript* ProfessorThaddeusPaleoGossip = new ProfessorThaddeusPaleo_Gossip;
	GossipScript* SaygeGossip = new Sayge_Gossip;
	GossipScript* SelinaDourmanGossip = new SelinaDourman_Gossip;
	GossipScript* SilasDarkmoonGossip = new SilasDarkmoon_Gossip;

	mgr->register_gossip_script(14849, ActiveCarniesGossip); 						// Active Carine Gossip
	//mgr->register_gossip_script(14849, SetupCarniesGossip); 						// Setup Carine Gossip
	mgr->register_gossip_script(14827, BurthGossip); 								// Burth Gossip
	mgr->register_gossip_script(14866, FliksFrogGossip); 							// Flik's Frog Gossip
	mgr->register_gossip_script(15303, MaximaBlastenheimerGossip); 					// Maxima Blastenheimer Gossip
	mgr->register_gossip_script(14847, ProfessorThaddeusPaleoGossip); 				// Professor Thaddeus Paleo Gossip
	mgr->register_gossip_script(14822, SaygeGossip); 								// Sayge Gossip
	mgr->register_gossip_script(10445, SelinaDourmanGossip); 						// Selina Dourman
	mgr->register_gossip_script(14823, SilasDarkmoonGossip); 						// Silas Darkmoon
}

void SetupDarkmoonFaireBarker(ScriptMgr* mgr)
{
	mgr->register_creature_script(14849, &ActiveCarnies_Bark::Create); 				// Active Carines
	//mgr->register_creature_script(14849, &SetupCarnies_Bark::Create); 				// Setup Carine
	mgr->register_creature_script(14860, &Flik_Bark::Create); 						// Flik
	mgr->register_creature_script(14828, &GevasGrimegate_Bark::Create); 			// Gevas Grimegate
	mgr->register_creature_script(14846, &Lhara_Bark::Create); 						// Lhara
	mgr->register_creature_script(14871, &Morja_Bark::Create); 						// Morja
	mgr->register_creature_script(500000, &MusicDoodad::Create); 					// Darkmoon Faire Music Doodad
	mgr->register_creature_script(14847, &ProfessorThaddeusPaleo_Bark::Create); 	// Professor Thaddeus Paleo
	mgr->register_creature_script(14822, &Sayge_Bark::Create); 						// Sayge
	mgr->register_creature_script(14823, &SilasDarkmoon_Bark::Create); 				// Silas Darkmoon
	mgr->register_creature_script(14845, &StampThunderhorn_Bark::Create); 			// StampThunderhorn
	mgr->register_creature_script(14844, &Sylannia_Bark::Create); 					// Sylannia
}