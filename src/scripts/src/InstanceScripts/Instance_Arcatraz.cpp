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
/* Instance_Arcatraz.cpp Script											*/
/************************************************************************/

// Zereketh the UnboundAI
#define CN_ZEREKETH			20870
#define CN_VOIDZONEARC		21101

#define SEED_OF_C			36123	//32865, 36123
#define SHADOW_NOVA			36127 // 30533, 39005, 36127 (normal mode), 39005 (heroic mode?)
#define SHADOW_NOVA_H		39005
#define CONSUMPTION			30498
#define CONSUMPTION_H		39004
// #define VOID_ZONE 36119	// DBC: 36119; it's not fully functionl without additional core support (for dmg and random place targeting).

class ZerekethAI : public MoonScriptBossAI
{
	public:
		MOONSCRIPT_FACTORY_FUNCTION(ZerekethAI, MoonScriptBossAI);

		ZerekethAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
		{
			AddSpell(SEED_OF_C, Target_RandomPlayer, 6.0f, 2, 20, 0, 100.0f);

			if(!IsHeroic())
				AddSpell(SHADOW_NOVA, Target_Self, 15, 2, 15);
			else
				AddSpell(SHADOW_NOVA_H, Target_Self, 15, 2, 15);

			AddEmote(Event_OnCombatStart, "Life energy to... consume.", Text_Yell, 11250);
			AddEmote(Event_OnTargetDied, "This vessel...is empty.", Text_Yell, 11251);
			AddEmote(Event_OnTargetDied, "No... more... life.", Text_Yell, 11252);
			AddEmote(Event_OnDied, "The Void... beckons.", Text_Yell, 11255);
		}
		void OnCombatStart(Unit* mTarget)
		{
			VoidTimer = AddTimer((RandomUInt(10) + 30) * 1000);
			SpeechTimer = AddTimer((RandomUInt(10) + 40) * 1000);

			ParentClass::OnCombatStart(mTarget);
		}

		void OnDied(Unit* mKiller)
		{
			//despawn voids
			Creature* creature = NULL;
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd();)
			{
				Object* obj = *itr;
				++itr;
				if(obj->IsCreature())
				{
					creature = TO_CREATURE(obj);

					if(creature->GetCreatureInfo()->Id == 21101 && creature->isAlive())
					{
						creature->Despawn(0, 0);
					}
				}
			}

			ParentClass::OnDied(mKiller);
		}

		void Speech()
		{
			switch(RandomUInt(1))
			{
				case 0:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The shadow... will engulf you.");
					_unit->PlaySoundToSet(11253);
					break;
				case 1:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Darkness... consumes all.");
					_unit->PlaySoundToSet(11254);
					break;
			}
			ResetTimer(SpeechTimer, (RandomUInt(10) + 40) * 1000);
		}

		void VoidZoneArc()
		{
			ResetTimer(VoidTimer, (RandomUInt(10) + 30) * 1000);

			std::vector<Player*> TargetTable;
			set< Object* >::iterator Itr = _unit->GetInRangePlayerSetBegin();
			for(; Itr != _unit->GetInRangePlayerSetEnd(); Itr++)
			{
				Player* RandomTarget = NULL;
				if(!(*Itr)->IsPlayer())
					continue;
				RandomTarget = TO< Player* >(*Itr);
				if(RandomTarget && RandomTarget->isAlive() && isHostile(*Itr, _unit))
					TargetTable.push_back(RandomTarget);
			}

			if(!TargetTable.size())
				return;

			size_t RandTarget = rand() % TargetTable.size();

			Player*  RTarget = TargetTable[RandTarget];

			if(!RTarget)
				return;

			float vzX = RandomUInt(5) * cos(RandomFloat(6.28f)) + RTarget->GetPositionX();
			float vzY = RandomUInt(5) * cos(RandomFloat(6.28f)) + RTarget->GetPositionY();
			float vzZ = RTarget->GetPositionZ();
			MoonScriptCreatureAI* VoidZone = SpawnCreature(CN_VOIDZONEARC, vzX, vzY, vzZ);
			VoidZone->GetUnit()->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
			VoidZone->GetUnit()->m_noRespawn = true;
			if(!VoidZone->GetUnit()->IsInWorld())
			{
				VoidZone->Despawn();
				return;
			}
			RTarget = NULL;
			VoidZone->Despawn(60000, 0);
		}

		void AIUpdate()
		{
			if(IsTimerFinished(SpeechTimer))
				Speech();

			if(IsTimerFinished(VoidTimer))
				VoidZoneArc();

			ParentClass::AIUpdate();
		}

	protected:

		int32 SpeechTimer;
		int32 VoidTimer;
};

