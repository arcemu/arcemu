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

#include <Python.h>
#include <cstdio>

#include "ServerHookRegistry.hpp"

extern int registerArcPyGameObject( PyObject *module );
extern int registerArcPyGuild( PyObject *module );
extern int registerArcPyUnit( PyObject *module );
extern int registerArcPyPlayer( PyObject *module );
extern int registerArcPyQuest( PyObject *module );
extern int registerArcPySpell( PyObject *module );
extern int registerArcPyWorldSession( PyObject *module );

static PyObject* arcemu_info( PyObject *self, PyObject *args )
{
	printf( "Arcemu info called\n" );
	return Py_None;
}

static PyObject* arcemu_RegisterServerHook( PyObject *self, PyObject *args )
{
	printf( "Arcemu RegisterServerHook called\n" );

	unsigned long serverEvent = 0;
	PyObject *callback = NULL;

	if( !PyArg_ParseTuple( args, "IO", &serverEvent, &callback ) )
	{
		return NULL;
	}

	printf( "RegisterServerHook event %u\n", serverEvent );

	ServerHookRegistry::addHook( serverEvent, callback );

	return Py_None;
}

static PyMethodDef ArcemuMethods[] = {
	{ "info", arcemu_info, METH_NOARGS, "Prints Arcemu info" },
	{ "RegisterServerHook", arcemu_RegisterServerHook, METH_VARARGS, "Registers a server hook function" },
	{NULL, NULL, 0, NULL }
};

static PyModuleDef ArcemuModule = {
	PyModuleDef_HEAD_INIT, "arcemu", NULL, -1, ArcemuMethods,
	NULL, NULL, NULL, NULL
};

PyObject* PyInit_Arcemu(void)
{
	PyObject *module = PyModule_Create(&ArcemuModule);
	
	registerArcPyGameObject( module );
	registerArcPyGuild( module );
	registerArcPyUnit( module );
	registerArcPyPlayer( module );
	registerArcPyQuest( module );
	registerArcPySpell( module );
	registerArcPyWorldSession( module );

	return module;
}
