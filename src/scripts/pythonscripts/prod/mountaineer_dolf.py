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
# Creature AI script for Mountaineer Dolf (12423)
# for the quest Garments of the Light (5624)
# 
# The player should get quest credit after the following:
# * Heal the NPC with Lesser Heal II
# * Buff the NPC with Power Word: Fortitude I
#

import arcemu
from arcemu import Unit
from arcemu import Player

CREATUREID_MOUNTAINEER_DOLF = 12427
QUESTID_GARMENTS_OF_THE_LIGHT_DWARF = 5625
SPELLID_LESSERHEAL_II = 2052
SPELLID_POWERWORD_FORTITUDE_I = 1243

def MountaineerDolf_onLoad( unit, event ):
	creature = unit.toCreature()
	
	creature.setPvPFlag()
	creature.setStandState( arcemu.STANDSTATE_KNEEL )
	creature.setCanRegenerateHP( 0 )
	creature.setHealth( int( creature.getMaxHealth() * 0.73 ) )
	creature.setMovementType( arcemu.MOVEMENTTYPE_DONTMOVEWP )
	creature.resetWaypoint()
	
	creature.destroyCustomWaypoints()
	creature.createCustomWaypoint( -5662.362793, -454.450104, 392.654297, 0.088353, 250, arcemu.WAYPOINT_FLAG_RUN, 0 )
	creature.createCustomWaypoint( -5652.207520, -452.030426, 392.206996, 0.481052, 250, arcemu.WAYPOINT_FLAG_RUN, 0 )
	creature.createCustomWaypoint( -5638.114746, -442.093018, 394.157471, 0.661694, 250, arcemu.WAYPOINT_FLAG_RUN, 0 )
	
	
def MountaineerDolf_onHealed( unit, healer, spellId, amount ):
	if not healer.isPlayer():
		return
		
	creature = unit.toCreature()
		
	if creature.getStandState() == arcemu.STANDSTATE_KNEEL and spellId == SPELLID_LESSERHEAL_II:
		creature.setStandState( arcemu.STANDSTATE_STAND )
		creature.faceUnit( healer )
		creature.emote( arcemu.EMOTE_ONESHOT_BOW )
		creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Ah, priest, you came along just in time. I appreciate it." )
	
def MountaineerDolf_onApplyAura( unit, caster, spellId ):
	if not caster.isPlayer():
		return
		
	pu = caster.toPlayer()
	creature = unit.toCreature()
	
	if creature.getStandState() == arcemu.STANDSTATE_STAND and spellId == SPELLID_POWERWORD_FORTITUDE_I:
		pu.markQuestObjectiveAsComplete( QUESTID_GARMENTS_OF_THE_LIGHT_DWARF, 0, creature.getGUID() )
		creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Thank you! Thank you, priest. Now I can take on those bears with the Light on my side!" )
		creature.stopMovement( 2500 )
		creature.setMovementType( arcemu.MOVEMENTTYPE_FORWARDTHENSTOP )		
		creature.emote( arcemu.EMOTE_ONESHOT_CHEER )
		
def MountaineerDolf_onReachWP( unit, event, waypointId, forward ):
	creature = unit.toCreature()
	
	if forward:
		if waypointId == 1:
			creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Farewell to you, and may the Light be with you always." )
			
		elif waypointId == 3:
			creature.despawn( 0, 3000 )
		
arcemu.RegisterUnitEvent( CREATUREID_MOUNTAINEER_DOLF, arcemu.CREATURE_EVENT_ON_LOAD, MountaineerDolf_onLoad )
arcemu.RegisterUnitEvent( CREATUREID_MOUNTAINEER_DOLF, arcemu.CREATURE_EVENT_ON_HEALED, MountaineerDolf_onHealed )
arcemu.RegisterUnitEvent( CREATUREID_MOUNTAINEER_DOLF, arcemu.CREATURE_EVENT_ON_APPLY_AURA, MountaineerDolf_onApplyAura )
arcemu.RegisterUnitEvent( CREATUREID_MOUNTAINEER_DOLF, arcemu.CREATURE_EVENT_ON_REACH_WP, MountaineerDolf_onReachWP )
