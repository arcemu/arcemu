import arcemu
from arcemu import Player
from arcemu import Unit

def horsey_onFirstPassengerEntered( vehicle, passenger ):
	vehicle.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Finally someone! Hi " + passenger.getName() )
	
def horsey_onVehicleFull( vehicle ):
	vehicle.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Now I feel full!" )
	
def horsey_onLastPassengerLeft( vehicle, passenger ):
	vehicle.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Bye " + passenger.getName() + "!" )
	vehicle.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "So ronery..." )

arcemu.RegisterUnitEvent( 28605, arcemu.CREATURE_EVENT_ON_FIRST_PASSENGER_ENTERED, horsey_onFirstPassengerEntered )
arcemu.RegisterUnitEvent( 28605, arcemu.CREATURE_EVENT_ON_VEHICLE_FULL, horsey_onVehicleFull )
arcemu.RegisterUnitEvent( 28605, arcemu.CREATURE_EVENT_ON_LAST_PASSENGER_LEFT, horsey_onLastPassengerLeft )
