''' This is just a test script for the Scourge Transporter. The point was testing GO gossip functionality'''

import arcemu
from arcemu import GossipMenu
from arcemu import Player

def go_onActivate( go, event, player ):
	menu = GossipMenu( 1, go, arcemu.GOSSIP_AUTOSEND_FALSE )
	menu.addItem( arcemu.ICON_CHAT, "Teleport to Goldshire #1", 1, 0 )
	menu.addItem( arcemu.ICON_VENDOR, "Teleport to Goldshire #2", 2, 0 )
	menu.addItem( arcemu.ICON_DOT, "Teleport to Goldshire #3", 3, 0 )
	menu.sendToPlayer( player )
	
def go_onSelectOption( go, player, id, enteredCode ):
	GossipMenu.complete( player )
	
	if id == 1:
		player.teleport( 0, -9459.524414, 76.275986, 56.855736 )
	elif id == 2:
		player.teleport( 0, -9490.390625, 67.313934, 56.004055 )
	elif id == 3:
		player.teleport( 0, -9458.459961, 47.227226, 56.605110 )

arcemu.RegisterGameObjectEvent( 202242, arcemu.GO_EVENT_ON_USE, go_onActivate )
arcemu.RegisterGOGossipEvent( 202242, arcemu.GOSSIP_EVENT_SELECT, go_onSelectOption )
