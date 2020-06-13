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
/* Instance_RagefireChasm.cpp Script									*/
/************************************************************************/

/* TRASH MOBS */
// Ragefire Shaman AI
#define CN_RAGEFIRE_SHAMAN			11319
class RagefireShamanAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(RagefireShamanAI, MoonScriptCreatureAI);
		RagefireShamanAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			// Healing Wave
			AddSpell(11986, Target_WoundedFriendly, 15, 3, 10);
			// Lightning Bolt
			AddSpell(9532, Target_Current, 20, 3, 0);
		}
};

// Ragefire Trogg AI by Soulshifter
#define CN_RAGEFIRE_TROGG			11318
class RagefireTroggAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(RagefireTroggAI, MoonScriptCreatureAI);
		RagefireTroggAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			// Strike
			AddSpell(11976, Target_Current, 40, 0, 0);
		}
};

// Searing Blade Warlock AI by Soulshifter
#define CN_SEARING_BLADE_WARLOCK	11324
class SearingBladeWarlockAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(SearingBladeWarlockAI, MoonScriptCreatureAI);
		SearingBladeWarlockAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			// Shadow Bolt
			AddSpell(20791, Target_Current, 20, 3, 0);
			// Summon Voidwalker -- Doesnt work (Disabled for now)
			//CastSpellNowNoScheduling(AddSpell(12746, Target_Self, 0, 0, 0));
		}
};

#define CN_SEARING_BLADE_ENFORCER	11323
class SearingBladeEnforcerAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(SearingBladeEnforcerAI, MoonScriptCreatureAI);
		SearingBladeEnforcerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			// Shield Slam
			AddSpell(8242, Target_Current, 15, 0, 0);
		}
};

// Blade Cultist AI by Soulshifter
#define CN_BLADE_CULTIST			11322
class BladeCultistAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(BladeCultistAI, MoonScriptCreatureAI);
		BladeCultistAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			// Curse of Agony
			AddSpell(18266, Target_Current, 30, 0, 15);
		}
};

// Molten Elemental AI by Soulshifter
#define CN_MOLTEN_ELEMENTAL			11321
class MoltenElementalAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(MoltenElementalAI, MoonScriptCreatureAI);
		MoltenElementalAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			// Fire Shield
			AddSpell(134, Target_Self, 40, 1, 15);
		}
};

// Earthborer AI by Soulshifter
#define CN_EARTHBORER				11320
class EarthborerAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(EarthborerAI, MoonScriptCreatureAI);
		EarthborerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			// Earthborer Acid
			AddSpell(18070, Target_Current, 15, 0, 0);
		}
};

/* GAMEOBJECTS */
class BloodFilledOrb : public GameObjectAIScript
{
	public:
		BloodFilledOrb(GameObject*  goinstance) : GameObjectAIScript(goinstance) {}
		static GameObjectAIScript* Create(GameObject*  GO) { return new BloodFilledOrb(GO); }

		void OnActivate(Player*  pPlayer)
		{
			// Make sure player has the quest and Zelemar isn't spawned yet
			if(!pPlayer->HasQuest(9692)) // The Path of the Adept
			{
				pPlayer->GetSession()->SendNotification("Request quest `The Path of the Adept`.");
				return;
			}
			Creature* Zelemar = NULL;
			Zelemar = _gameobject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-370.133f, 162.519f, -21.1299f, 17830);
			if(Zelemar)
				return;

			// Spawn Zelemar the Wrathful
			Zelemar = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(17830, -370.133f, 162.519f, -21.1299f, -1.29154f, true, false, 0, 0);
			if(Zelemar)
			{
				Zelemar->m_noRespawn = true;
				Zelemar = NULL;
			}
		}
};

/* BOSSES */

//Oggleflint
#define CN_OGGLEFLINT	11517
class OggleflintAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(OggleflintAI, MoonScriptCreatureAI);
		OggleflintAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			// Cleave
			AddSpell(40505, Target_Current, 10, 0, 1);
		}
};

//Taragaman the Hungerer
#define CN_TARAGAMAN	11520
class TaragamanAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(TaragamanAI, MoonScriptCreatureAI);
		TaragamanAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			// Fire Nova
			AddSpell(11970, Target_Self, 10, 2, 0);
			// Uppercut
			AddSpell(18072, Target_Current, 10, 0, 0);
		}
};

//Jergosh The Invoker
#define CN_JERGOSH			11518
class JergoshAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(JergoshAI, MoonScriptCreatureAI);
		JergoshAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			// Immolate
			AddSpell(20800, Target_Current, 15, 2, 0);
			// Curse of Weakness
			AddSpell(18267, Target_Current, 10, 0, 0);
		}
};

//Bazzalan
#define CN_BAZZALAN	11519
class BazzalanAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(BazzalanAI, MoonScriptCreatureAI);
		BazzalanAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			// Sinister Strike
			AddSpell(14873, Target_Current, 15, 0, 0);
			// Poison (Correct ID?)
			AddSpell(18197, Target_Current, 5, 0, 0);
		}
};

void SetupRagefireChasm(ScriptMgr* mgr)
{
	/* TRASH MOBS */
	mgr->register_creature_script(CN_SEARING_BLADE_WARLOCK, &SearingBladeWarlockAI::Create);
	mgr->register_creature_script(CN_EARTHBORER, &EarthborerAI::Create);
	mgr->register_creature_script(CN_MOLTEN_ELEMENTAL, &MoltenElementalAI::Create);
	mgr->register_creature_script(CN_RAGEFIRE_SHAMAN, &RagefireShamanAI::Create);
	mgr->register_creature_script(CN_RAGEFIRE_TROGG, &RagefireTroggAI::Create);
	mgr->register_creature_script(CN_BLADE_CULTIST, &BladeCultistAI::Create);
	mgr->register_creature_script(CN_SEARING_BLADE_ENFORCER, &SearingBladeEnforcerAI::Create);

	/* GAMEOBJECTS */
	mgr->register_gameobject_script(182024, &BloodFilledOrb::Create);

	/* BOSSES */
	mgr->register_creature_script(CN_OGGLEFLINT, &OggleflintAI::Create);
	mgr->register_creature_script(CN_TARAGAMAN, &TaragamanAI::Create);
	mgr->register_creature_script(CN_JERGOSH, &JergoshAI::Create);
	mgr->register_creature_script(CN_BAZZALAN, &BazzalanAI::Create);
}