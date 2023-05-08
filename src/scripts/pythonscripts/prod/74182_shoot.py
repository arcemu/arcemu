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
# Note: This implementation is not the same as Blizzard's as that requires more spell work in Arcemu
#

import arcemu

MECHANO_TANK_ATTACK_TARGET_NPC = 39711
SHOT_IMPACT_VISUAL_SPELL_ID = 74179
SHOOT_74182_SPELL_ID = 74182
SHOOT_CREDIT_SPELL_ID = 74184

def shoot_74182_handleDummyEffect( effectIndex, spell ):
	caster = spell.getCaster()
	unitCaster = arcemu.toUnit( caster )
	
	if unitCaster is not None:
		objects = unitCaster.getObjectsInRange()
		for o in objects:
			c = arcemu.toCreature( o )
			if c is not None:
				if unitCaster.calcDistance( c ) <= 20:
					if c.getId() == MECHANO_TANK_ATTACK_TARGET_NPC:
						if unitCaster.isInArc( c, 20 ):
							c.castSpell( SHOT_IMPACT_VISUAL_SPELL_ID, False, c )
							controller = unitCaster.getVehicleController()
							if controller is not None:
								player = controller.toPlayer()
								if player is not None:
									player.castSpell( SHOOT_CREDIT_SPELL_ID, False, player )
							break
		
	return True

arcemu.RegisterDummySpellHandler( SHOOT_74182_SPELL_ID, shoot_74182_handleDummyEffect )
