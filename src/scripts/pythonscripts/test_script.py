import arcemu
import unit
from unit import Unit

def onEmote( pPlayer, emote, pUnit ):
	print( "Player emote ", emote )
	
	print( pUnit )
	
	if pUnit is not None:
		print( "The unit's name is: ", pUnit.getName() )
		pUnit.sendChatMessage( 12, 0, 'This is a message' )
		

arcemu.info()
arcemu.RegisterServerHook( 8, onEmote )
