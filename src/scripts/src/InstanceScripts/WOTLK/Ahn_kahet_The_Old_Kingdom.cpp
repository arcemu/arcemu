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

// Prince Taldaram
#define PrinceTaldaram	29308

// Spells
#define Bloodthirst	55968
#define ConjureFlameSphere	55931
#define EmbraceoftheVampyr	55959
#define Vanish			55964

class PrinceTaldaramAI : public ArcScriptBossAI
{
    ARCSCRIPT_FACTORY_FUNCTION(PrinceTaldaramAI, ArcScriptBossAI);
	PrinceTaldaramAI(Creature* pCreature) : ArcScriptBossAI(pCreature)
	{
		AddSpell(Bloodthirst, Target_Current, 15, 0, 10);
		ConjureSpell = AddSpell(ConjureFlameSphere, Target_Current, 0, 0, 0);
		VanishSpell = AddSpell(Vanish, Target_Self, 0, 0, 0);
		AddSpell(EmbraceoftheVampyr, Target_RandomPlayer, 15, 0, 10, 0, 0, false, "So appetizing.", Text_Yell);

		AddEmote(Event_OnCombatStart, "I will feast on your remains.", Text_Yell);
		AddEmote(Event_OnTargetDied, "I will drink no blood before it's time.", Text_Yell);
		AddEmote(Event_OnTargetDied, "One final embrace.", Text_Yell);
		AddEmote(Event_OnDied, "Still I hunger, still I thirst.", Text_Yell);

		VanishTimer = AddTimer(30000);
	}

	void AIUpdate()
	{
		if(IsTimerFinished(VanishTimer))
		{
		VanishTimer2 = AddTimer(3000);
		CastSpellNowNoScheduling(ConjureSpell);
		if(IsTimerFinished(VanishTimer2))
		{
		CastSpellNowNoScheduling(VanishSpell);
		}
		}
		ParentClass::AIUpdate();
	}

	uint32 VanishTimer2;
	uint32 VanishTimer;
	SpellDesc* VanishSpell;
	SpellDesc* ConjureSpell;

};
void SetupAhnkahetTheOldKingdom(ScriptMgr* pScriptMgr)
{
	pScriptMgr->register_creature_script(PrinceTaldaram, &PrinceTaldaramAI::Create);
}