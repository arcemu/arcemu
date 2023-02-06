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
#include "Python.hpp"

Python::Python()
{
	Py_InitializeEx( 0 );
}

Python::~Python()
{
	Py_Finalize();
}

void Python::printError()
{
	PyErr_Print();
}

void Python::setPath( const wchar_t *path )
{
	Py_SetPath( path );
}

int Python::runSimpleFile( const char *fileName )
{
	FILE *fp = fopen( fileName, "rt" );
	if( fp == NULL )
	{
		return -1;
	}

	int value = PyRun_SimpleFile( fp, fileName );

	fclose( fp );

	return value;
}
