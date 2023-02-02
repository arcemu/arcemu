import arcemu
from arcemu import Unit

def npc_onCombatStart( unit, event, target ):
	unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_YELL, arcemu.LANG_UNIVERSAL, "I am going to kill you " + target.getName() + "!" )
	
def npc_onCombatStop( unit, event, target ):
	unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Okay, Okay, I yield." )

def npc_onDamageTaken( unit, event, attacker, amount ):
	unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "I am damaged for " + str( amount ) )
	
def npc_onHit( unit, event, target, amount ):
	unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "I have hit " + target.getName() + " for " + str( amount ) )
	
def npc_onDied( unit, event, killer ):
	unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, killer.getName() + " has killed me." )
	
def npc_onTargetDied( unit, event, target ):
	unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "Hahahaha. " + target.getName() + " you are not prepared!" )
	
def npc_onLoad( unit, event ):
	print( "Loaded creature " + unit.getName() )
	

arcemu.RegisterUnitEvent( 113, arcemu.CREATURE_EVENT_ON_ENTER_COMBAT, npc_onCombatStart )
arcemu.RegisterUnitEvent( 113, arcemu.CREATURE_EVENT_ON_LEAVE_COMBAT, npc_onCombatStop )
arcemu.RegisterUnitEvent( 113, arcemu.CREATURE_EVENT_ON_HIT, npc_onHit )
arcemu.RegisterUnitEvent( 113, arcemu.CREATURE_EVENT_ON_DAMAGE_TAKEN, npc_onDamageTaken )
arcemu.RegisterUnitEvent( 113, arcemu.CREATURE_EVENT_ON_DIED, npc_onDied )
arcemu.RegisterUnitEvent( 113, arcemu.CREATURE_EVENT_ON_TARGET_DIED, npc_onTargetDied )
arcemu.RegisterUnitEvent( 113, arcemu.CREATURE_EVENT_ON_LOAD, npc_onLoad )
