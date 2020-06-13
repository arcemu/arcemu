/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2012 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "StdAfx.h"

void WorldSession::HandleArenaTeamRosterOpcode(WorldPacket & recv_data)
{
	uint32 teamId;
	ArenaTeam* team;
	recv_data >> teamId;
	team = objmgr.GetArenaTeamById(teamId);
	if(team)
	{
		//slot = TeamCountToId[team->m_type];
		WorldPacket data(1000);
		team->Roster(data);
		SendPacket(&data);
	}
}

void WorldSession::HandleArenaTeamQueryOpcode(WorldPacket & recv_data)
{
	ArenaTeam* team;
	uint32 team_id;
	recv_data >> team_id;

	team = objmgr.GetArenaTeamById(team_id);
	if(team != NULL)
	{
		WorldPacket data(1000);
		team->Query(data);
		SendPacket(&data);

		team->Stat(data);
		SendPacket(&data);
	}
}

void WorldSession::HandleArenaTeamAddMemberOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	WorldPacket data(SMSG_ARENA_TEAM_INVITE, 40);
	string player_name;
	uint32 teamId;
	recv_data >> teamId >> player_name;

	ArenaTeam* pTeam = objmgr.GetArenaTeamById(teamId);
	if(!pTeam)
		return;

	if(!pTeam->HasMember(GetPlayer()->GetLowGUID()))
	{
		GetPlayer()->SoftDisconnect();
		return;
	}

	Player* plr = objmgr.GetPlayer(player_name.c_str(), false);
	if(plr == NULL)
	{
		SystemMessage("Player `%s` is non-existent or not online.", player_name.c_str());
		return;
	}

	if(pTeam->m_leader != _player->GetLowGUID())
	{
		SystemMessage("You are not the captain of this arena team.");
		return;
	}

	if(plr->getLevel() < PLAYER_ARENA_MIN_LEVEL)
	{
		SystemMessage("Player must be level %u to join an arena team.", PLAYER_ARENA_MIN_LEVEL);
		return;
	}

	if(plr->m_arenaTeams[pTeam->m_type] != NULL)
	{
		SystemMessage("That player is already in an arena team of this type.");
		return;
	}

	if(plr->m_arenateaminviteguid != 0)
	{
		SystemMessage("That player is already invited to an arena team");
		return;
	}

	if(plr->GetTeam() != _player->GetTeam() && !HasGMPermissions())
	{
		SystemMessage("That player is a member of a different faction.");
		return;
	}

	plr->m_arenateaminviteguid = _player->m_arenaTeams[pTeam->m_type]->m_id;
	data << _player->GetName();
	data << _player->m_arenaTeams[pTeam->m_type]->m_name;
	plr->GetSession()->SendPacket(&data);
}

void WorldSession::HandleArenaTeamRemoveMemberOpcode(WorldPacket & recv_data)
{

	CHECK_INWORLD_RETURN

	ArenaTeam* team;
	uint8 slot;
	uint32 teamId;
	string name;
	PlayerInfo* inf;
	recv_data >> teamId >> name;

	team = objmgr.GetArenaTeamById(teamId);
	if(!team)
	{
		GetPlayer()->SoftDisconnect();
		return;
	}

	slot = static_cast<uint8>(team->m_type);

	if((team = _player->m_arenaTeams[slot]) == NULL)
	{
		SystemMessage("You are not in an arena team of this type.");
		return;
	}

	if(team->m_leader != _player->GetLowGUID())
	{
		SystemMessage("You are not the leader of this team.");
		return;
	}

	if((inf = objmgr.GetPlayerInfoByName(name.c_str())) == NULL)
	{
		SystemMessage("That player cannot be found.");
		return;
	}

	if(!team->HasMember(inf->guid))
	{
		SystemMessage("That player is not in your arena team.");
		return;
	}

	if(team->RemoveMember(inf))
	{
		char buffer[1024];
		WorldPacket* data;
		snprintf(buffer, 1024, "%s was removed from the arena team '%s'.", inf->name, team->m_name.c_str());
		data = sChatHandler.FillSystemMessageData(buffer);
		team->SendPacket(data);
		delete data;
		SystemMessage("Removed %s from the arena team '%s'.", inf->name, team->m_name.c_str());
	}
}

