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
#include "Setup.h"
#ifdef WIN32
#pragma warning(disable:4305)		// warning C4305: 'argument' : truncation from 'double' to 'float'
#pragma warning(disable:4018)		// warning C4018: '<' : signed/unsigned mismatch
#endif

class tele_Orgrimarr_to_Undercity: public GameObjectAIScript // Orgrimarr to Undercity
{
public:
	tele_Orgrimarr_to_Undercity(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	void OnActivate(Player * pPlayer)
	{
		pPlayer->SafeTeleport(0, 0, 2066.594727, 284.810089, 97.031319, 1);
	}
	static GameObjectAIScript *Create(GameObject * GO) { return new tele_Orgrimarr_to_Undercity(GO); }
};
GameObjectAIScript * create_go20000000(GameObject * GO) { return new tele_Orgrimarr_to_Undercity(GO); }

class tele_Undercity_to_Orgrimarr: public GameObjectAIScript // Undercity to Orgrimarr
{
public:
	tele_Undercity_to_Orgrimarr(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	void OnActivate(Player * pPlayer)
	{
		pPlayer->SafeTeleport(1, 0, 1325.607544, -4641.119141, 54.085419, 1);
	}
	static GameObjectAIScript *Create(GameObject * GO) { return new tele_Undercity_to_Orgrimarr(GO); }
};
GameObjectAIScript * create_go20000001(GameObject * GO) { return new tele_Undercity_to_Orgrimarr(GO); }

class tele_Orgrimarr_to_Gromgol: public GameObjectAIScript // Orgrimarr to Grom'gol
	{
public:
	tele_Orgrimarr_to_Gromgol(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	void OnActivate(Player * pPlayer)
	{
		pPlayer->SafeTeleport(0, 0, -12444.510742, 225.886551, 31.814957, 1);
	}
	static GameObjectAIScript *Create(GameObject * GO) { return new tele_Orgrimarr_to_Gromgol(GO); }
};
GameObjectAIScript * create_go20000002(GameObject * GO) { return new tele_Orgrimarr_to_Gromgol(GO); }

class tele_Gromgol_to_Orgrimarr: public GameObjectAIScript // Grom'gol to Orgrimarr
{
public:
	tele_Gromgol_to_Orgrimarr(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	void OnActivate(Player * pPlayer)
	{
		pPlayer->SafeTeleport(1, 0, 1365.987061, -4644.096680, 54.071957, 1);
	}
	static GameObjectAIScript *Create(GameObject * GO) { return new tele_Gromgol_to_Orgrimarr(GO); }
};
GameObjectAIScript * create_go20000003(GameObject * GO) { return new tele_Gromgol_to_Orgrimarr(GO); }

class tele_Undercity_to_Gromgol: public GameObjectAIScript // Undercity to Grom'gol
{
public:
	tele_Undercity_to_Gromgol(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	void OnActivate(Player * pPlayer)
	{
		pPlayer->SafeTeleport(0, 0, -12411.027344, 212.190964, 31.314737, 1);
	}
	static GameObjectAIScript *Create(GameObject * GO) { return new tele_Undercity_to_Gromgol(GO); }
};
GameObjectAIScript * create_go20000004(GameObject * GO) { return new tele_Undercity_to_Gromgol(GO); }

class tele_Gromgol_to_Undercity: public GameObjectAIScript // Grom'gol to Undercity
{
public:
	tele_Gromgol_to_Undercity(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	void OnActivate(Player * pPlayer)
	{
		pPlayer->SafeTeleport(0, 0, 2058.663818, 242.936096, 99.767365, 1);
	}
	static GameObjectAIScript *Create(GameObject * GO) { return new tele_Gromgol_to_Undercity(GO); }
};
GameObjectAIScript * create_go20000005(GameObject * GO) { return new tele_Gromgol_to_Undercity(GO); }

class tele_Ratchet_to_BootyBay: public GameObjectAIScript // Ratchet to Booty Bay
{
public:
	tele_Ratchet_to_BootyBay(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	void OnActivate(Player * pPlayer)
	{
		pPlayer->SafeTeleport(0, 0, -14292.999023, 550.385437, 8.834858, 1);
	}
	static GameObjectAIScript *Create(GameObject * GO) { return new tele_Ratchet_to_BootyBay(GO); }
};
GameObjectAIScript * create_go20000006(GameObject * GO) { return new tele_Ratchet_to_BootyBay(GO); }

class tele_BootyBay_to_Ratchet: public GameObjectAIScript // Booty Bay to Ratchet
{
public:
	tele_BootyBay_to_Ratchet(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	void OnActivate(Player * pPlayer)
	{
	pPlayer->SafeTeleport(1, 0, -988.534302, -3827.509277, 5.643883, 1);
	}
	static GameObjectAIScript *Create(GameObject * GO) { return new tele_BootyBay_to_Ratchet(GO); }
};
GameObjectAIScript * create_go20000007(GameObject * GO) { return new tele_BootyBay_to_Ratchet(GO); }

class tele_Auberdine_to_Menathil : public GameObjectAIScript // Auberdine to Menathil Harbor
{
public:
	tele_Auberdine_to_Menathil(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	void OnActivate(Player * pPlayer)
	{
		pPlayer->SafeTeleport(0, 0, -3734.677490, -589.108459, 6.349339, 1);
	}
	static GameObjectAIScript *Create(GameObject * GO) { return new tele_Auberdine_to_Menathil(GO); }
};
GameObjectAIScript * create_go20000008(GameObject * GO) { return new tele_Auberdine_to_Menathil(GO); }

class tele_Menathil_to_Auberdine: public GameObjectAIScript // Menathil Harbor to Auberdine
{
public:
	tele_Menathil_to_Auberdine(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	void OnActivate(Player * pPlayer)
	{
		pPlayer->SafeTeleport(1, 0, 6422.081055, 816.562622, 5.587342, 1);
	}
	static GameObjectAIScript *Create(GameObject * GO) { return new tele_Menathil_to_Auberdine(GO); }
};
GameObjectAIScript * create_go20000009(GameObject * GO) { return new tele_Menathil_to_Auberdine(GO); }

class tele_Menathil_to_Theramore: public GameObjectAIScript // Menathil Harbor to Theramore Isle
{
public:
	tele_Menathil_to_Theramore(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	void OnActivate(Player * pPlayer)
	{
		pPlayer->SafeTeleport(1, 0, -3994.010986, -4730.822754, 4.917009, 1);
	}
	static GameObjectAIScript *Create(GameObject * GO) { return new tele_Menathil_to_Theramore(GO); }
};
GameObjectAIScript * create_go20000010(GameObject * GO) { return new tele_Menathil_to_Theramore(GO); }

class tele_Theramore_to_Menathil: public GameObjectAIScript // Theramore Isle to Menathil Harbor
{
public:
	tele_Theramore_to_Menathil(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	void OnActivate(Player * pPlayer)
	{
		pPlayer->SafeTeleport(0, 0, -3892.864258, -598.846680, 5.530023, 1);
	}
	static GameObjectAIScript *Create(GameObject * GO) { return new tele_Theramore_to_Menathil(GO); }
};
GameObjectAIScript * create_go20000011(GameObject * GO) { return new tele_Theramore_to_Menathil(GO); }

class tele_Auberdine_to_Azuremyst : public GameObjectAIScript // Auberdine to Azuremyst Isle
{
public:
	tele_Auberdine_to_Azuremyst(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	void OnActivate(Player * pPlayer)
	{
		pPlayer->SafeTeleport(530, 0, -4264.176270, -11331.436523, 5.437645, 1);
	}
	static GameObjectAIScript *Create(GameObject * GO) { return new tele_Auberdine_to_Azuremyst(GO); }
};
GameObjectAIScript * create_go20000012(GameObject * GO) { return new tele_Auberdine_to_Azuremyst(GO); }

class tele_Azuremyst_to_Auberdine: public GameObjectAIScript // Azuremyst Isle to Auberdine
{
public:
	tele_Azuremyst_to_Auberdine(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	void OnActivate(Player * pPlayer)
	{
		pPlayer->SafeTeleport(1, 0, 6543.706543, 926.260193, 6.056714, 1);
	}
	static GameObjectAIScript *Create(GameObject * GO) { return new tele_Azuremyst_to_Auberdine(GO); }
};
GameObjectAIScript * create_go20000013(GameObject * GO) { return new tele_Azuremyst_to_Auberdine(GO); }

class tele_Teldrassil_to_Auberdine: public GameObjectAIScript // Teldrassil to Auberdine
{
public:
	tele_Teldrassil_to_Auberdine(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	void OnActivate(Player * pPlayer)
	{
		pPlayer->SafeTeleport(1, 0, 8553.734375, 1021.275146, 5.642698, 1);
	}
	static GameObjectAIScript *Create(GameObject * GO) { return new tele_Teldrassil_to_Auberdine(GO); }
};
GameObjectAIScript * create_go20000014(GameObject * GO) { return new tele_Teldrassil_to_Auberdine(GO); }

class tele_Auberdine_to_Teldrassil: public GameObjectAIScript // Auberdine to Teldrassil
{
public:
	tele_Auberdine_to_Teldrassil(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	void OnActivate(Player * pPlayer)
	{
		pPlayer->SafeTeleport(1, 0, 6575.7773344, 765.997742, 5.741073, 1);
	}
	static GameObjectAIScript *Create(GameObject * GO) { return new tele_Auberdine_to_Teldrassil(GO); }
};
GameObjectAIScript * create_go20000015(GameObject * GO) { return new tele_Auberdine_to_Teldrassil(GO); }

class tele_Stormwind_to_Ironforge: public GameObjectAIScript // Teldrassil to Auberdine
{
public:
	tele_Stormwind_to_Ironforge(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	void OnActivate(Player * pPlayer)
	{
		pPlayer->SafeTeleport(369, 0, 18.794186, 2489.258789, -4.292023, 1);
	}
	static GameObjectAIScript *Create(GameObject * GO) { return new tele_Stormwind_to_Ironforge(GO); }
};
GameObjectAIScript * create_go20000016(GameObject * GO) { return new tele_Stormwind_to_Ironforge(GO); }

class tele_Ironforge_to_Stormwind: public GameObjectAIScript // Auberdine to Teldrassil
{
public:
	tele_Ironforge_to_Stormwind(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	void OnActivate(Player * pPlayer)
	{
		pPlayer->SafeTeleport(369, 0, 15.849360, 8.222371, -4.296173, 1);
	}
	static GameObjectAIScript *Create(GameObject * GO) { return new tele_Ironforge_to_Stormwind(GO); }
};
GameObjectAIScript * create_go20000017(GameObject * GO) { return new tele_Ironforge_to_Stormwind(GO); }

void SetupPortals(ScriptMgr * mgr)
{
	mgr->register_gameobject_script(20000000, &tele_Orgrimarr_to_Undercity::Create);
	mgr->register_gameobject_script(20000001, &tele_Undercity_to_Orgrimarr::Create);
	mgr->register_gameobject_script(20000002, &tele_Orgrimarr_to_Gromgol::Create);
	mgr->register_gameobject_script(20000003, &tele_Gromgol_to_Orgrimarr::Create);
	mgr->register_gameobject_script(20000004, &tele_Undercity_to_Gromgol::Create);
	mgr->register_gameobject_script(20000005, &tele_Gromgol_to_Undercity::Create);
	mgr->register_gameobject_script(20000006, &tele_Ratchet_to_BootyBay::Create);
	mgr->register_gameobject_script(20000007, &tele_BootyBay_to_Ratchet::Create);
	mgr->register_gameobject_script(20000008, &tele_Auberdine_to_Menathil::Create);
	mgr->register_gameobject_script(20000009, &tele_Menathil_to_Auberdine::Create);
	mgr->register_gameobject_script(20000010, &tele_Menathil_to_Theramore::Create);
	mgr->register_gameobject_script(20000011, &tele_Theramore_to_Menathil::Create);
	mgr->register_gameobject_script(20000012, &tele_Auberdine_to_Azuremyst::Create);
	mgr->register_gameobject_script(20000013, &tele_Azuremyst_to_Auberdine::Create);
	mgr->register_gameobject_script(20000014, &tele_Teldrassil_to_Auberdine::Create);
	mgr->register_gameobject_script(20000015, &tele_Auberdine_to_Teldrassil::Create);
	mgr->register_gameobject_script(20000017, &tele_Stormwind_to_Ironforge::Create);
	mgr->register_gameobject_script(20000016, &tele_Ironforge_to_Stormwind::Create);
}
