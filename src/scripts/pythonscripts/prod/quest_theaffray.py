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
import arcemu
import ArcPyMath as Math

THEAFFRAY_QUEST_ID = 1719
TWIGGY_FLATHEAD_NPC_ID = 6248
AFFRAY_CHALLENGER_NPC_ID = 6240

def TheAffray_onExploreArea( areaId, player, questId ):
	if areaId != 522:
		return

	mapMgr = player.getMapMgr()	
	twiggy = mapMgr.getCreatureNearestCoords( -1686.14, -4323.04, 4.28, TWIGGY_FLATHEAD_NPC_ID )
	if twiggy is not None:
		twiggy.activate( player )
		
	return
		
arcemu.RegisterQuestEvent( THEAFFRAY_QUEST_ID, arcemu.QUEST_EVENT_ON_EXPLORE_AREA, TheAffray_onExploreArea )

############################################################################################################################################

AFFRAY_CHALLENGER_DISPLAY_IDS = [ 4968, 4969, 4970, 4971 ]
AFFRAY_EMOTES = [
	arcemu.EMOTE_ONESHOT_NONE,
	arcemu.EMOTE_ONESHOT_CHEER,
	arcemu.EMOTE_ONESHOT_LAUGH,
	arcemu.EMOTE_ONESHOT_RUDE,
	arcemu.EMOTE_ONESHOT_ROAR	]

def AffrayChallenger_onLoad( unit, event ):	
	unit.setDisplayId( AFFRAY_CHALLENGER_DISPLAY_IDS[ Math.randomUInt( 3 ) ] )
	unit.RegisterAIUpdateEvent( 5000 )
	return
	
def AffrayChallenger_onAIUpdate( unit, event ):
	unit.emote( AFFRAY_EMOTES[ Math.randomUInt( 4 )  ] )
	return
	
def AffrayChallenger_onCombatStart( unit, event, target ):
	unit.removeAIUpdateEvent()
	return

def AffrayChallenger_onDied( unit, event, killer ):
	mapMgr = unit.getMapMgr()
	twiggy = mapMgr.getCreatureNearestCoords( -1686.14, -4323.04, 4.28, TWIGGY_FLATHEAD_NPC_ID )
	if twiggy is not None:
		twiggy.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Challenger is down!" )
		
	return
	
arcemu.RegisterUnitEvent( AFFRAY_CHALLENGER_NPC_ID, arcemu.CREATURE_EVENT_ON_LOAD, AffrayChallenger_onLoad )
arcemu.RegisterUnitEvent( AFFRAY_CHALLENGER_NPC_ID, arcemu.CREATURE_EVENT_ON_AIUPDATE, AffrayChallenger_onAIUpdate )
arcemu.RegisterUnitEvent( AFFRAY_CHALLENGER_NPC_ID, arcemu.CREATURE_EVENT_ON_ENTER_COMBAT, AffrayChallenger_onCombatStart )
arcemu.RegisterUnitEvent( AFFRAY_CHALLENGER_NPC_ID, arcemu.CREATURE_EVENT_ON_DIED, AffrayChallenger_onDied )

############################################################################################################################################

BIG_WILL_WPS = [
	[-1712.484619, -4342.101563, 6.033428, 6.187793],
	[-1704.122803, -4342.901855, 4.705498, 6.187793],
	[-1694.583862, -4343.577637, 4.385377, 6.265548],
	[-1687.913574, -4340.006348, 3.681255, 0.594973],
	[-1686.386963, -4336.733398, 3.146416, 1.176167],
	[-1684.013428, -4328.933594, 2.787777, 1.136898]
]

BIG_WILL_NPC_ID = 6238

def BigWill_onLoad( unit, event ):
	bigwill = arcemu.toCreature( unit )
	
	bigwill.setFaction( 35 )
	bigwill.resetWaypoint()
	
	bigwill.destroyCustomWaypoints()
	for wp in BIG_WILL_WPS:
		bigwill.createCustomWaypoint( wp[ 0 ], wp[ 1 ], wp[ 2 ], wp[ 3 ], 250, arcemu.WAYPOINT_FLAG_RUN, 0 )
		
	bigwill.setMovementType( arcemu.MOVEMENTTYPE_FORWARDTHENSTOP )
	
	return
	
def BigWill_onReachWP( unit, event, waypointId ,forward ):
	bigwill = arcemu.toCreature( unit )
	
	if forward:
		if waypointId == 6:
			bigwill.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Ready when you are, warrior!" )
			bigwill.setFaction( 189 )
			bigwill.setMovementType( arcemu.MOVEMENTTYPE_DONTMOVEWP )
	
	return
	
arcemu.RegisterUnitEvent( BIG_WILL_NPC_ID, arcemu.CREATURE_EVENT_ON_LOAD, BigWill_onLoad )
arcemu.RegisterUnitEvent( BIG_WILL_NPC_ID, arcemu.CREATURE_EVENT_ON_REACH_WP, BigWill_onReachWP )

############################################################################################################################################

THEAFFRAY_CHALLENGER_COORDS = [
	[-1675.129761, -4324.336426, 2.787686],
	[-1674.289429, -4325.222656, 2.787686],
	[-1674.365356, -4326.533203, 2.787686],
	[-1684.320923, -4330.712891, 2.787613],
	[-1683.388428, -4333.585449, 2.787884],
	[-1677.736084, -4334.410156, 2.788052],
]

THEAFFRAY_CHALLENGER_DELAY = 20

TWIGGY_FLATHEAD_STATE = {}

class TwiggyState:
	state = 0
	timer = 0
	player = 0
	challengers = []
	bigwillGUID = 0
	timeout = 0
	
	def __init__( self, state, timer, playerGUID ):
		self.state = state
		self.timer = timer
		self.player = playerGUID
		return

