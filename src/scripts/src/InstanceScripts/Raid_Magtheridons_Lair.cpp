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

/************************************************************************/
/* Raid_MagtheridonsLair.cpp Script										*/
/************************************************************************/

/*
 *	TO DO:	Fire Magtheridon throws around the battlefield.
 *			Some banish/channeling spells must be replaced.				DONE
 *			Cave-in - not fully supported yet, but gonna work on it :)	DONE
 *			Yells - not all added. Some used in bad situations.			DONE
 *			Reset system - not added yet.								DONE
 */

// Encounter Settings
#define BANISH_TIMER			120		// 2 minutes
#define YELL_TIMER				260		// 4 minutes and 20 seconds
#define ACTIVE_CUBES_TO_BANISH	5		// 5 cubes

// Channelers Coords is list of spawn points of all 5 channelers casting spell on Magtheridon
static Location Channelers[]=
{
	{ -55.638000f,   1.869050f, 0.630946f },
	{ -31.861300f, -35.919399f, 0.630945f },
	{  10.469200f, -19.894800f, 0.630910f },
	{  10.477100f,  24.445499f, 0.630891f },
	{ -32.171600f,  39.926800f, 0.630921f }
};

// Columns coords used for Cave In spell to give "collapse" effect
static Location Columns[]=
{
	{  17.7522f,  34.5464f,  0.144816f },
	{  19.0966f, -29.2772f,  0.133036f },
	{ -30.8852f,  46.6723f, -0.497104f },
	{ -60.2491f,  27.9361f, -0.018443f },
	{ -60.8202f, -21.9269f, -0.030260f },
	{ -29.7699f, -43.4445f, -0.522461f }
};

// Cave In Target Triggers coords
static Location CaveInPos[]=
{
	{ -37.183399f, -19.491400f,  0.312451f },
	{ -11.374900f, -29.121401f,  0.312463f },
	{  13.133100f,   2.757930f, -0.312492f },
	{  -3.110930f,  29.142401f, -0.312490f },
	{ -29.691000f,  29.643000f, -0.034676f },
	{ -12.111600f,   1.011050f, -0.303638f }
};

// Cube Triggers coords
static Location CubeTriggers[]=
{
	{ -54.277199f,   2.343740f, 2.404560f },
	{ -31.471001f, -34.155998f, 2.335100f },
	{   8.797220f, -19.480101f, 2.536460f },
	{   9.358900f,  23.228600f, 2.348950f },
	{ -31.891800f,  38.430302f, 2.286470f }
};

// Magtheridon Trigger AI - Creature AI Class
#define CN_MAGTHERIDON_TRIGGER 133338

// Spell macros used in whole script
#define BANISH			30231	// 31797
#define BANISHMENT		40825
#define SOUL_TRANSFER	30531
#define SHADOW_GRASP	30166	//30207
#define SHADOW_GRASP2	30410

class MagtheridonTriggerAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(MagtheridonTriggerAI);
	std::vector<Unit*> ChannelersTable;	// Vector "list" of Channelers
	bool KilledChanneler[5];			// Bool that says if channeler died or not

    MagtheridonTriggerAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// We set default value for KillerdChanneler array to avoid any unexpected situations
		for (int i = 0; i < 5; i++)
		{
			KilledChanneler[i] = false;
		}
		// Variable initialization
		YellTimer = YELL_TIMER;
		EventStarted = false;
		PhaseOneTimer = 0;
		Phase = 0;
		// Trigger settings
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		RegisterAIUpdateEvent(1000);
		// We set Gate (if exists) as opened
		GameObject*  Gate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-72.5866f, 1.559f, 0.0f, 183847);
		if (Gate)
			Gate->SetState(0);
	}

	void AIUpdate()
    {
		// Magtheridon yells when raid progresses, but during fight with himself
		YellTimer--;
		if (Phase <= 1 && !YellTimer)
		{
			// If Magtheridon can be found we let him yell one of six different texts
			Unit* Magtheridon = NULL;
			Magtheridon = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-22.657900f, 2.159050f, -0.345542f, 17257);
			if (Magtheridon)
			{
				uint32 SoundID = 0;
				std::string Text = "";
				
				switch (RandomUInt(6))
				{
				case 1:
					Text = "Wretched, meddling insects! Release me, and perhaps I will grant you a merciful death!";
					SoundID = 10247;
					break;
				case 2:
					Text = "Vermin! Leeches! Take my blood and choke on it!";
					SoundID = 10248;
					break;
				case 3:
					Text = "Illidan is an arrogant fool! I will crush him and reclaim Outland as my own!";
					SoundID = 10249;
					break;
				case 4:
					Text = "Away, you mindless parasites! My blood is my own!";
					SoundID = 10250;
					break;
				case 5:
					Text = "How long do you believe your pathetic sorcery can hold me?";
					SoundID = 10251;
					break;
				case 6:
					Text = "My blood will be the end of you!";
					SoundID = 10252;
					break;
				default:
					Text = "My blood will be the end of you!";
					SoundID = 10252;
				}

				if (SoundID && Text != "")
				{
					Magtheridon->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, Text.c_str());
					Magtheridon->PlaySoundToSet(SoundID);
				}
			}
			// We reset YellTimer to default value to let Pit Lord say something again and again
			YellTimer = YELL_TIMER;
		}
		// If our channeler "list" has unexpected size we try to recreate it
		if (ChannelersTable.size() < 5 && !EventStarted)
		{
			// We clear old "list"
			ChannelersTable.clear();
			// In order to recreate channeler "list" we need ot look for them in hardcoded spawn positions
			Unit* Channeler;
			for (int i = 0; i < 5; i++)
			{
				Channeler = NULL;
				Channeler = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(Channelers[i].x, Channelers[i].y, Channelers[i].z, 17256);
				if (!Channeler)
					continue;
				// If Channeler was found we push him at the end of our "list"
				ChannelersTable.push_back(Channeler);
				// If Magtheridon is spawned we tell channeler to cast spell on Pit Lord
				Unit* Magtheridon = NULL;
				Magtheridon = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-22.657900f, 2.159050f, -0.345542f, 17257);
				if (Magtheridon && Channeler->isAlive() && !Channeler->GetAIInterface()->getNextTarget())
				{
					Channeler->SetChannelSpellTargetGUID(Magtheridon->GetGUID());
					Channeler->SetChannelSpellId(SHADOW_GRASP);
				}
			}
		}
		// If ChannelersTable is not empty we check list to find if none of channelers died
		if (ChannelersTable.size() > 0)
		{
			// We look through list
			size_t Counter = 0;
			for (size_t i = 0; i < ChannelersTable.size(); i++)
			{
				// If channeler was already dead we count him as already dead one and go to others
				if (KilledChanneler[i])
				{
					Counter++;
					continue;
				}

				// Safe check to prevent memory corruptions
				Unit* Channeler = ChannelersTable[i];
				if ( Channeler && !Channeler->IsInWorld() )
				{
					ChannelersTable[i] = NULL;
					Channeler = NULL;
					continue;
				}

				// If channeler wasn't dead before and now is not alive we buff other alive channelers
				if ( Channeler && !Channeler->isAlive() && Channeler->IsInWorld() )
				{
					// We look through list of channelers to find alive ones and buff them
					Unit* BuffedChanneler;
					for (size_t x = 0; x < ChannelersTable.size(); x++)
					{
						// Safe check to prevent memory corruption
						BuffedChanneler = ChannelersTable[x];
						if ( BuffedChanneler && !BuffedChanneler->IsInWorld() )
						{
							ChannelersTable[x] = NULL;
							BuffedChanneler = NULL;
							continue;
						}

						// If channeler is found, alive and is not channeler we checked before if he's dead we move on
						if (BuffedChanneler && BuffedChanneler != Channeler && BuffedChanneler->isAlive())
						{
							// We apply Soul Transfer Aura to channeler who should be buffed
							Aura* aura = new Aura( dbcSpell.LookupEntry(SOUL_TRANSFER), (uint32)-1, BuffedChanneler, BuffedChanneler );
							BuffedChanneler->AddAura(aura);
						}
					}
					// We count channeler which died between last and this trigger as dead and count him as dead one
					KilledChanneler[i] = true;
					Counter++;
				}
			}
			// If only one channeler is alive we can clear list, because we won't need other channelers to be buffed
			/*if (Counter >= ChannelersTable.size() - 1)
				ChannelersTable.clear();*/
		}
		// If table is empty (0 channelers spawned) we remove banish and go to phase 2 at once
		if (!ChannelersTable.size() && !Phase)
		{
			Unit* Magtheridon = NULL;
			Magtheridon = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-22.657900f, 2.159050f, -0.345542f, 17257);
			if (Magtheridon)
			{
				Magtheridon->GetAIInterface()->SetAllowedToEnterCombat(true);
				Magtheridon->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
				Magtheridon->RemoveAura(BANISHMENT);
				Magtheridon->RemoveAura(BANISH);
			}

			Phase = 2;
		}
		// If Event wasn't started yet we check if it shouldn't be marked as started
		if (!EventStarted)
		{
			// We look for channeler that may be In Combat or was killed by that unit enemy
			Unit* Channeler = NULL;
			Unit* UnitTarget = NULL;
			for (size_t i = 0; i < ChannelersTable.size(); i++)
			{
				// Safe check to prevent memory corruptions
				Channeler = ChannelersTable[i];
				if ( Channeler && !Channeler->IsInWorld() )
				{
					ChannelersTable[i] = NULL;
					Channeler = NULL;
					continue;
				}

				// If dead or channeler In Combat is found we check if we have already copied target
				if (Channeler && Channeler->isAlive() && Channeler->GetAIInterface()->getNextTarget())
				{
					// If channeler is In Combat and we haven't copied any target yet we copy it
					if (Channeler->GetAIInterface()->getNextTarget() && !UnitTarget)
					{
						UnitTarget = Channeler->GetAIInterface()->getNextTarget();
					}
					// We switch phase and mark event as started
					EventStarted = true;
					Phase = 1;
				}
			}
			// Immediately after phase switching we check if channelers are In Combat
			if (EventStarted)
			{
				// We look through all channelers if they are In Combat and have targets
				for (size_t i = 0; i < ChannelersTable.size(); i++)
				{
					// Safe check to prevent memory corruption
					Channeler = ChannelersTable[i];
					if ( Channeler && !Channeler->IsInWorld() )
					{
						ChannelersTable[i] = NULL;
						Channeler = NULL;
						continue;
					}

					// If channeler is not In Combat we force him to attack target we copied before
					if (Channeler && !Channeler->GetAIInterface()->getNextTarget() && UnitTarget)
					{
						Channeler->GetAIInterface()->SetAllowedToEnterCombat(true);
						Channeler->GetAIInterface()->AttackReaction(UnitTarget, 1, 0);
					}
				}
				// If Magtheridon is found we remove Banish aura from him
				Unit* Magtheridon = NULL;
				Magtheridon = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-22.657900f, 2.159050f, -0.345542f, 17257);
				if (Magtheridon)
					Magtheridon->RemoveAura(BANISH);

				// If Gate is found we close it
				GameObject*  Gate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-72.5866f, 1.559f, 0.0f, 183847);
				if (Gate)
					Gate->SetState(1);
			}
		}
		// We use different functions for each phase
		switch (Phase)
		{
		case 1:
			// If we are about to release Magtheridon we remove his Banishment aura, change his flag and use emotes that should be used
			PhaseOneTimer++;
			if (PhaseOneTimer == BANISH_TIMER-2)
			{
				Unit* Magtheridon = NULL;
				Magtheridon = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-22.657900f, 2.159050f, -0.345542f, 17257);
				if (Magtheridon)
				{
					if (RandomUInt(4) == 1)
					{	// on movies I saw only another text, but Magtheridon may use this one rarely too, so added here
						Magtheridon->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Thank you for releasing me. Now... die!");
						Magtheridon->PlaySoundToSet(10254);
					}
					else
					{
						Magtheridon->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I... am... unleashed!");
						Magtheridon->PlaySoundToSet(10253);
					}

					Magtheridon->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
					Magtheridon->Emote(EMOTE_ONESHOT_CREATURE_SPECIAL);
					Magtheridon->RemoveAura(BANISHMENT);
				}
			}
			// Time runs out, phase switches and Magtheridon can get In Combat
			if (PhaseOneTimer == BANISH_TIMER)
			{
				Unit* Magtheridon = NULL;
				Magtheridon = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-22.657900f, 2.159050f, -0.345542f, 17257);
				if (Magtheridon)
				{
					Magtheridon->GetAIInterface()->SetAllowedToEnterCombat(true);
					Magtheridon->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
				}

				Phase = 2;
			}
			// Setting checks to reset encounter
			if (PhaseOneTimer == 115 || PhaseOneTimer == 105 || PhaseOneTimer == 90 || PhaseOneTimer == 75 ||
				PhaseOneTimer == 60 || PhaseOneTimer == 45 || PhaseOneTimer == 30 || PhaseOneTimer == 15)
			{
				// We check if any of channelers casts banish spell on Magtheridon and then we reset timer and setting
				Unit* Magtheridon = NULL;
				Magtheridon = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-22.657900f, 2.159050f, -0.345542f, 17257);
				if (Magtheridon)
				{
					Aura* aura = Magtheridon->FindAura(BANISH);
					if (aura)
					{
						EventStarted = false;
						PhaseOneTimer = 0;
						Phase = 0;
					}
				}
				// Script creates vector "list" of alive channelers and counts those In Combat
				Unit* Channeler = NULL;
				size_t AliveInCombat = 0;
				std::vector <Unit*> AliveChannelers;
				for (size_t i = 0; i < ChannelersTable.size(); i++)
				{
					Channeler = ChannelersTable[i];
					if ( Channeler && !Channeler->IsInWorld() )
					{
						ChannelersTable[i] = NULL;
						Channeler = NULL;
						continue;
					}

					if (Channeler && Channeler->isAlive())
					{
						AliveChannelers.push_back(Channeler);
						if (Channeler->GetAIInterface()->getNextTarget())
							AliveInCombat++;
					}
				}
				// If less than half of alive channelers is out of combat we open Magtheridon's gate
				if (AliveInCombat < AliveChannelers.size()/2)
				{
					GameObject*  Gate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-72.5866f, 1.559f, 0.0f, 183847);
					if (Gate)
						Gate->SetState(0);
				}
				// After doing our job we can clear temporary channeler list
				AliveChannelers.clear();
			}
			break;
		default:
			{
			}
		}
    }

