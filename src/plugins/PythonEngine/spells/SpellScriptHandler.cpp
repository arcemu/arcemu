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

#include "spells/SpellScriptHandler.hpp"

#include "engine/FunctionRegistry.hpp"

#include "Python.h"

#include "python/PythonObject.hpp"
#include "python/PythonValue.hpp"
#include "python/PythonCallable.hpp"
#include "python/ArcPython.hpp"

#include "python/ArcPyTuple.hpp"

bool SpellScriptHandler::handleDummySpell( uint32 spellEffectIndex, Spell *spell )
{
	Guard g( ArcPython::getLock() );

	uint32 spellId = spell->GetProto()->Id;

	void *function = FunctionRegistry::getDummySpellHandler( spellId );
	if( function == NULL )
		return false;

	bool returnValue = true;

	ArcPyTuple args( 2 );
	args.setItem( 0, spellEffectIndex );
	args.setItemSpell( 1, spell );

	PythonCallable callable( function );
	PythonValue value = callable.call( args );
	if( value.isEmpty() )
	{
		Python::printError();
		returnValue = false;
	}
	else
	{
		if( value.isBool() && !value.getBoolValue() )
		{
			returnValue = false;
		}
		value.decref();
	}

	return returnValue;
}

bool SpellScriptHandler::handleScriptedEffect( uint32 spellEffectIndex, Spell *spell )
{
	Guard g( ArcPython::getLock() );

	uint32 spellId = spell->GetProto()->Id;

	void *function = FunctionRegistry::getScriptedEffectHandler( spellId );
	if( function == NULL )
		return false;

	bool returnValue = true;

	ArcPyTuple args( 2 );
	args.setItem( 0, spellEffectIndex );
	args.setItemSpell( 1, spell );

	PythonCallable callable( function );
	PythonValue value = callable.call( args );
	if( value.isEmpty() )
	{
		Python::printError();
		returnValue = false;
	}
	else
	{
		if( value.isBool() && !value.getBoolValue() )
		{
			returnValue = false;
		}
		value.decref();
	}

	return returnValue;
}

bool SpellScriptHandler::handleDummyAura( uint32 i, Aura *aura, bool apply )
{
	Guard g( ArcPython::getLock() );

	uint32 spellId = aura->GetSpellId();

	void *function = FunctionRegistry::getDummyAuraHandler( spellId );
	if( function == NULL )
		return false;

	bool returnValue = true;

	ArcPyTuple args( 3 );
	args.setItem( 0, i );
	args.setItemAura( 1, aura );
	args.setItemBool( 2, apply );

	PythonCallable callable( function );
	PythonValue value = callable.call( args );
	if( value.isEmpty() )
	{
		Python::printError();
		returnValue = false;
	}
	else
	{
		if( value.isBool() && !value.getBoolValue() )
		{
			returnValue = false;
		}
		value.decref();
	}

	return returnValue;
}
