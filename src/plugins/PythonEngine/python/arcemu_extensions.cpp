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

PyObject* PyInit_Arcemu(void);

class PythonExtensionRegistrar
{
private:
	static void registerExtension( const char *name, PyObject* (*initfunc)(void) )
	{
		if( PyImport_AppendInittab( name, initfunc ) < 0 )
		{
			printf( "Failed to initialize extension: %s\n", name );
		}
	}

public:
	static void registerExtensions()
	{
		registerExtension( "arcemu", &PyInit_Arcemu );
	}
};

void register_arcemu_extensions()
{
	PythonExtensionRegistrar::registerExtensions();
}
