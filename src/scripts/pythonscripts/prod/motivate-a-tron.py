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
import ArcPyMath as Math

MOTIVATE_SPELL_ID = 74035
AFGG_QUEST_ID = 25229

def motivate_handleDummyEffect( effectIndex, spell ):
	playerCaster = spell.getPlayerCaster()
	unitTarget = spell.getUnitTarget()

	if playerCaster is None:
		return True
		
	if unitTarget is None:
		return True
		
	creature = unitTarget.toCreature()
	if creature is None:
		return True
		
	creatureId = creature.getId()
		
	# The device should only work on Gnome Citizens
	if creatureId != 39253 and creatureId != 39623:
		return True
	
	# The device is not perfect, it can malfunction
	spells = [ 74046, 74062, 0 ]
	spell = spells[ Math.randomUInt( 2 ) ]
	
	# In case it works we select the right spell based on the target creature's Id
	# Then give credit for having motivated the citizen
	if spell == 0:
		if creatureId == 39253:
			spell = 73943
		else:
			spell = 74080
		
		playerCaster.addQuestKill( AFGG_QUEST_ID, 0, 0 )
		
	playerCaster.castSpell( spell, False, unitTarget )
		
	return True

arcemu.RegisterDummySpellHandler( MOTIVATE_SPELL_ID, motivate_handleDummyEffect )
