import arcemu
from arcemu import GameObject
from arcemu import Player
from arcemu import Unit

def instance_onPlayerDeath( instanceId, victim, killer ):
	print( "Instance: " + killer.getName() + " has slain " + victim.getName() )

def instance_onPlayerEnter( instanceId, player ):
	print( "Instance: Player " + player.getName() + " has entered " + str( instanceId ) )

def instance_onAreaTrigger( instanceId, player, areaId ):
	print( "Instance: Player " + player.getName() + " has triggered areatrigger " + str( areaId ) )

def instance_onZoneChange( instanceId, player, newZone, oldZone ):
	print( "Instance: Player " + player.getName() + " has changed zone from " + str( oldZone ) + " to " + str( newZone ) )

def instance_onCreatureDeath( instanceId, victim, killer ):
	print( "Instance: Creature " + victim.getName() + " killed by " + killer.getName() )

def instance_onCreaturePush( instanceId, creature ):
	print( "Instance: Creature pushed to world: " + creature.getName()  )

def instance_onGOActivated( instanceId, go, player ):
	print( "Instance: GO " + go.getName() + "(" + str( go.getId() ) + ") activated by " + player.getName() )

def instance_onGameObjectPush( instanceId, go ):
	print( "Instance: GO pushed to world: " + go.getName() + "(" + str( go.getId() ) + ")" )

def instance_onLoad( instanceId ):
	print( "Instance: Loaded " + str( instanceId ) )
	
def instance_onDestroy( instanceId ):
	print( "Instance: Destroyed " + str( instanceId ) )
	

arcemu.RegisterInstanceEvent( 34, arcemu.INSTANCE_EVENT_ON_PLAYER_DEATH, instance_onPlayerDeath )
arcemu.RegisterInstanceEvent( 34, arcemu.INSTANCE_EVENT_ON_PLAYER_ENTER, instance_onPlayerEnter )
arcemu.RegisterInstanceEvent( 34, arcemu.INSTANCE_EVENT_ON_AREA_TRIGGER, instance_onAreaTrigger )
arcemu.RegisterInstanceEvent( 34, arcemu.INSTANCE_EVENT_ON_ZONE_CHANGE, instance_onZoneChange )
arcemu.RegisterInstanceEvent( 34, arcemu.INSTANCE_EVENT_ON_CREATURE_DEATH, instance_onCreatureDeath )
arcemu.RegisterInstanceEvent( 34, arcemu.INSTANCE_EVENT_ON_CREATURE_PUSH, instance_onCreaturePush )	
arcemu.RegisterInstanceEvent( 34, arcemu.INSTANCE_EVENT_ON_GO_ACTIVATE, instance_onGOActivated )
arcemu.RegisterInstanceEvent( 34, arcemu.INSTANCE_EVENT_ON_GO_PUSH, instance_onGameObjectPush )
arcemu.RegisterInstanceEvent( 34, arcemu.INSTANCE_EVENT_ONLOAD, instance_onLoad )
arcemu.RegisterInstanceEvent( 34, arcemu.INSTANCE_EVENT_DESTROY, instance_onDestroy )
