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

#include "creature/CreatureFunctionTuple.hpp"
#include "creature/CreatureFunctionTupleVisitor.hpp"

#include "gossip/GossipFunctionTuple.hpp"
#include "gossip/GossipFunctionTupleVisitor.hpp"

#include "gameobject/GOFunctionTuple.hpp"
#include "gameobject/GOFunctionTupleVisitor.hpp"

#include "instance/InstanceFunctionTuple.hpp"
#include "instance/InstanceFunctionTupleVisitor.hpp"

#include "quest/QuestFunctionTuple.hpp"
#include "quest/QuestFunctionTupleVisitor.hpp"

class FunctionRegistry
{
public:
	static void registerCreatureGossipFunction( unsigned int creatureId, unsigned int gossipEvent, void* function );

	static void registerGOGossipFunction( unsigned int goId, unsigned int gossipEvent, void* function );

	static void registerItemGossipFunction( unsigned int creatureId, unsigned int gossipEvent, void* function );



	static void registerGOEventFunction( unsigned int goId, unsigned int goEvent, void* function );
	static void registerCreatureEventFunction( unsigned int creatureId, unsigned int creatureEvent, void* function );
	static void registerQuestEventFunction( unsigned int questId, unsigned int questEvent, void* function );
	static void registerInstanceEventFunction( unsigned int instanceId, unsigned int instanceEvent, void* function );


	static void visitCreatureGossipFunctions( GossipFunctionTupleVisitor *visitor );

	static void visitGOGossipFunctions( GossipFunctionTupleVisitor *visitor );

	static void visitItemGossipFunctions( GossipFunctionTupleVisitor *visitor );



	static void visitGOEventFunctions( GOFunctionTupleVisitor *visitor );
	static void visitInstanceEventFunctions( InstanceFunctionTupleVisitor *visitor );
	static void visitCreatureEventFunctions( CreatureFunctionTupleVisitor *visitor );
	static void visitQuestEventFunctions( QuestFunctionTupleVisitor *visitor );


	static void releaseFunctions();


	static GOFunctionTuple* getGOEventFunctions( unsigned int goId );
	static CreatureFunctionTuple* getCreatureEventFunctions( unsigned int creatureId );
	static QuestFunctionTuple* getQuestFunctions( unsigned int questId );
	static InstanceFunctionTuple* getInstanceFunctions( unsigned int mapId );

private:
	static HM_NAMESPACE::HM_HASH_MAP< unsigned int, GossipFunctionTuple* > creatureGossipFunctions;
	static HM_NAMESPACE::HM_HASH_MAP< unsigned int, GossipFunctionTuple* > goGossipFunctions;
	static HM_NAMESPACE::HM_HASH_MAP< unsigned int, GossipFunctionTuple* > itemGossipFunctions;

	static HM_NAMESPACE::HM_HASH_MAP< unsigned int, GOFunctionTuple* > goFunctions;
	static HM_NAMESPACE::HM_HASH_MAP< unsigned int, InstanceFunctionTuple* > instanceFunctions;
	static HM_NAMESPACE::HM_HASH_MAP< unsigned int, CreatureFunctionTuple* > creatureFunctions;
	static HM_NAMESPACE::HM_HASH_MAP< unsigned int, QuestFunctionTuple* > questFunctions;
};

#endif