protected:

	bool EventStarted;

	uint32 PhaseOneTimer;
	uint32 YellTimer;
	uint32 Phase;
};

// Manticron Cube Gameobject

#define MANTICRON_CUBE	181713

#define MIND_EXHAUSTION	44032

class ManticronCubeGO : public GameObjectAIScript
{
public:
	ManticronCubeGO(GameObject*  pGameObject) : GameObjectAIScript(pGameObject)
	{
		Magtheridon = CubeTrigger = NULL;
	}

	void OnActivate(Player*  pPlayer)
	{
		// We check if player has aura that prevents anyone from using this GO
		Aura* aura = pPlayer->FindAura(MIND_EXHAUSTION);
		if (aura)
			return;

		// If we don't have Magtheridon we try to find it (with normal "getting creature" it was NOT working mostly).
		if (!Magtheridon)
		{
			for (int i = 0; i < 6; i++)
			{
				if (Magtheridon)
					continue;

				Magtheridon = _gameobject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(CaveInPos[i].x, CaveInPos[i].y, CaveInPos[i].z, 17257);
			}
		}
		
		// We check if after trying to find Magtheridon we have found it at least
		if (!Magtheridon)
			return;

		// We check if Magtheridon is in world, is alive, has correct flag and so on
		if (!Magtheridon->isAlive() || !Magtheridon->GetAIInterface()->getNextTarget())
			return;

		// If we haven't "filled" pointer already we do that now
		if (!CubeTrigger)
			CubeTrigger = _gameobject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_gameobject->GetPositionX(), _gameobject->GetPositionY(), _gameobject->GetPositionZ(), 17376);

