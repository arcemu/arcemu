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

STEAMCRANK_TEXTS = [
"All right, you twisted mess of broken gears, let's get to work!",
"I am going to teach you all what it takes to be a proper soldier!",
"The first thing you need to learn is proper discipline.",
"Show me that discipline with a proper salute when I say so!",
"Okay, recruits, salute!",
"Nice job!",
"When you go into combat, it is important that you know how to intimidate the enemy with a deafening battle roar!",
"When I give the signal, let loose your greatest roar!",
"Let me hear that battle roar!",
"Wow, nice and scary!",
"Remember though, a key factor in winning any battle is positive reinforcement.",
"Reward your fellow soldiers in battle with a resounding cheer. Now, cheer at me when I tell you to!",
"Everyone, cheer!",
"Fantastic!",
"However, the most important part of battle is how you celebrate a good hard earned victory!",
"Show me your best victory dance when I give the signal!",
"Now, dance!",
"Superb!",
"You might be the best set of recruits I've ever seen! Let's go through all of that again!"
]

STEAMCRANK_STATE = {
}

SARGE_STEAMCRANK_ID = 39368
GNOME_TRAINEE_ID = 39349
BASIC_ORDERS_QUEST_ID = 25199

def makeTraineesEmote( unit, emote ):
	objects = unit.getObjectsInRange()
	
	for o in objects:
		c = arcemu.toCreature( o )
		if c is not None and c.getId() == GNOME_TRAINEE_ID:
			c.emote( emote )
			c.emote( arcemu.EMOTE_ONESHOT_NONE, 1000 )

def sarge_steamcrank_onAIUpdate( unit, event ):
	guid = unit.getGUID()
	# After reloading scripts this is uninitialized
	if guid not in STEAMCRANK_STATE:
		STEAMCRANK_STATE[ guid ] = 0
		
	state = STEAMCRANK_STATE[ guid ]
	
	unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, STEAMCRANK_TEXTS[ state ] )
	
	if state == 0:
		makeTraineesEmote( unit, arcemu.EMOTE_STATE_NONE )		
	elif state == 4:
		makeTraineesEmote( unit, arcemu.EMOTE_ONESHOT_SALUTE )
	elif state == 8:
		makeTraineesEmote( unit, arcemu.EMOTE_ONESHOT_ROAR )
	elif state == 12:
		makeTraineesEmote( unit, arcemu.EMOTE_ONESHOT_CHEER )
	elif state == 16:
		makeTraineesEmote( unit, arcemu.EMOTE_ONESHOT_DANCE )
	
	if state == 18:
		state = 0
	else:
		state = state + 1
		
	STEAMCRANK_STATE[ unit.getGUID() ] = state
	
def sarge_steamcrank_onEmote( unit, event, player, emote ):
	state = STEAMCRANK_STATE[ unit.getGUID() ]
	
	if state == 5 and emote == arcemu.EMOTE_ONESHOT_SALUTE:
		player.markQuestObjectiveAsComplete( BASIC_ORDERS_QUEST_ID, 0, unit.getGUID() )
	elif state == 9 and emote == arcemu.EMOTE_ONESHOT_ROAR:
		player.markQuestObjectiveAsComplete( BASIC_ORDERS_QUEST_ID, 1, unit.getGUID() )
	elif state == 13 and emote == arcemu.EMOTE_ONESHOT_CHEER:
		player.markQuestObjectiveAsComplete( BASIC_ORDERS_QUEST_ID, 2, unit.getGUID() )
	elif state == 17 and emote == arcemu.EMOTE_STATE_DANCE:
		player.markQuestObjectiveAsComplete( BASIC_ORDERS_QUEST_ID, 3, unit.getGUID() )
	
def sarge_steamcrank_onLoad( unit, event ):
	STEAMCRANK_STATE[ unit.getGUID() ] = 0
	unit.RegisterAIUpdateEvent( 5000 )

arcemu.RegisterUnitEvent( SARGE_STEAMCRANK_ID, arcemu.CREATURE_EVENT_ON_LOAD, sarge_steamcrank_onLoad )
arcemu.RegisterUnitEvent( SARGE_STEAMCRANK_ID, arcemu.CREATURE_EVENT_ON_EMOTE, sarge_steamcrank_onEmote )
arcemu.RegisterUnitEvent( SARGE_STEAMCRANK_ID, arcemu.CREATURE_EVENT_ON_AIUPDATE, sarge_steamcrank_onAIUpdate )
