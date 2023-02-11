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

#ifndef PYTHON_QUESTSCRIPT_FACTORY_H
#define PYTHON_QUESTSCRIPT_FACTORY_H

#include "quest/PythonQuestScript.hpp"

class PythonQuestScriptFactory
{
public:
	static PythonQuestScript* createQuestScript( unsigned int questId );

	static void onReload();
	static void onShutDown();

	static void removeScript( PythonQuestScript* script );

private:
	static HM_NAMESPACE::HM_HASH_SET< PythonQuestScript* > createdScripts;

	static Mutex lock;
};

#endif
