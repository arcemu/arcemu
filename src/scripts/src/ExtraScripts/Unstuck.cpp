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

int32	unstuckCooldownTime = 60;
bool	useBindPosition = false;

// Props to Deej for streamlining my code :)
bool OnChat(Player * pPlayer, uint32 type, uint32 lang, const char * msg, const char * misc)
{
	const char *cmd = "+unstuck";

	// No match, match must be exact, but is case insensitive
	if (stricmp(msg, cmd) != 0)
		return true;

	time_t now = UNIXTIME;
	int cooldownRemaining = (int)(unstuckCooldownTime - (now - pPlayer->GetLastUnstuckTime()));
	if ((pPlayer->GetLastUnstuckTime() == 0) || cooldownRemaining < 0)
	{
		// We've past the cooldown period
		if (pPlayer->getDeathState() == 1) // Just died
			pPlayer->RemoteRevive();
		if (pPlayer->getDeathState() != 0) // Not alive
			pPlayer->ResurrectPlayer();

		// Teleport
		if (useBindPosition) // FIX, NOT WORKING FOR SOME DUMB ASS REASON
		{
			// Use the player's bind (Hearthstone) location
		pPlayer->SafeTeleport(pPlayer->GetBindMapId(), 0, pPlayer->GetBindPositionX(), pPlayer->GetBindPositionY(), pPlayer->GetBindPositionZ(), 0.0f);
		}
		else
		{
			// Use primary capital cities rather than bind location
			if (pPlayer->GetTeam()) // Horde, Orgrimmar
				pPlayer->SafeTeleport(1, 0, 1502.71f, -4415.42f, 21.6f, 0.0f);
			else // Alliance, Stormwind
				pPlayer->SafeTeleport(0, 0, -9010.99f, 475.757f, 96.4711f, 0.0f);
		}
		
		// Setup current time to this player's SetLastUnstuckTime
		pPlayer->SetLastUnstuckTime(now);
		return false;
	}

	// We are still in cooldown
	int hours, minutes, seconds;
	hours = cooldownRemaining / 3600;
	cooldownRemaining -= hours * 3600;
	minutes = cooldownRemaining / 60;
	cooldownRemaining -= minutes * 60;
	seconds = cooldownRemaining;
	
	// Notify User
	if (hours != 0)
		sChatHandler.RedSystemMessage(pPlayer->GetSession(), "You have %02i:%02i:%02i remaining before you can use +unstuck again.",
			hours, minutes, seconds);
	else if (minutes != 0)
		sChatHandler.RedSystemMessage(pPlayer->GetSession(), "You have %02i:%02i remaining before you can use +unstuck again.",
			minutes, seconds);
	else
		sChatHandler.RedSystemMessage(pPlayer->GetSession(), "You have %i seconds remaining before you can use +unstuck again.",
			seconds);

	return false;
}

void SetupUnstuck(ScriptMgr * mgr)
{
	Config.OptionalConfig.GetBool("Extra", "UnstuckToBind", &useBindPosition);
	sLog.outDebug("UnstuckToBind: %u", uint32(useBindPosition));
	Config.OptionalConfig.GetInt("Extra", "UnstuckCooldown", &unstuckCooldownTime);
    mgr->register_hook(SERVER_HOOK_EVENT_ON_CHAT, (void*)OnChat);
	// Disable spell 7355, GM Ticket Tool
}

