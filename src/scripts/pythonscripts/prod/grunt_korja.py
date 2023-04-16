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
# Creature AI script for Grunt Kor'ja (12430)
# for the quest Garments of Spirituality (5648)
# 
# The player should get quest credit after the following:
# * Heal the NPC with Lesser Heal II
# * Buff the NPC with Power Word: Fortitude I
#

import arcemu
from arcemu import Unit
from arcemu import Player

CREATUREID_GRUNT_KORJA = 12430
QUESTID_GARMENTS_OF_SPIRITUALITY = 5648
SPELLID_LESSERHEAL_II = 2052
SPELLID_POWERWORD_FORTITUDE_I = 1243

def GruntKorja_onLoad( unit, event ):
	creature = unit.toCreature()
	
	creature.setPvPFlag()
	creature.setStandState( arcemu.STANDSTATE_KNEEL )
	creature.setCanRegenerateHP( 0 )
	creature.setHealth( int( creature.getMaxHealth() * 0.73 ) )
	creature.setMovementType( arcemu.MOVEMENTTYPE_DONTMOVEWP )
	creature.resetWaypoint()
	
	creature.destroyCustomWaypoints()
	creature.createCustomWaypoint( 164.528015, -4775.344727, 14.596917, 3.848427, 250, arcemu.WAYPOINT_FLAG_RUN, 0 )
	creature.createCustomWaypoint( 129.442719, -4784.962402, 15.120217, 3.365407, 250, arcemu.WAYPOINT_FLAG_RUN, 0 )
	
def GruntKorja_onHealed( unit, healer, spellId, amount ):
	if not healer.isPlayer():
		return
		
	creature = unit.toCreature()
		
	if creature.getStandState() == arcemu.STANDSTATE_KNEEL and spellId == SPELLID_LESSERHEAL_II:
		creature.setStandState( arcemu.STANDSTATE_STAND )
		creature.faceUnit( healer )
		creature.emote( arcemu.EMOTE_ONESHOT_BOW )
		creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Ah, priest, you came along just in time. I appreciate it." )
	
def GruntKorja_onApplyAura( unit, caster, spellId ):
	if not caster.isPlayer():
		return
		
	creature = unit.toCreature()
	pu = caster.toPlayer()
	
	if creature.getStandState() == arcemu.STANDSTATE_STAND and spellId == SPELLID_POWERWORD_FORTITUDE_I:
		pu.markQuestObjectiveAsComplete( QUESTID_GARMENTS_OF_SPIRITUALITY, 0, creature.getGUID() )
		creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Thank you! Thank you, priest. Now I can take on those humans with your blessing!" )
		creature.stopMovement( 2500 )
		creature.setMovementType( arcemu.MOVEMENTTYPE_FORWARDTHENSTOP )		
		creature.emote( arcemu.EMOTE_ONESHOT_CHEER )
		
def GruntKorja_onReachWP( unit, event, waypointId, forward ):
	creature = unit.toCreature()
	
	if forward:
		if waypointId == 1:
			creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Farewell to you, and may the spirits be with you always." )
			
		elif waypointId == 2:
			creature.despawn( 0, 3000 )
		
arcemu.RegisterUnitEvent( CREATUREID_GRUNT_KORJA, arcemu.CREATURE_EVENT_ON_LOAD, GruntKorja_onLoad )
arcemu.RegisterUnitEvent( CREATUREID_GRUNT_KORJA, arcemu.CREATURE_EVENT_ON_HEALED, GruntKorja_onHealed )
arcemu.RegisterUnitEvent( CREATUREID_GRUNT_KORJA, arcemu.CREATURE_EVENT_ON_APPLY_AURA, GruntKorja_onApplyAura )
arcemu.RegisterUnitEvent( CREATUREID_GRUNT_KORJA, arcemu.CREATURE_EVENT_ON_REACH_WP, GruntKorja_onReachWP )
