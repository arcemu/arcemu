/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2022 <http://www.ArcEmu.org/>
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

#include <Python.h>

#include "ServerHookRegistry.hpp"

#include "FunctionRegistry.hpp"

extern void registerArcemuConstants( PyObject *module );

extern int registerArcPyAura( PyObject *module );
extern int registerArcPyGameObject( PyObject *module );
extern int registerArcPyGossipMenu( PyObject *module );
extern int registerArcPyGuild( PyObject *module );
extern int registerArcPyItem( PyObject *module );
extern int registerArcPyUnit( PyObject *module );
extern int registerArcPyPlayer( PyObject *module );
extern int registerArcPyQuest( PyObject *module );
extern int registerArcPySpell( PyObject *module );
extern int registerArcPyWorldSession( PyObject *module );

static PyObject* arcemu_RegisterServerHook( PyObject *self, PyObject *args )
{
	unsigned long serverEvent = 0;
	PyObject *callback = NULL;

	if( !PyArg_ParseTuple( args, "IO", &serverEvent, &callback ) )
	{
		return NULL;
	}

	if( strcmp( Py_TYPE( callback )->tp_name, "function" ) != 0 )
	{
		PyErr_SetString( PyExc_TypeError, "Second argument should be a function!" );
		return NULL;
	}

	Py_IncRef( callback );
	ServerHookRegistry::addHook( serverEvent, callback );
	
	sLog.Debug( "APE", "Registered handler for server hook event %u", serverEvent );

	Py_RETURN_NONE;
}

static PyObject* arcemu_RegisterUnitGossipEvent( PyObject *self, PyObject *args )
{
	unsigned long creatureId = 0;
	unsigned long gossipEvent = 0;
	PyObject *callback = NULL;

	if( !PyArg_ParseTuple( args, "IIO", &creatureId, &gossipEvent, &callback ) )
	{
		return NULL;
	}

	if( strcmp( Py_TYPE( callback )->tp_name, "function" ) != 0 )
	{
		PyErr_SetString( PyExc_TypeError, "Third argument should be a function!" );
		return NULL;
	}

	Py_IncRef( callback );
	FunctionRegistry::registerCreatureGossipFunction( creatureId, gossipEvent, callback );

	Py_RETURN_NONE;
}

static PyMethodDef ArcemuMethods[] = {
	{ "RegisterServerHook", arcemu_RegisterServerHook, METH_VARARGS, "Registers a server hook function" },
	{ "RegisterUnitGossipEvent", arcemu_RegisterUnitGossipEvent, METH_VARARGS, "Registers a Unit gossip event" },
	{NULL, NULL, 0, NULL }
};

static PyModuleDef ArcemuModule = {
	PyModuleDef_HEAD_INIT, "arcemu", NULL, -1, ArcemuMethods,
	NULL, NULL, NULL, NULL
};

PyObject* PyInit_Arcemu(void)
{
	PyObject *module = PyModule_Create(&ArcemuModule);

	registerArcemuConstants( module );

	registerArcPyAura( module );
	registerArcPyGameObject( module );
	registerArcPyGossipMenu( module );
	registerArcPyGuild( module );
	registerArcPyItem( module );
	registerArcPyUnit( module );
	registerArcPyPlayer( module );
	registerArcPyQuest( module );
	registerArcPySpell( module );
	registerArcPyWorldSession( module );

	return module;
}