		// We check if Cube Trigger we want to use exists and if is alive
		if (!CubeTrigger || (CubeTrigger && !CubeTrigger->isAlive()))
			return;

		// We check if Cube Trigger is not in use
		if (CubeTrigger && CubeTrigger->GetChannelSpellId() == SHADOW_GRASP && CubeTrigger->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT) == Magtheridon->GetGUID())
			return;

		// We set player to channel spell "on Cube"
		pPlayer->CastSpell(pPlayer, dbcSpell.LookupEntry(SHADOW_GRASP2), false);

		// We trigger channeling spell on Magtheridon for Cube Trigger
		CubeTrigger->SetChannelSpellTargetGUID(Magtheridon->GetGUID());
		CubeTrigger->SetChannelSpellId(SHADOW_GRASP);

		// We save player data in pointer as well as his position for further use
		x = pPlayer->GetPositionX();
		y = pPlayer->GetPositionY();
		z = pPlayer->GetPositionZ();
		Channeler = pPlayer;

		// We save/initialize vars
		MagYell = false;

		// We register AI to check if GO is still being used
		RegisterAIUpdateEvent(1000);
	}

	void AIUpdate()
	{
		// Channeler settings check
		// We check if pointer has Channeler data and if so we check if that channeler is alive, in world and if channels Cube
		if (Channeler && (!Channeler->isAlive() || !Channeler->IsInWorld()))
		{
			CubeTrigger->SetChannelSpellTargetGUID(0);
			CubeTrigger->SetChannelSpellId(0);

			Channeler = NULL;
		}
		
		// If player still exists (is in world, alive and so on) we check if he has "channeling aura"
		Aura* aura = NULL;
		if (Channeler)
			aura = Channeler->FindAura(SHADOW_GRASP2);

		// If player doesn't have aura we interrupt channeling
		if (Channeler && (!aura || !Channeler->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT)))
		{
			CubeTrigger->SetChannelSpellTargetGUID(0);
			CubeTrigger->SetChannelSpellId(0);

			// If player's channeling went over (and he was hit before) aura won't be removed when channeling ends - core bug
			Channeler->RemoveAura(SHADOW_GRASP2);

			Channeler = NULL;
		}

		// Safe check to prevent crashes when Channeler was nulled
		if (!Channeler)
		{
			uint32 Counter = 0;
			for (int i = 0; i < 5; i++)
			{
				Unit* GlobalCubeTrigger = NULL;
				GlobalCubeTrigger = _gameobject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(CubeTriggers[i].x, CubeTriggers[i].y, CubeTriggers[i].z, 17376);
				if (GlobalCubeTrigger && GlobalCubeTrigger->GetChannelSpellId() == SHADOW_GRASP && CubeTrigger->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT) == Magtheridon->GetGUID())
					Counter++;
			}

			if (!Counter)
			{
				if (Magtheridon && Magtheridon->isAlive())
					Magtheridon->RemoveAura(BANISH);

				MagYell = true;
			}

			return;
		}

		// We check if Magtheridon is spawned, is in world and so on
		if (!Magtheridon || (Magtheridon && (!Magtheridon->isAlive() || !Magtheridon->IsInWorld() || !Magtheridon->GetAIInterface()->getNextTarget())))
		{
			CubeTrigger->SetChannelSpellTargetGUID(0);
			CubeTrigger->SetChannelSpellId(0);
		}

		// We count Cubes that channel spell on Magtheridon
		uint32 Counter = 0;
		for (int i = 0; i < 5; i++)
		{
			Unit* GlobalCubeTrigger = NULL;
			GlobalCubeTrigger = _gameobject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(CubeTriggers[i].x, CubeTriggers[i].y, CubeTriggers[i].z, 17376);
			if (GlobalCubeTrigger && GlobalCubeTrigger->GetChannelSpellId() == SHADOW_GRASP && CubeTrigger->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT) == Magtheridon->GetGUID())
				Counter++;
		}

		// If it's the first and the only one Cube triggering spell we use Magtheridon's yell
		if (Counter == 1 && !MagYell)
		{
			Magtheridon->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Not again... NOT AGAIN!");
			Magtheridon->PlaySoundToSet(10256);

			MagYell = true;
		}

		// If we have all req. Cubes active we may banish Magtheridon
		if (Counter >= ACTIVE_CUBES_TO_BANISH && Magtheridon && Magtheridon->isAlive())
		{
			Magtheridon->CastSpell(Magtheridon, dbcSpell.LookupEntry(BANISH), true);
			Magtheridon->GetAIInterface()->StopMovement(3000);
			Magtheridon->setAttackTimer(3000, false);

			if(Magtheridon->GetCurrentSpell())
				Magtheridon->GetCurrentSpell()->cancel();

			// We add channeling player aura that does not allow that go to be used again in 1.3 min
			Aura* aura = new Aura( dbcSpell.LookupEntry(MIND_EXHAUSTION),(uint32)78000, Magtheridon, Channeler );
			Channeler->AddAura(aura);

			MagYell = true;

			return;
		}

		// If not enough Cubes are active we eventually Banish from Magtheridon
		if (Counter < ACTIVE_CUBES_TO_BANISH && Magtheridon && Magtheridon->isAlive())
		{
			Magtheridon->RemoveAura(BANISH);

			MagYell = true;
		}

	}

	static GameObjectAIScript *Create(GameObject*  GO) { return new ManticronCubeGO(GO); }

