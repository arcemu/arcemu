#
# ArcEmu MMORPG Server
# Copyright (C) 2008-2023 <http://www.ArcEmu.org/>
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
# 
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
# 
#
# Creature AI script for Guard Roberts (12423)
# for the quest Garments of the Light (5624)
# 
# The player should get quest credit after the following:
# * Heal the NPC with Lesser Heal II
# * Buff the NPC with Power Word: Fortitude I
#

import arcemu
from arcemu import Unit
from arcemu import Player

CREATUREID_GUARD_ROBERTS = 12423
QUESTID_GARMENTS_OF_THE_LIGHT_HUMAN = 5624
SPELLID_LESSERHEAL_II = 2052
SPELLID_POWERWORD_FORTITUDE_I = 1243

def GuardRoberts_onLoad( unit, event ):
	creature = unit.toCreature()
	
	creature.setPvPFlag()
	creature.setStandState( arcemu.STANDSTATE_KNEEL )
	creature.setCanRegenerateHP( 0 )
	creature.setHealth( int( creature.getMaxHealth() * 0.73 ) )
	creature.setMovementType( arcemu.MOVEMENTTYPE_DONTMOVEWP )
	creature.resetWaypoint()
	
	creature.destroyCustomWaypoints()
	creature.createCustomWaypoint( -9510.285156, -142.285202, 59.149242, 5.255516, 250, arcemu.WAYPOINT_FLAG_RUN, 0 )
	creature.createCustomWaypoint( -9513.697266, -152.955444, 59.462231, 4.462268, 250, arcemu.WAYPOINT_FLAG_RUN, 0 )
	creature.createCustomWaypoint( -9516.402344, -164.903122, 59.309666, 4.489758, 250, arcemu.WAYPOINT_FLAG_RUN, 0 )
	
	
def GuardRoberts_onHealed( unit, healer, spellId, amount ):
	creature = unit.toCreature()
	
	if not healer.isPlayer():
		return
		
	if creature.getStandState() == arcemu.STANDSTATE_KNEEL and spellId == SPELLID_LESSERHEAL_II:
		creature.setStandState( arcemu.STANDSTATE_STAND )
		creature.faceUnit( healer )
		creature.emote( arcemu.EMOTE_ONESHOT_BOW )
		creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Ah, priest, you came along just in time. I appreciate it." )
	
def GuardRoberts_onApplyAura( unit, caster, spellId ):
	creature = unit.toCreature()
	
	if not caster.isPlayer():
		return
		
	pu = caster.toPlayer()
	
	if creature.getStandState() == arcemu.STANDSTATE_STAND and spellId == SPELLID_POWERWORD_FORTITUDE_I:
		pu.markQuestObjectiveAsComplete( QUESTID_GARMENTS_OF_THE_LIGHT_HUMAN, 0, creature.getGUID() )
		creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Thank you! Thank you, priest. Now I can take on those murlocs with the Light on my side!" )
		creature.stopMovement( 2500 )
		creature.setMovementType( arcemu.MOVEMENTTYPE_FORWARDTHENSTOP )		
		creature.emote( arcemu.EMOTE_ONESHOT_CHEER )
		
def GuardRoberts_onReachWP( unit, event, waypointId, forward ):
	creature = unit.toCreature()
	
	if forward:
		if waypointId == 1:
			creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Farewell to you, and may the Light be with you always." )
			
		elif waypointId == 3:
			creature.despawn( 0, 3000 )
		
arcemu.RegisterUnitEvent( CREATUREID_GUARD_ROBERTS, arcemu.CREATURE_EVENT_ON_LOAD, GuardRoberts_onLoad )
arcemu.RegisterUnitEvent( CREATUREID_GUARD_ROBERTS, arcemu.CREATURE_EVENT_ON_HEALED, GuardRoberts_onHealed )
arcemu.RegisterUnitEvent( CREATUREID_GUARD_ROBERTS, arcemu.CREATURE_EVENT_ON_APPLY_AURA, GuardRoberts_onApplyAura )
arcemu.RegisterUnitEvent( CREATUREID_GUARD_ROBERTS, arcemu.CREATURE_EVENT_ON_REACH_WP, GuardRoberts_onReachWP )