void WorldSession::HandleArenaTeamInviteAcceptOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	ArenaTeam* team;

	if(_player->m_arenateaminviteguid == 0)
	{
		SystemMessage("You have not been invited into another arena team.");
		return;
	}

	team = objmgr.GetArenaTeamById(_player->m_arenateaminviteguid);
	_player->m_arenateaminviteguid = 0;
	if(team == 0)
	{
		SystemMessage("That arena team no longer exists.");
		return;
	}

	if(team->m_memberCount >= team->m_slots)
	{
		SystemMessage("That team is now full.");
		return;
	}

	if(_player->m_arenaTeams[team->m_type] != NULL)		/* shouldn't happen */
	{
		SystemMessage("You have already been in an arena team of that size.");
		return;
	}

	if(team->AddMember(_player->m_playerInfo))
	{
		char buffer[1024];
		WorldPacket* data;
		snprintf(buffer, 1024, "%s joined the arena team, '%s'.", _player->GetName(), team->m_name.c_str());
		data = sChatHandler.FillSystemMessageData(buffer);
		team->SendPacket(data);
		delete data;
	}
	else
	{
		SendNotification("Internal error.");
	}
}

void WorldSession::HandleArenaTeamInviteDenyOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	ArenaTeam* team;
	if(_player->m_arenateaminviteguid == 0)
	{
		SystemMessage("You were not invited.");
		return;
	}

	team = objmgr.GetArenaTeamById(_player->m_arenateaminviteguid);
	_player->m_arenateaminviteguid = 0;
	if(team == NULL)
		return;

	Player* plr = objmgr.GetPlayer(team->m_leader);
	if(plr != NULL)
		plr->GetSession()->SystemMessage("%s denied your arena team invitation for %s.", _player->GetName(), team->m_name.c_str());
}

void WorldSession::HandleArenaTeamLeaveOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	ArenaTeam* team;
	uint32 teamId;
	recv_data >> teamId;

	team = objmgr.GetArenaTeamById(teamId);

	if(!team)
	{
		GetPlayer()->SoftDisconnect();
		return;
	}

	if((team = _player->m_arenaTeams[team->m_type]) == NULL)
	{
		SystemMessage("You are not in an arena team of this type.");
		return;
	}

	if(team->m_leader == _player->GetLowGUID() && team->m_memberCount == 1)
	{
		team->Destroy();
		return;
	}

	if(team->m_leader == _player->GetLowGUID())
	{
		SystemMessage("You cannot leave the team yet, promote someone else to captain first.");
		return;
	}

	if(team->RemoveMember(_player->m_playerInfo))
	{
		char buffer[1024];
		WorldPacket* data;
		snprintf(buffer, 1024, "%s left the arena team, '%s'.", _player->GetName(), team->m_name.c_str());
		data = sChatHandler.FillSystemMessageData(buffer);
		team->SendPacket(data);
		delete data;
		SystemMessage("You have left the arena team, '%s'.", team->m_name.c_str());
	}
}

void WorldSession::HandleArenaTeamDisbandOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	ArenaTeam* team;
	uint32 teamId;
	recv_data >> teamId;

	team = objmgr.GetArenaTeamById(teamId);
	if(!team)
	{
		GetPlayer()->SoftDisconnect();
		return;
	}

	if((team = _player->m_arenaTeams[team->m_type]) == NULL)
	{
		SystemMessage("You are not in an arena team of this type.");
		return;
	}

	if(team->m_leader != _player->GetLowGUID())
	{
		SystemMessage("You aren't the captain of this team.");
		return;
	}

	team->Destroy();
}

void WorldSession::HandleArenaTeamPromoteOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint32 teamId;
	uint8 slot;
	string name;
	ArenaTeam* team;
	PlayerInfo* inf;
	recv_data >> teamId >> name;

	team = objmgr.GetArenaTeamById(teamId);
	if(!team)
	{
		GetPlayer()->SoftDisconnect();
		return;
	}

	slot = static_cast<uint8>(team->m_type);

	if(slot >= NUM_ARENA_TEAM_TYPES)
		return;

	if((team = _player->m_arenaTeams[slot]) == NULL)
	{
		SystemMessage("You are not in an arena team of this type.");
		return;
	}

	if(team->m_leader != _player->GetLowGUID())
	{
		SystemMessage("You aren't the captain of this team.");
		return;
	}

	if((inf = objmgr.GetPlayerInfoByName(name.c_str())) == NULL)
	{
		SystemMessage("That player cannot be found.");
		return;
	}

	if(!team->HasMember(inf->guid))
	{
		SystemMessage("That player is not a member of your arena team.");
		return;
	}

	team->SetLeader(inf);
}
