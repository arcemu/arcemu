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
#

import arcemu

EJECTOR_EJECT_SPELL_ID = 74153
IN_AND_OUT_QUEST_ID = 25285

def ejector_eject_handleScriptedEffect( effectIndex, spell ):
	target = spell.getUnitTarget()
	ejector = arcemu.toCreature( target )
	if ejector is None:
		return True
		
	controller = ejector.getVehicleController()
	if controller is None:
		return True
		
		
	player = controller.toPlayer()
	if player is None:
		return True
		
	ejector.ejectAllPassengers()
	player.knockBack( 3.0, 10.0 )
	player.addQuestKill( IN_AND_OUT_QUEST_ID, 0, 0 )
	ejector.kill()
		
	return True

arcemu.RegisterScriptedEffectHandler( EJECTOR_EJECT_SPELL_ID, ejector_eject_handleScriptedEffect )
