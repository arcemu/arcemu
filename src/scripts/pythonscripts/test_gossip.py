import arcemu
from arcemu import GameObject
from arcemu import GossipMenu
from arcemu import Item
from arcemu import Player
from arcemu import Unit

def mohawk_onHello( unit, event, player ):
	print( player.getName() + " said gossip hello to " + unit.getName() )
	print( "Event id: " + str( event ) )
	
	menu = GossipMenu( 1, unit, 0 )
	menu.addItem( arcemu.ICON_CHAT, "First menu item", 1, 0 )
	menu.addItem( arcemu.ICON_VENDOR, "Second menu item", 2, 0 )
	menu.addItem( arcemu.ICON_DOT, "Third menu item", 3, 0 )
	menu.sendToPlayer( player )
	
def mohawk_onSelectOption( unit, player, id, enteredCode ):
	print( player.getName() + " sent gossip onselectoption to " + unit.getName() )
	print( "Selected id: " + str( id ) )
	GossipMenu.complete( player )
	
	unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, player.getName() + " you've selected option " + str( id ) )
	
	
def item_onHello( item, event, player ):
	menu = GossipMenu( 1, item, 0 )
	menu.addItem( arcemu.ICON_CHAT, "First menu item", 1, 0 )
	menu.addItem( arcemu.ICON_VENDOR, "Second menu item", 2, 0 )
	menu.addItem( arcemu.ICON_DOT, "Third menu item", 3, 0 )
	menu.sendToPlayer( player )
	
def item_onSelectOption( item, player, id, enteredCode ):
	player.sendChatMessage( arcemu.CHAT_MSG_SAY, arcemu.LANG_UNIVERSAL, "I have selected " + str( id )  )
	GossipMenu.complete( player )
	
	
def go_onActivate( go, event, player ):
	menu = GossipMenu( 1, go, 0 )
	menu.addItem( arcemu.ICON_CHAT, "First GO menu item", 1, 0 )
	menu.addItem( arcemu.ICON_VENDOR, "Second GO menu item", 2, 0 )
	menu.addItem( arcemu.ICON_DOT, "Third GO menu item", 3, 0 )
	menu.sendToPlayer( player )
	
def go_onSelectOption( go, player, id, enteredCode ):
	player.sendChatMessage( arcemu.CHAT_MSG_SAY, arcemu.LANG_UNIVERSAL, "I have selected GO menu item " + str( id )  )
	GossipMenu.complete( player )	
	
arcemu.RegisterUnitGossipEvent( 31111, 1, mohawk_onHello )
arcemu.RegisterUnitGossipEvent( 31111, 2, mohawk_onSelectOption )

arcemu.RegisterItemGossipEvent( 8051, 1, item_onHello )
arcemu.RegisterItemGossipEvent( 8051, 2, item_onSelectOption )

arcemu.RegisterGameObjectEvent( 202242, arcemu.GO_EVENT_ON_USE, go_onActivate )
arcemu.RegisterGOGossipEvent( 202242, 2, go_onSelectOption )

