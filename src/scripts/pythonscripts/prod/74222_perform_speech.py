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
# TODO: Missing visuals
#

import math
import arcemu

MEKKATORQUE_SPEECH_NPC_ID = 39712

PLAYBACK_SPEECH_SPELL_ID = 74209
PERFORM_SPEECH_SPELL_ID = 74222

NPC_OZZIE_TOGGLEVOLT = 1268
NPC_TOG_RUSTSPROCKET = 6119
NPC_MILLI_FEATHERWHISTLE = 7955

def PerformSpeech_handleDummyEffect( effectIndex, spell ):
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

	# Check if we cast the spell on the right NPC
	if creatureId != NPC_OZZIE_TOGGLEVOLT and creatureId != NPC_TOG_RUSTSPROCKET and creatureId != NPC_MILLI_FEATHERWHISTLE:
		spell.cancel()
		return True

	# Spawn Mekkatorque
	mapMgr = creature.getMapMgr()
	x = creature.getPositionX()
	y = creature.getPositionY()
	o = creature.getOrientation()
	spawnArc = ( 2 * math.pi ) - ( ( 2 * math.pi ) / 10.0 )
	
	x = x + 2 * math.cos( o + spawnArc )
	y = y + 2 * math.sin( o + spawnArc )
	
	spawn = mapMgr.spawnCreature( MEKKATORQUE_SPEECH_NPC_ID, x, y, creature.getPositionZ() )
	if spawn is not None:
		spawn.setScale( 0.5 )
		spawn.setUInt64Value( arcemu.UNIT_FIELD_CREATEDBY, playerCaster.getGUID() )
		spawn.setUInt64Value( arcemu.UNIT_FIELD_SUMMONEDBY, playerCaster.getGUID() )
		spawn.setUInt64Value( arcemu.UNIT_CREATED_BY_SPELL, PLAYBACK_SPEECH_SPELL_ID )
		spawn.faceUnit( unitTarget )
		
		# Sets the NPC who listens to the speech as target, so that the AI script will know who the speech is for
		spawn.setTargetGUID( unitTarget.getGUID() )
		
	return True

arcemu.RegisterDummySpellHandler( PERFORM_SPEECH_SPELL_ID, PerformSpeech_handleDummyEffect )
