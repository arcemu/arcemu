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
/* Raid_GruulsLair.cpp Script											*/
/************************************************************************/

#define CN_LAIR_BRUTE				19389
#define LAIR_BRUTE_MORTALSTRIKE		39171
#define LAIR_BRUTE_CLEAVE			39174
#define LAIR_BRUTE_CHARGE			24193

void SpellFunc_LairBrute_Charge(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);

class LairBruteAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(LairBruteAI, MoonScriptCreatureAI);
		LairBruteAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			AddSpell(LAIR_BRUTE_CLEAVE, Target_Current, 20, 0, 15, 0, 7);
			AddSpell(LAIR_BRUTE_MORTALSTRIKE, Target_Current, 8, 0, 20, 0, 7);
			mCharge = AddSpell(LAIR_BRUTE_CHARGE, Target_Current, 0, 0, 0, 0, 40);
			AddSpellFunc(&SpellFunc_LairBrute_Charge, Target_Current, 7, 0, 35, 0, 40);
		}

		SpellDesc*	mCharge;
};

void SpellFunc_LairBrute_Charge(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType)
{
	LairBruteAI* pBruteAI = (pCreatureAI != NULL) ? TO< LairBruteAI* >(pCreatureAI) : NULL;
	if(pBruteAI != NULL)
	{
		Unit* pCurrentTarget = pBruteAI->GetUnit()->GetAIInterface()->getNextTarget();
		if(pCurrentTarget != NULL && pCurrentTarget != pTarget)
		{
			pBruteAI->GetUnit()->GetAIInterface()->AttackReaction(pTarget, 500);
			pBruteAI->GetUnit()->GetAIInterface()->setNextTarget(pTarget);
			pBruteAI->GetUnit()->GetAIInterface()->RemoveThreatByPtr(pCurrentTarget);
		}

		pBruteAI->CastSpell(pBruteAI->mCharge);
	}
}

#define CN_GRONN_PRIEST					21350
#define GRONN_PRIEST_PSYCHICSCREAM		22884
#define GRONN_PRIEST_RENEW				36679
#define GRONN_PRIEST_HEAL				36678

class GronnPriestAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(GronnPriestAI, MoonScriptCreatureAI);
		GronnPriestAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			AddSpell(GRONN_PRIEST_PSYCHICSCREAM, Target_Self, 8, 0, 20);
			AddSpell(GRONN_PRIEST_RENEW, Target_WoundedFriendly, 6, 0, 25, 0, 100);
			AddSpell(GRONN_PRIEST_HEAL, Target_WoundedFriendly, 5, 2, 30, 0, 100);
		}
};

#define CN_HIGH_KING_MAULGAR				18831
#define HIGH_KING_MAULGAR_BERSERKER_CHARGE	26561
#define HIGH_KING_MAULGAR_INTIMIDATING_ROAR	16508
#define HIGH_KING_MAULGAR_MIGHTY_BLOW		33230
#define HIGH_KING_MAULGAR_FLURRY			33232
#define HIGH_KING_MAULGAR_ARCING_SMASH		28168
#define HIGH_KING_MAULGAR_ARCING_SMASH2		39144
#define HIGH_KING_MAULGAR_WHIRLWIND			33238
#define HIGH_KING_MAULGAR_WHIRLWIND2		33239

void SpellFunc_Maulgar_Enrage(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);
// 4th unit sometimes cannot be found - blame cell system
uint32 Adds[4] = { 18832, 18834, 18836, 18835 };

class HighKingMaulgarAI : public MoonScriptBossAI
{
		MOONSCRIPT_FACTORY_FUNCTION(HighKingMaulgarAI, MoonScriptBossAI);
		HighKingMaulgarAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
		{
			AddPhaseSpell(2, AddSpell(HIGH_KING_MAULGAR_BERSERKER_CHARGE, Target_RandomPlayer, 10, 0, 25, 0, 40));
			AddPhaseSpell(2, AddSpell(HIGH_KING_MAULGAR_INTIMIDATING_ROAR, Target_Current, 7, 0, 20, 0, 5));
			AddSpell(HIGH_KING_MAULGAR_ARCING_SMASH, Target_Current, 8, 0, 15, 0, 10);
			AddSpell(HIGH_KING_MAULGAR_WHIRLWIND, Target_Self, 7, 15, 25);					// SpellFunc for range check?
			AddSpell(HIGH_KING_MAULGAR_MIGHTY_BLOW, Target_Current, 7, 0, 20, 0, 5);
			mEnrage = AddSpellFunc(&SpellFunc_Maulgar_Enrage, Target_Self, 0, 0, 0);
			mEnrage->AddEmote("You will not defeat the hand of Gruul!", Text_Yell, 11368);
			AddEmote(Event_OnCombatStart, "Gronn are the real power in Outland!", Text_Yell, 11367);
			AddEmote(Event_OnTargetDied, "You not so tough after all!", Text_Yell, 11373);
			AddEmote(Event_OnTargetDied, "Maulgar is king!", Text_Yell, 11375);
			AddEmote(Event_OnTargetDied, "", Text_Emote, 11374);
			AddEmote(Event_OnDied, "Grull... will crush you!", Text_Yell, 11376);

			mLastYell = -1;
			mAliveAdds = 0;
		}

