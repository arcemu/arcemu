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

MOTIVATED_GNOME_EXCLAMATIONS = [
"Anything for King Mekkatorque!",
"Can I bring my wrench?",
"Is this going to hurt?",
"I'd love to help!",
"Sign me up!",
"Wow! We're taking back Gnomeregan? I'm in!",
"My wrench of vengance awaits!",
"I want to drive a Spider Tank!"
]

AFGG_QUEST_ID = 25229

def motivatedcitizen_onAIUpdate( unit, event ):
	# If we're near the quest finisher give credit and despawn
	#creature = unit.toCreature()
	#playerOwner = creature.getPlayerOwner()
	#playerOwner.addQuestKill( AFGG_QUEST_ID, 1, 0 )
	return

def motivatedcitizen_onLoad( unit, event ):
	creature = unit.toCreature()

	unit.RegisterAIUpdateEvent( 1000 )
	
	# These creatures have an invisible display by default, let's set one based on the creature
	if creature.getId() == 39466:
		creature.setDisplayId( 2590 )
	else:
		creature.setDisplayId( 2581 )
	
	exclamation = MOTIVATED_GNOME_EXCLAMATIONS[ Math.randomUInt( len( MOTIVATED_GNOME_EXCLAMATIONS ) ) ]
	
	# Send the exclamation message with a little delay, so the client has time to notice the spawning NPC
	unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, exclamation, 1000 )

arcemu.RegisterUnitEvent( 39466, arcemu.CREATURE_EVENT_ON_LOAD, motivatedcitizen_onLoad )
arcemu.RegisterUnitEvent( 39466, arcemu.CREATURE_EVENT_ON_AIUPDATE, motivatedcitizen_onAIUpdate )
arcemu.RegisterUnitEvent( 39624, arcemu.CREATURE_EVENT_ON_LOAD, motivatedcitizen_onLoad )
arcemu.RegisterUnitEvent( 39624, arcemu.CREATURE_EVENT_ON_AIUPDATE, motivatedcitizen_onAIUpdate )
