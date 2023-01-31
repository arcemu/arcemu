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

#ifndef P_CREATURE_AI_S_F_
#define P_CREATURE_AI_S_F_

class PythonCreatureAIScriptFactory
{
public:
	static CreatureAIScript* createScript( Creature* src );

	static void onReload();

	static void onShutdown();

	static void removeScript( uint32 creatureId );

private:
	static HM_NAMESPACE::HM_HASH_MAP< uint32, CreatureAIScript* > createdScripts;
};

#endif
