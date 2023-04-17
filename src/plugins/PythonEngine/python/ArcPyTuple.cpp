/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2023 <http://www.ArcEmu.org/>
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

#include "Python.h"

#include "python/PythonObject.hpp"
#include "python/PythonTuple.hpp"
#include "python/ArcPyTuple.hpp"

#include "python/modules/ArcPyAura.hpp"
#include "python/modules/ArcPyGameObject.hpp"
#include "python/modules/ArcPyGuild.hpp"
#include "python/modules/ArcPyItem.hpp"
#include "python/modules/ArcPyPlayer.hpp"
#include "python/modules/ArcPyQuest.hpp"
#include "python/modules/ArcPySpell.hpp"
#include "python/modules/ArcPyUnit.hpp"
#include "python/modules/ArcPyWorldSession.hpp"

ArcPyTuple::ArcPyTuple( unsigned long size ) :
PythonTuple( size )
{
}

ArcPyTuple::~ArcPyTuple()
{
}

void ArcPyTuple::setItemAura( unsigned long idx, Aura* aura )
{
	if( aura == NULL )
	{
		setItemNone( idx );
		return;
	}

	ArcPyAura *apa = createArcPyAura();
	apa->auraPtr = aura;
	PyTuple_SetItem( getObject(), idx, (PyObject*)apa );
}

void ArcPyTuple::setItemGameObject( unsigned long idx, GameObject* gameObject )
{
	if( gameObject == NULL )
	{
		setItemNone( idx );
		return;
	}

	ArcPyGameObject *apgo = createArcPyGameObject( gameObject );
	PyTuple_SetItem( getObject(), idx, (PyObject*)apgo );
}

void ArcPyTuple::setItemGuild( unsigned long idx, Guild* guild )
{
	if( guild == NULL )
	{
		setItemNone( idx );
		return;
	}

	ArcPyGuild *apg = createArcPyGuild();
	apg->guildPtr = guild;
	PyTuple_SetItem( getObject(), idx, (PyObject*)apg );
}

void ArcPyTuple::setItemItem( unsigned long idx, Item* item )
{
	if( item == NULL )
	{
		setItemNone( idx );
		return;
	}

	ArcPyItem *api = createArcPyItem();
	api->itemPtr = item;
	PyTuple_SetItem( getObject(), idx, (PyObject*)api );
}

void ArcPyTuple::setItemPlayer( unsigned long idx, Player* player )
{
	if( player == NULL )
	{
		setItemNone( idx );
		return;
	}

	ArcPyPlayer *app = createArcPyPlayer(player);
	PyTuple_SetItem( getObject(), idx, (PyObject*)app );
}

void ArcPyTuple::setItemQuest( unsigned long idx, Quest* quest )
{
	if( quest == NULL )
	{
		setItemNone( idx );
		return;
	}

	ArcPyQuest *apq = createArcPyQuest();
	apq->questPtr = quest;
	PyTuple_SetItem( getObject(), idx, (PyObject*)apq );
}

void ArcPyTuple::setItemSpell( unsigned long idx, Spell* spell )
{
	if( spell == NULL )
	{
		setItemNone( idx );
		return;
	}

	ArcPySpell *aps = createArcPySpell();
	aps->spellPtr = spell;
	PyTuple_SetItem( getObject(), idx, (PyObject*)aps );
}

void ArcPyTuple::setItemUnit( unsigned long idx, Unit* unit )
{
	if( unit == NULL )
	{
		setItemNone( idx );
		return;
	}

	ArcPyUnit *apu = createArcPyUnit(unit);
	PyTuple_SetItem( getObject(), idx, (PyObject*)apu );
}

void ArcPyTuple::setItemWorldSession( unsigned long idx, WorldSession* worldSession )
{
	if( worldSession == NULL )
	{
		setItemNone( idx );
		return;
	}

	ArcPyWorldSession *apws = createArcPyWorldSession();
	apws->worldSessionPtr = worldSession;
	PyTuple_SetItem( getObject(), idx, (PyObject*)apws );
}

