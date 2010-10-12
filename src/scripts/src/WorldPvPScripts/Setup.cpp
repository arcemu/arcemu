#include "StdAfx.h"
#define SKIP_ALLOCATOR_SHARING 1
#include <ScriptSetup.h>

void SetupPvPTerokkarForest(ScriptMgr *mgr);
void SetupPvPHellfirePeninsula(ScriptMgr *mgr);
void SetupPvPSilithus(ScriptMgr *mgr);
void SetupPvPZangarmarsh(ScriptMgr *mgr);

extern "C" SCRIPT_DECL uint32 _exp_get_script_type()
{
	return SCRIPT_TYPE_MISC;
}

extern "C" SCRIPT_DECL void _exp_script_register(ScriptMgr* mgr)	// Comment any script to disable it
{
	SetupPvPTerokkarForest(mgr);
	SetupPvPHellfirePeninsula(mgr);
	SetupPvPSilithus(mgr);
	SetupPvPZangarmarsh(mgr);
}

#ifdef WIN32

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
	return TRUE;
}

#endif
