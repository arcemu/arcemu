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
# Dummy spell handler script for the spell Shoot (74182) for the quest Press Fire (25295)
# The player casts this spell from a Shooting Mechano-Tank as test firing at a Mechano-Tank attack target
# We're supposed to hit the attack target to get credit.
# 
#
# 
#

import arcemu

EXPLOSIVE_SHEEP_SPELL_ID = 4050

def explosive_sheep_4050_handleDummyEffect( effectIndex, spell ):
	caster = spell.getCaster()
	c = arcemu.toCreature( caster )
	
	if c is not None:
		c.despawn( 1, 0 )
	
	return True

arcemu.RegisterDummySpellHandler( EXPLOSIVE_SHEEP_SPELL_ID, explosive_sheep_4050_handleDummyEffect )
