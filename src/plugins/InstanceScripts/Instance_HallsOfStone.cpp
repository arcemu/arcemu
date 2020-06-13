/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009 ArcEmu Team <http://www.arcemu.org/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
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

//Dark Rune Stormcaller
#define CN_DR_STORMCALLER 27984

#define STORMCALLER_LIGHTNINGBOLT 12167
#define STORMCALLER_SHADOWWORD 15654

class DarkRuneStormcallerAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(DarkRuneStormcallerAI, MoonScriptCreatureAI);
		DarkRuneStormcallerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			AddSpell(STORMCALLER_LIGHTNINGBOLT, Target_RandomPlayer, 60, 3, 6);
			AddSpell(STORMCALLER_SHADOWWORD, Target_RandomPlayer, 16, 0, 12);
		};

};

//Iron Golem Custodian
#define CN_GOLEM_CUSTODIAN 27985

#define CUSTODIAN_CRUSH_ARMOR 33661 //Suden armor?
#define CUSTODIAN_GROUND_SMASH 12734 //STUN

class IronGolemCustodianAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(IronGolemCustodianAI, MoonScriptCreatureAI);
		IronGolemCustodianAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			AddSpell(CUSTODIAN_CRUSH_ARMOR, Target_Current, 50, 0, 5);
			AddSpell(CUSTODIAN_GROUND_SMASH, Target_ClosestPlayer, 20, 0, 14);
		};

};

//Dark Rune Protector
#define CN_DR_PROTECTOR 27983

#define PROTECTOR_CHARGE 22120
#define PROTECTOR_CLAVE 42724

class DarkRuneProtectorAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(DarkRuneProtectorAI, MoonScriptCreatureAI);
		DarkRuneProtectorAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			AddSpell(PROTECTOR_CHARGE, Target_RandomPlayerNotCurrent, 20, 0, 14, 10);
			AddSpell(PROTECTOR_CLAVE, Target_Current, 35, 0, 8);
		};

};

//Lesser Air Elemental
#define CN_LASSER_AIR_ELEMENTAL 28384

#define ELEMENTAL_LIGHTNING_BOLT 15801

class LesserAirElementalAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(LesserAirElementalAI, MoonScriptCreatureAI);
		LesserAirElementalAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			AddSpell(ELEMENTAL_LIGHTNING_BOLT, Target_RandomPlayerNotCurrent, 20, 3, 14);
		};

};

//Dark Rune Worker
#define CN_DR_WORKER 27961

#define WORKER_ENRAGE 51499 //not really enrage :) 
#define WORKER_PIERCE_ARMOR 46202

class DarkRuneWorkerAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(DarkRuneWorkerAI, MoonScriptCreatureAI);
		DarkRuneWorkerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			AddSpell(WORKER_ENRAGE, Target_Self, 5, 0, 60, 10);
			AddSpell(WORKER_PIERCE_ARMOR, Target_Current, 35, 0, 45);
		};

};

//Dark Rune Warrior
#define CN_DR_WARRIOR 27960

#define WARRIOR_CLAVE 42724
#define WARRIOR_HEROIC_STRIKE 53395

class DarkRuneWarriorAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(DarkRuneWarriorAI, MoonScriptCreatureAI);
		DarkRuneWarriorAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			AddSpell(WARRIOR_CLAVE, Target_Current, 15, 0, 8);
			AddSpell(WARRIOR_HEROIC_STRIKE, Target_Current, 35, 0, 12);
		};

};

//Dark Rune Theurgist
#define CN_DR_THEURGIST 27963

#define THEURGIST_BLAST_WAVE 22424 //Cast on self 12466
#define THEURGIST_FIREBOLT 12466 //Random target?
#define THEURGIST_IRON_MIGHT 51484 //Cast on self, some kind of enrage.

class DarkRuneTheurgistAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(DarkRuneTheurgistAI, MoonScriptCreatureAI);
		DarkRuneTheurgistAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			AddSpell(THEURGIST_BLAST_WAVE, Target_Self, 22, 0, 25);
			AddSpell(THEURGIST_FIREBOLT, Target_RandomPlayer, 40, 3, 6);
			AddSpell(THEURGIST_IRON_MIGHT, Target_Self, 5, 0, 60);
		};

};

