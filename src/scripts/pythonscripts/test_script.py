import arcemu

def onEmote( player, emote, unit ):
	print( "Player emote ", emote )

arcemu.info()
arcemu.RegisterServerHook( 8, onEmote )
