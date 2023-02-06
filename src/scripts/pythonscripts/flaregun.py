''' This is just a test script that uses the flare gun. The point was to test item gossip functionality. '''

import arcemu
from arcemu import GossipMenu
from arcemu import Item
from arcemu import Player

def item_onHello( item, event, player ):
	menu = GossipMenu( 1, item, arcemu.GOSSIP_AUTOSEND_FALSE )
	menu.addItem( arcemu.ICON_CHAT, "First menu item", 1, 0 )
	menu.addItem( arcemu.ICON_VENDOR, "Second menu item", 2, 0 )
	menu.addItem( arcemu.ICON_DOT, "Third menu item", 3, 0 )
	menu.sendToPlayer( player )
	
def item_onSelectOption( item, player, id, enteredCode ):
	player.sendChatMessage( arcemu.CHAT_MSG_SAY, arcemu.LANG_UNIVERSAL, "I have selected " + str( id )  )
	GossipMenu.complete( player )
	
	
arcemu.RegisterItemGossipEvent( 8051, arcemu.GOSSIP_EVENT_HELLO, item_onHello )
arcemu.RegisterItemGossipEvent( 8051, arcemu.GOSSIP_EVENT_SELECT, item_onSelectOption )