protected:

	bool MagYell;
	float x, y, z;
	Unit* Magtheridon;
	Player* Channeler;
	Unit* CubeTrigger;
};

#define CN_CUBE_TRIGGER 17376

class CubeTriggerAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(CubeTriggerAI);

    CubeTriggerAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
	}
};

// Hellfire Warder
#define CN_HELLFIRE_WARDER 18829

#define HW_SHADOW_BOLT_VOLLEY	36275	// 39175
#define SHADOW_WORD_PAIN		34441
#define UNSTABLE_AFFLICTION		35183
#define DEATH_COIL				33130
#define RAIN_OF_FIRE			34435
#define HW_FEAR					34259	// this one is probably wrong
#define SHADOW_BURST			34436

class HellfireWarderAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HellfireWarderAI);
	SP_AI_Spell spells[7];
	bool m_spellcheck[7];

    HellfireWarderAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 7;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
			spells[i].casttime = 0;
		} 

		spells[0].info = dbcSpell.LookupEntry(HW_SHADOW_BOLT_VOLLEY);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 15.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 5;

		spells[1].info = dbcSpell.LookupEntry(SHADOW_WORD_PAIN);
		spells[1].targettype = TARGET_RANDOM_SINGLE;	
		spells[1].instant = true;
		spells[1].perctrigger = 6.0f;
		spells[1].attackstoptimer = 1000;
		spells[1].mindist2cast = 0.0f;
		spells[1].maxdist2cast = 30.0f;
		spells[1].minhp2cast = 0;
		spells[1].maxhp2cast = 100;
		spells[1].cooldown = 7;

		spells[2].info = dbcSpell.LookupEntry(UNSTABLE_AFFLICTION);
		spells[2].targettype = TARGET_RANDOM_SINGLE;
		spells[2].instant = true;
		spells[2].perctrigger = 6.0f;
		spells[2].attackstoptimer = 1000;
		spells[2].mindist2cast = 0.0f;
		spells[2].maxdist2cast = 30.0f;
		spells[2].minhp2cast = 0;
		spells[2].maxhp2cast = 100;
		spells[2].cooldown = 7;

		spells[3].info = dbcSpell.LookupEntry(DEATH_COIL);
		spells[3].targettype = TARGET_RANDOM_SINGLE;
		spells[3].instant = true;
		spells[3].perctrigger = 5.0f;
		spells[3].attackstoptimer = 1000;
		spells[3].mindist2cast = 0.0f;
		spells[3].maxdist2cast = 30.0f;
		spells[3].minhp2cast = 0;
		spells[3].maxhp2cast = 100;
		spells[3].cooldown = 8;

		spells[4].info = dbcSpell.LookupEntry(RAIN_OF_FIRE);
		spells[4].targettype = TARGET_RANDOM_DESTINATION;
		spells[4].instant = true;
		spells[4].perctrigger = 5.0f;
		spells[4].attackstoptimer = 1000;
		spells[4].mindist2cast = 0.0f;
		spells[4].maxdist2cast = 30.0f;
		spells[4].minhp2cast = 0;
		spells[4].maxhp2cast = 100;
		spells[4].cooldown = 6;

		spells[5].info = dbcSpell.LookupEntry(HW_FEAR);
		spells[5].targettype = TARGET_RANDOM_SINGLE;
		spells[5].instant = false;
		spells[5].perctrigger = 4.0f;
		spells[5].attackstoptimer = 1000;
		spells[5].mindist2cast = 0.0f;
		spells[5].maxdist2cast = 30.0f;
		spells[5].minhp2cast = 0;
		spells[5].maxhp2cast = 100;
		spells[5].cooldown = 10;

		spells[6].info = dbcSpell.LookupEntry(SHADOW_BURST);
		spells[6].targettype = TARGET_VARIOUS;
		spells[6].instant = false;							// not sure but works ;)
		spells[6].perctrigger = 4.0f;
		spells[6].attackstoptimer = 8;
	}

	void OnCombatStart(Unit* mTarget)
    {
        RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));

		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
    }

    void OnCombatStop(Unit* mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

	void OnDied(Unit* mKiller)
    {
		RemoveAIUpdateEvent();
    }

	void AIUpdate()
    {
		float val = RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
        {
			float comulativeperc = 0;
		    Unit* target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->getNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
        {
			std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && (*itr)->IsUnit()) // isAttackable(_unit, (*itr)) && 
				{
					Unit* RandomTarget = NULL;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit*  RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}

protected:

	int nrspells;
};

// Hellfire Channeler
#define CN_HELLFIRE_CHANNELER 17256

#define SHADOW_BOLT_VOLLEY	30510	// 39175
#define FEAR				30615	// not sure
#define DARK_MENDING		30528
#define BURNING_ABYSSAL		30511

class HellfireChannelerAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(HellfireChannelerAI);
	SP_AI_Spell spells[5];
	bool m_spellcheck[5];
	
	HellfireChannelerAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
			spells[i].casttime = 0;
		}
			
		spells[0].info = dbcSpell.LookupEntry(SHADOW_BOLT_VOLLEY);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 5;

		spells[1].info = dbcSpell.LookupEntry(FEAR);
		spells[1].targettype = TARGET_RANDOM_SINGLE;	
		spells[1].instant = false;
		spells[1].perctrigger = 7.0f;
		spells[1].attackstoptimer = 1000;
		spells[1].mindist2cast = 0.0f;
		spells[1].maxdist2cast = 20.0f;
		spells[1].minhp2cast = 0;
		spells[1].maxhp2cast = 100;
		spells[1].cooldown = 10;
		
		spells[2].info = dbcSpell.LookupEntry(DARK_MENDING);
		spells[2].targettype = TARGET_RANDOM_FRIEND;	
		spells[2].instant = false;
		spells[2].perctrigger = 8.0f;
		spells[2].attackstoptimer = 1000;
		spells[2].mindist2cast = 0.0f;
		spells[2].maxdist2cast = 40.0f;
		spells[2].minhp2cast = 0;
		spells[2].maxhp2cast = 70;
		spells[2].cooldown = 8;

		spells[3].info = dbcSpell.LookupEntry(BURNING_ABYSSAL);
		spells[3].targettype = TARGET_RANDOM_SINGLE;	
		spells[3].instant = true;
		spells[3].perctrigger = 6.0f;
		spells[3].attackstoptimer = 1000;
		spells[3].mindist2cast = 0.0f;
		spells[3].maxdist2cast = 30.0f;
		spells[3].minhp2cast = 0;
		spells[3].maxhp2cast = 100;
		spells[3].cooldown = 30;

		spells[4].info = dbcSpell.LookupEntry(SOUL_TRANSFER);
		spells[4].targettype = TARGET_VARIOUS;	
		spells[4].instant = true;

		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
	}

	void OnCombatStart(Unit* mTarget)
	{
		RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));

		_unit->SetChannelSpellTargetGUID(0);
		_unit->SetChannelSpellId(0);

		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
	}
		
	void OnCombatStop(Unit* mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();

		if (_unit->isAlive())
		{

			Unit* Magtheridon = NULL;
			Magtheridon = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-22.657900f, 2.159050f, -0.345542f, 17257);
			if (Magtheridon && Magtheridon->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9))
			{
				_unit->SetChannelSpellTargetGUID(Magtheridon->GetGUID());
				_unit->SetChannelSpellId(SHADOW_GRASP);

				Magtheridon->CastSpell(Magtheridon, dbcSpell.LookupEntry(BANISH), true);
			}
		}
	}

	void OnDamageTaken(Unit* mAttacker, uint32 fAmount)
	{
		if (!_unit->GetAIInterface()->GetAllowedToEnterCombat())
			_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
	}
		
	void OnDied(Unit* mKiller)
	{
		RemoveAIUpdateEvent();

		_unit->SetChannelSpellTargetGUID(0);
		_unit->SetChannelSpellId(0);
	}
		
	void AIUpdate()
	{
		float val = RandomFloat(100.0f);
		SpellCast(val);
	}
	
	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
		{
			float comulativeperc = 0;
			Unit* target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->getNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					if (i == 2 && _unit->GetHealthPct() > 50)
						continue;

					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
        {
			std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && (*itr)->IsUnit()) // isAttackable(_unit, (*itr)) && 
				{
					Unit* RandomTarget = NULL;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit*  RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}
		
protected:
		
	int nrspells;
};

