/*
 * ArcEmu MMORPG Server
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

#ifndef SPAWN_TIMED_EMOTES_H_
#define SPAWN_TIMED_EMOTES_H_

struct spawn_timed_emotes
{
	uint8		type; //1 standstate, 2 emotestate, 3 emoteoneshot
	uint32		value; //get yar list elsewhere
	char*		msg; //maybe we wish to say smething while changing emote state
	uint8		msg_type; //yell ? say ?
	uint8		msg_lang; //yell ? say ?
	uint32		expire_after; //going to nex faze in
};

typedef std::list<spawn_timed_emotes*> TimedEmoteList;

#endif