def TwiggyFlathead_onActivate( unit, activator ):
	# If the event is in progress, let's not restart
	guid = unit.getGUID()
	if guid in TWIGGY_FLATHEAD_STATE:
		return
		
	state = TwiggyState( 0, 0, activator.getGUID() )
	state.timeout = arcemu.getGameTime() + 10 * 60 * 1000
	TWIGGY_FLATHEAD_STATE[ guid ] = state
		
	mapMgr = unit.getMapMgr()
	
	# Spawn challengers
	for i in range( 0, 6 ):
		coords = THEAFFRAY_CHALLENGER_COORDS[ i ]
		challenger = mapMgr.spawnCreature( AFFRAY_CHALLENGER_NPC_ID, coords[ 0 ], coords[ 1 ], coords[ 2 ] )
		if challenger is not None:
			challenger.setFaction( 35 )
			challenger.faceUnit( activator )
			challenger.equipWeapons( 923, 0, 0 )
			challenger.setSheatState( arcemu.SHEATSTATE_SHEATHED )
			challenger.despawn( 5 * 60 * 1000, 0 )
	
	unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "The Affray has begun. " + activator.getName() + ", get ready to fight!" )
	unit.RegisterAIUpdateEvent( 1000 )
	return
	
def TwiggyFlathead_onDeactivate( unit ):
	unit.RemoveAIUpdateEvent()	
	guid = unit.getGUID()
	
	if guid in TWIGGY_FLATHEAD_STATE:
		del TWIGGY_FLATHEAD_STATE[ guid ]
		
	return
		
def TwiggyFlathead_activateChallenger( unit, playerGUID, idx ):
	twiggy = arcemu.toCreature( unit )
	guid = twiggy.getGUID()
	
	if guid not in TWIGGY_FLATHEAD_STATE:
		return
		
	state = TWIGGY_FLATHEAD_STATE[ guid ]
		
	mapMgr = twiggy.getMapMgr()
	player = mapMgr.getUnit( playerGUID )
	if player is None:
		return
		
	coords = THEAFFRAY_CHALLENGER_COORDS[ idx ]
	challenger = mapMgr.getCreatureNearestCoords( coords[ 0 ], coords[ 1 ], coords[ 2 ], AFFRAY_CHALLENGER_NPC_ID )
	if challenger is None:
		return
		
	twiggy.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "You! Enter the fray! " )
	challenger.setFaction( 16 )
	challenger.wipeTargetList()
	challenger.setNextTarget( player )
	
	state.challengers.append( challenger.getGUID() )
	TWIGGY_FLATHEAD_STATE[ guid ] = state		
	return
	
def TwiggyFlathead_onAIUpdate( unit, event ):
	twiggy = arcemu.toCreature( unit )
	guid = twiggy.getGUID()
	
	if guid not in TWIGGY_FLATHEAD_STATE:
		return
		
	state = TWIGGY_FLATHEAD_STATE[ guid ]
	
	time = arcemu.getGameTime()
	
	# state 0 - Spawn a challenger immediately
	if state.state == 0:
		TwiggyFlathead_activateChallenger( unit, state.player, 0 )
		state.state = 1
		state.timer = time + THEAFFRAY_CHALLENGER_DELAY
	
	# Every 20 s spawn a new challenger
	elif state.state >= 1 and state.state <= 5:
		if time >= state.timer:
			TwiggyFlathead_activateChallenger( unit, state.player, state.state )
			state.state += 1
			state.timer = time + THEAFFRAY_CHALLENGER_DELAY
		
	# Wait until all challengers are dead then proceed to call up Big Will
	elif state.state == 6:
		challengersDead = True
		
		mapMgr = twiggy.getMapMgr()
		
		for challengerGUID in state.challengers:
			challenger = mapMgr.getUnit( challengerGUID )
			if challenger is not None:
				if challenger.getHealth() != 0:
					challengersDead = False
					break
					
		if challengersDead:
			state.challengers = []
			state.state = 7
		
	# Spawn Big Will
	elif state.state == 7:
		twiggy.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "The first stage is over! Big Will... Come on down!" )
		mapMgr = twiggy.getMapMgr()
		bigwill = mapMgr.spawnCreature( BIG_WILL_NPC_ID, BIG_WILL_WPS[ 0 ][ 0 ], BIG_WILL_WPS[ 0 ][ 1 ], BIG_WILL_WPS[ 0 ][ 2 ] )
		if bigwill is not None:
			bigwill.despawn( 5 * 60 * 1000, 0 )
			state.bigwillGUID = bigwill.getGUID()
		
		state.state = 8
		
	# Wait until Big Will dies
	elif state.state == 8:
		mapMgr = twiggy.getMapMgr()
		bigwill = mapMgr.getUnit( state.bigwillGUID )
		if bigwill == None or bigwill.getHealth() == 0:
			state.state = 9

	# Event is over - deactivate
	if state.state == 9 or time >= state.timeout:
		twiggy.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "The affray is over!" )
		twiggy.deactivate()
	else:
		TWIGGY_FLATHEAD_STATE[ guid ] = state
		
	#print( str( time ) )
	
	return
	
arcemu.RegisterUnitEvent( TWIGGY_FLATHEAD_NPC_ID, arcemu.CREATURE_EVENT_ON_ACTIVATE, TwiggyFlathead_onActivate )
arcemu.RegisterUnitEvent( TWIGGY_FLATHEAD_NPC_ID, arcemu.CREATURE_EVENT_ON_DEACTIVATE, TwiggyFlathead_onDeactivate )
arcemu.RegisterUnitEvent( TWIGGY_FLATHEAD_NPC_ID, arcemu.CREATURE_EVENT_ON_AIUPDATE, TwiggyFlathead_onAIUpdate )