//Dark Rune Shaper
#define CN_DR_SHAPER 27965

#define SHAPER_RAY 51496 //Debuff

class DarkRuneShaperAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(DarkRuneShaperAI, MoonScriptCreatureAI);
		DarkRuneShaperAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			AddSpell(SHAPER_RAY, Target_RandomPlayer, 35, 1.5, 12);
		};

};

//Dark Rune Scholar
#define CN_DR_SCHOLAR 27964

#define SCHOLAR_SILANCE 51612 //not rly silance but something like it :)

class DarkRuneScholarAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(DarkRuneScholarAI, MoonScriptCreatureAI);
		DarkRuneScholarAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			AddSpell(SCHOLAR_SILANCE, Target_RandomPlayerNotCurrent, 35, 2.5, 12);
		};

};

//Dark Rune Giant
#define CN_DR_GIANT 27969

#define GIANT_FIST 51494 //also some kind of enrage
#define GIANT_STOMP 51493 //Knockback

class DarkRuneGiantAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(DarkRuneGiantAI, MoonScriptCreatureAI);
		DarkRuneGiantAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			AddSpell(GIANT_FIST, Target_Self, 3, 2, 40);
			AddSpell(GIANT_STOMP, Target_RandomPlayer, 35, 0, 14, 0, 10);
		};

};


//Raging Construct
#define CN_RAGING_CONSTRUCT 27970

#define RAGING_CLAVE 28168
#define RAGING_POTENT_JOLT 51819 // he should stack this in about every 6 seconds or something

class DarkRuneConstructAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(DarkRuneConstructAI, MoonScriptCreatureAI);
		DarkRuneConstructAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			AddSpell(RAGING_POTENT_JOLT, Target_Self, 95, 0, 8);
			AddSpell(RAGING_CLAVE, Target_Current, 30, 0, 9, 0, 10);
		};

};

//Lightning Construct
#define CN_LIGHTNING_CONSTRUCT 27972

#define LIGHTNING_CHAIN_LIGHTNING 52383
#define LIGHTNING_ELECTRICAL_OVERLOAD 52341 //explode?

class DarkLightningConstructAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(DarkLightningConstructAI, MoonScriptCreatureAI);
		DarkLightningConstructAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			AddSpell(LIGHTNING_ELECTRICAL_OVERLOAD, Target_Self, 5, 1.5, 14);
			AddSpell(LIGHTNING_CHAIN_LIGHTNING, Target_Current, 30, 3, 8, 0, 30);
		};

};

//Forged Iron Trogg
#define CN_FI_TRAGG 27979

#define TRAGG_SHOCK 50900

class ForgedIronTroggAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(ForgedIronTroggAI, MoonScriptCreatureAI);
		ForgedIronTroggAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			AddSpell(LIGHTNING_CHAIN_LIGHTNING, Target_RandomPlayer, 30, 2, 8, 0, 10);
		};

};

//Maiden of Grief
#define BOSS_MAIDEN_OF_GRIEF 27975

#define MAIDEN_PILLAR_OF_WOE 50761 //apply at long/min range (all in that void zone should get it )
#define MAIDEN_SHOCK_OF_SORROW 50760
#define MAIDEN_STORM_OF_GRIEF 50752

class MaidenOfGriefAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(MaidenOfGriefAI, MoonScriptCreatureAI);
		MaidenOfGriefAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			AddSpell(MAIDEN_STORM_OF_GRIEF, Target_RandomPlayerDestination, 100, 0, 19);
			AddSpell(MAIDEN_PILLAR_OF_WOE, Target_RandomPlayerNotCurrent, 30, 0, 8);
			mShock = AddSpell(MAIDEN_SHOCK_OF_SORROW, Target_Self, 20, 0, 18);
			mShock->AddEmote("So much lost time... that you'll never get back!", Text_Yell, 13492);

			//EMOTES
			AddEmote(Event_OnCombatStart, "You shouldn't have come...now you will die! ", Text_Yell, 13487);
			AddEmote(Event_OnTargetDied, "My burden grows heavier.", Text_Yell, 13488);
			AddEmote(Event_OnTargetDied, "This is your own fault!", Text_Yell, 13489);
			AddEmote(Event_OnTargetDied, "You had it coming!", Text_Yell, 13490);
			AddEmote(Event_OnTargetDied, "Why must it be this way?", Text_Yell, 13491);
			AddEmote(Event_OnDied, "I hope you all rot! I never...wanted...this.", Text_Yell, 13493);
		};

		void OnCombatStart(Unit* pTarget)
		{
			mShock->TriggerCooldown();
			ParentClass::OnCombatStart(pTarget);
		};

	protected:
		SpellDesc* mShock;
};

