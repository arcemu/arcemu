''' This is just a test script that uses the flare gun. The point was to test item gossip functionality. '''

import arcemu
from arcemu import GossipMenu
from arcemu import GossipScript

class FlareGunGossip( GossipScript ):
	def __init__( self ):
		GossipScript.__init__( self )
		
	def OnHello( self, object, player ):
		item = arcemu.toItem( object )

		menu = GossipMenu( 1, object, arcemu.GOSSIP_AUTOSEND_FALSE )
		menu.addItem( arcemu.ICON_CHAT, "First menu item", 1, 0 )
		menu.addItem( arcemu.ICON_VENDOR, "Second menu item", 2, 0 )
		menu.addItem( arcemu.ICON_DOT, "Third menu item", 3, 0 )
		menu.sendToPlayer( player )
		
	def OnSelectOption( self, object, player, id, enteredCode ):
		player.sendChatMessage( arcemu.CHAT_MSG_SAY, arcemu.LANG_UNIVERSAL, "I have selected " + str( id )  )
		GossipMenu.complete( player )

arcemu.RegisterItemGossipScript( 10444, FlareGunGossip() )
