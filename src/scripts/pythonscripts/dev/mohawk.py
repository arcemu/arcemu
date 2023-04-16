import arcemu
from arcemu import GossipMenu
from arcemu import Player
from arcemu import Unit

def mohawk_onHello( unit, event, player ):
	print( player.getName() + " said gossip hello to " + unit.getName() )
	print( "Event id: " + str( event ) )
	
	menu = GossipMenu( 1, unit, arcemu.GOSSIP_AUTOSEND_FALSE )
	menu.addItem( arcemu.ICON_CHAT, "Vehicles", 0, 0 )
	
	menu.addItem( arcemu.ICON_CHAT, "Pity the fool!", 8, 0 )
	menu.addItem( arcemu.ICON_CHAT, "Pity the fool faster!", 9, 0 )
	menu.addItem( arcemu.ICON_CHAT, "Don't pity the fool!", 10, 0 )
	
	menu.addItem( arcemu.ICON_CHAT, "I think your hair is ugly", 12, 0 )
	menu.addItem( arcemu.ICON_CHAT, "Follow me!", 13, 0 )
	menu.addItem( arcemu.ICON_CHAT, "Stop following me!", 14, 0 )
	
	menu.addItem( arcemu.ICON_CHAT, "Get large!", 15, 0 )
	menu.addItem( arcemu.ICON_CHAT, "Get small!", 16, 0 )
	menu.addItem( arcemu.ICON_CHAT, "Get back to normal size!", 17, 0 )
	
	menu.addItem( arcemu.ICON_CHAT, "Mount up!", 18, 0 )
	menu.addItem( arcemu.ICON_CHAT, "Dismount!", 19, 0 )
	
	menu.addItem( arcemu.ICON_CHAT, "Arm yourself!", 20, 0 )
	menu.addItem( arcemu.ICON_CHAT, "Disarm yourself!", 21, 0 )
	
	menu.addItem( arcemu.ICON_CHAT, "Show me where the Stormwind Auction House is", 22, 0 )
	
	menu.addItem( arcemu.ICON_CHAT, "Are you PvP flagged?", 23, 0 )
	menu.addItem( arcemu.ICON_CHAT, "Flag yourself for PvP!", 24, 0 )
	menu.addItem( arcemu.ICON_CHAT, "Remove your PvP flag!", 25, 0 )
	
	menu.addItem( arcemu.ICON_CHAT, "Are you buffed with Power Word: Fortitude I?", 26, 0 )
	menu.addItem( arcemu.ICON_CHAT, "Remove Power Word: Fortitude I", 27, 0 )
	
	menu.addItem( arcemu.ICON_CHAT, "Standstate", 28, 0 )
	menu.addItem( arcemu.ICON_CHAT, "Tag", 200, 0 )
	
	menu.addItem( arcemu.ICON_CHAT, "Respawn!", 29, 0 )
	menu.addItem( arcemu.ICON_CHAT, "Am I a player?", 30, 0 )
	menu.addItem( arcemu.ICON_CHAT, "Heal me!", 31, 0 )
	
	menu.addItem( arcemu.ICON_CHAT, "Media", 300, 0 )
	menu.addItem( arcemu.ICON_CHAT, "Items", 400, 0 )
	
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
		

def mohawk_onEnterVehicle( unit ):
	unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Nice I am on a vehicle now!" )
	
def mohawk_onExitVehicle( unit ):
	unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Aw I got booted :(" )
	
def mohawk_onAIUpdate( unit, event ):
	unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "I pity the fool!" )
	
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

arcemu.RegisterUnitGossipEvent( 31111, arcemu.GOSSIP_EVENT_HELLO, mohawk_onHello )
arcemu.RegisterUnitGossipEvent( 31111, arcemu.GOSSIP_EVENT_SELECT, mohawk_onSelectOption )
arcemu.RegisterUnitEvent( 31111, arcemu.CREATURE_EVENT_ON_ENTER_VEHICLE, mohawk_onEnterVehicle )
arcemu.RegisterUnitEvent( 31111, arcemu.CREATURE_EVENT_ON_EXIT_VEHICLE, mohawk_onExitVehicle )
arcemu.RegisterUnitEvent( 31111, arcemu.CREATURE_EVENT_ON_AIUPDATE, mohawk_onAIUpdate )
arcemu.RegisterUnitEvent( 31111, arcemu.CREATURE_EVENT_ON_HEALED, mohawk_onHealed )
arcemu.RegisterUnitEvent( 31111, arcemu.CREATURE_EVENT_ON_APPLY_AURA, mohawk_onApplyAura )
arcemu.RegisterUnitEvent( 31111, arcemu.CREATURE_EVENT_ON_LOAD, mohawk_onLoad )
