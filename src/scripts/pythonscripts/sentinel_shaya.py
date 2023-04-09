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
# Creature AI script for Sentinel Shaya (12429)
# for the quest Garments of the Moon (5621)
# 
# The player should get quest credit after the following:
# * Heal the NPC with Lesser Heal II
# * Buff the NPC with Power Word: Fortitude I
#

import arcemu
from arcemu import Unit
from arcemu import Player

CREATUREID_SENTINEL_SHAYA = 12429
QUESTID_GARMENTS_OF_THE_MOON = 5621
SPELLID_LESSERHEAL_II = 2052
SPELLID_POWERWORD_FORTITUDE_I = 1243

def SentinelShaya_onLoad( unit, event ):
	unit.setPvPFlag()
	unit.setStandState( arcemu.STANDSTATE_KNEEL )
	unit.setCanRegenerateHP( 0 )
	unit.setHealth( int( unit.getMaxHealth() * 0.73 ) )
	unit.setMovementType( arcemu.MOVEMENTTYPE_DONTMOVEWP )
	unit.resetWaypoint()
	
	unit.destroyCustomWaypoints()
	unit.createCustomWaypoint( 9669.819336, 894.693054, 1281.398560, 3.689695, 250, arcemu.WAYPOINT_FLAG_RUN, 0 )
	unit.createCustomWaypoint( 9654.602539, 885.896912, 1269.025391, 3.539762, 250, arcemu.WAYPOINT_FLAG_RUN, 0 )	
	unit.createCustomWaypoint( 9637.291016, 878.614929, 1266.609985, 3.539762, 250, arcemu.WAYPOINT_FLAG_RUN, 0 )
	
	
def SentinelShaya_onHealed( unit, healer, spellId, amount ):
	if not healer.isPlayer():
		return
		
	if unit.getStandState() == arcemu.STANDSTATE_KNEEL and spellId == SPELLID_LESSERHEAL_II:
		unit.setStandState( arcemu.STANDSTATE_STAND )
		unit.faceUnit( healer )
		unit.emote( arcemu.EMOTE_ONESHOT_BOW )
		unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Ah, priest, you came along just in time. I appreciate it." )
	
def SentinelShaya_onApplyAura( unit, caster, spellId ):
	if not caster.isPlayer():
		return
		
	pu = caster.toPlayer()
	
	if unit.getStandState() == arcemu.STANDSTATE_STAND and spellId == SPELLID_POWERWORD_FORTITUDE_I:
		pu.markQuestObjectiveAsComplete( QUESTID_GARMENTS_OF_THE_MOON, 0 )
		unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Thank you! Thank you, priest. Now I can take on those timberlings with Elune on my side!" )
		unit.stopMovement( 2500 )
		unit.setMovementType( arcemu.MOVEMENTTYPE_FORWARDTHENSTOP )		
		unit.emote( arcemu.EMOTE_ONESHOT_CHEER )
		
def SentinelShaya_onReachWP( unit, event, waypointId, forward ):
	if forward:
		if waypointId == 1:
			unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Farewell to you, and may Elune be with you always." )
			
		elif waypointId == 3:
			unit.despawn( 0, 3000 )
		
arcemu.RegisterUnitEvent( CREATUREID_SENTINEL_SHAYA, arcemu.CREATURE_EVENT_ON_LOAD, SentinelShaya_onLoad )
arcemu.RegisterUnitEvent( CREATUREID_SENTINEL_SHAYA, arcemu.CREATURE_EVENT_ON_HEALED, SentinelShaya_onHealed )
arcemu.RegisterUnitEvent( CREATUREID_SENTINEL_SHAYA, arcemu.CREATURE_EVENT_ON_APPLY_AURA, SentinelShaya_onApplyAura )
arcemu.RegisterUnitEvent( CREATUREID_SENTINEL_SHAYA, arcemu.CREATURE_EVENT_ON_REACH_WP, SentinelShaya_onReachWP )
