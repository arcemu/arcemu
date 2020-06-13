/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009-2010 ArcEmu Team <http://www.arcemu.org/>
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

/////////////////////////////////////////////////////////////////////////////////
////// Halls of Lightning Instance
#define MAP_HALLS_OF_LIGHTNING		602
#define CN_GENERAL_BJARNGRIM		28586
#define CN_VOLKHAN					28587
#define CN_LOKEN					28923
#define CN_IONAR					28546

#define GO_GENERAL_DOORS			191416
#define GO_VOLKHAN_DOORS			191325
#define GO_LOKEN_DOORS				191324
#define GO_IONAR_DOORS1				191326
#define GO_IONAR_DOORS2				191328

class HallsOfLightningScript : public MoonInstanceScript
{
	public:
		uint32		mGeneralGUID;
		uint32		mVolkhanGUID;
		uint32		mLokenGUID;
		uint32		mIonarGUID;

		uint32		mGeneralDoorsGUID;
		uint32		mVolkhanDoorsGUID;
		uint32		mLokenDoorsGUID;
		uint32		mIonarDoors1GUID;
		uint32		mIonarDoors2GUID;

		MOONSCRIPT_INSTANCE_FACTORY_FUNCTION(HallsOfLightningScript, MoonInstanceScript);
		HallsOfLightningScript(MapMgr* pMapMgr) : MoonInstanceScript(pMapMgr)
		{
			mGeneralGUID = 0;
			mVolkhanGUID = 0;
			mLokenGUID = 0;
			mIonarGUID = 0;

			mGeneralDoorsGUID = 0;
			mVolkhanDoorsGUID = 0;
			mLokenDoorsGUID = 0;
			mIonarDoors1GUID = 0;
			mIonarDoors2GUID = 0;
		};

		void OnCreaturePushToWorld(Creature* pCreature)
		{
			switch(pCreature->GetEntry())
			{
				case CN_GENERAL_BJARNGRIM:
					{
						mGeneralGUID = pCreature->GetLowGUID();
						mEncounters.insert(EncounterMap::value_type(CN_GENERAL_BJARNGRIM, BossData(0, mGeneralGUID)));
					}
					break;
				case CN_VOLKHAN:
					{
						mVolkhanGUID = pCreature->GetLowGUID();
						mEncounters.insert(EncounterMap::value_type(CN_VOLKHAN, BossData(0, mVolkhanGUID)));
					}
					break;
				case CN_LOKEN:
					{
						mLokenGUID = pCreature->GetLowGUID();
						mEncounters.insert(EncounterMap::value_type(CN_LOKEN, BossData(0, mLokenGUID)));
					}
					break;
				case CN_IONAR:
					{
						mIonarGUID = pCreature->GetLowGUID();
						mEncounters.insert(EncounterMap::value_type(CN_IONAR, BossData(0, mIonarGUID)));
					}
					break;
			};
		};

		void OnGameObjectPushToWorld(GameObject* pGameObject)
		{
			switch(pGameObject->GetEntry())
			{
				case GO_GENERAL_DOORS:
					mGeneralDoorsGUID = pGameObject->GetLowGUID();
					break;
				case GO_VOLKHAN_DOORS:
					mVolkhanDoorsGUID = pGameObject->GetLowGUID();
					break;
				case GO_LOKEN_DOORS:
					mLokenDoorsGUID = pGameObject->GetLowGUID();
					break;
				case GO_IONAR_DOORS1:
					mIonarDoors1GUID = pGameObject->GetLowGUID();
					break;
				case GO_IONAR_DOORS2:
					mIonarDoors2GUID = pGameObject->GetLowGUID();
					break;
			}

			ParentClass::OnGameObjectPushToWorld(pGameObject);
		};

		void SetInstanceData(uint32 pType, uint32 pIndex, uint32 pData)
		{
			EncounterMap::iterator Iter = mEncounters.find(pIndex);
			if(Iter == mEncounters.end())
				return;

			(*Iter).second.mState = (EncounterState)pData;
		};

		uint32 GetInstanceData(uint32 pType, uint32 pIndex)
		{
			if(pType != Data_EncounterState || pIndex == 0)
				return 0;

			EncounterMap::iterator Iter = mEncounters.find(pIndex);
			if(Iter == mEncounters.end())
				return 0;

			return (*Iter).second.mState;
		};

