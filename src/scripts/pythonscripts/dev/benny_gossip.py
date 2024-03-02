'''Test creature gossip script'''

import arcemu
from arcemu import GossipMenu
from arcemu import GossipScript

class BennyGossip( GossipScript ):
	def __init__( self ):
		GossipScript.__init__( self )
		
	def OnHello( self, object, player ):
		creature = arcemu.toCreature( object )
		print( creature.getName() + " gossip OnHello" )

		menu = GossipMenu( 1, object, arcemu.GOSSIP_AUTOSEND_FALSE )
		menu.addItem( arcemu.ICON_CHAT, "First menu item", 1, 0 )
		menu.addItem( arcemu.ICON_VENDOR, "Second menu item", 2, 0 )
		menu.addItem( arcemu.ICON_DOT, "Third menu item", 3, 0 )
		menu.sendToPlayer( player )
		
	def OnSelectOption( self, object, player, id, enteredCode ):
		GossipMenu.complete( player )
		
		creature = arcemu.toCreature( object )
		print( creature.getName() + " gossip OnSelectOption" )
		
		player.sendChatMessage( arcemu.CHAT_MSG_SAY, arcemu.LANG_UNIVERSAL, "I have selected " + str( id )  )
		

arcemu.RegisterCreatureGossipScript( 19, BennyGossip() )
