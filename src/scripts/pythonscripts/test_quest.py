import arcemu

from arcemu import Player
from arcemu import Unit

def quest_onStart( player, questId ):
	print( player.getName() + " has started quest " + str( questId ) )
	
def quest_onComplete( player, questId ):
	print( player.getName() + " has completed quest " + str( questId ) )
	
def quest_onCancel( player ):
	print( player.getName() + " has canceled a quest." )
	
def quest_onGameObjectActivate( goId, player, questId ):
	print( player.getName() + " has activated gameobject " + str( goId ) + " while on quest " + str( questId ) )
	
def quest_onCreatureKill( creatureId, player, questId ):
	print( player.getName() + " has killed creature " + str( creatureId ) + " while on quest " + str( questId ) )
	
def quest_onExploreArea( areaId, player, questId ):
	print( player.getName() + " has explored area " + str( areaId ) + " while on quest " + str( questId ) )

def quest_onPlayerItemPickup( itemId, total, player, questId ):
	print( player.getName() + " has picked up item " + str( itemId ) + " total: " + str( total ) + " while on quest " + str( questId ) )
	
arcemu.RegisterQuestEvent( 7, arcemu.QUEST_EVENT_ON_ACCEPT, quest_onStart )
arcemu.RegisterQuestEvent( 7, arcemu.QUEST_EVENT_ON_COMPLETE, quest_onComplete )
arcemu.RegisterQuestEvent( 7, arcemu.QUEST_EVENT_ON_CANCEL, quest_onCancel )
arcemu.RegisterQuestEvent( 498, arcemu.QUEST_EVENT_GAMEOBJECT_ACTIVATE, quest_onGameObjectActivate )
arcemu.RegisterQuestEvent( 7, arcemu.QUEST_EVENT_ON_CREATURE_KILL, quest_onCreatureKill )
arcemu.RegisterQuestEvent( 62, arcemu.QUEST_EVENT_ON_EXPLORE_AREA, quest_onExploreArea )
arcemu.RegisterQuestEvent( 33, arcemu.QUEST_EVENT_ON_PLAYER_ITEMPICKUP, quest_onPlayerItemPickup )
