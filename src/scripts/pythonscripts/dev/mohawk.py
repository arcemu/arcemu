import arcemu
import ArcPyMath as Math
from arcemu import GossipMenu
from arcemu import Player
from arcemu import Unit
from arcemu import ObjectLocator

def mohawk_onHello( unit, event, player ):
	print( player.getName() + " said gossip hello to " + unit.getName() )
	print( "Event id: " + str( event ) )
	
	creature = unit.toCreature()
	
	menu = GossipMenu( 1, unit, arcemu.GOSSIP_AUTOSEND_FALSE )
	
	if creature.hasNpcFlag( arcemu.NPC_FLAG_VENDOR ):
		menu.addItem( arcemu.ICON_VENDOR, "Show me your wares!", 34, 0 )
		
	if creature.hasNpcFlag( arcemu.NPC_FLAG_TRAINER ):
		menu.addItem( arcemu.ICON_CHAT, "I'd like to learn", 35, 0 )
	
	menu.addItem( arcemu.ICON_CHAT, "Vehicles", 0, 0 )
	
	menu.addItem( arcemu.ICON_CHAT, "AI updates", 600, 0 )
	
	menu.addItem( arcemu.ICON_CHAT, "AI stuff", 700, 0 )
	
	menu.addItem( arcemu.ICON_CHAT, "Visuals", 800, 0 )
	
	menu.addItem( arcemu.ICON_CHAT, "Show me where the Stormwind Auction House is", 22, 0 )
	
	menu.addItem( arcemu.ICON_CHAT, "PvP", 900, 0 )
	
	menu.addItem( arcemu.ICON_CHAT, "Spells", 1000, 0 )
	
	menu.addItem( arcemu.ICON_CHAT, "Media", 300, 0 )
	menu.addItem( arcemu.ICON_CHAT, "Items", 400, 0 )
	
	menu.addItem( arcemu.ICON_CHAT, "Tag", 200, 0 )
	
	menu.addItem( arcemu.ICON_CHAT, "NPC flags", 500, 0 )
	
	menu.addItem( arcemu.ICON_CHAT, "Misc", 1100, 0 )
	
	menu.addItem( arcemu.ICON_CHAT, "Misc2", 1200, 0 )
	
	menu.addItem( arcemu.ICON_CHAT, "Misc3", 1300, 0 )
	
	menu.addQuests( unit, player )
	menu.sendToPlayer( player )
	
