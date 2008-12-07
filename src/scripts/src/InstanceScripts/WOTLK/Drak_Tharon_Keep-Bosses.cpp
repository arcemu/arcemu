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

	// Initial import of my scripts - Bapes

#include "StdAfx.h"
#include "../Setup.h"
#include "../Base.h"


//* Trollgore *\\ 
#define TrollgoreEntry 26630

//* Spells *\\ 
#define Consume 49380 // http://wotlk.wowhead.com/?spell=49380
#define CorpseExplode 49555 // http://wotlk.wowhead.com/?spell=49555
#define Crush 49639 // http://wotlk.wowhead.com/?spell=49639
#define InfectedWound 49637 // http://wotlk.wowhead.com/?spell=49637

class TrollgoreAI : public ArcScriptBossAI
{
    ARCSCRIPT_FACTORY_FUNCTION(TrollgoreAI, ArcScriptBossAI);
	TrollgoreAI(Creature* pCreature) : ArcScriptBossAI(pCreature)
	{
		AddSpell(Consume, Target_Self, 14, 0, 37, 0, 50);
		//AddSpell(CorpseExplode, Target_ClosestCorpse, 59, 0, 57);  Not yet supported.
		AddSpell(Crush, Target_Current, 17, 0, 20);
		AddSpell(InfectedWound, Target_Current, 20, 0, 11);
	}
};


//* Novos the Summoner *\\ 
#define NovostheSummonerEntry 26631

//* Spells *\\ 
#define ArcaneBlast 49198 // http://wotlk.wowhead.com/?spell=49198
#define ArcaneField 47346 // http://wotlk.wowhead.com/?spell=47346
#define Blizzard 49034 // http://wotlk.wowhead.com/?spell=49034
#define Frostbolt 49037 // http://wotlk.wowhead.com/?spell=49037
#define WrathofMisery 50089 // http://wotlk.wowhead.com/?spell=50089

//* Notes *\\ 

//- Phase 1:
//Casts Arcane Field, making him immune to damage and inflicting two further effects on anyone within 10 yards. Slows movement speed by 50% and inflicts 1665 to 1935 Arcane damage every second.


//- Phase 2:
//He cannot be tanked and has no aggro list, he will cast spells randomly on the party.
//Casts Frostbolt. 1.5 second cast. Slows movement by 50% and hits for 2313 to 2687 Frost damage on random party members, much like Shade of Aran or High Astromancer Solarian.
//Casts Blizzard. 1.5 second cast. Slows movement speed by up to 75% and deals 1665-1935 Frost damage every two seconds for six seconds in a 10 yard area.
//Casts Wrath of Misery. 1 second cast. Curses a random target causing 1619 to 1881 Shadow damage every 2 seconds for 8 seconds.
//Casts Arcane Blast Instant cast. Inflicts 1749 to 1951 Arcane damage to a target and knocks them back 15 yards.


// TODO:  
// - Find adds
// - No agro table on phase two.

class NovostheSummonerAI : public ArcScriptBossAI
{
   ARCSCRIPT_FACTORY_FUNCTION(NovostheSummonerAI, ArcScriptBossAI);
	NovostheSummonerAI(Creature* pCreature) : ArcScriptBossAI(pCreature)
	{
		if (!_unit->HasAura(47346))
		{
		_unit->Root();
		_unit->WipeHateList();
		_unit->WipeTargetList();
		_unit->GetAIInterface()->disable_melee = true;
		AddSpell(ArcaneBlast, Target_RandomPlayer, 90, 0, 9);
		AddSpell(Frostbolt, Target_RandomPlayer, 90, 0, 14);
		AddSpell(Blizzard, Target_Destination, 75, 0, 25);
		AddSpell(WrathofMisery, Target_RandomPlayer, 75, 0, 25);
		}
		AddEmote(Event_OnCombatStart, "The chill that you feel is the herald of your doom!", Text_Yell);
		AddEmote(Event_OnTargetDied, "Such is the fate of all who oppose the Lich King.", Text_Yell);
		AddEmote(Event_OnDied, "Your efforts... are in vain.", Text_Yell);

	}

	void OnCombatStart(Unit* pTarget)
	{
		_unit->Root();
		_unit->WipeHateList();
		_unit->WipeTargetList();
		_unit->GetAIInterface()->disable_melee = true;
		ApplyAura(ArcaneField);
		ParentClass::OnCombatStart(pTarget);
	}
};


//* The Prophet Tharon'ja *\\ 
#define TheProphetTharonjaEntry 26632

//* Spells *\\ 
#define CurseofLife 49527 // http://wotlk.wowhead.com/?spell=49527
#define DecayFlesh 49356 // http://wotlk.wowhead.com/?spell=49356
#define EyeBeam 49544 // http://wotlk.wowhead.com/?spell=49544
#define LightningBreath 49537 // http://wotlk.wowhead.com/?spell=49537
#define PoisonCloud 49548 // http://wotlk.wowhead.com/?spell=49548
#define ReturnFlesh 53463 // http://wotlk.wowhead.com/?spell=53463
#define ShadowVolley 49528 // http://wotlk.wowhead.com/?spell=49528

// TODO:  
// - Phase Two
class TheProphetTharonjaAI : public ArcScriptBossAI
{
    ARCSCRIPT_FACTORY_FUNCTION(TheProphetTharonjaAI, ArcScriptBossAI);
	TheProphetTharonjaAI(Creature* pCreature) : ArcScriptBossAI(pCreature)
	{
		
		AddPhaseSpell(1, AddSpell(ShadowVolley, Target_Current, 70, 0, 17));
		AddPhaseSpell(1, AddSpell(CurseofLife, Target_RandomPlayer, 90, 0, 25));
		AddPhaseSpell(1, AddSpell(LightningBreath, Target_RandomPlayer, 47, 0, 35, 0, 30));

		AddPhaseSpell(2, AddSpell(EyeBeam, Target_RandomPlayer, 87, 0, 14));
		AddPhaseSpell(2, AddSpell(PoisonCloud, Target_Destination, 60, 0, 30));

		AddEmote(Event_OnCombatStart, "Tharon'ja sees all! The work of mortals shall not end the eternal dynasty!", Text_Yell);
		AddEmote(Event_OnTargetDied, "As Tharon'ja predicted.", Text_Yell);
		AddEmote(Event_OnTargetDied, "As it was written.", Text_Yell);
		AddEmote(Event_OnDied, "Im... impossible! Tharon'ja is eternal! Tharon'ja... is...", Text_Yell);

	}

};

void SetupDrakTharonKeepBosses(ScriptMgr* pScriptMgr)
{
	pScriptMgr->register_creature_script(TrollgoreEntry, &TrollgoreAI::Create);
	pScriptMgr->register_creature_script(NovostheSummonerEntry, &NovostheSummonerAI::Create);
	pScriptMgr->register_creature_script(TheProphetTharonjaEntry, &TheProphetTharonjaAI::Create);
}