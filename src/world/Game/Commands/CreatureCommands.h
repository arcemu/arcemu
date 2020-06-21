/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2019 <http://www.ArcEmu.org/>
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

#ifndef _CREATURECOMMANDS_H
#define _CREATURECOMMANDS_H

class SERVER_DECL CreatureSendChatMessageToPlayerCommand
{
private:
	Creature *creature;
	Player *player;

	uint8 emote;
	uint32 language;
	string message;

public:
	CreatureSendChatMessageToPlayerCommand( Creature* creature, Player *player )
	{
		this->creature = creature;
		this->player = player;
	}

	void setEmote( uint8 emote ){ this->emote = emote; }
	void setLanguage( uint32 language ){ this->language = language; }
	void setMessage( string message ){ this->message = message; }

	void execute()
	{
		Messenger::SendChatMessageToPlayer( creature,
					    player,
					    emote,
					    language,
					    string( creature->GetCreatureInfo()->Name ),
					    string( message ) );
	}
};

#endif
