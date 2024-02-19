'''
Test Python CreatureScript for Arcemu
'''

import arcemu
from arcemu import CreatureScript

NPC_ID_RABBIT = 721
SPELL_ID_FORTITUDE = 1243

class RabbitCreatureScript( CreatureScript ):
	def __init__( self, unit ):
		CreatureScript.__init__( self, unit )
	
	def OnLoad( self ):
		print( self.creature.getName() + " spawning..." )
		
	def OnDied( self, killer ):
		print( killer.getName() + " has killed " + self.creature.getName() )
		self.creature.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, "You are mean! :(" )
		
	def OnEmote( self, player, emote ):
		self.creature.castSpell( SPELL_ID_FORTITUDE, False )
		player.castSpell( SPELL_ID_FORTITUDE, False )
		
	@staticmethod
	def create( unit ):
		return RabbitCreatureScript( unit )
		
arcemu.RegisterCreatureScript( NPC_ID_RABBIT, RabbitCreatureScript.create )
