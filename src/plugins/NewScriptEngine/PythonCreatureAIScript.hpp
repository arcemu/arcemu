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

#ifndef P_CREATURE_AI_SCRIPT_H
#define P_CREATURE_AI_SCRIPT_H

#include "CreatureFunctionTuple.hpp"

class PythonCreatureAIScript : public CreatureAIScript
{
public:
	PythonCreatureAIScript( Creature* src, CreatureFunctionTuple &functions );
	virtual ~PythonCreatureAIScript();

	void setFunctions( CreatureFunctionTuple &functions );




	void OnCombatStart( Unit* target );
	void OnCombatStop( Unit* target );
	void OnDamageTaken( Unit* attacker, uint32 amount );
	void OnCastSpell( uint32 spellId );

	void OnTargetParried( Unit* target );
	void OnTargetDodged( Unit* target );
	void OnTargetBlocked( Unit* target, int32 amount );
	void OnTargetCritHit( Unit* mTarget, int32 amount );
	void OnTargetDied( Unit* target );

	void OnHit( Unit* target, float amount );
	void OnDied( Unit* killer );
	

	void OnLoad();

private:
	CreatureFunctionTuple functions;
};

#endif