		void OnCreatureDeath(Creature* pVictim, Unit* pKiller)
		{
			EncounterMap::iterator Iter = mEncounters.find(pVictim->GetEntry());
			if(Iter == mEncounters.end())
				return;

			(*Iter).second.mState = State_Finished;

			GameObject* pDoors = NULL;
			switch(pVictim->GetEntry())
			{
				case CN_GENERAL_BJARNGRIM:
					{
						SetInstanceData(Data_EncounterState, CN_GENERAL_BJARNGRIM, State_Finished);
						pDoors = GetGameObjectByGuid(mGeneralDoorsGUID);
						if(pDoors)
							pDoors->SetState(0);
					}
					break;
				case CN_VOLKHAN:
					{
						SetInstanceData(Data_EncounterState, CN_VOLKHAN, State_Finished);
						pDoors = GetGameObjectByGuid(mVolkhanDoorsGUID);
						if(pDoors)
							pDoors->SetState(0);
					}
					break;
				case CN_LOKEN:
					{
						SetInstanceData(Data_EncounterState, CN_LOKEN, State_Finished);
						pDoors = GetGameObjectByGuid(mLokenDoorsGUID);
						if(pDoors)
							pDoors->SetState(0);
					}
					break;
				case CN_IONAR:
					{
						SetInstanceData(Data_EncounterState, CN_IONAR, State_Finished);
						pDoors = GetGameObjectByGuid(mIonarDoors1GUID);
						if(pDoors)
							pDoors->SetState(0);

						pDoors = GetGameObjectByGuid(mIonarDoors2GUID);
						if(pDoors)
							pDoors->SetState(0);
					}
					break;
			};
		};

};

/////////////////////////////////////////////////////////////////////////////////
////// General Bjarnrim Script

// Spells
#define SPELL_ARC_WELD				59085
#define SPELL_BATTLE_AURA			41106
#define SPELL_BATTLE_STANCE			53792
#define SPELL_BERSERKER_AURA		41107
#define SPELL_BERSERKER_STANCE		53791
#define SPELL_CHARGE_UP				52098
#define SPELL_CLEAVE				15284
#define SPELL_DEFENSIVE_AURA		41105
#define SPELL_DEFENSIVE_STANCE		53790
#define SPELL_INTERCEPT				58769
#define SPELL_IRONFORM				52022
#define SPELL_KNOCK_AWAY			52029
#define SPELL_MORTAL_STRIKE			16856
#define SPELL_PUMMEL				12555
#define SPELL_SLAM					52026
#define SPELL_SPELL_REFLECTION		36096
#define SPELL_WHIRLWIND				52027

#define TIMER_STANCE_CHANGE			18000

enum GENERAL_STANCES
{
    STANCE_BATTLE		= 1,
    STANCE_BERSERKER	= 2,
    STANCE_DEFENSIVE	= 3,
};

class GeneralBjarngrimAI : public MoonScriptBossAI
{
		MOONSCRIPT_FACTORY_FUNCTION(GeneralBjarngrimAI, MoonScriptBossAI);
		GeneralBjarngrimAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
		{
			mInstance = GetInstanceScript();
			// Battle Stance
			AddPhaseSpell(1, AddSpell(SPELL_MORTAL_STRIKE, Target_Current, 25, 0, 5));
			AddPhaseSpell(1, AddSpell(SPELL_WHIRLWIND, Target_Self, 90, 8, 30));

			// Berserker Stance
			AddPhaseSpell(2, AddSpell(SPELL_CLEAVE, Target_Current, 30, 0, 5));

			// Defensive Stance
			AddPhaseSpell(3, AddSpell(SPELL_SPELL_REFLECTION, Target_Self, 20, 0, 10));
			AddPhaseSpell(3, AddSpell(SPELL_INTERCEPT, Target_RandomPlayerNotCurrent, 40, 0, 6));
			AddPhaseSpell(3, AddSpell(SPELL_PUMMEL, Target_Current, 40, 0, 5));

			AddEmote(Event_OnCombatStart, "I am the greatest of my father's sons! Your end has come!", Text_Yell, 14149);
			AddEmote(Event_OnTargetDied, "So ends your curse!", Text_Yell, 14153);
			AddEmote(Event_OnTargetDied, "Flesh... is... weak!", Text_Yell, 14154);
			AddEmote(Event_OnDied, "How can it be...? Flesh is not... stronger!", Text_Yell, 14156);

			mStanceTimer = INVALIDATE_TIMER;
		};

