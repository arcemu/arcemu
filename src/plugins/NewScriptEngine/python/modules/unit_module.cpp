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

static PyObject* unit_getName( PyObject *self, PyObject *args )
{
	printf( "Unit getName called.\n" );
	return PyUnicode_FromString( "This is the unit's name" );
}

static PyObject* unit_sendChatMessage( PyObject *self, PyObject *args )
{
	printf( "Unit sendChatMessage called.\n" );

	const char *message;

	if( !PyArg_ParseTuple( args, "s", &message ) )
	{
		printf( "Failed to parse parameters of sendChatMessage.\n" );
		return Py_None;
	}

	printf( "Send chat message: %s\n", message );

	return Py_None;
}

static PyMethodDef ArcPyUnitMethods[] = {
	{ "getName", unit_getName, METH_NOARGS, "Returns the name of the unit" },
	{ "sendChatMessage", unit_sendChatMessage, METH_VARARGS, "Sends a chat message" },
	{NULL, NULL, 0, NULL }
};

static PyModuleDef ArcPyUnitModule = {
	PyModuleDef_HEAD_INIT,
	"unit", NULL, -1, ArcPyUnitMethods,
	NULL, NULL, NULL, NULL
};

PyObject* PyInit_ArcPyUnit(void)
{
	return PyModule_Create(&ArcPyUnitModule);
}