		void OnCombatStart(Unit* pTarget)
		{
			SetDisplayWeapon(true, true);
			ParentClass::OnCombatStart(pTarget);

			mAliveAdds = 0;
			mLastYell = -1;
			for(int i = 0; i < 4; ++i)
			{
				Unit* pAdd = ForceCreatureFind(Adds[i]);
				if(pAdd != NULL && pAdd->isAlive())
				{
					Unit* pTarget = GetBestPlayerTarget();
					if(pTarget != NULL)
					{
						pAdd->GetAIInterface()->AttackReaction(pTarget, 200);
					}

					++mAliveAdds;
				}
			}
			if(mAliveAdds > 1)
			{
				SetCanEnterCombat(false);
				SetBehavior(Behavior_Spell);
				SetCanMove(false);
			}
		}

		void OnCombatStop(Unit* pTarget)
		{
			ParentClass::OnCombatStop(pTarget);
			SetCanEnterCombat(true);
		}

		void OnDied(Unit* mKiller)
		{
			ParentClass::OnDied(mKiller);

			GameObject* pDoor = mKiller->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(95.26f, 251.836f, 0.47f, 183817);
			if(pDoor != NULL)
			{
				pDoor->SetState(0);
			}
		}

		void AIUpdate()
		{
			if(mAliveAdds > 1)
				return;

			if(GetPhase() == 1 && GetHealthPercent() <= 50)
			{
				SetPhase(2, mEnrage);
			}

			ParentClass::AIUpdate();
		}

		void OnAddDied()
		{
			if(mAliveAdds > 0)
			{
				--mAliveAdds;
				if(mAliveAdds > 1)
				{
					uint32 RandomText = RandomUInt(1);
					while((int)RandomText == mLastYell)
					{
						RandomText = RandomUInt(1);
					}

					switch(RandomText)
					{
						case 0:
							Emote("You not kill next one so easy!", Text_Yell, 11369);
							break;
						case 1:
							Emote("Does not prove anything!", Text_Yell, 11370);
							break;
					}

					mLastYell = RandomText;
				}
				else if(mAliveAdds == 1)
				{
					Emote("Good, now you fight me!", Text_Yell, 0);
					SetCanEnterCombat(true);
					SetBehavior(Behavior_Default);
					SetCanMove(true);
				}
			}
		}

		uint32		mAliveAdds;
		int32		mLastYell;
		SpellDesc*	mEnrage;
};

void SpellFunc_Maulgar_Enrage(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType)
{
	HighKingMaulgarAI* pMaulgarAI = (pCreatureAI != NULL) ? TO< HighKingMaulgarAI* >(pCreatureAI) : NULL;
	if(pMaulgarAI != NULL)
	{
		pMaulgarAI->ApplyAura(HIGH_KING_MAULGAR_FLURRY);
		pMaulgarAI->SetDisplayWeapon(false, false);
	}
}

#define CN_KIGGLER_THE_CRAZED					18835
#define KIGGLER_THE_CRAZED_LIGHTNING_BOLT		36152
#define KIGGLER_THE_CRAZED_GREATER_POLYMORPH	33173
#define KIGGLER_THE_CRAZED_ARCANE_EXPLOSION		33237
#define	KIGGLER_THE_CRAZED_ARCANE_SHOCK			33175

class KigglerTheCrazedAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(KigglerTheCrazedAI, MoonScriptCreatureAI);
		KigglerTheCrazedAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			AddSpell(KIGGLER_THE_CRAZED_LIGHTNING_BOLT, Target_Current, 70, 2, 0, 0, 40);
			AddSpell(KIGGLER_THE_CRAZED_GREATER_POLYMORPH, Target_RandomPlayer, 8, 0, 15, 0, 30);	// Additional SpellFunc for removing target from target list if there are different targets?
			AddSpell(KIGGLER_THE_CRAZED_ARCANE_EXPLOSION, Target_Self, 8, 0, 20);
			AddSpell(KIGGLER_THE_CRAZED_ARCANE_SHOCK, Target_RandomPlayer, 10, 0, 15, 0, 30);
		}

		void OnCombatStart(Unit* pTarget)
		{
			ParentClass::OnCombatStart(pTarget);

			if(GetRangeToUnit(pTarget) <= 40.0f)
			{
				SetBehavior(Behavior_Spell);
				SetCanMove(false);
			}
		}

		void OnDied(Unit* mKiller)
		{
			ParentClass::OnDied(mKiller);
			Creature* pMaulgar = TO_CREATURE(ForceCreatureFind(CN_HIGH_KING_MAULGAR, 143.048996f, 192.725998f, -11.114700f));
			if(pMaulgar != NULL && pMaulgar->isAlive() && pMaulgar->GetScript())
			{
				HighKingMaulgarAI* pMaulgarAI = TO< HighKingMaulgarAI* >(pMaulgar->GetScript());
				pMaulgarAI->OnAddDied();
			}
		}

		void AIUpdate()
		{
			ParentClass::AIUpdate();

			Unit* pTarget = _unit->GetAIInterface()->getNextTarget();
			if(pTarget != NULL)
			{
				if(GetRangeToUnit(pTarget) <= 40.0f)
				{
					SetBehavior(Behavior_Spell);
					SetCanMove(false);
				}
			}
		}
};

#define CN_BLINDEYE_THE_SEER						18836
#define BLINDEYE_THE_SEER_PRAYER_OF_HEALING			33152
#define BLINDEYE_THE_SEER_GREAT_POWER_WORD_SHIELD	33147
#define BLINDEYE_THE_SEER_HEAL						33144

class BlindeyeTheSeerAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(BlindeyeTheSeerAI, MoonScriptCreatureAI);
		BlindeyeTheSeerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			AddSpell(BLINDEYE_THE_SEER_PRAYER_OF_HEALING, Target_Self, 5, 4, 30);				// Affects players? Core bugzor?
			AddSpell(BLINDEYE_THE_SEER_GREAT_POWER_WORD_SHIELD, Target_Self, 8, 0, 30);			// Strategies don't say anything about buffing friends
			AddSpell(BLINDEYE_THE_SEER_HEAL, Target_WoundedFriendly, 8, 1.5, 25);
		}

		void OnDied(Unit* mKiller)
		{
			ParentClass::OnDied(mKiller);
			Creature* pMaulgar = TO_CREATURE(ForceCreatureFind(CN_HIGH_KING_MAULGAR, 143.048996f, 192.725998f, -11.114700f));
			if(pMaulgar != NULL && pMaulgar->isAlive() && pMaulgar->GetScript())
			{
				HighKingMaulgarAI* pMaulgarAI = TO< HighKingMaulgarAI* >(pMaulgar->GetScript());
				pMaulgarAI->OnAddDied();
			}
		}
};

#define CN_OLM_THE_SUMMONER							18834
#define OLM_THE_SUMMONER_DEATH_COIL					33130
#define OLM_THE_SUMMONER_SUMMON_WILD_FELHUNTER		33131
#define OLM_THE_SUMMONER_DARK_DECAY					33129

class OlmTheSummonerAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(OlmTheSummonerAI, MoonScriptCreatureAI);
		OlmTheSummonerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			AddSpell(OLM_THE_SUMMONER_DEATH_COIL, Target_RandomPlayer, 7, 0, 10, 0, 30);
			AddSpell(OLM_THE_SUMMONER_SUMMON_WILD_FELHUNTER, Target_Self, 7, 3, 15);
			AddSpell(OLM_THE_SUMMONER_DARK_DECAY, Target_RandomPlayer, 10, 0, 6);
		}

		void OnDied(Unit* mKiller)
		{
			ParentClass::OnDied(mKiller);
			Creature* pMaulgar = TO_CREATURE(ForceCreatureFind(CN_HIGH_KING_MAULGAR, 143.048996f, 192.725998f, -11.114700f));
			if(pMaulgar != NULL && pMaulgar->isAlive() && pMaulgar->GetScript())
			{
				HighKingMaulgarAI* pMaulgarAI = TO< HighKingMaulgarAI* >(pMaulgar->GetScript());
				pMaulgarAI->OnAddDied();
			}
		}
};