def mohawk_onSelectOption( unit, player, id, enteredCode ):
	GossipMenu.complete( player )
	creature = unit.toCreature()
	
	if id == 0:
		menu = GossipMenu( 1, unit, arcemu.GOSSIP_AUTOSEND_FALSE )
		
		menu.addItem( arcemu.ICON_CHAT, "Give me a vehicle!", 1, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Give me a big vehicle!", 801, 0 )		
		menu.addItem( arcemu.ICON_CHAT, "Get to the choppa'!", 802, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Dismiss my vehicle!", 2, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Am I on a vehicle?", 3, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Add a passenger to my vehicle!", 4, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Shut up woman and get on my vehicle!", 5, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Get off my vehicle!", 6, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Speed up my vehicle!", 7, 0 )
		
		menu.sendToPlayer( player )
	
	if id == 1:
		if player.isOnVehicle():
			unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "You are already on a vehicle. Fool!" )
		else:
			unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Here it is, Fool!" )
			player.spawnAndEnterVehicle( 28605, 1000 )
			
	if id == 801:
		if player.isOnVehicle():
			unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "You are already on a vehicle. Fool!" )
		else:
			unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Here it is, Fool!" )
			player.castSpell( 61465, False )
			
	if id == 802:
		if player.isOnVehicle():
			unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "You are already on a vehicle. Fool!" )
		else:
			unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Here it is, Fool!" )
			player.castSpell( 60424 )
			
	elif id == 2:
		if player.isOnVehicle():
			unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Go away, Fool!" )
			player.dismissVehicle()
		else:
			unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "You don't have a vehicle, Fool!" )
			
	elif id == 3:
		if player.isOnVehicle():
			unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Ofc you are on a vehicle, Fool!" )
		else:
			unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Ofc you are not on a vehicle, Fool!" )
			
	elif id == 4:
		if player.isOnVehicle():
			if player.hasEmptyVehicleSeat():
				unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Here you go, Fool!" )
				player.addVehiclePassenger( 31111 )
			else:
				unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "You don't event have a free seat, Fool!" )
		else:
			unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "You don't even have a vehicle, Fool!" )
	
	elif id == 5:
		if player.isOnVehicle():
			if player.hasEmptyVehicleSeat():
				if unit.isOnVehicle():
					unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "I am already on a vehicle, Fool!" )
				else:
					vb = player.getVehicleBase()
					unit.enterVehicle( vb.getGUID(), 0 )
			else:
				unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "You don't have an empty seat, Fool!" )
		else:
			unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "You are not even on a vehicle, Fool!" )
			
	elif id == 6:
		if unit.isOnVehicle():
			unit.exitVehicle()
		else:
			unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "I am NOT on a vehicle, Fool!" )
			
	elif id == 7:
		if player.isOnVehicle():
			vb = player.getVehicleBase()
			vb.setSpeeds( 100.0 )
		else:
			unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "You are not on a vehicle, Fool!" )
			
	elif id == 8:
		unit.RegisterAIUpdateEvent( 2500 )
		
	elif id == 9:
		unit.ModifyAIUpdateEvent( 1000 )
		
	elif id == 10:
		unit.RemoveAIUpdateEvent()
		
	elif id == 12:
		unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "You really are a fool!" )
		unit.RegisterAIUpdateEvent( 1000 )
		unit.setFaction( 168 )
		
	elif id == 13:
		unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Let's move then, fool!" )
		unit.setUnitToFollow( player, 2.0 )
		
	elif id == 14:
		unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Alright then, fool!" )
		unit.stopFollowing()
		
	elif id == 15:
		unit.setScale( 5.0 )
		
	elif id == 16:
		unit.setScale( 0.25 )
		
	elif id == 17:
		unit.setScale( 1.0 )
		
	elif id == 18:
		unit.setMount( 6080 )
		
	elif id == 19:
		unit.setMount( 0 )
		
	elif id == 20:
		unit.equipWeapons( 32837, 32837 )
		unit.setSheatState( arcemu.SHEATSTATE_UNSHEATHED )
		
	elif id == 21:
		unit.setSheatState( arcemu.SHEATSTATE_SHEATHED )
		unit.equipWeapons( 0, 0, 0 )
		
	elif id == 22:
		unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Alright then, fool!" )
		player.sendGossipPOI( -8811.46, 667.46, 7, 6, 0, "Stormwind Auction House" )
		
	elif id == 23:
		if unit.isPvPFlagged():
			unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "I am!" )
		else:
			unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "I am NOT!" )
			
	elif id == 24:
		unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Bring it on, fool!" )
		unit.setPvPFlag()
		
	elif id == 25:
		unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Okay, I back down..." )
		unit.removePvPFlag()
		
	elif id == 26:
		aura = unit.getAuraBySpellId( 1243 )
		if aura is not None:
			print( "Found Aura: " + aura.getSpellName() )
			print( "Caster GUID: " + str( aura.getCasterGuid() ) )
		
		if unit.hasAura( 1243 ):
			unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "I am" )
		else:
			unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "I am not" )
			
	elif id == 27:
		aura = unit.getAuraBySpellId( 1243 )
		if aura is not None:
			aura.remove();
			unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Done" )
		else:
			unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "I don't have such an aura." )
			
	elif id == 28:
		state = unit.getStandState()
		print( "Current standstate: " + str( state ) )
		
		menu = GossipMenu( 1, unit, arcemu.GOSSIP_AUTOSEND_FALSE )
		
		menu.addItem( arcemu.ICON_CHAT, "Stand", 100, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Sit", 101, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Sleep", 102, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Dead", 103, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Kneel", 104, 0 )
		
		menu.sendToPlayer( player )
		
	elif id == 29:
		creature.despawn( 1500, 1500 )
		
	elif id == 30:
		if player.isPlayer():
			p = player.toPlayer()
			unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "You are, fool" )
			unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "...and your name is " + p.getName() )
			
	elif id == 31:
		unit.castSpell( 2052, False, player )
		
	if id == 32:
		x = unit.getPositionX()
		y = unit.getPositionY()
		z = unit.getPositionZ()
		
		coords = "(" + str( x ) + "," + str( y ) + "," + str( z ) + ")"
		
		unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "I am at " + coords )
		
	if id == 33:
		x = player.getPositionX()
		y = player.getPositionY()
		z = player.getPositionZ()
		
		coords = "(" + str( x ) + "," + str( y ) + "," + str( z ) + ")"
		
		unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "You are at " + coords )
		
	if id == 34:
		session = player.getSession()
		session.sendInventoryList( creature )
		
	if id == 35:
		session = player.getSession()
		session.sendTrainerList( creature )
		
	elif id == 100:
		unit.setStandState( arcemu.STANDSTATE_STAND )
		
	elif id == 101:
		unit.setStandState( arcemu.STANDSTATE_SIT )
		
	elif id == 102:
		unit.setStandState( arcemu.STANDSTATE_SLEEP )
		
	elif id == 103:
		unit.setStandState( arcemu.STANDSTATE_DEAD )
		
	elif id == 104:
		unit.setStandState( arcemu.STANDSTATE_KNEEL )
		
	elif id == 200:
		menu = GossipMenu( 1, unit, arcemu.GOSSIP_AUTOSEND_FALSE )
		
		menu.addItem( arcemu.ICON_CHAT, "Are you tagged?", 201, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Did I tag you?", 202, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Tag!", 203, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Untag!", 204, 0 )
		
		menu.sendToPlayer( player )
		
	elif id == 201:
		if unit.isTagged():
			unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Yes, I am tagged" )
		else:
			unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "No, I am not tagged" )
			
	elif id == 202:
		tagger = unit.getTaggerGuid()
		if tagger == player.getGUID():
			unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Yes, you tagged me" )
		else:
			unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "No, you did not tag me" )
			
	elif id == 203:
		unit.tag( player.getGUID() )
		unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Done" )
		
	elif id == 204:
		unit.untag()
		unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Done" )
		
	if id == 300:
		menu = GossipMenu( 1, unit, arcemu.GOSSIP_AUTOSEND_FALSE )
		
		menu.addItem( arcemu.ICON_CHAT, "Let's play the Power of The Horde!", 301, 0 )		
		menu.addItem( arcemu.ICON_CHAT, "Show me the World of Warcraft intro movie!", 302, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Show me the Wrath Gate movie!", 303, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Show me the Fall of the Lich King movie!", 304, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Show me the Human cinematic sequence!", 305, 0 )
		
		menu.sendToPlayer( player )
		
	elif id == 301:
		unit.playSoundToSet( 11803 )
		
	if id == 302:
		player.sendMovie( 2 )
		
	if id == 303:
		player.sendMovie( 14 )
		
	if id == 304:
		player.sendMovie( 16 )
		
	if id == 305:
		player.sendCinematic( 81 )
		
	if id == 400:
		menu = GossipMenu( 1, unit, arcemu.GOSSIP_AUTOSEND_FALSE )
		menu.addItem( arcemu.ICON_CHAT, "Give me a Darnassian Bleu!", 401, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Take a Darnassian Blue", 402, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Do I have a Darnassian Bleu?", 403, 0 )
		menu.sendToPlayer( player )
		
	if id == 401:
		player.addItem( 2070, 1 )
		
	if id == 402:
		player.removeItem( 2070, 1 )
		
	if id == 403:
		if player.hasItem( 2070, 1 ):
			unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Yes" )
		else:
			unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "No" )
			
	if id == 500:
		menu = GossipMenu( 1, unit, arcemu.GOSSIP_AUTOSEND_FALSE )
		menu.addItem( arcemu.ICON_CHAT, "Do you have the questgiver flag?", 501, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Set the questgiver flag", 502, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Remove the questgiver flag", 503, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Do you have the vendor flag?", 504, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Set the vendor flag", 505, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Remove the vendor flag", 506, 0 )		
		menu.addItem( arcemu.ICON_CHAT, "Do you have the trainer flag?", 507, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Set the trainer flag", 508, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Remove the trainer flag", 509, 0 )
		
		
		menu.sendToPlayer( player )
		
	if id == 501:
		if creature.hasNpcFlag( arcemu.NPC_FLAG_QUESTGIVER ):
			creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "I do" )
		else:
			creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "I don't" )
			
	if id == 502:
		creature.addNpcFlag( arcemu.NPC_FLAG_QUESTGIVER )
		
	if id == 503:
		creature.removeNpcFlag( arcemu.NPC_FLAG_QUESTGIVER )
		
	if id == 504:
		if creature.hasNpcFlag( arcemu.NPC_FLAG_VENDOR ):
			creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "I do" )
		else:
			creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "I don't" )
			
	if id == 505:
		creature.addNpcFlag( arcemu.NPC_FLAG_VENDOR )
		
	if id == 506:
		creature.removeNpcFlag( arcemu.NPC_FLAG_VENDOR )
		
	if id == 507:
		if creature.hasNpcFlag( arcemu.NPC_FLAG_TRAINER ):
			creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "I do" )
		else:
			creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "I don't" )
			
	if id == 508:
		creature.addNpcFlag( arcemu.NPC_FLAG_TRAINER )
		
	if id == 509:
		creature.removeNpcFlag( arcemu.NPC_FLAG_TRAINER )

		
	if id == 600:
		menu = GossipMenu( 1, unit, arcemu.GOSSIP_AUTOSEND_FALSE )
		
		menu.addItem( arcemu.ICON_CHAT, "Pity the fool!", 8, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Pity the fool faster!", 9, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Don't pity the fool!", 10, 0 )
		
		menu.sendToPlayer( player )
		
	if id == 700:
		menu = GossipMenu( 1, unit, arcemu.GOSSIP_AUTOSEND_FALSE )
		
		menu.addItem( arcemu.ICON_CHAT, "I think your hair is ugly", 12, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Follow me!", 13, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Stop following me!", 14, 0 )
		
		menu.sendToPlayer( player )
		
	if id == 800:
		menu = GossipMenu( 1, unit, arcemu.GOSSIP_AUTOSEND_FALSE )
		
		menu.addItem( arcemu.ICON_CHAT, "Get large!", 15, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Get small!", 16, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Get back to normal size!", 17, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Mount up!", 18, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Dismount!", 19, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Arm yourself!", 20, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Disarm yourself!", 21, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Standstate", 28, 0 )
		
		menu.sendToPlayer( player )
		
	if id == 900:
		menu = GossipMenu( 1, unit, arcemu.GOSSIP_AUTOSEND_FALSE )
		
		menu.addItem( arcemu.ICON_CHAT, "Are you PvP flagged?", 23, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Flag yourself for PvP!", 24, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Remove your PvP flag!", 25, 0 )
		
		menu.sendToPlayer( player )
		
	if id == 1000:
		menu = GossipMenu( 1, unit, arcemu.GOSSIP_AUTOSEND_FALSE )
		
		menu.addItem( arcemu.ICON_CHAT, "Are you buffed with Power Word: Fortitude I?", 26, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Remove Power Word: Fortitude I", 27, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Heal me!", 31, 0 )
		
		menu.sendToPlayer( player )
		
	if id == 1100:
		menu = GossipMenu( 1, unit, arcemu.GOSSIP_AUTOSEND_FALSE )
		
		menu.addItem( arcemu.ICON_CHAT, "Respawn!", 29, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Am I a player?", 30, 0 )
		menu.addItem( arcemu.ICON_CHAT, "What are your coordinates?", 32, 0 )
		menu.addItem( arcemu.ICON_CHAT, "What are my coordinates?", 33, 0 )
		
		menu.addItem( arcemu.ICON_CHAT, "Remove lesser healing potion from your vendor inventory", 1101, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Remove all Items from your vendor inventory", 1102, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Tell me a random integer", 1103, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Tell me a random integer up until 123", 1104, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Tell me a random float", 1105, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Tell me a random integer up until 12.34", 1106, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Dump objects in range", 1107, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Dump units in range", 1108, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Make units in range cast", 1109, 0 )
		menu.addItem( arcemu.ICON_CHAT, "How far am I from you?", 1110, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Knock me back", 1111, 0 )
		
		menu.sendToPlayer( player )
		
	if id == 1101:
		creature.removeVendorItem( 858 )
		
	if id == 1102:
		creature.removeVendorItems()
		
	if id == 1103:
		creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, str( Math.randomUInt() ) )
		
	if id == 1104:
		creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, str( Math.randomUInt( 123 ) ) )
		
	if id == 1105:
		creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, str( Math.randomFloat() ) )
		
	if id == 1106:
		creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, str( Math.randomFloat( 12.34 ) ) )
		
	if id == 1107:
		objects = creature.getObjectsInRange()
		for o in objects:
			print( str( o ) )

	if id == 1108:
		objects = creature.getObjectsInRange()
		for o in objects:
			u = arcemu.toUnit( o )
			if u is not None:
				print( u.getName() )
	
	if id == 1109:
		objects = creature.getObjectsInRange()
		for o in objects:
			u = arcemu.toUnit( o )
			if u is not None:
				u.castSpell( 74153, False )
				
	if id == 1110:
		distance = creature.calcDistance( player )
		creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, str( distance ) )
		
	if id == 1111:
		player.knockBack( 3.0, 20.0 )
		
	if id == 1200:
		menu = GossipMenu( 1, unit, arcemu.GOSSIP_AUTOSEND_FALSE )
		
		menu.addItem( arcemu.ICON_CHAT, "Send me an areatrigger message!", 1201, 0 )		
		menu.addItem( arcemu.ICON_CHAT, "Send me a broadcast message!", 1202, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Move to my selection!", 1203, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Set the next target to my selection", 1204, 0 )
		menu.addItem( arcemu.ICON_CHAT, "What is the map Id of this map?", 1205, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Spawn a creature", 1206, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Spawn a GameObject", 1207, 0 )		
		menu.addItem( arcemu.ICON_CHAT, "Set the target guid (visual) to my selection", 1208, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Channel a spell to my selection", 1209, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Set our phases to 256", 1210, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Set our phases to 1", 1211, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Add phase 256 to our phases", 1212, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Remove phase 256 from our phases", 1213, 0 )
		menu.addItem( arcemu.ICON_CHAT, "What are your current phases?", 1214, 0 )
		menu.addItem( arcemu.ICON_CHAT, "What zone am in?", 1215, 0 )
		menu.addItem( arcemu.ICON_CHAT, "What map am I on?", 1216, 0 )
		menu.addItem( arcemu.ICON_CHAT, "What area am in?", 1217, 0 )
		
		menu.sendToPlayer( player )
		
	if id == 1201:
		player.sendAreaTriggerMessage( "I pity the fool!" )
		
	if id == 1202:
		player.sendAreaTriggerMessage( "Fool!" )
		
	if id == 1203:
		selectedUnit = player.getSelectedUnit()
		if selectedUnit is not None:
			creature.faceUnit( selectedUnit )
			o = creature.getOrientation()
			creature.moveTo( selectedUnit.getPositionX(), selectedUnit.getPositionY(), selectedUnit.getPositionZ(), o )
			
	if id == 1204:
		selectedUnit = player.getSelectedUnit()
		creature.setNextTarget( selectedUnit )
		
	if id == 1205:
		mapMgr = creature.getMapMgr()
		creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "The map Id of this map is " + str( mapMgr.getMapId() ) )
		
	if id == 1206:
		mapMgr = creature.getMapMgr();
		spawn = mapMgr.spawnCreature( 31111, player.getPositionX(), player.getPositionY(), player.getPositionZ() )
		
		if spawn is not None:
			spawn.setFaction( player.getFaction() )
			spawn.faceUnit( unit )
			spawn.equipWeapons( 32837, 32837 )
			spawn.setSheatState( arcemu.SHEATSTATE_UNSHEATHED )
			spawn.setScale( 1.5 )
			
	if id == 1207:
		mapMgr = creature.getMapMgr()
		go = mapMgr.spawnGameObject( 142075, player.getPositionX(), player.getPositionY(), player.getPositionZ() )
		
		if go is not None:
			go.setScale( 0.5 )
			go.despawn( 5000, 0 )
			
	if id == 1208:
		selectedUnit = player.getSelectedUnit()
		if selectedUnit is not None:
			creature.setTargetGUID( selectedUnit.getGUID() )
			print( str( creature.getTargetGUID() ) )
			
	if id == 1209:
		selectedUnit = player.getSelectedUnit()
		if selectedUnit is not None:
			creature.faceUnit( selectedUnit )
			creature.setTargetGUID( selectedUnit.getGUID() )
			creature.setChannelSpellTargetGUID( selectedUnit.getGUID() )
			creature.setChannelSpellId( 39857 )
			
	if id == 1210:
		player.setPhase( 256 )
		creature.setPhase( 256 )
		
	if id == 1211:
		player.setPhase( 1 )
		creature.setPhase( 1 )
		
	if id == 1212:
		player.addPhase( 256 )
		creature.addPhase( 256 )
		
	if id == 1213:
		player.removePhase( 256 )
		creature.removePhase( 256 )
		
	if id == 1214:
		creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, str( creature.getPhase() ) )
		
	if id == 1215:
		creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, str( player.getZoneId() ) )
		
	if id == 1216:
		creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, str( player.getMapId() ) )
		
	if id == 1217:
		creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, str( player.getAreaId() ) )
			
	if id == 1300:
		menu = GossipMenu( 1, unit, arcemu.GOSSIP_AUTOSEND_FALSE )
		
		menu.addItem( arcemu.ICON_CHAT, "What is your scale?", 1301, 0 )		
		menu.addItem( arcemu.ICON_CHAT, "What is your entry?", 1302, 0 )
		menu.addItem( arcemu.ICON_CHAT, "What is your GUID?", 1303, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Creep!", 1304, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Increase the number of Alliance controlled forts in Hellfire Penninsula", 1305, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Make Marshal Dugan larger", 1306, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Make the mailbox smaller", 1307, 0 )
		menu.addItem( arcemu.ICON_CHAT, "What is the name of my selection?", 1308, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Closest friendly", 1309, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Closest enemy", 1310, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Closest player", 1311, 0 )
		menu.addItem( arcemu.ICON_CHAT, "Random friendly", 1312, 0 )
		
		menu.sendToPlayer( player )
		
	if id == 1301:
		scale = creature.getFloatValue( arcemu.OBJECT_FIELD_SCALE_X );
		creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "My scale is " + str( scale ) )
		
	if id == 1302:
		entry = creature.getUInt32Value( arcemu.OBJECT_FIELD_ENTRY );
		creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "My entry is " + str( entry ) )
		
	if id == 1303:
		guid = creature.getUInt64Value( arcemu.OBJECT_FIELD_GUID );
		creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "My GUID is " + str( guid ) )
		
	if id == 1304:
		creature.setByteFlags( arcemu.UNIT_FIELD_BYTES_1, arcemu.STANDFLAGS_BYTE, 2 )
		
	if id == 1305:
		# Note: this works in Hellfire Penninsula only!
		mapMgr = creature.getMapMgr()
		value = mapMgr.getWorldState( 3483, 2476 )
		mapMgr.updateWorldState( 3483, 2476, value + 1 )
		
	if id == 1306:
		# Note: This will work in Goldshire only
		mapMgr = creature.getMapMgr()
		marshal = mapMgr.getCreatureNearestCoords( -9465.51, 74.01, 56.77, 240 )
		if marshal is not None:
			marshal.setScale( 3.0 )
			
	if id == 1307:
		# Note: This will work in Goldshire only
		mapMgr = creature.getMapMgr()
		mailbox = mapMgr.getGameObjectNearestCoords( -9455.85, 46.68, 56.60, 142075 )
		if mailbox is not None:
			mailbox.setScale( 0.5 )
			# For some reason GOs need to be respawned for the new scale to be visible in the client
			mailbox.despawn( 1, 1 )
			
	if id == 1308:
		guid = player.getSelection()
		if guid == 0:
			creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "You have not selected anything" )
		else:
			mapMgr = player.getMapMgr()
			obj = mapMgr.getObject( guid )
			if obj is not None:
				u = arcemu.toUnit( obj )
				if u is not None:
					creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, u.getName() )
					
	if id == 1309:
		locator = ObjectLocator( creature )
		friendly = locator.findClosestFriendly()
		if friendly is not None:
			creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "The closest friendly Unit is " + friendly.getName() + " " + str( friendly.getGUID() ) )
		else:
			creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "I don't see a friendly" )
		
	if id == 1310:
		locator = ObjectLocator( creature )
		enemy = locator.findClosestEnemy()
		if enemy is not None:
			creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "The closest enemy Unit is " + enemy.getName() + " " + str( enemy.getGUID() ) )
		else:
			creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "I don't see an enemy" )
			
	if id == 1311:
		locator = ObjectLocator( creature )
		closestPlayer = locator.findClosestPlayer()
		if closestPlayer is not None:
			creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "The closest player is " + closestPlayer.getName() + " " + str( closestPlayer.getGUID() ) )
		else:
			creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "I don't see an players" )
			
	if id == 1312:
		locator = ObjectLocator( creature )
		friendly = locator.getRandomFriendly()
		if friendly is not None:
			creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "The random friendly Unit is " + friendly.getName() + " " + str( friendly.getGUID() ) )
		else:
			creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "I don't see a friendly" )

