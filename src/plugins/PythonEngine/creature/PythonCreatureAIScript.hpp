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

#include "creature/CreatureFunctionTuple.hpp"

/// A Creature AI Script that calls Python functions
class PythonCreatureAIScript : public CreatureAIScript
{
public:
	///
	/// Initializes this Python Creature AI Script
	///
	/// Parameters
	///   src         -   The Creature that uses this script
	///   functions   -   A Tuple that contains the Python functions called by this script
	///
	/// Return value
	///   None
	///
	PythonCreatureAIScript( Creature* src, CreatureFunctionTuple &functions );

	/// Destructs this Python Creature AI Script
	virtual ~PythonCreatureAIScript();

	///
	/// Adds / replaces the Python functions in this Python Creature AI Script
	///
	/// Parameters
	///   functions   -   A Tuple that contains the Python functions called by this script
	///
	/// Return value
	///   None
	///
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
	void OnParried( Unit* target );
	void OnDodged( Unit* target );
	void OnBlocked( Unit* target, int32 amount );
	void OnCritHit( Unit* target, int32 amount );
	void OnHit( Unit* target, float amount );
	void OnDied( Unit* killer );
	void OnAssistTargetDied( Unit* assistTarget );
	void OnFear( Unit* feared, uint32 spellId );
	void OnFlee( Unit* flee );
	void OnCallForHelp();
	void OnLoad();
	void OnDespawn();
	void OnReachWP( uint32 waypointId, bool forwards );
	void OnLootTaken( Player* player, ItemPrototype* itemPrototype );
	void AIUpdate();
	void OnEmote( Player* player, EmoteType emote );
	void OnEnterVehicle();
	void OnExitVehicle();
	void OnFirstPassengerEntered( Unit *passenger );
	void OnVehicleFull();
	void OnLastPassengerLeft( Unit *passenger );
	void OnHealed( Unit* healer, uint32 spellId, int32 amount );
	void OnApplyAura( Unit* caster, uint32 spellId );
	void OnActivate( Unit* activator );
	void OnDeactivate();

private:
	/// Contains the Python functions that this Python Creature AI Script calls
	CreatureFunctionTuple functions;
};

#endif
