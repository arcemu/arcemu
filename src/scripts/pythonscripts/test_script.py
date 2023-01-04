import arcemu
from arcemu import Unit
from arcemu import Player

def onEnterWorld( player ):
	print( "Player '" + player.getName() + "' has entered the world." )

def onEmote( pPlayer, emote, pUnit ):
	print( "Player emote ", emote )
	
	if pPlayer is not None:
		print( "The player's name is: ", pPlayer.getName() )
		pPlayer.sendChatMessage( 12, 0, "Hello my name is " + pPlayer.getName() )
	
	if pUnit is not None:
		print( "The unit's name is: ", pUnit.getName() )
		pUnit.sendChatMessage( 12, 0, 'Hello, my name is ' + pUnit.getName() )
		
		
def onEnterCombat( pPlayer, pUnit ):
	print( "Player " + pPlayer.getName() + " entered combat with Unit " + pUnit.getName() )		

arcemu.info()
arcemu.RegisterServerHook( 4, onEnterWorld )
arcemu.RegisterServerHook( 8, onEmote )
arcemu.RegisterServerHook( 9, onEnterCombat )