#define CN_WILD_FEL_STALKER			18847
#define WILD_FEL_STALKER_WILD_BITE	33086

class WildFelStalkerAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(WildFelStalkerAI, MoonScriptCreatureAI);
		WildFelStalkerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			AddSpell(WILD_FEL_STALKER_WILD_BITE, Target_Current, 10, 0, 10, 0, 5);
			AggroRandomPlayer(200);
		}
};

#define CN_KROSH_FIREHAND      18832

/* He will first spellshield on himself, and recast every 30 sec,
   then spam great fireball to the target, also if there is any unit
   close to him (15yr) he'll cast blast wave
*/

#define GREAT_FIREBALL         33051
#define BALST_WAVE                     33061
#define SPELLSHIELD                    33054

class KroshFirehandAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(KroshFirehandAI, MoonScriptCreatureAI);
		KroshFirehandAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			//spells
			mBlastWave = AddSpell(BALST_WAVE, Target_Self, 0, 0, 0, 0, 15);
			AddSpell(GREAT_FIREBALL, Target_Current, 100, 3, 0, 0, 100);
			mSpellShield = AddSpell(SPELLSHIELD, Target_Self, 0, 0, 0);

			mEventTimer = AddTimer(30000);
			mBlastWaveTimer = -1;
			SetAIUpdateFreq(250);
		}

		void OnCombatStart(Unit* pTarget)
		{
			CastSpellNowNoScheduling(mSpellShield);

			ParentClass::OnCombatStart(pTarget);
		}

		void AIUpdate()
		{
			if(!IsCasting())
			{
				if(mBlastWaveTimer == -1 || IsTimerFinished(mBlastWaveTimer))
				{
					Unit* unit = GetBestUnitTarget(TargetFilter_Closest);
					if(unit && GetRangeToUnit(unit) < 15.0f)
					{
						CastSpellNowNoScheduling(mBlastWave);
						if(mBlastWaveTimer == -1)
							mBlastWaveTimer = AddTimer(6000);
						else
							ResetTimer(mBlastWaveTimer, 6000);
						ParentClass::AIUpdate();
						return;
					}
				}

				if(IsTimerFinished(mEventTimer))
				{
					ResetTimer(mEventTimer, 30000);
					CastSpellNowNoScheduling(mSpellShield);
				}
			}

			ParentClass::AIUpdate();
		}

		void OnDied(Unit* mKiller)
		{
			ParentClass::OnDied(mKiller);
			Creature* pMaulgar = TO_CREATURE(ForceCreatureFind(CN_HIGH_KING_MAULGAR, 143.048996f, 192.725998f, -11.114700f));
			if(pMaulgar != NULL && pMaulgar->isAlive() && pMaulgar->GetScript())
			{
				HighKingMaulgarAI* pMaulgarAI = TO< HighKingMaulgarAI* >(pMaulgar->GetScript());
				pMaulgarAI->OnAddDied();
			}
		}

		SpellDesc*      mSpellShield;
		SpellDesc*      mBlastWave;
		int32           mEventTimer;
		int32           mBlastWaveTimer;
};

#define CN_GRUUL_THE_DRAGONKILLER					19044
#define GRUUL_THE_DRAGONKILLER_GROWTH				36300	// +
#define GRUUL_THE_DRAGONKILLER_CAVE_IN				36240	// + 
#define GRUUL_THE_DRAGONKILLER_GROUND_SLAM			33525	// +
#define GRUUL_THE_DRAGONKILLER_GROUND_SLAM2			39187	// +
#define GRUUL_THE_DRAGONKILLER_SHATTER				33671	// does not make dmg - to script
#define GRUUL_THE_DRAGONKILLER_HURTFUL_STRIKE		33813	// +
#define GRUUL_THE_DRAGONKILLER_REVERBERATION		36297	// +
#define GRUUL_THE_DRAGONKILLER_STONED				33652	// +
#define GRUUL_THE_DRAGONKILLER_GRONN_LORDS_GRASP	33572	// Should be used only after Ground Slam

void SpellFunc_Gruul_GroundSlam(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);
void SpellFunc_Gruul_Stoned(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);
void SpellFunc_Gruul_Shatter(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);

class GruulTheDragonkillerAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(GruulTheDragonkillerAI, MoonScriptCreatureAI);
		GruulTheDragonkillerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			mHurtfulStrike = AddSpell(GRUUL_THE_DRAGONKILLER_HURTFUL_STRIKE, Target_Current, 0, 0, 0, 0, 8);
			mGroundSlam = AddSpell(GRUUL_THE_DRAGONKILLER_GROUND_SLAM, Target_Self, 0, 1, 0);
			mGroundSlam->AddEmote("Scurry.", Text_Yell, 11356);
			mGroundSlam->AddEmote("No escape.", Text_Yell, 11357);
			mGroundSlam2 = AddSpell(GRUUL_THE_DRAGONKILLER_GROUND_SLAM2, Target_Self, 0, 1, 0);
			mStoned = AddSpellFunc(&SpellFunc_Gruul_Stoned, Target_Self, 0, 2, 0);
			mShatter = AddSpellFunc(&SpellFunc_Gruul_Shatter, Target_Self, 0, 3, 0);
			mShatter2 = AddSpell(GRUUL_THE_DRAGONKILLER_SHATTER, Target_Self, 0, 1, 0);
			mShatter2->AddEmote("Stay...", Text_Yell, 11358);
			mShatter2->AddEmote("Beg for life.", Text_Yell, 11359);
			AddSpell(GRUUL_THE_DRAGONKILLER_REVERBERATION, Target_Self, 4, 0, 30);
			AddSpell(GRUUL_THE_DRAGONKILLER_CAVE_IN, Target_RandomPlayerDestination, 7, 0, 25);
			AddSpellFunc(&SpellFunc_Gruul_GroundSlam, Target_Self, 6, 1, 35);
			AddEmote(Event_OnCombatStart, "Come and die.", Text_Yell, 11355);
			AddEmote(Event_OnTargetDied, "No more.", Text_Yell, 11360);
			AddEmote(Event_OnTargetDied, "Unworthy.", Text_Yell, 11361);
			AddEmote(Event_OnTargetDied, "Die.", Text_Emote, 11362);
			AddEmote(Event_OnDied, "", Text_Yell, 11363);

			mGrowthTimer = mHurtfulTimer = -1;
			mGrowthStacks = 0;
		}

		void OnCombatStart(Unit* pTarget)
		{
			ParentClass::OnCombatStart(pTarget);

			mGrowthTimer = AddTimer(30000);
			mHurtfulTimer = AddTimer(8000);
			mGrowthStacks = 0;

			GameObject* pGate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(166.897f, 368.226f, 16.9209f, 184662);
			if(pGate != NULL)
				pGate->SetState(1);
		}

		void OnCombatStop(Unit* pTarget)
		{
			ParentClass::OnCombatStop(pTarget);

			GameObject* pGate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(166.897f, 368.226f, 16.9209f, 184662);
			if(pGate != NULL)
				pGate->SetState(0);
		}

		void OnDied(Unit* mKiller)
		{
			ParentClass::OnDied(mKiller);

			GameObject* pGate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(166.897f, 368.226f, 16.9209f, 184662);
			if(pGate != NULL)
				pGate->SetState(0);
		}

		void AIUpdate()
		{
			if(!IsCasting())
			{
				if(IsTimerFinished(mGrowthTimer))
				{
					if(mGrowthStacks == 30)
					{
						RemoveAura(GRUUL_THE_DRAGONKILLER_GROWTH);
						mGrowthStacks = 0;
					}
					if(mGrowthStacks != 29)
					{
						ResetTimer(mGrowthTimer, 30000);
					}
					else if(mGrowthStacks == 29)
					{
						ResetTimer(mGrowthTimer, 300000);
					}

					ApplyAura(GRUUL_THE_DRAGONKILLER_GROWTH);
					++mGrowthStacks;
				}
				else if(IsTimerFinished(mHurtfulTimer))
				{
					Unit* pCurrentTarget = _unit->GetAIInterface()->getNextTarget();
					if(pCurrentTarget != NULL)
					{
						Unit* pTarget = pCurrentTarget;
						for(set< Object* >::iterator itr = _unit->GetInRangePlayerSetBegin(); itr != _unit->GetInRangePlayerSetEnd(); ++itr)
						{
							Player* pPlayer = TO< Player* >(*itr);
							if(!pPlayer->isAlive())
								continue;
//						if (pPlayer->m_auracount[SPELL_AURA_MOD_INVISIBILITY])
//							continue;
							if(pPlayer->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_FEIGN_DEATH))
								continue;
							if(GetRangeToUnit(pPlayer) > 8.0f)
								continue;
							if(_unit->GetAIInterface()->getThreatByPtr(pPlayer) >= _unit->GetAIInterface()->getThreatByPtr(pCurrentTarget))
								continue;

							pTarget = TO_UNIT(pPlayer);
						}

						if(pTarget == pCurrentTarget)
							CastSpellNowNoScheduling(mHurtfulStrike);
						else
							_unit->CastSpell(pTarget, GRUUL_THE_DRAGONKILLER_HURTFUL_STRIKE, true);
					}

					ResetTimer(mHurtfulTimer, 8000);
				}
			}

			ParentClass::AIUpdate();
		}

		UnitArray GetInRangePlayers()
		{
			UnitArray TargetArray;
			for(set< Object* >::iterator itr = _unit->GetInRangePlayerSetBegin(); itr != _unit->GetInRangePlayerSetEnd(); ++itr)
			{
				if(IsValidUnitTarget(*itr, TargetFilter_None))
				{
					TargetArray.push_back(TO< Unit* >(*itr));
				}
			}

			return TargetArray;
		}

		uint32		mGrowthStacks;		// temporary way to store it
		int32		mHurtfulTimer;
		int32		mGrowthTimer;

		SpellDesc*	mHurtfulStrike;
		SpellDesc*	mGroundSlam;
		SpellDesc*	mGroundSlam2;
		SpellDesc*	mStoned;
		SpellDesc*	mShatter;
		SpellDesc*	mShatter2;
};

