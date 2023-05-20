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

MEKKATORQUE_SPEECH_TEXTS = [
	# Ozzie
	[
		"What I want out of each and every one of you is a hard-target search of every refuelling station, residence, warehouse, farmhouse, henhouse, outhouse and doghouse in his area.",
		"Your fugitive's name is Mekgineer Thermaplugg",
		"Go get him!"
	],

	# Milli
	[
		"They may take our lives, but they'll never take...",
		"... our INNOVATION!"
	],

	# Tog
	[
		"We Will not go quietly into the night! We will not vanish without a fight!",
		"We're going to live on! We're going to survive! Today we celebrate...",
		"...our Autonomy Day!"
	]
]

MEKKATORQUE_REACTION_TEXTS = [
	# Ozzie
	"Hmm. I suppose it could work. But it could really use some more umph",
	
	# Milli
	"What? I don't even know what you are talking about! That's terrible!",
	
	# Tog
	"Horrible! Well, all right, maybe it just needs a little cleaning up?"
]

MEKKATORQUE_REACTION_SPELLS = [ 74154, 74155, 74156 ]

MEKKATORQUE_SPEECH_STATE = {
}

MEKKATORQUE_SPEECH_NPC_ID = 39712

# Returns the target of Mekkatorque ( the NPC who listens to the speech )
# The target is set by the dummy spell handler that summon Mekkatorque
def MekkaTorque_getTargetCreature( unit ):
	creatureGUID = unit.getTargetGUID()
	if creatureGUID != 0:
		mapMgr = unit.getMapMgr()
		
		targetUnit = mapMgr.getUnit( creatureGUID )
		if targetUnit is not None:
			targetCreature = targetUnit.toCreature()
			return targetCreature
			
	return None
	
# Returns the creator of MekkaTorque (the player)
def MekkaTorque_getCreator( unit ):
	guid = unit.getUInt64Value( arcemu.UNIT_FIELD_CREATEDBY )
	if guid != 0:
		mapMgr = unit.getMapMgr()
		creator = mapMgr.getUnit( guid )
		return creator
	
	return None
	
# Sends the reaction text message, and casts the quest credit spell on the player
def MekkaTorque_speechReaction( unit, idx ):
	targetCreature = MekkaTorque_getTargetCreature( unit )
	if targetCreature is not None:
		targetCreature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, MEKKATORQUE_REACTION_TEXTS[ idx ] )
		
		creator = MekkaTorque_getCreator( unit )
		if creator is not None:
			targetCreature.castSpell( MEKKATORQUE_REACTION_SPELLS[ idx ], False, creator )

def Mekkatorque_Speech_npc_onAIUpdate( unit, event ):
	guid = unit.getGUID()

	if guid not in MEKKATORQUE_SPEECH_STATE:
		MEKKATORQUE_SPEECH_STATE[ guid ] = 0
		
	state = MEKKATORQUE_SPEECH_STATE[ guid ]
	
	if state == 0:
		targetCreature = MekkaTorque_getCreature( unit, unit.getTargetGUID() )
		if targetCreature is not None:
			if targetCreature.getId() == 1268:
				state = 1
			elif targetCreature.getId() == 6119:
				state = 8
			elif targetCreature.getId() == 7955:
				state = 5
				
	elif state == 1:
		unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, MEKKATORQUE_SPEECH_TEXTS[ 0 ][ 0 ] )
		state = 2
		
	elif state == 2:
		unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, MEKKATORQUE_SPEECH_TEXTS[ 0 ][ 1 ] )
		state = 3
		
	elif state == 3:
		unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, MEKKATORQUE_SPEECH_TEXTS[ 0 ][ 2 ] )
		state = 4
		
	elif state == 4:
		MekkaTorque_speechReaction( unit, 0 )		
		state = 200
		
						
	elif state == 5:
		unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, MEKKATORQUE_SPEECH_TEXTS[ 1 ][ 0 ] )
		state = 6
		
	elif state == 6:
		unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, MEKKATORQUE_SPEECH_TEXTS[ 1 ][ 1 ] )
		state = 7
		
	elif state == 7:
		MekkaTorque_speechReaction( unit, 1 )		
		state = 200
		
	elif state == 8:
		unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, MEKKATORQUE_SPEECH_TEXTS[ 2 ][ 0 ] )
		state = 9
		
	elif state == 9:
		unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, MEKKATORQUE_SPEECH_TEXTS[ 2 ][ 1 ] )
		state = 10
		
	elif state == 10:
		unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, MEKKATORQUE_SPEECH_TEXTS[ 2 ][ 2 ] )
		state = 11
		
	elif state == 11:
		MekkaTorque_speechReaction( unit, 2 )		
		state = 200
		
	elif state == 200:
		unit.RemoveAIUpdateEvent()
		unit.toCreature().despawn( 1, 0 )
		
	MEKKATORQUE_SPEECH_STATE[ unit.getGUID() ] = state
	
def Mekkatorque_Speech_npc_onLoad( unit, event ):
	MEKKATORQUE_SPEECH_STATE[ unit.getGUID() ] = 0
	unit.RegisterAIUpdateEvent( 2000 )

arcemu.RegisterUnitEvent( MEKKATORQUE_SPEECH_NPC_ID, arcemu.CREATURE_EVENT_ON_LOAD, Mekkatorque_Speech_npc_onLoad )
arcemu.RegisterUnitEvent( MEKKATORQUE_SPEECH_NPC_ID, arcemu.CREATURE_EVENT_ON_AIUPDATE, Mekkatorque_Speech_npc_onAIUpdate )
