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

def tread_sparknozzle_onAIUpdate( unit, event ):
	objects = unit.getObjectsInRange()
	
	for o in objects:
		c = arcemu.toCreature( o )
		if c is not None:
			if unit.calcDistance( c ) <= 5:
				if c.getId() == 39466 or c.getId() == 39624:
					owner = c.getPlayerOwner()
					if owner is not None:
						c.despawn( 500, 0 )
						owner.addQuestKill( 25229, 1, 0 )
	
def tread_sparknozzle_onLoad( unit, event ):
	unit.RegisterAIUpdateEvent( 1000 )

arcemu.RegisterUnitEvent( 39675, arcemu.CREATURE_EVENT_ON_LOAD, tread_sparknozzle_onLoad )
arcemu.RegisterUnitEvent( 39675, arcemu.CREATURE_EVENT_ON_AIUPDATE, tread_sparknozzle_onAIUpdate )
