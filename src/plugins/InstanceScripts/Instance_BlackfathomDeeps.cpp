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

#define CN_LadySarevess 4831
class LadySarevessAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(LadySarevessAI, MoonScriptCreatureAI);
	LadySarevessAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddEmote(Event_OnCombatStart, "You should not be here! Slay them!", Text_Yell, 5799);

		AddSpell(8435, Target_Current, 10, 2, 0);	// Forked Lightning
		AddSpell(865, Target_Self, 15, 0, 25);		// Frost Nova
		AddSpell(246, Target_Current, 15, 0, 10);	// Slow
	}
};

#define CN_BaronAquanis 12876
class BaronAquanisAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(BaronAquanisAI, MoonScriptCreatureAI);
	BaronAquanisAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		// Frostbolt
		AddSpell(15043, Target_Current, 20, 3, 0);
		AggroNearestPlayer();
		SetDespawnWhenInactive(true);
	}
};

#define OBJ_FathomStone 177964
class FathomStone : public GameObjectAIScript
{
public:
	FathomStone(GameObject*  goinstance) : GameObjectAIScript(goinstance)
	{
		SpawnBaronAquanis = true;
	}

	static GameObjectAIScript* Create(GameObject*  GO) { return new FathomStone(GO); }

	void OnActivate(Player*  pPlayer)
	{
		if(pPlayer->IsTeamHorde() && SpawnBaronAquanis == true) // Horde
		{
			// Spawn Baron Aquanis
			_gameobject->GetMapMgr()->GetInterface()->SpawnCreature(CN_BaronAquanis, -782.021f, -63.5876f, -45.0935f, -2.44346f, true, false, 0, 0);
			SpawnBaronAquanis = false;
		}
	}

protected:
	bool SpawnBaronAquanis;
};

#define CN_TwilightLordKelris 4832
class KelrisAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(KelrisAI, MoonScriptCreatureAI);
	KelrisAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddEmote(Event_OnCombatStart, "Who dares disturb my meditation?!", Text_Yell, 5802);
		AddEmote(Event_OnTargetDied, "Dust to dust.", Text_Yell, 5803);

		AddSpell(8399, Target_RandomPlayer, 12, 1.3f, 0, 0, 0, false, "Sleep...", Text_Yell, 5804);	// Sleep
		AddSpell(15587, Target_Current, 16, 1.5f, 0);	// Mind Blast
	}
};

#define CN_Akumai 4829
class AkumaiAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(AkumaiAI, MoonScriptCreatureAI);
	AkumaiAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddEmote(Event_OnCombatStart, "Who dares disturb my meditation?!", Text_Yell); // Should this really be here?
		AddEmote(Event_OnTargetDied, "Dust to dust.", Text_Yell); // Should this really be here?

		AddSpell(3490, Target_Self, 12, 0, 0);	// Frenzied Rage
		AddSpell(3815, Target_Self, 16, 0, 45);	// Poison Cloud
	}
};

#define GS_Morridune 6729
class MorriduneGossip : public Arcemu::Gossip::Script
{
public:
	void OnHello(Object* pObject, Player*  plr)
	{
		Arcemu::Gossip::Menu menu(pObject->GetGUID(), 7247);

		if(plr->IsTeamAlliance())
			menu.AddItem(0, "Please Teleport me to Darnassus.", 1);
		menu.AddItem(0, "I wish to leave this horrible place", 2);
		menu.Send(plr);
	}

	void OnSelectOption(Object* pObject, Player* plr, uint32 Id, const char* Code)
	{
		switch(Id)
		{
			case 0: OnHello(pObject, plr);	break;
				// port to Darnassus
			case 1:	plr->SafeTeleport(1, 0, 9951.52f, 2280.32f, 1341.39f, 0); break;
				// Port to entrance
			case 2:	plr->SafeTeleport(1, 0, 4247.74f, 745.879f, -24.2967f, 4.36996f); break;
		}
	}
};

void SetupBlackfathomDeeps(ScriptMgr* mgr)
{
	mgr->register_creature_script(CN_LadySarevess, &LadySarevessAI::Create);
	mgr->register_creature_script(CN_BaronAquanis, &BaronAquanisAI::Create);
	mgr->register_gameobject_script(OBJ_FathomStone, &FathomStone::Create);
	mgr->register_creature_script(CN_TwilightLordKelris, &KelrisAI::Create);
	mgr->register_creature_script(CN_Akumai, &AkumaiAI::Create);
	mgr->register_creature_gossip(GS_Morridune, new MorriduneGossip);
}