// Krystallus
#define BOSS_KRYSTALLUS 27977

#define KRYSTALLUS_BOULDER_TOSS 50843
#define KRYSTALLUS_SHATTER 50810
#define KRYSTALLUS_STOMP 50868

// some macros
#define STOMP_TIMER	    35000
#define SHATTER_TIMER	4500

class KrystallusAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(KrystallusAI, MoonScriptCreatureAI);
		KrystallusAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			AddSpell(KRYSTALLUS_BOULDER_TOSS, Target_ClosestPlayer, 35, 1, 4);
			mShatter = AddSpell(KRYSTALLUS_SHATTER, Target_Self, 0, 0, 0);
			mStomp = AddSpell(KRYSTALLUS_STOMP, Target_Self, 0, 0, 0);
			mShatter->AddEmote("Break.... you....", Text_Yell, 14178);

			//EMOTES
			AddEmote(Event_OnCombatStart, "Crush....", Text_Yell, 14176);
			AddEmote(Event_OnTargetDied, "Ha.... ha... hah....", Text_Yell, 14177);
			AddEmote(Event_OnDied, "Uuuuhhhhhhhhhh......", Text_Yell, 14179);

			mStompTimer = INVALIDATE_TIMER;
			mShatterTimer = INVALIDATE_TIMER;
		}

		void OnCombatStart(Unit* pTarget)
		{
			mStompTimer = AddTimer(STOMP_TIMER);
			ParentClass::OnCombatStart(pTarget);
		};

		void AIUpdate()
		{
			if(IsTimerFinished(mStompTimer))
			{
				CastSpellNowNoScheduling(mStomp);
				SetCanMove(false);
				ResetTimer(mStompTimer, (STOMP_TIMER + SHATTER_TIMER));
				mShatterTimer = AddTimer(SHATTER_TIMER);

			}
			if(IsTimerFinished(mShatterTimer))
			{
				CastSpellNowNoScheduling(mShatter);
				SetCanMove(true);
				RemoveTimer(mShatterTimer);
			}
		}

	protected:
		SpellDesc* mStomp;
		SpellDesc* mShatter;

		int32 mStompTimer;
		int32 mShatterTimer;
};


void SetupHallsOfStone(ScriptMgr* mgr)
{
	// Trash!
	mgr->register_creature_script(CN_DR_STORMCALLER, &DarkRuneStormcallerAI::Create);
	mgr->register_creature_script(CN_GOLEM_CUSTODIAN, &IronGolemCustodianAI::Create);
	mgr->register_creature_script(CN_DR_PROTECTOR, &DarkRuneProtectorAI::Create);
	mgr->register_creature_script(CN_LASSER_AIR_ELEMENTAL, &LesserAirElementalAI::Create);
	mgr->register_creature_script(CN_DR_WORKER, &DarkRuneWorkerAI::Create);
	mgr->register_creature_script(CN_DR_THEURGIST, &DarkRuneTheurgistAI::Create);
	mgr->register_creature_script(CN_DR_SHAPER, &DarkRuneShaperAI::Create);
	mgr->register_creature_script(CN_DR_SCHOLAR, &DarkRuneScholarAI::Create);
	mgr->register_creature_script(CN_DR_GIANT, &DarkRuneGiantAI::Create);
	mgr->register_creature_script(CN_RAGING_CONSTRUCT, &DarkRuneConstructAI::Create);
	mgr->register_creature_script(CN_LIGHTNING_CONSTRUCT, &DarkLightningConstructAI::Create);
	mgr->register_creature_script(CN_FI_TRAGG, &ForgedIronTroggAI::Create);

	// Bosses!
	mgr->register_creature_script(BOSS_MAIDEN_OF_GRIEF, &MaidenOfGriefAI::Create);
	mgr->register_creature_script(BOSS_KRYSTALLUS, &KrystallusAI::Create);
}