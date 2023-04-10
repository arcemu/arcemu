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
# Creature AI script for Tavara (17551)
# for the quest Help Tavara (9586)
# 
# The player should get quest credit after the following:
# * Heal the NPC
#

import arcemu
from arcemu import Unit
from arcemu import Player

CREATUREID_TAVARA = 17551
QUESTID_HELP_TAVARA = 9586

def Tavara_onLoad( unit, event ):
	unit.setPvPFlag()
	unit.setStandState( arcemu.STANDSTATE_KNEEL )
	unit.setCanRegenerateHP( 0 )
	unit.setHealth( int( unit.getMaxHealth() * 0.73 ) )
	unit.setMovementType( arcemu.MOVEMENTTYPE_DONTMOVEWP )
	unit.resetWaypoint()
	
	unit.destroyCustomWaypoints()
	unit.createCustomWaypoint( -4123.090332, -12778.934570, 10.249404, 1.815878, 250, arcemu.WAYPOINT_FLAG_RUN, 0 )
	unit.createCustomWaypoint( -4131.844238, -12747.574219, 15.111283, 1.796243, 250, arcemu.WAYPOINT_FLAG_RUN, 0 )	

	
def Tavara_onHealed( unit, healer, spellId, amount ):
	if not healer.isPlayer():
		return
		
	player = healer.toPlayer()
		
	if unit.getStandState() == arcemu.STANDSTATE_KNEEL:
		unit.setStandState( arcemu.STANDSTATE_STAND )
		unit.faceUnit( healer )
		unit.emote( arcemu.EMOTE_ONESHOT_BOW )
		unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Thank you for helping me!" )
		player.markQuestObjectiveAsComplete( QUESTID_HELP_TAVARA, 0 )
		unit.stopMovement( 2500 )
		unit.setMovementType( arcemu.MOVEMENTTYPE_FORWARDTHENSTOP )
	
def Tavara_onReachWP( unit, event, waypointId, forward ):
	if forward:
		if waypointId == 1:
			unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Farewell to you, and may the Naaru be with you always." )
			
		elif waypointId == 2:
			unit.despawn( 0, 3000 )
		
arcemu.RegisterUnitEvent( CREATUREID_TAVARA, arcemu.CREATURE_EVENT_ON_LOAD, Tavara_onLoad )
arcemu.RegisterUnitEvent( CREATUREID_TAVARA, arcemu.CREATURE_EVENT_ON_HEALED, Tavara_onHealed )
arcemu.RegisterUnitEvent( CREATUREID_TAVARA, arcemu.CREATURE_EVENT_ON_REACH_WP, Tavara_onReachWP )
