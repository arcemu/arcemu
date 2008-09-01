#include "StdAfx.h"
#include "Setup.h"
#include <ScriptSetup.h>

extern "C" SCRIPT_DECL uint32 _exp_get_script_type()
{
	return SCRIPT_TYPE_MISC;
}

extern "C" SCRIPT_DECL void _exp_script_register(ScriptMgr* mgr)
{
	if(Config.OptionalConfig.GetBoolDefault("Extra", "Instant70", false))
	{
		sLog.outColor(TGREEN,"\n	Instant70 Enabled");
		SetupInstant70(mgr);
	}
/*
	if(Config.MainConfig.GetBoolDefault("Extra", "L70etc", false))
	{
		sLog.outColor(TGREEN,"\n	L70etc Enabled");
		SetupL70etc(mgr);
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
	sLog.outColor(TNORMAL, "\n");
}

#ifdef WIN32

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
	return TRUE;
}

#endif
