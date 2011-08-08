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


// Infiltrating Dragonmaw Fortress Quest
class InfiltratingDragonmawFortressQAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(InfiltratingDragonmawFortressQAI);
		InfiltratingDragonmawFortressQAI(Creature* pCreature) : CreatureAIScript(pCreature)  {}

		void OnDied(Unit* mKiller)
		{
			if(mKiller->IsPlayer())
			{
				QuestLogEntry* en = (TO_PLAYER(mKiller))->GetQuestLogForEntry(10836);
				if(en && en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
				{
					uint32 newcount = en->GetMobCount(0) + 1;
					en->SetMobCount(0, newcount);
					en->SendUpdateAddKill(0);
					en->UpdatePlayerFields();
					return;
				}
			}
		}
};

class KneepadsQAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(KneepadsQAI);
		KneepadsQAI(Creature* pCreature) : CreatureAIScript(pCreature)  {}

		void OnDied(Unit* mKiller)
		{
			if(mKiller->IsPlayer())
			{
				QuestLogEntry* en = NULL;
				en = (TO_PLAYER(mKiller))->GetQuestLogForEntry(10703);
				if(en == NULL)
				{
					en = (TO_PLAYER(mKiller))->GetQuestLogForEntry(10702);
					if(en == NULL)
					{
						return;
					}
				}

				if(en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
				{
					uint32 newcount = en->GetMobCount(0) + 1;
					en->SetMobCount(0, newcount);
					en->SendUpdateAddKill(0);
					en->UpdatePlayerFields();
				}
			}
			return;
		}
};



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////// Deathbringer Jovaan
#define CN_DEATHBRINGER_JOVAAN	21633

//WP Coords Wait Times
struct WPWaitTimes
{
	LocationExtra mCoords;
	uint32 WaitTime;
};
const WPWaitTimes DeathbringerJovaanWP[] =
{
	{ { }, 0},
	{ { -3310.743896f, 2951.929199f, 171.132538f, 5.054039f, Flag_Walk }, 0 },
	{ { -3308.501221f, 2940.098389f, 171.025772f, 5.061895f, Flag_Walk }, 0 },
	{ { -3306.261203f, 2933.843210f, 170.934145f, 5.474234f, Flag_Walk }, 44000 },
	{ { -3310.743896f, 2951.929199f, 171.132538f, 1.743588f, Flag_Walk }, 0 }
};

class DeathbringerJovaanAI : public MoonScriptCreatureAI
{
	public:
		MOONSCRIPT_FACTORY_FUNCTION(DeathbringerJovaanAI, MoonScriptCreatureAI);
		DeathbringerJovaanAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			mJovaanTimer = INVALIDATE_TIMER;
			mJovaanPhase = -1;

			for(int i = 1; i < 5; ++i)
			{
				AddWaypoint(CreateWaypoint(i, DeathbringerJovaanWP[i].WaitTime, DeathbringerJovaanWP[i].mCoords.addition, DeathbringerJovaanWP[i].mCoords));
			}
		}

		void AIUpdate()
		{
			if(IsTimerFinished(mJovaanTimer))
			{
				switch(mJovaanPhase)
				{
					case 0:
						{
							MoonScriptCreatureAI* pRazuunAI = SpawnCreature(21502, -3300.47f, 2927.22f, 173.870f, 2.42924f, false);	// Spawn Razuun
							if(pRazuunAI != NULL)
							{
								pRazuunAI->GetUnit()->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
								pRazuunAI->SetCanEnterCombat(false);
								pRazuunAI->SetMoveType(Move_DontMoveWP);
								pRazuunAI->SetCanMove(false);
							}
							_unit->SetStandState(STANDSTATE_KNEEL);
							_unit->Emote(EMOTE_ONESHOT_TALK);
							_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Everything is in readiness, warbringer.");
							mJovaanPhase = 1;
							ResetTimer(mJovaanTimer, 6000);
						}
						break;
					case 1:
						{
							_unit->Emote(EMOTE_ONESHOT_TALK);
							_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Warbringer, that will require the use of all the hold's infernals. It may leave us vulnerable to a counterattack.");
							mJovaanPhase = 2;
							ResetTimer(mJovaanTimer, 11000);
						}
						break;
					case 2:
						{
							_unit->SetStandState(STANDSTATE_STAND);
							mJovaanPhase = 3;
							ResetTimer(mJovaanTimer, 1000);
						}
						break;
					case 3:
						{
							_unit->Emote(EMOTE_ONESHOT_SALUTE);
							_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "It shall be as you say, warbringer. One last question, if I may...");
							mJovaanPhase = 4;
							ResetTimer(mJovaanTimer, 10000);
						}
						break;
					case 4:
						{
							_unit->Emote(EMOTE_ONESHOT_QUESTION);
							_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "What's in the crate?");
							mJovaanPhase = 5;
							ResetTimer(mJovaanTimer, 10000);
						}
						break;
					case 5:
						{
							_unit->Emote(EMOTE_ONESHOT_SALUTE);
							mJovaanPhase = -1;
							RemoveTimer(mJovaanTimer);
						}
						break;
				}
			}
			ParentClass::AIUpdate();
		}

		void OnReachWP(uint32 iWaypointId, bool bForwards)
		{
			switch(iWaypointId)
			{
				case 3:
					{
						RegisterAIUpdateEvent(1000);
						_unit->Emote(EMOTE_ONESHOT_POINT);
						mJovaanPhase = 0;
						mJovaanTimer = AddTimer(1500);
					}
					break;
				case 4:
					{
						Despawn(1, 0);
					}
					break;
			}
		}

		int32	mJovaanTimer;
		int32	mJovaanPhase;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////// Warbringer Razuun
