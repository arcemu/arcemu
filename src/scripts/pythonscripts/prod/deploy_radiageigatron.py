import arcemu

def deploy_radiageigatron_handleDummyEffect( effectIndex, spell ):
	msg = "Processing spell '" + spell.getName() + "' effect #" + str( effectIndex )
	print( msg )

arcemu.RegisterDummySpellHandler( 73082, deploy_radiageigatron_handleDummyEffect )
