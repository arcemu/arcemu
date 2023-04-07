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
QUESTID_GARMENTS_OF_THE_LIGHT = 5624
SPELLID_LESSERHEAL_II = 2052
SPELLID_POWERWORD_FORTITUDE_I = 1243

def GuardRoberts_onLoad( unit, event ):
	unit.setPvPFlag()
	unit.setStandState( arcemu.STANDSTATE_KNEEL )
	unit.setHealth( int( unit.getMaxHealth() * 0.73 ) )
	
def GuardRoberts_onHealed( unit, healer, spellId, amount ):
	if not healer.isPlayer():
		return
		
	if unit.getStandState() == arcemu.STANDSTATE_KNEEL and spellId == SPELLID_LESSERHEAL_II:
		unit.setStandState( arcemu.STANDSTATE_STAND )
		unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Ah, priest, you came along just in time. I appreciate it." )
	
def GuardRoberts_onApplyAura( unit, caster, spellId ):
	if not caster.isPlayer():
		return
		
	pu = caster.toPlayer()
	
	if unit.getStandState() == arcemu.STANDSTATE_STAND and spellId == SPELLID_POWERWORD_FORTITUDE_I:
		pu.markQuestObjectiveAsComplete( QUESTID_GARMENTS_OF_THE_LIGHT, 0 )
		unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Thank you! Thank you, priest. Now I can take on those murlocs with the Light on my side!" )
		unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Farewell to you, and may the Light be with you always." )
		unit.despawn( 3000, 3000 )
	
arcemu.RegisterUnitEvent( CREATUREID_GUARD_ROBERTS, arcemu.CREATURE_EVENT_ON_LOAD, GuardRoberts_onLoad )
arcemu.RegisterUnitEvent( CREATUREID_GUARD_ROBERTS, arcemu.CREATURE_EVENT_ON_HEALED, GuardRoberts_onHealed )
arcemu.RegisterUnitEvent( CREATUREID_GUARD_ROBERTS, arcemu.CREATURE_EVENT_ON_APPLY_AURA, GuardRoberts_onApplyAura )
