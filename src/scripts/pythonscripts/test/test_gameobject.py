import arcemu
from arcemu import GameObject
from arcemu import Player

def go_onCreate( go ):
	print( "GameObject being created: " + go.getName() + "(" + str( go.getId() ) + ")" )
	
def go_onSpawn( go ):
	print( "GameObject being spawned: " + go.getName() + "(" + str( go.getId() ) + ")" )
	#go.RegisterAIUpdateEvent( 5000 )
	
def go_onDespawn( go ):
	print( "GameObject being despawned: " + go.getName() + "(" + str( go.getId() ) + ")" )
	
def go_onAIUpdate( go ):
	print( "GameObject AI update: " + go.getName() + "(" + str( go.getId() ) + ")" )

arcemu.RegisterGameObjectEvent( 202242, arcemu.GO_EVENT_ON_CREATE, go_onCreate )
arcemu.RegisterGameObjectEvent( 202242, arcemu.GO_EVENT_ON_SPAWN, go_onSpawn )
arcemu.RegisterGameObjectEvent( 202242, arcemu.GO_EVENT_ON_DESPAWN, go_onDespawn )
arcemu.RegisterGameObjectEvent( 202242, arcemu.GO_EVENT_ON_AIUPDATE, go_onAIUpdate )

def chest_onLootTaken( go, event, player, itemId ):
	goName = go.getName()
	goId = go.getId()
	player.sendChatMessage( arcemu.CHAT_MSG_SAY, arcemu.LANG_UNIVERSAL, "I have looted item " + str( itemId ) + " from " + goName + "(" + str( goId ) + ")"  )
	
def chest_onUse( go, event, player ):
	goName = go.getName()
	goId = go.getId()
	player.sendChatMessage( arcemu.CHAT_MSG_SAY, arcemu.LANG_UNIVERSAL, "I have activated " + goName + "(" + str( goId ) + ")"  )

arcemu.RegisterGameObjectEvent( 2843, arcemu.GO_EVENT_ON_LOOT_TAKEN, chest_onLootTaken )
arcemu.RegisterGameObjectEvent( 2843, arcemu.GO_EVENT_ON_USE, chest_onUse )


def fortressDoor_onDamaged( go, damage ):
	print( "GameObject " + go.getName() + "(" + str( go.getId() ) + ")" + " damaged for " + str( damage ) )
	
def fortressDoor_onDestroyed( go ):
	print( "GameObject " + go.getName() + "(" + str( go.getId() ) + ")" + " has been destroyed" )
	
arcemu.RegisterGameObjectEvent( 190375, arcemu.GO_EVENT_ON_DAMAGED, fortressDoor_onDamaged )
arcemu.RegisterGameObjectEvent( 190375, arcemu.GO_EVENT_ON_DESTROYED, fortressDoor_onDestroyed )
