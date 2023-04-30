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

#include "StdAfx.h"

/// calcDistance
///   Calculates the distance between two points
///
/// Parameters
///   ax      -  Point a's X coordinate
///   ay      -  Point a's Y coordinate
///   az      -  Point a's Z coordinate
///   bx      -  Point b's X coordinate
///   by      -  Point b's Y coordinate
///   bz      -  Point b's Z coordinate
///
/// Return value
///   Returns the distance between the two specified points
///
/// Example
///   distance = Math.calcDistance( 1.0, 2.0, 3.0, 9.0, 8.0, 7.0 )
///
static PyObject* ArcPyMath_calcDistance( PyObject *self, PyObject *args )
{
	double ax;
	double ay;
	double az;
	double bx;
	double by;
	double bz;

	if( !PyArg_ParseTuple( args, "dddddd", &ax, &ay, &az, &bx, &by, &bz ) )
	{
		PyErr_SetString( PyExc_ValueError, "This function requires ax, ay, az, bx, by, bz float coordinates be specified" );
		return NULL;
	}

	double distance = Math::CalcDistance( ax, ay, az, bx, by, bz );
	PyObject *distanceObject = PyFloat_FromDouble( distance );

	return distanceObject;
}


static PyMethodDef ArcPyMathMethods[] = {
	{ "calcDistance", ArcPyMath_calcDistance, METH_VARARGS, "Calculates the distance between two points" },
	{NULL, NULL, 0, NULL }
};

static PyModuleDef ArcPyMathModule = {
	PyModuleDef_HEAD_INIT,
	"ArcPyMath",
	NULL,
	-1,
	ArcPyMathMethods,
	NULL, NULL, NULL, NULL
};

/// Initializes the Arcemu Math Python module
PyObject* PyInit_ArcPyMath( void )
{
	PyObject *module = PyModule_Create(&ArcPyMathModule);
	return module;
}
