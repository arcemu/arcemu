/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008 Arcemu Team
 * Copyright (C) 2007 Moon++ <http://www.moonplusplus.com/>
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

#include "StdAfx.h"
#include "../Setup.h"
#include "../Base.h"

// Meathook
#define Meathook			26529
#define ConstrictingChains	52696
#define DiseaseExpulsion	52666

class MeathookAI : public ArcScriptBossAI
{
    ARCSCRIPT_FACTORY_FUNCTION(MeathookAI, ArcScriptBossAI);
	MeathookAI(Creature* pCreature) : ArcScriptBossAI(pCreature)
	{
		AddEmote(Event_OnCombatStart, "New toys!", Text_Yell);
		AddEmote(Event_OnTargetDied, "Boring...", Text_Yell);
		AddEmote(Event_OnTargetDied, "Why you stop moving?", Text_Yell);
		AddEmote(Event_OnTargetDied, "Get up! Me not done!", Text_Yell);
		AddEmote(Event_OnDied, "This not fun...", Text_Yell);

		AddSpell(ConstrictingChains, Target_RandomPlayer, 10, 0, 15);
		AddSpell(DiseaseExpulsion, Target_ClosestPlayer, 5, 0, 6);
	}
};

// Salramm the Fleshcrafter 
#define SalrammtheFleshcrafter		26530
#define ExplodeGhoul				52480
#define ShadowBolt					15232
#define StealFlesh					52708
#define SummonGhouls				52451

class SalrammtheFleshcrafterAI : public ArcScriptBossAI
{
	ARCSCRIPT_FACTORY_FUNCTION(SalrammtheFleshcrafterAI, ArcScriptBossAI);
	SalrammtheFleshcrafterAI(Creature* pCreature) : ArcScriptBossAI(pCreature)
	{
		AddEmote(Event_OnCombatStart, "Ah, the entertainment has arrived!", Text_Yell);
		AddEmote(Event_OnTargetDied, "The fun is just beginning!", Text_Yell);
		AddEmote(Event_OnTargetDied, "Aah, quality materials!", Text_Yell);
		AddEmote(Event_OnTargetDied, "Don't worry, I'll make good use of you.", Text_Yell);
		AddEmote(Event_OnDied, "You only advance... the master's plan...", Text_Yell);

		AddSpell(ExplodeGhoul, Target_Self, 3, 0, 20, 0, 0, false, "Blood... destruction... EXHILARATING!", Text_Yell);
		AddSpell(ShadowBolt, Target_RandomPlayer, 15, 0, 5);
		AddSpell(StealFlesh, Target_RandomPlayer, 7, 0, 15, 0, 0, false, "I want a sample...", Text_Yell);
		AddSpell(SummonGhouls, Target_Self, 20, 0, 30, 0, 0, false, "Say hello to some friends of mine.", Text_Yell);
	}

};

// Chrono-Lord Epoch 
#define ChronoLordEpoch	26532
#define CurseofExertion	52772
#define TimeWarp		52766
#define WoundingStrike	52771

class ChronoLordEpochAI : public ArcScriptBossAI
{
	ARCSCRIPT_FACTORY_FUNCTION(ChronoLordEpochAI, ArcScriptBossAI);
	ChronoLordEpochAI(Creature* pCreature) : ArcScriptBossAI(pCreature)
	{
		AddEmote(Event_OnCombatStart, "We'll see about that, young prince.", Text_Yell);
		AddEmote(Event_OnTargetDied, "There is no future for you.", Text_Yell);
		AddEmote(Event_OnTargetDied, "This is the hour of our greatest triumph!", Text_Yell);
		AddEmote(Event_OnTargetDied, "You were destined to fail.", Text_Yell);

		AddSpell(CurseofExertion, Target_RandomPlayer, 17, 0, 6);
		AddSpell(TimeWarp, Target_ClosestPlayer, 10, 0, 20, 0, 0, false, "Tick tock, tick tock...", Text_Yell);
		AddSpell(WoundingStrike, Target_Current, 20, 0, 5);
	}

};

// Mal'Ganis 
#define MalGanis		26533
#define CarrionSwarm	52720
#define MindBlast		52722
#define Sleep			52721
#define VampiricTouch	52723

class MalGanisAI : public ArcScriptBossAI
{
	ARCSCRIPT_FACTORY_FUNCTION(MalGanisAI, ArcScriptBossAI);
	MalGanisAI(Creature* pCreature) : ArcScriptBossAI(pCreature)
	{
		AddEmote(Event_OnCombatStart, "This will be a fine test...Prince Arthas...", Text_Yell);
		AddEmote(Event_OnTargetDied, "My Onslaught will wash over the Lich King's forces!", Text_Yell);
		AddEmote(Event_OnTargetDied, "Your death is in vain, tiny mortal.", Text_Yell);
		AddEmote(Event_OnTargetDied, "Your time has come to an end.", Text_Yell);
		AddEmote(Event_OnDied, "Your journey has just begun, young prince...Gather your forces, and meet me in the arctic land of Northrend...It is there we shall settle the score between us...It is there that your true destiny will unfold...", Text_Yell);
		
		AddSpell(CarrionSwarm, Target_Self, 20, 0, 15);
		AddSpell(Sleep, Target_RandomPlayerNotCurrent, 8, 0, 7);
		AddSpell(MindBlast, Target_Current, 12, 0, 10);
		AddSpell(VampiricTouch, Target_Self, 5, 0, 30);
	}

	void AIUpdate()
	{
		if(GetHealthPercent() <= 2) // Will go back to city or something at 1%.
		{
			Emote("Your journey has just begun, young prince...Gather your forces, and meet me in the arctic land of Northrend...It is there we shall settle the score between us...It is there that your true destiny will unfold...", Text_Emote, 0);
			_unit->RemoveFromWorld(false, false);
			SetCanEnterCombat(false);
			SetAllowMelee(false);
			SetAllowRanged(false);
			SetAllowSpell(false);
			SetAllowTargeting(false);
			SetCanMove(false);
		}
		ParentClass::AIUpdate();
	}

};

void SetupTheCullingofStratholme(ScriptMgr* pScriptMgr)
{
	pScriptMgr->register_creature_script(Meathook, &MeathookAI::Create);
	pScriptMgr->register_creature_script(SalrammtheFleshcrafter, &SalrammtheFleshcrafterAI::Create);
	pScriptMgr->register_creature_script(ChronoLordEpoch, &ChronoLordEpochAI::Create);
	pScriptMgr->register_creature_script(MalGanis, &MalGanisAI::Create);
}