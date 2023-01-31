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

#ifndef _FUNCTION_REGISTRY_H_
#define _FUNCTION_REGISTRY_H_

#include "CreatureFunctionTuple.hpp"
#include "CreatureFunctionTupleVisitor.hpp"

#include "GossipFunctionTuple.hpp"
#include "GossipFunctionTupleVisitor.hpp"

#include "GOFunctionTuple.hpp"
#include "GOFunctionTupleVisitor.hpp"

class FunctionRegistry
{
public:
	static void registerCreatureGossipFunction( unsigned int creatureId, unsigned int gossipEvent, void* function );

	static void registerGOGossipFunction( unsigned int goId, unsigned int gossipEvent, void* function );

	static void registerItemGossipFunction( unsigned int creatureId, unsigned int gossipEvent, void* function );



	static void registerGOEventFunction( unsigned int goId, unsigned int goEvent, void* function );
	static void registerCreatureEventFunction( unsigned int creatureId, unsigned int creatureEvent, void* function );



	static void visitCreatureGossipFunctions( GossipFunctionTupleVisitor *visitor );

	static void visitGOGossipFunctions( GossipFunctionTupleVisitor *visitor );

	static void visitItemGossipFunctions( GossipFunctionTupleVisitor *visitor );



	static void visitGOEventFunctions( GOFunctionTupleVisitor *visitor );
	static void visitCreatureEventFunctions( CreatureFunctionTupleVisitor *visitor );


	static void releaseFunctions();


	static GOFunctionTuple* getGOEventFunctions( unsigned int goId );
	static CreatureFunctionTuple* getCreatureEventFunctions( unsigned int creatureId );

private:
	static HM_NAMESPACE::HM_HASH_MAP< unsigned int, GossipFunctionTuple* > creatureGossipFunctions;
	static HM_NAMESPACE::HM_HASH_MAP< unsigned int, GossipFunctionTuple* > goGossipFunctions;
	static HM_NAMESPACE::HM_HASH_MAP< unsigned int, GossipFunctionTuple* > itemGossipFunctions;

	static HM_NAMESPACE::HM_HASH_MAP< unsigned int, GOFunctionTuple* > goFunctions;
	static HM_NAMESPACE::HM_HASH_MAP< unsigned int, CreatureFunctionTuple* > creatureFunctions;
};

#endif