// Burning Abyssal AI

#define CN_BURNING_ABYSSAL 17454

#define FIRE_BLAST 37110

class BurningAbyssalAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(BurningAbyssalAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    BurningAbyssalAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
			spells[i].casttime = 0;
		} 

		spells[0].info = dbcSpell.LookupEntry(FIRE_BLAST);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].mindist2cast = 0.0f;
		spells[0].maxdist2cast = 20.0f;
		spells[0].minhp2cast = 0;
		spells[0].maxhp2cast = 100;
		spells[0].cooldown = 10;

		_unit->m_noRespawn = true;
	}

	void OnCombatStart(Unit* mTarget)
    {
        RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));

		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
    }

    void OnCombatStop(Unit* mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

	void OnDied(Unit* mKiller)
    {
		RemoveAIUpdateEvent();
    }

	void AIUpdate()
    {
		float val = RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
        {
			float comulativeperc = 0;
		    Unit* target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->getNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
        {
			std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && (*itr)->IsUnit()) // isAttackable(_unit, (*itr)) && 
				{
					Unit* RandomTarget = NULL;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit*  RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}

protected:

	int nrspells;
};

// Magtheridon
#define CN_MAGTHERIDON 17257

// Normal Casts
#define CLEAVE			31345	// not sure; should be better, but not sure if it gives 8k dmg
#define CONFLAGRATION	23023	// 35840 - this one was affecting caster; not sure - it's not right spell, but better than nothing for now

// Timed Casts
#define QUAKE1			30571	// Each 40 sec after Phase 2 starts
#define QUAKE2			30658	// Effect
#define BLAST_NOVA		30616	// Each 60 sec after Phase 2 starts
#define CAVE_IN			36240	// don't know correct timer
#define CAMERA_SHAKE	36455	// is used when Magtheridon uses Cave In
#define ENRAGE			34624	// dunno if it's correct spell	-- 20 min after Phase 2 starts

class MagtheridonAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(MagtheridonAI);
	SP_AI_Spell spells[7];
	bool m_spellcheck[7];

    MagtheridonAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = true;
			spells[i].casttime = 0;
		}

		spells[0].info = dbcSpell.LookupEntry(CLEAVE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 6.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 15;

		spells[1].info = dbcSpell.LookupEntry(CONFLAGRATION);
		spells[1].targettype = TARGET_RANDOM_SINGLE;
		spells[1].instant = true;
		spells[1].perctrigger = 7.0f;
		spells[1].attackstoptimer = 1000;
		spells[1].mindist2cast = 0.0f;
		spells[1].maxdist2cast = 30.0f;
		spells[1].minhp2cast = 0;
		spells[1].maxhp2cast = 100;
		spells[1].cooldown = 35;

		spells[2].info = dbcSpell.LookupEntry(QUAKE1);
		spells[2].targettype = TARGET_VARIOUS;
		spells[2].instant = true;

		spells[3].info = dbcSpell.LookupEntry(QUAKE2);
		spells[3].targettype = TARGET_VARIOUS;
		spells[3].instant = true;

		spells[4].info = dbcSpell.LookupEntry(BLAST_NOVA);
		spells[4].targettype = TARGET_VARIOUS;
		spells[4].instant = false;

		spells[5].info = dbcSpell.LookupEntry(CAVE_IN);
		spells[5].instant = true;

		spells[6].info = dbcSpell.LookupEntry(ENRAGE);
		spells[6].targettype = TARGET_SELF;
		spells[6].instant = true;

		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);

		Aura* aura = new Aura( dbcSpell.LookupEntry(BANISHMENT), (uint32)-1, _unit, _unit );
		_unit->AddAura(aura);

		_unit->CastSpell(_unit, dbcSpell.LookupEntry(BANISH), true);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);

		timer_quake = timer_enrage = timer_blastNova = timer_caveIn = 0;
		PhaseSwitch = false;
	}

	void OnCombatStart(Unit* mTarget)
    {		
		timer_quake = timer_enrage = timer_blastNova = timer_caveIn = 0;
		PhaseSwitch = false;

        RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));

		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;

		GameObject* Gate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-72.5866f, 1.559f, 0.0f, 183847);
		if (Gate)
			Gate->SetState(1);
    }

    void OnCombatStop(Unit* mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();

		if (_unit->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9) || _unit->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2))
			return;

		GameObject*  Gate = NULL;
		for (int i = 0; i < 6; i++)
		{
			Gate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(Columns[i].x, Columns[i].y, Columns[i].z, 184634+i);
			if (Gate)
				Gate->SetState(1);
		}

		Gate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(0.0f, 0.0f, 0.0f, 184653);
		if (Gate)
			Gate->SetState(1);

		Gate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-72.5866f, 1.559f, 0.0f, 183847);
		if (Gate)
			Gate->SetState(0);
    }

	void OnDied(Unit* mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The Legion... will consume you... all....");
		_unit->PlaySoundToSet(10258);

       RemoveAIUpdateEvent();
    }

	void OnTargetDied(Unit* mTarget)
	{
		if (_unit->GetHealthPct() > 0)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Did you think me weak? Soft? Who is the weak one now?!");
			_unit->PlaySoundToSet(10255);
		}
	}
	
	void AIUpdate()
    {
		if (PhaseSwitch)
			PhaseThree();
		else PhaseTwo();
    }

	void PhaseTwo()
	{
		Aura* aura = _unit->FindAura(BANISH);

		if (_unit->GetHealthPct() <= 30)
		{
			timer_caveIn = 1;
			PhaseSwitch = true;
		}

		timer_quake++;
		timer_enrage++;
		timer_blastNova++;

		if (timer_quake > 27)
		{
			if (timer_quake < 33)
			{
				_unit->CastSpell(_unit, spells[2].info, spells[2].instant);

				for (int i = 0; i < 6; i++)
				{
					Unit* Trigger = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(CaveInPos[i].x, CaveInPos[i].y, CaveInPos[i].z, 17474);
					if (Trigger)
					{
						Trigger->CastSpell(Trigger, spells[3].info, spells[3].instant);
					}
				}
			}

			if (timer_quake == 32)
				timer_quake = 0;
		}
		
		if (timer_blastNova == 33)
		{
			_unit->SendChatMessageAlternateEntry(17257, CHAT_MSG_EMOTE, LANG_UNIVERSAL, " begins to cast Blast Nova!");
		}
		if(timer_blastNova > 33 && _unit->GetCurrentSpell() == NULL && !aura)
		{
			_unit->GetAIInterface()->StopMovement(3000);
			_unit->setAttackTimer(3000, false);
			
			_unit->CastSpell(_unit, dbcSpell.LookupEntry(BLAST_NOVA), false);

			timer_blastNova = 0;
			timer_quake = 0;
			return;
		}

		if(timer_enrage > 667 && _unit->GetCurrentSpell() == NULL && !aura)
		{
			_unit->CastSpell(_unit, dbcSpell.LookupEntry(ENRAGE), true);

			timer_enrage = 0;
		}

		if (!aura)
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}
	}

	void PhaseThree()
	{
		Aura* aura = _unit->FindAura(BANISH);

		timer_quake++;
		timer_enrage++;
		timer_blastNova++;

		if (timer_quake > 27)
		{
			if (timer_quake < 33)
			{
				_unit->CastSpell(_unit, spells[2].info, spells[2].instant);

				for (int i = 0; i < 6; i++)
				{
					Unit* Trigger = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(CaveInPos[i].x, CaveInPos[i].y, CaveInPos[i].z, 17474);
					if (Trigger)
					{
						Trigger->CastSpell(Trigger, spells[3].info, spells[3].instant);
					}
				}
			}

			if (timer_quake == 32)
				timer_quake = 0;
		}

		if (timer_blastNova == 33)
		{
			_unit->SendChatMessageAlternateEntry(17257, CHAT_MSG_MONSTER_EMOTE, LANG_UNIVERSAL, " begins to cast Blast Nova!");
		}
		if(timer_blastNova > 33 && _unit->GetCurrentSpell() == NULL && !aura)
		{
			_unit->GetAIInterface()->StopMovement(3000);
			_unit->setAttackTimer(3000, false);

			_unit->CastSpell(_unit, dbcSpell.LookupEntry(BLAST_NOVA), false);

			timer_blastNova = 0;
			timer_quake = 0;
			return;
		}

		if(timer_caveIn && (timer_caveIn != 1 || (_unit->GetCurrentSpell() == NULL && timer_caveIn == 1 && !aura)))
		{
			timer_caveIn++;
			if (timer_caveIn == 2)
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I will not be taken so easily. Let the walls of this prison tremble... and FALL!!!");
				_unit->PlaySoundToSet(10257);

				_unit->GetAIInterface()->StopMovement(2000);
				_unit->setAttackTimer(2000, false);

				_unit->CastSpell(_unit, dbcSpell.LookupEntry(CAMERA_SHAKE), true);
				return;
			}

			if (timer_caveIn == 3)
			{
				GameObject*  Gate = NULL;
				for (int i = 0; i < 6; i++)
				{
					Gate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(Columns[i].x, Columns[i].y, Columns[i].z, 184634+i);
					if (Gate)
						Gate->SetState(0);
				}

				Gate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(0.0f, 0.0f, 0.0f, 184653);
				if (Gate)
					Gate->SetState(0);
			}

			if (timer_caveIn == 5)
			{
				for (int i = 0; i < 6; i++)
				{
					Unit* Trigger = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(CaveInPos[i].x, CaveInPos[i].y, CaveInPos[i].z, 17474);
					if (Trigger)
					{
						Trigger->CastSpellAoF(CaveInPos[i].x, CaveInPos[i].y, CaveInPos[i].z, spells[5].info, spells[5].instant);
					}
				}

				timer_caveIn = 0;
			}
		}

		if(timer_enrage > 667 && _unit->GetCurrentSpell() == NULL && !aura)
		{
			_unit->CastSpell(_unit, dbcSpell.LookupEntry(ENRAGE), true);

			timer_enrage = 0;
			//return;
		}

		if (!aura)
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}
	}
	
	void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
        {
			float comulativeperc = 0;
		    Unit* target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->getNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
        {
			std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && (*itr)->IsUnit()) // isAttackable(_unit, (*itr)) && 
				{
					Unit* RandomTarget = NULL;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit*  RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}

protected:

	int nrspells;
	int timer_quake;
	int timer_enrage;
	int timer_caveIn;
	int timer_blastNova;

	bool PhaseSwitch;
};

void SetupMagtheridonsLair(ScriptMgr * mgr)
{
	mgr->register_gameobject_script(MANTICRON_CUBE, &ManticronCubeGO::Create);

	mgr->register_creature_script(CN_MAGTHERIDON_TRIGGER, &MagtheridonTriggerAI::Create);
	mgr->register_creature_script(CN_CUBE_TRIGGER, &CubeTriggerAI::Create);
	mgr->register_creature_script(CN_HELLFIRE_WARDER, &HellfireWarderAI::Create);
	mgr->register_creature_script(CN_HELLFIRE_CHANNELER, &HellfireChannelerAI::Create);
	mgr->register_creature_script(CN_BURNING_ABYSSAL, &BurningAbyssalAI::Create);
	mgr->register_creature_script(CN_MAGTHERIDON, &MagtheridonAI::Create);	
}
