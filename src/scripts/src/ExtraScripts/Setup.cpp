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
#include <ScriptSetup.h>

extern "C" SCRIPT_DECL uint32 _exp_get_script_type()
{
	return SCRIPT_TYPE_MISC;
}

extern "C" SCRIPT_DECL void _exp_script_register(ScriptMgr* mgr)
{
	if(Config.OptionalConfig.GetBoolDefault("Extra", "Instant80", false))
	{
		sLog.outColor(TGREEN,"\n	Instant80 Enabled");
		SetupInstant80(mgr);
	}
/*
	if(Config.MainConfig.GetBoolDefault("Extra", "L80etc", false))
	{
		sLog.outColor(TGREEN,"\n	L80etc Enabled");
		SetupL80etc(mgr);
	}
*/
	if(Config.OptionalConfig.GetBoolDefault("Extra", "Portals", false))
	{
		sLog.outColor(TGREEN,"\n	Portals Enabled");
		SetupPortals(mgr);
	}
	if(Config.OptionalConfig.GetBoolDefault("Extra", "PVPToken", false))
	{
		sLog.outColor(TGREEN,"\n	PvP Tokens Enabled");
		SetupPvPToken(mgr);
	}
/*
	if(Config.OptionalConfig.GetBoolDefault("Extra", "WeddingNPC", false))
	{
		sLog.outColor(TGREEN,"\n	WeddingNPC Enabled");
		SetupWeddingNPC(mgr);
	}
*/

    // Allows users to type "+unstuck" without quotes to revive them if
	// needed and teleport them to either Stormwind or Orgrimmar
    if(Config.OptionalConfig.GetBoolDefault("Extra", "Unstuck", false))
    {
        sLog.outColor(TGREEN,"\n    Unstuck Enabled");
        SetupUnstuck(mgr);
    }

	sLog.outColor(TNORMAL, "\n");
}

#ifdef WIN32

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
	return TRUE;
}

#endif