class VoidZoneARC : public MoonScriptCreatureAI
{
	public:
		MOONSCRIPT_FACTORY_FUNCTION(VoidZoneARC, MoonScriptCreatureAI);
		VoidZoneARC(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			StopMovement();
			SetCanMove(false);
			SetCanEnterCombat(false);
			RegisterAIUpdateEvent(1000);
		};

		void AIUpdate()
		{
			// M4ksiu: I'm not sure if it should be cast once, on start
			uint32 SpellId = CONSUMPTION;
			if(IsHeroic())
				SpellId = CONSUMPTION_H;

			_unit->CastSpell(_unit, SpellId, true);
			RemoveAIUpdateEvent();
		};
};


// Dalliah the DoomsayerAI

#define CN_DALLIAH_THE_DOOMSAYER 20885

#define GIFT_OF_THE_DOOMSAYER 36173 // DBC: 36173
#define WHIRLWIND 36175	// DBC: 36142, 36175
#define HEAL 36144
#define SHADOW_WAVE 39016	// Heroic mode spell
// sounds missing related to Wrath... (look on script below this one)

class DalliahTheDoomsayerAI : public MoonScriptBossAI
{
	public:
		MOONSCRIPT_FACTORY_FUNCTION(DalliahTheDoomsayerAI, MoonScriptBossAI);
		DalliahTheDoomsayerAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
		{
			AddSpell(GIFT_OF_THE_DOOMSAYER, Target_Current, 8.0f, 0.0f, -1);

			SpellDesc* WhirlTemp = AddSpell(WHIRLWIND, Target_Self, 15.0f, 0.0f, -1);
			WhirlTemp->AddEmote("Reap the Whirlwind!", Text_Yell, 11089);
			WhirlTemp->AddEmote("I'll cut you to peices!", Text_Yell, 11090);

			SpellDesc* HealTemp = AddSpell(HEAL, Target_Self, 8.0f, 0, -1);
			HealTemp->AddEmote("That is much better.", Text_Yell, 11091);
			HealTemp->AddEmote("Ah, just what I needed.", Text_Yell, 11092);

			if(IsHeroic())
				AddSpell(SHADOW_WAVE, Target_Current, 8.0f, 0, -1);

			AddEmote(Event_OnCombatStart, "It is unwise to anger me.", Text_Yell, 11086);
			AddEmote(Event_OnTargetDied, "Completely ineffective! Just like someone else I know!", Text_Yell, 11087);
			AddEmote(Event_OnTargetDied, "You chose the wrong opponent!", Text_Yell, 11088);
			AddEmote(Event_OnDied, "Now I'm really... angry...", Text_Yell, 11093);
		}

		void OnDied(Unit* mKiller)
		{
			GameObject* door2 = NULL;
			door2 = GetNearestGameObject(184319);
			if(door2 != NULL)
				door2->SetState(0);

			ParentClass::OnDied(mKiller);
		}

};

// Wrath-Scryer SoccothratesAI
// TO DO: Add moar sounds
#define CN_WRATH_SCRYER_SOCCOTHRATES 20886

#define IMMOLATION 35959 // DBC: 36051, 35959
#define FELFIRE_SHOCK 35759
#define FELFIRE_LINE_UP 35770	// ?
#define KNOCK_AWAY 20686 // DBC: 36512; but it uses it on himself too so changed to other
#define CHARGE 35754 // DBC: 36058, 35754 =( =(
// CHARGE_TARGETING 36038 ?
// There are more sounds connected with Dalliah and some spells, but I don't know situation in which they are used
// so haven't added them.

