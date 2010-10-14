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

/************************************************************************/
#define CN_Ghamoora 4887
class GhamooraAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(GhamooraAI, MoonScriptCreatureAI);
    GhamooraAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		// Trample
		AddSpell(5568, Target_Self, 12, 0, 0);
	}
};

#define CN_LadySarevess 4831
class LadySarevessAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(LadySarevessAI, MoonScriptCreatureAI);
    LadySarevessAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		// Forked Lightning
		AddSpell(8435, Target_Current, 10, 2, 0);
		// Frost Nova
		AddSpell(865, Target_Self, 15, 0, 25);
		// Slow
		AddSpell(246, Target_Current, 15, 0, 10);
		AddEmote(Event_OnCombatStart, "You should not be here! Slay them!", Text_Yell, 5799);
	}
};

#define CN_Gelihast 6243
class GelihastAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(GelihastAI, MoonScriptCreatureAI);
    GelihastAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		// Net
		AddSpell(6533, Target_RandomPlayer, 10, 0, 0);
	}
};

#define CN_LorgusJett 12902
class LorgusJettAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(LorgusJettAI, MoonScriptCreatureAI);
    LorgusJettAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		// Lightning Bolt
		AddSpell(12167, Target_Current, 20, 3, 0);
	}
};

#define CN_BaronAquanis 12876
class BaronAquanisAI : public MoonScriptCreatureAI
{
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
	FathomStone(GameObject*  goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject*  GO) { return new FathomStone(GO); }

	void OnActivate(Player*  pPlayer)
	{
		if(pPlayer->IsTeamHorde() && !BaronAquanis) // Horde
		{
			// Spawn Baron Aquanis
			_gameobject->GetMapMgr()->GetInterface()->SpawnCreature(CN_BaronAquanis, -782.021f, -63.5876f, -45.0935f, -2.44346f, true, false, 0, 0);
			BaronAquanis = true;

		}
	}

protected:
	bool BaronAquanis;
};

#define CN_TwilightLordKelris 4832
class KelrisAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(KelrisAI, MoonScriptCreatureAI);
    KelrisAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddEmote(Event_OnCombatStart, "Who dares disturb my meditation?!", Text_Yell, 5802);
		AddEmote(Event_OnTargetDied, "Dust to dust.", Text_Yell, 5803);

		// Sleep
		AddSpell(8399, Target_RandomPlayer, 12, 1.3f, 0, 0, 0, false, "Sleep...", Text_Yell, 5804);
		// Mind Blast
		AddSpell(15587, Target_Current, 16, 1.5f, 0);
	}
};

#define CN_Akumai 4829
class AkumaiAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AkumaiAI, MoonScriptCreatureAI);
    AkumaiAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddEmote(Event_OnCombatStart, "Who dares disturb my meditation?!", Text_Yell); // Should this really be here?
		AddEmote(Event_OnTargetDied, "Dust to dust.", Text_Yell); // Should this really be here?

		// Frenzied Rage
		AddSpell(3490, Target_Self, 12, 0, 0, 0, 0);
		// Poison Cloud
		AddSpell(3815, Target_Self, 16, 0, 45);
	}
};
#define GS_Morridune 6729
class MorriduneGossip : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player*  plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7247, plr);
        
		if(plr->IsTeamAlliance())
			Menu->AddItem( 0, "Please Teleport me to Darnassus.", 1);
		Menu->AddItem( 0, "I wish to leave this horrible place", 2);

        if(AutoSend)
            Menu->SendTo(plr);
    }

    void GossipSelectOption(Object* pObject, Player*  plr, uint32 Id, uint32 IntId, const char * Code)
    {
		switch (IntId)
		{
			case 0:
				GossipHello(pObject, plr, true);
			break;
			case 1:
				// port to Darnassus
				plr->SafeTeleport(1, 0, 9951.52f, 2280.32f, 1341.39f, 0);
			break;
			case 2:
				// Port to entrance
				plr->SafeTeleport(1, 0, 4247.74f, 745.879f, -24.2967f, 4.36996f);
			break;
		}
	}

};

void SetupBlackfathomDeeps(ScriptMgr * mgr)
{
	mgr->register_creature_script(CN_Ghamoora, &GhamooraAI::Create);
	mgr->register_creature_script(CN_LadySarevess, &LadySarevessAI::Create);
	mgr->register_creature_script(CN_Gelihast, &GelihastAI::Create);
	mgr->register_creature_script(CN_LorgusJett, &LorgusJettAI::Create);
	mgr->register_creature_script(CN_BaronAquanis, &BaronAquanisAI::Create);

	mgr->register_gameobject_script(OBJ_FathomStone, &FathomStone::Create);

	mgr->register_creature_script(CN_TwilightLordKelris, &KelrisAI::Create);
	mgr->register_creature_script(CN_Akumai, &AkumaiAI::Create);

	GossipScript * Morridune = new MorriduneGossip();
	mgr->register_gossip_script(GS_Morridune, Morridune);
}