#define CN_WARBRINGER_RAZUUN	21502

class WarbringerRazuunAI : public MoonScriptCreatureAI
{
	public:
		MOONSCRIPT_FACTORY_FUNCTION(WarbringerRazuunAI, MoonScriptCreatureAI);
		WarbringerRazuunAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			RegisterAIUpdateEvent(1000);
			mRazuunTimer = AddTimer(800);
			mRazuunPhase = 0;
		}

		void AIUpdate()
		{
			if(IsTimerFinished(mRazuunTimer))
			{
				switch(mRazuunPhase)
				{
					case 0:
						{
							_unit->Emote(EMOTE_ONESHOT_TALK);
							_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Doom Lord Kazzak will be pleased. You are to increase the pace of your attacks. Destroy the orcish and dwarven strongholds with all haste.");
							mRazuunPhase = 1;
							ResetTimer(mRazuunTimer, 9000);
						}
						break;
					case 1:
						{
							_unit->Emote(EMOTE_ONESHOT_TALK);
							_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Don't worry about that. I've increased production at the Deathforge. You'll have all the infernals you need to carry out your orders. Don't fail, Jovaan.");
							mRazuunPhase = 2;
							ResetTimer(mRazuunTimer, 15000);
						}
						break;
					case 2:
						{
							_unit->Emote(EMOTE_ONESHOT_QUESTION);
							_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Yes?");
							mRazuunPhase = 3;
							ResetTimer(mRazuunTimer, 8000);
						}
						break;
					case 3:
						{
							_unit->Emote(EMOTE_ONESHOT_QUESTION);
							_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Crate? I didn't send you a crate, Jovaan. Don't you have more important things to worry about? Go see to them!");
							mRazuunPhase = 4;
							ResetTimer(mRazuunTimer, 5000);
						}
						break;
					case 4:
						{
							mRazuunPhase = -1;
							RemoveTimer(mRazuunTimer);
							Despawn(0, 0);
							return;
						}
						break;
				}
			}
			ParentClass::AIUpdate();
		}

		int32	mRazuunTimer;
		int32	mRazuunPhase;
};





class NeltharakusTale_Gossip : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			if(plr->GetQuestLogForEntry(10814))
			{
				GossipMenu* Menu;
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10613, plr);
				Menu->AddItem(0, "I am listening, dragon", 1);
				Menu->SendTo(plr);
			}
		}

		void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char* Code)
		{
			if(!pObject->IsCreature())
				return;

			switch(IntId)
			{
				case 1:
					{
						GossipMenu* Menu;
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10614, plr);
						Menu->AddItem(0, "But you are dragons! How could orcs do this to you?", 2);
						Menu->SendTo(plr);
					}
					break;
				case 2:
					{
						GossipMenu* Menu;
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10615, plr);
						Menu->AddItem(0, "Your mate?", 3);
						Menu->SendTo(plr);
					}
					break;
				case 3:
					{
						GossipMenu* Menu;
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10616, plr);
						Menu->AddItem(0, "I have battled many beasts, dragon. I will help you.", 4);
						Menu->SendTo(plr);
					}
					break;
				case 4:
					{
						QuestLogEntry* pQuest = plr->GetQuestLogForEntry(10814);
						if(pQuest && pQuest->GetMobCount(0) < pQuest->GetQuest()->required_mobcount[0])
						{
							pQuest->SetMobCount(0, 1);
							pQuest->SendUpdateAddKill(0);
							pQuest->UpdatePlayerFields();
						}
					}
					break;
			}
		}

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////// Warbringer Razuun
#define CN_ENSLAVED_NETHERWING_DRAKE	21722