class WrathScryerSoccothratesAI : public MoonScriptBossAI
{
	public:
		MOONSCRIPT_FACTORY_FUNCTION(WrathScryerSoccothratesAI, MoonScriptBossAI);
		WrathScryerSoccothratesAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
		{
			AddSpell(IMMOLATION, Target_Self, 10.0f, 0, -1);
			AddSpell(FELFIRE_SHOCK, Target_Current, 8.0f, 0, -1);
			AddSpell(FELFIRE_LINE_UP, Target_Self, 8.0f, 0, -1);
			AddSpell(KNOCK_AWAY, Target_Destination, 6.0f, 0, -1);
			AddSpell(CHARGE, Target_Current, 4.0f, 0, -1);

			AddEmote(Event_OnCombatStart, "At last, a target for my frustrations!", Text_Yell, 11238);
			AddEmote(Event_OnTargetDied, "Yes, that was quiet... satisfying.", Text_Yell, 11239);
			AddEmote(Event_OnTargetDied, "Ha! Much better!", Text_Yell, 11240);
			AddEmote(Event_OnDied, "Knew this was... the only way out.", Text_Yell, 11243);
		}

		void OnDied(Unit* mKiller)
		{
			GameObject* door1 = NULL;
			door1 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(199.969f, 118.5837f, 22.379f, 184318);
			if(door1 != NULL)
				door1->SetState(0);

			ParentClass::OnDied(mKiller);
		}

};

// Harbinger SkyrissAI
// Full event must be scripted for this guy.
#define CN_HARBRINGER_SKYRISS 20912

#define MIND_REND 36924 // DBC: 36859, 36924;
#define FEAR 39415
#define DOMINATION 37162
#define SUMMON_ILLUSION_66 36931	// those 2 don't work
#define SUMMON_ILLUSION_33 36932
// BLINK_VISUAL 36937 ?
// SIMPLE_TELEPORT 12980 ?
// Add sounds related to his dialog with mind controlled guy

class HarbringerSkyrissAI : public MoonScriptBossAI
{
	public:
		MOONSCRIPT_FACTORY_FUNCTION(HarbringerSkyrissAI, MoonScriptBossAI);

		HarbringerSkyrissAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
		{
			AddSpell(MIND_REND, Target_Current, 15.0f, 0, -1);

			SpellDesc* Fear = AddSpell(FEAR, Target_Current, 8.0f, 0, -1);
			Fear->AddEmote("Flee in terror!", Text_Yell, 11129);
			Fear->AddEmote("I will show you horrors undreamed of.", Text_Yell, 11130);

			SpellDesc* Domination = AddSpell(DOMINATION, Target_Current, 6.0f, 0, -1);
			Domination->AddEmote("You will do my bidding, weakling.", Text_Yell, 11127);
			Domination->AddEmote("Your will is no longer your own.", Text_Yell, 11128);

			Illusion66 = AddSpell(SUMMON_ILLUSION_66, Target_Self, 0, 0, -1, 0, 0, false, "", Text_Yell, 11131);
			Illusion66->mEnabled = false;

			Illusion33 = AddSpell(SUMMON_ILLUSION_33, Target_Self, 0, 0, -1, 0, 0, false, "",  Text_Yell, 11131);
			Illusion33->mEnabled = false;

			AddEmote(Event_OnCombatStart, "Bear witness to the agent of your demise!", Text_Yell, 11123);
			AddEmote(Event_OnTargetDied, "Your fate is written!", Text_Yell, 11124);
			AddEmote(Event_OnTargetDied, "The chaos I have sown here is but a taste....", Text_Yell, 11125);
			AddEmote(Event_OnDied, "I am merely one of... infinite multitudes.", Text_Yell, 11126);

			IllusionCount = 0;
		}

		void OnCombatStart(Unit* mTarget)
		{
			IllusionCount = 0;
			ParentClass::OnCombatStart(mTarget);
		}

