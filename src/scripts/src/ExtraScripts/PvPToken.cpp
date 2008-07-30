#include "StdAfx.h"
#include "Setup.h"

void onPvpKill(Player* plr, Player* victim)
{
	if( plr->GetTeam() != victim->GetTeam())
	{
		char message[200];
		sprintf(message, "%s has killed %s", plr->GetName(), victim->GetName());
		sWorld.SendWorldWideScreenText(message);

		int32 gold_check_p = plr->GetUInt32Value( PLAYER_FIELD_COINAGE );
		int32 gold_check_v = victim->GetUInt32Value( PLAYER_FIELD_COINAGE );
		int32 new_gold_p = gold_check_p + 10000;
		int32 new_gold_v;
		if(gold_check_v < 5000)
		{
			new_gold_v = 0;
		}
		else
		{
			new_gold_v = gold_check_v - 5000;
		}

		plr->SetUInt32Value( PLAYER_FIELD_COINAGE, new_gold_p );
		victim->SetUInt32Value( PLAYER_FIELD_COINAGE, new_gold_v );
	}
}

void SetupPvPToken(ScriptMgr * mgr)
{
	mgr->register_hook(SERVER_HOOK_EVENT_ON_KILL_PLAYER, (void*)onPvpKill);
}
