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

#ifndef ARCPY_IS_R_H
#define ARCPY_IS_R_H

#include "instance/InstanceFunctionTuple.hpp"
#include "instance/InstanceFunctionTupleVisitor.hpp"

/// Registers Python Instance scripts in the core script manager
class InstanceScriptRegisterer : public InstanceFunctionTupleVisitor
{
private:
	ScriptMgr *mgr;

public:
	InstanceScriptRegisterer( ScriptMgr *mgr )
	{
		this->mgr = mgr;
	}

	void visit( unsigned int id, InstanceFunctionTuple &tuple );
};

#endif