		void AIUpdate()
		{
			if(GetHealthPercent() <= 66 && IllusionCount == 0)
			{
				IllusionCount = 1;
				CastSpell(Illusion66);
			}
			else if(GetHealthPercent() <= 33 && IllusionCount == 1)
			{
				IllusionCount = 2;
				CastSpell(Illusion33);
			}
			ParentClass::AIUpdate();
		}


	protected:

		uint32 IllusionCount;
		SpellDesc* Illusion66;
		SpellDesc* Illusion33;
};

#define CN_WARDEN_MELLICHAR 20904

#define BLAZING_TRICKSTER 20905
#define WARP_STALKER 20906
#define AKKIRIS_LIGHTNING_WAKER 20908
#define SULFURON_MAGMA_THROWER 20909
#define TWILIGHT_DRAKONAAR 20910
#define BLACKWING_DRAKONAAR 20911
#define MILLHOUSE_MANASTORM 20977

class WardenMellicharAI : public MoonScriptBossAI
{
	public:
		MOONSCRIPT_FACTORY_FUNCTION(WardenMellicharAI, MoonScriptBossAI);
		WardenMellicharAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
		{
			SetCanMove(false);
			Phase_Timer = -1;
			SetPhase(0);
			Spawncounter = 0;
			NPC_orb1 = NULL;
			NPC_orb2 = NULL;
			NPC_orb3 = NULL;
			NPC_orb4 = NULL;
			NPC_orb5 = NULL;
			shield = NULL;
			orb1 = NULL;
			orb2 = NULL;
			orb3 = NULL;
			orb4 = NULL;
		}

		void OnCombatStart(Unit* mTarget)
		{
			SetPhase(0);
			Phasepart = 0;
			SetCanMove(false);
			Phase_Timer = AddTimer(55000);

			SetCanEnterCombat(false);
			_unit->SetEmoteState(EMOTE_ONESHOT_READY1H); // to be replaced for the standstate

			shield = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(445.786f, -169.263f, 43.0466f, 184802);
			if(shield)
				shield->SetState(1);

			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I knew the prince would be angry but, I... I have not been myself. I had to let them out! The great one speaks to me, you see. Wait--outsiders. Kael'thas did not send you! Good... I'll just tell the prince you released the prisoners!");
			_unit->PlaySoundToSet(11222);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "The naaru kept some of the most dangerous beings in existence here in these cells. Let me introduce you to another...", 27000);
			sEventMgr.AddEvent(TO_OBJECT(_unit), &Object::PlaySoundToSet, (uint32)11223, EVENT_UNK, 27000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

			ParentClass::OnCombatStart(mTarget);
		}

		void OnCombatStop(Unit* mTarget)
		{
			Reset_Event();
			ParentClass::OnCombatStop(mTarget);
		}

		void AIUpdate()
		{
			SetCanEnterCombat(false);
			SetCanMove(false);
			SetAllowMelee(false);
			SetAllowSpell(false);

			// ORB ONE
			if(IsTimerFinished(Phase_Timer) && GetPhase() == 0)
			{
				if(Phasepart == 0)
				{
					Spawncounter = 0;
					orb1 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(482.929f, -151.114f, 43.654f, 183961);
					if(orb1)
						orb1->SetState(0);

					switch(RandomUInt(1))
					{
							NPC_ID_Spawn = 0;
						case 0:
							NPC_ID_Spawn = BLAZING_TRICKSTER;
							break;
						case 1:
							NPC_ID_Spawn = WARP_STALKER;
							break;
					}
					ResetTimer(Phase_Timer, 8000);
					Phasepart = 1;
					return;
				}

				else if(Phasepart == 1)
				{
					if(!NPC_orb1 && NPC_ID_Spawn != 0 && Spawncounter == 0)
					{
						++Spawncounter;
						NPC_orb1 = SpawnCreature(NPC_ID_Spawn, 475.672f, -147.086f, 42.567f, 3.184015f);
						return;
					}
					else if(NPC_orb1 && !NPC_orb1->IsAlive())
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Yes, yes... another! Your will is mine! Behold another terrifying creature of incomprehensible power!");
						_unit->PlaySoundToSet(11224);
						SetPhase(1);
						Phasepart = 0;
						ResetTimer(Phase_Timer, 6000);
						return;
					}
					else
					{
						return;
					}
					return;
				}
				//return;
			}

