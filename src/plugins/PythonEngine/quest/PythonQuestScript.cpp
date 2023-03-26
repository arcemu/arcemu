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
#include "quest/PythonQuestScript.hpp"

#include "Python.h"
#include "python/ArcPython.hpp"
#include "python/PythonCallable.hpp"
#include "python/ArcPyTuple.hpp"

PythonQuestScript::PythonQuestScript( const QuestFunctionTuple &tuple )
{
	setFunctions( tuple );
}

PythonQuestScript::~PythonQuestScript()
{
}

void PythonQuestScript::setFunctions( const QuestFunctionTuple &tuple )
{
	for( int i = 0; i < PYTHON_QUEST_EVENT_COUNT; i++ )
	{
		functions.setFunction( i, tuple.getFunction( i ) );
	}
}

void PythonQuestScript::clearFunctions()
{
	functions.clearFunctions();
}

void PythonQuestScript::OnQuestStart( Player* target, QuestLogEntry* questLogEntry )
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_QUEST_EVENT_ON_ACCEPT;
	if( ! functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 2 );
	args.setItemPlayer( 0, target );
	args.setItem( 1, questLogEntry->GetQuest()->id );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonQuestScript::OnQuestComplete( Player* target, QuestLogEntry* questLogEntry )
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_QUEST_EVENT_ON_COMPLETE;
	if( ! functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 2 );
	args.setItemPlayer( 0, target );
	args.setItem( 1, questLogEntry->GetQuest()->id );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonQuestScript::OnQuestCancel( Player* target )
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_QUEST_EVENT_ON_CANCEL;
	if( ! functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 1 );
	args.setItemPlayer( 0, target );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonQuestScript::OnGameObjectActivate( uint32 entry, Player* target, QuestLogEntry* questLogEntry )
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_QUEST_EVENT_GAMEOBJECT_ACTIVATE;
	if( ! functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 3 );
	args.setItem( 0, entry );
	args.setItemPlayer( 1, target );
	args.setItem( 2, questLogEntry->GetQuest()->id );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonQuestScript::OnCreatureKill( uint32 entry, Player* target, QuestLogEntry* questLogEntry )
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_QUEST_EVENT_ON_CREATURE_KILL;
	if( ! functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 3 );
	args.setItem( 0, entry );
	args.setItemPlayer( 1, target );
	args.setItem( 2, questLogEntry->GetQuest()->id );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonQuestScript::OnExploreArea( uint32 areaId, Player* target, QuestLogEntry* questLogEntry )
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_QUEST_EVENT_ON_EXPLORE_AREA;
	if( ! functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 3 );
	args.setItem( 0, areaId );
	args.setItemPlayer( 1, target );
	args.setItem( 2, questLogEntry->GetQuest()->id );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonQuestScript::OnPlayerItemPickup( uint32 itemId, uint32 totalCount, Player* target, QuestLogEntry* questLogEntry )
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_QUEST_EVENT_ON_PLAYER_ITEMPICKUP;
	if( ! functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 4 );
	args.setItem( 0, itemId );
	args.setItem( 1, totalCount );
	args.setItemPlayer( 2, target );
	args.setItem( 3, questLogEntry->GetQuest()->id );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

