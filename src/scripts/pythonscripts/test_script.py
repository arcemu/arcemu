import arcemu
import unit

def onEmote( pPlayer, emote, pUnit ):
	print( "Player emote ", emote )
	print( "The unit's name is: ", unit.getName() )
	unit.sendChatMessage( 'This is a message' )

arcemu.info()
arcemu.RegisterServerHook( 8, onEmote )