			// ORB TWO
			else if(IsTimerFinished(Phase_Timer) && GetPhase() == 1)
			{
				if(Phasepart == 0)
				{
					Spawncounter = 0;
					orb2 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(409.062f, -152.161f, 43.653f, 183963);
					if(orb2)
						orb2->SetState(0);

					ResetTimer(Phase_Timer, 8000);
					Phasepart = 1;
					return;
				}

				else if(Phasepart == 1)
				{
					if(!NPC_orb2 && Spawncounter == 0)
					{
						++Spawncounter;
						NPC_orb2 = SpawnCreature(MILLHOUSE_MANASTORM, 413.192f, -148.586f, 42.569f, 0.024347f);
						return;
					}
					else if(NPC_orb2 && NPC_orb2->IsAlive())
					{
						Unit* millhouse = TO_UNIT(ForceCreatureFind(MILLHOUSE_MANASTORM));
						if(millhouse)
						{
							millhouse->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Where in Bonzo's brass buttons am I? And who are-- yaaghh, that's one mother of a headache!", 2000);
							sEventMgr.AddEvent(TO_OBJECT(millhouse), &Object::PlaySoundToSet, (uint32)11171, EVENT_UNK, 2000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "What is this? A lowly gnome? I will do better, oh great one.", 13000);
							sEventMgr.AddEvent(TO_OBJECT(_unit), &Object::PlaySoundToSet, (uint32)11226, EVENT_UNK, 13000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

							millhouse->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Lowly? Nobody refers to the mighty Millhouse Manastorm as lowly! I have no idea what goes on here, but I will gladly join your fight against this impudent imbecile!", 22000);
							sEventMgr.AddEvent(TO_OBJECT(millhouse), &Object::PlaySoundToSet, (uint32)11172, EVENT_UNK, 22000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
						}
						SetPhase(2);
						Phasepart = 0;
						ResetTimer(Phase_Timer, 25000);
						return;
					}
					else
					{
						return;
					}
					return;

				}
				//return;
			}

			// ORB THREE
			else if(IsTimerFinished(Phase_Timer) && GetPhase() == 2)
			{
				if(Phasepart == 0)
				{
					Spawncounter = 0;
					orb3 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(415.167f, -174.338f, 43.654f, 183964);
					if(orb3)
						orb3->SetState(0);

					switch(RandomUInt(1))
					{
							NPC_ID_Spawn = 0;
						case 0:
							NPC_ID_Spawn = SULFURON_MAGMA_THROWER;
							break;
						case 1:
							NPC_ID_Spawn = AKKIRIS_LIGHTNING_WAKER;
							break;
					}
					ResetTimer(Phase_Timer, 8000);
					Phasepart = 1;
					return;
				}

				else if(Phasepart == 1)
				{
					if(!NPC_orb3 && NPC_ID_Spawn != 0 && Spawncounter == 0)
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "1");
						++Spawncounter;
						NPC_orb3 = SpawnCreature(NPC_ID_Spawn, 420.050f, -173.500f, 42.580f, 6.110f);
						return;
					}
					else if(!NPC_orb3)
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "2");
						NPC_orb3 = GetNearestCreature(NPC_ID_Spawn);
					}
					else if(NPC_orb3 && !NPC_orb3->IsAlive())
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Anarchy! Bedlam! Oh, you are so wise! Yes, I see it now, of course!");
						_unit->PlaySoundToSet(11227);
						SetPhase(3);
						Phasepart = 0;
						ResetTimer(Phase_Timer, 8000);
						return;
					}
					else
					{
						return;
					}
					return;
				}
				//return;
			}

			// ORB FOUR
			else if(IsTimerFinished(Phase_Timer) && GetPhase() == 3)
			{
				if(Phasepart == 0)
				{
					Spawncounter = 0;
					orb4 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(476.422f, -174.517f, 42.748f, 183962);
					if(orb4)
						orb4->SetState(0);

					switch(RandomUInt(1))
					{
							NPC_ID_Spawn = 0;
						case 0:
							NPC_ID_Spawn = TWILIGHT_DRAKONAAR;
							break;
						case 1:
							NPC_ID_Spawn = BLACKWING_DRAKONAAR;
							break;
					}
					ResetTimer(Phase_Timer, 8000);
					Phasepart = 1;
					return;
				}

				else if(Phasepart == 1)
				{
					if(!NPC_orb4 && NPC_ID_Spawn != 0 && Spawncounter == 0)
					{
						++Spawncounter;
						NPC_orb4 = SpawnCreature(NPC_ID_Spawn, 471.153f, -174.715f, 42.589f, 3.097f);
						return;
					}
					else if(!NPC_orb4)
					{
						NPC_orb4 = GetNearestCreature(NPC_ID_Spawn);
					}
					else if(NPC_orb4 && !NPC_orb4->IsAlive())
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Yes, O great one, right away!");
						_unit->PlaySoundToSet(11228);
						SetPhase(4);
						Phasepart = 0;
						ResetTimer(Phase_Timer, 6000);
						return;
					}
					else
					{
						return;
					}
					return;
				}
				//return;
			}

			else if(IsTimerFinished(Phase_Timer) && GetPhase() == 4)
			{

			}

			ParentClass::AIUpdate();
			SetCanMove(false);
			SetAllowMelee(false);
			SetAllowSpell(false);

		}

		void Reset_Event()
		{
			SetCanMove(true);
			SetAllowMelee(true);
			SetAllowSpell(true);
			_unit->SetEmoteState(8); // to be replaced for the standstate

			if(shield)
				shield->SetState(0);

			if(orb1)
				orb1->SetState(1);

			if(orb2)
				orb2->SetState(1);

			if(orb3)
				orb3->SetState(1);

			if(orb4)
				orb4->SetState(1);

			if(NPC_orb1)
			{
				NPC_orb1->Despawn(0);
				NPC_orb1 = NULL;
			}

			if(NPC_orb2)
			{
				NPC_orb2->Despawn(0);
				NPC_orb2 = NULL;
			}

			if(NPC_orb3)
			{
				NPC_orb3->Despawn(0);
				NPC_orb3 = NULL;
			}

			if(NPC_orb4)
			{
				NPC_orb4->Despawn(0);
				NPC_orb4 = NULL;
			}

			if(NPC_orb5)
			{
				NPC_orb5->Despawn(0);
				NPC_orb5 = NULL;
			}

		}

	protected:

		uint32 Phasepart;
		uint32 NPC_ID_Spawn;
		uint32 Spawncounter;
		int32 Phase_Timer;

		MoonScriptCreatureAI*	NPC_orb1;
		MoonScriptCreatureAI*	NPC_orb2;
		MoonScriptCreatureAI*	NPC_orb3;
		MoonScriptCreatureAI*	NPC_orb4;
		MoonScriptCreatureAI*	NPC_orb5;
		GameObject* shield;
		GameObject* orb1;
		GameObject* orb2;
		GameObject* orb3;
		GameObject* orb4;


};

void SetupArcatraz(ScriptMgr* mgr)
{
	mgr->register_creature_script(CN_ZEREKETH, &ZerekethAI::Create);
	mgr->register_creature_script(CN_VOIDZONEARC, &VoidZoneARC::Create);

	mgr->register_creature_script(CN_DALLIAH_THE_DOOMSAYER, &DalliahTheDoomsayerAI::Create);
	mgr->register_creature_script(CN_WRATH_SCRYER_SOCCOTHRATES, &WrathScryerSoccothratesAI::Create);
	mgr->register_creature_script(CN_HARBRINGER_SKYRISS, &HarbringerSkyrissAI::Create);
	//mgr->register_creature_script(CN_WARDEN_MELLICHAR, &WardenMellicharAI::Create);
}
