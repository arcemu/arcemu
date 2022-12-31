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

#ifndef PYTHON_TUPLE_H
#define PYTHON_TUPLE_H

class PythonTuple
{
public:
	PythonTuple( unsigned long size );
	~PythonTuple();

	void setItem( unsigned long idx, long value );
	void setItem( unsigned long idx, PythonObject &object );

	void setItemNone( unsigned long idx );

	unsigned long getSize(){ return size; }

	PyObject* getObject() const{ return obj; }

private:
	PyObject *obj;
	unsigned long size;
};

#endif