void SpellFunc_Gruul_GroundSlam(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType)
{
	GruulTheDragonkillerAI* pGruul = (pCreatureAI != NULL) ? TO< GruulTheDragonkillerAI* >(pCreatureAI) : NULL;
	if(pGruul != NULL)
	{
		pGruul->CastSpellNowNoScheduling(pGruul->mGroundSlam);
		pGruul->CastSpell(pGruul->mGroundSlam2);
		pGruul->CastSpell(pGruul->mStoned);
	}
}

void SpellFunc_Gruul_Stoned(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType)
{
	GruulTheDragonkillerAI* pGruul = (pCreatureAI != NULL) ? TO< GruulTheDragonkillerAI* >(pCreatureAI) : NULL;
	if(pGruul != NULL)
	{
		UnitArray TargetArray = pGruul->GetInRangePlayers();
		if(TargetArray.size() > 0)
		{
			for(size_t i = 0; i < TargetArray.size(); ++i)
			{
				Unit* pTarget = TargetArray[i];
				pTarget->CastSpell(pTarget, GRUUL_THE_DRAGONKILLER_STONED, true);
			}
		}

		pGruul->CastSpell(pGruul->mShatter);
	}
}

void SpellFunc_Gruul_Shatter(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType)
{
	GruulTheDragonkillerAI* pGruul = (pCreatureAI != NULL) ? TO< GruulTheDragonkillerAI* >(pCreatureAI) : NULL;
	if(pGruul != NULL)
	{
		pGruul->CastSpell(pGruul->mShatter2);	// Spell to script
		UnitArray TargetArray = pGruul->GetInRangePlayers();
		if(TargetArray.size() > 0)
		{
			for(size_t i = 0; i < TargetArray.size(); ++i)
			{
				Unit* pTarget = TargetArray[i];
				pTarget->RemoveAura(GRUUL_THE_DRAGONKILLER_STONED);
			}
		}
	}
}

void SetupGruulsLair(ScriptMgr* mgr)
{
	mgr->register_creature_script(CN_LAIR_BRUTE, &LairBruteAI::Create);
	mgr->register_creature_script(CN_GRONN_PRIEST, &GronnPriestAI::Create);
	mgr->register_creature_script(CN_KIGGLER_THE_CRAZED, &KigglerTheCrazedAI::Create);
	mgr->register_creature_script(CN_BLINDEYE_THE_SEER, &BlindeyeTheSeerAI::Create);
	mgr->register_creature_script(CN_OLM_THE_SUMMONER, &OlmTheSummonerAI::Create);
	mgr->register_creature_script(CN_WILD_FEL_STALKER, &WildFelStalkerAI::Create);
	mgr->register_creature_script(CN_KROSH_FIREHAND, &KroshFirehandAI::Create);
	mgr->register_creature_script(CN_HIGH_KING_MAULGAR,	&HighKingMaulgarAI::Create);
	mgr->register_creature_script(CN_GRUUL_THE_DRAGONKILLER, &GruulTheDragonkillerAI::Create);
}

