import arcemu
from arcemu import Unit

def npc_onDamageTaken( unit, event, attacker, amount ):
	unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "I am damaged for " + str( amount ) )
	
def npc_onHit( unit, event, target, amount ):
	unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "I have hit " + target.getName() + " for " + str( amount ) )
	
def npc_onDied( unit, event, killer ):
	unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, killer.getName() + " has killed me." )
	
def npc_onLoad( unit, event ):
	print( "Loaded creature " + unit.getName() )
	
	
arcemu.RegisterUnitEvent( 113, arcemu.CREATURE_EVENT_ON_HIT, npc_onHit )
arcemu.RegisterUnitEvent( 113, arcemu.CREATURE_EVENT_ON_DAMAGE_TAKEN, npc_onDamageTaken )
arcemu.RegisterUnitEvent( 113, arcemu.CREATURE_EVENT_ON_DIED, npc_onDied )
arcemu.RegisterUnitEvent( 113, arcemu.CREATURE_EVENT_ON_LOAD, npc_onLoad )