def mohawk_onEnterVehicle( unit ):
	unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Nice I am on a vehicle now!" )
	
def mohawk_onExitVehicle( unit ):
	unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Aw I got booted :(" )
	
def mohawk_onAIUpdate( unit, event ):
	unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "I pity the fool!" )
	
	creature = unit.toCreature()
	tank = creature.getMostHated()
	if tank is not None:
		print( "The most hated is " + tank.getName() + " " + str( tank.getGUID() ) )
		
	offTank = creature.getSecondMostHated()
	if offTank is not None:
		print( "The second most hated is " + offTank.getName() + " " + str( offTank.getGUID() ) )
	
def mohawk_onHealed( unit, healer, spellId, amount ):
	aura = unit.getAuraBySpellId( 1243 )
	if aura is not None:
		unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Booyah!" )
	else:
		unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Thanks for healing me with " + str( spellId ) + " for " + str( amount ) )
	
def mohawk_onApplyAura( unit, caster, spellId ):
    unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Thanks " + caster.getName() + " for buffing me with " + str( spellId ) )
	
def mohawk_onLoad( unit, event ):
	print( "Mohawk maxhealth: " + str( unit.getMaxHealth() ) )
	print( "Mohawk health: " + str( unit.getHealth() ) )
	
	unit.setMaxHealth( 300000 )
	unit.setHealth( unit.getMaxHealth() )
	
	creature = unit.toCreature()	
	creature.addVendorItem( 858 )
	creature.addVendorItem( 3371, 5 )
	creature.addVendorItem( 3372, 10 )

arcemu.RegisterUnitGossipEvent( 31111, arcemu.GOSSIP_EVENT_HELLO, mohawk_onHello )
arcemu.RegisterUnitGossipEvent( 31111, arcemu.GOSSIP_EVENT_SELECT, mohawk_onSelectOption )
arcemu.RegisterUnitEvent( 31111, arcemu.CREATURE_EVENT_ON_ENTER_VEHICLE, mohawk_onEnterVehicle )
arcemu.RegisterUnitEvent( 31111, arcemu.CREATURE_EVENT_ON_EXIT_VEHICLE, mohawk_onExitVehicle )
arcemu.RegisterUnitEvent( 31111, arcemu.CREATURE_EVENT_ON_AIUPDATE, mohawk_onAIUpdate )
arcemu.RegisterUnitEvent( 31111, arcemu.CREATURE_EVENT_ON_HEALED, mohawk_onHealed )
arcemu.RegisterUnitEvent( 31111, arcemu.CREATURE_EVENT_ON_APPLY_AURA, mohawk_onApplyAura )
arcemu.RegisterUnitEvent( 31111, arcemu.CREATURE_EVENT_ON_LOAD, mohawk_onLoad )
