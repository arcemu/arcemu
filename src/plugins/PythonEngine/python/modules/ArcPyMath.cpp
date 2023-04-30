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


/// randomUInt
///   Generates a random unsigned integer
///
/// Parameters
///   N - Optional upper bound
///
/// Return value
///   Returns a randomly generated unsigned integer
///
/// Example
///   rand = randomUInt() # Generates a random unsigned integer between 0 and RAND_MAX
///   rand = randomUInt( 1234 ) # Generates a random unsigned integer between 0 and 1234
///
static PyObject* ArcPyMath_randomUInt( PyObject *self, PyObject *args )
{
	uint32 N = 0;

	if( !PyArg_ParseTuple( args, "|I", &N ) )
	{
		PyErr_SetString( PyExc_ValueError, "This function requires either no parameter or an integer parameter" );
		return NULL;
	}

	uint32 result;
	if( N > 0 )
		result = RandomUInt( N );
	else
		result = RandomUInt();

	return PyLong_FromUnsignedLong( result );
}

/// randomFloat
///   Generates a random floating point number
///
/// Parameters
///   N - Optional upper bound
///
/// Return value
///   Returns a randomly generated floating point number
///
/// Example
///   rand = randomFloat() # Generates a random floating point number in the interval 0.0 <= x < 1.0
///   rand = randomFloat( 12.34 ) # Generates a random floating point number in the interval 0.0 <= x < 12.34
///
static PyObject* ArcPyMath_randomFloat( PyObject *self, PyObject *args )
{
	float N = 0;

	if( !PyArg_ParseTuple( args, "|f", &N ) )
	{
		PyErr_SetString( PyExc_ValueError, "This function requires either no parameter or a floating point parameter" );
		return NULL;
	}

	float result;
	if( N > 0.0f )
		result = RandomFloat( N );
	else
		result = RandomFloat();

	return PyFloat_FromDouble( result );
}

static PyMethodDef ArcPyMathMethods[] = {
	{ "calcDistance", ArcPyMath_calcDistance, METH_VARARGS, "Calculates the distance between two points" },
	{ "randomUInt", ArcPyMath_randomUInt, METH_VARARGS, "Generates a random unsigned integer" },
	{ "randomFloat", ArcPyMath_randomFloat, METH_VARARGS, "Generates a random floating point number" },
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
