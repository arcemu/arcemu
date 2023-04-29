import arcemu

def deploy_radiageigatron_handleDummyEffect( effectIndex, spell ):
	msg = "Processing spell '" + spell.getName() + "' effect #" + str( effectIndex )
	print( msg )
	
	playerCaster = spell.getPlayerCaster()
	if playerCaster is not None:
		print( "Player " + playerCaster.getName() + " has cast this spell" )
	else:
		print( "The Player was not cast by a Player" )
		
	destX = spell.getTargetDestinationX()
	destY = spell.getTargetDestinationY()
	destZ = spell.getTargetDestinationZ()
	
	destination = "( " + str( destX ) + ", " + str( destY ) + ", " + str( destZ ) + " ) "
	print( "Target destination: " + destination )

arcemu.RegisterDummySpellHandler( 73082, deploy_radiageigatron_handleDummyEffect )
