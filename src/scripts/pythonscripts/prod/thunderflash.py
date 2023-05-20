#
# ArcEmu MMORPG Server
# Copyright (C) 2008-2023 <http://www.ArcEmu.org/>
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
# 
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
# 
#
# Creature AI script for Thunderflash (39396)
# for the quest Vent Horizon (25212)
# 
# The player should be presented with a gossip menu when talking to Thunderflash
# When selecting the menu item: "Board the flying machine" the Player should be put a flight to the Gnomeregan vents and back
#
import arcemu
from arcemu import GossipMenu

THUNDERFLASH_ID = 39396
THUNDERFLASH_TEXT_ID = 15615
THUNDERFLASH_DISPLAY_ID = 22719
VENT_HORIZON_QUEST_ID = 25212
VENT_HORIZON_TAXI_SPELL = 73896
WORDS_FOR_DELIVERY2_QUEST_ID = 25287
WORDS_FOR_DELIVERY2_TAXI_SPELL = 74166

def thunderflash_onHello( unit, event, player ):
	menu = GossipMenu( THUNDERFLASH_TEXT_ID, unit, arcemu.GOSSIP_AUTOSEND_FALSE )
	
	if player.hasQuest( VENT_HORIZON_QUEST_ID ):
		menu.addItem( arcemu.ICON_CHAT, "Board the flying machine", 1, 0 )
	elif player.hasQuest( WORDS_FOR_DELIVERY2_QUEST_ID ):
		menu.addItem( arcemu.ICON_CHAT, "Take me to Mekkatorque!", 2, 0 )
	
	menu.sendToPlayer( player )
	
def thunderflash_onSelectOption( unit, player, id, enteredCode ):
	GossipMenu.complete( player )
	
	if id == 1:
		unit.castSpell( VENT_HORIZON_TAXI_SPELL, False, player )
	elif id == 2:
		unit.castSpell( WORDS_FOR_DELIVERY2_TAXI_SPELL, False, player )

arcemu.RegisterUnitGossipEvent( THUNDERFLASH_ID, arcemu.GOSSIP_EVENT_HELLO, thunderflash_onHello )
arcemu.RegisterUnitGossipEvent( THUNDERFLASH_ID, arcemu.GOSSIP_EVENT_SELECT, thunderflash_onSelectOption )
