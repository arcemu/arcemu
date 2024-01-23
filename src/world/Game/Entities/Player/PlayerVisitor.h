/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2024 <http://www.ArcEmu.org/>
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

#ifndef PLAYERVISITOR_H
#define PLAYERVISITOR_H

class SERVER_DECL PlayerVisitor
{
public:
	virtual ~PlayerVisitor(){}
	virtual void visit( Player *player ) = 0;
};


class SERVER_DECL CastSpellOnPlayers : public PlayerVisitor
{
private:
	bool triggered;
	uint32 spellId;

public:
	CastSpellOnPlayers( uint32 spellId, bool triggered )
	{
		this->spellId = spellId;
		this->triggered = triggered;
	}

	void visit( Player *player )
	{
		player->CastSpell( player, spellId, triggered );
	}
};

class SERVER_DECL RemoveAura : public PlayerVisitor
{
private:
	uint32 spellId;

public:
	RemoveAura( uint32 spellId )
	{
		this->spellId = spellId;
	}

	void visit( Player *player )
	{
		player->RemoveAura( spellId );
	}
};


#endif