class EnslavedNetherwingDrakeAI : public MoonScriptCreatureAI
{
	public:
		MOONSCRIPT_FACTORY_FUNCTION(EnslavedNetherwingDrakeAI, MoonScriptCreatureAI);
		EnslavedNetherwingDrakeAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			LocationExtra WayPoint = { _unit->GetPositionX(), _unit->GetPositionY() + 30, _unit->GetPositionZ() + 100, _unit->GetOrientation(), Flag_Fly };
			SetCanMove(false);
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_FEIGN_DEATH | UNIT_FLAG_NOT_ATTACKABLE_2);
			AddWaypoint(CreateWaypoint(1, 0, Flag_Fly, WayPoint));
		}

		void OnReachWP(uint32 iWaypointId, bool bForwards)
		{
			if(iWaypointId == 1)
			{
				Despawn(0, 3 * 60 * 1000);
			}
		}
};



class KarynakuChains : public GameObjectAIScript
{
	public:
		KarynakuChains(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
		static GameObjectAIScript* Create(GameObject* GO) { return new KarynakuChains(GO); }

		void OnActivate(Player* pPlayer)
		{
			QuestLogEntry* pQuest = pPlayer->GetQuestLogForEntry(10872);

			if(pQuest == NULL)
				return;

			pQuest->SetMobCount(0, pQuest->GetMobCount(0) + 1);
			pQuest->SendUpdateAddKill(0);
			pQuest->UpdatePlayerFields();
		}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////// Flanis Swiftwing
class FlanisSwiftwing_Gossip : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* Plr);
		void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char* Code);
		void GossipEnd(Object* pObject, Player* Plr, Creature* pCreature);

};

void FlanisSwiftwing_Gossip::GossipHello(Object* pObject, Player* plr)
{
	GossipMenu* Menu;
	objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 40002, plr);
	if(plr->GetQuestLogForEntry(10583) != NULL)
		Menu->AddItem(0, "Examine the corpse", 1);

	Menu->SendTo(plr);
};

void FlanisSwiftwing_Gossip::GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char* Code)
{
	Creature* pCreature = (pObject->IsCreature()) ? TO_CREATURE(pObject) : NULL;
	if(pCreature == NULL)
		return;
	if(IntId == 1)
	{
		Item* item;
		item = objmgr.CreateItem(30658 , plr);
		item->SetStackCount(1);
		if(!plr->GetItemInterface()->AddItemToFreeSlot(item))
		{
			plr->GetSession()->SendNotification("No free slots were found in your inventory!");
			item->DeleteMe();
		}
		else
		{
			plr->SendItemPushResult(false, true, false, true, plr->GetItemInterface()->LastSearchResult()->ContainerSlot,
			                        plr->GetItemInterface()->LastSearchResult()->Slot, 1, item->GetEntry(), item->GetItemRandomSuffixFactor(),
			                        item->GetItemRandomPropertyId(), item->GetStackCount());
		}
	}
};
void FlanisSwiftwing_Gossip::GossipEnd(Object* pObject, Player* Plr, Creature* pCreature)
{
	GossipScript::GossipEnd(pObject, Plr);
}

void SetupShadowmoon(ScriptMgr* mgr)
{
	mgr->register_creature_script(11980, &InfiltratingDragonmawFortressQAI::Create);
	mgr->register_creature_script(21718, &InfiltratingDragonmawFortressQAI::Create);
	mgr->register_creature_script(21719, &InfiltratingDragonmawFortressQAI::Create);
	mgr->register_creature_script(21720, &InfiltratingDragonmawFortressQAI::Create);
	mgr->register_creature_script(22253, &InfiltratingDragonmawFortressQAI::Create);
	mgr->register_creature_script(22274, &InfiltratingDragonmawFortressQAI::Create);
	mgr->register_creature_script(22331, &InfiltratingDragonmawFortressQAI::Create);
	mgr->register_creature_script(23188, &InfiltratingDragonmawFortressQAI::Create);
	mgr->register_creature_script(21717, &InfiltratingDragonmawFortressQAI::Create);
	mgr->register_creature_script(21878, &KneepadsQAI::Create);
	mgr->register_creature_script(21879, &KneepadsQAI::Create);
	mgr->register_creature_script(21864, &KneepadsQAI::Create);
	mgr->register_creature_script(CN_DEATHBRINGER_JOVAAN, &DeathbringerJovaanAI::Create);
	mgr->register_creature_script(CN_WARBRINGER_RAZUUN, &WarbringerRazuunAI::Create);
	mgr->register_creature_script(CN_ENSLAVED_NETHERWING_DRAKE, &EnslavedNetherwingDrakeAI::Create);

	mgr->register_gameobject_script(185156, &KarynakuChains::Create);

	GossipScript* NeltharakusTaleGossip = new NeltharakusTale_Gossip;
	mgr->register_gossip_script(21657, NeltharakusTaleGossip);

	GossipScript* FlanisSwiftwingGossip = new FlanisSwiftwing_Gossip;
	mgr->register_gossip_script(21727, FlanisSwiftwingGossip); //Add Flanis' Pack
}
