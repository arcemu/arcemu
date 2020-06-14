/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2020 <http://www.ArcEmu.org/>
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
#include "PlayerPacketHandlers.h"

void LearnTalentPacketHandler::handlePacket( WorldSession &session, WorldPacket &recv_data )
{
	Player *_player = session.GetPlayer();
	CHECK_INWORLD_RETURN
		
	uint32 talent_id, requested_rank, unk;
	recv_data >> talent_id >> requested_rank >> unk;
	_player->LearnTalent(talent_id, requested_rank);
}

void UnlearnTalentsPacketHandler::handlePacket( WorldSession &session, WorldPacket &recv_data )
{
	Player *_player = session.GetPlayer();
	CHECK_INWORLD_RETURN
	uint32 price = _player->CalcTalentResetCost(_player->GetTalentResetTimes());
	if(_player->HasGold(price))
		return;

	_player->SetTalentResetTimes(_player->GetTalentResetTimes() + 1);
	_player->ModGold(-(int32) price);
	_player->Reset_Talents();
}
