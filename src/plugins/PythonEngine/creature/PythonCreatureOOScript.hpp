/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2024 <http://www.ArcEmu.org/>
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

#ifndef A_PCOS_H_
#define A_PCOS_H_

class PythonCreatureOOScript : public CreatureAIScript
{
public:
	PythonCreatureOOScript( Creature *creature, PyObject *obj );
	~PythonCreatureOOScript();

	void setObject( PyObject *obj );

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
	PyObject *obj;
};

#endif