		void OnCombatStart(Unit* pTarget)
		{
			mStanceTimer = AddTimer(TIMER_STANCE_CHANGE + (RandomUInt(7) * 1000));
			switchStance(RandomUInt(2));

			ParentClass::OnCombatStart(pTarget);

			if(mInstance)
				mInstance->SetInstanceData(Data_EncounterState, _unit->GetEntry(), State_InProgress);
		};

		void OnCombatStop(Unit* pTarget)
		{
			if(mInstance)
				mInstance->SetInstanceData(Data_EncounterState, _unit->GetEntry(), State_Performed);

			ParentClass::OnCombatStop(pTarget);
		};

		void AIUpdate()
		{
			if(IsTimerFinished(mStanceTimer))
			{
				switch(GetPhase())
				{
					case STANCE_BATTLE:
						switchStance(RandomUInt(1) + 2);
						break;
					case STANCE_BERSERKER:
						if(RandomUInt(1) == 1)
							switchStance(STANCE_BATTLE);
						else
							switchStance(STANCE_DEFENSIVE);
						break;
					case STANCE_DEFENSIVE:
						switchStance(RandomUInt(1) + 1);
						break;
				};


				ResetTimer(mStanceTimer, TIMER_STANCE_CHANGE + (RandomUInt(7) * 1000));
			};

			ParentClass::AIUpdate();
		};

		void switchStance(int32 pStance)
		{
			switch(pStance)
			{
				case STANCE_BATTLE:
					ApplyAura(SPELL_BATTLE_AURA);
					ApplyAura(SPELL_BATTLE_STANCE);
					Emote("Defend yourself, for all the good it will do!", Text_Yell, 14151);
					Announce("General Bjarngrim switches to Battle Stance!");
					SetPhase(1);
					break;
				case STANCE_BERSERKER:
					ApplyAura(SPELL_BERSERKER_AURA);
					ApplyAura(SPELL_BERSERKER_STANCE);
					Emote("GRAAAAAH! Behold the fury of iron and steel! ", Text_Yell, 14152);
					Announce("General Bjarngrim switches to Berserker Stance!");
					SetPhase(2);
					break;
				case STANCE_DEFENSIVE:
					ApplyAura(SPELL_DEFENSIVE_AURA);
					ApplyAura(SPELL_DEFENSIVE_STANCE);
					Emote("Give me your worst! ", Text_Yell, 14150);
					Announce("General Bjarngrim switches to Defensive Stance!");
					SetPhase(3);
					break;
			}
		};

	private:
		int32		        mStanceTimer;
		MoonInstanceScript* mInstance;
};

/////////////////////////////////////////////////////////////////////////////////
////// Volkhan Script

/*static Location MoltenGolemCoords[]=
{
	{ 1335.296265f, -89.237503f, 56.717800f, 1.994538f },
	{ 1340.615234f, -89.083313f, 56.717800f, 0.028982f },
};*/

#define CN_MOLTEN_GOLEM				28695
#define CN_BRITTLE_GOLEM			28681
#define CN_VOLKHANS_ANVIL			28823

// Main Spells
#define SPELL_HEAT					HeroicInt( 52237, 59529 )
#define SPELL_SHATTERING_STOMP		HeroicInt( 52237, 59529 )
#define SPELL_TEMPER				52238

// Molten Golem Spells
#define SPELL_BLAST_WAVE			23113
#define SPELL_IMMOLATION_STRIKE		HeroicInt( 52433, 59530 )
#define SPELL_SHATTER				HeroicInt( 52429, 59527 )
// 24 seconds + up to 6
#define TIMER_STOMP					24000

