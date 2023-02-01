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

#ifndef P_GO_AI_SCRIPT_H
#define P_GO_AI_SCRIPT_H

#include "GOFunctionTuple.hpp"

class PythonGameObjectAIScript : public GameObjectAIScript
{
public:
	PythonGameObjectAIScript( GameObject* src, GOFunctionTuple &functions );
	virtual ~PythonGameObjectAIScript();

	void setFunctions( GOFunctionTuple &functions );
	
	void OnCreate();
	void OnSpawn();
	void OnDespawn();
	void OnLootTaken( Player* looter, ItemPrototype* itemInfo);
	void OnActivate( Player* player );
	void OnDamaged( uint32 damage );
	void OnDestroyed();
	void AIUpdate();

	GameObject* getGameObject(){ return _gameobject; }

private:
	GOFunctionTuple functions;
};

#endif
