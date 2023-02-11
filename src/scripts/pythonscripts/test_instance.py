import arcemu

def instance_onLoad( instanceId ):
	print( "Loaded instance " + str( instanceId ) )
	
def instance_onDestroy( instanceId ):
	print( "Destroyed instance " + str( instanceId ) )
	
arcemu.RegisterInstanceEvent( 34, arcemu.INSTANCE_EVENT_ONLOAD, instance_onLoad )
arcemu.RegisterInstanceEvent( 34, arcemu.INSTANCE_EVENT_DESTROY, instance_onDestroy )