class Volkhan : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(Volkhan, MoonScriptCreatureAI);
		Volkhan(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			mInstance = GetInstanceScript();

			AddSpell(SPELL_HEAT, Target_WoundedFriendly, 15, 1.5f, 15);
			mStomp = AddSpell(SPELL_SHATTERING_STOMP, Target_Self, 0, 3, 0);

			mStomp->AddEmote("I will crush you beneath my boots!", Text_Yell, 13963);
			mStomp->AddEmote("All my work... undone!", Text_Yell, 13964);

			AddEmote(Event_OnCombatStart, "It is you who have destroyed my children? You... shall... pay!", Text_Yell, 13960);
			AddEmote(Event_OnTargetDied, "The armies of iron will conquer all!", Text_Yell, 13965);
			AddEmote(Event_OnTargetDied, "Ha, pathetic!", Text_Yell, 13966);
			AddEmote(Event_OnTargetDied, "You have cost me too much work!", Text_Yell, 13967);
			AddEmote(Event_OnDied, "The master was right... to be concerned.", Text_Yell, 13968);

			m_cVolkhanWP.x = 1328.666870f;
			m_cVolkhanWP.y = -97.022758f;
			m_cVolkhanWP.z = 56.675297f;
			m_cVolkhanWP.o = 2.235341f;

			SetMoveType(Move_DontMoveWP);
			AddWaypoint(CreateWaypoint(1, 0, Flag_Run, m_cVolkhanWP));
			mStompTimer = INVALIDATE_TIMER;
			mPhase = 0;
		}

		void OnCombatStart(Unit* pTarget)
		{
			m_bStomp = false;
			mStompTimer = AddTimer(TIMER_STOMP + (RandomUInt(6) * 1000));
			mPhase = 0;

			ParentClass::OnCombatStart(pTarget);

			if(mInstance)
				mInstance->SetInstanceData(Data_EncounterState, _unit->GetEntry(), State_InProgress);
		};

		void OnCombatStop(Unit* pTarget)
		{
			if(mInstance)
				mInstance->SetInstanceData(Data_EncounterState, _unit->GetEntry(), State_Performed);

			ParentClass::OnCombatStop(pTarget);
		};

		void AIUpdate()
		{
			if(IsTimerFinished(mStompTimer))
			{
				if(m_bStomp == false)
				{
					m_bStomp = true;
					Announce("Volkhan prepares to shatter his Brittle Golems!");
					CastSpellNowNoScheduling(mStomp);
					ResetTimer(mStompTimer, 3000);
				}
				else
				{
					DoStomp();
					ResetTimer(mStompTimer, TIMER_STOMP + (RandomUInt(6) * 1000));
				}
			};

			if(GetHealthPercent() <= (100 - (20 * mPhase)))
			{
				ForceWaypointMove(1);
				Announce("Volkhan runs to his anvil!");
				++mPhase;
			}

			ParentClass::AIUpdate();
		};

		void OnReachWP(uint32 iWaypointId, bool bForwards)
		{
			if(iWaypointId == 1)
			{
				switch(RandomUInt(2))
				{
					case 0:
						Emote("Life from the lifelessness... death for you.",			Text_Yell, 13961);
						break;
					case 1:
						Emote("Nothing is wasted in the process. You will see....",	Text_Yell, 13962);
						break;
				};

				MoonScriptCreatureAI* pAnvil = GetNearestCreature(CN_VOLKHANS_ANVIL);
				if(pAnvil)
					_unit->CastSpell(pAnvil->GetUnit(), SPELL_TEMPER, true);
				else
					_unit->CastSpell(GetUnit(), SPELL_TEMPER, true);

				SetCanEnterCombat(true);
				_unit->GetAIInterface()->AttackReaction(GetNearestPlayer() , 1);   // hackfix
			};
		};

		void DoStomp()
		{
			for(set< Object* >::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr)
			{
				if((*itr) && (*itr)->IsCreature() && (*itr)->GetEntry() == CN_BRITTLE_GOLEM)
				{
					Creature* pCreature = TO< Creature* >((*itr));
					pCreature->CastSpell(pCreature, SPELL_SHATTER, true);

					pCreature->Despawn(1000, 0);
				};
			};

			m_bStomp = false;
		};

		SpellDesc*	        mTemper;
		SpellDesc*	        mStomp;
		Location	        m_cVolkhanWP;
		bool		        m_bStomp;
		int32		        mStompTimer;
		int32               mPhase;
		MoonInstanceScript* mInstance;
};

