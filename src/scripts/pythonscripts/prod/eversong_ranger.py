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
import arcemu
from arcemu import Unit
from arcemu import Player

CREATUREID_EVERSONG_RANGER = 15938
SPELLID_POWERWORD_FORTITUDE_I = 1243

def EversongRanger_onLoad( unit, event ):
	unit.setPvPFlag()
	unit.setStandState( arcemu.STANDSTATE_STAND )

def EversongRanger_onApplyAura( unit, caster, spellId ):
	if not caster.isPlayer():
		return
		
	pu = caster.toPlayer()
	
	if spellId == SPELLID_POWERWORD_FORTITUDE_I:
		unit.faceUnit( caster )
		unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Your faith guides me to victory!" )
		
		# The salute emote keeps repeating, so reset it after 1 sec
		unit.emote( 66 )
		unit.emote( arcemu.EMOTE_ONESHOT_NONE, 1000 )
		
arcemu.RegisterUnitEvent( CREATUREID_EVERSONG_RANGER, arcemu.CREATURE_EVENT_ON_LOAD, EversongRanger_onLoad )
arcemu.RegisterUnitEvent( CREATUREID_EVERSONG_RANGER, arcemu.CREATURE_EVENT_ON_APPLY_AURA, EversongRanger_onApplyAura )
