/********************************************************************
  Artifact Emulation Services
  All Services Are Provided by Free Use of Services.
  Productions by ArcEmu are only mend for use of game play.
  
  The game services that ArcEmu provides are only for exploration
  and not of use to be a full striving game server replacement.
  For this use please play on the official services of Blizzard.

  The uses above are not ment to be released for those services
  since this is in the state of internet piracy.

  ArcEmu only provides the services on voluntary base and is not
  willing to provide the sources to secondary parties.

  All Copyrights and Trademarks are under name of ArcEmu

  Copyrights (c) 2008 ArcEmu
********************************************************************/

#include "StdAfx.h"
#include "Setup.h"

extern "C" SCRIPT_DECL uint32 _exp_get_version()
{
    return MAKE_SCRIPT_VERSION(SCRIPTLIB_VERSION_MAJOR, SCRIPTLIB_VERSION_MINOR);
}

extern "C" SCRIPT_DECL void _exp_script_register(ScriptMgr* mgr)
{
	SetupCustom_Teleporters(mgr);
}

#ifdef WIN32

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
    return TRUE;
}

#endif