class MoltenGolem : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(MoltenGolem, MoonScriptCreatureAI);
		MoltenGolem(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			AddSpell(SPELL_BLAST_WAVE, Target_Self, 25, 0, 20);
			AddSpell(SPELL_IMMOLATION_STRIKE, Target_Current, 15, 0, 15);
		};

		void OnDied(Unit* pKiller)
		{
			SpawnCreature(CN_BRITTLE_GOLEM);
			Despawn(1);
		}
};

class BrittleGolem : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(BrittleGolem, MoonScriptCreatureAI);
		BrittleGolem(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			SetCanEnterCombat(false);
			SetCanMove(false);
		};
};

class VolkhansAnvil : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(VolkhansAnvil, MoonScriptCreatureAI);
		VolkhansAnvil(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
			SetCanMove(false);
		};

};



/////////////////////////////////////////////////////////////////////////////////
////// Ionar
// Status: Basic script, missing spark phase

#define CN_SPARK				28926

#define DISPRESE				52770
#define BALL_LIGHTNING			HeroicInt( 52780, 59800 )
#define STATIC_OVERLOAD			HeroicInt( 52658, 59795 )
#define SPELL_SUMMON_SPARK		52746

#define ARCING_BURN				HeroicInt( 52671, 59834 )

class IonarAI : public MoonScriptBossAI
{
		MOONSCRIPT_FACTORY_FUNCTION(IonarAI, MoonScriptBossAI);
		IonarAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
		{
			mInstance = GetInstanceScript();

			AddSpell(BALL_LIGHTNING, Target_RandomPlayerNotCurrent, 20, 1.5f, 5);
			AddSpell(STATIC_OVERLOAD, Target_RandomPlayerNotCurrent, 15, 0, 12);

			AddEmote(Event_OnCombatStart, "You wish to confront the master? You must weather the storm!", Text_Yell, 14453);
			AddEmote(Event_OnTargetDied, "Shocking, I know", Text_Yell, 14456);
			AddEmote(Event_OnTargetDied, "You attempt the impossible", Text_Yell, 14457);
			AddEmote(Event_OnTargetDied, "Your spark of life is..extinguished!", Text_Yell, 14458);
			AddEmote(Event_OnDied, "Master... you have guests.", Text_Yell, 14459);
		};

		void OnCombatStart(Unit* pTarget)
		{
			ParentClass::OnCombatStart(pTarget);

			if(mInstance)
				mInstance->SetInstanceData(Data_EncounterState, _unit->GetEntry(), State_InProgress);
		};

		void OnCombatStop(Unit* pTarget)
		{
			ParentClass::OnCombatStop(pTarget);

			if(mInstance)
				mInstance->SetInstanceData(Data_EncounterState, _unit->GetEntry(), State_Performed);
		};

		MoonInstanceScript* mInstance;
};

/////////////////////////////////////////////////////////////////////////////////
////// Loken
#define PULSING_SHOCKWAVE		HeroicInt( 52961, 59836 )
#define PULSING_SHOCKWAVE_AURA	59414
#define ARC_LIGHTNING			52921
#define LIGHTNING_NOVA			HeroicInt( 52960, 59835 )
// 14 seconds + random up to 8
#define TIMER_NOVA				14000
#define TIMER_RESPOND			18000

class LokenAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(LokenAI, MoonScriptCreatureAI);
		LokenAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			mInstance = GetInstanceScript();
			mNova = AddSpell(LIGHTNING_NOVA, Target_Self, 0, 4.0f, 0);
			AddSpell(ARC_LIGHTNING, Target_RandomPlayer, 25, 0, 6);

			Emote("I have witnessed the rise and fall of empires. The birth and extinction of entire species. Over countless millennia the foolishness of mortals has remained the only constant. Your presence here confirms this.", Text_Yell, 14160);

			AddEmote(Event_OnCombatStart, "What hope is there for you? None!", Text_Yell, 14162);
			AddEmote(Event_OnTargetDied, "Only mortal...", Text_Yell, 14166);
			AddEmote(Event_OnTargetDied, "I... am... FOREVER!", Text_Yell, 14167);
			AddEmote(Event_OnTargetDied, "What little time you had, you wasted!", Text_Yell, 14168);
			AddEmote(Event_OnDied, "My death... heralds the end of this world.", Text_Yell, 14172);

			mNovaTimer = INVALIDATE_TIMER;
			mRespondTimer = AddTimer(TIMER_RESPOND);
			RegisterAIUpdateEvent(1000);
			mSpeech = 1;
		};

		void OnCombatStart(Unit* pTarget)
		{
			ParentClass::OnCombatStart(pTarget);
			mSpeech = 1;
			ApplyAura(PULSING_SHOCKWAVE);
			mNovaTimer = AddTimer(TIMER_NOVA);
			CastOnAllInrangePlayers(PULSING_SHOCKWAVE_AURA);

			if(mInstance)
				mInstance->SetInstanceData(Data_EncounterState, _unit->GetEntry(), State_InProgress);
		};

		void OnCombatStop(Unit* pTarget)
		{
			RemoveAuraOnPlayers(PULSING_SHOCKWAVE_AURA);
			ParentClass::OnCombatStop(pTarget);

			if(mInstance)
				mInstance->SetInstanceData(Data_EncounterState, _unit->GetEntry(), State_Performed);
		};

		void OnDied(Unit* pKiller)
		{
			RemoveAuraOnPlayers(PULSING_SHOCKWAVE_AURA);
			ParentClass::OnDied(pKiller);
		};

		void AIUpdate()
		{
			if(IsTimerFinished(mNovaTimer))
			{
				switch(RandomUInt(2))
				{
					case 0:
						Emote("You cannot hide from fate!",			Text_Yell,	14163);
						break;
					case 1:
						Emote("Come closer. I will make it quick.",	Text_Yell,	14164);
						break;
					case 2:
						Emote("Your flesh cannot hold out for long.",	Text_Yell,	14165);
						break;
				};

				Announce("Loken begins to cast Lightning Nova!");
				CastSpellNowNoScheduling(mNova);
				ResetTimer(mNovaTimer, TIMER_NOVA + (RandomUInt(8) * 1000));
			};

			if(mSpeech == 4)
				return;

			if(GetHealthPercent() <= (100 - (25 * mSpeech)))
			{
				switch(mSpeech)
				{
					case 1:
						Emote("You stare blindly into the abyss!",	                                    Text_Yell, 14169);
						break;
					case 2:
						Emote("Your ignorance is profound. Can you not see where this path leads?",    Text_Yell, 14170);
						break;
					case 3:
						Emote("You cross the precipice of oblivion!",                                  Text_Yell, 14171);
						break;
						++mSpeech;
				};
			};

			if(IsTimerFinished(mRespondTimer))
			{
				Emote("My master has shown me the future, and you have no place in it. Azeroth will be reborn in darkness. Yogg-Saron shall be released! The Pantheon shall fall!", Text_Yell, 14161);
				RemoveTimer(mRespondTimer);
				RemoveAIUpdateEvent();
			};

			ParentClass::AIUpdate();
		};

		SpellDesc*	mNova;
		MoonInstanceScript* mInstance;

		int32		mNovaTimer;
		int32		mRespondTimer;
		uint8		mSpeech;
};

void SetupHallsOfLightning(ScriptMgr* mgr)
{
	mgr->register_instance_script(MAP_HALLS_OF_LIGHTNING, &HallsOfLightningScript::Create);

	mgr->register_creature_script(CN_GENERAL_BJARNGRIM, &GeneralBjarngrimAI::Create);

	mgr->register_creature_script(CN_VOLKHAN, &Volkhan::Create);
	mgr->register_creature_script(CN_MOLTEN_GOLEM, &MoltenGolem::Create);
	mgr->register_creature_script(CN_BRITTLE_GOLEM, &BrittleGolem::Create);
	mgr->register_creature_script(CN_VOLKHANS_ANVIL, &VolkhansAnvil::Create);

	mgr->register_creature_script(CN_IONAR, &IonarAI::Create);

	mgr->register_creature_script(CN_LOKEN, &LokenAI::Create);
}