import arcemu
from arcemu import Unit
from arcemu import Player

def onEnterWorld( player ):
	print( "Player '" + player.getName() + "' has entered the world." )
	
def onPlayerDeath( player ):
	print( "Player '" + player.getName() + "' has died." )
	
def onPlayerRepop( player ):
	print( "Player '" + player.getName() + "' has been repopped." )
	
def onPlayerResurrect( player ):
	print( "Player '" + player.getName() + "' has resurrected." )

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
	
def onPreDie( killer, victim ):
	print( killer.getName() + " is killing " + victim.getName() )
	victim.sendChatMessage( 12, 0, killer.getName() + " is killing me softly...  ...with his spell." )

arcemu.info()
arcemu.RegisterServerHook( 4, onEnterWorld )
arcemu.RegisterServerHook( 6, onPlayerDeath )
arcemu.RegisterServerHook( 7, onPlayerRepop )
arcemu.RegisterServerHook( 32, onPlayerResurrect )
arcemu.RegisterServerHook( 8, onEmote )
arcemu.RegisterServerHook( 9, onEnterCombat )

arcemu.RegisterServerHook( 28, onPreDie )

